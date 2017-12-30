using PyPlot
p = [2,2,2,3,3,5] # czasy wykonania zadań

# times - czasy wykonania zadań, m - liczba maszyn
function ListAlgorithm(times, m)
    machines = Array{Int64}(m)
    for i in 1:m
        machines[i] = 0
    end
    sortedTimes = sort(times, rev=true)
    FindMinIndex(sortedTimes)
    for p in sortedTimes
        index = FindMinIndex(machines)
        machines[index] += p
    end
    println(machines)

    # x = [1:1:50;]
    # y = 100*rand(50);
    #
    # fig = figure("pyplot_barplot",figsize=(10,10))
    # subplot(211)
    # b = barh(x,y,color="#0f87bf",align="center",alpha=0.4)
    # axis("tight")
    # title("Vertical Bar Plot")
    # grid("on")
    # xlabel("X")
    # ylabel("Y")
end # ListAlgorithm

# Znajdź indeks komórki przechowującej najmniejszą wartość
function FindMinIndex(arr)
    index = 0
    min = typemax(Int64)
    for (i, v) in enumerate(arr)
        if v < min
            min = v
            index = i
        end
    end
    return index
end # FindMinIndex

ListAlgorithm(p, 3)
