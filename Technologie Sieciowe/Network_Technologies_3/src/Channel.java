
public class Channel {
	
	
	public String data;
	public boolean busy;
	
	Channel() {
		busy = false;
		data = "";
	}
	
	public boolean isAnyoneTransmitting() {
		return busy;
	}	
}
