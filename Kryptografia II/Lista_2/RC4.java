import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.List;

public class RC4 {

  public static void swap(int[] s, int source, int destination) {
    int buffer = s[source];
    s[source] = s[destination];
    s[destination] = buffer;
  }

  public static int[] ksa(int keyLength, int n, int t) {
	  byte[] k = new byte[keyLength / 8];
	  SecureRandom secureRandom = new SecureRandom();
	  secureRandom.nextBytes(k); // Generate random key of length "keyLength" (in BYTES)
	  int l = k.length;
	  System.out.println("L = " + l*8);
	  int[] s = new int[n];
	  for (int i = 0; i < n; i++) {
		  s[i] =  i;
	  }
	  int j = 0;
	  for (int i = 0; i <= t; i++) {
		  j = (j + s[i % n] + Byte.toUnsignedInt(k[i % l])) % n;
		  swap(s, i % n, j % n);
	  }
	  return s;
  }
  
  public static int[] ksa_rs(int keyLength, int n, int t) {
	  SecureRandom secureRandom = new SecureRandom();
	  byte[] keyBytes = new byte[keyLength / 8];
	  secureRandom.nextBytes(keyBytes);
	  BitSet k = BitSet.valueOf(keyBytes);
	  int l = k.length();
	  System.out.println("L = " + l);
	  int[] s = new int[n];
	  int[] newS = new int[n];
	  for (int i = 0; i < n; i++) {
		  s[i] = i;
	  }
	  for (int r = 0; r <= t; r++) {
//		  System.out.println("----S----");
//		  for (int x: s) System.out.println(x);
		  List<Integer> top = new ArrayList<Integer>();
		  List<Integer> bottom = new ArrayList<Integer>();
		  for (int i = 0; i < n; i++) {
			  if (!k.get((r*n + i) % l))
				  top.add(i);
			  else 
				  bottom.add(i);
		  }
		  int topSize = top.size();
		  int bottomSize = bottom.size();
		  for (int i = 0; i < topSize; i++) {
			  newS[i] = s[top.get(i)];
		  }
		  for (int i = 0; i < bottomSize; i++) {
			  newS[topSize + i] = s[bottom.get(i)];
		  }
		  for (int i = 0; i < n; i++)
			  s[i] = newS[i];
	  }
	  return s;
  }
  
  public static boolean stoppingRule(int[][] cards, int round, BitSet kBits) {
	  int n = cards.length;
	  round = round % n;
	  int bit = 0;
	  if (kBits.get(round % kBits.length())) bit = 1;
	  int j = Math.abs(n - bit) % n;
	  // Count marked cards
	  int markedCards = 0;
	  for (int i = 0; i < n; i++) {
		  if (cards[i][1] == 1)
			  markedCards++;
	  }
	  // Check if there is less than ceiling((n - 1) / 2) marked cards
	  if (markedCards < Math.ceil((n - 1) / 2)) {
		  if (cards[round][1] == 0 && cards[j][1] == 0) {
			  cards[round][1] = 1;
		  }
	  } else {
		  if ((cards[round][1] == 0 && cards[j][1] == 1) || (cards[round][1] == 0 && round == j)) {
			  cards[round][1] = 1;
		  }
	  }
	  
	  markedCards = 0;
	  for (int i = 0; i < n; i++) {
		  if (cards[i][1] == 1)
			  markedCards++;
	  }
	  if (markedCards == n) {
		  return true;
	  } else {
		  return false;
	  }
  }

  public static int[] ksa_sst(int keyLength, int n) {
	  byte[] k = new byte[keyLength / 8];
	  SecureRandom secureRandom = new SecureRandom();
	  secureRandom.nextBytes(k); // Generate random key of length "keyLength" (in BYTES)
	  BitSet kBits = BitSet.valueOf(k);
	  int l = k.length;
	  System.out.println("L = " + l*8);
	  int[] s = new int[n];
	  for (int i = 0; i < n; i++) {
		  s[i] =  i;
	  }
	  int j = 0; int round = 0;
	  int[][] cards = new int[n][2];
	  for (int i = 0; i < n; i++) {
		  cards[i][0] = i;
		  cards[i][1] = 0;
	  }
	  while (!stoppingRule(cards, round, kBits)) {
		  j = (j + s[round % n] + Byte.toUnsignedInt(k[round % l])) % n;
		  swap(s, round % n, j % n);
		  round++;
	  }
	  System.out.println("Rounds = " + round);
//	  System.out.println("------S---------");
//	  for (int i: s) {
//		  System.out.println(i);
//	  }
	  return s;
  }
  
  // Desired size of file is in BYTES
  public static void prga_s(int[] s, int n, String pathToOutputFile, int desiredSizeOfFile) throws IOException {
	  OutputStream output = new FileOutputStream(pathToOutputFile, false); // False means that it will not append the file, but overwrite the content
	  int i = 0; int j = 0;
	  int totalGeneratedSize = 0;
	  int flag = 0;
	  int finalOutput = 0;
	  while (totalGeneratedSize < desiredSizeOfFile) {
		  i = (i + j) % n;
		  j = (j + s[i]) % n;
		  swap(s, i, j);
		  int z = s[(s[i] + s[j]) % n];
		  int base = (int) ((Math.log(n) / Math.log(2)));
		  	if (flag < n / base) {
				if (flag < (n / base) - 2) {
					finalOutput += z;
					finalOutput = finalOutput << base;
					flag++;
				}
                else if (flag == (n/base) -2) {
                    finalOutput += z;
                    finalOutput = finalOutput << (n % base);
                    flag++;
                }
		  		else if (flag == (n/base) -1) {
					if (n % base != 0) {
						z = z >> base - (n % base);
					}
                    finalOutput += z;
					flag++;
				}
			} else {
		  		output.write(finalOutput);
		  		finalOutput = 0;
		  		flag = 0;
				totalGeneratedSize += 1;
			}

	  }
	  System.out.println(totalGeneratedSize);
  }
  
  public static void rc4(int keyLength, int n, int t) throws IOException {
	  int[] s = ksa(keyLength, n, t);
	  prga_s(s, n, "RC4("+n+", "+t+").in", 1000000000);
  }
  
  public static void rc4_rs(int keyLength, int n, int t) throws IOException {
	  int[] s = ksa_rs(keyLength, n, t);
	  prga_s(s, n, "RC4-RS("+n+", "+t+").in", 1000000000);
  }
  
  public static void rc4_sst(int keyLength, int n) throws IOException {
	  int[] s = ksa_sst(keyLength, n);
	  prga_s(s, n, "RC4-SST("+n+").in", 1000000000);
  }
  
  public static void main(String[] args) throws IOException {
  	int keyLength = Integer.parseInt(args[1]);
  	int n = Integer.parseInt(args[2]);

  	if (args[0].equals("RC4")) {
		rc4(keyLength, n, n);
	} else if (args[0].equals("RC4-RS")) {
        int t = (int) (2.0 * (Math.log(n) / Math.log(2)));
		rc4_rs(keyLength, n, t);
	} else {
		rc4_sst(keyLength, n);
	}
  }
  
}
