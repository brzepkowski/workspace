
public class CSMA_CD_transmission {

	public static Node[] nodes;
	public static Channel channel;
	
	public static void main(String[] args) {
	
		channel = new Channel();
		
		nodes = new Node[5];
		for (int i = 0; i < nodes.length; i++) {
		    nodes[i] = new Node(i);
		    nodes[i].start();
		}
	}
}
