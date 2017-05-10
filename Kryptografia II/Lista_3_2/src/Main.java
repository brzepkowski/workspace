import org.apache.commons.codec.binary.Base64;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.*;
import java.security.Key;
import java.security.KeyStore;
import java.util.Random;

public class Main {

    public static String encrypt(Key key, String initVector, String value) {
        try {
            IvParameterSpec iv = new IvParameterSpec(initVector.getBytes("UTF-8"));
//            SecretKeySpec skeySpec = new SecretKeySpec(key.getBytes("UTF-8"), "AES");
            SecretKeySpec skeySpec = new SecretKeySpec(key.getEncoded(), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.ENCRYPT_MODE, skeySpec, iv);

            byte[] encrypted = cipher.doFinal(value.getBytes());

            String str = new String(encrypted);
            return new String(Base64.encodeBase64(encrypted));
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        return null;
    }

    public static String decrypt(Key key, String initVector, String encrypted) {
        try {
            IvParameterSpec iv = new IvParameterSpec(initVector.getBytes("UTF-8"));
//            SecretKeySpec skeySpec = new SecretKeySpec(key.getBytes("UTF-8"), "AES");
            SecretKeySpec skeySpec = new SecretKeySpec(key.getEncoded(), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.DECRYPT_MODE, skeySpec, iv);

            byte[] original = cipher.doFinal(Base64.decodeBase64(encrypted.getBytes()));

            return new String(original);
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        return null;
    }

    public static Key getKey(String pathToKeystore, String keyStorePass, String keyAlias, String keyPass) {
        Key key = null;
        try {
            InputStream keystoreStream = new FileInputStream(pathToKeystore);
            KeyStore keystore = KeyStore.getInstance("JCEKS");
            keystore.load(keystoreStream, keyStorePass.toCharArray());
            if (!keystore.containsAlias(keyAlias)) {
                throw new RuntimeException ("Alias for key not found");
            }
            key = keystore.getKey(keyAlias, keyPass.toCharArray());
            System.out.println(key.toString());
        } catch (Exception e) {
            System.out.println(e);
        }
        return key;
    }

    public static void processFile(String mode, String sourceFileLocation, String destFileLocation, Key key, String initVector) {
        try {
            BufferedReader inFile = new BufferedReader(new FileReader(sourceFileLocation));
            BufferedWriter outFile = null;
            if (mode.equals("encrypt")) outFile = new BufferedWriter(new FileWriter(destFileLocation + "_enc"));
            else outFile = new BufferedWriter(new FileWriter(destFileLocation + "_dec"));

            String line = null;
            while ((line = inFile.readLine()) != null) {
                if (mode.equals("encrypt")) {
                    String encrypted = encrypt(key, initVector, line);
                    outFile.write(encrypted + "\n");
                } else {
                    String decrypted = decrypt(key, initVector, line);
                    outFile.write(decrypted + "\n");
                }
            }
            inFile.close();
            outFile.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }

    public static void main(String[] args) throws IOException {
        Console console = System.console();

        if (args[0].equals("oracle")) {
            System.out.println("Tryb wyroczni");
            if (args.length < 5) System.out.println("Podano za mało argumentów");

            String mode = args[1]; // Says if oracle will encrypt or decrytp
            String pathToKeystore = args[2];
            String keyAlias = args[3];
            String keyStorePass = new String(console.readPassword("Podaj hasło do keyStore: "));
            String keyPass = new String(console.readPassword("Podaj hasło do klucza: "));
            String initVector = new String(console.readLine("Podaj IV: "));
            Key key = getKey(pathToKeystore, keyStorePass, keyAlias, keyPass);

            if (!mode.equals("encrypt") && !mode.equals("decrypt")) {
                System.out.println("Podano niepoprawny tryb działania: " + mode);
            } else {
                for (int i = 4; i < args.length; i++) {
                    processFile(mode, args[i], args[i], key, initVector);
                }
            }
        } else if (args[0].equals("challenge")) {
            System.out.println("Tryb challange");
            if (args.length != 5) System.out.println("Podano niepoprawną liczbę argumentów");
            String pathToKeystore = args[1];
            String keyAlias = args[2];
            String keyStorePass = new String(console.readPassword("Podaj hasło do keyStore: "));
            String keyPass = new String(console.readPassword("Podaj hasło do klucza: "));
            String initVector = new String(console.readLine("Podaj IV: "));
            String message1 = args[3];
            String message2 = args[4];
            Key key = getKey(pathToKeystore, keyStorePass, keyAlias, keyPass);
            Random randomGenerator = new Random();
            int random = randomGenerator.nextInt(2);
            if (random == 0) {
                processFile("encrypt", message1, "messageB", key, initVector);
            } else {
                processFile("encrypt", message2, "messageB", key, initVector);
            }

            BufferedWriter ivFile = new BufferedWriter(new FileWriter("IV"));
            ivFile.write(initVector); // Zapisanie IV wykorzystywanego w challenge do pliku
            ivFile.close();
            BufferedWriter answerFile = new BufferedWriter(new FileWriter("answer"));
            answerFile.write(Integer.toString(random)); // Zapisanie odpowiedzi z challenge do osobnego pliku
            answerFile.close();
        } else {
            System.out.println("Podano niepoprawny tryb: " + args[0]);
        }
    }
}
