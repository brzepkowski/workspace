package models.server;

import java.util.ArrayList;
import java.util.Collections;

/**Klasa definiujaca talie kart i stos kart odrzuconych
 * @author Kornel Mirkowski
 *
 */
public class Deck {
	/**Talia kart*/
	private ArrayList<Card> deck = new ArrayList<Card>();
	/**Stos odrzuconych kart*/
	private ArrayList<Card> dump = new ArrayList<Card>();
	
	/**Konstruktor, tworzy karty i tasuje talie*/
	public Deck() {
		deck.add(new Card("As"));deck.add(new Card("2s"));deck.add(new Card("3s"));deck.add(new Card("4s"));deck.add(new Card("5s"));deck.add(new Card("6s"));deck.add(new Card("7s"));deck.add(new Card("8s"));deck.add(new Card("9s"));deck.add(new Card("0s"));deck.add(new Card("Js"));deck.add(new Card("Qs"));deck.add(new Card("Ks"));
		deck.add(new Card("Ah"));deck.add(new Card("2h"));deck.add(new Card("3h"));deck.add(new Card("4h"));deck.add(new Card("5h"));deck.add(new Card("6h"));deck.add(new Card("7h"));deck.add(new Card("8h"));deck.add(new Card("9h"));deck.add(new Card("0h"));deck.add(new Card("Jh"));deck.add(new Card("Qh"));deck.add(new Card("Kh"));
		deck.add(new Card("Ad"));deck.add(new Card("2d"));deck.add(new Card("3d"));deck.add(new Card("4d"));deck.add(new Card("5d"));deck.add(new Card("6d"));deck.add(new Card("7d"));deck.add(new Card("8d"));deck.add(new Card("9d"));deck.add(new Card("0d"));deck.add(new Card("Jd"));deck.add(new Card("Qd"));deck.add(new Card("Kd"));
		deck.add(new Card("Ac"));deck.add(new Card("2c"));deck.add(new Card("3c"));deck.add(new Card("4c"));deck.add(new Card("5c"));deck.add(new Card("6c"));deck.add(new Card("7c"));deck.add(new Card("8c"));deck.add(new Card("9c"));deck.add(new Card("0c"));deck.add(new Card("Jc"));deck.add(new Card("Qc"));deck.add(new Card("Kc"));

		Collections.shuffle(deck);
	}
	
	/**Metoda zwracajaca wielkosc talii
	 * @return Wielkosc talii
	 */
	public int size() {
		return deck.size();
	}
	
	public ArrayList<Card> sizee() {
		ArrayList<Card> res = new ArrayList<Card>();
		if (deck.size() > 0)
			res.addAll(deck);
		if (dump.size() > 0)
			res.addAll(dump);
		return res;
	}
	
	/**Wziecie jednej karty z talii
	 * Jesli brak kart, tasowany jest stos odrzuconych
	 * @return Karta z talii
	 */
	public Card take() {
		if (size() == 0) {
			deck.addAll(dump);
			Collections.shuffle(deck);
			dump.clear();
		}
		
		Card tmp = deck.get(0);
		deck.remove(0);
		return tmp;
	}

	/**Wrzucenie karty na stos
	 * @param c Karta do odrzucenia
	 */
	public void dump(Card c) {
		dump.add(c);
	}
	
	/**Wrzucenie kart na stos
	 * @param list Karty do odrzucenia
	 */
	public void dump(ArrayList<Card> list) {
		dump.addAll(list);
	}
}
