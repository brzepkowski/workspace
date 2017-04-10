// @SOURCE:/home/bartas/Instalations/play-2.2.6/samples/java/websocket-chat/conf/routes
// @HASH:5065bd6aacbadc046864531824f9912456751c1c
// @DATE:Sun Jan 18 13:35:59 CET 2015

import Routes.{prefix => _prefix, defaultPrefix => _defaultPrefix}
import play.core._
import play.core.Router._
import play.core.j._

import play.api.mvc._
import play.libs.F

import Router.queryString


// @LINE:12
// @LINE:9
// @LINE:8
// @LINE:7
// @LINE:6
package controllers {

// @LINE:12
class ReverseAssets {
    

// @LINE:12
def at(file:String): Call = {
   Call("GET", _prefix + { _defaultPrefix } + "assets/" + implicitly[PathBindable[String]].unbind("file", file))
}
                                                
    
}
                          

// @LINE:9
// @LINE:8
// @LINE:7
// @LINE:6
class ReverseApplication {
    

// @LINE:7
def chatRoom(username:String = null): Call = {
   Call("GET", _prefix + { _defaultPrefix } + "room" + queryString(List(if(username == null) None else Some(implicitly[QueryStringBindable[String]].unbind("username", username)))))
}
                                                

// @LINE:9
def chatRoomJs(username:String): Call = {
   Call("GET", _prefix + { _defaultPrefix } + "assets/javascripts/chatroom.js" + queryString(List(Some(implicitly[QueryStringBindable[String]].unbind("username", username)))))
}
                                                

// @LINE:6
def index(): Call = {
   Call("GET", _prefix)
}
                                                

// @LINE:8
def chat(username:String): Call = {
   Call("GET", _prefix + { _defaultPrefix } + "room/chat" + queryString(List(Some(implicitly[QueryStringBindable[String]].unbind("username", username)))))
}
                                                
    
}
                          
}
                  


// @LINE:12
// @LINE:9
// @LINE:8
// @LINE:7
// @LINE:6
package controllers.javascript {

// @LINE:12
class ReverseAssets {
    

// @LINE:12
def at : JavascriptReverseRoute = JavascriptReverseRoute(
   "controllers.Assets.at",
   """
      function(file) {
      return _wA({method:"GET", url:"""" + _prefix + { _defaultPrefix } + """" + "assets/" + (""" + implicitly[PathBindable[String]].javascriptUnbind + """)("file", file)})
      }
   """
)
                        
    
}
              

// @LINE:9
// @LINE:8
// @LINE:7
// @LINE:6
class ReverseApplication {
    

// @LINE:7
def chatRoom : JavascriptReverseRoute = JavascriptReverseRoute(
   "controllers.Application.chatRoom",
   """
      function(username) {
      return _wA({method:"GET", url:"""" + _prefix + { _defaultPrefix } + """" + "room" + _qS([(username == null ? null : (""" + implicitly[QueryStringBindable[String]].javascriptUnbind + """)("username", username))])})
      }
   """
)
                        

// @LINE:9
def chatRoomJs : JavascriptReverseRoute = JavascriptReverseRoute(
   "controllers.Application.chatRoomJs",
   """
      function(username) {
      return _wA({method:"GET", url:"""" + _prefix + { _defaultPrefix } + """" + "assets/javascripts/chatroom.js" + _qS([(""" + implicitly[QueryStringBindable[String]].javascriptUnbind + """)("username", username)])})
      }
   """
)
                        

// @LINE:6
def index : JavascriptReverseRoute = JavascriptReverseRoute(
   "controllers.Application.index",
   """
      function() {
      return _wA({method:"GET", url:"""" + _prefix + """"})
      }
   """
)
                        

// @LINE:8
def chat : JavascriptReverseRoute = JavascriptReverseRoute(
   "controllers.Application.chat",
   """
      function(username) {
      return _wA({method:"GET", url:"""" + _prefix + { _defaultPrefix } + """" + "room/chat" + _qS([(""" + implicitly[QueryStringBindable[String]].javascriptUnbind + """)("username", username)])})
      }
   """
)
                        
    
}
              
}
        


// @LINE:12
// @LINE:9
// @LINE:8
// @LINE:7
// @LINE:6
package controllers.ref {


// @LINE:12
class ReverseAssets {
    

// @LINE:12
def at(path:String, file:String): play.api.mvc.HandlerRef[_] = new play.api.mvc.HandlerRef(
   controllers.Assets.at(path, file), HandlerDef(this, "controllers.Assets", "at", Seq(classOf[String], classOf[String]), "GET", """ Map static resources from the /public folder to the /assets URL path""", _prefix + """assets/$file<.+>""")
)
                      
    
}
                          

// @LINE:9
// @LINE:8
// @LINE:7
// @LINE:6
class ReverseApplication {
    

// @LINE:7
def chatRoom(username:String): play.api.mvc.HandlerRef[_] = new play.api.mvc.HandlerRef(
   controllers.Application.chatRoom(username), HandlerDef(this, "controllers.Application", "chatRoom", Seq(classOf[String]), "GET", """""", _prefix + """room""")
)
                      

// @LINE:9
def chatRoomJs(username:String): play.api.mvc.HandlerRef[_] = new play.api.mvc.HandlerRef(
   controllers.Application.chatRoomJs(username), HandlerDef(this, "controllers.Application", "chatRoomJs", Seq(classOf[String]), "GET", """""", _prefix + """assets/javascripts/chatroom.js""")
)
                      

// @LINE:6
def index(): play.api.mvc.HandlerRef[_] = new play.api.mvc.HandlerRef(
   controllers.Application.index(), HandlerDef(this, "controllers.Application", "index", Seq(), "GET", """ Home page""", _prefix + """""")
)
                      

// @LINE:8
def chat(username:String): play.api.mvc.HandlerRef[_] = new play.api.mvc.HandlerRef(
   controllers.Application.chat(username), HandlerDef(this, "controllers.Application", "chat", Seq(classOf[String]), "GET", """""", _prefix + """room/chat""")
)
                      
    
}
                          
}
        
    