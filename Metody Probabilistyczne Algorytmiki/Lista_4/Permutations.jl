using PyPlot

function HarmonicNumber(n)
    sum = 0
    for i in 1:n
        sum += 1/i
    end
    return sum
end

function Sum(n)
    sum = 0
    for i in 1:n
        sum += (1 - (1/(n - i + 1)))^2
    end
    return sum
end

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
    sizes = []
    while elementsChecked < n
        i = 1
        while permutation[i] == 0
            i += 1
        end
        currentSize = 0
        beginning = permutation[i]
        if beginning == i # permutation[i] == i
            push!(sizes, 1)
            permutation[i] = 0
            elementsChecked += 1
        else
            next = permutation[beginning]
            permutation[i] = 0
            permutation[beginning] = 0
            elementsChecked += 2
            currentSize += 2
            while next != i
                buffer = permutation[next]
                permutation[next] = 0
                next = buffer
                elementsChecked += 1
                currentSize += 1
            end
            push!(sizes, currentSize)
        end
        cycles += 1
    end
    return (cycles, sizes)
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
function TestFixedPoints(m, n)
    fig, ax = PyPlot.subplots()
    x = []
    fixedPoints = []
    expectedValues = []
    czebyszewBounds = []
    azumaBounds = []
    for i in 1000:1000:n
        push!(x, i)
        tempFixedPoints = []
        E = 0 # wartośc oczekiwana
        V = 0 # wariancja
        for j in 1:m
            fixedPointsAmount = PermutationFixedPoints(i)
            push!(tempFixedPoints, fixedPointsAmount)
            E += fixedPointsAmount
        end
        E = E / m
        for c in tempFixedPoints
            V += (c - E)^2
        end
        V = V / (m - 1)
        println("E: ", E)
        push!(expectedValues, E)
        println("V: ", V)
        push!(czebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
        λ = sqrt(6*(i - HarmonicNumber(i))*log(2))
        push!(azumaBounds, [-λ, λ])
        push!(fixedPoints, tempFixedPoints)
        # ----------------------------------------------------------------------
        # Dodaj opisy, ile procent wyników przypada na daną wartość (dla danego
        # punktu, ile było takich wyników spośród wszystkich eksperymentów)
        fixedPointsAppearance = Dict{Int64, Int64}()
        for j in 1:m
            fixedPointsAmount = tempFixedPoints[j]
            if haskey(fixedPointsAppearance,fixedPointsAmount)
                fixedPointsAppearance[fixedPointsAmount] = fixedPointsAppearance[fixedPointsAmount] + 1
            else
                fixedPointsAppearance[fixedPointsAmount] = 1
            end
        end
        for (index, (key, value)) in enumerate(fixedPointsAppearance)
            percentage = value / m
            annotate(string(percentage),
        	xy=[i + 0.15, key + 0.15],
        	xytext=[i + 0.15, key + 0.15],
        	xycoords="data")
        end
        # ----------------------------------------------------------------------
    end

    ax[:plot](x, fixedPoints, color="red", "o")
    ax[:plot](x, expectedValues, color="red", "-")
    ax[:plot](x, czebyszewBounds, color="red", "--")
    ax[:plot](x, azumaBounds, color="black", "--")
    # ax[:legend](loc="best")

    grid("on")
    xlabel("Długosc tablicy")
    ylabel("Liczba punktow stalych")
    title("Liczba punktow stalych w losowej permutacji")
end # TestFixedPoints

# m - liczba eksperymentów dla danej tablicy, n - długośc tablicy (wielokrotność 1000)
function TestCyclesAmounts(m, n)
    fig, ax = PyPlot.subplots()
    x = []
    cyclesList = []
    expectedValues = []
    czebyszewBounds = []
    azumaBounds = []
    for i in 1000:1000:n
        push!(x, i)
        tempCyclesList = []
        E = 0 # wartośc oczekiwana
        V = 0 # wariancja
        for j in 1:m
            (cycles, sizes) = PermutationCycles(i)
            push!(tempCyclesList, cycles)
            E += cycles
        end
        E = E / m
        for c in tempCyclesList
            V += (c - E)^2
        end
        V = V / (m - 1)
        println("E: ", E)
        push!(expectedValues, E)
        println("V: ", V)
        push!(czebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
        λ = sqrt(6*Sum(i)*log(2))
        push!(azumaBounds, [-λ, λ])
        push!(cyclesList, tempCyclesList)
        # ----------------------------------------------------------------------
        # Dodaj opisy, ile procent wyników przypada na daną wartość (dla danego
        # punktu, ile było takich wyników spośród wszystkich eksperymentów)
        cyclesAppearance = Dict{Int64, Int64}()
        for j in 1:m
            cyclesAmount = tempCyclesList[j]
            if haskey(cyclesAppearance,cyclesAmount)
                cyclesAppearance[cyclesAmount] = cyclesAppearance[cyclesAmount] + 1
            else
                cyclesAppearance[cyclesAmount] = 1
            end
        end
        for (index, (key, value)) in enumerate(cyclesAppearance)
            percentage = value / m
            annotate(string(percentage),
        	xy=[i + 0.15, key + 0.15],
        	xytext=[i + 0.15, key + 0.15],
        	xycoords="data")
        end
        # ----------------------------------------------------------------------
    end

    ax[:plot](x, cyclesList, color="red", "o")
    ax[:plot](x, expectedValues, color="red", "-")
    ax[:plot](x, czebyszewBounds, color="red", "--")
    ax[:plot](x, azumaBounds, color="black", "--")
    # ax[:legend](loc="best")

    grid("on")
    xlabel("Długosc tablicy")
    ylabel("Liczba cykli")
    title("Liczba cykli w losowej permutacji")
end # TestCyclesAmounts

# m - liczba eksperymentów dla danej tablicy, n - długośc tablicy (wielokrotność 1000)
function TestCyclesSizes(m, n)
    fig, ax = PyPlot.subplots()
    x = []
    cyclesList = []
    expectedValues = []
    czebyszewBounds = []
    for i in 1000:1000:n
        push!(x, i)
        tempCyclesList = []
        E = 0 # wartośc oczekiwana
        V = 0 # wariancja
        for j in 1:m
            (cycles, sizes) = PermutationCycles(i)
            for v in sizes
                push!(tempCyclesList, v)
            end
        end
        # E = E / m
        for c in tempCyclesList
            E += c
            # V += (c - E)^2
        end
        E = E / length(tempCyclesList)
        for c in tempCyclesList
            V += (c - E)^2
        end
        V = V / (length(tempCyclesList) - 1)
        # println("E: ", E)
        push!(expectedValues, E)
        # println("V: ", V)
        push!(czebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
        # push!(cyclesList, tempCyclesList)
        ax[:plot]([i], [tempCyclesList], color="red", "o")
    end

    println("x: ", x)
    println("cyclesList: ", cyclesList)

    # ax[:plot](x, cyclesList, color="red", "o")
    ax[:plot](x, expectedValues, color="red", "-")
    ax[:plot](x, czebyszewBounds, color="red", "--")
    # ax[:legend](loc="best")

    grid("on")
    xlabel("Długosc tablicy")
    ylabel("Liczba cykli")
    title("Liczba cykli w losowej permutacji")
end # TestCyclesSizes

# m - liczba eksperymentów dla danej tablicy, n - długośc tablicy (wielokrotność 1000)
function TestRecords(m, n)
    fig, ax = PyPlot.subplots()
    x = []
    records = []
    expectedValues = []
    czebyszewBounds = []
    for i in 1000:1000:n
        push!(x, i)
        tempRecords = []
        E = 0 # wartośc oczekiwana
        V = 0 # wariancja
        for j in 1:m
            recordsAmount = PermutationRecords(i)
            push!(tempRecords, recordsAmount)
            E += recordsAmount
        end
        E = E / m
        for c in tempRecords
            V += (c - E)^2
        end
        V = V / (m - 1)
        println("E: ", E)
        push!(expectedValues, E)
        println("V: ", V)
        push!(czebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
        push!(records, tempRecords)
        # ----------------------------------------------------------------------
        # Dodaj opisy, ile procent wyników przypada na daną wartość (dla danego
        # punktu, ile było takich wyników spośród wszystkich eksperymentów)
        recordsAppearance = Dict{Int64, Int64}()
        for j in 1:m
            recordsAmount = tempRecords[j]
            if haskey(recordsAppearance,recordsAmount)
                recordsAppearance[recordsAmount] = recordsAppearance[recordsAmount] + 1
            else
                recordsAppearance[recordsAmount] = 1
            end
        end
        for (index, (key, value)) in enumerate(recordsAppearance)
            percentage = value / m
            annotate(string(percentage),
        	xy=[i + 0.15, key + 0.15],
        	xytext=[i + 0.15, key + 0.15],
        	xycoords="data")
        end
        # ----------------------------------------------------------------------
    end

    ax[:plot](x, records, color="red", "o")
    ax[:plot](x, expectedValues, color="red", "-")
    ax[:plot](x, czebyszewBounds, color="red", "--")
    # ax[:legend](loc="best")

    grid("on")
    xlabel("Długosc tablicy")
    ylabel("Liczba rekordów")
    title("Liczba rekordów w losowej permutacji")
end # TestRecords

# TestCyclesAmounts(400, 10000) # E(X) ~ Hₙ
TestCyclesSizes(100, 5000) # TODO
# TestFixedPoints(400, 10000) # E(X) ~ 1
# TestRecords(400, 10000) # E(X) ~ Hₙ
