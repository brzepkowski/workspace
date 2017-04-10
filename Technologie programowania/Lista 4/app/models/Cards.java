import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;



public class Cards {

	/**
	 * array of cards = deck
	 * clubs, diamonds, hearts, spades
	 */
	
	// "1" instead of "10" to make every String the same length
	static String[] deck = new String[]{"2c","3c","4c","5c","6c","7c","8c","9c","1c","jc","qc","kc","ac","2d","3d","4d","5d","6d","7d","8d","9d","1d","jd","qd","kd","ad","2h","3h","4h","5h","6h","7h","8h","9h","1h","jh","qh","kh","ah","2s","3s","4s","5s","6s","7s","8s","9s","1s","js","qs","ks","as"};
	public static int cardsInGame = 0;
	static String[] cards1= new String[4];
	static String[] cards2= new String[4];
	static String[] cards3= new String[4];
	static String[] cards4= new String[4];
	static String[] cards5= new String[4];
	static String[] cards6= new String[4];

	static ArrayList<String> shuffledDeck = new ArrayList<>();
	
	private static void sDeck (){
		shuffledDeck = new ArrayList<String>(Arrays.asList(deck));
		Collections.shuffle(shuffledDeck);
	}
	
	/**
	 * shuffles deck and creates arraylist with shuffled cards ready for playing
	 */
	public static void shuffleDeck(){
		sDeck();
		System.out.println("The deck has been shuffled");
	}
	
	private static String[] gCards(int howMany){
		String[] orderedCards = new String[howMany];
		for(int i=0; i<howMany; i++){
			orderedCards[i]=shuffledDeck.get(0);
			shuffledDeck.remove(0);
		}
		
		return orderedCards;
		
	}
	
	public static synchronized String[] getCards (int howMany){
		if(howMany==1) {
			//System.out.println("Returned "+ howMany+" card");
		} else {
		//	System.out.println("Returned "+ howMany+" cards");
		}//System.out.println("");
		return gCards(howMany);
		
	}
	
	private static void rCards (String[] returned){
		for(int i=0; i<returned.length; i++){
			shuffledDeck.add(returned[i]);
		}
	}
	
	public static synchronized void returnCards(String[] returned){
		rCards(returned);
		System.out.println("Returned "+returned.length+" cards, "+shuffledDeck.size()+" left in deck");
	}
}
