import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.nio.charset.StandardCharsets;

public class Main {

    public static String encrypt(String key, String initVector, String value) {
        try {
            IvParameterSpec iv = new IvParameterSpec(initVector.getBytes("UTF-8"));
            SecretKeySpec skeySpec = new SecretKeySpec(key.getBytes("UTF-8"), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.ENCRYPT_MODE, skeySpec, iv);

            byte[] encrypted = cipher.doFinal(value.getBytes());

            String str = new String(encrypted);
            System.out.println("encrypted string: " + str);
            return new String(encrypted);
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        return null;
    }

    public static String decrypt(String key, String initVector, String encrypted) {
        try {
            IvParameterSpec iv = new IvParameterSpec(initVector.getBytes("UTF-8"));
            SecretKeySpec skeySpec = new SecretKeySpec(key.getBytes("UTF-8"), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.DECRYPT_MODE, skeySpec, iv);

            byte[] original = cipher.doFinal(encrypted.getBytes());

            return new String(original);
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        return null;
    }

    public static void main(String[] args) {
        if (args[0].equals("oracle")) {
            if (args.length < 5) System.out.println("Podano za mało argumentów");
            System.out.println("Tryb wyroczni");
        } else if (args[0].equals("challenge")) {
            if (args.length != 5) System.out.println("Podano niepoprawną liczbę argumentów");
            System.out.println("Tryb challange");
        } else {
            System.out.println("Podano niepoprawny tryb");
        }
//        String key = "Bar12345Bar12345"; // 128 bit key
//        String initVector = "RandomInitVector"; // 16 bytes IV
//
//        System.out.println(decrypt(key, initVector,
//                encrypt(key, initVector, "Hello World")));
    }
}
