
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
    # println(bits)
    keyBits = string(keyBits, bits)
  end
  # println(keyBits)
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

function countMarkedCards(cards::BitArray)
  markedCards = 0
  for card in cards # !!!! Tu zrównoleglić !!!!
    if (card)
      markedCards += 1
    end
  end
  return markedCards
end # countMarkedCards

# Ta wersja wykonuje jedynie permutację zbioru S (też uzywa stoppingRule)
function PRGAₛPerm(S, N, iˈ) # iˈ dodane jako argument w celu przyspieszenia
  cards = falses(N)
  cards[N] = true
  i = 0
  j = 0
  for t in 1:N # Zamiast stoppingRule dodany stały licznik N
    i = (i + 1) % N
    j = (j + S[iˈ[i]]) % N
    swap(iˈ[i], iˈ[j], S)
  end
  return S
end # PRGAₛPerm

function PRGAₛ(S, N, iˈ)
  i = 0
  j = 0
  Z = []
  while (length(Z) < N)
    i = (i + 1) % N
    j = (j + S[iˈ[i]]) % N
    swap(iˈ[i], iˈ[j], S)
    z = S[iˈ[(S[iˈ[i]] + S[iˈ[j]]) % N]]
    push!(Z, z)
  end
  return Z
end # PRGAₛ

function KSAₖStoppingRule(N, L, iˈ) # iˈ dodane jeko argument w celu przyspieszenia
  (K, keyBits) = generateKey(L)
  cards = falses(N)
  cards[N] = true
  S = Array{Int}(N)
  for i in 0:N-1
    S[iˈ[i]] = i
  end
  j = 0
  r = 0 # r - round
  while (countMarkedCards(cards) < N)
    j = (j + S[iˈ[r % N]] + Int(K[iˈ[r % L]])) % N
    swap(iˈ[r % N], iˈ[j % N], S)
    if (!cards[iˈ[r % N]] && (r%N) == (j%N))
      cards[iˈ[r%N]] = true
    end
    if (!cards[iˈ[r%N]] && cards[iˈ[j%N]])
      cards[iˈ[r%N]] = true
    end
    r += 1
  end
  # println(S)
  return S
end # KSAₖStoppingRule

function randomWalker(N, L, d, l, T) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)

  s = collect(0:N-1)
  for i in 1:d
    # push!(S, s[randperm(length(s))])
    push!(S, KSAₖStoppingRule(N, L, iˈ))
  end

  println("#==============================================
# generator Park       seed = 1
#=============================================
type: d
count: ", T,
"\nnumbit: ", Int(log2(N)))

  vₖ = 0
  for t in 1:T
    for k in 0:l-1
      vₖ = S[iˈ[k % d]][vₖ + 1]
      println(vₖ)
    end
  end
end # randomWalker

function randomWalkerShiftingPerms(N, L, d, l, T) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)

  s = collect(0:N-1)
  for i in 1:d
    # push!(S, s[randperm(length(s))])
    push!(S, KSAₖStoppingRule(N, L, iˈ))
  end

  println("#==============================================
# generator Park       seed = 1
#=============================================
type: d
count: ", T,
"\nnumbit: ", Int(log2(N)))

  vₖ = 0
  for t in 1:T
    for k in 0:l-1
      vₖ = S[iˈ[k % d]][vₖ + 1]
      # write(vₖ)
      println(vₖ)
      for i in 1:d # !!!! To zrównoleglić !!!!
        S[i] = PRGAₛPerm(S[i], N, iˈ)
      end
    end
  end
end # randomWalkerShiftingPerms

function randomWalkerAdditionalRC4(N, L, d, l, T) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)

  s = collect(0:N-1)
  for i in 1:d
    # push!(S, s[randperm(length(s))])
    push!(S, KSAₖStoppingRule(N, L, iˈ))
  end
  push!(S, KSAₖStoppingRule(d, L, iˈ))

  println("#==============================================
# generator Park       seed = 1
#=============================================
type: d
count: ", T,
"\nnumbit: ", Int(log2(N)))

  vₖ = 0
  for t in 1:T
    for k in 0:l-1
      bₖ = S[d+1][iˈ[k%d]]
      vₖ = S[iˈ[bₖ]][vₖ + 1]
      # write(vₖ)
      println(vₖ)
    end
  end
end # randomWalkerAdditionalRC4

function randomWalkerAdditionalRC4ShiftingPerms(N, L, d, l, T) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)

  s = collect(0:N-1)
  for i in 1:d
    # push!(S, s[randperm(length(s))])
    push!(S, KSAₖStoppingRule(N, L, iˈ))
  end
  push!(S, KSAₖStoppingRule(d, L, iˈ))

  println("#==============================================
# generator Park       seed = 1
#=============================================
type: d
count: ", T,
"\nnumbit: ", Int(log2(N)))

  vₖ = 0
  for t in 1:T
    for k in 0:l-1
      bₖ = S[d+1][iˈ[k%d]]
      vₖ = S[iˈ[bₖ]][vₖ + 1]
      # write(vₖ)
      println(vₖ)
      for i in 1:d # !!!! To zrównoleglić !!!!
        S[i] = PRGAₛPerm(S[i], N, iˈ)
      end
    end
  end
end # randomWalkerAdditionalRC4

randomWalker(16, 16, 4, 4, 1000000)
# randomWalkerShiftingPerms(256, 64, 16, 24, 1000000)
# randomWalkerAdditionalRC4(256, 64, 16, 24)
# randomWalkerAdditionalRC4ShiftingPerms(256, 64, 16, 24)
