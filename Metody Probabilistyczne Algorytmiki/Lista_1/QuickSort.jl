using PyPlot

function Harmonic(n)
  sum = 0
  for i in 1:n
    sum = sum + (1 / i)
  end
  return sum
end # Harmonic

function quickSort(A, left, right)
  Cₙ = 0 # liczba porównań
  Sₙ = 0 # liczba przestawień
  if right > left
    pivot = A[right]
    i, j = left, right
    while i <= j
      while A[i] < pivot
          i += 1
          Cₙ += 1
      end
      while A[j] > pivot
          j -= 1
          Cₙ += 1
      end
      Cₙ += 2
      if i <= j
          A[i], A[j] = A[j], A[i]; Sₙ += 1
          i += 1
          j -= 1
      end
    end
    C₀, S₀ = quickSort(A, left, j)
    C₁, S₁ = quickSort(A, i, right)
    Cₙ += C₀ + C₁
    Sₙ += S₀ + S₁
  end
  return Cₙ, Sₙ
end # quickSort

# m - liczba eksperymentów, n - długość tablicy
function testQuickSort(m, n)
  ECₙ = 0
  ECₙ² = 0
  ESₙ = 0
  ESₙ² = 0
  CₙResults = []
  SₙResults = []
  for i in 1:m
    A = shuffle(1:n)
    Cₙ, Sₙ = quickSort(A, 1, length(A))
    push!(CₙResults, Cₙ)
    push!(SₙResults, Sₙ)
    ECₙ += Cₙ
    ECₙ² += Cₙ^2
    ESₙ += Sₙ
    ESₙ² += Sₙ^2
  end
  ECₙ = ECₙ / m
  ESₙ = ESₙ / m
  ECₙ² = ECₙ² / m
  ESₙ² = ESₙ² / m
  VCₙ = ECₙ² - ECₙ^2
  VSₙ = ESₙ² - ESₙ^2
  println("ECₙ = ", ECₙ, ", ESₙ = ", ESₙ)
  # EC = (2*(n + 1)*Harmonic(n+1)) - ((8/3)*(n+1))
  # ES = ((1/3)*(n+1)*Harmonic(n+1)) - ((7/9)*(n+1)) + (1/2)
  # println("EC  = ", EC, ", ES  = ", ES)
  println("VCₙ = ", VCₙ, ", VSₙ = ", VSₙ)
  return CₙResults, SₙResults, ECₙ, VCₙ, ESₙ, VSₙ
end # testQuickSort

# Yaroslavskiy's version
function dualPivotQuickSort(A, left, right)
  Cₙ = 0 # liczba porównań
  Sₙ = 0 # liczba przestawień
  if right - left >= 1
    p = A[left]; q = A[right]
    if p > q
      p = A[right]
      q = A[left]
      A[left], A[right] = A[right], A[left]; Sₙ = Sₙ + 1
    end
    Cₙ = Cₙ + 1
    l = left + 1; g = right - 1; k = l
    while k <= g
      Cₙ = Cₙ + 1
      if A[k] < p
        A[k], A[l] = A[l], A[k]; Sₙ = Sₙ + 1
        l = l + 1
      else
        Cₙ = Cₙ + 1
        if A[k] > q
          Cₙ = Cₙ + 1
          while A[g] > q && k < g
            Cₙ = Cₙ + 1
            g = g - 1
          end
          A[k], A[g] = A[g], A[k]; Sₙ = Sₙ + 1
          g = g - 1
          Cₙ = Cₙ + 1
          if A[k] < p
            A[k], A[l] = A[l], A[k]; Sₙ = Sₙ + 1
            l = l + 1
          end
        end
      end
      k = k + 1
    end # while
    l = l - 1; g = g + 1
    A[left], A[l] = A[l], A[left] # Bring pivots to final position
    A[right], A[g] = A[g], A[right]
    Sₙ = Sₙ + 2
    C₀, S₀ = dualPivotQuickSort(A, left, l - 1)
    C₁, S₁ = dualPivotQuickSort(A, l + 1, g - 1)
    C₂, S₂ = dualPivotQuickSort(A, g + 1, right)
    Cₙ = Cₙ + C₀ + C₁ + C₂
    Sₙ = Sₙ + S₀ + S₁ + S₂
  end # if
  return Cₙ, Sₙ
end # dualPivotQuickSort

# m - liczba eksperymentów, n - długość tablicy
function testDualPivotQuickSort(m, n)
  ECₙ = 0
  ECₙ² = 0
  ESₙ = 0
  ESₙ² = 0
  CₙResults = []
  SₙResults = []
  for i in 1:m
    A = shuffle(1:n)
    Cₙ, Sₙ = dualPivotQuickSort(A, 1, length(A))
    push!(CₙResults, Cₙ)
    push!(SₙResults, Sₙ)
    ECₙ += Cₙ
    ECₙ² += Cₙ^2
    ESₙ += Sₙ
    ESₙ² += Sₙ^2
  end
  ECₙ = ECₙ / m
  ESₙ = ESₙ / m
  ECₙ² = ECₙ² / m
  ESₙ² = ESₙ² / m
  VCₙ = ECₙ² - ECₙ^2
  VSₙ = ESₙ² - ESₙ^2
  println("ECₙ = ", ECₙ, ", ESₙ = ", ESₙ)
<<<<<<< HEAD
  # EC = ((19/10)*(n + 1)*Harmonic(n+1)) - ((711/200)*(n+1)) + (3/2)
  # ES = ((3/5)*(n+1)*Harmonic(n+1)) - ((27/100)*(n+1)) - (7/12)
  # println("EC  = ", EC, ", ES  = ", ES)
=======
  EC = ((19/10)*(n + 1)*Harmonic(n+1)) - ((711/200e)*(n+1)) + (3/2)
  ES = ((3/5)*(n+1)*Harmonic(n+1)) - ((27/100)*(n+1)) - (7/12)
  println("EC  = ", EC, ", ES  = ", ES)
>>>>>>> 3c1a97a39a0d3885fa7d773ce0a4dc5cdcb9843d
  println("VCₙ = ", VCₙ, ", VSₙ = ", VSₙ)
  return CₙResults, SₙResults, ECₙ, VCₙ, ESₙ, VSₙ
end # testQuickSort

<<<<<<< HEAD
# testQuickSort(1000, 100)
# testDualPivotQuickSort(1000, 100)
=======
testQuickSort(100, 10)
# testDualPivotQuickSort(100, 10)
>>>>>>> 3c1a97a39a0d3885fa7d773ce0a4dc5cdcb9843d

# m - liczba eksperymentów, n - długość tablicy
function plotComparisonsQuickSort(m, n, ax)
  x = []
  CₙResults = []
  ECₙResults = []
  VCₙResults = []
  Bounds1 = []
  Bounds2 = []
  for i in 1000:1000:n
    push!(x, i)
    Cₙ, Sₙ, ECₙ, VCₙ, ESₙ, VSₙ = testQuickSort(m, i) # Zamienione oznaczenie Cₙ i Sₙ (w tym miejscu to są tablice)
    push!(CₙResults, Cₙ)
    push!(ECₙResults, ECₙ)
    push!(VCₙResults, VCₙ)
    push!(Bounds1, ECₙ + 2*sqrt(VCₙ))
    push!(Bounds2, ECₙ - 2*sqrt(VCₙ))
    # EC = ((19/10)*(i + 1)*Harmonic(i+1)) - ((711/200)*(i+1)) + (3/2)
    # push!(ECₙResults, EC)
  end
  ax[:plot](x, CₙResults, color="blue", "+")
  ax[:plot](x, ECₙResults, label="QuickSort", color="blue", linestyle="-")
  ax[:plot](x, Bounds1, color="black", "--")
  ax[:plot](x, Bounds2, color="black", "--")
  ax[:legend](loc="best")
  grid("on")
  title("Comparisons")
end #plotDualPivotQuickSort

# m - liczba eksperymentów, n - długość tablicy
function plotComparisonsDualPivotQuickSort(m, n, ax)
  x = []
  CₙResults = []
  ECₙResults = []
  VCₙResults = []
  Bounds1 = []
  Bounds2 = []
  for i in 1000:1000:n
    push!(x, i)
    Cₙ, Sₙ, ECₙ, VCₙ, ESₙ, VSₙ = testDualPivotQuickSort(m, i) # Zamienione oznaczenie Cₙ i Sₙ (w tym miejscu to są tablice)
    push!(CₙResults, Cₙ)
    push!(ECₙResults, ECₙ)
    push!(VCₙResults, VCₙ)
    push!(Bounds1, ECₙ + 2*sqrt(VCₙ))
    push!(Bounds2, ECₙ - 2*sqrt(VCₙ))
    # EC = ((19/10)*(i + 1)*Harmonic(i+1)) - ((711/200)*(i+1)) + (3/2)
    # push!(ECₙResults, EC)
  end
  ax[:plot](x, CₙResults, color="red", "+")
  ax[:plot](x, ECₙResults, label="Dual Pivot QuickSort", color="red", linestyle="-")
  ax[:plot](x, Bounds1, color="black", "--")
  ax[:plot](x, Bounds2, color="black", "--")
  ax[:legend](loc="best")
  grid("on")
  title("Comparisons")
end #plotDualPivotQuickSort


# m - liczba eksperymentów, n - długość tablicy
function plotSwapsQuickSort(m, n, ax)
  x = []
  SₙResults = []
  ESₙResults = []
  VSₙResults = []
  Bounds1 = []
  Bounds2 = []
  for i in 1000:1000:n
    push!(x, i)
    Cₙ, Sₙ, ECₙ, VCₙ, ESₙ, VSₙ = testQuickSort(m, i) # Zamienione oznaczenie Cₙ i Sₙ (w tym miejscu to są tablice)
    push!(SₙResults, Sₙ)
    push!(ESₙResults, ESₙ)
    push!(VSₙResults, VSₙ)
    push!(Bounds1, ESₙ + 2*sqrt(VSₙ))
    push!(Bounds2, ESₙ - 2*sqrt(VSₙ))
    # EC = ((19/10)*(i + 1)*Harmonic(i+1)) - ((711/200)*(i+1)) + (3/2)
    # push!(ECₙResults, EC)
  end
  ax[:plot](x, SₙResults, color="blue", "+")
  ax[:plot](x, ESₙResults, label="QuickSort", color="blue", linestyle="-")
  ax[:plot](x, Bounds1, color="black", "--")
  ax[:plot](x, Bounds2, color="black", "--")
  ax[:legend](loc="best")
  grid("on")
  title("Swaps")
end #plotDualPivotQuickSort

<<<<<<< HEAD
# m - liczba eksperymentów, n - długość tablicy
function plotSwapsDualPivotQuickSort(m, n, ax)
  x = []
  SₙResults = []
  ESₙResults = []
  VSₙResults = []
  Bounds1 = []
  Bounds2 = []
  for i in 1000:1000:n
    push!(x, i)
    Cₙ, Sₙ, ECₙ, VCₙ, ESₙ, VSₙ = testDualPivotQuickSort(m, i) # Zamienione oznaczenie Cₙ i Sₙ (w tym miejscu to są tablice)
    push!(SₙResults, Sₙ)
    push!(ESₙResults, ESₙ)
    push!(VSₙResults, VSₙ)
    push!(Bounds1, ESₙ + 2*sqrt(VSₙ))
    push!(Bounds2, ESₙ - 2*sqrt(VSₙ))
    # EC = ((19/10)*(i + 1)*Harmonic(i+1)) - ((711/200)*(i+1)) + (3/2)
    # push!(ECₙResults, EC)
  end
  ax[:plot](x, SₙResults, color="red", "+")
  ax[:plot](x, ESₙResults, label="Dual Pivot QuickSort", color="red", linestyle="-")
  ax[:plot](x, Bounds1, color="black", "--")
  ax[:plot](x, Bounds2, color="black", "--")
  ax[:legend](loc="best")
  grid("on")
  title("Swaps")
end #plotDualPivotQuickSort

fig, ax = PyPlot.subplots()
plotComparisonsQuickSort(100, 10000, ax)
plotComparisonsDualPivotQuickSort(100, 10000, ax)

fig, ax = PyPlot.subplots()
plotSwapsQuickSort(100, 10000, ax)
plotSwapsDualPivotQuickSort(100, 10000, ax)
=======
# plotDualPivotQuickSort(10, 100)
>>>>>>> 3c1a97a39a0d3885fa7d773ce0a4dc5cdcb9843d
