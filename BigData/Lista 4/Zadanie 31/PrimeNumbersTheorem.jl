using Primes

x = BigInt(1)
x = x << 64
a = x
x = x / log(x)
y = BigInt(1)
y = y << 64
y = y + 1000
b = y
y = y / log(y)
println(y - x)
j = 1
for i in a:b
	if isprime(i)
		println(j, ": ", i)
		j = j + 1
	end
end
