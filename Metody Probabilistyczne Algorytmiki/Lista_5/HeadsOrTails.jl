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
function TestGame1(m)
    E = 0
    V = 0
    results = []
    for i in 1:m
        result = HeadsOrTails1()
        E += result
        push!(results, result)
    end
    E = E / m
    # for r in results
    #     V += (r - E)^2
    # end
    # V = V / (m - 1)
    println("E: ", E)
    # println("V: ", V)
end # TestGame1

# m - liczba eksperymentów
function TestGame2(m)
    E = 0
    V = 0
    results = []
    for i in 1:m
        result = HeadsOrTails2()
        E += result
        push!(results, result)
    end
    E = E / m
    # for r in results
    #     V += (r - E)^2
    # end
    # V = V / (m - 1)
    println("E: ", E)
    # println("V: ", V)
end # TestGame2

# println("1: ", HeadsOrTails1())
# println("2: ", HeadsOrTails2())
TestGame1(10000)
TestGame2(10000)
