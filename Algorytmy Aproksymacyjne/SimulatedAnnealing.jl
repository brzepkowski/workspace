# Copyright (c) 2018 Mateusz K. Pyzik, all rights reserved.
module SimulatedAnnealing

export simulatedAnnealing

function f(time :: Vector{Int}, machines :: Int, x :: Vector{Int})
  jobs = length(time)
  processingTime = zeros(Int, machines)
  for j = 1:jobs
    processingTime[x[j]] += time[j]
  end
  return maximum(processingTime)
end

function randomStart(time :: Vector{Int}, machines :: Int)
  jobs = length(time)
  x = [rand(1:machines) for i=1:jobs]
  return x, f(time, machines, x)
end

function randomNeighbour(time :: Vector{Int}, machines :: Int,
    x₀ :: Vector{Int})
  jobs = length(time)
  job = rand(1:jobs)
  machine = rand(2:machines)
  if x₀[job] == machine
    machine = 1
  end
  x = copy(x₀)
  x[job] = machine
  return x, f(time, machines, x)
end

function simulatedAnnealing(time :: Vector{Int}, machines :: Int,
    Tmax :: Float64,
    Tmin :: Float64 = 1e-4*Tmax,
    maxit :: Int = 100000)
  β = (Tmax-Tmin)/(maxit*Tmax*Tmin)
  x₀, fx₀ = randomStart(time, machines)
  T = Tmax
  xₒₚₜ = x₀
  fxₒₚₜ = fx₀
  for step = 1:maxit
    x, fx = randomNeighbour(time, machines, x₀)
    if rand() < exp((fx₀-fx)/T)
      x₀ = x
      fx₀ = fx
      if fx₀ < fxₒₚₜ
        xₒₚₜ = x₀
        fxₒₚₜ = fx₀
      end
    end
    T /= 1 + β * T
  end
  solution = [Set{Int}() for i=1:machines]
  for (j, i) in enumerate(xₒₚₜ)
    push!(solution[xₒₚₜ[j]], j)
  end
  makespan = fxₒₚₜ
  return solution, makespan
end

end
