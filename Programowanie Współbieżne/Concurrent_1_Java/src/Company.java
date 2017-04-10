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
	
	public static char[][] Board = new char[Board_Size][3];
	public static int[] Magazine = new int[Magazine_Size];
	
	public static void main(String args[]) {
		System.out.println("Company started");
		
		if (Mode.equals("Silent")) {
			System.out.println("Company started in Silent mode");
			L_Thread = new Listener_Thread();
			L_Thread.start();
		}
		else {
			System.out.println("Company started in Talkative mode");
		}
		
		P_Thread = new President_Thread(President_Delay); 
		P_Thread.start();
		
		W_Threads = new Worker_Thread[Number_Of_Workers];
		for (int i = 0; i < W_Threads.length; i++) {
		    W_Threads[i] = new Worker_Thread(Worker_Delay);
		    W_Threads[i].start();
		}
		
		B_Thread = new Buyer_Thread(Buyer_Delay); 
		B_Thread.start();
		
	}
}
