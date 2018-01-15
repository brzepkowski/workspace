# Copyright (c) 2017 Bartosz Rzepkowski, all rights reserved.
module LocalSearchHeuristic

export localSearchHeuristic

function Makespan(scheme)
  makespan = maximum(sum(scheme[i]) for i=1:length(scheme) if !isempty(scheme[i]))
#    makespan = 0
#    for s in scheme
#        makespanˈ = sum(s)
#        if makespanˈ > makespan
#            makespan = makespanˈ
#        end
#    end
  return makespan
end # Makespan

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

function FindMaxMakespanIndex(machinesJobs)
    index = 0
    maxMakespan = 0
    for (i, jobs) in enumerate(machinesJobs)
        if jobs != []
            makespan = sum(jobs)
        else
            makespan = 0
        end
        if makespan > maxMakespan
            maxMakespan = makespan
            index = i
        end
    end
    return index
end # FindMaxMakespanIndex

function FindRandomSolution(times, m)
    machines = zeros(m)
    machinesJobs = Array{Any}(m)
    for i in 1:m
        machinesJobs[i] = []
    end
    for t in times
        i = rand(1:m)
        push!(machinesJobs[i], t)
    end
    return machinesJobs
end # FindRandomSolution

function ReassignmentNeighbourhood(s, m)
    while true
        mMax = FindMaxMakespanIndex(s)
        JMax = s[mMax]
        EMax = []
        for j in JMax # j jest w tym miejscu czasem wykonania danej pracy, a nie indeksem pracy
            for i in 1:m
                if i != mMax
                    push!(EMax, (i, j))
                end
            end
        end
        g = deepcopy(s)
        while Makespan(s) <= Makespan(g)
            g = deepcopy(s)
            if EMax == []
                return g
            else
                (i, j) = pop!(EMax)
                deleteat!(g[mMax], findin(g[mMax], [j]))
                push!(g[i], j)
            end
        end
        s = g
    end
end # ReassignmentNeighbourhood

function InterchangeNeighbourhood(schedule, m)
    i₁ = 1; i₂ = 1
    exchange = true
    g = deepcopy(schedule)
    while true
        M = []
        for (i, s) in enumerate(schedule)
            if s != []
                push!(M, (sum(s), i))
            else
                push!(M, (0, i))
            end
        end

        M₁ = sort(M, rev=true); M₂ = sort(M, rev=false)
        if exchange
            i₁ = 1; i₂ = 1
            m₁ = M₁[i₁][2]; m₂ = M₂[i₂][2]
        end
        J₁ = schedule[m₁]; J₂ = schedule[m₂]
        k = 1
        j = 1
        exchange = false
        while !exchange && k <= length(J₁)
            g = deepcopy(schedule)
            if k <= length(J₁) && j <= length(J₂)
                j₁ = J₁[k]
                j₂ = J₂[j]
                g = SwapJobs(g, m₁, m₂, j₁, j₂)
            end
            if Makespan(g) < Makespan(schedule)
                exchange = true
            else
                j += 1
            end
            if j >= length(J₂)
                k += 1
                j = 1
            end
        end
        if !exchange
            if m₁ == M₂[1][2]
                break
            end
            if m₁ == m₂
                i₁ += 1
                m₁ = M₁[i₁][2]
                i₂ = 1
                m₂ = M₂[1][2]
            else
                i₂ += 1
                m₂ = M₂[i₂][2]
            end
        else
            schedule = g
        end
    end
    return schedule
end # InterchangeNeighbourhood

function SwapJobs(schedule, m₁, m₂, j₁, j₂)
    deleteat!(schedule[m₁], findin(schedule[m₁], [j₁]))
    deleteat!(schedule[m₂], findin(schedule[m₂], [j₂]))
    push!(schedule[m₁], j₂)
    push!(schedule[m₂], j₁)
    return schedule
end # SwapJobs

function localSearchHeuristic(times, m)
    s = FindRandomSolution(times, m)
    sˈ = ReassignmentNeighbourhood(s, m)
    sˈˈ = InterchangeNeighbourhood(sˈ, m)
    return sˈˈ, Makespan(sˈˈ)
end # LocalSearchHeuristic

end
