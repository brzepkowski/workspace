package models;

import play.mvc.*;
import play.libs.*;
import scala.concurrent.Await;
import scala.concurrent.duration.Duration;
import akka.actor.*;
import static akka.pattern.Patterns.ask;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.ObjectNode;

import java.util.*;

import models.msgs.*;
import models.msgs.Number;
import static java.util.concurrent.TimeUnit.*;

public class Table extends UntypedActor {
    
    // Default table.
    static ActorRef defaultTable = Akka.system().actorOf(Props.create(Table.class));
    
    // Members of this table.
    static Map<String, ActorRef> members = new HashMap<String, ActorRef>();
       
    public static void join(final String name, WebSocket.In<JsonNode> in, WebSocket.Out<JsonNode> out ) throws Exception{

        // Send the Join message to the table
        String result = (String)Await.result(ask(defaultTable,new Join(name), 1000), Duration.create(1, SECONDS));
                
        if("OK".equals(result)) {
        	
            ActorRef player =  Akka.system().actorOf(
                    Props.create(Human.class, name, in, out,  defaultTable ));
            
            members.put(name, player);            
            String text =  "my Actor has been added to Table"; 
            notifyAll(new Info(text,name));                             
        }
    }
    
    public static void quit(final String username) throws Exception{

        //to do
    }
    
       
    public void onReceive(Object message) throws Exception {
        
        if(message instanceof Join) {
            
            Join join = (Join) message;
                        
            if(false){
            	// if username is already taken do sth        	
            }
            else {                
                getSender().tell("OK", getSelf());
            }
        } else if (message instanceof Number)
        {   
        	int nr = ((Number) message).getNumber();
        	String name = ((Number) message).getName();  
        	getSender().tell(Deck.giveCard(nr), getSelf());
        	String text = "I've sent    " +  Deck.giveCard(nr).getRank() + " " + Deck.giveCard(nr).getColor() + "    to " + name;
        	notifyAll(new Info(text,"Table"));
        }
        else if (message instanceof Info)
        {   
        	String text = ((Info) message).getText();
        	String name = ((Info) message).getName();  
        	
        	notifyAll(new Info(text, name));
        }  
        else if(message instanceof Quit)  {            
           
            String name = ((Quit)message).getName();            
            members.remove(name);
            String text = "has left the game";            		
            notifyAll(new Info(text, name));
          
        
        } else {
            unhandled(message);
        }
        
    }
    
        
    static public void notifyAll(Object msg)
    {
        for (ActorRef member : members.values())
        {
            member.tell(msg, defaultTable);
        }
    }
    
    
    
    
}

