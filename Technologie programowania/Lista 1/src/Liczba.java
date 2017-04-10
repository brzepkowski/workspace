import java.util.Scanner;



public class Liczba {
	
	public int liczbaNaturalna;
	public int podstawaSystemu;
	public boolean spozaZakresu = false;
	public boolean ujemna = false;
	
	
	
	public Liczba(int a, int b) {
		liczbaNaturalna = a;
		podstawaSystemu = b;
		if (liczbaNaturalna < 0) {
			ujemna = true;
			throw new IllegalArgumentException("Podana liczba jest ujemna");
		}
		
		
		
	}
	
	
	public String obliczenia() {
		String wynik = "";
		int liczbaNaturalna = this.liczbaNaturalna;
		int podstawaSystemu = this.podstawaSystemu;
		
		if (podstawaSystemu < 2 || podstawaSystemu > 16 ) {
			wynik = null;
			spozaZakresu = true;
		}
		else {
			while (liczbaNaturalna > 0) {
				int reszta = liczbaNaturalna % podstawaSystemu;
				if (reszta == 10) wynik = 'A' + wynik;
				else if (reszta == 11) wynik = 'B' + wynik;
				else if (reszta == 12) wynik = 'C' + wynik;
				else if (reszta == 13) wynik = 'D' + wynik;
				else if (reszta == 14) wynik = 'E' + wynik;
				else if (reszta == 15) wynik = 'F' + wynik;
				else if (reszta == 16) wynik = 'G' + wynik;
				else wynik = reszta + wynik;
				//System.out.print(reszta);
				liczbaNaturalna = liczbaNaturalna / podstawaSystemu;
			}
		}
		System.out.println(wynik);
		return wynik;
	}
	
	
	public static void main(String args[]) {
		Liczba liczba = new Liczba(-3, 2);
		liczba.obliczenia();
		/*
		String wynik = "";
		int a = 0, b = 0;
		try {
			a = Integer.parseInt(args[0]);
			b = Integer.parseInt(args[1]);
		}
		catch (NumberFormatException e) {
			System.out.println("Niepoprawne dane!");
			System.exit(0);
		}
	
		wynik = liczba.obliczenia(a,b);
		System.out.println(wynik);*/
	}
}
