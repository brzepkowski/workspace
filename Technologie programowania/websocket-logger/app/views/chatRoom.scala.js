@(username: String)

$(function() {
    var WS = window['MozWebSocket'] ? MozWebSocket : WebSocket
 var chatSocket = new WS("@routes.Application.chat(username).webSocketURL(request)")

    var sendMessage = function() {
        chatSocket.send(JSON.stringify(
            {nr: $("#nr").val()}
        ))
        $("#nr").val('')

    }

    var receiveEvent = function(event) {
        var data = JSON.parse(event.data)

        // Handle errors
        if(data.error) {
            chatSocket.close()
            $("#onError span").text(data.error)
            $("#onError").show()
            return
        } else {
            $("#onChat").show()
        }
        
        // Create the message element       
	        var el = $('<div class="message"><p style="font-size:16px"></p></div>')
	        $("p", el).text(data.message)
	        $(el).addClass('me')
	        $('#messages').append(el) 
    }

    var handleReturnKey = function(e) {
        if(e.charCode == 13 || e.keyCode == 13) {
            e.preventDefault()
            sendMessage()
        }
    }

    $("#nr").keypress(handleReturnKey)


    chatSocket.onmessage = receiveEvent

})
