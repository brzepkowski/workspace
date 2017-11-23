using PyPlot

# m - liczba kul, n - liczba urn
function ThrowBallsInUrns(m :: Int64, n :: Int64)
    urns = zeros(Int64, n)
    for i in 1:m
        index = rand(1:n)
        urns[index] += 1
    end

    println(urns)
end # ThrowBallsInUrns

# m - liczba kul, n - liczba urn, a - liczba eksperymentów
function RunExperiment(m :: Int, n :: Int, a :: Int)
    x = []
    EmptyUrnsResults = []
    push!(x, n)
    EmptyUrns = []
    for i in 1:a
        Result = ThrowBallsInUrns(m, n)
        MaxBallsAmount = maximum(Result)
        NumberOfEmptyUrns = 0
        for k in 1:n
            if Result[k] == 0
                NumberOfEmptyUrns += 1
            end
        end
        push!(EmptyUrns, NumberOfEmptyUrns)
    end
    push!(EmptyUrnsResults, EmptyUrns)
    PyPlot.plot(x, EmptyUrnsResults, color="red", "o")
    # PyPlot.plot(x, ECₙResults, color="blue", linestyle="-")
    title("Wyniki")
end # RunExperiment

RunExperiment(10, 10, 100)
