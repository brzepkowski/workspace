#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;
//--------------------------------------------------------------------------------------------------
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

int mul_inv(int a, int b)
{
	int b0 = b, t, q;
	int x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}

class mRND
{
public:
    void seed( unsigned int s ) { _seed = s; }
    void printData() {
    	cout << "Seed = " << _seed << ", a = " << _a << ", c = " << _c << ", m = " << _m << endl;
    }

protected:
    mRND() : _seed( 0 ), _a( 0 ), _c( 0 ), _m( 214748364 ) {}
    int rnd() { return( _seed = ( _a * _seed + _c ) % _m ); }

    int _a, _c;
    unsigned int _m, _seed;
};
//--------------------------------------------------------------------------------------------------

class BSD_RND : public mRND
{
public:
    BSD_RND() { _a = 1103515245; _c = 12345; }
    BSD_RND(int seed, int a, int c, int m) { _seed = seed; _a = a; _c = c; _m = m; }
    int rnd() { return mRND::rnd(); }
};
//--------------------------------------------------------------------------------------------------

int main( int argc, char* argv[] ) {

//		BSD_RND bsd_rnd(time(NULL), 214013, 2531011, 2147483648);
		BSD_RND bsd_rnd(1, 25173, 13849, 32768);
    bsd_rnd.printData();

    //  tn = sn+1 - sn and un = |tn+2 * tn - t2n+1|;

    int x [1000];
    int t [1000];
    int u [1000];
    int j, k = 0;

    for (int i = 0; i < 14; i++) {
    	x[i] = bsd_rnd.rnd();

    	if (i > 0) {
    		t[j] = x[j+1] - x[j];
    		j++;

    		if (i >= 3) {
    			u[k] = abs((t[k+2]*t[k]) - (t[k+1]*t[k+1]));
    			k++;
    		}
    	}
    }

    for (int i = 0; i < 10; i++) {
    	cout << i << " - a: " << x[i] << ", t: " << t[i] << ", u[i]: " << u[i] << endl;
    }

    int gcd_val = u[0];
    for (int i = 0; i < 10; i++) {
    	gcd_val = gcd(gcd_val, u[i]);
    }

    cout << "GCD = " << abs(gcd_val) << endl;
		cout << x[2] << ", " << x[1] << ", " << x[0] << endl;

		int a = (x[1] - x[0]) % gcd_val;
		if (a < 0) a = (gcd_val + a) % gcd_val;
		int a2 = (x[2] - x[1]) % gcd_val;
		if (a2 < 0) a2 = (gcd_val + a2) % gcd_val;
		a = (mul_inv(a, gcd_val) * a2) % gcd_val;

		int c = (x[1] - a*x[0]) % gcd_val;
		if (c < 0) c = (gcd_val + c) % gcd_val;

		cout << "a = " << a << ", c = " << c << endl;

		int x_n = x[13];
		for (int i = 0; i < 7; i++) {
			x_n = ((a * x_n) + c) % gcd_val;
			cout << "BSD = " << bsd_rnd.rnd() << ", PREDICTED = " << x_n << endl;
		}

    return 0;
}
