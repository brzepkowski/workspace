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
  (β, w, q, r) = genKey(32)
  readFile = open(pathToFile);
  s = readstring(readFile)

  writeFile = open(string(pathToFile, "_enc"), "w")

  for plaintext in s # Iterate over each char in String
    ciphertext = 0
    α = bits(plaintext)
    for j in 1:32 # j represents each bit of char i
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

function decrypt(pathToFile::String, β, w, q, r)
  s = invmod(r, q)
end

(β, w, q, r) = encrypt("input")
decrypt("input_enc", β, w, q, r)
