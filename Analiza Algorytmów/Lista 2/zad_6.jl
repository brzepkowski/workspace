using PyPlot
using Nettle
using Roots

function ϵ(x, A)
  return in(x, A)
end

function max_val(len)
  sum = BigInt(0)
  multiplier = BigInt(1)
  for x in 1:len
    sum = sum + multiplier
    multiplier = multiplier * 2
  end
  return sum
end

function h_(x, mode::String, len)
  hash = parse(BigInt, hexdigest(mode, string(x)), 16)
  max = max_val(len)
  return BigFloat(hash / max)
end

function h(x)
  return h_(x, "md5", 128)
end

function counting(k, Multiset)
  M = fill(BigFloat(1.0), k)
  for x in Multiset
    h_x = h(x)
    if h_x < M[k] && ϵ(h_x, M) == false
      M[k] = h_x
      M = sort(M)
    end
  end
  if M[k] == 1
    return length([x for x in M if x != 1.0])
  else
    return Float64((k - 1) / M[k])
  end
end

function czebyszew(k, n, α)
  δ = sqrt((n - k + 1) / (n * (k - 2))) / sqrt(α)
  return δ
end

function chernoff_function(δ, k, α)
  return exp((k*δ - 1) / (1 + δ)) * (1 - ((k*δ - 1) / k*(1 + δ)))^k + exp((1 - k*δ) / (1 - δ)) * (1 + ((k*δ - 1) / k*(1 - δ)))^k - α
end

function chernoff(k, n, α)
  solutions = fzeros(δ -> chernoff_function(δ, k, α), 0.0, 2.0)
  minSolution = Inf
  for solution in solutions
    if solution < minSolution
      minSolution = solution
    end
  end
  return minSolution
end

function generate_Bounds(k, n, α)
  x = collect(1:n)
  y = Float64[]
  for i in 1:n
    push!(y, counting(k, collect(1:i)) / i)
  end
  close()
  PyPlot.plot(x, y, "bo")
  for i in 1:n
    y[i] = abs(y[i] - 1)
  end
  sort!(y)
  δ0 = y[Int(ceil(n*(1 - α)))] # May be even better
  δ1 = czebyszew(k, n, α)
  δ2 = chernoff(k, n, α)
  PyPlot.fill_between(x, 1 - δ0, 1 + δ0, alpha = 0.5)
  PyPlot.fill_between(x, 1 - δ1, 1 + δ1, alpha = 0.5)
  PyPlot.fill_between(x, 1 - δ2, 1 + δ2, alpha = 0.5)
end

generate_Bounds(400, 1000, 0.05)
