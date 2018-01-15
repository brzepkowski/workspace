# Copyright (c) 2018 Aleksander Spyra, all rights reserved.

module ExoticAlgorithms

using IntegerProgramming

export zkAlgorithm
export multifit

function zkAlgorithm(times, m, k)
  machines = zeros(Int64, m)
  sorted = sort(times, rev=true)
  kBests = sorted[1:k]
  assignment, _ = integerProgramming(kBests, m)
  for i ∈ 1:m
    if length(assignment[i]) != 0
      machines[i] = sum(x -> kBests[x], assignment[i])
    end
  end
  for p in k+1:length(sorted)
    index = indmin(machines)
    push!(assignment[index], p)
    machines[index] += sorted[p]
  end
  return assignment, maximum(machines)
end

function multifit(times, machines, kf)
  function calcCl(T, m)
    a = (1/m) * sum(T)
    b = maximum(T)
    return max(a, b)
  end
  function calcCu(T, m)
    a = (2/m) * sum(T)
    b = maximum(T)
    return max(a, b)
  end
  function ffd(T, C)
    P = [[] for i in 1:length(T)]
    for j in T
      k = findfirst(x -> if !isempty(x) sum(x) + j else false end <= C, P)
      push!(P[k], j)
    end
    return P, maximum(sum(P[i]) for i=1:length(P) if !isempty(P[i])), length(find(!isempty,P))
  end

  timesSorted = sort(times, rev=true)
  cl = []
  cu = []
  push!(cl, calcCl(timesSorted, machines))
  push!(cu, calcCu(timesSorted, machines))
  k = 50 + kf*10
  assignment = [[]]
  ms = 0
  for i in 2:k
    c = (cl[i-1] + cu[i-1])/2
    assignment, ms, result = ffd(timesSorted, c)
    if result <= machines
      push!(cu, c)
      push!(cl, cl[i-1])
    else
      push!(cl, c)
      push!(cu, cu[i-1])
    end
  end
  return assignment[1:findfirst(x -> isempty(x), assignment)-1], ms
end

end
