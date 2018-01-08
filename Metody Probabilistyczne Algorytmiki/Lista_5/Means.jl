using Distributions
using PyPlot

function GeoMean(arr)
    E = BigInt(1)
    for v in arr
        E *= v
    end
    E = abs(E)^(1/length(arr))
    return Float64(E)
end

function HarmMean(arr)
    E = 0
    for v in arr
        E += abs(1/v)
    end
    E = length(arr) / E
    return E
end

# m - rozmiar populacji
function TestMeans(m)
    fig, ax = PyPlot.subplots()
    x = []
    results = []
    E1 = []
    E2 = []
    E3 = []
    for i in 0.00:0.001:1
        d = Normal(10, 1)
        dˈ = Normal(100, 1)
        random = rand(d, Int(trunc((1-i)*m)))
        randomˈ = rand(dˈ, Int(trunc(i*m)))
        push!(x, i)
        allResults = union(random, randomˈ)
        push!(results, allResults)
        push!(E1, mean(allResults))
        push!(E2, GeoMean(allResults))
        push!(E3, HarmMean(allResults))
    end
    # println(x)
    # println(results)
    ax[:plot](x, E1, color="red", ".")
    ax[:plot](x, E2, color="blue", ".")
    ax[:plot](x, E3, color="black", ".")
    # ax[:plot](x, czebyszewBounds, color="red", "--")
    # ax[:plot](x, azumaBounds, color="black", "--")
    # ax[:legend](loc="best")

    # grid("on")
    # xlabel("Długosc tablicy")
    # ylabel("Liczba punktow stalych")
    # title("Liczba punktow stalych w losowej permutacji")
end # TestMeans

TestMeans(1000)
