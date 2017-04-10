/* Bartosz Rzepkowski
 * Języki Formalne i Techniki Translacji
 * Lista 1 - 28.10.2015
 */
public class DFA {
	
	static int[][] delta;
	static int[] Q;
	
	static int min(int a, int b) {
		if (a >= b) return b;
		else return a;
	}
	
	static int delta(int q, int a) {
		return delta[q][a];
	}

	public static void main(String[] args) {
		
		if (args.length < 3 || args[0].isEmpty() || args[1].isEmpty() || args[2].isEmpty()) {
			System.out.println("Niekompletne dane. Nie można uruchomić programu.");
			System.exit(0);
		} 
		String Sigma = args[0];
		String P = args[1];
		String T = args[2];
		int m = P.length(); // Length of pattern	
		int n = T.length(); // Length of text
		delta = new int[m+1][Sigma.length()];
		
		System.out.println("Alfabet: " + Sigma + "\nWzorzec: " + P + "\nTekst: " + T);
		
		for (int q = 0; q <= m; q++) {		// For every state q
			for (int a = 0; a < Sigma.length(); a++) {	// For every letter a in alphabet
				int k = min(m + 1, q + 2); // It really is min(m, q + 1) but later in "while" we are subtracting 1

				// repeat k <- k - 1
					// until prefix of P(k) is suffix of P(q)a
				String P_prim = ""; 			// P_prim = P + a
				for (int i = 0; i < q; i++) {
					P_prim = P_prim + P.charAt(i);
				}
				
				P_prim = P_prim + Sigma.charAt(a);
				
				//System.out.print("P_prim: " + P_prim);
				boolean is_suffix = false;
				while (is_suffix == false) {
					k--;
					is_suffix = true;
					for (int l = 0; l < k; l++) {
						if (P.charAt(l) != P_prim.charAt(P_prim.length() - k + l)) {
							is_suffix = false;
						}
					}
				}
				delta[q][a] = k;
				//System.out.println(" k = " + k + " delta[" + q + "][" + a + "] = " + delta[q][a]);
			}
		}
		int q = 0;
		for (int i = 0; i < n; i++) {
			int j = 0;
			while (Sigma.charAt(j) != T.charAt(i)) j++;
			q = delta(q, j);
			//System.out.println("q = " + q);
			if (q == m) System.out.println("Wzorzec znaleziony na pozycji: " + (i - m + 1)); // Text is indexed from 0
		}		
	}
}