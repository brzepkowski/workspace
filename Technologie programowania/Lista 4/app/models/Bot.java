import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.*;
import java.net.*;
import java.util.Random;

import javax.swing.ImageIcon;

class Bot extends Thread {
	
	public int someoneBet = 0;
	  public int someoneRaised = 0;
	  public int highestBid = 0;
	  public int lastOffer = 0;
	  public int foldStatus = 0;
	  public int allInStatus= 0;
	  public int tokens = 0;
	  public int numberOfCardsChange = 0;

	public String personalNumber = "";
	boolean isConnected = false;
	public String c1;
	public String c2;
	public String c3;
	public String c4;
	public String status;
	public String highest;

	public String[] parts;
	public String part1 = "";
	public String part2 = "";
	public String part3 = "";
	public String part4 = "";
	public String part5 = "";
	public String part6 = "";

	Socket socket = null;
	PrintWriter out = null;
	BufferedReader in = null;
	public int port;

	Bot(int p) {
		this.port = p;
		start();
		System.out.println("bot wystartował");
	}

	public void listenSocket(int port) {
		System.out.println("Weszło do listenSocket");
		try {
			if (isConnected == true) {}
			else if (isConnected == false){
				String hostAddress = "localhost";
				int portAddress = port;
				socket = new Socket(hostAddress, portAddress);
				out = new PrintWriter(socket.getOutputStream(), true);
				in = new BufferedReader(new InputStreamReader(
						socket.getInputStream()));
				personalNumber = in.readLine();
				System.out.println(personalNumber + " Połączono: " + socket);
				
				System.out.println("Connected, personalNumber is "
						+ personalNumber);
				isConnected = true;
			}
		} catch (UnknownHostException e) {
			System.out.println("Unknown host");
			System.exit(1);
		} catch (IOException e) {
			System.out.println("No I/O");
		}
	}

	@Override
	public void run() {
		while (true) {
			try {
				if (isConnected == false) {
					listenSocket(port);
				}
				String message = in.readLine();
				Interpreter(message);

				System.out.println("Message odebrana przez klienta: //"
						+ message);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}
	}

	public void Interpreter(String message) {
		try {
			parts = message.split("-");
			part1 = parts[0];
			part2 = parts[1];
			part3 = parts[2];
			part4 = parts[3];
			part5 = parts[4];
			part6 = parts[5];
		} catch (NullPointerException e) {
			System.exit(0);
		}

		if (part1.equals("Status")) {
			if (part2.equals(personalNumber)) {
				status = "dealer";
			} else if (part3.equals(personalNumber)) {
				status = "smallBlind";
			} else if (part4.equals(personalNumber)) {
				status = "bigBlind";
			}
		}
		else if (part1.equals("BasicCards")) {
			if (part2.equals(personalNumber)) {
				setCardImage();
			}
			
		}
			else if (part1.equals("Tokens")) {
			if (part2.equals(personalNumber))
				tokens = Integer.parseInt(part3);
		} else if (part1.equals("Auction")) {
			if (part2.equals("Highest")) {
				highest = part3;
			}
		} else if (part1.equals("UnlockAuctionButtons")) {
			if (part2.equals(personalNumber)) {
				randomAnswer();

			}
		} else if (part1.equals("GetCardValues")) {
			if (part2.equals(personalNumber)) {
				System.out.println("BOT CARD VALUES: "+ c1 + "-" + c2
					+ "-" + c3 + "-" + c4);
			String str = "CardValues-" + personalNumber + "-" + c1 + "-" + c2
					+ "-" + c3 + "-" + c4;
			out.println(str);
			}
		}
		/*else if (part1.equals("ChangeOfCards")) {
			  if (part2.equals(personalNumber)) {
				  System.out.println("Do zamiany: " + part3 + "-" + part4 + "-" + part5 + "-" + part6);
				  changeOfCards(part3, part4, part5, part6);
			  }
		  }*/
		else if (part1.equals("UnlockCardButtons")) {
			  if (part2.equals(personalNumber)) {
				  System.out.println("BOT CHANGE CARDS");
				  String str = "ChangeOfCards-" + personalNumber + "-0-0-0-0";
				  //unlockCardButtons();
			  }
		  }
		else if (part1.equals("Disconnected")) {
			System.out
					.println("Jeden z graczy siÄ™ rozĹ‚Ä…czyĹ‚, nastÄ…pi wyĹ‚Ä…czenie programu");
			System.exit(0);
		}
	}
	public void randomAnswer() {
		/*if (status.equals("dealer")) {
			String str = "Auction-" + personalNumber + "-call-0-0-0";
			lastOffer = highestBid;
			highestBid = lastOffer;
			out.println("LastOffer-" + personalNumber + "-" + lastOffer + "-0-0-0");
			out.println("HighestBid-" + personalNumber + "-" + lastOffer + "-0-0-0");
			   out.println(str);
		}
		else {*/
		/*System.out.println("BOT CARD VALUES: "+ c1 + "-" + c2
				+ "-" + c3 + "-" + c4);
		String str = "CardValues-" + personalNumber + "-" + c1 + "-" + c2
				+ "-" + c3 + "-" + c4;
		out.println(str);*/
		System.out.println("BOT: Weszlo do randomAnswer");
		foldStatus = 1;
		out.println("AnswerFoldStatus-" + personalNumber + "-" + foldStatus + "-0-0-0");
		out.println("Auction-" + personalNumber + "-fold-0-0-0");
		//}
		
	}
		
		public static int randInt(int min, int max) {

		    Random rand = new Random();

		    int randomNum = rand.nextInt((max - min) + 1) + min;

		    return randomNum;
		}
		public void setCardImage () {
			  
			  if (personalNumber.equals(part2)) {
		  
			  for (int i = 0; i < 4; i++) {
				  if (i == 0) {
					  
					  c1 = part3;
				  }
				  
				  if (i == 1) {
					  	
					  c2 = part4;
				  }
				  
				  if (i == 2) {
					  
					  c3 = part5;
				  }
				  
				  if (i == 3) {	
					  c4 = part6;
				  }
				  
			  }
			  }
		  
		  }

	
}
