import java.io.*;
import java.math.BigInteger;

public class Zad_2 {

    public static void main(String[] args) throws IOException {
        BufferedReader ivFile = new BufferedReader(new FileReader("IV"));
        BigInteger iv = new BigInteger(ivFile.readLine());
        BigInteger iv2 = iv.add(new BigInteger("1"));
        BufferedReader messageFile = new BufferedReader(new FileReader("message_2"));
        BigInteger message = new BigInteger(messageFile.readLine());
        message = message.xor(iv.xor(iv2));
        BufferedWriter outMessageFile = new BufferedWriter(new FileWriter("message_2_modified"));
        outMessageFile.write(message.toString());
        outMessageFile.close();
    }

}
