
public class Mechanic extends Thread {

	private Thread thread;
	private Object brokenMachine;
	
	public void run() {
		while (true) {
			try {
				brokenMachine = Company.brokenMachines.take();
				this.sleep(3000);
				if (brokenMachine instanceof AddingMachine) {
					Company.freeAddingMachines.put((AddingMachine) brokenMachine);
					System.out.println("Mechanic repaired Adding Machine");
				}
				else {
					Company.freeMultiplyingMachines.put((MultiplyingMachine) brokenMachine);
					System.out.println("Mechanic repaired Multiplying Machine");
				}
				
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public void start() {
		thread = new Thread(this, "Mechanic");
		thread.start();
		System.out.println("Mechanic started");
	}
}
