import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;


public class LiczbaTest4 {
	
	Liczba liczba = null;

	@Before
	public void setUp() throws Exception {
		liczba = new Liczba(9, 2);
	}
	
	@Test
	public void testOne() {
		int a = 145;
		int b = 16;
		String str = null;
		str = liczba.obliczenia();
		assertEquals(str, "1001");
	}
	
	@Ignore
	@Test
	public void testTwo() {
		int a = 145;
		int b = 28;
		String str = null;
		str = liczba.obliczenia();
		assertTrue(liczba.spozaZakresu);
	}
	
	@Test(timeout = 1)
	public void testThree() {
		int a = 145;
		int b = 28;
		String str = null;
		str = liczba.obliczenia();
		assertNotSame(str, b);
	}
	
	@Test(expected= IllegalArgumentException.class)
	public void testFour() {
		liczba = new Liczba(-7, 3);
		int a = Integer.parseInt("asd");
		int b = 28;
		String str = null;
		str = liczba.obliczenia();
	}
	

	@After
	public void tearDown() throws Exception {
		liczba = null;
	}

}
