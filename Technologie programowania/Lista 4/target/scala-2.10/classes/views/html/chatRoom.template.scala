
package views.html

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
import views.html._
/**/
object chatRoom extends BaseScalaTemplate[play.api.templates.HtmlFormat.Appendable,Format[play.api.templates.HtmlFormat.Appendable]](play.api.templates.HtmlFormat) with play.api.templates.Template1[String,play.api.templates.HtmlFormat.Appendable] {

    /**/
    def apply/*1.2*/(username: String):play.api.templates.HtmlFormat.Appendable = {
        _display_ {

Seq[Any](format.raw/*1.20*/("""

"""),_display_(Seq[Any](/*3.2*/main(username)/*3.16*/ {_display_(Seq[Any](format.raw/*3.18*/("""
    
    <div class="page-header">
        <h1>Welcome to the chat room <small>You are chatting as """),_display_(Seq[Any](/*6.66*/username)),format.raw/*6.74*/("""</small></h1>
    </div>
    
    <div id="onError" class="alert-message error">
        <p>
            <strong>Oops!</strong> <span></span>
        </p>
    </div>
    
    <div id="onChat" class="row">
        <div class="span10" id="main">
            <div id="messages">
            </div>
            <textarea id="talk"></textarea>
        </div>
        <div class="span4">
            <h2>Members</h2>
            <ul id="members">
            </ul>
        </div>
    </div>
    
    <script type="text/javascript" charset="utf-8" src=""""),_display_(Seq[Any](/*28.58*/routes/*28.64*/.Application.chatRoomJs(username))),format.raw/*28.97*/(""""></script>
    
""")))})))}
    }
    
    def render(username:String): play.api.templates.HtmlFormat.Appendable = apply(username)
    
    def f:((String) => play.api.templates.HtmlFormat.Appendable) = (username) => apply(username)
    
    def ref: this.type = this

}
                /*
                    -- GENERATED --
                    DATE: Sun Jan 18 13:36:02 CET 2015
                    SOURCE: /home/bartas/Instalations/play-2.2.6/samples/java/websocket-chat/app/views/chatRoom.scala.html
                    HASH: 7336c75cfe5a26f4da97fe64c82f163674b01670
                    MATRIX: 777->1|889->19|926->22|948->36|987->38|1123->139|1152->147|1735->694|1750->700|1805->733
                    LINES: 26->1|29->1|31->3|31->3|31->3|34->6|34->6|56->28|56->28|56->28
                    -- GENERATED --
                */
            