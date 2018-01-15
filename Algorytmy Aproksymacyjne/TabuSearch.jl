# Copyright (c) 2017 Bartosz Rzepkowski, all rights reserved.
module TabuSearch

export tabuSearch

function Makespan(scheme)
  makespan = maximum(sum(scheme[i]) for i=1:length(scheme) if !isempty(scheme[i]))
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

function FindMinMakespanIndex(machinesJobs)
    index = 0
    maxMakespan = typemax(Int64)
    for (i, jobs) in enumerate(machinesJobs)
        if jobs != []
            makespan = sum(jobs)
        else
            makespan = 0
        end
        if makespan < maxMakespan
            maxMakespan = makespan
            index = i
        end
    end
    return index
end # FindMinMakespanIndex

function SwapJobsAndCountMakespan(machineTasks₁, machineTasks₂, t₁, t₂)
    # println("t₁: ", t₁)
    # println("t₂: ", t₂)
    # println("1) machineTasks₁: ", machineTasks₁)
    # println("1) machineTasks₂: ", machineTasks₂)
    deleteat!(machineTasks₁, findin(machineTasks₁, t₁)[1])
    deleteat!(machineTasks₂, findin(machineTasks₂, t₂)[1])
    push!(machineTasks₁, t₂)
    push!(machineTasks₂, t₁)
    # println("2) machineTasks₁: ", machineTasks₁)
    # println("2) machineTasks₂: ", machineTasks₂)
    if machineTasks₁ != []
        makespan₁ = sum(machineTasks₁)
    else
        makespan₁ = 0
    end
    if machineTasks₂ != []
        makespan₂ = sum(machineTasks₂)
    else
        makespan₂ = 0
    end
    return maximum([makespan₁, makespan₂])
end

function Termination(x)
    if x >= 20
        return true
    else
        return false
    end
end

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

function tabuSearch(times, machines, L)
    i = 0
    n = length(times)
    M = zeros(n)
    for j in 1:n
        M[j] = typemin(Int64)
    end
    W = FindRandomSolution(times, machines)
    D = zeros(machines)
    for j in 1:machines
        if W[j] != []
            D[j] = sum(W[j])
        else
            D[j] = 0
        end
    end
    dᵦ = typemax(Int64)
    Wᵦ = []
    tabuList = []
    done = false

    # println("M: ", M)
    # println("W: ", W)
    # println("D: ", D)
    # println("dᵦ: ", dᵦ)
    lastImprovement = 0
    while !done
        # println("W: ", W)
        i += 1
        m = FindMaxMakespanIndex(W)
        D = zeros(machines)
        for j in 1:machines
            if W[j] != []
                D[j] = sum(W[j])
            else
                D[j] = 0
            end
        end
        d = D[m]
        if d < dᵦ
            dᵦ = d
            Wᵦ = deepcopy(W)
        else
            lastImprovement += 1
        end
        if !Termination(lastImprovement)
            # push!(tabuList, W)
            if length(tabuList) > L
                deleteat!(tabuList, 1)
            end
            j = FindMinMakespanIndex(W)
            # Znajdź najlepsze zadania do wymieny między maszynami m i j
            r = -1; s = -1
            if m != j
                MJobs = W[m]
                JJobs = W[j]
                push!(JJobs, 0)
                # println("MJobs: ", MJobs)
                # println("JJobs: ", JJobs)
                # sleep(2)
                if MJobs != []
                    makespanₘ = sum(MJobs)
                else
                    makespanₘ = 0
                end
                if JJobs != []
                    makespanⱼ = sum(JJobs)
                else
                    makespanⱼ = 0
                end
                makespan = maximum([makespanₘ, makespanⱼ])
                for t₁ in MJobs
                    if !in(t₁, tabuList)
                        for t₂ in JJobs
                            if !in(t₂, tabuList)
                                # sleep(0.25)
                                machineTasks₁ = deepcopy(MJobs)
                                machineTasks₂ = deepcopy(JJobs)
                                swapMakespan = SwapJobsAndCountMakespan(machineTasks₁, machineTasks₂, t₁, t₂)
                                # println("swap makespan: ", swapMakespan)
                                # println("makespan: ", makespan)
                                if swapMakespan < makespan
                                    r = t₁
                                    s = t₂
                                    makespan = swapMakespan
                                end
                            end
                        end
                    end
                end
                pop!(JJobs)
            end
            # println("1) r: ", r)
            # println("1) s: ", s)
            iᵣ = 0; rRandom = false
            if r == -1
                indices = collect(1:machines)
                deleteat!(indices, findin(indices, j)[1])
                iᵣ = rand(indices)
                while W[iᵣ] == [] && indices != []
                    deleteat!(indices, findin(indices, iᵣ)[1])
                    iᵣ = rand(indices)
                end
                if indices != []
                    r = rand(W[iᵣ])
                    rRandom = true
                end
            end
            # println("2) r: ", r)
            # println("2) s: ", s)
            # println("m: ", m)
            # println("j: ", j)
            # Move task r to processor j
            if rRandom
                if r != -1
                    deleteat!(W[iᵣ], findin(W[iᵣ], r)[1])
                    push!(W[j], r)
                end
            else
                deleteat!(W[m], findin(W[m], r)[1])
                push!(W[j], r)
            end
            push!(tabuList, r)
            if s > 0
                # Move task s to processor m
                deleteat!(W[j], findin(W[j], s)[1])
                push!(W[m], s)
                push!(tabuList, s)
            end
        else
            done = true
        end
    end
    return Wᵦ, Makespan(Wᵦ)
end # TabuSearch

end
