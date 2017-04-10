
public class President_Thread extends Thread{
	
	private Thread thread;
	private int President_Delay;
	
	String Operators = "-+*";
	int First_Argument, Second_Argument;
	char Operator;

	President_Thread(int President_Delay) {
		this.President_Delay = President_Delay;
	}
	
	void Create_Task() {
		First_Argument = 0 + (int)(Math.random()*100); //randomNum = minimum + (int)(Math.random()*maximum); 
		Second_Argument = 0 + (int)(Math.random()*100);
		Operator = Operators.charAt(0 + (int)(Math.random()*3));
		Company.Board[Company.Board_Pointer][0] = (char) First_Argument;
		Company.Board[Company.Board_Pointer][1] = (char) Second_Argument;
		Company.Board[Company.Board_Pointer][2] = Operator;
		if (Company.Mode.equals("Talkative")) {
			System.out.println("President pined a task on Board: "
				+ (int)Company.Board[Company.Board_Pointer][0] + " | " + (int)Company.Board[Company.Board_Pointer][1]
						+ " | " + Company.Board[Company.Board_Pointer][2]);
		}
		Company.Board_Pointer++;
		Company.Board_Elements++;
		Company.Board_Spaces--;
	}
	
	public void run() {
		while (true) {
			if (Company.Board_Spaces > 0) {
				synchronized(Company.Board) {
					Create_Task();
				}
				try {
					this.sleep(President_Delay);
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
	}
	
	public void start() {
		thread = new Thread(this, "President");
		thread.start();
		System.out.println("President started");
	}
}
