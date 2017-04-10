import java.util.Random;

public class MultiplyingMachine extends Thread {

	public static int id;
	private Thread thread;
	private Task task;
	public boolean broken = false;
	public boolean otherButtonPushed = false;
	Random random = new Random();
	
	MultiplyingMachine(int id) {
		this.id = id;
	}
	
	public void storeRecord(Task task) {
		this.task = task;
	}
	
	public void pushButtonOne() throws InterruptedException {
			Company.neededWorker.put(this);
	}
	
	public void pushButtonTwo() throws InterruptedException  {
		float fail = random.nextFloat();
		if (fail > 0.95) {
			Company.brokenMachines.put(this);
			Company.numberOfBrokenMachines++;
			broken = true;
			System.out.println("Multiplying Machine [" + id + "] is broken");
		}
		else {
			System.out.println("!!!!!!Pracownik wciska drugi przycisk maszyny [" + id + "]");
			otherButtonPushed = true;
			task.product = task.argument_1 * task.argument_2;
		}
	}
	
	public Task takeRecord() {
		return task;
		
	}
	
	public void run() {
		
	}
	
	public void start() {
		thread = new Thread(this, "MultiplyingMachine");
		thread.start();
		System.out.println("MultiplyingMachine " + id + " started");
	}
}
