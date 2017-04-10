
public class President_Thread extends Thread{
	
	private Thread thread;
	private final Company company;
	private int President_Delay;
	private Task task;
	
	String Operators = "-+*";
	int First_Argument, Second_Argument;
	char Operator;

	President_Thread(int President_Delay, Company company) {
		this.President_Delay = President_Delay;
		this.company = company;
	}
	
	void Create_Task() throws InterruptedException {
		First_Argument = 0 + (int)(Math.random()*100); //randomNum = minimum + (int)(Math.random()*maximum); 
		Second_Argument = 0 + (int)(Math.random()*100);
		Operator = Operators.charAt(0 + (int)(Math.random()*3));

		task = new Task();
		task.argument_1 = First_Argument;
		task.argument_2 =  Second_Argument;
		task.operator = Operator;
		company.Board.put(task);
		if (company.Mode.equals("Talkative")) {
			System.out.println(company.companyIdentifier + ": President pined a task on Board: "
				+ task.argument_1 + " | " + task.argument_2
						+ " | " + task.operator + " ----> Board_Elements = " + (company.Board_Elements + 1));
		}
		company.Board_Elements++;
		company.Board_Spaces--;
	}
	
	public void run() {
		while (true) {
			if (company.Board_Spaces > 0) {
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
				/*System.out.println("B_Elements = " + company.Board_Elements
						+ "B_Spaces = " + company.Board_Spaces 
						+ "M_Elements = " + company.Magazine_Elements
						+ "M_Spaces = " + company.Magazine_Spaces);*/
			}
		}
	}
	
	public void start() {
		thread = new Thread(this, "President");
		thread.start();
		System.out.println("President started");
	}
}