
public class Main {

	public static int[] cable = new int[21];
	public static boolean cableFree = true;
	public static int[] locationOfNodes;
	public static int numberOfNodes = 3;
	public static Node[] nodes;
	
	public static boolean jam = false;
	public static int nodesNotifiedAboutJam = 0;
	
	public static void main(String[] args) {
		
		System.out.println("Transmission started");
		
		locationOfNodes = new int[numberOfNodes];
		nodes = new Node[numberOfNodes];
		
		nodes[0] = new Node(0, 0);
		locationOfNodes[0] = 0;
		
		nodes[1] = new Node(1, 10);
		locationOfNodes[1] = 10;
		
		nodes[2] = new Node(2, 20);
		locationOfNodes[2] = 20;
		
		
		nodes[0].start();
		nodes[1].start();
		nodes[2].start();
		
	}
}
