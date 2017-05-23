import java.math.BigInteger;

/******************************************************************************
 *  Compilation:  javac Polynomial.java
 *  Execution:    java Polynomial
 *
 *  A polynomial with arbitrary precision rational coefficients.
 *
 ******************************************************************************/

public class Polynomial {
    public final static Polynomial ZERO = new Polynomial(BigInteger.ZERO, 0);

    private BigInteger[] coef;   // coefficients
    private int deg;              // degree of polynomial (0 for the zero polynomial)

    // a * x^b
    public Polynomial(BigInteger a, int b) {
        coef = new BigInteger[b+1];
        for (int i = 0; i < b; i++)
            coef[i] = BigInteger.ZERO;
        coef[b] = a;
        deg = degree();
    }


    // return the degree of this polynomial (0 for the zero polynomial)
    public int degree() {
        int d = 0;
        for (int i = 0; i < coef.length; i++)
            if (coef[i].compareTo(BigInteger.ZERO) != 0) d = i;
        return d;
    }

    // return c = a + b
    public Polynomial plus(Polynomial b) {
        Polynomial a = this;
        Polynomial c = new Polynomial(BigInteger.ZERO, Math.max(a.deg, b.deg));
        for (int i = 0; i <= a.deg; i++) c.coef[i] = c.coef[i].add(a.coef[i]);
        for (int i = 0; i <= b.deg; i++) c.coef[i] = c.coef[i].add(b.coef[i]);
        c.deg = c.degree();
        return c;
    }

    // return c = a - b
    public Polynomial minus(Polynomial b) {
        Polynomial a = this;
        Polynomial c = new Polynomial(BigInteger.ZERO, Math.max(a.deg, b.deg));
        for (int i = 0; i <= a.deg; i++) c.coef[i] = c.coef[i].add(a.coef[i]);
        for (int i = 0; i <= b.deg; i++) c.coef[i] = c.coef[i].subtract(b.coef[i]);
        c.deg = c.degree();
        return c;
    }

    // return (a * b)
    public Polynomial times(Polynomial b) {
        Polynomial a = this;
        Polynomial c = new Polynomial(BigInteger.ZERO, a.deg + b.deg);
        for (int i = 0; i <= a.deg; i++)
            for (int j = 0; j <= b.deg; j++)
                c.coef[i+j] = c.coef[i+j].add(a.coef[i].multiply(b.coef[j]));
        c.deg = c.degree();
        return c;
    }

    // return (a / b)
    public Polynomial divides(Polynomial b) {
        Polynomial a = this;
        if ((b.deg == 0) && (b.coef[0].compareTo(BigInteger.ZERO) == 0))
            throw new RuntimeException("Divide by zero polynomial");

        if (a.deg < b.deg) return ZERO;

        BigInteger coefficient = a.coef[a.deg].divide(b.coef[b.deg]);
        int exponent = a.deg - b.deg;
        Polynomial c = new Polynomial(coefficient, exponent);
        return c.plus( (a.minus(b.times(c)).divides(b)) );
    }

    // truncate to degree d
    public Polynomial truncate(int d) {
        Polynomial p = new Polynomial(BigInteger.ZERO, d);
        for (int i = 0; i <= d; i++)
            p.coef[i] = coef[i];
        p.deg = p.degree();
        return p;
    }

    // use Horner's method to compute and return the polynomial evaluated at x
    public BigInteger evaluate(BigInteger x) {
        BigInteger p = BigInteger.ZERO;
        for (int i = deg; i >= 0; i--)
            p = coef[i].add(x.multiply(p));
        return p;
    }

/*    // differentiate this polynomial and return it
    public Polynomial differentiate() {
        if (deg == 0) return ZERO;
        Polynomial deriv = new Polynomial(BigInteger.ZERO, deg-1);
        for (int i = 0; i < deg; i++)
            deriv.coef[i] = coef[i+1].multiply(new BigInteger(i + 1));
        deriv.deg = deriv.degree();
        return deriv;
    }

    // return antiderivative
    public Polynomial integrate() {
        Polynomial integral = new Polynomial(BigInteger.ZERO, deg + 1);
        for (int i = 0; i <= deg; i++)
            integral.coef[i+1] = coef[i].divide(new BigInteger(i + 1));
        integral.deg = integral.degree();
        return integral;
    }


    // return integral from a to b
    public BigInteger integrate(BigInteger a, BigInteger b) {
        Polynomial integral = integrate();
        return integral.evaluate(b).subtract(integral.evaluate(a));
    }
*/

    // convert to string representation
    public String toString() {
        if (deg ==  0) return "" + coef[0];
        if (deg ==  1) return coef[1] + " x + " + coef[0];
        String s = coef[deg] + " x^" + deg;
        for (int i = deg-1; i >= 0; i--) {
            int cmp = coef[i].compareTo(BigInteger.ZERO);
            if      (cmp == 0) continue;
            else if (cmp  > 0) s = s + " + " + ( coef[i]);
            else if (cmp  < 0) s = s + " - " + (coef[i].negate());
            if      (i == 1) s = s + " x";
            else if (i >  1) s = s + " x^" + i;
        }
        return s;
    }
/*
    // test client
    public static void main(String[] args) {
        BigInteger half  = new BigInteger(1, 2);
        BigInteger three = new BigInteger(3, 1);

        Polynomial p = new Polynomial(half,  1);
        Polynomial q = new Polynomial(three, 2);
        Polynomial r = p.plus(q);
        Polynomial s = p.times(q);
        Polynomial t = r.times(r);
        Polynomial u = t.minus(q.times(q));
        Polynomial v = t.divides(q);
        Polynomial w = v.times(q);

        StdOut.println("p(x)                      = " + p);
        StdOut.println("q(x)                      = " + q);
        StdOut.println("r(x) = p(x) + q(x)        = " + r);
        StdOut.println("s(x) = p(x) * q(x)        = " + s);
        StdOut.println("t(x) = r(x) * r(x)        = " + t);
        StdOut.println("u(x) = t(x) - q^2(x)      = " + u);
        StdOut.println("v(x) = t(x) / q(x)        = " + v);
        StdOut.println("w(x) = v(x) * q(x)        = " + w);
        StdOut.println("t(3)                   = " + t.evaluate(three));
        StdOut.println("t'(x)                  = " + t.differentiate());
        StdOut.println("t''(x)                 = " + t.differentiate().differentiate());
        StdOut.println("f(x) = int of t(x)     = " + t.integrate());
        StdOut.println("integral(t(x), 1/2..3) = " + t.integrate(half, three));
    }
*/
}
