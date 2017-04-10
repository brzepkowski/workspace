import Definitions.*;

public class Shop_Client extends Thread {
	private Thread thread;
	private Shop shop;
	private Task task;
	private int delay;
	
	private Task product;
	String operators = "-+*";
	char operator;
	
	public Shop_Client(Shop shop) {
		this.shop = shop;
		delay = Definitions.shopClientDelay;
	}
	
	
	public void buyProduct() throws InterruptedException {
		operator = operators.charAt(0 + (int)(Math.random()*3));
		if (operator == '+') {
			task = shop.addingMagazine.take();
			System.out.println("Client bought product in shop: " + shop.identifier);
		}
		else if (operator == '-') {
			task = shop.subtractingMagazine.take();
			System.out.println("Client bought product in shop: " + shop.identifier);
		}
		else if (operator == '*') {
			task = shop.multiplyingMagazine.take();
			System.out.println("Client bought product in shop: " + shop.identifier);
		}
	}
	
	public void run() {
		while(true) {
			try {
				buyProduct();
				Thread.sleep(delay);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
	}
	
	public void start() {
		thread = new Thread(this, "ShopClient");
		thread.start();
		System.out.println("ShopClient started");
	}
}
