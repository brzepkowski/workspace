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

  β = zeros(BigInt, n)
  for i in 1:n
    β[i] = (w[i]*r) % q
  end

    return (β, w, q, r) # β is public key, whereas (w, q, r) is private key
end

genKey(20)

function encrypt(pathToFile::String)
  (β, w, q, r) = genKey(8)
  β = BigInt[479,972,930,888,283,559,611,743]
  w = BigInt[6,10,21,41,83,162,325,653]
  q = 1303
  r = 186
  println("β = ", β)
  println("w = ", w)
  println("q = ", q)
  println("r = ", r)
  readFile = open(pathToFile);
  s = readstring(readFile)

  writeFile = open(string(pathToFile, "_enc"), "w")

  for plaintext in s # Iterate over each char in String
    ciphertext = 0
    α = bits(plaintext)
    α = [α[i] for i in 24:32]
    print("bits = ", α, ", | ")
    for j in 1:8 # j represents each bit of char i
      #ciphertext += j * β
      ciphertext += BigInt(α[j]) * β[j]
    end
    write(writeFile, string(ciphertext,"\n"))
    println(plaintext, " => ", ciphertext)
  end
  close(readFile)
  close(writeFile)
  return (β, w, q, r)
end

function decrypt(pathToFile::String, w, q, r)
  readFile = open(pathToFile)

  w = BigInt[6,10,21,41,83,162,325,653]
  q = 1303
  r = 186
  println("w = ", w)
  println("q = ", q)
  println("r = ", r)

  s = invmod(r, q) # Obliczamy s tak, aby spełniało sr = 1 (mod q)
  println("s = ", s)

  for c in eachline(readFile)
    cˈ = (parse(BigInt, c)*s) % q
    # println("c = ", c, ", s = ", s, ", c' = ", cˈ)
    # cˈ = cs = ∑αᵢβᵢs (mod q), a ponieważ βᵢ = rwᵢ otrzymujemy
    # βᵢs = wᵢrs = wᵢ (mod q), więc
    # cˈ = ∑ αᵢwᵢ (mod q)
    println("c = ", c, " , c' = ", cˈ)
    plaintext = Int(0)
    for i in length(w):-1:1 # Jeśli ostatni element wₖ > cˈ, to αₖ = 0, a jeśli wₖ ≦ cˈ, to αₖ = 1
      αᵢ = 0
      if w[i] <= cˈ
        αᵢ = 1
      end
      cˈ = cˈ - w[i]*αᵢ
      print(αᵢ, ", ")
      plaintext += (2^(i-1)) * αᵢ
      # plaintext = string(αᵢ, plaintext)
    end
    # plaintext = string(plaintext)
    println(" -> ", plaintext)
  end
end

(β, w, q, r) = encrypt("input")
decrypt("input_enc", w, q, r)
