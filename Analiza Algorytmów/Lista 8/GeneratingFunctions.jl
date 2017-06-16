# Zadanie 13
function f(n, calls)
  calls = 1
  s = 0
  if (n == 0)
    return (1, calls)
  else
    for i in 0:n-1
      (sˈ, callsˈ) = f(i, calls)
      s += sˈ
      calls += callsˈ
    end # for
    return (s, calls)
  end
end # f

for i in 0:10
  println(f(i, 0))
end

# Funkcja tworząca: ∑(2x)ⁿ = 1 / (1 - 2x)
