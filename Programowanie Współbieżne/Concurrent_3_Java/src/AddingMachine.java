import java.util.Random;


public class AddingMachine extends Thread {
	public final Company company;
	private Thread thread;
	private Task task;
	public int id;
	public boolean broken = false;
	Random random = new Random();
	
	AddingMachine(int id, Company company) {
		this.id = id;
		this.company = company;
		
	}
	
	public void storeRecord(Task task) {
		this.task = task;
	}
	
	public void pushButton() throws InterruptedException {
		System.out.println("Button pushed in AddingMachine");
		thread.sleep(company.addingMachinesDelay);
		float fail = random.nextFloat();
		if (fail > 0.95) {
			company.brokenMachines.put(this);
			company.numberOfBrokenMachines++;
			broken = true;
			System.out.println("Adding Machine [" + id + "] is broken");
		}
		else {
			if (task.operator == '+') {
				task.product = task.argument_1 + task.argument_2;
			}
			else if (task.operator == '-') {
				task.product = task.argument_1 - task.argument_2;
			}
			else {
				System.out.println("Incorrect operator -> product = 0");
				task.product = 0;
			}
		}
	}
	
	public Task takeRecord() {
		return task;
	}
	
	public void run() {
		
	}
	
	public void start() {
		thread = new Thread(this, "AddingMachine");
		thread.start();
		System.out.println("AddingMachine " + id + " started");
	}
}
