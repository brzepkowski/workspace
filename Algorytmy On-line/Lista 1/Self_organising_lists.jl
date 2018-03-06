using StatsBase

function HarmonicNumber(n)
    harmonic_number = 0
    for i in 1:n
        harmonic_number += 1/i
    end
    return harmonic_number
end # HarmonicNumber

function Access(elem, list)
    list_len = length(list)
    i = 0
    while list[i] != elem && i < list_len
        i += 1
    end # while
    if i == list_len
        push!(list, elem)
    end
    return i
end # Access

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

list1 = collect(1:100)
println(UniformDistribution(list, 10))
println(HarmonicDistribution(list, 10))
println(GeometricDistribution(list, 10))
