using StatsBase
using PyPlot

function EqualMin(x, minVar)
    if x == minVar
        return true
    else
        return false
    end
end # EqualMin

# n - liczba kul i liczba urn
function MaxLoad(n :: Int)
    urns = zeros(Int64, n)
    for i in 1:n
        index = rand(1:n)
        urns[index] += 1
    end
    return maximum(urns)
end # BallsInUrns

# n - liczba kul i urn, d - rozmiar podzbioru urn, który wybieramy dla każdej kuli
function BalancedMaxLoad(n, d)
    urns = zeros(Int64, n)
    indices = collect(1:n)
    for i in 1:n
        subIndices = sample(indices, d, replace=false)
        minLoad = typemax(Int64)
        for j in subIndices
            if urns[j] < minLoad
                minLoad = urns[j]
            end
        end
        finalIndices = []
        for j in subIndices
            if urns[j] == minLoad
                push!(finalIndices, j)
            end
        end
        index = rand(finalIndices)
        urns[index] += 1
    end
    # println(urns)
    maxLoad = 0
    for i in 1:n
        if urns[i] > maxLoad
            maxLoad = urns[i]
        end
    end
    return maxLoad
end # BalancedMaxLoad

# n - liczba kul i urn, d - liczba grup, na które dzielimy urny
function BalancedMaxLoadDivided(n, d)
    urns = zeros(Int64, n)
    indices = collect(1:n)
    groups = []
    subGroupLength = n/d # zakładamy, że n jest wielokrotnością d
    subIndices = []
    dˈ = 1
    for i in 1:n # podział na grupy
        if dˈ < subGroupLength
            dˈ += 1
            push!(subIndices, i)
        else
            push!(subIndices, i)
            push!(groups, subIndices)
            subIndices = []
            dˈ = 1
        end
    end
    # println(groups)

    for i in 1:n # losowanie
        subIndices = []
        for group in groups
            j = rand(group)
            push!(subIndices, j)
        end
        minLoad = typemax(Int64)
        for j in subIndices # znajdź minLoad
            if urns[j] < minLoad
                minLoad = urns[j]
            end
        end
        finalSubIndices = []
        for j in subIndices # znajdź wszystkie urny z minLoad'em
            if urns[j] == minLoad
                push!(finalSubIndices, j)
            end
        end
        # println(finalSubIndices)
        urns[finalSubIndices[1]] += 1
    end
    # println(urns)
    maxLoad = 0
    for i in 1:n
        if urns[i] > maxLoad
            maxLoad = urns[i]
        end
    end
    return maxLoad
end # BalancedMaxLoadDivided

# m - liczba kul, n - liczba urn, m - liczba experymentów
function PlotMaxLoad(n, m, ax)
    x = []
    results = []
    expectedValues = []
    czebyszewBounds = []
    for i in 108:108:n
        push!(x, i)
        tempResults = []
        E = 0
        for j in 1:m
            result = MaxLoad(i)
            E += result
            push!(tempResults, result)
        end
        E = E / m
        V = 0
        for r in tempResults
            V += (r - E)^2
        end
        V = V / (m - 1)
        push!(results, tempResults)
        push!(expectedValues, E)
        push!(czebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
    end


  # ax[:plot](x, results, color="red", "o")
  ax[:plot](x, expectedValues, color="red", "-")
  ax[:plot](x, czebyszewBounds, color="red", "--")
  # ax[:legend](loc="best")

  grid("on")
  xlabel("Liczba urn")
  ylabel("Max load")
  title("Max Load")
end # PlotMaxLoad

# m - liczba kul, n - liczba urn, m - liczba experymentów, d - wielkośc podgrupy urn
function PlotBalancedMaxLoad(n, m, d, ax)
    x = []
    results = []
    expectedValues = []
    czebyszewBounds = []
    for i in 108:108:n
        push!(x, i)
        tempResults = []
        E = 0
        for j in 1:m
            result = BalancedMaxLoad(i, d)
            E += result
            push!(tempResults, result)
        end
        E = E / m
        V = 0
        for r in tempResults
            V += (r - E)^2
        end
        V = V / (m - 1)
        push!(results, tempResults)
        push!(expectedValues, E)
        push!(czebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
    end


  # ax[:plot](x, results, color="blue", "o")
  ax[:plot](x, expectedValues, color="blue", "-")
  ax[:plot](x, czebyszewBounds, color="blue", "--")
  # ax[:legend](loc="best")

  grid("on")
  xlabel("Liczba urn")
  ylabel("Max load")
  title("Max Load")
end # PlotBalancedMaxLoad

# m - liczba kul, n - liczba urn, m - liczba experymentów, d - liczba grup, na które dzielimy urny
function PlotBalancedMaxLoadDivided(n, m, d, ax)
    x = []
    results = []
    expectedValues = []
    czebyszewBounds = []
    for i in 108:108:n
        push!(x, i)
        tempResults = []
        E = 0
        for j in 1:m
            result = BalancedMaxLoadDivided(i, d)
            E += result
            push!(tempResults, result)
        end
        E = E / m
        V = 0
        for r in tempResults
            V += (r - E)^2
        end
        V = V / (m - 1)
        push!(results, tempResults)
        push!(expectedValues, E)
        push!(czebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
    end


  # ax[:plot](x, results, color="black", "o")
  ax[:plot](x, expectedValues, color="black", "-")
  ax[:plot](x, czebyszewBounds, color="black", "--")
  # ax[:legend](loc="best")

  grid("on")
  xlabel("Liczba urn")
  ylabel("Max load")
  title("Max Load")
end # PlotBalancedMaxLoadDivided

fig, ax = PyPlot.subplots()
PlotMaxLoad(1000, 1000, ax)
PlotBalancedMaxLoad(1000, 1000, 4, ax)
PlotBalancedMaxLoadDivided(1000, 1000, 4, ax)
