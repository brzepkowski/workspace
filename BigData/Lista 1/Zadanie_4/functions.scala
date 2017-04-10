def gcd(a: Integer, b:Integer): Integer = {
	if (a == 0 || b == 0) Math.abs(a);
	else Math.abs(gcd(b, a % b));
}

def lcm(a: Integer, b:Integer): Integer = {
	if (a == 0 || b == 0) 0;
	else Math.abs(a * b) / gcd(a, b);
}

def Euler(n: Integer): Integer = {
	Range(1, n+1).count(gcd(_, n) == 1)
}

Range(1,101).filter(100%_==0).map(x=>Euler(x))//.sum
