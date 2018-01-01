# Copyright (c) 2017 Bartosz Rzepkowski, all rights reserved.

p = [2,2,2,3,3,5,2,3,4,5,5,6,5,4,3,3,5,4,5,4,5,3] # czasy wykonania zadań

# times - czasy wykonania zadań, m - liczba maszyn
function ListAlgorithm(times, m)
    machines = Array{Int64}(m)
    beginnings = Array{Any}(m)
    x = []
    for i in 1:m
        machines[i] = 0
        push!(x, i)
    end
    for i in 1:m
        beginnings[i] = []
    end
    sortedTimes = sort(times, rev=true)
    FindMinIndex(sortedTimes)
    for p in sortedTimes
        index = FindMinIndex(machines)
        push!(beginnings[index], machines[index])
        machines[index] += p
    end
    println(machines)
    println(beginnings)
end # ListAlgorithm

# Znajdź indeks komórki przechowującej najmniejszą wartość
function FindMinIndex(arr)
    index = 0
    min = typemax(Int64)
    for (i, v) in enumerate(arr)
        if v < min
            min = v
            index = i
        end
    end
    return index
end # FindMinIndex

ListAlgorithm(p, 10)
