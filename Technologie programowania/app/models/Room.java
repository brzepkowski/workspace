package models;

import play.mvc.*;
import play.libs.*;
import play.libs.F.*;
import scala.concurrent.Await;
import scala.concurrent.duration.Duration;
import akka.actor.*;
import static akka.pattern.Patterns.ask;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.ObjectNode;
import com.fasterxml.jackson.databind.node.ArrayNode;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.*;

import models.server.Card;
import models.server.Data;
import models.server.TableServer;
import static java.util.concurrent.TimeUnit.*;

public class Room extends UntypedActor {
	// Default room.
	static ActorRef defaultRoom = Akka.system().actorOf(Props.create(Room.class));
	static ArrayList<Socket> sockets = new ArrayList<Socket>();
	static ArrayList<PrintWriter> outs = new ArrayList<PrintWriter>();
	static ArrayList<BufferedReader> ins = new ArrayList<BufferedReader>();
	static int acctualId = 0;
	static Data data;
	
	static {
		new Thread() {
			@Override
			public void run() {
										//port, players, startCash, bigBlind
				TableServer table = new TableServer(44, 3, 2000, 400);
				data = table.data;
				TableServer.main(table, 3, 2000);
			}
		}.start();
//
	}
	
	public static void join(final String username, WebSocket.In<JsonNode> in, WebSocket.Out<JsonNode> out) throws Exception {
		try {
			Socket socket = new Socket("localhost", 44);
			outs.add(new PrintWriter(socket.getOutputStream(), true));
			ins.add(new BufferedReader(new InputStreamReader(socket.getInputStream())));
			sockets.add(socket);
		} catch (Exception ex) {}
//		
		// Send the Join message to the room
		String result = (String)Await.result(ask(defaultRoom,new Join(username, out), 1000), Duration.create(1, SECONDS));
		
		if("OK".equals(result)) {
			// For each event received on the socket,
			in.onMessage(new Callback<JsonNode>() {
				public void invoke(JsonNode event) {
					// Send a Talk message to the room.
					//defaultRoom.tell(new Talk(username, event.get("text").asText()), null);
					int id = Integer.parseInt(username);
					outs.get(id).println(event.get("text").asText());
//
				}
			});
			// When the socket is closed.
			in.onClose(new Callback0() {
				public void invoke() {
					// Send a Quit message to the room.
					defaultRoom.tell(new Quit(username), null);
				}
			});
		} else {
			// Cannot connect, create a Json error.
			ObjectNode error = Json.newObject();
			error.put("error", result);
			
			// Send the error to the socket.
			out.write(error);
		}
	}
	
	// Members of this room.
	Map<String, WebSocket.Out<JsonNode>> members = new HashMap<String, WebSocket.Out<JsonNode>>();
	int id;
	String name;
//
	public void onReceive(Object message) throws Exception {
		if(message instanceof Join) {
			Join join = (Join)message;
			// Check if this username is free.
			if(members.containsKey(join.username)) {
				getSender().tell("This username is already used", getSelf());
			} else {
				members.put(join.username, join.channel);
				id = acctualId++;
				name = join.username;
				notifyAll("join", join.username, "has entered the room");
				
				Receiver rec = new Receiver(id);
				rec.start();
				getSender().tell("OK", getSelf());
//
			}
		} else if(message instanceof Talk)  {
			// Received a Talk message
			//Talk talk = (Talk)message;
			//notifyAll("talk", talk.username, talk.text);
		} else if(message instanceof Quit)  {
			Quit quit = (Quit)message;
			members.remove(quit.username);
			notifyAll("quit", quit.username, "has left the room");
			
			outs.get(id).println("X");
			outs.get(id).close();
			ins.get(id).close();
			sockets.get(id).close();
//
		} else {
			unhandled(message);
		}
	}	
	
	public void notifyMe(String kind, String user, String text) {
		
		WebSocket.Out<JsonNode> channel = members.get(user);
		ObjectNode event = Json.newObject();
		event.put("kind", kind);
		event.put("user", user);
		event.put("message", text);
		
		int pl = data.players();
		int tmp = Integer.parseInt(user);
		
		ArrayNode m = event.putArray("players");
		for(String x: members.keySet()) {
			if(!x.equals(tmp+"")) {
				m.add(x);
			}
		}

		ArrayNode b = event.putArray("bets");
		for (int i=0; i < pl; i++) {
			if(i != tmp) {
				b.add(data.betOf(i));
			}
		}
		
		ArrayNode c = event.putArray("cashes");
		for (int i=0; i < pl; i++){
			if(i != tmp){
				c.add(data.getCash(i));
			}
		}
		
		ArrayNode s = event.putArray("states");
		for (int i=0; i < pl; i++) {
			if(i != tmp) {
				s.add(data.status(i));
			}
		}
		
		ArrayNode r = event.putArray("roles");
		String[] ar = {"DB", "SB", "BB", "1", "2", "3"};
		for (int i=0; i < pl; i++) {
			if(i != tmp) {
				r.add(ar[data.role(i)]);
			}
		}
		
		ArrayNode o = event.putArray("other"); {
			o.add("pot: " + data.getPot(tmp));
			o.add("myBet: " + data.betOf(tmp));
			o.add("myCash: " + data.getCash(tmp));
			o.add("myRole: " + ar[data.role(tmp)]);
		}
		
		ArrayNode k = event.putArray("cards");
		if (data.getCards(tmp) != null) {
			for (Card x : data.getCards(tmp)) {
				k.add(x.toString());
			}
		}
		
		channel.write(event);
	}
	
	// Send a Json event to all members
	public void notifyAll(String kind, String user, String text) {
		for(WebSocket.Out<JsonNode> channel: members.values()) {
			ObjectNode event = Json.newObject();
			event.put("kind", kind);
			event.put("user", user);
			event.put("message", text);
			
			ArrayNode m = event.putArray("members");
			for(String u: members.keySet()) {
				m.add(u);
			}
			
			channel.write(event);
		}
	}
	
	
	public class Receiver extends Thread {
		int myId;
		String myName;
		
		public Receiver(int id) {
			myId = id;
			myName = Integer.toString(id);
		}
		
		@Override
		public void run() {
			while (true) {
				String line = "";
				try {
					line = ins.get(myId).readLine();
				} catch (IOException e) {continue;}
				notifyMe("talk", myName, line);
			}
		}
	}
//
	// -- Messages
	public static class Join {
		final String username;
		final WebSocket.Out<JsonNode> channel;
		
		public Join(String username, WebSocket.Out<JsonNode> channel) {
			this.username = username;
			this.channel = channel;
		}
	}
	
	public static class Talk {
		final String username;
		final String text;
		
		public Talk(String username, String text) {
			this.username = username;
			this.text = text;
		}
	}
	
	public static class Quit {
		final String username;
		
		public Quit(String username) {
			this.username = username;
		}
	}
}
