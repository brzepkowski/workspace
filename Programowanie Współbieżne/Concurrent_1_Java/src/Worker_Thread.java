public class Worker_Thread extends Thread {
	private Thread thread;
	private int Worker_Delay;
	
	int Product;
	
	int First_Argument, Second_Argument;
	char Operator;

	Worker_Thread(int Worker_Delay) {
		this.Worker_Delay = Worker_Delay;
	}
	
	void Get_Task() {
		if (Company.Board_Pointer != 0) {
			Company.Board_Pointer--;
		}
		First_Argument = (int) Company.Board[Company.Board_Pointer][0];
		Second_Argument = (int)Company.Board[Company.Board_Pointer][1];
		Operator = Company.Board[Company.Board_Pointer][2];
		if (Company.Mode.equals("Talkative")) {
			System.out.println("Worker got task: " + First_Argument + " | " 
					+ Second_Argument + " | " + Operator);
		}
		Company.Board[Company.Board_Pointer][0] = '-';
		Company.Board[Company.Board_Pointer][1] = '-';
		Company.Board[Company.Board_Pointer][2] = '-';
		Company.Board_Elements--;
		Company.Board_Spaces++;
	}
	
	void Produce() {
		if (Operator == '-') {
			Product = First_Argument - Second_Argument;
		}
		else if (Operator == '+') {
			Product = First_Argument + Second_Argument;
		}
		else if (Operator == '*') {
			Product = First_Argument * Second_Argument;
		}
	}
	
	void Store_Product() {
		Company.Magazine[Company.Magazine_Pointer] = Product;
		Company.Magazine_Pointer++;
		if (Company.Mode.equals("Talkative")) {
			System.out.println("Worker stored in Magazine Product: " + Product + " | Magazine_Pointer = " + Company.Magazine_Pointer);
		}
		Company.Magazine_Elements++;
		Company.Magazine_Spaces--;
	}
	
	public void run() {
		//------Zamienić while(true) na czekanie az miejsce się zwolni
		//np. wait()...
		while (true) {
			if (Company.Board_Elements > 0 && Company.Magazine_Spaces > 0) {
				synchronized(Company.Board) {
					Get_Task();
					Produce();
				}
				try {
					Thread.sleep(Worker_Delay);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				synchronized(Company.Magazine) {
					Store_Product();
				}
				/*System.out.println("B_Elements = " + Company.Board_Elements
						+ "B_Spaces = " + Company.Board_Spaces 
						+ "M_Elements = " + Company.Magazine_Elements
						+ "M_Spaces = " + Company.Magazine_Spaces);*/
			}
			
		}
	}
	
	public void start() {
		thread = new Thread(this, "Worker");
		thread.start();
		System.out.println("Worker started");
	}

}
