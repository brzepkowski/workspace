
public class Manager extends Thread {
	private Thread thread;
	private Shop shop;
	private Lorry lorry;
	
	public Manager(Shop shop) {
		this.shop = shop;
	}
	
	public void run() {
		while(true) {
			try {
				if (shop.addingMagazine.isEmpty()) {
					lorry = Main.freeLorries.take();
					System.out.println("////////---->Manager sent lorry: " + lorry.identifier + " to company");
					lorry.start('+', shop);
				}
				else if (shop.subtractingMagazine.isEmpty()) {
					lorry = Main.freeLorries.take();
					System.out.println("////////---->Manager sent lorry: " + lorry.identifier + " to company");
					lorry.start('-', shop);
				}
				else if (shop.multiplyingMagazine.isEmpty()) {
					lorry = Main.freeLorries.take();
					System.out.println("////////---->Manager sent lorry: " + lorry.identifier + " to company");
					lorry.start('*', shop);	
				}
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public void start() {
		thread = new Thread(this, "Manager");
		thread.start();
		System.out.println("Manager started");
	}

}
