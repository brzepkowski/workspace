# Copyright (c) 2018 Mateusz K. Pyzik, all rights reserved.
include("EnableLocalModules.jl")
module IntegerProgramming

using JuMP
using Cbc

export linearProgrammingLowerBound
export integerProgramming

function linearProgrammingLowerBound(time :: Vector{Int}, machines :: Int)
  return sum(time) / machines
end

function integerProgramming(time :: Vector{Int}, machines :: Int)
  jobs = length(time)
  m = Model(solver=CbcSolver())
  @variables m begin
    x[i=1:machines,j=1:jobs], Bin
    ms >= 0
  end
  @objective(m, Min, ms)
  @constraints m begin
    [j=1:jobs], sum(x[i,j] for i=1:machines) == 1
    [i=1:machines], ms >= sum(x[i,j]*time[j] for j=1:jobs)
  end
  status = solve(m)
  @assert status == :Optimal "Cannot happen, solution always exists!"
  table = getvalue(x)
  assignment = [Set(j for j=1:jobs if table[i,j] > 0.5) for i=1:machines]
  makespan = Int(round(getvalue(ms)))
  return assignment, makespan
end

end
