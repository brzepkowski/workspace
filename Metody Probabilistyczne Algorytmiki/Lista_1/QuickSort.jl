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
  for i in 1:m
    A = shuffle(1:n)
    Cₙ, Sₙ = quickSort(A, 1, length(A))
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
  EC = (2*(n + 1)*Harmonic(n+1)) - ((8/3)*(n+1))
  ES = ((1/3)*(n+1)*Harmonic(n+1)) - ((7/9)*(n+1)) + (1/2)
  println("EC  = ", EC, ", ES  = ", ES)
  println("VCₙ = ", VCₙ, ", VSₙ = ", VSₙ)
end # testQuickSort

# Yaroslavskiy's version
function dualPivotQuickSort(A, left, right)
  Cₙ = 0 # liczba porównań
  Sₙ = 0 # liczba przestawień
  if right - left >= 1
    p = A[left]; q = A[right]
    if p > q
      Cₙ = Cₙ + 1
      p = A[right]
      q = A[left]
      A[left], A[right] = A[right], A[left]; Sₙ = Sₙ + 1
    end
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
  EC = ((19/10)*(n + 1)*Harmonic(n+1)) - ((711/200)*(n+1)) + (3/2)
  ES = ((3/5)*(n+1)*Harmonic(n+1)) - ((27/100)*(n+1)) - (7/12)
  println("EC  = ", EC, ", ES  = ", ES)
  println("VCₙ = ", VCₙ, ", VSₙ = ", VSₙ)
  return CₙResults, SₙResults, ECₙ, VCₙ
end # testQuickSort

# testQuickSort(100, 10)
testDualPivotQuickSort(100, 10)


# m - liczba eksperymentów, n - długość tablicy
function plotDualPivotQuickSort(m, n)
  x = []
  CₙResults = []
  SₙResults = []
  ECₙResults = []
  VCₙResults = []
  Bounds1 = []
  Bounds2 = []
  for i in 1:n
    push!(x, i)
    Cₙ, Sₙ, ECₙ, VCₙ = testDualPivotQuickSort(m, i) # Zamienione oznaczenie Cₙ i Sₙ (w tym miejscu to są tablice)
    push!(CₙResults, Cₙ)
    push!(SₙResults, Sₙ)
    push!(ECₙResults, ECₙ)
    push!(VCₙResults, VCₙ)
    push!(Bounds1, ECₙ + 2*sqrt(VCₙ))
    push!(Bounds2, ECₙ - 2*sqrt(VCₙ))
  end
  PyPlot.plot(x, CₙResults, color="red", "o")
  PyPlot.plot(x, ECₙResults, color="blue", linestyle="-")
  PyPlot.plot(x, Bounds1, color="black", linestyle="--")
  PyPlot.plot(x, Bounds2, color="black", linestyle="--")
  title("Wyniki")
end #plotDualPivotQuickSort

plotDualPivotQuickSort(10, 100)
