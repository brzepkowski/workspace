using Graphs
using PyPlot

function GenerateGraph(n :: Int, p :: Float64)
    g = simple_graph(n, is_directed=false)
    erdos_renyi_graph(g, n, p)
    return g
end # GenerateGraph

function FindBiggestComponents(components)
    max = 0
    secondToMax = 0
    for i in components
        if i >= max
            max = i
        else
            if i >= secondToMax
                secondToMax = i
            end
        end
    end
    return (max, secondToMax)
end # FindBiggestComponents

function RunOneTest(n :: Int, p :: Float64)
    return FindBiggestComponents(map(length, connected_components(GenerateGraph(n, p))))
end # RunOneTest

function f1(n)
    return 1 / (2*n)
end # f1

function f2(n)
    return (1/n)-((n^(0.1))/(n^(4/3)))
end # f2

function f3(n)
    return (1/n)+((n^(0.1))/(n^(4/3)))
end # f3

function f4(n)
    return (1/n)-(2/(n^(4/3)))
end # f4

function f5(n)
    return (1/n)+(2/(n^(4/3)))
end # f5

function f6(n)
    return 2/n
end # f6

# n - maksymalna liczba wierzchołków w grafie,
# m - liczba eksperymentów
# f - funkcja obliczająca prawdopodobieństwo na podstawie n
# ax - plot, do którego będziemy dodawać dane
function RunTests(n :: Int, f, m :: Int, ax)
    x = []
    maxComponents = []
    secondToMaxComponents = []
    maxComponentExpectedValues = []
    secondToMaxComponentExpectedValues = []
    maxComponentBounds = []
    secondToMaxComponentBounds = []
    for i in 1000:1000:n
        p = f(i)
        push!(x, i)
        tempMaxComponents = []
        tempSecondToMaxComponents = []
        for j in 1:m
            (a, b) = RunOneTest(i, p)
            push!(tempMaxComponents, a)
            push!(tempSecondToMaxComponents, b)
        end
        # Max components
        mcE = sum(tempMaxComponents) / m
        mcV = 0
        for v in tempMaxComponents
            mcV += (v - mcE)^2
        end
        mcV = mcV / m
        # Second to max components
        smcE = sum(tempSecondToMaxComponents) / m
        smcV = 0
        for v in tempSecondToMaxComponents
            smcV += (v - smcE)^2
        end
        smcV = smcV / m
        # Dodawanie wartości do tabeli
        push!(maxComponents, tempMaxComponents)
        push!(maxComponentExpectedValues, mcE)
        push!(maxComponentBounds, [mcE + 2*sqrt(mcV), mcE - 2*sqrt(mcV)])
        push!(secondToMaxComponents, tempSecondToMaxComponents)
        push!(secondToMaxComponentExpectedValues, smcE)
        push!(secondToMaxComponentBounds, [smcE + 2*sqrt(smcV), smcE - 2*sqrt(smcV)])
    end
    println("X: ", x)
    println("Max: ", maxComponentExpectedValues)
    println("SecondMax: ", secondToMaxComponentExpectedValues)
    ax[:plot](x, maxComponents, color="red", "o")
    ax[:plot](x, maxComponentExpectedValues, color="red", "-")
    ax[:plot](x, maxComponentBounds, color="red", "--")
    # ax[:plot](x, expectedValue, label="Teoretyczna wart. oczekiwana" color="blue", "-")
    ax[:plot](x, secondToMaxComponents, color="blue", "o")
    ax[:plot](x, secondToMaxComponentExpectedValues, color="blue", "-")
    ax[:plot](x, secondToMaxComponentBounds, color="blue", "--")
    # ax[:legend](loc="best")

    grid("on")
    xlabel("Liczba wierzcholkow w grafie")
    ylabel("Rozmiar komponenty")
    title("Wielkosci najwiekszej i drugej najwiekszej komponenty w grafie losowym G(n p)")
end # RunTests

fig, ax = PyPlot.subplots()
RunTests(1000, f6, 10, ax)
