import junit.framework.TestCase;


public class LiczbaTest3 extends TestCase {
	
	Liczba liczba = null;

	protected void setUp() throws Exception {
		liczba = new Liczba(9, 2);
	}

	public void testOne() {
		String str = null;
		str = liczba.obliczenia();
		assertEquals( "1001",str);
	}
	/*
	public void testTwo() {
		String str = null;
		
		str = liczba.obliczenia();
		assertTrue("Warunek nieprawdziwy!", liczba.spozaZakresu);
	}*/
	
	public void testThree() {
		String str = null;
		int a = 1567;
		int b = 0;
		str = liczba.obliczenia();
		assertNotNull( str);
	}
	
	public void testFour() {
		String str = null;
		int a = 16789;
		int b = 6;
		str = liczba.obliczenia();
		assertNotSame(str, a);
	}
	
	public void testFive() {
		String str = null;
		int a = 1567;
		int b = 12;
		str = liczba.obliczenia();
		assertFalse(liczba.spozaZakresu);
	}
	
	
	
	protected void tearDown() throws Exception {
		liczba = null;
	}

}
