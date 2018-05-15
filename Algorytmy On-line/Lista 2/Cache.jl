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
function DoubleHarmonicDistribution(list, m)
    n = length(list)
    nth_harmonic_number = HarmonicNumber(n)
    weights = Array{Float64}(n)
    for i in 1:n
        weights[i] = 1/(list[i]^2*nth_harmonic_number)
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

# --------------------------------------------------

function FirstInFirstOut(elem, list, cache_size)
    if find(list .== elem) != []
        return 0
    else
        if length(list) < cache_size
            push!(list, elem)
        else
            deleteat!(list, 1)
            push!(list, elem)
        end
        return 1
    end
end # FirstInFirstOut

function FlushWhenFull(elem, list, cache_size)
    if find(list .== elem) != []
        return 0
    else
        if length(list) < cache_size
            push!(list, elem)
        else
            empty!(list)
            push!(list, elem)
        end
        return 1
    end
end # FlushWhenFull

function LeastRecentlyUsed(elem, list, cache_size, t)
    index = 0
    list_len = length(list)
    for i in 1:list_len
        if list[i][1] == elem
            index = i
            break
        end
    end
    if index != 0
        list[index] = (elem, t)
        return 0
    else
        if length(list) < cache_size
            push!(list, (elem, t))
        else
            min = typemax(Int64)
            min_index = 0
            for i in 1:length(list)
                if list[i][2] < min
                    min = list[i][2]
                    min_index = i
                end
            end
            deleteat!(list, min_index)
            push!(list, (elem, t))
        end
        return 1
    end
end # LeastRecentlyUsed

function LeastFrequentlyUsed(elem, list, cache_size)
    index = 0
    list_len = length(list)
    for i in 1:list_len
        if list[i][1] == elem
            index = i
            break
        end
    end
    if index != 0
        list[index] = (elem, list[index][2] + 1)
        return 0
    else
        if length(list) < cache_size
            push!(list, (elem, 1))
        else
            min = typemax(Int64)
            min_index = 0
            for i in 1:length(list)
                if list[i][2] < min
                    min = list[i][2]
                    min_index = i
                end
            end
            deleteat!(list, min_index)
            push!(list, (elem, 1))
        end
        return 1
    end
end # LeastFrequentlyUsed

function Random(elem, list, cache_size)
    # println("elem:", elem)
    # println("cache: ", list)
    if find(list .== elem) != []
        # println("Weszło 1)")
        return 0
    else
        if length(list) < cache_size
            # println("Weszło 2)")
            push!(list, elem)
        else
            # println("Weszło 3)")
            removed_item_index = UniformDistribution(collect(1:cache_size), 1)
            # println("Do wywalenia: ", removed_item_index)
            # println("PRZED: ", list)
            deleteat!(list, removed_item_index)
            # println("PO:  ", list)
            push!(list, elem)
            # println("PO2: ", list)
        end
        return 1
    end
end # Random

function RandomizedMarkupAlgorithm(elem, list, cache_size)
    index = 0
    list_len = length(list)
    for i in 1:list_len
        if list[i][1] == elem
            index = i
            break
        end
    end
    if index != 0
        if list[index][2] == 0
            list[index] = (elem, 1)
        end
        return 0
    else
        if length(list) < cache_size # Jest jeszcze miejsce w cache
            push!(list, (elem, 1))
        else
            marked_elements_counter = 0
            # not_marked_elements = Vector{Tuple{Int64, Int64}}()
            not_marked_elements = []
            for i in 1:list_len
                if list[i][2] == 1
                    marked_elements_counter += 1
                else
                    push!(not_marked_elements, list[i])
                end
            end
            if marked_elements_counter == list_len # All elements are marked
                for i in 1:list_len
                    list[i] = (list[i][1], 0)
                end
                not_marked_elements = copy(list)
            end # At this point all unmarked elements are stored in vector not_marked_elements
            if list_len > 0
                p = UniformDistribution(not_marked_elements, 1)
                for i in 1:list_len
                    if list[i] == p[1]
                        deleteat!(list, i)
                        break
                    end
                end
            end
            push!(list, (elem, 1))
        end
        return 1
    end
end # RandomizedMarkupAlgorithm

# list - set from which we will be getting our samples (ex. {1,2,...,n}), n - size of sample, Access - type of access function
function Experiment(n, cache_size, CacheHandlingMethod, r)
    list = collect(1:n)
    sample_uniform_list = UniformDistribution(list, r)
    sample_harmonic_list = HarmonicDistribution(list, r)
    sample_double_harmonic_list = DoubleHarmonicDistribution(list, r)
    sample_geometric_list = GeometricDistribution(list, r)

    # Uniform distribution
    cache = Vector{Int64}()
    costs = []
    for i in sample_uniform_list
        push!(costs, CacheHandlingMethod(i, cache, cache_size))
    end
    average_cost_uniform = mean(costs)

    # Harmonic distribution
    cache = Vector{Int64}()
    empty!(costs)
    for i in sample_harmonic_list
        push!(costs, CacheHandlingMethod(i, cache, cache_size))
    end
    average_cost_harmonic = mean(costs)

    # Double harmonic distribution
    cache = Vector{Int64}()
    empty!(costs)
    for i in sample_double_harmonic_list
        push!(costs, CacheHandlingMethod(i, cache, cache_size))
    end
    average_cost_double_harmonic = mean(costs)

    # Geometric distribution
    cache = Vector{Int64}()
    empty!(costs)
    for i in sample_geometric_list
        push!(costs, CacheHandlingMethod(i, cache, cache_size))
    end
    average_cost_geometric = mean(costs)

    return (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric)
end

# list - set from which we will be getting our samples (ex. {1,2,...,n}), n - size of sample, Access - type of access function
function Experiment2(n, cache_size, CacheHandlingMethod, r)
    list = collect(1:n)
    sample_uniform_list = UniformDistribution(list, r)
    sample_harmonic_list = HarmonicDistribution(list, r)
    sample_double_harmonic_list = DoubleHarmonicDistribution(list, r)
    sample_geometric_list = GeometricDistribution(list, r)

    # Uniform distribution
    cache = Vector{Tuple{Int64, Int64}}()
    costs = []
    for (index, i) in enumerate(sample_uniform_list)
        push!(costs, CacheHandlingMethod(i, cache, cache_size, index))
    end
    average_cost_uniform = mean(costs)

    # Harmonic distribution
    cache = Vector{Tuple{Int64, Int64}}()
    empty!(costs)
    for (index, i) in enumerate(sample_harmonic_list)
        push!(costs, CacheHandlingMethod(i, cache, cache_size, index))
    end
    average_cost_harmonic = mean(costs)

    # Double harmonic distribution
    cache = Vector{Tuple{Int64, Int64}}()
    empty!(costs)
    for (index, i) in enumerate(sample_double_harmonic_list)
        push!(costs, CacheHandlingMethod(i, cache, cache_size, index))
    end
    average_cost_double_harmonic = mean(costs)

    # Geometric distribution
    cache = Vector{Tuple{Int64, Int64}}()
    empty!(costs)
    for (index, i) in enumerate(sample_geometric_list)
        push!(costs, CacheHandlingMethod(i, cache, cache_size, index))
    end
    average_cost_geometric = mean(costs)

    return (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric)
end

# list - set from which we will be getting our samples (ex. {1,2,...,n}), n - size of sample, Access - type of access function
function Experiment3(n, cache_size, CacheHandlingMethod, r)
    list = collect(1:n)
    sample_uniform_list = UniformDistribution(list, r)
    sample_harmonic_list = HarmonicDistribution(list, r)
    sample_double_harmonic_list = DoubleHarmonicDistribution(list, r)
    sample_geometric_list = GeometricDistribution(list, r)

    # Uniform distribution
    cache = Vector{Tuple{Int64, Int64}}()
    costs = []
    for (index, i) in enumerate(sample_uniform_list)
        push!(costs, CacheHandlingMethod(i, cache, cache_size))
    end
    average_cost_uniform = mean(costs)

    # Harmonic distribution
    cache = Vector{Tuple{Int64, Int64}}()
    empty!(costs)
    for (index, i) in enumerate(sample_harmonic_list)
        push!(costs, CacheHandlingMethod(i, cache, cache_size))
    end
    average_cost_harmonic = mean(costs)

    # Double harmonic distribution
    cache = Vector{Tuple{Int64, Int64}}()
    empty!(costs)
    for (index, i) in enumerate(sample_double_harmonic_list)
        push!(costs, CacheHandlingMethod(i, cache, cache_size))
    end
    average_cost_double_harmonic = mean(costs)

    # Geometric distribution
    cache = Vector{Tuple{Int64, Int64}}()
    empty!(costs)
    for (index, i) in enumerate(sample_geometric_list)
        push!(costs, CacheHandlingMethod(i, cache, cache_size))
    end
    average_cost_geometric = mean(costs)

    return (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric)
end

function PlotResults(x, y₁, y₂, y₃, y₄, y₅, y₆, title_str)
    fig, ax = PyPlot.subplots()

    ax[:plot](x, y₁, label="FIRST IN FIRST OUT", color="blue", "-")
    ax[:plot](x, y₂, label="FLUSH WHEN FULL", color="black", "-")
    ax[:plot](x, y₃, label="LEAST RECENTLY USED", color="red", "-")
    ax[:plot](x, y₄, label="LEAST FREQUENTLY USED", color="green", "-")
    ax[:plot](x, y₅, label="RANDOM", color="orange", "-")
    ax[:plot](x, y₆, label="RANDOMIZED MARKUP ALGORITHM", color="purple", "-")
    ax[:legend](loc="best")

    grid("on")
    xlabel("Number of pages n, that can be requested")
    ylabel("Average cost of accesses")
    title("$title_str")
end

R = 1000 # R - number of requests
M = 300 # M - number of experiments
K = [10, 9, 8, 7, 6, 5]
N = [20, 30, 40, 50, 60, 70, 80, 90, 100]

for k in K
    FIFO_uni = zeros(Float64, length(N))
    FWF_uni = zeros(Float64, length(N))
    RND_uni = zeros(Float64, length(N))
    LRU_uni = zeros(Float64, length(N))
    LFU_uni = zeros(Float64, length(N))
    RMA_uni = zeros(Float64, length(N))

    FIFO_harm = zeros(Float64, length(N))
    FWF_harm = zeros(Float64, length(N))
    RND_harm = zeros(Float64, length(N))
    LRU_harm = zeros(Float64, length(N))
    LFU_harm = zeros(Float64, length(N))
    RMA_harm = zeros(Float64, length(N))

    FIFO_double_harm = zeros(Float64, length(N))
    FWF_double_harm = zeros(Float64, length(N))
    RND_double_harm = zeros(Float64, length(N))
    LRU_double_harm = zeros(Float64, length(N))
    LFU_double_harm = zeros(Float64, length(N))
    RMA_double_harm = zeros(Float64, length(N))

    FIFO_geo = zeros(Float64, length(N))
    FWF_geo = zeros(Float64, length(N))
    RND_geo = zeros(Float64, length(N))
    LRU_geo = zeros(Float64, length(N))
    LFU_geo = zeros(Float64, length(N))
    RMA_geo = zeros(Float64, length(N))

    for (i, n) in enumerate(N)
        print("k: ", k, ", n: ", n)
        cache_size = trunc(Int64, ceil(n/k))
        for m in 1:M
            (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric) = Experiment(n, cache_size, FirstInFirstOut, R)
            # println((average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric))
            FIFO_uni[i] += average_cost_uniform; FIFO_harm[i] += average_cost_harmonic; FIFO_double_harm[i] += average_cost_double_harmonic; FIFO_geo[i] += average_cost_geometric
        end
        for m in 1:M
            (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric) = Experiment(n, cache_size, FlushWhenFull, R)
            # println((average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric))
            FWF_uni[i] += average_cost_uniform; FWF_harm[i] += average_cost_harmonic; FWF_double_harm[i] += average_cost_double_harmonic; FWF_geo[i] += average_cost_geometric
        end
        for m in 1:M
            (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric) = Experiment(n, cache_size, Random, R)
            # println((average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric))
            RND_uni[i] += average_cost_uniform; RND_harm[i] += average_cost_harmonic; RND_double_harm[i] += average_cost_double_harmonic; RND_geo[i] += average_cost_geometric
        end
        for m in 1:M
            (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric) = Experiment2(n, cache_size, LeastRecentlyUsed, R)
            # println((average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric))
            LRU_uni[i] += average_cost_uniform; LRU_harm[i] += average_cost_harmonic; LRU_double_harm[i] += average_cost_double_harmonic; LRU_geo[i] += average_cost_geometric
        end
        for m in 1:M
            (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric) = Experiment3(n, cache_size, LeastFrequentlyUsed, R)
            # println((average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric))
            LFU_uni[i] += average_cost_uniform; LFU_harm[i] += average_cost_harmonic; LFU_double_harm[i] += average_cost_double_harmonic; LFU_geo[i] += average_cost_geometric
        end
        for m in 1:M
            (average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric) = Experiment3(n, cache_size, RandomizedMarkupAlgorithm, R)
            # println((average_cost_uniform, average_cost_harmonic, average_cost_double_harmonic, average_cost_geometric))
            RMA_uni[i] += average_cost_uniform; RMA_harm[i] += average_cost_harmonic; RMA_double_harm[i] += average_cost_double_harmonic; RMA_geo[i] += average_cost_geometric
        end

        FIFO_uni[i] /= M
        FWF_uni[i] /= M
        RND_uni[i] /= M
        LRU_uni[i] /= M
        LFU_uni[i] /= M
        RMA_uni[i] /= M

        FIFO_harm[i] /= M
        FWF_harm[i] /= M
        RND_harm[i] /= M
        LRU_harm[i] /= M
        LFU_harm[i] /= M
        RMA_harm[i] /= M

        FIFO_double_harm[i] /= M
        FWF_double_harm[i] /= M
        RND_double_harm[i] /= M
        LRU_double_harm[i] /= M
        LFU_double_harm[i] /= M
        RMA_double_harm[i] /= M

        FIFO_geo[i] /= M
        FWF_geo[i] /= M
        RND_geo[i] /= M
        LRU_geo[i] /= M
        LFU_geo[i] /= M
        RMA_geo[i] /= M
    end
    PlotResults(N, FIFO_uni, FWF_uni, LRU_uni, LFU_uni, RND_uni, RMA_uni, "Uniform | k = n/$k, $R zapytań, $M prób")
    PlotResults(N, FIFO_harm, FWF_harm, LRU_harm, LFU_harm, RND_harm, RMA_harm, "Harmonic | k = n/$k, $R zapytań, $M prób")
    PlotResults(N, FIFO_double_harm, FWF_double_harm, LRU_double_harm, LFU_double_harm, RND_double_harm, RMA_double_harm, "Double harmonic | k = n/$k, $R zapytań, $M prób")
    PlotResults(N, FIFO_geo, FWF_geo, LRU_geo, LFU_geo, RND_geo, RMA_geo, "Geometric | k = n/$k, $R zapytań, $M prób")
end
