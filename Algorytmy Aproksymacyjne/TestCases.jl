# Copyright (c) 2018 Mateusz K. Pyzik, all rights reserved.
include("EnableLocalModules.jl")
module TestCases

using IntegerProgramming
using Distributions


export easyTestCase
export uniformTestCaseLowerBound
export uniformTestCaseOptimum
export exponentialTestCase
export cauchyTestCaseLowerBound
export paretoTestCaseLowerBound

struct TestCase
  time :: Vector{Int}
  machines :: Int
  makespan :: Int
end

function easyTestCase(jobs, machines, typicalJobTime)
  time = rand(1:2typicalJobTime, jobs-1)
  makespan = sum(time)
  push!(time, makespan)
  return TestCase(time, machines, makespan)
end

function parallelTestCase(jobs, maxJobTime)
  time = rand(1:maxJobTime, jobs)
  makespan = maximum(time)
  return TestCase(time, jobs, makespan)
end

function uniformTestCaseLowerBound(jobs, machines, maxJobTime)
  time = rand(1:maxJobTime, jobs)
  makespan = convert(Int64, round(linearProgrammingLowerBound(time, machines)))
  return TestCase(time, machines, makespan)
end

function uniformTestCaseOptimum(jobs, machines, maxJobTime)
  time = rand(1:maxJobTime, jobs)
  _, makespan = integerProgramming(time, machines)
  return TestCase(time, machines, makespan)
end

function exponentialTestCase(jobs, machines, f)
  time = randexp(jobs)
  timeInt = map(x -> convert(Int64, round(1000*x)), time)
  _, makespan = integerProgramming(timeInt, machines)
  return TestCases.TestCase(timeInt, machines, makespan)
end

function paretoTestCaseLowerBound(jobs, machines, maxJobTime)
  time = rand(Pareto(100, 100), jobs)
  timeInt = map(x -> convert(Int64, abs(round(x))), time)
  makespan = convert(Int64, round(linearProgrammingLowerBound(timeInt, machines)))
  return TestCases.TestCase(timeInt, machines, makespan)
end

function cauchyTestCaseLowerBound(jobs, machines, maxJobTime)
  time = rand(Cauchy(100, 100), jobs)
  timeInt = map(x -> convert(Int64, abs(round(x))), time)
  makespan = convert(Int64, round(linearProgrammingLowerBound(timeInt, machines)))
  return TestCases.TestCase(timeInt, machines, makespan)
end



end
