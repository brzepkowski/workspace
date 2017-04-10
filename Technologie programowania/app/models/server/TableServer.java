package models.server;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Random;

/**Klasa definiujaca serwer gry
 * @author Kornel Mirkowski
 */
public class TableServer {
	/**Gniazdko serwera*/
	private ServerSocket server = null;
	/**Dane gry*/
	public Data data;
	/**Generator liczb losowych*/
	private Random generator = new Random();
	
	/**Konstruktor serwera
	 * @param port Port serwera
	 * @param players Liczba graczy
	 * @param startCash Poczatkowa liczba zetonow
	 * @param bigBlind Wysokosc BigBlind
	 */
	public TableServer(int port, int players, int startCash, int bigBlind) { 
		try {
			server = new ServerSocket(port); 
		} catch (IOException e) {
			System.out.println("Could not listen on port " + port); System.exit(-1);
		}
		data = new Data(players, startCash, bigBlind);
		System.out.println("Serwer otwarty");
	}
	
	/**Laczenie graczy z serwerem
	 * @return Gniazdko serwera
	 */
	Socket serverAccept() {
		Socket temp = null;
		try {
			temp = server.accept();
		} catch (IOException e) { System.out.println("Accept failed: 4444"); System.exit(-1); }
		return temp;
	}
	
	/**Zakonczenie pracy i zakmniecie gniazdka
	 * @see java.lang.Object#finalize()
	 */
	@Override
	public void finalize() {
		try {
			server.close();
		} catch (IOException e) {
			System.out.println("Problem z zamknieciem"); System.exit(-1);
		}
	}
	
	/**Poczatkowe ustawienie serwer, wyslanie do graczy informacji o id, liczbie graczy i poczatkowych zetonach*/
	public void setUpTable () {
		for (int i=0; i < data.players(); i++)
			data.sendTo(i, ""+ i + data.players() + data.getStartCash());
	}
	
	/**Ustawienie rozgrywki, wyslanie kart i roli(blindy, dealer, itp) do graczy
	 * @param first Czy to pierwsza rozgrywka w grze (wtedy trzeba stworzyc talie i wylosowac kto zaczyna)
	 */
	public void setUpGame (boolean first) {
		int pl = data.playersInGame();
		data.setAllStatuses("", true);
		int n;
		if (first) {
			n = generator.nextInt(pl);
			data.makeDeck();
		} else {
			n = data.nextInRound(data.indexOfDealer);
			ArrayList<Card> t = data.deckSizee();
			if (t.size() != 52)
				System.out.println("deck size: " + t.size() +" "+ t);
		}
		
		int role = 0;
		if (pl == 2) {
			role = 1;
		}
			
		for (int i=0; i < pl; i++, role++) {
			ArrayList<Card> tempCards = new ArrayList<Card>();
			tempCards = data.takeCards(4);
			tempCards.sort(null);
			String message = Tools.cardsToString(tempCards);
			if (first) {
				data.setRole(n, role);
				data.sendTo(n, role + message + data.sb());
			} else {
				data.sendTo(n, data.role(n) + message + data.sb());
			}

			data.setCards(n, tempCards);
			n = data.nextInRound(n);
		}
	}
	
	/**Runda gry, wyslanie do graczy, ze jego kolej i odpowiednia oblsuga odpowiedzi.<br>
	 * Jesli gracz wysle zla wiadomosc, zostaje wyrzucony z gry.<br>
	 * Schemat rundy wyglada nastepujaco:<br>
	 * 1. serwer -> gracze : kogo kolej<br>
	 * 2. odpowiedni gracz -> serwer : zagranie<br>
	 * 3. serwer -> gracze : kto co zagral
	 * @param firstInRound Kto zaczyna, rownowaznie mozna powiedziec, czy to pierwsza runda rozgrywki,
	 * czyli czy maja byc zagrane blindy 
	 */
	void round(int firstInRound) {
		boolean wasBet = false;
		if (firstInRound == 3) {
			firstInRound = firstInRound % data.playersInGame();

			int j = data.indexOfSB;
			
			try {
				data.bid(j, data.sb());
			} catch (BadugiException e) {
				System.out.println(e.getMessage());
				int bid = data.allIn(j);
				try {
					data.bid(j, bid);
				} catch (BadugiException ex) { System.out.println(ex.getMessage()); }
				data.setStatus(j, "A");
			}
			
			j = data.indexOfBB;
			
			try {
				data.bid(j, data.bb());
			} catch (BadugiException e) {
				System.out.println(e.getMessage());
				int bid = data.allIn(j);
				try {
					data.bid(j, bid);
				} catch (BadugiException ex) { System.out.println(ex.getMessage()); }
				data.setStatus(j, "A");
			}
			
			wasBet = true;
		} else {
			data.setAllStatuses("", false);
		}
		
		int i;
		
		if (data.playersInGame() == 2) {
			i = data.indexOfSB;
		} else if (data.playersInGame() == 3 && firstInRound == 0) {
			i = data.indexOfDealer;
		} else if (firstInRound == 1) {
			i = data.nextInRound((data.indexOfSB -1 + data.players() ) % data.players());
		} else if (data.playersInGame() > 3 && firstInRound == 3) {
			i = data.nextInRound(data.indexOfBB);
		} else {
			i = -1;
			System.out.println("DUPA i = " + i);
		}
		
		while (!Tools.endOfRound(data)) {
			wasBet = playerMove(i, wasBet);
			i = data.nextInRound(i);
		}
	}
	
	public boolean playerMove(int i, boolean wasBet) {
		data.sendToAll(""+i);
		
		String message;
		try {
			message = data.call(i);
		} catch (IOException e) {
			System.out.println("Client "+ i +" disconnected");
			data.cheater(i, true);
			return wasBet;
		}
		
		if (message.length() == 0) {
			System.out.println("empty message");
			data.cheater(i, true);
			return wasBet;
		}
		
		boolean cheat = false;
		
		int bid = 0;
		switch (message.substring(0, 1)) {
		case("C"):
			if (data.betOf(i) != data.bet())
				cheat = true;
			break;
		case("B"):
			if (wasBet)
				cheat = true;
			else {
				try {
					bid = Integer.parseInt(message.substring(1));
					bid = bid + data.bet() - data.betOf(i);
				} catch (NumberFormatException ex) {cheat = true; break;}
				wasBet = true;
			}
			break;
		case("R"):
			if (!wasBet)
				cheat = true;
			else {
				try {
					bid = Integer.parseInt(message.substring(1));
					bid = bid + data.bet() - data.betOf(i);
				} catch (NumberFormatException ex) {cheat = true; break;}
			}
			break;
		case("K"):
			if (data.betOf(i) >= data.bet())
				cheat = true;
			else
				bid = data.bet() - data.betOf(i);
			break;
		case("F"):
			data.fold(i);
			break;
		case("A"):
			if (data.betOf(i) + data.getCash(i) > data.bet())
				cheat = true;
			else
				bid = data.allIn(i);
			break;
		default:
			cheat = true;
		}

		if (cheat) {
			data.cheater(i, true);
		} else {
			try {
				data.bid(i, bid);
				data.setStatus(i, message.substring(0, 1));
			} catch (BadugiException e) {
				System.out.println(e.getMessage());
				data.cheater(i, true);
				return wasBet;
			}
			data.sendToAll(i, i + message);
		}
		return wasBet;
	}
	
	/**Runda wymian kart, gracze po kolei wysylaja, ktore karty chca wymienic*/
	void exchange() {
		int i = data.nextInExchange((data.indexOfSB -1 + data.players() ) % data.players());
		int counter = 0;
		while (!Tools.endOfExchange(data, counter)) {
			data.sendToAll("E" + i);

			String cards;
			try {
				cards = data.call(i);
			} catch (IOException e) {
				System.out.println("Client "+ i +" disconnected");
				data.cheater(i, true);
				continue;
			}

			int numOfCards = cards.length();
			if (numOfCards > 0 && numOfCards <= 8 && numOfCards % 2 == 0) {
				ArrayList<Card> toEx = new ArrayList<Card>();
				for (int j=0; j < numOfCards; j+=2) {
						Card temp = new Card(cards.substring(j, j+2));
					toEx.add(temp);
				}
				numOfCards = numOfCards / 2;
				ArrayList<Card> newCards = data.takeCards(numOfCards);
				
				try {
					data.exchange(i, toEx, newCards);
				} catch (BadugiException e) {
					data.cheater(i, true);
					data.dumpCards(newCards);
					i = (i+1) % data.players();
					continue;
				}

				data.sendTo(i, Tools.cardsToString(data.getCards(i)));
			} else if (numOfCards == 0) { 
				data.sendTo(i, Tools.cardsToString(data.getCards(i)));
			} else {
				data.cheater(i, true);
				i = data.nextInExchange(i);
				continue;
			}
			
			data.sendToAll(i, i +""+ numOfCards);
			
			i = data.nextInExchange(i);
			counter++;
		}
	}
	
	/**Zakonczenie rozgrywki, porownanie kart i wyslanie do graczy kto i ile wygral.<br>
	 * Wyrzucenie graczy, ktorzy nie maja zetonow
	 * @throws BadugiException Gdy zostal jeden gracz - zwyciezca gry
	 */
	void endOfGame() throws BadugiException {
			ArrayList<ArrayList<Card>> hands = new ArrayList<ArrayList<Card>>();
			for (int i=0; i < data.players(); i++) {
				ArrayList<Card> c = data.dumpCards(i);
				if (c != null)
					data.dumpCards(c);
				hands.add(c);
			}
			int winner;
			String result = Tools.compareHands(hands);
			try {
				winner = Integer.parseInt(result.substring(0, 1));
			} catch (NumberFormatException ex) { throw new BadugiException("blad254"); }
			if (winner == -1) {
				data.sendToAll("D0");
			} else {
				int pot = data.getPot(winner);
				data.sendToAll("W"+ winner + pot);
				data.sendToAll(result.substring(1));
				data.addCash(winner, pot);
			}
			
			for (int i=0; i < data.players(); i++) {
				if ((data.cheater(i) || data.getCash(i) == 0 ) && !data.status(i).equals("X")) {
					data.kickout(i);
				}
			}
			if (data.playersInGame() == 1) {
				int i = 0;
				while (data.status(i).equals("X"))
					i++;
				
				data.kickout(i);
				throw new BadugiException("last player, winner: " + i);
			}
			
			data.waitForPlayers();
			data.moveRoles();
			data.resetBets();
			data.resetPlayersNumber();
	}
	
	/**Dodanie graczy do danych
	 * @param clients Gracze
	 */
	public void addClients(ArrayList<Client> clients) {
		data.addClients(clients);
	}
	
	/**Glowna metoda, otwiera serwer, laczy graczy i obsluguje rozgrywke
	 * @param args Parametry poczatkowe
	 */
	public static void main(TableServer table, int players, int startCash) {

		ArrayList<Client> clients = new ArrayList<Client>();
		
		for (int id = 0; id < players; id++) {
			clients.add(new Client(id, table, startCash));
			System.out.println("polaczono");
		}
		table.addClients(clients);
		
		table.setUpTable();
		boolean firstGame = true;
		while (table.data.playersInGame() > 1) {
			table.setUpGame(firstGame);
			firstGame = false;
			System.out.println("runda1");
			table.round(3);
			table.exchange();
			System.out.println("runda2");
			table.round(1);
			table.exchange();
			System.out.println("runda3");
			table.round(1);
			System.out.println("runda4");
			table.exchange();
			table.round(1);
			try {
				table.endOfGame();
			} catch (BadugiException e) {
				System.out.println(e.getMessage());
				break;
			}
		}
		
		table.finalize();
	}
}
