import java.util.Random;
 
 
public class Table {
     
    SocketServer server = null;
     
    public int dealer;
    public static int bigBlind;
    public static int smallBlind;
     
     
    public int tourNumber = 1;
    public int auctionRoundNumber = 0;
    //public int cardsRoundNumber = 0;
     
    public int[] tableOfFoldedPlayers;
    public int[] tableOfAllInPlayers;
     
    public Auction auction;
    public GameLogic logic;
    public int numberOfChangingCards = 0;
     
    public int notifiedAuctionPlayers = 0;
    public int notifiedCardPlayers = 0;
     
    Cards cards; 
    public static Bot[] bot;
     
     
    public Table(SocketServer s) {
        this.server = s;
        bot = new Bot[server.bots];
       
         
        //Creation of table of disconnected players
         
        tableOfFoldedPlayers = new int[server.getLimitOfPlayers()];
        tableOfAllInPlayers = new int[server.getLimitOfPlayers()];
        for (int i = 0; i <= SocketServer.getLimitOfPlayers() - 1; i++) {
            tableOfFoldedPlayers[i] = 0;
            tableOfAllInPlayers[i] = 0;
        }
         
        System.out.println("Wszyscy gracze pod��czeni, gra rozpocz�ta!");
        //server.broadcast("UnlockButtons-0-0-0-0-0");
         
        //Drawing Dealer, Small Blind and Big Blind and broadcasting information about it
        dealer = drawDealer(1, server.numberOfPlayers);
        if (server.numberOfPlayers == 2) {
            dealer = drawDealer(1, 2);
            if (dealer + 1 > 2) {
                smallBlind = 1;
            }
            else smallBlind = 2;
            String message = "Status-" + dealer + "-" + smallBlind + "-0-0-0";
            server.broadcast(message);
        }
        else {
            if (dealer + 1 > server.numberOfPlayers) {
                smallBlind = 1;
            }
            else {
                smallBlind = dealer + 1;
            }
            if (server.numberOfPlayers > 2) {
                if (smallBlind + 1 > server.numberOfPlayers) {
                    bigBlind = 1;
                }
                else {
                    bigBlind = smallBlind + 1;
                }
            }
            else {
                bigBlind = 0;
            }
            String message = "Status-" + dealer + "-" + smallBlind + "-" + bigBlind + "-0-0";
            server.broadcast(message);
 
        }
        // Creating object of Cards, shuffling the deck and giving 4 cards to each player
        cards = new Cards();
         
        cards.shuffleDeck();
         
        for (int i = 1; i <= server.numberOfPlayers; i++) {
         
            String[] basicCards = cards.getCards(4);
            String bCards = "BasicCards-" + i + "-" + basicCards[0] + "-" + basicCards[1] + "-" + basicCards[2] + "-" + basicCards[3];
            server.broadcast(bCards);
        }
        auction = new Auction(server, this);
        logic = new GameLogic();
        notifiedAuctionPlayers = 1; // <-- Because in two cases only one person won't have option to move this stays the
                                    //same for both cases (players more than two and equal to two
         
        //The game must be begun by first player on the left from Big Blind
                if (server.getLimitOfPlayers() > 2) {
                    if (bigBlind + 1 > server.getLimitOfPlayers()) {
                        //server.broadcast("UnlockAuctionButtons-1-0-0-0-0");
                        server.broadcast("UnlockAuctionButtons-1-" + auction.someoneBet + "-" + auction.someoneRaised + 
                                "-" + auction.highestBid + "-0");
                        //auction.playerActions(1);
                    }
                    else {
                        //server.broadcast("UnlockAuctionButtons-" + (bigBlind + 1) + "-0-0-0-0");
                        server.broadcast("UnlockAuctionButtons-" + (bigBlind + 1) + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                                "-" + auction.highestBid + "-0");
                        //auction.playerActions(bigBlind + 1);
                    }
                }
                // When there are only 2 players in the game, dealer begins the auction
                else {
                    server.broadcast("UnlockAuctionButtons-" + dealer + "-0-0-0-0");
                    server.broadcast("UnlockAuctionButtons-" + dealer + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                            "-" + auction.highestBid + "-0");
                }
         
         
    }
     
    public void interpreter(String message) {
        System.out.println("Interpreter obs�uzy message: ");
        String[] parts = message.split("-");
        String part1 = parts[0];
        String part2 = parts[1];
        String part3 = parts[2];
        String part4 = parts[3];
        String part5 = parts[4];
        String part6 = parts[5];
         
        if (part1.equals("AnswerFoldStatus")) {
            if (part3 != "0") {
                tableOfFoldedPlayers[Integer.parseInt(part2) - 1] = 1;
                 
                System.out.println("GRACZ 1: -> " + tableOfFoldedPlayers[0]);
                System.out.println("GRACZ 2: -> " + tableOfFoldedPlayers[1]);
                System.out.println("GRACZ 3: -> " + tableOfFoldedPlayers[2]);
            }
        }
        else if (part1.equals("AnswerAllInStatus")) {
            if (part3 != "0") {
                tableOfAllInPlayers[Integer.parseInt(part2) - 1] = 1;
            }
        }
         
        else if (part1.equals("ChangeOfCards")) {
            System.out.println("TABLE--Zamiana: " + part3 + "-" + part4 + "-" + part5 + "-" + part6);
                String cCards = "ChangeOfCards-" + part2 + "-";
                int counter=0;
                for(int i=2; i<6; i++)
                    if(parts[i]!="0")
                        counter++;
                String[] returned = new String[counter];
                for(int j=2, i=0; j<6; j++, i++)
                    if(parts[j]!="0")
                        returned[i]=parts[j];
                cards.returnCards(returned);
                System.out.println("Zamieniono karty");
                 
                if (part3.equals("0") == false && part4.equals("0") == false && part5.equals("0") == false && part6.equals("0") == false) {
                    String[] changedCards = cards.getCards(4);
                    cCards += changedCards[0] + "-" + changedCards[1] + "-" + changedCards[2] + "-" + changedCards[3];
                }
                else if (part3.equals("0") == false && part4.equals("0") == false && part5.equals("0") == false && part6.equals("0")) {
                    String[] changedCards = cards.getCards(3);
                    cCards += changedCards[0] + "-" + changedCards[1] + "-" + changedCards[2] + "-" + "0";
                }
                else if (part3.equals("0") == false && part4.equals("0") == false && part5.equals("0") && part6.equals("0") == false) {
                    String[] changedCards = cards.getCards(3);
                    cCards += changedCards[0] + "-" + changedCards[1] + "-0-" + changedCards[2];
                }
                else if (part3.equals("0") == false && part4.equals("0") == false && part5.equals("0") && part6.equals("0")) {
                    String[] changedCards = cards.getCards(2);
                    cCards += changedCards[0] + "-" + changedCards[1] + "-0-0";
                }
                else if (part3.equals("0") == false && part4.equals("0") && part5.equals("0") == false && part6.equals("0") == false) {
                    String[] changedCards = cards.getCards(3);
                    cCards += changedCards[0] + "-0-" + changedCards[1] + "-" + changedCards[2];
                }
                else if (part3.equals("0") == false && part4.equals("0") && part5.equals("0") == false && part6.equals("0")) {
                    String[] changedCards = cards.getCards(2);
                    cCards += changedCards[0] + "-0-" + changedCards[1] + "-0";
                }
                else if (part3.equals("0") == false && part4.equals("0") && part5.equals("0") && part6.equals("0") == false) {
                    String[] changedCards = cards.getCards(2);
                    cCards += changedCards[0] + "-0-0-" + changedCards[1];
                }
                else if (part3.equals("0") == false && part4.equals("0") && part5.equals("0") && part6.equals("0")) {
                    String[] changedCards = cards.getCards(1);
                    cCards += changedCards[0] + "-0-0-0";
                }
                else if (part3.equals("0") && part4.equals("0") == false && part5.equals("0") == false && part6.equals("0") == false) {
                    String[] changedCards = cards.getCards(3);
                    cCards +=  "0-" + changedCards[0] + "-" + changedCards[1] + "-" + changedCards[2];
                }
                else if (part3.equals("0") && part4.equals("0") == false && part5.equals("0") == false && part6.equals("0")) {
                    String[] changedCards = cards.getCards(2);
                    cCards += "0-" + changedCards[0] + "-" + changedCards[1] + "-0";
                }
                else if (part3.equals("0") && part4.equals("0") == false && part5.equals("0") && part6.equals("0") == false) {
                    String[] changedCards = cards.getCards(2);
                    cCards += "0-" + changedCards[0] + "-0-" + changedCards[1];
                }
                else if (part3.equals("0") && part4.equals("0") == false && part5.equals("0") && part6.equals("0")) {
                    String[] changedCards = cards.getCards(1);
                    cCards += "0-" + changedCards[0] + "-0-0";
                }
                else if (part3.equals("0") && part4.equals("0") && part5.equals("0") == false && part6.equals("0") == false) {
                    String[] changedCards = cards.getCards(2);
                    cCards += "0-0-" + changedCards[0] + "-" + changedCards[1];
                }
                else if (part3.equals("0") && part4.equals("0") && part5.equals("0") == false && part6.equals("0")) {
                    String[] changedCards = cards.getCards(1);
                    cCards += "0-0-" + changedCards[0] + "-0";
                }
                else if (part3.equals("0") && part4.equals("0") && part5.equals("0") && part6.equals("0") == false) {
                    String[] changedCards = cards.getCards(1);
                    cCards += "0-0-0-" + changedCards[0];
                }
                else if (part3.equals("0") && part4.equals("0") && part5.equals("0") && part6.equals("0")) {
                    cCards += "0-0-0-0";
                }
                 
                 
                System.out.println("cCards: " + cCards);
                server.broadcast(cCards);
                server.broadcast("BlockCardButtons-" + part2 + "-0-0-0-0");
                notifiedCardPlayers++;
                if (notifiedCardPlayers == server.getLimitOfPlayers()) {
                    System.out.println("Zakonczono rund� wymiany kart");
                    notifiedCardPlayers = 0;
                    auction.someoneBet = 0;
                    auction.someoneRaised = 0;
                     
                    //server.broadcast("CheckFoldStatus-" + smallBlind + "-0-0-0-0");
                //  server.broadcast("CheckAllInStatus-" + smallBlind + "-0-0-0-0");
                     
                    if (tableOfAllInPlayers[smallBlind - 1] == 1 || tableOfFoldedPlayers[smallBlind - 1] == 1) {
                        notifiedAuctionPlayers++;
                        server.broadcast("UnlockAuctionButtons-" + bigBlind + "-0-0-" + auction.highestBid + "-0");
                        System.out.println("---------------------------------> Wesz�o");
                    }
                    else {
                        System.out.println("--------------------------------->NIE Wesz�o");
                        server.broadcast("UnlockAuctionButtons-" + smallBlind + "-0-0-" + auction.highestBid + "-0");
                    }
                }
                numberOfChangingCards++;
                if (numberOfChangingCards == 3) {
                //Sending question for getting cards values
                for(int i=1; i<SocketServer.getLimitOfPlayers()+1; i++){
                    server.broadcast("GetCardValues-"+i+"-0-0-0-0");
                }
                }
                 
         
        }
         
        else if (part1.equals("LastOffer")) {
            System.out.println("!!!!!!!!PRZED BROADCAST!!!!!!!!!!!!!!!!!!!!!!!");
            System.out.println("TABLE: " + auction.someoneBet + "-" + auction.someoneRaised + 
                    "-" + auction.highestBid + "-0");
            server.broadcast("LastOffer-" + part2 + "-" + part3 + "-0-0-0");
        }
        else if (part1.equals("HighestBid")) {
            server.broadcast("HighestBid-" + part2 + "-" + part3 + "-0-0-0");
            System.out.println("TABLE: " + auction.someoneBet + "-" + auction.someoneRaised + 
                    "-" + auction.highestBid + "-0");
        }
        // Basic method for auctions and above is adding to notifiedCardPlayers
         
        else if (part1.equals("Auction")) {
            notifiedAuctionPlayers++;
             
            auction.basicAuction(message);
             
             
             
            if (notifiedAuctionPlayers < server.getLimitOfPlayers()) {
                server.broadcast("BlockAuctionButtons-" + part2 + "-0-0-0-0");
                int i = Integer.parseInt(part2);
                 
                //checking if someone folded or did All In
                 
                while (i < server.getLimitOfPlayers() && (tableOfFoldedPlayers[i] != 0 || tableOfAllInPlayers[i] != 0) && notifiedAuctionPlayers < server.getLimitOfPlayers()) {
                    notifiedAuctionPlayers++;
                    if (i + 1 > server.getLimitOfPlayers()) {
                        i = 1;
                    }
                    else {
                        i++;
                    }
                    System.out.println("WHILEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");
                }
                 
                 
                 
                //*********************************************************************************************************
                if (i + 1 > server.getLimitOfPlayers()) {
                    //auction.playerActions(1);    //
                    //server.broadcast("UnlockAuctionButtons-1-0-0-0-0");
                    server.broadcast("UnlockAuctionButtons-1-" + auction.someoneBet + "-" + auction.someoneRaised + 
                            "-" + auction.highestBid + "-0");
                    System.out.println("Serwer: UnlockAuctionButtons-1-" + auction.someoneBet + "-" + auction.someoneRaised + 
                            "-" + auction.highestBid + "-0");
                }
                else {
                    //auction.playerActions(i+1);//
                    //server.broadcast("UnlockAuctionButtons-" + Integer.toString(i + 1) + "-0-0-0-0");
                    server.broadcast("UnlockAuctionButtons-" + (i + 1) + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                            "-" + auction.highestBid + "-0");
                    System.out.println("Serwer: UnlockAuctionButtons-" + (i + 1) + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                            "-" + auction.highestBid + "-0");
                }
            }
            else if (notifiedAuctionPlayers == server.getLimitOfPlayers()){
                server.broadcast("BlockAuctionButtons-" + part2 + "-0-0-0-0");
                /*for (int i = 1; i <= server.getLimitOfPlayers(); i++) {
                    server.broadcast("BlockAuctionButtons-" + i + "-0-0-0-0");
                    //server.broadcast("UnlockCardButtons-" + Integer.toString(i) + "-0-0-0-0");
                }*/
                notifiedAuctionPlayers = 0;
                auctionRoundNumber++;
                 
                System.out.println("Zako�czono runde licytacji!!!!");
                if (auctionRoundNumber == 4) {
                    System.out.println("!!!!!Wyloniono zwyciezce, rozdano kase!!!!!!!!!!");
                     
                    //whoWon <-----------------------------------------------
                    /*for (int i = 1; i <= SocketServer.getLimitOfPlayers(); i++) {
                        System.out.println("<-------------------------------------------------------------->");
                        server.broadcast("GetCardValues-" + i + "-0-0-0-0");
                         
                    }*/
                     
                    if(server.getLimitOfPlayers()==2) {
                        auction.whoWon(logic.resultCompare(logic.cardCompare(Cards.cards1), logic.cardCompare(Cards.cards2), "fold", "fold", "fold", "fold"));
                    }
                    if(server.getLimitOfPlayers()==3) {
                        auction.whoWon(logic.resultCompare(logic.cardCompare(Cards.cards1), logic.cardCompare(Cards.cards2), logic.cardCompare(Cards.cards3), "fold", "fold", "fold"));
                    }
                    if(server.getLimitOfPlayers()==4) {
                        auction.whoWon(logic.resultCompare(logic.cardCompare(Cards.cards1), logic.cardCompare(Cards.cards2), logic.cardCompare(Cards.cards3), logic.cardCompare(Cards.cards4), "fold", "fold"));
                    }
                    if(server.getLimitOfPlayers()==5) {
                        auction.whoWon(logic.resultCompare(logic.cardCompare(Cards.cards1), logic.cardCompare(Cards.cards2), logic.cardCompare(Cards.cards3), logic.cardCompare(Cards.cards4), logic.cardCompare(Cards.cards5), "fold"));
                    }
                    if(server.getLimitOfPlayers()==6) {
                        auction.whoWon(logic.resultCompare(logic.cardCompare(Cards.cards1), logic.cardCompare(Cards.cards2), logic.cardCompare(Cards.cards3), logic.cardCompare(Cards.cards4), logic.cardCompare(Cards.cards5), logic.cardCompare(Cards.cards6)));
                    }
                     
                    tourNumber++;
                    auctionRoundNumber = 1;
                     
                     
                    if (server.getLimitOfPlayers() > 2) {
                        if (dealer + 1 > server.getLimitOfPlayers()) {
                            dealer = 1;
                        }
                        else {
                            dealer = dealer + 1;
                        }
                        if (smallBlind + 1 > server.getLimitOfPlayers()) {
                            smallBlind = 1;
                        }
                        else {
                            smallBlind = smallBlind + 1;
                        }
                        if (bigBlind + 1 > server.getLimitOfPlayers()) {
                            bigBlind = 1;
                        }
                        else {
                            bigBlind = bigBlind + 1;
                        }
                        auction.sb(smallBlind, SocketServer.smallBlindBet);
                        auction.bb(bigBlind, SocketServer.bigBlindBet);
                        for (int i = 1; i <= SocketServer.getLimitOfPlayers(); i++) {
                            server.broadcast("SetLastOffer-" + i + "-0-0-0-0");
                            server.broadcast("SetNotFoldStatus-" + i + "-0-0-0-0");
                            server.broadcast("SetNotAllInStatus-" + i + "-0-0-0-0");
                            tableOfFoldedPlayers[i-1] = 0;
                            tableOfAllInPlayers[i-1] = 0;
                        }
                        notifiedAuctionPlayers = 1;
                    }
                         
                        else {
                            if (dealer + 1 > server.getLimitOfPlayers()) {
                                dealer = 1;
                            }
                            else {
                                dealer = dealer + 1;
                            }
                            if (smallBlind + 1 > server.getLimitOfPlayers()) {
                                smallBlind = 1;
                            }
                            else {
                                smallBlind = smallBlind + 1;
                            }
                            auction.sb(smallBlind, SocketServer.smallBlindBet);
                            for (int i = 1; i <= SocketServer.getLimitOfPlayers(); i++) {
                                server.broadcast("SetLastOffer-" + i + "-0-0-0-0");
                                server.broadcast("SetNotFoldStatus-" + i + "-0-0-0-0");
                                server.broadcast("SetNotAllInStatus-" + i + "-0-0-0-0");
                                tableOfFoldedPlayers[i-1] = 0;
                                tableOfAllInPlayers[i-1] = 0;
                            }
                            notifiedAuctionPlayers = 1;
                        }
                     
                    String messageStatus = "Status-" + dealer + "-" + smallBlind + "-" + bigBlind + "-0-0";
                    server.broadcast(messageStatus);
                    numberOfChangingCards = 0;
                     
                    //The game must be begun by first player on the left from Big Blind
                    if (server.getLimitOfPlayers() > 2) {
                        if (bigBlind + 1 > server.getLimitOfPlayers()) {
                            //auction.playerActions(1); //
                            //server.broadcast("UnlockAuctionButtons-1-0-0-0-0");
                            server.broadcast("UnlockAuctionButtons-1-" + auction.someoneBet + "-" + auction.someoneRaised + 
                                    "-" + auction.highestBid + "-0");
                            System.out.println("Serwer: UnlockAuctionButtons-1-" + auction.someoneBet + "-" + auction.someoneRaised + 
                                    "-" + auction.highestBid + "-0");
                        }
                        else {
                            //auction.playerActions(bigBlind + 1); 
                            //server.broadcast("UnlockAuctionButtons-" + (bigBlind + 1) + "-0-0-0-0");
                            server.broadcast("UnlockAuctionButtons-" + (bigBlind + 1) + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                                    "-" + auction.highestBid + "-0");
                            System.out.println("Serwer: UnlockAuctionButtons-" + (bigBlind + 1) + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                                    "-" + auction.highestBid + "-0");
                        }
                    }
                    // When there are only 2 players in the game, dealer begins the licitation
                    else {
                        //auction.playerActions(dealer);  
                        //server.broadcast("UnlockAuctionButtons-" + dealer + "-0-0-0-0");
                        server.broadcast("UnlockAuctionButtons-" + dealer + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                                "-" + auction.highestBid + "-0");
                        System.out.println("UnlockAuctionButtons-" + dealer + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                                "-" + auction.highestBid + "-0");
                    }
                    //Rozdanie nowych kart graczom 
                    cards = new Cards();
                    cards.cardsInGame = 0;
                    System.out.println();
                    cards.shuffleDeck();
                     
                    for (int i = 1; i <= server.numberOfPlayers; i++) {
                     
                        String[] basicCards = cards.getCards(4);
                        String bCards = "BasicCards-" + i + "-" + basicCards[0] + "-" + basicCards[1] + "-" + basicCards[2] + "-" + basicCards[3];
                        server.broadcast(bCards);
                    }
                     
                }
                else {
                    for (int i = 1; i <= server.getLimitOfPlayers(); i++) {
                    	if (tableOfFoldedPlayers[i-1] != 0 ) {
                            notifiedCardPlayers++;
                    	}
                    	else {
                        server.broadcast("UnlockCardButtons-" + Integer.toString(i) + "-0-0-0-0");
                        //server.broadcast("UnlockAuctionButtons-" + i + "-" + auction.someoneBet + "-" + auction.someoneRaised + 
                            //  "-" + auction.highestBid + "-0");
                    	}
                    }
                }
            }
         
        }
        else if (part1.equals("CardValues")) {
            if(part2.equals("1")){
                Cards.cards1[0]=part3;
                Cards.cards1[1]=part4;
                Cards.cards1[2]=part5;
                Cards.cards1[3]=part6;
                System.out.println("TABLE: Karty GRACZA " + part2 + " :" + Cards.cards1[0] + "-" + Cards.cards1[1] + "-" + Cards.cards1[2] + "-" + Cards.cards1[3]);
                System.out.println("Player 1 cards");
            }
            if(part2.equals("2")){
                Cards.cards2[0]=part3;
                Cards.cards2[1]=part4;
                Cards.cards2[2]=part5;
                Cards.cards2[3]=part6;
                System.out.println("Player 2 cards");
            }
            if(part2.equals("3")){
                Cards.cards3[0]=part3;
                Cards.cards3[1]=part4;
                Cards.cards3[2]=part5;
                Cards.cards3[3]=part6;
                System.out.println("Player 3 cards");
            }
            if(part2.equals("4")){
                Cards.cards4[0]=part3;
                Cards.cards4[1]=part4;
                Cards.cards4[2]=part5;
                Cards.cards4[3]=part6;
                System.out.println("Player 4 cards");
            }
            if(part2.equals("5")){
                Cards.cards5[0]=part3;
                Cards.cards5[1]=part4;
                Cards.cards5[2]=part5;
                Cards.cards5[3]=part6;
                System.out.println("Player 5 cards");
            }
            if(part2.equals("6")){
                Cards.cards6[0]=part3;
                Cards.cards6[1]=part4;
                Cards.cards6[2]=part5;
                Cards.cards6[3]=part6;
                System.out.println("Player 6 cards");
            }
        }
         
    }
     
    public int drawDealer(int min, int max) {
        Random rand = new Random();
 
        // nextInt is normally exclusive of the top value,
        // so add 1 to make it inclusive
        int randomDealer = rand.nextInt((max - min) + 1) + min;
 
        return randomDealer;
    }
    /*
    //Checking if someone Folded or did All In
    if (i < 3) {
        if (tableOfAllInPlayers[i] == 1 || tableOfFoldedPlayers[i] == 1) {
            notifiedAuctionPlayers++;
            if (i + 1 == server.getLimitOfPlayers()) {
                i= 1;
            }
            else {
                i++;
            }
        }
    }
    else if (i == 3){
        if (tableOfAllInPlayers[0] == 1 || tableOfFoldedPlayers[0] == 1) {
            notifiedAuctionPlayers++;
            i = 2;
        }
        else {
            i = 3;
        }
    }*/
     
 
}