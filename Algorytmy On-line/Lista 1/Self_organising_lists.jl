using StatsBase

function HarmonicNumber(n)
    harmonic_number = 0
    for i in 1:n
        harmonic_number += 1/i
    end
    return harmonic_number
end # HarmonicNumber

# Three functions to get sample values from given sets (ex. {1, ..., 100})

# list - list from which we will be getting samples, m - number of samples
function UniformDistribution(list, m)
    n = length(list)
    weights = Array{Float64}(n)
    for i in 1:n
        weights[i] = 1/n
    end
    results = []
    for i in 1:m
        push!(results, sample(list, Weights(weights)))
    end
    return results
end # UniformDistribution

# list - list from which we will be getting samples, m - number of samples
function HarmonicDistribution(list, m)
    n = length(list)
    nth_harmonic_number = HarmonicNumber(n)
    weights = Array{Float64}(n)
    for i in 1:n
        weights[i] = 1/(list[i]*nth_harmonic_number)
    end
    results = []
    for i in 1:m
        push!(results, sample(list, Weights(weights)))
    end
    return results
end # HarmonicDistribution

# list - list from which we will be getting samples, m - number of samples
function GeometricDistribution(list, m)
    n = length(list)
    nth_harmonic_number = HarmonicNumber(n)
    weights = Array{BigFloat}(n)
    for i in 1:n
        if list[i] >= 100
            denominator = exp2(BigInt(99))
        else
            denominator = exp2(BigInt(list[i]))
        end
        weights[i] = BigFloat(1/denominator)
    end
    results = []
    for i in 1:m
        push!(results, sample(list, Weights(weights)))
    end
    return results
end # GeometricDistribution

function AccessNoSelfOrganisation(elem, list)
    list_len = length(list)
    i = 0 # Beginning cost of operations
    while i < list_len
        if list[i+1] == elem
            return i+1
        end # if
        i += 1
    end # while
    if i == list_len
        push!(list, elem)
    end
    return i # Return total cost of operations
end # AccessNoSelfOrganisation

function AccessMoveToFront(elem, list)
    list_len = length(list)
    i = 0 # Beginning cost of operations
    while i < list_len
        if list[i+1] == elem
            deleteat!(list, i+1)
            unshift!(list, elem)
            return i+1
        end # if
        i += 1
    end # while
    if i == list_len
        push!(list, elem)
    end
    return i # Return total cost of operations
end # AccessMoveToFront

function AccessTranspose(elem, list)
    list_len = length(list)
    i = 0 # Beginning cost of operations
    while i < list_len
        if list[i+1] == elem
            if i > 0
                list[i], list[i+1] = list[i+1], list[i]
            end
            return i+1
        end # if
        i += 1
    end # while
    if i == list_len
        push!(list, elem)
    end
    return i # Return total cost of operations
end # AccessTranspose

function AccessCounters(elem, list, counters)
    list_len = length(list)
    i = 0 # Beginning cost of operations
    while i < list_len
        if list[i+1] == elem
            counters[elem] += 1
            j = i
            k = i+1
            while j >= 1
                if counters[list[j]] < counters[elem]
                    list[j], list[k] = list[k], list[j]
                    k = j
                end
                j -= 1
            end
            return i+1
        end # if
        i += 1
    end # while
    if i == list_len
        push!(list, elem)
    end
    return i # Return total cost of operations
end # AccessCounters

# list - set from which we will be getting our samples (ex. {1,2,...,n}), n - size of sample, Access - type of access function
function Experiment(list, n, Access)
    sample_uniform_list = UniformDistribution(list, n)
    sample_harmonic_list = HarmonicDistribution(list, n)
    sample_geometric_list = GeometricDistribution(list, n)

    # Uniform distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_uniform_list
        push!(costs, Access(i, self_organised_list))
    end
    average_cost_uniform = sum(costs)/length(costs)
    # println(average_cost_uniform)

    # Harmonic distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_harmonic_list
        push!(costs, Access(i, self_organised_list))
    end
    average_cost_harmonic = sum(costs)/length(costs)
    # println(average_cost_harmonic)

    # Geometric distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_geometric_list
        push!(costs, Access(i, self_organised_list))
    end
    average_cost_geometric = sum(costs)/length(costs)
    # println(average_cost_geometric)
    return (average_cost_uniform, average_cost_harmonic, average_cost_geometric)
end

# list - set from which we will be getting our samples (ex. {1,2,...,n}), n - size of sample, Access - type of access function
function ExperimentCounters(list, n)
    sample_uniform_list = UniformDistribution(list, n)
    sample_harmonic_list = HarmonicDistribution(list, n)
    sample_geometric_list = GeometricDistribution(list, n)

    # Uniform distribution
    self_organised_list = Vector{Int64}()
    counters = Dict{Int64, Int64}()
    for i in list
        counters[i] = 0
    end
    costs = []
    for i in sample_uniform_list
        push!(costs, AccessCounters(i, self_organised_list, counters))
    end
    average_cost_uniform = sum(costs)/length(costs)
    # println(average_cost_uniform)

    # Harmonic distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_harmonic_list
        push!(costs, AccessCounters(i, self_organised_list, counters))
    end
    average_cost_harmonic = sum(costs)/length(costs)
    # println(average_cost_harmonic)

    # Geometric distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_geometric_list
        push!(costs, AccessCounters(i, self_organised_list, counters))
    end
    average_cost_geometric = sum(costs)/length(costs)
    # println(average_cost_geometric)
    return (average_cost_uniform, average_cost_harmonic, average_cost_geometric)
end

list = collect(1:100)
n = 1000
t = 1000 # Number of experiments

# No self organisation
unitary_results = []
harmonic_results = []
geometric_results = []
for i in 1:t
    (unitary_result, harmonic_result, geometric_result) = Experiment(list, n, AccessNoSelfOrganisation)
    push!(unitary_results, unitary_result)
    push!(harmonic_results, harmonic_result)
    push!(geometric_results, geometric_result)
end
println(mean(unitary_results), ", ", mean(harmonic_results), ", ", mean(geometric_results))
# println(TestMoveToFront(list, n))

# Move to front
unitary_results = []
harmonic_results = []
geometric_results = []
for i in 1:t
    (unitary_result, harmonic_result, geometric_result) = Experiment(list, n, AccessMoveToFront)
    push!(unitary_results, unitary_result)
    push!(harmonic_results, harmonic_result)
    push!(geometric_results, geometric_result)
end
println(mean(unitary_results), ", ", mean(harmonic_results), ", ", mean(geometric_results))

# Transpose
unitary_results = []
harmonic_results = []
geometric_results = []
for i in 1:t
    (unitary_result, harmonic_result, geometric_result) = Experiment(list, n, AccessTranspose)
    push!(unitary_results, unitary_result)
    push!(harmonic_results, harmonic_result)
    push!(geometric_results, geometric_result)
end
println(mean(unitary_results), ", ", mean(harmonic_results), ", ", mean(geometric_results))

# Counters
unitary_results = []
harmonic_results = []
geometric_results = []
for i in 1:t
    (unitary_result, harmonic_result, geometric_result) = ExperimentCounters(list, n)
    push!(unitary_results, unitary_result)
    push!(harmonic_results, harmonic_result)
    push!(geometric_results, geometric_result)
end
println(mean(unitary_results), ", ", mean(harmonic_results), ", ", mean(geometric_results))
