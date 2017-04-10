package models;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import models.msgs.*;
import models.msgs.Number;
import play.Logger;
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

import static java.util.concurrent.TimeUnit.*;

public class Human extends UntypedActor
{
    public final String               name;
    public final ActorRef             table;

    protected WebSocket.In<JsonNode>  in;
    protected WebSocket.Out<JsonNode> out;


    public Human(String _name, WebSocket.In<JsonNode> _in,
            WebSocket.Out<JsonNode> _out, ActorRef _table)
    {
        name = _name;        
        table = _table;
        in = _in;
        out = _out;
        

        in.onMessage(new Callback<JsonNode>()
        {
            @Override
            public void invoke(JsonNode event)
            {
                try
                {
                	int nr = event.get("nr").asInt();                
                	getSelf().tell(new Number(nr,name), getSelf() );                	
                }
                catch (Exception e)
                {
                    Logger.error("invokeError");
                }
                
            }
        });

        in.onClose(new Callback0()
        {
            @Override
            public void invoke()
            {
                table.tell(new Quit(name), getSelf() );
            }
        });
    }

    @Override
    public void preStart()
    {        
        String text = "my Actor is now running!";
    	table.tell(new Info(text, name), getSelf()); 
    }

    @Override
    public void postStop()
    {
        String text = "I've been killed";
    	table.tell(new Info(text, name), getSelf()); 
    }

    @Override
    public void onReceive(Object msg) throws Exception
    {
 
            if (msg instanceof Number)
            {   
            	int nr = ((Number) msg).getNumber();            	           			
            	String text = "I've got the number " + nr + " from WebSocket and send it to Table";
            	table.tell(new Info(text, name), getSelf()); 
            	table.tell(new Number(nr, name), getSelf());
            }  
            else if (msg instanceof Card)
            {
            	Card card = (Card) msg;    
            	String text = "I've got " + card.getRank() + " " + card.getColor() + " from Table" ;
            	table.tell(new Info(text, name), getSelf());  
            }            
            
            else if(msg instanceof Info)
            {        
            	Info info = (Info) msg;             	
	            ObjectNode event = Json.newObject();
	            event.put("message", "[ "+ info.getName()+ " ] : " + info.getText()); 
	            
	            out.write(event);
            }
            else {
                unhandled(msg);
            }
            
        }  
            
}

