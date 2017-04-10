package eu.jpereira.trainings.designpatterns.structural.decorator.channel.decorator;

import eu.jpereira.trainings.designpatterns.structural.decorator.channel.SocialChannel;

public class WordCensor extends SocialChannelDecorator {
	
	private String censoredWord;
	
	WordCensor (String censoredWord ) {
		this.censoredWord = censoredWord;
	}

	public WordCensor(String censoredWord, SocialChannel decoratedChannel) {
		this.censoredWord = censoredWord;
		this.delegate = decoratedChannel;
	}
	
	@Override
	public void deliverMessage(String message) {
		String censorship = "";
		String newMessage = "";
		if (message.contains(censoredWord)) {
			int i = 0;
			while (i < censoredWord.length()) {
				censorship += "#";
				i++;
			}
			//Because replaceAll doesn't change operated string, but creates new string we have to create new string
			newMessage = message.replaceAll(censoredWord, censorship);
		}
		delegate.deliverMessage(newMessage);
	}

}
