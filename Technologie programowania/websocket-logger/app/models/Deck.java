package models;

import models.msgs.Card;

public class Deck 
{
	static String[] ranks = {"2", "3", "4" ,"5", "6", "7", 
									 "8", "9", "10", "JACK", "QUEEN", "KING", "AS"};
	static String[] suits = {"CLUB", "DIAMOND", "SPADE", "HEART"};
	
	static Card giveCard(int nr) 
	{
		int r = nr % 13;
		int s = nr % 4;		
		return new Card(ranks[r], suits[s]);
	}
	
}
