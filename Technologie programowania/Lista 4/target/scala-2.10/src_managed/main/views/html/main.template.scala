
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
object main extends BaseScalaTemplate[play.api.templates.HtmlFormat.Appendable,Format[play.api.templates.HtmlFormat.Appendable]](play.api.templates.HtmlFormat) with play.api.templates.Template2[String,Html,play.api.templates.HtmlFormat.Appendable] {

    /**/
    def apply/*1.2*/(connected: String)(content: Html):play.api.templates.HtmlFormat.Appendable = {
        _display_ {

Seq[Any](format.raw/*1.36*/("""

<!DOCTYPE html>

<html>
    <head>
        <title>Websocket Chat-Room</title>
        <link rel="stylesheet" media="screen" href=""""),_display_(Seq[Any](/*8.54*/routes/*8.60*/.Assets.at("stylesheets/bootstrap.css"))),format.raw/*8.99*/("""">
        <link rel="stylesheet" media="screen" href=""""),_display_(Seq[Any](/*9.54*/routes/*9.60*/.Assets.at("stylesheets/main.css"))),format.raw/*9.94*/("""">
        <link rel="shortcut icon" type="image/png" href=""""),_display_(Seq[Any](/*10.59*/routes/*10.65*/.Assets.at("images/favicon.png"))),format.raw/*10.97*/("""">
        <script src=""""),_display_(Seq[Any](/*11.23*/routes/*11.29*/.Assets.at("javascripts/jquery-1.7.1.min.js"))),format.raw/*11.74*/("""" type="text/javascript"></script>
    </head>
    <body>
        
        <div class="topbar">
            <div class="fill">
                <div class="container">
                    <a class="brand" href=""""),_display_(Seq[Any](/*18.45*/routes/*18.51*/.Application.index())),format.raw/*18.71*/("""">Websocket Chat-Room</a>
                    
                    """),_display_(Seq[Any](/*20.22*/if(connected != null)/*20.43*/ {_display_(Seq[Any](format.raw/*20.45*/("""
                        <p class="pull-right">
                            Logged in as """),_display_(Seq[Any](/*22.43*/connected)),format.raw/*22.52*/(""" —
                            <a href=""""),_display_(Seq[Any](/*23.39*/routes/*23.45*/.Application.index())),format.raw/*23.65*/("""">Disconnect</a>
                        </p>
                    """)))}/*25.23*/else/*25.28*/{_display_(Seq[Any](format.raw/*25.29*/("""
                        <form action=""""),_display_(Seq[Any](/*26.40*/routes/*26.46*/.Application.chatRoom())),format.raw/*26.69*/("""" class="pull-right">
                            <input id="username" name="username" class="input-small" type="text" placeholder="Username">
                            <button class="btn" type="submit">Sign in</button>
                        </form>
                    """)))})),format.raw/*30.22*/("""
                    
                </div>
            </div>
        </div>

        <div class="container">

            <div class="content">
                """),_display_(Seq[Any](/*39.18*/content)),format.raw/*39.25*/("""
            </div>

            <footer>
                <p>
                    <a href="http://www.playframework.com">www.playframework.com</a>
                </p>
            </footer>

        </div>
        
    </body>
</html>
"""))}
    }
    
    def render(connected:String,content:Html): play.api.templates.HtmlFormat.Appendable = apply(connected)(content)
    
    def f:((String) => (Html) => play.api.templates.HtmlFormat.Appendable) = (connected) => (content) => apply(connected)(content)
    
    def ref: this.type = this

}
                /*
                    -- GENERATED --
                    DATE: Sun Jan 18 13:36:02 CET 2015
                    SOURCE: /home/bartas/Instalations/play-2.2.6/samples/java/websocket-chat/app/views/main.scala.html
                    HASH: 0e854377dd1f927496ef0b797e71ee930a84884b
                    MATRIX: 778->1|906->35|1074->168|1088->174|1148->213|1239->269|1253->275|1308->309|1405->370|1420->376|1474->408|1535->433|1550->439|1617->484|1864->695|1879->701|1921->721|2025->789|2055->810|2095->812|2221->902|2252->911|2329->952|2344->958|2386->978|2472->1046|2485->1051|2524->1052|2600->1092|2615->1098|2660->1121|2967->1396|3167->1560|3196->1567
                    LINES: 26->1|29->1|36->8|36->8|36->8|37->9|37->9|37->9|38->10|38->10|38->10|39->11|39->11|39->11|46->18|46->18|46->18|48->20|48->20|48->20|50->22|50->22|51->23|51->23|51->23|53->25|53->25|53->25|54->26|54->26|54->26|58->30|67->39|67->39
                    -- GENERATED --
                */
            