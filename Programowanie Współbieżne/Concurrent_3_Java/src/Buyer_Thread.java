
public class Buyer_Thread extends Thread {
	private Thread thread;
	private int Buyer_Delay;
	private final Company company;
	
	
	int Product = 0;

	Buyer_Thread(int Buyer_Delay, Company company) {
		this.Buyer_Delay = Buyer_Delay;
		this.company = company;
	}
	
	void Buy_Product() {
		if (company.Magazine_Elements > 0) {
			if (company.Magazine_Pointer != 0) {
				company.Magazine_Pointer--;
			}
			Product = company.Magazine[company.Magazine_Pointer].product;
			company.Magazine[company.Magazine_Pointer] = null;
			company.Magazine_Elements--;
			company.Magazine_Spaces++;
			if (company.Mode.equals("Talkative")) {
				System.out.println("Buyer bought Product: " + Product);
			}
		}
	}
	
	public void run() {
		while (true) {
				synchronized(company.Magazine) {
					Buy_Product();
				}
				try {
					Thread.sleep(Buyer_Delay);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				/*System.out.println("B_Elements = " + company.Board_Elements
						+ "B_Spaces = " + company.Board_Spaces 
						+ "M_Elements = " + company.Magazine_Elements
						+ "M_Spaces = " + company.Magazine_Spaces);*/
			}
	}
	
	public void start() {
		thread = new Thread(this, "Buyer");
		thread.start();
		System.out.println("Buyer started");
	}

}
