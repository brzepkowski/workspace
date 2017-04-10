package models.server;

import java.util.ArrayList;
import java.util.Collections;

/**Pomocnicze metody dla serwera
 * @author Kornel Mirkowski
 */
public class Tools {
	/**Pomocnicza klasa, przechowujaca karty, ilosc kart i id gracza.
	 * Porownuje karty wg schematu.
	 * @author Kornel Mirkowski
	 */
	static class Hand implements Comparable<Hand> {
		/**Karty gracza*/
		private ArrayList<Card> cards = new ArrayList<Card>();
		/**Id gracza*/
		private int id;
		/**Ilosc kart*/
		private int number;
		/**Konstruktor
		 * @param cards Karty gracza
		 * @param id Id gracza
		 */
		Hand(ArrayList<Card> cards, int id) { this.cards = cards; this.id = id; number = cards.size(); }
		/**Zwracanie kart
		 * @return Karty
		 */
		ArrayList<Card> getCards() { return cards; }
		/**Zwracanie Id
		 * @return Id gracza
		 */
		int getId() { return id; }
		/**Reprezentacja klasy jako String
		 * @see java.lang.Object#toString()
		 */
		public String toString() { return id + cards.toString(); }
		/**Porownywanie rak 2 graczy wg schematu (zaczynamy od najwiekszej itd)
		 * @see java.lang.Comparable#compareTo(java.lang.Object)
		 * @param o Reka drugiego gracza
		 */
		@Override
		public int compareTo(Hand o) {
			for (int i=0; i < number; i++) {
				if (cards.get(i).compareTo(o.getCards().get(i)) == 0)
					continue;
				else
					return cards.get(i).compareTo(o.getCards().get(i));
			}
			return 0;
		}
	}
	
	/**Metoda odrzuca powtorzone karty z reki gracza, zostawiajac najlepszy uklad
	 * @param cards Karty gracza
	 * @return Najlepszy uklad kart gracza
	 */
	public static ArrayList<Card> bestHand (ArrayList<Card> cards) {
		cards.sort(null);
		if (cards.get(0).sameRankAs(cards.get(1)) && cards.get(0).sameRankAs(cards.get(2)) && cards.get(0).sameRankAs(cards.get(3))) {
			cards.remove(1);cards.remove(1);cards.remove(1);
		} else if (cards.get(0).sameRankAs(cards.get(1)) && cards.get(0).sameRankAs(cards.get(2))) {
			if (cards.get(0).sameSuitAs(cards.get(3))) {
				cards.remove(0); cards.remove(0);
			} else {
				cards.remove(1); cards.remove(1);
			}
		} else if (cards.get(1).sameRankAs(cards.get(2)) && cards.get(1).sameRankAs(cards.get(3))) {
			if (cards.get(1).sameSuitAs(cards.get(0))) {
				cards.remove(1); cards.remove(1);
			} else {
				cards.remove(2); cards.remove(2);
			}
		} else {
			boolean rotated = false;
			boolean finalRotated = false;
			int n=3;
			for (int i=0; i < n; i++) {
				for (int j=i+1; j <= n; j++) {
					if (cards.get(i).sameSuitAs(cards.get(j)) && !(cards.get(i).sameRankAs(cards.get(j)))) {
						cards.remove(j);
						n--;
						j--;
					} else if ((finalRotated || rotated) && i != 1 && cards.get(i).eliminates(cards.get(j))) {
						cards.remove(j);
						n--;
						j--;
					} else if (cards.get(i).sameRankAs(cards.get(j)) && !finalRotated) {
						if (i == n-1 && j == n) {
							cards.remove(j);
							n--;
						} else if (cards.size() > j+1 && cards.get(j+1).sameRankAs(cards.get(j))) {
							Collections.rotate(cards, 1);
							finalRotated = true;
						} else if (cards.size() > i+2) {
							Collections.rotate(cards.subList(i, i+3), 1);
							rotated = true;
						}
						j--;
					}
				}
			}
		}
			
		cards.sort(null);
		return cards;
	}
	
	/**Porownywanie kart graczy
	 * @param args Lista kart poszczegolnych graczy
	 * @return Id gracza ktory zwyciezyl i karty innych
	 */
	public static String compareHands(ArrayList<ArrayList<Card>> args) {
		ArrayList<Hand> hands = new ArrayList<Hand>();
		int i = 0;
		int currMax = 0;
		String result = "";
		
		boolean firstCards = true;
		
		for(ArrayList<Card> temp : args) {
			if (temp != null) {
				temp = Tools.bestHand(temp);
				temp.sort(new Card.ReverseComparator());
				if (firstCards) {
					result = cardsToString(temp);
					firstCards = false;
				} else {
					result += "\n" + cardsToString(temp);
				}
				if (temp.size() == currMax)
					hands.add(new Hand(temp,i));
				else if (temp.size() > currMax) {
					hands.clear();
					hands.add(new Hand(temp,i));
					currMax = temp.size();
				}
			} else {
				if (firstCards) {
					result = "X";
					firstCards = false;
				} else {
					result += "\nX";
				}
			}
			i++;
		}
		
		hands.sort(null);
		if (hands.size() > 1 && hands.get(0).compareTo(hands.get(1)) == 0) {
			return "-1";
		} else		
			return hands.get(0).getId() + result;
	}
	
	/**Konwersja {@link ArrayList} kart na String 
	 * @param cards Lista kart do konwersji
	 * @return Reprezentacja kart w String
	 */
	public static String cardsToString(ArrayList<Card> cards) {
		String result = "";
		for (Card x : cards)
			result += x.toString();
		return result;
	}
	
	
	/**Metoda okresla czy runda powinna sie skonczyc
	 * @param data Klasa z danymi gry
	 * @return Czy runda ma si� zakonczyc
	 */
	public static boolean endOfRound (Data data) {
		//co jest jest 2 graczy i jeden dal all-in
		for (int i=0; i <data.players(); i++) {
			System.out.print(i +"."+ data.betOf(i));
			System.out.print(data.status(i) +"\t");
			System.out.println(data.getCash(i));
		}
		
		if (data.playersInPlay() == 1)
			return true;
		else /*if (data.playersInPlay() == 2)*/ {
			int c = 0;
			int d = 0;
			for (int i=0; i < data.players(); i++) {
				if (data.status(i).equals("A"))
					c++;
				if (data.status(i).equals("F") || data.status(i).equals("A"))
					d++;
			}
			if (c == data.playersInPlay() - 1 || d == data.playersInGame() -1)
				return true;
			
			for (int i=0; i < data.players(); i++) {
				if (data.status(i).equals("") || (data.betOf(i) != data.bet()
					&& !data.status(i).equals("F") && !data.status(i).equals("A")
					&& !data.status(i).equals("X") )) {
					return false;
				}
			}
		}
		return true;
	}
	
	public static boolean endOfExchange (Data data, int counter) {
		return !(data.playersInPlay() > 1 && counter < data.playersInPlay());
	}
}
