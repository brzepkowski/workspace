
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
object index extends BaseScalaTemplate[play.api.templates.HtmlFormat.Appendable,Format[play.api.templates.HtmlFormat.Appendable]](play.api.templates.HtmlFormat) with play.api.templates.Template0[play.api.templates.HtmlFormat.Appendable] {

    /**/
    def apply():play.api.templates.HtmlFormat.Appendable = {
        _display_ {

Seq[Any](_display_(Seq[Any](/*2.2*/main(null)/*2.12*/ {_display_(Seq[Any](format.raw/*2.14*/("""
    
    """),_display_(Seq[Any](/*4.6*/if(flash.containsKey("error"))/*4.36*/ {_display_(Seq[Any](format.raw/*4.38*/("""
        
        <div class="alert-message error">
            <p>
                <strong>Oops!</strong> """),_display_(Seq[Any](/*8.41*/flash/*8.46*/.get("error"))),format.raw/*8.59*/("""
            </p>
        </div>
        
    """)))})),format.raw/*12.6*/("""
        
    <div class="alert-message block-message info">
        <p>
            <strong>This is the Play Websocket sample application!</strong> 
            To start, choose a username and sign in using the top right form.
        </p>
    </div>
    
""")))})))}
    }
    
    def render(): play.api.templates.HtmlFormat.Appendable = apply()
    
    def f:(() => play.api.templates.HtmlFormat.Appendable) = () => apply()
    
    def ref: this.type = this

}
                /*
                    -- GENERATED --
                    DATE: Sun Jan 18 13:36:01 CET 2015
                    SOURCE: /home/bartas/Instalations/play-2.2.6/samples/java/websocket-chat/app/views/index.scala.html
                    HASH: 71adbb369fbea9e03673fa18da313ef0b854cadd
                    MATRIX: 864->2|882->12|921->14|966->25|1004->55|1043->57|1186->165|1199->170|1233->183|1311->230
                    LINES: 29->2|29->2|29->2|31->4|31->4|31->4|35->8|35->8|35->8|39->12
                    -- GENERATED --
                */
            