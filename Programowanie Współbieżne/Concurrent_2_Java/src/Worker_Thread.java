public class Worker_Thread extends Thread {
	private Thread thread;
	private int Worker_Delay;
	private Task task;
	private boolean machineIsBroken = true;
	int Product;
	private int id;
	
	int First_Argument, Second_Argument;
	char Operator;
	
	private static AddingMachine addingMachine;
	private static MultiplyingMachine multiplyingMachine;


	Worker_Thread(int Worker_Delay, int id) {
		this.Worker_Delay = Worker_Delay;
		this.id = id;
	}
	
	void Get_Task() throws InterruptedException {
		
		task = new Task();
		task = Company.Board.take();
		First_Argument = task.argument_1;
		Second_Argument = task.argument_2;
		Operator = task.operator;
		if (Company.Mode.equals("Talkative")) {
			System.out.println("Worker got task: " + First_Argument + " | " 
					+ Second_Argument + " | " + Operator);
		}
		Company.Board_Spaces++;		
		Company.Board_Elements--;
	}
	
	void Produce() throws InterruptedException {
		if (Operator == '-') {
			while (true) {
				addingMachine = Company.freeAddingMachines.take();
				System.out.println("Worker [" + id + "] stored record in Adding_Machine [" + addingMachine.id + "]");
				addingMachine.storeRecord(task);
				addingMachine.pushButton();
				if (addingMachine.broken == false) {
					task = addingMachine.takeRecord();
					Product = task.product;
					Company.freeAddingMachines.put(addingMachine);
					break;
				}
			}
			System.out.println("Worker [" + id + "] got product from Adding_Machine [" + addingMachine.id + "] = " + Product);
			//Product = First_Argument - Second_Argument;
		}
		else if (Operator == '+') {
			while (true) {
				addingMachine = Company.freeAddingMachines.take();
				System.out.println("Worker [" + id + "] stored record in Adding_Machine [" + addingMachine.id + "]");
				addingMachine.storeRecord(task);
				addingMachine.pushButton();
				if (addingMachine.broken == false) {
					task = addingMachine.takeRecord();
					Product = task.product;
					Company.freeAddingMachines.put(addingMachine);
					break;
				}
			}
			System.out.println("Worker [" + id + "] got product from Adding_Machine [" + addingMachine.id + "] = " + Product);

			//Product = First_Argument + Second_Argument;
		}
		else if (Operator == '*') {
			while (true) {
			multiplyingMachine = Company.freeMultiplyingMachines.take();
			System.out.println("Worker [" + id + "] stored record in Multiplying_Machine [" + multiplyingMachine.id + "]");
			multiplyingMachine.storeRecord(task);
			multiplyingMachine.pushButtonOne();
			while (multiplyingMachine.otherButtonPushed == false) {
				System.out.println("====>Czekam na drugiego pracownika");
				this.sleep(2000);
			}
			if (multiplyingMachine.broken == false) {
				task = multiplyingMachine.takeRecord();
				Product = task.product;
				Company.freeMultiplyingMachines.put(multiplyingMachine);
				break;
			}
			//Product = First_Argument * Second_Argument;
			 }
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
			if (Company.Board_Elements > 0 && Company.Magazine_Spaces > 0 || Company.neededWorker.isEmpty() == false) {
				if (Company.neededWorker.isEmpty() == false) {
					try {
						multiplyingMachine = Company.neededWorker.take();
						multiplyingMachine.pushButtonTwo();
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				else {
				synchronized(Company.Board) {
					try {
						Get_Task();
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					try {
						Produce();
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
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
	}
	
	public void start() {
		thread = new Thread(this, "Worker");
		thread.start();
		System.out.println("Worker started");
	}

}
