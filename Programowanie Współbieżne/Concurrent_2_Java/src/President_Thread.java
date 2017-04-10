
public class President_Thread extends Thread{
	
	private Thread thread;
	private int President_Delay;
	private Task task;
	
	String Operators = "-+*";
	int First_Argument, Second_Argument;
	char Operator;

	President_Thread(int President_Delay) {
		this.President_Delay = President_Delay;
	}
	
	void Create_Task() throws InterruptedException {
		First_Argument = 0 + (int)(Math.random()*100); //randomNum = minimum + (int)(Math.random()*maximum); 
		Second_Argument = 0 + (int)(Math.random()*100);
		Operator = Operators.charAt(0 + (int)(Math.random()*3));

		task = new Task();
		task.argument_1 = First_Argument;
		task.argument_2 =  Second_Argument;
		task.operator = Operator;
		Company.Board.put(task);
		if (Company.Mode.equals("Talkative")) {
			System.out.println("President pined a task on Board: "
				+ task.argument_1 + " | " + task.argument_2
						+ " | " + task.operator);
		}
		Company.Board_Elements++;
		Company.Board_Spaces--;
	}
	
	public void run() {
		while (true) {
			if (Company.Board_Spaces > 0) {
					try {
						Create_Task();
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
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