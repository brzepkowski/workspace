import java.util.Arrays;
import java.util.Random;
import java.lang.Math;
public class Node extends Thread {

	public  int location;
	private int identifier;
	private Thread thread;
	Random rand = new Random();
	boolean leftSide = false;
	boolean collision = false;
	int failedTransmissions = 0;
	double power = 1;

	
	Node(int identifier, int location) {
		this.identifier = identifier;
		this.location = location;
	}
	
	public void transmit() {	
		
		  int randomNode = identifier;
		  while (randomNode == identifier) {
			  randomNode = rand.nextInt(((Main.numberOfNodes-1) - 0) + 1) + 0;
		  }
		  System.out.println("[ " + identifier + " ] Węzeł " + identifier + " będzie nadawał do węzła " + randomNode);
		  try {
				this.sleep(2000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		  
		  double s = Main.locationOfNodes[randomNode] - Main.locationOfNodes[identifier];
		  if (s < 0) {
			  leftSide = true;
			  s = -s;
		  }
		  double t = s / 300000;
		  
		  //System.out.println("[ " + identifier + " ] Droga z " + identifier + " do " + randomNode + " = " + s + " | czas = " + t);
		  
		  double littleTime = 1/300000.0;
		  double timeOfCurrentTravel = 0;
		  
		  collision = false;
		  
		  if (leftSide == false) {
			  System.out.println("[ " + identifier + " ] - W PRAWO");
			  for (int i = location; i < Main.locationOfNodes[randomNode]; i++) {
				  if(Main.jam == true) {
					  failedTransmissions++;
					  System.out.println("|JAM| [ " + identifier + " ] Będę losował czas do oczekiwania");
					  Main.nodesNotifiedAboutJam++;
					  if (Main.nodesNotifiedAboutJam == Main.numberOfNodes) {
						  System.out.println("====> wszyscy powiadomieni o JAM");
						  Main.nodesNotifiedAboutJam = 0;
						  Main.jam = false;
					  }
					  power++;
					  int delay = rand.nextInt(((int)Math.pow(2.0, power) - 1) + 1) + 1;
					  try {
						//this.sleep(1000 * delay);
						  this.sleep((long) (t * delay));
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					  collision = true;
					  break;
				  }
				  //if(Main.cableFree == false) {
				  if (Main.cable[location] != 0) {
					  failedTransmissions++;
					  System.out.println("|Zajęty kabel| [ " + identifier + " ] Będę losował czas do oczekiwania");
					  collision = true;
					  break;
				  }
				  if (Main.cable[i+1] == 0) {
					  System.out.println("[ " + identifier + " ] - cable[" + (i+1) + "] = " + Main.cable[i+1]);
					  Main.cable[i+1] = 1;
					 /* try {
							this.sleep(500);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}*/
					  timeOfCurrentTravel += littleTime;
				  }
				  else {
					  failedTransmissions++;
					 // System.out.println("[ " + identifier + " ] Natrafiono na kolizję. Czas podróży = " + timeOfCurrentTravel);
					  try {
						  this.sleep((long) timeOfCurrentTravel*2);
						  //Main.cable.wait((long) (timeOfCurrentTravel*2));
						  if (Main.jam != true) {
							  Main.jam = true;
							  System.out.println("[ " + identifier + " ] sending |JAM| signal");
							  Main.nodesNotifiedAboutJam++;
							  //Main.cable.notify();
						  }
						  else {
							  System.out.println("|JAM| [ " + identifier + " ] Będę losował czas do oczekiwania");
							  Main.nodesNotifiedAboutJam++;
							  if (Main.nodesNotifiedAboutJam == Main.numberOfNodes) {
								  System.out.println("====> wszyscy powiadomieni o JAM");
								  Main.nodesNotifiedAboutJam = 0;
								  Main.jam = false;
							  }
							  if (power < 10) power++;
							  int delay = rand.nextInt(((int)Math.pow(2.0, power) - 1) + 1) + 1;
							  //this.sleep(1000 * delay);
							  this.sleep((long) (t * delay));
						  }
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					  collision = true;
					  break;
				  }
			  }
			  
			  //if (collision == false && Main.cableFree) {
			  if (collision == false) {
			  	  failedTransmissions = 0;
				  power = 1;
				  System.out.println("===>SUKCES | Będa wysyłane dane z " + identifier + " do " + randomNode);
				  //if (Main.cableFree) Main.cableFree = false;
				  Arrays.fill(Main.cable, 0);
				  System.out.println("[ " + identifier + " ] Wysyła dane...");
				  try {
					this.sleep(3000);
				  } catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				  }
				  /* for (int i = location; i < Main.locationOfNodes[randomNode]; i++) {
						  Main.cable[i+1] = 0;
				  }*/
				  //if (Main.cableFree==false) Main.cableFree = true;
			  }
			  else {
				  System.out.println("==>Kolizja z " + identifier + " do " + randomNode);
				  Arrays.fill(Main.cable, 0);
				  /*for (int i = location; i < Main.locationOfNodes[randomNode]; i++) {
					  Main.cable[i+1] = 0;
				  }	*/
			  }
			  
		  }
		  else {
			  System.out.println("[ " + identifier + " ] - W LEWO");
			  for (int i = location; i > Main.locationOfNodes[randomNode]; i--) {
				  if(Main.jam == true) {
					  failedTransmissions++;
					  System.out.println("|JAM| [ " + identifier + " ] Będę losował czas do oczekiwania");
					  Main.nodesNotifiedAboutJam++;
					  if (Main.nodesNotifiedAboutJam == Main.numberOfNodes) {
						  System.out.println("====> wszyscy powiadomieni o JAM");
						  Main.nodesNotifiedAboutJam = 0;
						  Main.jam = false;
					  }
					  power++;
					  int delay = rand.nextInt(((int)Math.pow(2.0, power) - 1) + 1) + 1;
					  try {
						//this.sleep(1000 * delay);
						  this.sleep((long) (t * delay));
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					  collision = true;
					  break;
				  }
				  //if(Main.cableFree == false) {
				  if (Main.cable[location] != 0) {
					  System.out.println("|Zajęty kabel| [ " + identifier + " ] Będę losował czas do oczekiwania");
					  failedTransmissions++;
					  collision = true;
					  break;
				  }
				  if (Main.cable[i-1] == 0) {
					  System.out.println("[ " + identifier + " ] - cable[" + (i-1) + "] = " + Main.cable[i-1]);
					  Main.cable[i-1] = 1;
					 /* try {
							this.sleep(500);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}*/
					  timeOfCurrentTravel += littleTime;
				  }
				  else {
					  failedTransmissions++;
					  System.out.println("[ " + identifier + " ] Natrafiono na kolizję. Czas podróży = " + timeOfCurrentTravel);
					  try {
							  this.sleep((long) timeOfCurrentTravel*2);
							  //Main.cable.wait((long) (timeOfCurrentTravel*2));
							  if (Main.jam != true) {
								  Main.jam = true;
								  System.out.println("[ " + identifier + " ] wysyła sygnał |JAM|");
								  Main.nodesNotifiedAboutJam++;
								  //Main.cable.notify();
							  }
							  else {
								  System.out.println("|JAM| [ " + identifier + " ] Będę losował czas do oczekiwania");
								  Main.nodesNotifiedAboutJam++;
								  if (Main.nodesNotifiedAboutJam == Main.numberOfNodes) {
									  System.out.println("====> wszyscy powiadomieni o JAM");
									  Main.nodesNotifiedAboutJam = 0;
									  Main.jam = false;
								  }
								  if (power < 10) power++;
								  int delay = rand.nextInt(((int)Math.pow(2.0, power) - 1) + 1) + 1;
								  //this.sleep(1000 * delay);
								  this.sleep((long) (t * delay));
							  }
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					  collision = true;
					  break;
				  }
			  }
			 // if (collision == false && Main.cableFree) {
			  if (collision == false) {
			  System.out.println("===>SUKCES | Będą wysyłane dane z " + identifier + " do " + randomNode);
				  failedTransmissions = 0;
				  power = 1;
				  //if (Main.cableFree) Main.cableFree = false;
				  Arrays.fill(Main.cable, 0);
				  System.out.println("[ " + identifier + " ] Wysyła dane...");
				  try {
					this.sleep(3000);
				  } catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				  }
				/*  for (int i = location; i > Main.locationOfNodes[randomNode]; i--) {
						  Main.cable[i-1] = 0;
				  }*/
				  //if (Main.cableFree==false) Main.cableFree = true;
			  }
			  else {
				  System.out.println("==>Kolizja z " + identifier + " do " + randomNode);
				  Arrays.fill(Main.cable, 0);
				/*  for (int i = location; i > Main.locationOfNodes[randomNode]; i--) {
					  Main.cable[i-1] = 0;
				  }	*/
			  }
		  }
	
	
	}
	
	public void run() {
		try {
			  int delay = rand.nextInt((5 - 1) + 1) + 1;
			  //this.sleep(1000 * delay);
			  this.sleep(2000);
		  } catch (InterruptedException e) {
			  // TODO Auto-generated catch block
			  e.printStackTrace();
		  }
		while (true) {
			//if (Main.cableFree == true) {
				if (failedTransmissions == 16) {
					System.out.println("[ " + identifier + " ] - 16 kolizji pod rząd -> Węzeł kończy wysyłanie.");
					break;
				}
				transmit();
			//}
		}
	}
	
	public void start() {
		thread = new Thread(this, "Node");
		thread.start();
		System.out.println("Node " + identifier + " started");
	}
	
}
