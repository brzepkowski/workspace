package controllers;

import play.mvc.*;

import com.fasterxml.jackson.databind.JsonNode; 
import views.html.*;

import models.*;

public class Application extends Controller {
  
	/**
	 * Display the home page.
	 */
	public static Result index() {
		return ok(index.render());
	}
  
	/**
	 * Display the chat room.
	 */
	public static Result room(String username) {
		if(username == null || username.trim().equals("")) {
			flash("error", "Please choose a valid username.");
			return redirect(routes.Application.index());
		}
		return ok(room.render(username));
	}

	public static Result roomJs(String username) {
		return ok(views.js.room.render(username));
	}
	
	/**
	 * Handle the chat websocket.
	 */
	public static WebSocket<JsonNode> chat(final String username) {
		return new WebSocket<JsonNode>() {
			
			// Called when the Websocket Handshake is done.
			public void onReady(WebSocket.In<JsonNode> in, WebSocket.Out<JsonNode> out){
				// Join the chat room.
				try { 
					Room.join(username, in, out);
				} catch (Exception ex) {
					System.out.println("DUPA Application.java 47");
					//ex.printStackTrace();
				}
			}
		};
	}
  
}
