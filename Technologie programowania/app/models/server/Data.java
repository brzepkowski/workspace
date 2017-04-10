package models.server;

import java.io.IOException;
import java.util.ArrayList;

/**Klasa z danymi gry
 * @author Kornel Mirkowski
 */
public class Data {
	/**Lista klientow*/
	private ArrayList<Client> clients = new ArrayList<Client>();
	/**Talia kart*/
	private Deck deck;
	/**Aktualny najwyzszy zaklad*/
	private int currentBet;
	/**Duza ciemna*/
	private int bigBlind;
	/**Mala ciemna*/
	private int smallBlind;
	/**Poczatkowe zetony graczy*/
	private int startCash;
	/**Liczba graczy*/
	private int players;
	/**Liczba graczy w rozgrywce*/
	private int playersInPlay;
	/**Liczba graczy w grze*/
	private int playersInGame;
	
	public int indexOfDealer;
	public int indexOfSB;
	public int indexOfBB;
	
	/**Konstruktor przypisuje wartosci pol
	 * @param players Liczba graczy
	 * @param startCash Poczatkowa liczba zetonow
	 * @param bigBlind Duza ciemna
	 */
	Data (int players, int startCash, int bigBlind) {
		this.players = players;
		playersInPlay = players;
		playersInGame = players;
		this.startCash = startCash;
		this.bigBlind = bigBlind;
		smallBlind = bigBlind/2;
		currentBet = bigBlind;
	}
	
	/**Dodanie zetonow graczowi
	 * @param index Id gracza
	 * @param cash Zetony do dodania
	 */
	public void addCash(int index, int cash) {
		clients.get(index).addCash(cash);
	}
	/**Dodanie graczy
	 * @param clients Gracze
	 */
	public void addClients(ArrayList<Client> clients) {
		this.clients = clients;
	}
	
	
	
	
	
	/**Wziecie wszystkich zetonow przy zagraniu all-in
	 * @param index Id gracza
	 * @return Zetony gracza
	 */
	public int allIn(int index) {
		return clients.get(index).getCash();
	}
	
	/**Wysokosc big blind
	 * @return Sysokosc big blind
	 */
	public int bb() {
		return bigBlind;
	}
	
	/**Aktualny najwiekszy zaklad
	 * @return Najwiekszy zaklad
	 */
	public int bet() {
		return currentBet;
	}
	
	/**Zaklad gracza
	 * @param index Id gracza
	 * @return Zaklad gracza
	 */
	public int betOf(int index) {
		return clients.get(index).getBet();
	}
	
	/**Postawienie stawki
	 * @param index Id gracza
	 * @param value Wartosc stawki
	 * @throws BadugiException Gdy nie ma odpowiedniej ilosci zetonow
	 */
	public void bid(int index, int value) throws BadugiException {
		if (value > clients.get(index).getCash() || value < 0) {
			throw new BadugiException("not enough cash: " + index);
		}
		clients.get(index).bid(value);
		if (clients.get(index).getBet() > currentBet)
			currentBet = clients.get(index).getBet();
	}
	
	/**Wywolanie call gracza, czyli pobranie wiadomosci
	 * @param index Id gracza
	 * @return Wiadomosc od klienta
	 * @throws IOException Gdy klient sie rozlaczy
	 * @see java.util.concurrent.Callable#call()
	 */
	public String call(int index) throws IOException {
		String result = "";
		try {
			result = clients.get(index).call();
		} catch (IOException e) { throw e; }
		System.out.println(index +"<< "+ result+" <<");
		return result;
	}
	
	public boolean cheater(int index) {
		return clients.get(index).cheater();
	}
	
	public void cheater(int index, boolean cheater) {
		playerOut();
		setStatus(index, "F");
		sendToAll(index, "S" + index);
		deck.dump(clients.get(index).fold());
		clients.get(index).cheater(cheater);
	}
	
	public ArrayList<Card> deckSizee() {
		return deck.sizee();
	}
	
	/**Wrzucenie kart na stos
	 * @param list Karty do wrzucenia
	 */
	public void dumpCards(ArrayList<Card> list) {
		deck.dump(list);
	}
	
	/**Wziecie kart od gracza
	 * @param index Id gracza
	 * @return Karty gracza
	 */
	public ArrayList<Card> dumpCards(int index) {
		return clients.get(index).dumpCards();
	}
	
	/**Wymiana kart gracza
	 * @param index Id gracza
	 * @param toEx Karty do wymiany
	 * @param newCards Nowe karty
	 * @return Wszystkie karty gracza
	 * @throws BadugiException Gdy podane sa zle karty
	 */
	public void exchange(int index, ArrayList<Card> toEx, ArrayList<Card> newCards) throws BadugiException {
		clients.get(index).exchange(toEx, newCards);
		deck.dump(toEx);
		return;
	}
	/**Fold gracza
	 * @param index Id gracza
	 */
	public void fold(int index) {
		playerOut();
		deck.dump(clients.get(index).fold());
	}
	
	/**Zwracanie kart Gracza
	 * @param index Id gracza
	 * @return Karty gracza
	 */
	public ArrayList<Card> getCards(int index) {
		return clients.get(index).getCards();
	}
	
	/**Zwracanie zetonow gracza
	 * @param index Id gracza
	 * @return Zetony gracza
	 */
	public int getCash(int index) {
		return clients.get(index).getCash();
	}
	/**Ilosc graczy w grze
	 * @return Ilosc graczy w grze
	 */
	public int getPlayersInGame() {
		return playersInPlay;
	}
	
	/**Zwracana laczna pula zakladow przy wygranej.<br>
	 * Jesli wygral gracz z all-in, nie dostanie wiecej niz postawil
	 * @param index Id gracza
	 * @return Zetony wygrane przez gracza
	 */
	public int getPot(int index) {
		int pot = 0;
		for (Client x : clients)
			pot += x.getBet();
		
		/*if (players*betOf(index) < pot)
			pot = players*betOf(index);*/
		
		return pot;
	}
	
	/**Zwracanie poczatkowej liczby zetonow
	 * @return Poczatkowa liczba zetonow
	 */
	public int getStartCash() {
		return startCash;
	}
	/**Wyrzucanie gracza za oszustwo lub brak zetonow
	 * @param index Id gracza
	 */
	public void kickout(int index) {
		System.out.println("usuwamy "+index);
		clients.get(index).inGame(false);
		playersInGame--;
		playersInPlay--;
		clients.get(index).close();
		clients.get(index).resetCash();
		clients.get(index).setRole(-1);
		setStatus(index, "X");
		
		//sendToAll(index, index + "X");
		//sendToAll(index, "S" + index);
	}
	/**Stworzenie talii {@link Deck}*/
	public void makeDeck() {
		deck = new Deck();
	}
	/**Przesuwanie rol (blindy, dealer) po zakonczenie rundy.<br>
	 * O 1 w lewo z pominieciem przegranych*/
	public void moveRoles() {
		for (Client x : clients)
			System.out.println(x.getId() + "\t"+ x.getRole() + "\t"+ x.getStatus());
		
		
		int i;
		if (indexOfDealer == -1)
			i = (indexOfBB + 1) % players;
		else
			i= (indexOfDealer + 1) % players;
		
		while (status(i).equals("X"))
			i = (i+1) % players;
			
		if (playersInGame == 2) {
			indexOfDealer = -1;
			setRole(i, 2);
			i = (i + 1) % players;
			while (status(i).equals("X"))
				i = (i+1) % players;
			
			setRole(i, 1);
		} else {
			int role = 0;
			
			for (int x=0; x < playersInGame; x++) {
				setRole(i, role++);
				i = (i+1)%players;
				while (status(i).equals("X"))
					i = (i+1)%players;
			}
		}
		for (Client x : clients)
			System.out.println(x.getId() + "\t"+ x.getRole() + "\t"+ x.getStatus());
		
	}
	public int nextInExchange(int index) {
		int next;
		next = (index + 1) % players;
		
		while (status(next).equals("F") || status(next).equals("X"))
			next = (next + 1) % players;
		
		return next;
	}
	public int nextInRound(int index) {
		int next;
		next = (index + 1) % players;
		
		while (status(next).equals("F") || status(next).equals("A") || status(next).equals("X"))
			next = (next + 1) % players;
		
		return next;
	}
	
	/**Zmniejszenie liczby graczy w rozgrywce (gdy fold)*/
	public void playerOut() {
		playersInPlay--;
	}
	
	/**Liczba graczy
	 * @return Liczba graczy
	 */
	public int players() {
		return players;
	}
	/**Liczba graczy w grze
	 * @return Liczba graczy w grze
	 */
	public int playersInGame() {
		return playersInGame;
	}
	/**Liczba graczy w rozgrywce
	 * @return Liczba graczy w rozgrywce
	 */
	public int playersInPlay() {
		return playersInPlay;
	}
	/**Resetowanie zakladow graczy*/
	public void resetBets() {
		currentBet = 0;
		for (Client x : clients) {
			x.resetBet();
		}
	}
	/**Resetowanie graczy w rozgrywce (gdy ktos odpadl)*/
	public void resetPlayersNumber() {
		playersInPlay = playersInGame;
	}
	/**Zwracanie roli gracza
	 * @param index Id gracza
	 * @return Rola gracza
	 */
	public int role(int index) {
		return clients.get(index).getRole();
	}
	/**Zwracanie small blind
	 * @return Wysokosc small blind
	 */
	public int sb() {
		return smallBlind;
	}
	/**Wyslanie wiadomosci do gracza
	 * @param index Id gracza
	 * @param message Wiadomosc do wyslania
	 */
	public void sendTo(int index, String message) {
		clients.get(index).send(message);
	}
	/**Wyslanie wiadomosci do wszystkich graczy oprocz okreslonego
	 * @param index Id gracza pominietego
	 * @param message Wiadomosc do wyslania
	 */
	void sendToAll(int index, String message) {
		for (Client x : clients) {
			if (x.getId() != index && !x.getStatus().equals("X"))
				x.send(message);
		}
	}
	/**Wyslanie wiadomosci do wszystkich graczy
	 * @param message Wiadomosc do wyslania
	 */
	void sendToAll(String message) {
		for (Client x : clients) {
			if (!x.getStatus().equals("X"))
				x.send(message);
		}
	}
	/**Ustawienie wszsytkich statusow.<br>
	 * W zaleznosci czy rozpoczynamy gre, czy runde licytacji, gracze z fold lub all-in zostaja lub zmieniaja status
	 * @param status Nowy status dla wszystkich
	 * @param newGame Czy rozpoczynamy nowa gre, czy runde licytacji
	 */
	public void setAllStatuses(String status, boolean newGame) {
		for (Client x : clients) {
			if (newGame && !x.getStatus().equals("X"))
				x.setStatus(status);
			else if (!newGame && !x.getStatus().equals("X") && !x.getStatus().equals("F")
					&& !x.getStatus().equals("A"))
				x.setStatus(status);
		}
	}
	/**Ustawienie big blind
	 * @param bigBlind Duza ciemna
	 */
	public void setBB(int bigBlind) {
		this.bigBlind = bigBlind;
	}
	
	/**Ustawienie kart graczowi
	 * @param index Id gracza
	 * @param cards Nowe karty
	 */
	public void setCards(int index, ArrayList<Card> cards) {
		clients.get(index).setCards(cards);
	}
	/**Ustawienie liczby graczy
	 * @param players Liczba graczy
	 */
	public void setNumberOfPlayers(int players) {
		this.players = players;
	}
	/**Ustawienie roli (blindy) graczowi
	 * @param index Id gracza
	 * @param role Nowa rola gracza
	 */
	public void setRole(int index, int role) {
		if (role == 0)
			indexOfDealer = index;
		else if (role == 1)
			indexOfSB = index;
		else if (role == 2)
			indexOfBB = index;
		
		clients.get(index).setRole(role);
	}
	/**Ustawienie small blind
	 * @param smallBlind Mala ciemna
	 */
	public void setSB(int smallBlind) {
		this.smallBlind = smallBlind;
	}
	/**Ustawienie poczatkowej liczby zetonow
	 * @param startCash Poczatkowa liczba zetonow
	 */
	public void setStartCash(int startCash) {
		this.startCash = startCash;
	}
	/**Ustawienie statusu graczowi (ostatnie zagranie)
	 * @param index Id gracza
	 * @param status Nowy status gracza
	 */
	public void setStatus(int index, String status) {
		clients.get(index).setStatus(status);
	}
	/**Laczna pula zakladow graczy
	 * @return Pula na stole
	 */
	public int stake() {
		int result = 0;
		for (Client x : clients) {
			result += x.getBet();
		}
		return result;
	}
	/**Zwracanie statusu gracza
	 * @param index Id gracza
	 * @return Status gracza
	 */
	public String status(int index) {
		return clients.get(index).getStatus();
	}
	/**Wziecie kilku kart z talii {@link Deck}
	 * @param number Liczba kart do wziecia
	 * @return Karty z talii
	 */
	public ArrayList<Card> takeCards(int number) {
		ArrayList<Card> result = new ArrayList<Card>();
		for (; number > 0; number--)
			result.add(deck.take());
		return result;
	}
	
	public void waitForPlayers() {
		for (Client x : clients) {
			if (!x.getStatus().equals("X")) {
				try {
					x.call();
				} catch (IOException e) { kickout(x.getId()); }
			}
		}	
	}
}
