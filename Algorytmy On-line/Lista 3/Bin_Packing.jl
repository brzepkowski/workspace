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

function GeneratePackingItems(DistributionFunction)
    range = collect(0:0.01:1)
    k_range = collect(1:10)
    items = []
    while length(items) < 100
        item = UniformDistribution(range, 1)[1]
        k = DistributionFunction(k_range, 1)[1]
        while length(items) < 100 && k > 0
            push!(items, item)
            k -= 1
        end
    end
    return items
end # GeneratePackingItems

# -----------------------------------------------------------------------------
function Optimum(items)
    return ceil(sum(items))
end # Optimum

function NextFit(items)
    number_of_bins = 0
    current_bin = 0
    for (i, item) in enumerate(items)
        if current_bin + item <= 1
            current_bin += item
        else
            number_of_bins += 1
            current_bin = item
        end
    end
    return number_of_bins
end # NextFit

function RandomFit(items)
    bins = [0.0]
    for (i, item) in enumerate(items)
        item_packed = false
        available_bins = [] # It will be list of indices, not capacities
        for j in 1:length(bins)
            if bins[j] + item <= 1
                push!(available_bins, j)
            end
        end
        if length(available_bins) > 0
            random_index = UniformDistribution(available_bins, 1)[1]
            if bins[random_index] + item <= 1
                bins[random_index] += item
            end
        else
            push!(bins, item)
        end
    end
    return length(bins)
end # RandomFit

function FirstFit(items)
    bins = [0.0]
    for (i, item) in enumerate(items)
        # println(bins, " || ", item)
        item_packed = false
        for j in 1:length(bins)
            if bins[j] + item <= 1
                bins[j] += item
                item_packed = true
                break
            end
        end
        if !item_packed
            push!(bins, item)
        end
    end
    return length(bins)
end # FirstFit

function BestFit(items)
    bins = [0.0]
    for (i, item) in enumerate(items)
        best_bin = 0
        best_bin_index = 0
        for j in 1:length(bins)
            if bins[j] + item <= 1
                if best_bin < bins[j] + item
                    best_bin_index = j
                    best_bin = bins[j] + item
                end
            end
        end
        if best_bin_index == 0 # There are no space in so far created bins
            push!(bins, item)
        else
            bins[best_bin_index] += item
        end
    end
    return length(bins)
end # BestFit

function WorstFit(items)
    bins = [0.0]
    for (i, item) in enumerate(items)
        worst_bin = 1.0
        worst_bin_index = 0
        for j in 1:length(bins)
            if bins[j] + item <= 1
                if worst_bin > bins[j] + item
                    worst_bin_index = j
                    worst_bin = bins[j] + item
                end
            end
        end
        if worst_bin_index == 0 # There are no space in so far created bins
            push!(bins, item)
        else
            bins[worst_bin_index] += item
        end
    end
    return length(bins)
end # WorstFit

# m - number of experiments
function Experiment(r, PackingMethod, DistributionFunction)
    results = []
    for i in 1:r
        items = GeneratePackingItems(DistributionFunction)
        push!(results, PackingMethod(items))
    end
    return mean(results)
end # Experiment

# m - number of experiments
function PlotResults(m)
    fig, ax = PyPlot.subplots()
    x = ["Uniform", "Harmonic", "DoubleHarmonic", "Geometric"]

    NextFitResults = [Experiment(m, NextFit, UniformDistribution), Experiment(m, NextFit, HarmonicDistribution), Experiment(m, NextFit, DoubleHarmonicDistribution), Experiment(m, NextFit, GeometricDistribution)]
    RandomFitResults = [Experiment(m, RandomFit, UniformDistribution), Experiment(m, RandomFit, HarmonicDistribution), Experiment(m, RandomFit, DoubleHarmonicDistribution), Experiment(m, RandomFit, GeometricDistribution)]
    FirstFitResults = [Experiment(m, FirstFit, UniformDistribution), Experiment(m, FirstFit, HarmonicDistribution), Experiment(m, FirstFit, DoubleHarmonicDistribution), Experiment(m, FirstFit, GeometricDistribution)]
    BestFitResults = [Experiment(m, BestFit, UniformDistribution), Experiment(m, BestFit, HarmonicDistribution), Experiment(m, BestFit, DoubleHarmonicDistribution), Experiment(m, BestFit, GeometricDistribution)]
    WorstFitResults = [Experiment(m, WorstFit, UniformDistribution), Experiment(m, WorstFit, HarmonicDistribution), Experiment(m, WorstFit, DoubleHarmonicDistribution), Experiment(m, WorstFit, GeometricDistribution)]
    OptimumResults = [Experiment(m, Optimum, UniformDistribution), Experiment(m, Optimum, HarmonicDistribution), Experiment(m, Optimum, DoubleHarmonicDistribution), Experiment(m, Optimum, GeometricDistribution)]

    ax[:plot](x, OptimumResults, label="OPT", color="gray", "-")
    ax[:plot](x, NextFitResults, label="NEXT FIT", color="blue", "-")
    ax[:plot](x, RandomFitResults, label="RANDOM FIT", color="red", "-")
    ax[:plot](x, FirstFitResults, label="FIRST FIT", color="green", "-")
    ax[:plot](x, BestFitResults, label="BEST FIT", color="orange", "-")
    ax[:plot](x, WorstFitResults, label="WORST FIT", color="black", "-")
    ax[:legend](loc="best")

    grid("on")
    xlabel("Type of distribution")
    ylabel("Average number of bins")
    title("Average number of bins in bin-packing problem")
    # ------------------------
    fig, ax = PyPlot.subplots()

    for i in 1:4
        NextFitResults[i] /= OptimumResults[i]
        RandomFitResults[i] /= OptimumResults[i]
        FirstFitResults[i] /= OptimumResults[i]
        BestFitResults[i] /= OptimumResults[i]
        WorstFitResults[i] /= OptimumResults[i]
    end

    ax[:plot](x, NextFitResults, label="NEXT FIT", color="blue", "-")
    ax[:plot](x, RandomFitResults, label="RANDOM FIT", color="red", "-")
    ax[:plot](x, FirstFitResults, label="FIRST FIT", color="green", "-")
    ax[:plot](x, BestFitResults, label="BEST FIT", color="orange", "-")
    ax[:plot](x, WorstFitResults, label="WORST FIT", color="black", "-")
    ax[:legend](loc="best")

    grid("on")
    xlabel("Type of distribution")
    ylabel("Competitiveness factor")
    title("Competitiveness factor for bin-packing problem")
end

PlotResults(10000)
