using StatsBase
using PyPlot

function HarmonicNumber(n)
    harmonic_number = 0
    for i in 1:n
        harmonic_number += 1/i
    end
    return harmonic_number
end # HarmonicNumber

# m - number of samples
function UniformDistribution(m)
    list = collect(1:64)
    weights = Array{Float64}(64)
    for i in 1:64
        weights[i] = 1/64
    end
    results = []
    for i in 1:m
        push!(results, sample(list, Weights(weights)))
    end
    return results
end # UniformDistribution

# m - number of samples
function HarmonicDistribution(m)
    list = collect(1:64)
    nth_harmonic_number = HarmonicNumber(64) # It has to be H64
    weights = Array{Float64}(64)
    for i in 1:64
        weights[i] = 1/(list[i]*nth_harmonic_number)
    end
    results = []
    for i in 1:m
        push!(results, sample(list, Weights(weights)))
    end
    return results
end # HarmonicDistribution

# m - number of samples
function DoubleHarmonicDistribution(m)
    list = collect(1:64)
    n = length(list)
    nth_harmonic_number = HarmonicNumber(64) # It has to be H64
    weights = Array{Float64}(64)
    for i in 1:64
        weights[i] = 1/((list[i]^2)*nth_harmonic_number)
    end
    results = []
    for i in 1:m
        push!(results, sample(list, Weights(weights)))
    end
    return results
end # DoubleHarmonicDistribution

# --------------------------------------------------

function HammingDistance(a, b)
    first_bin = bin(a - 1, 6)
    second_bin = bin(b - 1, 6)
    counter = 0
    for i in 1:6
        if first_bin[i] != second_bin[i]
            counter += 1
        end
    end
    return counter
end # HammingDistance

function TorusDistance(a, b)
    (a_z, a_y, a_x) = base(4, a - 1, 3)
    (b_z, b_y, b_x) = base(4, b - 1, 3)
    x = min(4 - abs(a_x - b_x), abs(a_x - b_x))
    y = min(4 - abs(a_y - b_y), abs(a_y - b_y))
    z = min(4 - abs(a_z - b_z), abs(a_z - b_z))
    return x + y + z
end # TorusDistance

# t - number of iterations, D - cost of moving page (and also how long a part of the chain of requests is)
function MoveToMin(t, D, Distribution, Metric)
    cost = 0
    resource = 1 # Index of vertex, which has resource
    for i in 1:t
        # ----Generate package of requests-----
        requests = Distribution(D)
        # ---Calculate costs of requests for resource-------
        for j in 1:D
            cost += Metric(requests[j], resource)
        end
        # ---Pick best vertex for new resource, move it and increase cost------
        new_resource = 0
        min_sum = typemax(Int)
        for best_resource in 1:64
            sum = 0
            for j in 1:D
                sum += Metric(requests[j], best_resource)
            end
            if sum < min_sum
                new_resource = best_resource
                min_sum = sum
            end
        end
        cost += Metric(resource, new_resource) * D
        resource = new_resource
    end
    return cost
end # MoveToMinHipercube

# number of requests in Move-to-min it was equal to t*D; here it will be the same
function Flip(t, D, Distribution, Metric)
    requests = Distribution(t*D)
    cost = 0
    resource = 1
    p = 1/(2*D)
    for i in 1:(t*D)
        distance = Metric(requests[i], resource)
        cost += distance
        q = rand(1)[1]
        if q <= p
            cost += distance * D
            resource = requests[i]
        end
    end
    return cost
end # Flip

# t - number of iterations (in the inside programs), m - number of experiments
function Experiment(t, D, Distribution, Metric, Algorithm, m)
    results = []
    for i in 1:m
        push!(results, Algorithm(t, D, Distribution, Metric))
    end
    return mean(results)
end # Test

# t - number of tests for each case
function PlotResults(t)
    fig, ax = PyPlot.subplots()

    moveToMinResultsHypercube = []
    push!(moveToMinResultsHypercube, Experiment(32, 32, UniformDistribution, HammingDistance, MoveToMin, t))
    push!(moveToMinResultsHypercube, Experiment(32, 32, HarmonicDistribution, HammingDistance, MoveToMin, t))
    push!(moveToMinResultsHypercube, Experiment(32, 32, DoubleHarmonicDistribution, HammingDistance, MoveToMin, t))

    flipResultsHypercube = []
    push!(flipResultsHypercube, Experiment(32, 32, UniformDistribution, HammingDistance, Flip, t))
    push!(flipResultsHypercube, Experiment(32, 32, HarmonicDistribution, HammingDistance, Flip, t))
    push!(flipResultsHypercube, Experiment(32, 32, DoubleHarmonicDistribution, HammingDistance, Flip, t))

    moveToMinResultsTorus = []
    push!(moveToMinResultsTorus, Experiment(32, 32, UniformDistribution, TorusDistance, MoveToMin, t))
    push!(moveToMinResultsTorus, Experiment(32, 32, HarmonicDistribution, TorusDistance, MoveToMin, t))
    push!(moveToMinResultsTorus, Experiment(32, 32, DoubleHarmonicDistribution, TorusDistance, MoveToMin, t))

    flipResultsTorus = []
    push!(flipResultsTorus, Experiment(32, 32, UniformDistribution, TorusDistance, Flip, t))
    push!(flipResultsTorus, Experiment(32, 32, HarmonicDistribution, TorusDistance, Flip, t))
    push!(flipResultsTorus, Experiment(32, 32, DoubleHarmonicDistribution, TorusDistance, Flip, t))

    x = ["Uniform", "Harmonic", "DoubleHarmonic"]

    ax[:plot](x, moveToMinResultsHypercube, label="MOVE-TO-MIN (HYPERCUBE)", color="black", "-")
    ax[:plot](x, moveToMinResultsTorus, label="MOVE-TO-MIN (TORUS)", color="blue", "-")
    ax[:plot](x, flipResultsHypercube, label="FLIP (HYPERCUBE)", color="red", "-")
    ax[:plot](x, flipResultsTorus, label="FLIP (TORUS)", color="orange", "-")
    ax[:legend](loc="best")

    grid("on")
    xlabel("Type of distribution")
    ylabel("Average cost")
    title("Average cost in Page Migration problem - HYPERCUBE")
end

PlotResults(1000)
