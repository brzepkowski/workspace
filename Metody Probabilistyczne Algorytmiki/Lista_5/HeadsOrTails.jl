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
    E1 = BigInt(0)
    E2 = BigInt(1)
    E3 = BigInt(0)
    V = BigInt(0)
    results = []
    for i in 1:m
        result = HeadsOrTails1()
        E1 += result
        E2 *= result
        E3 += 1/result
        push!(results, result)
    end
    E1 = E1 / m
    E2 = E2^(1/m)
    E3 = m / E3
    for r in results
        V += (r - E1)^2
    end
    V = V / (m - 1)
    println("E1: ", E1)
    println("E2: ", E2)
    println("E3: ", E3)
    println("V: ", V)
end # TestGame1

# m - liczba eksperymentów
function TestGame2(m)
    E1 = BigInt(0)
    E2 = BigInt(1)
    E3 = BigInt(0)
    V = 0
    results = []
    for i in 1:m
        result = HeadsOrTails2()
        E1 += result
        E2 *= result
        E3 += 1/result
        push!(results, result)
    end
    E1 = E1 / m
    E2 = E2^(1/m)
    E3 = m / E3
    for r in results
        V += (r - E1)^2
    end
    V = V / (m - 1)
    println("E1: ", E1)
    println("E2: ", E2)
    println("E3: ", E3)
    println("V: ", V)
end # TestGame2

TestGame1(10000)
TestGame2(10000)
