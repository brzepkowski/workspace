import Definitions.*;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.CopyOnWriteArrayList;

public class Main {
	public static int numberOfCompanies = Definitions.numberOfCompanies;
	public static int numberOfShops = Definitions.numberOfShops;
	public static int numberOfLorries = Definitions.numberOfLorries;
	
	//public static BlockingQueue<Company> companies;
	public static Company[] companies;
	public static BlockingQueue<Lorry> freeLorries;
	public static BlockingQueue<Shop> shops;
	
	
	public static void main (String args[]) throws InterruptedException {
		
		//companies = new ArrayBlockingQueue(numberOfCompanies);
		companies = new Company[numberOfCompanies];
		for (int i = 0; i < numberOfCompanies; i++) {
			 /*Company company = new Company(i);
			 companies.add(company);*/
			Company company = new Company(i);
			companies[i] = company;
			companies[i].start();
		}
		
		freeLorries = new ArrayBlockingQueue(Definitions.numberOfLorries);
		for (int i = 0; i < Definitions.numberOfLorries; i++) {
			Lorry lorry = new Lorry(i);
			//lorry.start();
			freeLorries.put(lorry);
		}
		
		shops = new ArrayBlockingQueue(Definitions.numberOfShops);
		for (int i = 0; i < Definitions.numberOfShops; i++) {
			Shop shop = new Shop(i);
			shops.put(shop);
		}
		
	}
}
