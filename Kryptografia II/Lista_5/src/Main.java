import java.io.*;
import java.math.BigInteger;
import java.util.Random;

public class Main {

    public static BigInteger[] generateKeys() {
        Random randomGenerator = new Random();
        BigInteger p = new BigInteger(8, randomGenerator);
        BigInteger q = new BigInteger(8, randomGenerator);
        while (!p.isProbablePrime(1000000)) {
            p = new BigInteger(8, randomGenerator);
        }
        while (!q.isProbablePrime(1000000)) {
            q = new BigInteger(8, randomGenerator);
        }
        BigInteger n = p.multiply(q);
        System.out.println("p = " + p);
        System.out.println("q = " + q);
        System.out.println("n = " + n);

        BigInteger[] keys = {p, q, n}; // p and q are private keys / n is public key
        return keys;
    }

    public static void main(String[] args) throws IOException {
        String mode = args[0];
        BigInteger[] keys = generateKeys();
        BigInteger p = keys[0];
        BigInteger q = keys[1];
        BigInteger n = keys[2];
        Random randomGenerator = new Random();

        if (mode.equals("encrypt")) {
            System.out.println("Szyfrowanie...");

            BufferedReader readFileBR = new BufferedReader(new FileReader(args[1]));
            PrintWriter writeFilePW = new PrintWriter(args[2]);
            int currentSign = -1;
            while ((currentSign = readFileBR.read()) != -1) {
                BigInteger m = BigInteger.valueOf(currentSign);
                BigInteger c = m.pow(2).mod(n); // c = m^2 (mod n)
                writeFilePW.println(c.toString());
            }
            readFileBR.close();
            writeFilePW.close();


        } else if (mode.equals("decrypt")) {
            System.out.println("Deszyfrowanie...");

            BufferedReader readFileBR = new BufferedReader(new FileReader(args[1]));
            PrintWriter writeFilePW = new PrintWriter(args[2]);
            String currentLine = "";
            while ((currentLine = readFileBR.readLine()) != null) {
                BigInteger c = new BigInteger(currentLine);
                SquareRootThread thread1 = new SquareRootThread(c, p, randomGenerator);
                thread1.run();
                SquareRootThread thread2 = new SquareRootThread(c, q, randomGenerator);
                thread2.run();
//                writeFilePW.println(c.toString());
            }
            readFileBR.close();
            writeFilePW.close();

        } else {
            System.out.println("Podano niepoprawny tryb działania: " + mode);
        }
    }
}