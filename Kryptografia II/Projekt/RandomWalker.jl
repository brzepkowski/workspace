
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

function randomWalker(N, L, d, l) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)

  s = collect(0:N-1)
  for i in 1:d
    push!(S, s[randperm(length(s))])
    # push!(S, KSAₖStoppingRule(N, L, iˈ))
  end

  vₖ = 0
  for k in 0:l-1
    vₖ = S[iˈ[k % d]][vₖ + 1]
  end
  # println(vₖ)
  return vₖ
end # randomWalker

function randomWalkerShiftingPerms(N, L, d, l) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)

  s = collect(0:N-1)
  for i in 1:d
    # push!(S, s[randperm(length(s))])
    push!(S, KSAₖStoppingRule(N, L, iˈ))
  end

  vₖ = 0
  for k in 0:l-1
    vₖ = S[iˈ[k % d]][vₖ + 1]
    for i in 1:d
      S[i] = PRGAₛPerm(S[i], N, iˈ)
    end
  end
  # println(vₖ)
  return vₖ
end # randomWalker

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
end # KSAₖ

function countMarkedCards(cards::BitArray)
  markedCards = 0
  for card in cards
    if (card)
      markedCards += 1
    end
  end
  return markedCards
end

# Ta wersja wykonuje jedynie permutację zbioru S (też uzywa stoppingRule)
function PRGAₛPerm(S, N, iˈ) # iˈ dodane jako argument w celu przyspieszenia
  cards = falses(N)
  cards[N] = true
  i = 0
  j = 0
  while (countMarkedCards(cards) < N)
    i = (i + 1) % N
    j = (j + S[iˈ[i]]) % N
    swap(iˈ[i], iˈ[j], S)
    if (!cards[iˈ[i]] && i == j)
      cards[iˈ[i]] = true
    end
    if (!cards[iˈ[i]] && cards[iˈ[j]])
      cards[iˈ[i]] = true
    end
  end
  return S
end # PRGAₛPerm

# function generateFile(mode, amountOfNumbers, n, L, d, l) # n - liczba bitów
#   N = 2^n
#   header = string("#==============================================\n",
#   "# generator Park       seed = 1\n",
#   "#=============================================\n",
#   "type: d\n",
#   "count: ", amountOfNumbers,
#   "\nnumbit: ", n, "\n")
#   open("test.in", "w") do f
#   write(f, header)
#     for i in 1:amountOfNumbers
#       write(f, string(randomWalker(N, L, d, l), "\n"))
#     end
#   end
# end # generateFile

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

function randomWalkerAdditionalRC4(N, L, d, l) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  S = [] # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)

  s = collect(0:N-1)
  for i in 1:d
    # push!(S, s[randperm(length(s))])
    push!(S, KSAₖStoppingRule(N, L, iˈ))
  end
  push!(S, PRGAₛ(KSAₖStoppingRule(d, L, iˈ), d, iˈ))

  vₖ = 0
  for k in 0:l-1
    bₖ = S[d+1][iˈ[k%d]]
    vₖ = S[iˈ[bₖ]][vₖ + 1]
  end
  # println(vₖ)
  return vₖ
end # randomWalker

while (true)
  write(randomWalker(256, 64, 16, 24))
end

# println(PRGAₛ([6,5,4,3,2,1], 6, generateIndexes(6)))
# println(randomWalkerAdditionalRC4(256, 64, 16, 24))
