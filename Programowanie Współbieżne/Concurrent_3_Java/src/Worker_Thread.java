public class Worker_Thread extends Thread {
	private Thread thread;
	private int Worker_Delay;
	private Task task;
	private boolean machineIsBroken = true;
	int Product;
	private int id;
	private final Company company;
	
	int First_Argument, Second_Argument;
	char Operator;
	
	private static AddingMachine addingMachine;
	private static MultiplyingMachine multiplyingMachine;


	Worker_Thread(int Worker_Delay, int id, Company company) {
		this.Worker_Delay = Worker_Delay;
		this.id = id;
		this.company = company;
	}
	
	void Get_Task() throws InterruptedException {
		
		task = new Task();
		if (!company.Board.isEmpty()) {
			task = company.Board.take();
			if (task == null) {
				System.out.println("~~W~~ Task occured to be null");
				Get_Task();
			}
			First_Argument = task.argument_1;
			Second_Argument = task.argument_2;
			Operator = task.operator;
			if (company.Mode.equals("Talkative")) {
				System.out.println("Worker got task: " + First_Argument + " | " 
						+ Second_Argument + " | " + Operator);
			}
			company.Board_Spaces++;		
			company.Board_Elements--;
		}
		else Get_Task();
	}
	
	void Produce() throws InterruptedException {
		if (Operator == '-') {
			while (true) {
				addingMachine = company.freeAddingMachines.take();
				System.out.println("Worker [" + id + "] stored record in Adding_Machine [" + addingMachine.id + "]");
				addingMachine.storeRecord(task);
				addingMachine.pushButton();
				if (addingMachine.broken == false) {
					task = addingMachine.takeRecord();
					Product = task.product;
					System.out.println("~~W~~ Worker got product from AddingMachine");
					//company.freeAddingMachines.put(addingMachine);
					Main.companies[addingMachine.company.companyIdentifier].freeAddingMachines.put(addingMachine);
					break;
				}
			}
			System.out.println("Worker [" + id + "] got product from Adding_Machine [" + addingMachine.id + "] = " + Product);
			//Product = First_Argument - Second_Argument;
		}
		else if (Operator == '+') {
			while (true) {
				addingMachine = company.freeAddingMachines.take();
				System.out.println("Worker [" + id + "] stored record in Adding_Machine [" + addingMachine.id + "]");
				addingMachine.storeRecord(task);
				addingMachine.pushButton();
				if (addingMachine.broken == false) {
					task = addingMachine.takeRecord();
					Product = task.product;
					System.out.println("~~W~~ Worker got product from AddingMachine");
					//company.freeAddingMachines.put(addingMachine);
					Main.companies[addingMachine.company.companyIdentifier].freeAddingMachines.put(addingMachine);
					break;
				}
			}
			System.out.println("Worker [" + id + "] got product from Adding_Machine [" + addingMachine.id + "] = " + Product);

			//Product = First_Argument + Second_Argument;
		}
		else if (Operator == '*') {
			while (true) {
			multiplyingMachine = company.freeMultiplyingMachines.take();
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
				System.out.println("~~W~~ Worker got product from MultiplyingMachine");
				//company.freeMultiplyingMachines.put(multiplyingMachine);
				Main.companies[multiplyingMachine.company.companyIdentifier].freeMultiplyingMachines.put(multiplyingMachine);
				break;
			}
			//Product = First_Argument * Second_Argument;
			 }
		}
	}
	
	void Store_Product() {
		company.Magazine[company.Magazine_Pointer] = task;
		company.Magazine_Pointer++;
		if (company.Mode.equals("Talkative")) {
			System.out.println("Worker stored in Magazine Product: " + Product + " | Magazine_Pointer = " + company.Magazine_Pointer);
		}
		company.Magazine_Elements++;
		company.Magazine_Spaces--;
	}
	
	public void run() {
		//------Zamienić while(true) na czekanie az miejsce się zwolni
		//np. wait()...
		while (true) {
			if (company.Board_Elements > 0 && company.Magazine_Spaces > 0 || company.neededWorker.isEmpty() == false) {
				if (company.neededWorker.isEmpty() == false) {
					try {
						multiplyingMachine = company.neededWorker.take();
						multiplyingMachine.pushButtonTwo();
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				else {
				synchronized(company.Board) {
					try {
						Get_Task();
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					try {
						Produce();
						System.out.println("~~W~~ Worker: " + id + " produced and will put product to the Queue");
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
				//synchronized(company.Magazine) {
					//Store_Product();
					try {
						System.out.println("~~W~~ I want to store task");
						if (task.operator == '+') {
							company.addingQueue.put(task);
							System.out.println("~~~~Worker stored task: " + task.argument_1 + " | " + task.argument_2 + " | " + task.operator + " | " + task.product + " | in addingQueue");
						}	
						else if (task.operator == '-') {
							company.subtractingQueue.put(task);
							System.out.println("~~~~Worker stored task: " + task.argument_1 + " | " + task.argument_2 + " | " + task.operator + " | " + task.product + " | in subtractingQueue");
						}
						else if (task.operator == '*') {
							company.multiplyingQueue.put(task);
							System.out.println("~~~~Worker stored task: " + task.argument_1 + " | " + task.argument_2 + " | " + task.operator + " | " + task.product + " | in multiplyingQueue");
						}
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
			//	}
				/*System.out.println("B_Elements = " + company.Board_Elements
						+ "B_Spaces = " + company.Board_Spaces 
						+ "M_Elements = " + company.Magazine_Elements
						+ "M_Spaces = " + company.Magazine_Spaces);*/
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
