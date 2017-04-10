import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;


public class Framing {
	
	public static String Code (String stream, int frameSize) {
		int i = 0, j = 0;
		String frame = "";
		String crc;
		String flag = "01111110";
		String output = "";
		int index = 0;
		
		while (index < stream.length()) {
		
			if (index + 32 <= stream.length()) {
				frame = stream.substring(index, index+32);
				index += 32;
			}
			else {
				frame = stream.substring(index, stream.length());
				index += stream.length();
			}
			//Append frame with crc
			crc = calculateCRC(frame);
			frame += crc;
			
			//Replace all "11111" with "111110"
			frame = frame.replaceAll("11111", "111110");
			
			//Add flags at the beginning and the end
			frame = flag + frame + flag;
			
			//Append output
			output += frame;
			
		}
		return output;
	}
	
	public static String Decode (String stream) {
		String decoded = "";
		stream = stream.replaceAll("0111111001111110", "-");
		stream = stream.replaceAll("01111110", "");
		for (String retval: stream.split("-")) {
	         //System.out.println(retval);
	         retval = retval.replaceAll("111110", "11111");
	         retval = retval.substring(0, retval.length() - 8);
	         decoded += retval;
	    }
		
		return decoded;
	}
	
	
	public static String readFile(String source) {
		FileReader fr = null;
		String buffer = "";
		// OTWIERANIE PLIKU:
	 	   try {
	 	     fr = new FileReader(source);
	 	   } catch (FileNotFoundException e) {
	 	       System.out.println("BŁĄD PRZY OTWIERANIU PLIKU!");
	 	       System.exit(1);
	 	   }  	
	 	   
	 	   BufferedReader bfr = new BufferedReader(fr);
	 	   String line = "";
	 	   // ODCZYT KOLEJNYCH LINII Z PLIKU:
	 	   try { 		   
	 	     while((line = bfr.readLine()) != null ){ 
	 	    	 buffer += line;
	 	    	 System.out.println(buffer);
	 	     }
	 	    } catch (IOException e) {
	 	        System.out.println("BŁĄD ODCZYTU Z PLIKU!");
	 	        System.exit(2);
	 	   }

	 	   // ZAMYKANIE PLIKU
	 	   try {
	 	     fr.close();
	 	    } catch (IOException e) {
	 	         System.out.println("BŁĄD PRZY ZAMYKANIU PLIKU!");
	 	         System.exit(3);
	 	    }
		return buffer;
	}
	
	public static void saveStream(String source, String output) {
		
	      try {
	    	// creates a FileWriter Object
		      FileWriter writer = new FileWriter(source); 
		      // Writes the content to the file
		      writer.write(output);
		      writer.flush();
		      writer.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	      

	}
	
	public static String calculateCRC(String input) {
		StringBuilder sb = new StringBuilder();
		String crc = "";
		String dzielnik = "10101001";
		int i = 0;
		int j = 0;
		input = input + "00000000";
		sb.append(input);
		
		while (i+j < sb.length()) {
			if (dzielnik.charAt(j) != sb.charAt(i+j)) {
				sb.setCharAt(i+j, '1');
			}
			else {
				sb.setCharAt(i+j, '0');
			}
			
			if (j == 7) {
				j = 0;
				i++;
			}
			else {
				j++;
			}
		}
		
		crc = sb.substring(sb.length() - 8, sb.length());
		return crc;
	}
	
	public static void main(String[] args) {

 	   	String buffer = "";
 	   	int frameSize = 32;

 	  
 	   buffer = readFile("Z.txt");
 	   System.out.println("Pobrano " + buffer.length() + " bity");
 	  
 	   String coded = Code(buffer, frameSize);
 	   System.out.println("Output = " + coded);
 	
 	   saveStream("W.txt", coded);
 	   
 	   buffer = readFile("W.txt");    
	
	   System.out.println("Pobrano " + buffer.length() + " bity" + " z pliku W.txt");
	  
	   
	   String decoded = Decode(buffer);
	   //System.out.println("Decoded = " + decoded);
	   
	   buffer = readFile("Z.txt");
	   if (buffer.equals(decoded)) {
		   System.out.println("Takie same");
	   }
	   else {
		   System.out.println("Różne");
	   }
 	   
 	   
	}
}
