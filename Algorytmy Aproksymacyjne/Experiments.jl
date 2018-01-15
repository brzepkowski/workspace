# Copyright (c) 2018 Aleksander Spyra, all rights reserved.
# Experiments for P||C_max
include("EnableLocalModules.jl")
using TestCases
using LargestProcessingTime
using LocalSearchHeuristic
using ExoticAlgorithms
using TabuSearch

function testSet(machines, testCaseGenerator :: Function, farg)
  for i ∈ 10:10:100
    testCase = testCaseGenerator(i, machines, farg)
    times = testCase.time
    ms = testCase.makespan
    a1 = listSchedulingAlgorithm(times, machines)
    a2 = largestProcessingTime(times, machines)
    a3 = zkAlgorithm(times, machines, 5)
    a4 = multifit(times, machines, farg)
    a5 = localSearchHeuristic(times, machines)
    a5 = tabuSearch(times, machines, 5)
    # a6 = symulowane wyżarzanie
    println("$i & $(testCase.makespan) & $(a1[2]) & $(a2[2]) & $(a3[2]) & $(a4[2]) & $(a5[2])\\\\ \\hline")
    println("   & OPT & $((a1[2]/ms)) & $(a2[2]/ms) & $(a3[2]/ms) & $(a4[2]/ms) & $(a5[2]/ms) \\\\ \\hline")
  end
end

# 18,6,20
 testSet(10, uniformTestCaseOptimum, 10)

 testSet(3, easyTestCase, 10)
# testSet(3, uniformTestCaseOptimum, 10)

 testSet(3, uniformTestCaseLowerBound, 10)
#

testSet(3, cauchyTestCaseLowerBound, 10)
testSet(3, paretoTestCaseLowerBound, 10)
