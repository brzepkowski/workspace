package models.server;
import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.concurrent.Callable;

/**Klasa definiujaca gracza, przechowuje jego karty i dane<br>
 * Implementuje {@link Callable}, jako pobieranie informacji od gui
 * @see Callable
 * @author Kornel Mirkowski
 */
public class Client implements Callable<String> {
	/**Gniazdko sieciowe*/
	private Socket client = null;
	/**Strumien wejscia*/
	private BufferedReader in = null;
	/**Strumien wyjscia*/
	private PrintWriter out = null;
	/**Pobrana linia wiadomosci*/
	private String line = "";
	
	/**Id gracza*/
	private final int id;
	/**Karty gracza*/
	private ArrayList<Card> cards;
	/**Aktualne zetony*/
	private int cash;
	/**Aktulanie postawiona stawka - zaklad*/
	private int currBet = 0;
	/**Rola gracza, blindy, dealer itp*/
	private int role;
	/**Aktualny status, czyli poprzednie zagranie (F fold, B bet itp)*/
	private String status = "";
	/**Czy gracz jest aktywny w rozgrywce*/
	private boolean inPlay = true;
	/**Czy gracz gra (nie przegral calkowicie lub nie zostal wyrzucony za oszustwo)*/
	private boolean inGame = false;
	/***/
	private boolean cheater = false;
	
	/**Konstruktor klienta
	 * @param id Id gracza
	 * @param table Stol/Serwer
	 * @param cash Poczatkowe zetony
	 */
	public Client(int id, TableServer table, int cash) {
		client = table.serverAccept();
		try {
			in = new BufferedReader(new InputStreamReader(client.getInputStream()));
			out = new PrintWriter(client.getOutputStream(), true);
		} catch (IOException e) { System.out.println("Accept failed"); System.exit(-1); }

		this.cash = cash;
		this.id = id;
	}
	
	/**Pobranie wiadomosci od gracza
	 * @see java.util.concurrent.Callable#call()
	 */
	@Override
	public String call() throws IOException {
		try {
			System.out.println("teraz "+id);
			line = in.readLine();
		} catch (IOException e) { throw e; }
		return line;
	}

	/**Dodanie zetonow
	 * @param value Liczba zetonow
	 */
	public void addCash(int value) {
		cash += value;
	}
	
	/**Zagranie stawki
	 * @param value Stawka
	 */
	public void bid(int value) {
		cash = cash - value;
		currBet = currBet + value;
	}

	public void cheater(boolean cheater) {
		this.cheater = cheater;
	}
	
	public boolean cheater() {
		return cheater;
	}
	
	/**Zamkniecie strumieni i serwera*/
	public void close() {
		try {
			in.close();
			out.close();
			client.close();
		} catch (IOException e) {
			System.out.println("Could not close."); System.exit(-1);
		}
	}
	/**Wyrzucenie kart
	 * @return Karty gracza
	 */
	public ArrayList<Card> dumpCards() {
		ArrayList<Card> tempCards = new ArrayList<Card>();
		if (cards != null)
			tempCards.addAll(cards);
		else
			return null;
		cards = null;
		return tempCards;
	}
	/**Wymiana kart
	 * @param toEx Karty do wymiany
	 * @param newCards Nowe karty
	 * @return Karty gracza po wymianie
	 * @throws BadugiException Gdy gracz podal karty inne niz mial
	 */
	public void exchange(ArrayList<Card> toEx, ArrayList<Card> newCards) throws BadugiException {
		int n = 4;
		for (Card x : toEx) {
			for (int j=0; j < n; j++) {
				if (x.equals(cards.get(j))) {
					cards.remove(j--);
					n--;
				}
			}
		}
		cards.addAll(newCards);
		if (cards.size() > 4) {
			throw new BadugiException("wrong cards!");
		}
		cards.sort(null);
		this.setCards(cards);
		return;
	}
	
	/**Zagranie fold, oddanie kart i opuszczenie rozgrywki
	 * @return Karty gracza
	 */
	public ArrayList<Card> fold() {
		inPlay = false;
		ArrayList<Card> tempCards = new ArrayList<Card>();
		tempCards.addAll(cards);
		cards = null;
		return tempCards;
	}
	/**Zwracany jest zaklad gracza
	 * @return Zaklad gracza
	 */
	public int getBet() {
		return currBet;
	}
	/**Zwracane karty gracza
	 * @return Karty gracza
	 */
	public ArrayList<Card> getCards() {
		return cards;
	}
	/**Zwracane zetony gracza
	 * @return Zetony gracza
	 */
	public int getCash() {
		return cash;
	}
	/**Zwracane Id gracza
	 * @return Id gracza
	 */
	public int getId() {
		return id;
	}
	/**Zwracana rola gracza (blind dealer itp)
	 * @return Rola gracza
	 */
	public int getRole() {
		return role;
	}
	
	/**Zwracany status gracza (poprzednie zagranie)
	 * @return Status gracza
	 */
	public String getStatus() {
		return status;
	}
	/**Czy gracz jest w grze (nie przegral calkowicie)
	 * @return Czy gracz jest w grze
	 */
	public boolean inGame() {
		return inGame;
	}
	/**Ustawienie, czy gracz jest w grze
	 * @param newVal Czy gracz jest w grze
	 */
	public void inGame(boolean newVal) {
		inGame = newVal;
	}
	/**Czy gracz jest w rozgrywce (nie dal fold)
	 * @return Czy gracz jest w rozgrywce
	 */
	public boolean isActive() {
		return inPlay;
	}
	/**Resetuje zaklad gracza*/
	public void resetBet() {
		currBet = 0;
	}
	/**Resetuje zetony gracza*/
	public void resetCash() {
		cash = 0;
	}
	/**Wyslanie wiadomosci do gracza
	 * @param message Wysylana wiadomosc
	 */
	public void send(String message) {
		System.out.println(id+">> "+message + " >>");
		out.println(message);
	}
	/**Ustawienie kart graczowi
	 * @param cards Karty gracza
	 */
	public void setCards (ArrayList<Card> cards) {
		cards.sort(null);
		this.cards = cards;
	}
	/**Ustawienie roli gracza
	 * @param role Nowa rola gracza
	 */
	public void setRole(int role) {
		this.role = role;
	}
	
	/**Ustawienie statusu gracza
	 * @param status Nowy status gracza
	 */
	public void setStatus(String status) {
		this.status = status;
	}
}
