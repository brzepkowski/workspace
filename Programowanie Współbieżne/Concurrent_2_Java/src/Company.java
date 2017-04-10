import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import Definitions.*;
public class Company {
	public static String Mode = Definitions.Mode;
	

	public static int Board_Size = Definitions.Board_Size;
	public static int Magazine_Size = Definitions.Magazine_Size;
	public static int Board_Pointer = Definitions.Board_Pointer;
	public static int Magazine_Pointer = Definitions.Magazine_Pointer;
	public static int President_Delay = Definitions.President_Delay;
	public static int Worker_Delay = Definitions.Worker_Delay;
	public static int Buyer_Delay = Definitions.Buyer_Delay;
	public static int Number_Of_Workers = Definitions.Number_Of_Workers;
	
	
	public static int Board_Spaces = Board_Size;
	public static int Board_Elements = 0;
	public static int Magazine_Spaces = Magazine_Size;
	public static int Magazine_Elements = 0;
	
	
	public static President_Thread P_Thread;
	public static Worker_Thread[] W_Threads;
	public static Buyer_Thread B_Thread;
	public static Listener_Thread L_Thread;
	
	public static BlockingQueue<Task> Board = new ArrayBlockingQueue(Board_Size);
	public static int[] Magazine = new int[Magazine_Size];
	
	//Queues, Machines and Mechanic
	public static Mechanic mechanic;
	public static int addingMachinesDelay = Definitions.addingMachinesDelay;
	public static int multiplyingMachinesDelay = Definitions.multiplyingMachinesDelay;
	
	public static int numberOfAddingMachines = Definitions.numberOfAddingMachines;
	public static int numberOfMultiplyingMachines = Definitions.numberOfMultiplyingMachines;
	public static int numberOfBrokenMachines = 0;
	public static BlockingQueue<AddingMachine> freeAddingMachines = new ArrayBlockingQueue(numberOfAddingMachines);
	public static BlockingQueue<MultiplyingMachine> freeMultiplyingMachines = new ArrayBlockingQueue(numberOfMultiplyingMachines);
	public static BlockingQueue<Object> brokenMachines = new ArrayBlockingQueue(numberOfMultiplyingMachines 
																						+ numberOfAddingMachines);
	public static BlockingQueue<MultiplyingMachine> neededWorker = new ArrayBlockingQueue(numberOfMultiplyingMachines);

	public static AddingMachine[] addingMachines = new AddingMachine[numberOfAddingMachines];
	public static MultiplyingMachine[] multiplyingMachines = new MultiplyingMachine[numberOfMultiplyingMachines];
	
	public static void main(String args[]) throws InterruptedException {
		System.out.println("Company started");
		
		if (Mode.equals("Silent")) {
			System.out.println("Company started in Silent mode");
			L_Thread = new Listener_Thread();
			L_Thread.start();
		}
		else {
			System.out.println("Company started in Talkative mode");
		}
		
		//Machines
		for (int i = 0; i < numberOfAddingMachines; i++) {
			addingMachines[i] = new AddingMachine(i);
			addingMachines[i].start();
			freeAddingMachines.put(addingMachines[i]);
		}
		for (int i = 0; i < numberOfMultiplyingMachines; i++) {
			multiplyingMachines[i] = new MultiplyingMachine(i);
			multiplyingMachines[i].start();
			freeMultiplyingMachines.put(multiplyingMachines[i]);
		}
		
		P_Thread = new President_Thread(President_Delay); 
		P_Thread.start();
		
		mechanic = new Mechanic();
		mechanic.start();
		
		W_Threads = new Worker_Thread[Number_Of_Workers];
		for (int i = 0; i < W_Threads.length; i++) {
		    W_Threads[i] = new Worker_Thread(Worker_Delay, i);
		    W_Threads[i].start();
		}
		
		B_Thread = new Buyer_Thread(Buyer_Delay); 
		B_Thread.start();
		
	}
}
