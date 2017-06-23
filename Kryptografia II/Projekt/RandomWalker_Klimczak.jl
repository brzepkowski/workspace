
function swap(x, y, S)
  temp = S[x]
  S[x] = S[y]
  S[y] = temp
end # swap

function generateKey(L)
  return rand(UInt8, L)
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
  return sum(cards)
end # countMarkedCards

# Ta wersja wykonuje jedynie permutację zbioru S (też uzywa stoppingRule)
function PRGAₛPerm(S, N, iˈ) # iˈ dodane jako argument w celu przyspieszenia

  shuffle!(S)
  #=
  cards = falses(N)
  cards[N] = true
  i = 0
  j = 0
  for t in 1:N # Zamiast stoppingRule dodany stały licznik N
    i = (i + 1) % N
    j = (j + S[iˈ[i]]) % N
    swap(iˈ[i], iˈ[j], S)
  end=#
  return S

end # PRGAₛPerm


function KSAₖStoppingRule(N, L, iˈ) # iˈ dodane jeko argument w celu przyspieszenia
  K = generateKey(L)
  cards = falses(N)
  cards[N] = true
  S = Vector{Int64}(N)
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

function randomWalker(n, L, d, l, T) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  N = 2^n
  S = Vector{Vector{Int64}}(d) # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)
  io = STDOUT
  buf = 0
  licz = 1

  Threads.@threads for i in 1:d
    S[i] = KSAₖStoppingRule(N, L, iˈ)
  end

  vₖ = 0
  for t in 1:T
    if t%100000 == 0
      write(STDERR, "$(t/T)\n")
    end
    for k in 0:l-1
      vₖ = S[iˈ[k % d]][vₖ + 1]
    end

    w = vₖ
    buf += w
    buf <<= n
    licz+=1
    if licz == 64/n
      write(buf)
      # flush(io)
      buf = 0
      licz = 1
    end
  end
end # randomWalker

function randomWalkerShiftingPerms(n, L, d, l, T) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  N = 2^n
  S = Vector{Vector{Int64}}(d) # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)
  io = STDOUT
  buf = 0
  licz = 1

  Threads.@threads for i in 1:d
    S[i] = KSAₖStoppingRule(N, L, iˈ)
  end

  vₖ = 0
  for t in 1:T
    if t%1 == 0
      write(STDERR, "$(t/T)\n")
    end
    for k in 0:l-1
      vₖ = S[iˈ[k % d]][vₖ + 1]
      Threads.@threads for i in 1:d # !!!! To zrównoleglić !!!!
        S[i] = PRGAₛPerm(S[i], N, iˈ)
      end
    end
    w = vₖ
    buf += w
    buf <<= n
    licz+=1
    if licz == 64/n
      write(io, buf)
      buf = 0
      licz = 1
    end
  end
#  return io
end # randomWalkerShiftingPerms

function randomWalkerAdditionalRC4(n, L, d, l, T) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  N = 2^n
  S = Vector{Vector{Int64}}(d+1) # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)
  io = STDOUT
  buf = 0
  licz = 1
  Threads.@threads for i in 1:d
    S[i] = KSAₖStoppingRule(N, L, iˈ)
  end
  S[d+1]= KSAₖStoppingRule(d, L, iˈ)
  buf = 0
  licz = 1
  vₖ = 0
  for t in 1:T
    if t%100000 == 0
      write(STDERR, "$(t/T)\n")
    end
    for k in 0:l-1
      bₖ = S[d+1][iˈ[k%d]]
      vₖ = S[iˈ[bₖ]][vₖ + 1]
    end
    w = vₖ
    buf += w
    buf <<= n
    licz+=1
    if licz == 64/n
      write(io, buf)
      buf = 0
      licz = 1
    end
  end
#  return io
end # randomWalkerAdditionalRC4

function randomWalkerAdditionalRC4ShiftingPerms(n, L, d, l, T) # N - maksymalny rozmiar wyjściowych liczb, L - długość klucza, d - stopień wierzchołków, l - liczba kroków
  N = 2^n
  S = Vector{Vector{Int64}}(d+1) # Tablica zawierająca wszystkie permutacje Sⱼ
  iˈ = generateIndexes(N)
  io = STDOUT
  buf = 0
  licz = 1
  Threads.@threads for i in 1:d
    S[i] = KSAₖStoppingRule(N, L, iˈ)
  end
  S[d+1 ]= KSAₖStoppingRule(d, L, iˈ)


  vₖ = 0
  for t in 1:T
    if t%100000 == 0
      write(STDERR, "$(t/T)\n")
    end
    for k in 0:l-1
      bₖ = S[d+1][iˈ[k%d]]
      vₖ = S[iˈ[bₖ]][vₖ + 1]
      Threads.@threads for i in 1:d # !!!! To zrównoleglić !!!!
        S[i] = PRGAₛPerm(S[i], N, iˈ)
      end
    end
    w = vₖ
    buf += w
    buf <<= n
    licz+=1
    if licz == 64/n
      write(io, buf)
      buf = 0
      licz = 1
    end
  end
#  return io
end # randomWalkerAdditionalRC4

function io_to_file(io::IOBuffer, file::String)
  f = open(file, "w")
  write(f, takebuf_array(io))
  close(f)
end


function generateTests()
  println("Rw444")
  io_to_file(randomWalker(4,16,4,4,1000000), "Rw_4_4_4.in")
  println("RwSp444")
  io_to_file(randomWalkerShiftingPerms(4,16,4,4,1000000), "RwSp_4_4_4.in")
  println("RwArc444")
  io_to_file(randomWalkerAdditionalRC4(4,16,4,4,1000000), "RwArc_4_4_4.in")
  println("RwArcSp444")
  io_to_file(randomWalkerAdditionalRC4ShiftingPerms(4,16,4,4,1000000), "RwArcSp_4_4_4.in")


  println("Rw81616")
  io_to_file(randomWalker(8,16,16,16,1000000), "Rw_8_16_16.in")
  println("RwSp81616")
  io_to_file(randomWalkerShiftingPerms(8,16,16,16,1000000), "Rwsp_8_16_16.in")
  println("RwArc81616")
  io_to_file(randomWalkerAdditionalRC4(8,16,16,16,1000000), "Rwarc_8_16_16.in")
  println("RwArcSp81616")
  io_to_file(randomWalkerAdditionalRC4ShiftingPerms(8,16,16,16,1000000), "Rwarcsp_8_16_16.in")

end
