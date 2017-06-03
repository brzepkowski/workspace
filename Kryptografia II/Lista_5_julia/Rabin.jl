using Primes
using MultiPoly

function jacobiSymbol(a::BigInt, n::BigInt)
  if a == 0
    return 0
  end
  if a == 1
    return 1
  end
  x = BigInt(0)
  y = a

  solution = -2

  if isodd(x) && (n % 8 == 3 || n % 8 == 5)
    solution = -1
  else
    solution = 1
  end

  if n % 4 == 3 && y % 4 == 3
    solution = -solution
  end
  if y == 1
    return solution
  else
    return solution * jacobiSymbol(n % y, y)
  end
end # jacobiSymbol

function generateKeys(n)
  range = (2^n) - 1
  p = BigInt(rand(1:range))
  q = BigInt(rand(1:range))
  while !isprime(p) || p % 4 != 1
    p = BigInt(rand(1:range))
  end
  while !isprime(q) || q % 4 != 1
    q = BigInt(rand(1:range))
  end
  n = p*q
  return (p, q, n)
end # generateKeys

function divmod(N::MPoly, D::MPoly, prime)
    pu = PolyUnion([N, D])
    d = zero(pu)
    x = generator(MPoly{BigInt}, :x)
    q = zero(pu)

    while deg(N) >= deg(D)
      d = zero(pu)
      for (i, val) in D
        if val != 0
          d[newexps(pu, i, 1) + (deg(N) - deg(D))] = val
        end
      end
      q[Int(deg(N) - deg(D))] = N[Int(deg(N))] / d[Int(deg(d))]
      d = d * BigInt(q[Int(deg(N) - deg(D))])
      N = N - d
    end
    r = N
    for (i, val) in r
      r[i] = val % prime
    end
    return (q, r)
end # divmod

function exponentModPolynomial(g::MPoly, f::MPoly, k, prime) # g - polynomial, f - modular polynomial, k - power
    kBits = bits(k)
    kLength = 0
    for i in length(kBits):-1:1
      if kBits[i] == '1'
        kLength = length(kBits) - i + 1
      end
    end
    x = generator(MPoly{BigInt}, :x)
    s = 1
    if k == 0
      return s
    end
    G = g
    if kBits[length(kBits)] == '1'
      s = g
    end
    for i in 2:kLength
      # println("BIT = ", kBits[length(kBits) - i + 1])
      (q, r) = divmod(G^2, f, prime)
      G = r
      # println("G = ", G)
      if kBits[length(kBits) - i + 1] == '1'
        # println("Weszlo 2")
        (q, r) = divmod(G * s, f, prime)
        s = r
      end
      # println("s = ", s)
    end
    return s
end # exponentModPolynomial

function encrypt(pathToFile::String, keyLength)
  (p, q, n) = generateKeys(keyLength)
  readFile = open(pathToFile);
  s = readstring(readFile)
  writeFile = open(string(pathToFile, "_enc"), "w")

  for m in s # Iterate over each char in String
    c = BigInt(m)^2 % n
    write(writeFile, string(c,"\n"))
  end

  close(readFile)
  close(writeFile)
  return (p, q, n)
end # encrypt

function calculateRoot(c::BigInt, prime::BigInt, keyLength)
  range = (2^keyLength) - 1
  c = c % prime
  # println("Dostałem c = ", c, ", prime = ", prime)

  # ------- Generowanie b ----------
  x = generator(MPoly{BigInt}, :x)
  solution = x
  while deg(solution) != 0
    b = BigInt(rand(1:prime))
    while jacobiSymbol((b^2 - 4*c) % prime, prime) != -1
      b = BigInt(rand(1:prime))
    end
    # println("b = ", b)
    #------- Cipollas version ----------
    #= println("(b + sqrt(b^2 - c)) = ", (b + sqrt(b^2 - c)))
    # x = (b + sqrt(b^2 - c))^((prime+1)/2) % prime
    # println("x = ", x) =#
    #-----------------------------------

    x = generator(MPoly{BigInt}, :x)
    f = x^2 - b*x + c
    # power = BigInt(2)
    g = x
    k = Int64((prime+1)/2)
    # println("f = ", f)
    # println("g = ", g)
    solution = exponentModPolynomial(g, f, k, prime)
  end
  plaintext = BigInt(solution[0])
  if plaintext < 0
    plaintext = prime + plaintext # Actaually + becomes -, because plaintext is lower than zero
  end
  if plaintext <= 127 && plaintext > 31
    println("PLAINTEXT = ", Char(plaintext))
  else
    println("PLAINTEXT = ...")
  end

  # -----prime = 53, c = 1089, b = 90------
  # f = x^2 - 90*x + 1089
  # g = x
  # println(exponentModPolynomial(g, f, 5))
end # calculateRoot

function decrypt()
  keyLength = BigInt(16)
  (p, q, n) = encrypt("input", keyLength)
  println("p = ", p, ", q = ", q, ", n = ", n)
  readFile = open("input_enc");
  writeFile = open("input_enc_dec", "w")

  for line in eachline(readFile)
    c = parse(BigInt, line)

    # Parallelisation
    pair = (p,q)
    Threads.@threads for i=1:2
      calculateRoot(c, pair[i], keyLength)
    end
  end

  close(readFile)
  close(writeFile)
end # decrypt

decrypt()
