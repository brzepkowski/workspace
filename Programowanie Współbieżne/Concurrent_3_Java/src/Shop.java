import Definitions.*;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class Shop {

		public int identifier;
		public static int magazineSize = Definitions.shopMagazineSize;
	
		public static BlockingQueue<Task> addingMagazine = new ArrayBlockingQueue(magazineSize);
		public static BlockingQueue<Task> subtractingMagazine = new ArrayBlockingQueue(magazineSize);
		public static BlockingQueue<Task> multiplyingMagazine = new ArrayBlockingQueue(magazineSize);
		
		
		public static Manager manager;
		public static Shop_Client[] shopClients;
		
		public Shop(int n) {
			identifier = n;
			System.out.println("Shop: " + identifier + " started");
			
			addingMagazine = new ArrayBlockingQueue(magazineSize);
			subtractingMagazine = new ArrayBlockingQueue(magazineSize);
			multiplyingMagazine = new ArrayBlockingQueue(magazineSize);
			
			manager = new Manager(this);	
			manager.start();
			
			shopClients = new Shop_Client[Definitions.numberOfShopClients];
			for (int i = 0; i < Definitions.numberOfShopClients; i++) {
				shopClients[i] = new Shop_Client(this);
				shopClients[i].start();
			}
			
			
		}
		
		
}
