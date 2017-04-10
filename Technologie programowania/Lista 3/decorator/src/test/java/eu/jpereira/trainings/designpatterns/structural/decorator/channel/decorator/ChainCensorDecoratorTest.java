package eu.jpereira.trainings.designpatterns.structural.decorator.channel.decorator;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import eu.jpereira.trainings.designpatterns.structural.decorator.channel.SocialChannel;
import eu.jpereira.trainings.designpatterns.structural.decorator.channel.SocialChannelBuilder;
import eu.jpereira.trainings.designpatterns.structural.decorator.channel.SocialChannelProperties;
import eu.jpereira.trainings.designpatterns.structural.decorator.channel.SocialChannelPropertyKey;
import eu.jpereira.trainings.designpatterns.structural.decorator.channel.spy.TestSpySocialChannel;

public class ChainCensorDecoratorTest extends AbstractSocialChanneldDecoratorTest {

	@Test
	public void testChainTwoDecorators() {
		// Create the builder
		SocialChannelBuilder builder = createTestSpySocialChannelBuilder();

		// create a spy social channel
		SocialChannelProperties props = new SocialChannelProperties().putProperty(SocialChannelPropertyKey.NAME, TestSpySocialChannel.NAME);

		// Chain decorators
		SocialChannel channel = builder.
				with(new WordCensor("message")).
				with(new URLAppender("http://jpereira.eu")).
				getDecoratedChannel(props);

		channel.deliverMessage("this is a message");
		// Spy channel. Should get the one created before
		TestSpySocialChannel spyChannel = (TestSpySocialChannel) builder.buildChannel(props);
		assertEquals("this is a ####### http://jpereira.eu", spyChannel.lastMessagePublished());
	}

	@Test
	public void testChainTwoDecoratorsWithoutBuilder() {
		
		SocialChannel channel = new TestSpySocialChannel();
		
		SocialChannel urlAppenderChannel = new URLAppender("http://jpereira.eu", channel);
		
		//Now create a truncator
		SocialChannel wordCensorChannel = new WordCensor("message", urlAppenderChannel);
		
		wordCensorChannel.deliverMessage("this is a message");
		// Spy channel. Should get the one created before
		TestSpySocialChannel spy = (TestSpySocialChannel)channel;
		assertEquals("this is a ####### http://jpereira.eu", spy.lastMessagePublished());
	}

	@Test
	public void testOtherChainTwoDecorators() {
		// Create the builder
		SocialChannelBuilder builder = createTestSpySocialChannelBuilder();

		// create a spy social channel
		SocialChannelProperties props = new SocialChannelProperties().putProperty(SocialChannelPropertyKey.NAME, TestSpySocialChannel.NAME);

		// Chain decorators
		SocialChannel channel = builder.with(new URLAppender("http://jpereira.eu")).andWith(new WordCensor("message")).getDecoratedChannel(props);

		channel.deliverMessage("this is a message");
		// Spy channel. Should get the one created before
		TestSpySocialChannel spyChannel = (TestSpySocialChannel) builder.buildChannel(props);
		assertEquals("this is a ####### http://jpereira.eu", spyChannel.lastMessagePublished());
	}

}
