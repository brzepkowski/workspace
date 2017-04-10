import java.util.ArrayList;
 
/*
 * Class contains methods for comparing cards
 */
public class GameLogic {
 
//doda� por�wnanie kart z tym samym kolorem, bra� ni�sz�
     
    /*
     * compares given cards
     * @returns result in a pattern : (how many unique cards)-(highest card value)
     */
    public String cardCompare(String cards[]){
        String result="";
        String value[] = new String[4];
        String color[] = new String[4];
        int valueNumber[] = new int[4];
        boolean uniqueValue[] = new boolean[4];
        boolean uniqueColor[] = new boolean[4];
        int tempValue=0;
        String tempColor="";
         
        for (int i = 0; i < 4; i++) {
            System.out.println("GAMELOGIC: " + cards[i]);
        }
         
        for(int i=0; i<4; i++){
            value[i]=cards[i].substring(0, 1);
            color[i]=cards[i].substring(1);
        }
         
        //creating tables to work with
        for(int i=0; i<4; i++){
            uniqueColor[i]=true;
            uniqueColor[i]=true;
            if(value[i].equals("a"))
                value[i]="0";
            else
            if(value[i].equals("1"))
                value[i]="10";
            else
            if(value[i].equals("j"))
                value[i]="11";
            else
            if(value[i].equals("q"))
                value[i]="12";
            else
            if(value[i].equals("k"))
                value[i]="13";
            valueNumber[i]=Integer.parseInt(value[i]);
             
        }
         
        //checking if a card is unique
            for(int i=0; i<4; i++){
                tempValue=valueNumber[i];
                tempColor=color[i];
                for(int j=0; j<4; j++){
                    if(tempValue==valueNumber[j] && i!=j)
                        uniqueValue[j]=false;
                    if(tempColor.equals(color[j]) && i!=j)
                        uniqueColor[j]=false;
                }
            }
            //counting unique cards
            int uniqueCards=0;
            for(int i=0; i<4; i++){
                if(uniqueColor[i]==true && uniqueValue[i]==true)
                uniqueCards++;
            }
             
            int onlyUnique[];
            int max=0;
             
            //if there's less than 4 unique cards check which cards are unique
            if(uniqueCards<4){
                onlyUnique=new int[uniqueCards];
                for(int i=0; i<uniqueCards; i++){
                    if (uniqueColor[i]==true && uniqueValue[i]==true){
                        onlyUnique[i]=valueNumber[i];
                    }
                }
                 
                //finding highest card in hand
                for(int i=0; i<uniqueCards; i++){
                    if(onlyUnique[i]>max)
                        max=onlyUnique[i];
                }
                 
            }
            result=uniqueCards+"-"+max;
            return result;
        }
     
    /** !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! gdy b�dzie mniej ni� 6 graczy zawsze podawaj innych jako fold, b�dzie �atwiej
     * ~!!~!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! wynik wykorzysta� w Auction funkcja whoWon + wys�a� do klient�w, ma tam wyskoczy� popup z info kt�rzy wygrali
     * compares results of cardCompare
     * returns winners
     * player can have his result as "fold", he won't be counted
     * @param playerOne
     * @param playerTwo
     * @param playerThree
     * @param playerFour
     * @param playerFive
     * @param playerSix
     * @return result : array containing winners (more than 1 if there's a draw)
     */
    public int[] resultCompare(String playerOne, String playerTwo, String playerThree, String playerFour, String playerFive, String playerSix){
        int[] result = new int[0];
        String[][] temp = new String[6][2];
        int[][] everyResult = new int[6][2];
        ArrayList<Integer> playersWithHighest = new ArrayList<Integer>();
        ArrayList<Integer> winningNumbers = new ArrayList<Integer>();
         
        if(playerOne!="fold"){
            temp[0]=playerOne.split("-");
        }
        else{
            temp[0][0]="0";
            temp[0][1]="100";
        }
        if(playerTwo!="fold"){
            temp[1]=playerTwo.split("-");
        }
        else{
            temp[1][0]="0";
            temp[1][1]="100";
        }
        if(playerThree!="fold"){
            temp[2]=playerThree.split("-");
        }
        else{
            temp[2][0]="0";
            temp[2][1]="100";
        }
        if(playerFour!="fold"){
            temp[3]=playerFour.split("-");
        }
        else{
            temp[3][0]="0";
            temp[3][1]="100";
        }
        if(playerFive!="fold"){
            temp[4]=playerFive.split("-");
        }
        else{
            temp[4][0]="0";
            temp[4][1]="100";
        }
        if(playerSix!="fold"){
            temp[5]=playerSix.split("-");
        }
        else{
            temp[5][0]="0";
            temp[5][1]="100";
        }
        for(int i=0; i<6; i++){
            for(int j=0; j<2; j++){
                everyResult[i][j]=Integer.parseInt(temp[i][j]);
            }
        }
         
        int max=0;
        for(int i=0; i<6; i++){
            if(everyResult[i][0]>max)
                max=everyResult[i][0];
        }
        int counter=0;
        for(int i=0; i<6; i++){
            if(everyResult[i][0]==max)
                counter++;
        }
        int min=100;
         
        if(counter==1){ //situation where only 1 player has highest amount of unique cards
            for(int i=0; i<6; i++){
                if(max==everyResult[i][0]){
                    result = new int[1];
                    result[0]=i+1;
                    return result;
                }
            }
        }
        else{
            for (int s=0; s<6; s++){
                if(everyResult[s][1]<min && everyResult[s][0]==max)
                    min=everyResult[s][1];
                System.out.println("min "+min);
            }
                       
	            for(int s=0; s<6; s++){
	                if(min==everyResult[s][1] && everyResult[s][0]==max){
	                	winningNumbers.add(s);
	                }
	            }
            
            int k = 0;
            int[] winners = new int[winningNumbers.size()];
            for(int s : winningNumbers){
                winners[k]=s+1;
                k++;
            }
            return winners;
        }
        return result;
    }
}