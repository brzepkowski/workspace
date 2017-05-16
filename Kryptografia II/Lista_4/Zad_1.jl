function genKey(n)
  w = zeros(BigInt, n)
  total = 0
  for i in 1:n
    if (i == 1)
      random = rand(1:n)
      w[i] = random
      total += random
    else
      random = total + rand(1:n)
      w[i] = random
      total += random
    end
  end

  q = total + rand(1:n)

  r = rand(1:q)
  while (gcd(r, q) != 1)
    r = rand(1:q)
  end
  println("q = ", q, ", r = ", r, ", gcd = ", gcd(r,q))

  β = zeros(BigInt, n)
  for i in 1:n
    β[i] = (w[i]*r) % q
  end

    return (β, w, q, r) # β is public key, whereas (w, q, r) is private key
end

genKey(20)

function encrypt(pathToFile::String)
  (β, w, q, r) = genKey(8)
  readFile = open(pathToFile);
  s = readstring(readFile)

  writeFile = open(string(pathToFile, "_enc"), "w")

  for plaintext in s # Iterate over each char in String
    ciphertext = 0
    α = bits(plaintext)
    α = [α[i] for i in 25:32]
    for j in 1:8 # j represents each bit of char i
      if α[j] == '1'
        ciphertext += β[length(β) - j + 1]
      end
    end
    write(writeFile, string(ciphertext,"\n"))
  end
  close(readFile)
  close(writeFile)
  return (β, w, q, r)
end

function decrypt(pathToFile::String, w, q, r)
  readFile = open(pathToFile)
  s = invmod(r, q) # Obliczamy s tak, aby spełniało sr = 1 (mod q)

  for c in eachline(readFile)
    cˈ = (parse(BigInt, c)*s) % q
    # cˈ = cs = ∑αᵢβᵢs (mod q), a ponieważ βᵢ = rwᵢ otrzymujemy
    # βᵢs = wᵢrs = wᵢ (mod q), więc
    # cˈ = ∑ αᵢwᵢ (mod q)
    plaintext = Char(0)
    for i in length(w):-1:1 # Jeśli ostatni element wₖ > cˈ, to αₖ = 0, a jeśli wₖ ≦ cˈ, to αₖ = 1
      if w[i] <= cˈ
        plaintext += (2^(i-1))
        cˈ = cˈ - w[i]
      end
    end
    println(" -> ", plaintext)
  end
end

(β, w, q, r) = encrypt("input")
decrypt("input_enc", w, q, r)
