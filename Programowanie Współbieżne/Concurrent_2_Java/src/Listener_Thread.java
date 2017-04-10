import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;


public class Listener_Thread extends Thread {
	
	private Thread thread;
	BufferedReader buffered_Reader;
	String Option;
	
	public void run() {
		while (true) {
			System.out.println("Available options:");
			System.out.println("-Show Board");
			System.out.println("-Show Magazine");
			try {
				Option = buffered_Reader.readLine();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if (Option.equals("Show Board")) {
				for (int i = 0; i < Company.Board_Pointer; i++) {
					System.out.println(Company.Board);
				}
			}
			else if (Option.equals("Show Magazine")) {
				for (int i = 0; i < Company.Magazine_Pointer; i++) {
					System.out.println(Company.Magazine[i]);
				}
			}
			else {
				System.out.println("Incorrect option.");
			}
			
		}
	}
	
	public void start() {
		buffered_Reader = new BufferedReader(new InputStreamReader(System.in));
		thread = new Thread(this, "President");
		thread.start();
	}


}
