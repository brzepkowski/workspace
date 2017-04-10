
public class Buyer_Thread extends Thread {
	private Thread thread;
	private int Buyer_Delay;
	
	int Product = 0;

	Buyer_Thread(int Buyer_Delay) {
		this.Buyer_Delay = Buyer_Delay;
	}
	
	void Buy_Product() {
		if (Company.Magazine_Elements > 0) {
			if (Company.Magazine_Pointer != 0) {
				Company.Magazine_Pointer--;
			}
			Product = Company.Magazine[Company.Magazine_Pointer];
			Company.Magazine[Company.Magazine_Pointer] = 0;
			Company.Magazine_Elements--;
			Company.Magazine_Spaces++;
			if (Company.Mode.equals("Talkative")) {
				System.out.println("Buyer bought Product: " + Product);
			}
		}
	}
	
	public void run() {
		while (true) {
				synchronized(Company.Magazine) {
					Buy_Product();
				}
				try {
					Thread.sleep(Buyer_Delay);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				/*System.out.println("B_Elements = " + Company.Board_Elements
						+ "B_Spaces = " + Company.Board_Spaces 
						+ "M_Elements = " + Company.Magazine_Elements
						+ "M_Spaces = " + Company.Magazine_Spaces);*/
			}
	}
	
	public void start() {
		thread = new Thread(this, "Buyer");
		thread.start();
		System.out.println("Buyer started");
	}

}
