using StatsBase
using PyPlot

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
    average_cost_uniform = sum(costs)
    # println(average_cost_uniform)

    # Harmonic distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_harmonic_list
        push!(costs, Access(i, self_organised_list))
    end
    average_cost_harmonic = sum(costs)
    # println(average_cost_harmonic)

    # Geometric distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_geometric_list
        push!(costs, Access(i, self_organised_list))
    end
    average_cost_geometric = sum(costs)
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
    average_cost_uniform = sum(costs)
    # println(average_cost_uniform)

    # Harmonic distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_harmonic_list
        push!(costs, AccessCounters(i, self_organised_list, counters))
    end
    average_cost_harmonic = sum(costs)
    # println(average_cost_harmonic)

    # Geometric distribution
    self_organised_list = Vector{Int64}()
    costs = []
    for i in sample_geometric_list
        push!(costs, AccessCounters(i, self_organised_list, counters))
    end
    average_cost_geometric = sum(costs)
    # println(average_cost_geometric)
    return (average_cost_uniform, average_cost_harmonic, average_cost_geometric)
end

function PlotResults(x, y₁, y₂, y₃, y₄, title_str)
    fig, ax = PyPlot.subplots()

    ax[:plot](x, y₁, label="No self organisation", color="blue", "-")
    ax[:plot](x, y₂, label="Move to front", color="black", "-")
    ax[:plot](x, y₃, label="Transpose", color="red", "-")
    ax[:plot](x, y₄, label="Count", color="green", "-")
    ax[:legend](loc="best")

    grid("on")
    xlabel("Number of accesses - n")
    ylabel("Average cost of n accesses")
    title("$title_str")
end


# Main part of the program
list = collect(1:100)
N = [100, 500, 1000, 5000, 10000, 50000, 100000]
t = 1000 # Number of experiments
no_self_organisation_uni = []
no_self_organisation_harm = []
no_self_organisation_geo = []

move_to_front_uni = []
move_to_front_harm = []
move_to_front_geo = []

transpose_uni = []
transpose_harm = []
transpose_geo = []

count_uni = []
count_harm = []
count_geo = []

for n in N
    println("(", n, ")")

    # No self organisation
    unitary_results₁ = []
    harmonic_results₁ = []
    geometric_results₁ = []
    for i in 1:t
        (unitary_result, harmonic_result, geometric_result) = Experiment(list, n, AccessNoSelfOrganisation)
        push!(unitary_results₁, unitary_result)
        push!(harmonic_results₁, harmonic_result)
        push!(geometric_results₁, geometric_result)
    end
    push!(no_self_organisation_uni, mean(unitary_results₁))
    push!(no_self_organisation_harm, mean(harmonic_results₁))
    push!(no_self_organisation_geo, mean(geometric_results₁))

    # Move to front
    unitary_results₂ = []
    harmonic_results₂ = []
    geometric_results₂ = []
    for i in 1:t
        (unitary_result, harmonic_result, geometric_result) = Experiment(list, n, AccessMoveToFront)
        push!(unitary_results₂, unitary_result)
        push!(harmonic_results₂, harmonic_result)
        push!(geometric_results₂, geometric_result)
    end
    push!(move_to_front_uni, mean(unitary_results₂))
    push!(move_to_front_harm, mean(harmonic_results₂))
    push!(move_to_front_geo, mean(geometric_results₂))

    # Transpose
    unitary_results₃ = []
    harmonic_results₃ = []
    geometric_results₃ = []
    for i in 1:t
        (unitary_result, harmonic_result, geometric_result) = Experiment(list, n, AccessTranspose)
        push!(unitary_results₃, unitary_result)
        push!(harmonic_results₃, harmonic_result)
        push!(geometric_results₃, geometric_result)
    end
    push!(transpose_uni, mean(unitary_results₃))
    push!(transpose_harm, mean(harmonic_results₃))
    push!(transpose_geo, mean(geometric_results₃))

    # Counters
    unitary_results₄ = []
    harmonic_results₄ = []
    geometric_results₄ = []
    for i in 1:t
        (unitary_result, harmonic_result, geometric_result) = ExperimentCounters(list, n)
        push!(unitary_results₄, unitary_result)
        push!(harmonic_results₄, harmonic_result)
        push!(geometric_results₄, geometric_result)
    end
    push!(count_uni, mean(unitary_results₄))
    push!(count_harm, mean(harmonic_results₄))
    push!(count_geo, mean(geometric_results₄))
end # for

println("Uniform:")
println("No self organisation: ", no_self_organisation_uni)
println("Move to front: ", move_to_front_uni)
println("Transpose: ", transpose_uni)
println("Count: ", count_uni)

println("Harmonic:")
println("No self organisation: ", no_self_organisation_harm)
println("Move to front: ", move_to_front_harm)
println("Transpose: ", transpose_harm)
println("Count: ", count_harm)

println("Geometric:")
println("No self organisation: ", no_self_organisation_geo)
println("Move to front: ", move_to_front_geo)
println("Transpose: ", transpose_geo)
println("Count: ", count_geo)

PlotResults(N, no_self_organisation_uni, move_to_front_uni, transpose_uni, count_uni, "Uniform distribution")
PlotResults(N, no_self_organisation_harm, move_to_front_harm, transpose_harm, count_harm, "Harmonic distribution")
PlotResults(N, no_self_organisation_geo, move_to_front_geo, transpose_geo, count_geo, "Geometric distribution")
