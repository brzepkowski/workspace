using LightGraphs

function swap(x, y, S)
  temp = S[x]
  S[x] = S[y]
  S[y] = temp
end # swap

function generateKey(L)
  K = Array{Char}(L)
  keyBits = ""
  for i in 1:L
    K[i] = rand(0:(2^8)-1)
    bits = bin(K[i])
    while (length(bits) < 8)
      bits = string('0', bits)
    end
    println(bits)
    keyBits = string(keyBits, bits)
  end
  println(keyBits)
  return (K, keyBits)
end # generateKey

function generateIndexes(N)
  indexes = Dict{Int, Int}()
  # Zrobione do umożliwienia iterowania od 0
  for i in 0:N
    indexes[i] = i+1
  end
  return indexes
end # generateIndexes

function randomWalker(N, d, l)
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)

  s = collect(0:N-1)
  for i in 1:d
    push!(S, s[randperm(length(s))])
  end

  vₖ = 0
  for k in 0:l-1
    # println(S[iˈ[k % d]])
    # println("k = ", k ,"(", iˈ[k], ") , k mod d = ", k % d,"(", iˈ[k%d], ") , vₖ = ", vₖ, "(", iˈ[vₖ], ") , vₖ+1 = ", S[iˈ[k % d]][vₖ + 1], "(", iˈ[S[iˈ[k % d]][vₖ + 1]] , ")")
    vₖ = S[iˈ[k % d]][vₖ + 1]
  end
  println(vₖ)
end # randomWalker

function KSAₖ(N, T, L)
  iˈ = generateIndexes(N)
  (K, keyBits) = generateKey(L)
  round = 0
  cards = falses(N)

  L = length(K)
  S = Array{Int}(N)
  for i in 0:N-1
    S[iˈ[i]] = i
  end
  j = 0
  # for i in 0:T
  r = 0 # r - round
  while (!stoppingRuleKLZ(cards, r, keyBits))
    j = (j + S[iˈ[r % N]] + Int(K[iˈ[r % L]])) % N
    swap(iˈ[r % N], iˈ[j % N], S)
    r += 1
  end
  println(S)
end # KSAₖ

function countMarkedCards(cards::BitArray)
  markedCards = 0
  for i in 1:length(cards)
    if (cards[i])
      markedCards += 1
    end
  end
  return markedCards
end

function stoppingRuleKLZ(π::BitArray, r, keyBits) # π - tablica kart, które będziemy oznaczać, r - numer rundy, keyBits - klucz zapisany bitowo
  iˈ = generateIndexes(length(keyBits))
  n = length(π)
  r = r % n

  # value(Bits) oznacza, czy bit na miejscu `round mod length(keyBits)` jest równy 0 czy 1
  valueBits = 0
  if (keyBits[iˈ[r % length(keyBits)]] == '1')
    valueBits = 1
  end

  j = (n - 1) - valueBits
  m = countMarkedCards(π)

  if (m < ceil((n - 1)/2))
    if (!π[iˈ[r]] && !π[iˈ[j]])
      π[iˈ[r]] = true
    end
  else
    if (!π[iˈ[r]] && π[iˈ[j]]) || (!π[iˈ[r]] && r == j)
      π[iˈ[r]] = true
    end
  end

  m = countMarkedCards(π)
  if (m == n) # Wszystkie karty zostały oznaczone
    return true
  else
    return false
  end
end # stoppingRuleKLZ

# for i in 1:10
#   randomWalker(10, 2, 6)
# end


KSAₖ(16, 16, 4)
