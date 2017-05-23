import java.math.BigInteger;
import java.util.Random;

public class SquareRootThread implements Runnable {
    private BigInteger zero = new BigInteger("0");
    private BigInteger one = new BigInteger("1");
    private BigInteger two = new BigInteger("2");
    private BigInteger three = new BigInteger("3");
    private BigInteger four = new BigInteger("4");
    private BigInteger five = new BigInteger("5");
    private BigInteger six = new BigInteger("6");
    private BigInteger seven = new BigInteger("7");
    private BigInteger eight = new BigInteger("8");

    private BigInteger prime;
    private BigInteger c;
    private Random randomGenerator;

    public SquareRootThread(BigInteger c, BigInteger prime, Random randomGenerator) {
        this.c = c;
        this.prime = prime;
        this.randomGenerator = randomGenerator;
    }

    public static boolean isEven(BigInteger number) {
        return number.getLowestSetBit() != 0;
    }

    private int jacobiSymbol(BigInteger a, BigInteger n) {
        if (a.compareTo(zero) == 0) {
            return 0;
        }
        if (a.compareTo(one) == 0) {
            return 1;
        }
        BigInteger x = zero;
        BigInteger y = a;

        int solution = -2;

        if (!isEven(x) && (n.mod(eight).compareTo(three) == 0 || n.mod(eight).compareTo(five) == 0)) {
            solution = -1;
        } else {
            solution = 1;
        }
        if (n.mod(four).compareTo(three) == 0 && y.mod(four).compareTo(three) == 0) {
            solution = -solution;
        }
        if (y.compareTo(one) == 0) {
            return solution;
        } else {
            return solution * jacobiSymbol(n.mod(y), y);
        }
    }

    @Override
    public void run() {
        System.out.println(prime);
        BigInteger b = new BigInteger(8, randomGenerator);
        while (jacobiSymbol(b.pow(2).subtract(c.multiply(four)).mod(prime), prime) != -1) {
            b = new BigInteger(8, randomGenerator);
        }
        System.out.println("c = " + c);
        System.out.println("b = " + b);
        // Polynomial x^2 -bx + a
        Polynomial f1 = new Polynomial(one, 2); // 1 * x^2
        Polynomial f2 = new Polynomial(b, 1); // b * x
        Polynomial f3 = new Polynomial(c, 0); // c
        Polynomial f = f1.plus(f2).plus(f3);

        Polynomial g = new Polynomial(one, prime.add(one).divide(two).intValue());
        System.out.println(f.toString());
    }
}
