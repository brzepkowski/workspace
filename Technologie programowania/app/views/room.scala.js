@(username: String)

$(function() {
	var WS = window['MozWebSocket'] ? MozWebSocket : WebSocket
 var chatSocket = new WS("@routes.Application.chat(username).webSocketURL(request)")

	var sendMessage = function() {
		chatSocket.send(JSON.stringify(
			{text: $("#talk").val()}
		))
		$("#talk").val('')
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
		var el = $('<div class="message"><span></span><p></p></div>')
		$("span", el).text(data.user)
		$("p", el).text(data.message)
		$(el).addClass(data.kind)
		if(data.user == '@username') $(el).addClass('me')
		$('#messages').append(el)

		// Update the players list
		$("#players").html('')
		$(data.players).each(function() {
			var li = document.createElement('li');
			li.textContent = this;
			$("#players").append(li);
		})
		// Update the bets list
		$("#bets").html('')
		$(data.bets).each(function() {
			var li = document.createElement('li');
			li.textContent = this;
			$("#bets").append(li);
		})
		// Update the cashes list
		$("#cashes").html('')
		$(data.cashes).each(function() {
			var li = document.createElement('li');
			li.textContent = this;
			$("#cashes").append(li);
		})
		// Update the states list
		$("#states").html('')
		$(data.states).each(function() {
			var li = document.createElement('li');
			li.textContent = this;
			$("#states").append(li);
		})
		// Update the roles list
		$("#roles").html('')
		$(data.roles).each(function() {
			var li = document.createElement('li');
			li.textContent = this;
			$("#roles").append(li);
		})
		// Update the other list
		$("#other").html('')
		$(data.other).each(function() {
			var li = document.createElement('li');
			li.textContent = this;
			$("#other").append(li);
		})
		// Update the cards list
		$("#cards").html('')
		$(data.cards).each(function() {
			var li = document.createElement('li');
			li.textContent = this;
			$("#cards").append(li);
		})
	}

	var handleReturnKey = function(e) {
		if(e.charCode == 13 || e.keyCode == 13) {
			e.preventDefault()
			sendMessage()
		}
	}

	$("#talk").keypress(handleReturnKey)

	chatSocket.onmessage = receiveEvent

})
