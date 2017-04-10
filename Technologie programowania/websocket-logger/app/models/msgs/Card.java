package models.msgs;

public class Card {
	
	
        final String rank;
        final String color;
	    
	    public String getRank() {
			return rank;
		}
	    
	    public String getColor() {
			return color;
		}

		public Card(String rank, String color) {
	        this.rank = rank;
	        this.color = color;

	    }

}
