using PyPlot

function PermutationFixedPoints(n)
    default = collect(1:n)
    permutation = shuffle(default)
    fixedPoints = 0
    for i in 1:n
        if default[i] == permutation[i]
            fixedPoints += 1
        end
    end
    return fixedPoints
end # PermutationFixedPoints


function PermutationCycles(n)
    permutation = shuffle(1:n)
    cycles = 0
    elementsChecked = 0
    while elementsChecked < n
        i = 1
        while permutation[i] == 0
            i += 1
        end
        beginning = permutation[i]
        next = permutation[beginning]
        permutation[i] = 0
        permutation[beginning] = 0
        elementsChecked += 2
        while next != i
            buffer = permutation[next]
            permutation[next] = 0
            next = buffer
            elementsChecked += 1
        end
        cycles += 1
    end
    return cycles
end # PermutationCycles

function PermutationRecords(n)
    permutation = shuffle(1:n)
    records = 0 # liczba elementów permutacji, które są większe od wszystkich poprzednich
    previousMax = 0 # najwiekszy do tej pory napotkany element
    for i in 1:n
        if permutation[i] > previousMax
            records += 1
            previousMax = permutation[i]
        end
    end
    return records
end # PermutationRecords

# m - liczba eksperymentów dla danej tablicy, n - długośc tablicy (wielokrotność 1000)
function TestFixedPoints(m, n, ax)
    x = []
    fixedPoints = []
    for i in 1000:1000:n
        push!(x, i)
        tempFixedPoints = []
        for j in 1:m
            push!(tempFixedPoints, PermutationFixedPoints(i))
        end
        push!(fixedPoints, tempFixedPoints)
    end
    println(x)
    println(fixedPoints)

    ax[:plot](x, fixedPoints, color="red", "o")
    # ax[:legend](loc="best")

    grid("on")
    xlabel("Długosc tablicy")
    ylabel("Liczba punktow stalych")
    title("Liczba punktow stalych w losowej permutacji")
end # TestFixedPoints

# fig, ax = PyPlot.subplots(); TestFixedPoints(10, 3000, ax)
