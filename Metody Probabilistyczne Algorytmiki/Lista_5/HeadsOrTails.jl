using PyPlot

function HeadsOrTails1()
    prize = 0
    for i in 1:5
        result = rand(0:1)
        if result == 0 # wypada reszka
            prize += 11
        else
            prize -= 110
        end
    end
    return prize
end # HeadsOrTails1

function HeadsOrTails2()
    prize = 0
    for i in 1:1100
        result = rand(0:1)
        if result == 0 # wypada reszka
            prize -= 0.5
        else
            prize += 0.05
        end
    end
    return prize
end # HeadsOrTails2

# m - liczba eksperymentów
function TestGame1(m, ax)
    results = []
    x = []
    ExpectedVars = []
    Variances = []
    CzebyszewBounds = []
    above = 0
    for i in 1000:1000:m
        E = 0
        V = 0
        push!(x, i)
        tempResults = []
        for j in 1:i
            result = HeadsOrTails1()
            if result > -247.5
                above += 1
            end
            E += result
            push!(tempResults, result)
        end
        E = E / i
        for r in tempResults
            V += (r - E)^2
        end
        V = V / (i - 1)
        println("E: ", E)
        println("V: ", V)
        push!(results, tempResults)
        push!(ExpectedVars, E)
        push!(Variances, V)
        push!(CzebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
    end
    println("Above 1: ", above)
    # ax[:plot](x, results, color="red", ".")
    ax[:plot](x, ExpectedVars, color="black", "-")
    ax[:plot](x, CzebyszewBounds, color="black", "--")
end # TestGame1

# m - liczba eksperymentów
function TestGame2(m, ax)
    results = []
    x = []
    ExpectedVars = []
    Variances = []
    CzebyszewBounds = []
    above = 0
    for i in 1000:1000:m
        E = 0
        V = 0
        push!(x, i)
        tempResults = []
        for j in 1:i
            result = HeadsOrTails2()
            if result > -247.5
                above += 1
            end
            E += result
            push!(tempResults, result)
        end
        E = E / i
        for r in tempResults
            V += (r - E)^2
        end
        V = V / (i - 1)
        println("E: ", E)
        println("V: ", V)
        push!(results, tempResults)
        push!(ExpectedVars, E)
        push!(Variances, V)
        push!(CzebyszewBounds, [E - 2*sqrt(V), E + 2*sqrt(V)])
    end
    println("Above 2: ", above)
    ax[:plot](x, ExpectedVars, color="red", "-")
    ax[:plot](x, CzebyszewBounds, color="red", "--")
end # TestGame2

function TestGame1_2(m)
    above = 0
    for i in 1:m
        result = HeadsOrTails1()
        if result > -247.5
            above += 1
        end
    end
    return above/m
end # TestGame1_2

function TestGame2_2(m)
    above = 0
    for i in 1:m
        result = HeadsOrTails2()
        if result > -247.5
            above += 1
        end
    end
    return above/m
end # TestGame2_2

# fig, ax = PyPlot.subplots()
# TestGame1(10000, ax)
# TestGame2(10000, ax)

greater1 = 0
greater2 = 0
for i in 1:500
    result1 = TestGame1_2(1000)
    # println("result1: ", result1)
    result2 = TestGame2_2(1000)
    # println("result2: ", result2)
    if result1 > result2
        greater1 += 1
    else
        greater2 += 1
    end
end
println(greater1)
println(greater2)
