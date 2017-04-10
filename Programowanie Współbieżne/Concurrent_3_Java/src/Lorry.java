import java.util.Random;

import Definitions.*;

public class Lorry extends Thread {
	private Thread thread;
	public int identifier;
	private Company company;
	private Task task;
	private char operator;
	private Shop shop;
	
	public Lorry(int n) {
		this.identifier = n;
	}
	
	public void run() {
	
		while(!doesAnyCompanycontaintProdyct(operator));
				try {
					if (operator == '+') {
						for (int i = 0; i < Definitions.numberOfCompanies; i++) {
							if (Main.companies[i].addingQueue.isEmpty()) {
								System.out.println("~~~~~AddingQueue in company: " + i + " isEmpty");
							}
							else {
								task = Main.companies[i].addingQueue.take();
								if (task == null) run();
								company = Main.companies[i];
								System.out.println("Lorry: " + identifier + " took task from company: " + i);
								break;
							}
						}
					}
					else if (operator == '-') {
						for (int i = 0; i < Definitions.numberOfCompanies; i++) {
							if (Main.companies[i].subtractingQueue.isEmpty()) {
								System.out.println("~~~~~SubtractingQueue in company: " + i + " isEmpty");
							}
							else {
								task = Main.companies[i].subtractingQueue.take();
								if (task == null) run();
								company = Main.companies[i];
								System.out.println("Lorry: " + identifier + " took task from company: " + i);
								break;
							}
						}
					}
					else if (operator == '*') {
						for (int i = 0; i < Definitions.numberOfCompanies; i++) {
							if (Main.companies[i].multiplyingQueue.isEmpty()) {
								System.out.println("~~~~~MultiplyingQueue in company: " + i + " isEmpty");
							}
							else {
								task = Main.companies[i].multiplyingQueue.take();
								if (task == null) run();
								company = Main.companies[i];
								System.out.println("Lorry: " + identifier + " took task from company: " + i);
								break;
							}
						}
					}
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			
				try {
					
					int delay = 0 + (int)(Math.random()*5000);
					System.out.println("... Lorry: " + identifier + " durring time = " + delay);
					thread.sleep(delay);
					if (task == null) run();
					else storeProductInShop();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		
				try {
					Main.freeLorries.put(this);
					System.out.println("Lorry :" + identifier + " returned to freeLorries");
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
	}
	
	public boolean doesAnyCompanycontaintProdyct(char operator) {
		if (operator == '+') {
			for (Company company: Main.companies) {
				if (!company.addingQueue.isEmpty()) {
					System.out.println("%%%%%%  I found company, that has addingProduct");
					return true;
				}
				
			}
		}
		else if (operator == '-') {
			for (Company company: Main.companies) {
				if (!company.subtractingQueue.isEmpty()) {
					System.out.println("%%%%%%  I found company, that has subtractingProduct");
					return true;
				}
				
			}
		}
		else if (operator == '*') {
			for (Company company: Main.companies) {
				if (!company.multiplyingQueue.isEmpty()) {
					System.out.println("%%%%%%  I found company, that has multiplyingProduct");
					return true;
				}
			}
		}
		
		return false;
	}
	
	public void storeProductInShop() throws InterruptedException {
		if (operator == '+') {
			shop.addingMagazine.put(task);
		}
		else if (operator == '-') {
			shop.subtractingMagazine.put(task);
		}
		else if (operator == '*') {
			shop.multiplyingMagazine.put(task);
		}
		System.out.println("Lorry: " + identifier + " stored product in shop: " + shop.identifier);
	}
	
	public void start(char operator, Shop shop) throws InterruptedException {
		task = null;
		company = null;
		this.operator = operator;
		this.shop = shop;
		
		thread = new Thread(this, "Lorry");
		thread.start();
		System.out.println("Lorry: " + identifier + " started with operator: " + operator);
		
	}

}
