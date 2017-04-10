package models.server;

import java.util.Comparator;

/**Klasa definiujaca karte w grze, implementuje interfejs {@link Comparable}, aby mozna bylo je porownywac
 * @author Kornel Mirkowski
 * @see java.util.Comparator
 */
public class Card implements Comparable<Card> {	 // NOPMD by korne_000 on 02.11.14 17:47
	/**Wartosc karty, potrzebne do porownywania*/
	private final int value;
	/**Figura karty*/
	private final String rank;
	/**Kolor karty*/
	private final String suit;
	
	/**Konstruktor karty
	 * @param code Kod karty, np Js - Jack Spades
	 */
	public Card(String code) {
		int val = -1;
		String ran = code.substring(0, 1);
		try {
			val = Integer.parseInt(ran);
			if (val == 0)
				val = 10;
		} catch (NumberFormatException ex) {
			switch(ran) {
				case ("A"):
					val = 1;
					break;
				case ("J"):
					val = 11;
					break;
				case ("Q"):
					val = 12;
					break;
				case ("K"):
					val = 13;
					break;
			}
		}
		this.rank = code.substring(0, 1);
		this.suit = code.substring(1, 2);
		this.value= val;
	}
	
	/**Zwraca kod karty
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return rank+suit;
	}
	/**Zwraca wartosc karty
	 * @return wartosc karty
	 */
	public int getValue() {
		return value;
	}
	/**Zwraca kolor karty
	 * @return Kolor karty
	 */
	public String getSuit() {
		return suit;
	}
	/**Zwraca figure karty
	 * @return Figura karty
	 */
	public String getRank() {
		return rank;
	}

	/**Porownywanie kart po wartosciach
	 * @param o karta do porownania
	 * @see java.lang.Comparable#compareTo(java.lang.Object)
	 */
	@Override
	public int compareTo(Card o) {
		Integer thisValue = value;
		return thisValue.compareTo(o.getValue());
	}

	/**Okresla czy karta pierwsza eliminuje druga (ten sam kolor lub figura)
	 * @param o Karta do porownania
	 * @return czy karta eliminuje druga
	 */
	public boolean eliminates(Card o) {
		return sameSuitAs(o) || sameRankAs(o);
	}
	/**Okresla czy karty maja ten sam kolor
	 * @param o Druga karta
	 * @return czy karty maja taki sam kolor
	 */
	public boolean sameSuitAs(Card o) {
		return suit.compareTo(o.suit) == 0;
	}
	/**Okresla czy karty maja ta sama figure
	 * @param o Druga karta
	 * @return czy karty maja taka sama figur
	 */
	public boolean sameRankAs(Card o) {
		return rank.compareTo(o.rank) == 0;
	}
	/**Okresla czy karty sa takie same
	 * @param c Druga karta
	 * @return czy karty sa takie same
	 */
	public boolean equals(Card c) {
		return rank.equals(c.getRank()) && suit.equals(c.getSuit());
	}

	/**Pomocniczy komparator, do odwrotnego sortowania kart
	 * @author Kornel Mirkowski
	 */
	public static class ReverseComparator implements Comparator<Card> {
		/**Metoda porownujaca karty
		 * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
		 * @param arg0 pierwsza karta
		 * @param arg1 druga karta
		 */
		@Override
		public int compare(Card arg0, Card arg1) {
			return -(arg0.compareTo(arg1));
		}
    }
}
