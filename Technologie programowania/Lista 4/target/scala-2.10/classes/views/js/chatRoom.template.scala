
package views.js

import play.templates._
import play.templates.TemplateMagic._

import play.api.templates._
import play.api.templates.PlayMagic._
import models._
import controllers._
import java.lang._
import java.util._
import scala.collection.JavaConversions._
import scala.collection.JavaConverters._
import play.api.i18n._
import play.core.j.PlayMagicForJava._
import play.mvc._
import play.data._
import play.api.data.Field
import play.mvc.Http.Context.Implicit._
import views.js._
/**/
object chatRoom extends BaseScalaTemplate[play.api.templates.JavaScriptFormat.Appendable,Format[play.api.templates.JavaScriptFormat.Appendable]](play.api.templates.JavaScriptFormat) with play.api.templates.Template1[String,play.api.templates.JavaScriptFormat.Appendable] {

    /**/
    def apply/*1.2*/(username: String):play.api.templates.JavaScriptFormat.Appendable = {
        _display_ {

Seq[Any](format.raw/*1.20*/("""

$(function() """),format.raw/*3.14*/("""{"""),format.raw/*3.15*/("""
    var WS = window['MozWebSocket'] ? MozWebSocket : WebSocket
 var chatSocket = new WS(""""),_display_(Seq[Any](/*5.28*/routes/*5.34*/.Application.chat(username).webSocketURL(request))),format.raw/*5.83*/("""")

    var sendMessage = function() """),format.raw/*7.34*/("""{"""),format.raw/*7.35*/("""
        chatSocket.send(JSON.stringify(
            """),format.raw/*9.13*/("""{"""),format.raw/*9.14*/("""text: $("#talk").val()"""),format.raw/*9.36*/("""}"""),format.raw/*9.37*/("""
        ))
        $("#talk").val('')
    """),format.raw/*12.5*/("""}"""),format.raw/*12.6*/("""

    var receiveEvent = function(event) """),format.raw/*14.40*/("""{"""),format.raw/*14.41*/("""
        var data = JSON.parse(event.data)

        // Handle errors
        if(data.error) """),format.raw/*18.24*/("""{"""),format.raw/*18.25*/("""
            chatSocket.close()
            $("#onError span").text(data.error)
            $("#onError").show()
            return
        """),format.raw/*23.9*/("""}"""),format.raw/*23.10*/(""" else """),format.raw/*23.16*/("""{"""),format.raw/*23.17*/("""
            $("#onChat").show()
        """),format.raw/*25.9*/("""}"""),format.raw/*25.10*/("""

        // Create the message element
        var el = $('<div class="message"><span></span><p></p></div>')
        $("span", el).text(data.user)
        $("p", el).text(data.message)
        $(el).addClass(data.kind)
        if(data.user == '"""),_display_(Seq[Any](/*32.27*/username)),format.raw/*32.35*/("""') $(el).addClass('me')
        $('#messages').append(el)

        // Update the members list
        $("#members").html('')
        $(data.members).each(function() """),format.raw/*37.41*/("""{"""),format.raw/*37.42*/("""
            var li = document.createElement('li');
            li.textContent = this;
            $("#members").append(li);
        """),format.raw/*41.9*/("""}"""),format.raw/*41.10*/(""")
    """),format.raw/*42.5*/("""}"""),format.raw/*42.6*/("""

    var handleReturnKey = function(e) """),format.raw/*44.39*/("""{"""),format.raw/*44.40*/("""
        if(e.charCode == 13 || e.keyCode == 13) """),format.raw/*45.49*/("""{"""),format.raw/*45.50*/("""
            e.preventDefault()
            sendMessage()
        """),format.raw/*48.9*/("""}"""),format.raw/*48.10*/("""
    """),format.raw/*49.5*/("""}"""),format.raw/*49.6*/("""

    $("#talk").keypress(handleReturnKey)

    chatSocket.onmessage = receiveEvent

"""),format.raw/*55.1*/("""}"""),format.raw/*55.2*/(""")
"""))}
    }
    
    def render(username:String): play.api.templates.JavaScriptFormat.Appendable = apply(username)
    
    def f:((String) => play.api.templates.JavaScriptFormat.Appendable) = (username) => apply(username)
    
    def ref: this.type = this

}
                /*
                    -- GENERATED --
                    DATE: Sun Jan 18 13:36:01 CET 2015
                    SOURCE: /home/bartas/Instalations/play-2.2.6/samples/java/websocket-chat/app/views/chatRoom.scala.js
                    HASH: 8b205febb8d99f027aa02b9518a1ad9a3d58fcb0
                    MATRIX: 797->1|915->19|957->34|985->35|1111->126|1125->132|1195->181|1259->218|1287->219|1367->272|1395->273|1444->295|1472->296|1542->339|1570->340|1639->381|1668->382|1788->474|1817->475|1984->615|2013->616|2047->622|2076->623|2144->664|2173->665|2455->911|2485->919|2678->1084|2707->1085|2867->1218|2896->1219|2929->1225|2957->1226|3025->1266|3054->1267|3131->1316|3160->1317|3253->1383|3282->1384|3314->1389|3342->1390|3454->1475|3482->1476
                    LINES: 26->1|29->1|31->3|31->3|33->5|33->5|33->5|35->7|35->7|37->9|37->9|37->9|37->9|40->12|40->12|42->14|42->14|46->18|46->18|51->23|51->23|51->23|51->23|53->25|53->25|60->32|60->32|65->37|65->37|69->41|69->41|70->42|70->42|72->44|72->44|73->45|73->45|76->48|76->48|77->49|77->49|83->55|83->55
                    -- GENERATED --
                */
            