import java.util.Random;


public class Node extends Thread {

	private Thread thread;
	private int identifier;

	Node(int identifier) {
		this.identifier = identifier;
		
	}
	
	
	public void run() {
		Random rand = new Random();
		while (true) {
			if (CSMA_CD_transmission.channel.isAnyoneTransmitting() == false) {
				//synchronized(CSMA_CD_transmission.channel) {
					CSMA_CD_transmission.channel.data = "";
					//Header
					CSMA_CD_transmission.channel.busy = true;
					CSMA_CD_transmission.channel.data += "HEADER_" + identifier;
					CSMA_CD_transmission.channel.busy = false;
				//}
					try {
						this.sleep(500);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//Data
					//synchronized(CSMA_CD_transmission.channel) {
					CSMA_CD_transmission.channel.busy = true;
					CSMA_CD_transmission.channel.data += "-data-";
					CSMA_CD_transmission.channel.busy = false;
					//}
					try {
						this.sleep(500);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					//Ending
					//synchronized(CSMA_CD_transmission.channel) {
					CSMA_CD_transmission.channel.busy = true;
					CSMA_CD_transmission.channel.data += "FOOTER_" + identifier;
					CSMA_CD_transmission.channel.busy = false;
					checkCorrectness();
					//System.out.println("Węzeł " + identifier + " napisał | Channel.data = " + CSMA_CD_transmission.channel.data);
					//}
				try {
					int randomNum = rand.nextInt((10 - 1) + 1) + 1;
					this.sleep(1000 * randomNum);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			else {
				System.out.println("Węzeł " + identifier + " czeka");
				try {
					int randomNum = rand.nextInt((10 - 1) + 1) + 1;
					this.sleep(1000 * randomNum);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	
	public void start() {
		thread = new Thread(this, "Node");
		thread.start();
		System.out.println("Node " + identifier + " started");
	}
	
	public void checkCorrectness() {
		String[] split1 = null, split2 = null;
		synchronized(CSMA_CD_transmission.channel) {
		 String[] splitArray = CSMA_CD_transmission.channel.data.split("-");
		 if (splitArray.length > 3) {
			 System.out.println("|JAM " + identifier + "| Data = " + CSMA_CD_transmission.channel.data);
		 }
		 else {
			 try {
			 if (splitArray[1].equals("data")) {
				 
					 split1 = splitArray[0].split("_");
					 split2 = splitArray[2].split("_");
				
			
			if (split1[0].equals("HEADER") && split2[0].equals("FOOTER")) {
				if (split1[1].equals(split2[1])) {
					System.out.println("|SUCCESS " + identifier + "| Data = " + CSMA_CD_transmission.channel.data);
				}
				else {
					System.out.println("|JAM " + identifier + "| Data = " + CSMA_CD_transmission.channel.data);
				}
			}
			else {
				System.out.println("|JAM " + identifier + "| Data = " + CSMA_CD_transmission.channel.data);
			}
		 }
			 }
			 catch (java.lang.ArrayIndexOutOfBoundsException e) {
				 System.out.println("!|JAM " + identifier + "| Data = " + CSMA_CD_transmission.channel.data);
				 return;
			 }
		 }
			 
	}
		
	}
}

