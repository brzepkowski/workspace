using PyPlot

# m - liczba kul, n - liczba urn
function BallsInUrns(m :: Int, n :: Int)
    urns = zeros(Int64, n)
    for i in 1:m
        index = rand(1:n)
        urns[index] += 1
    end

    return urns
end # BallsInUrns

# e - liczba eksperymentów, m powinno być równe n
function MaxLoad(m :: Int, n :: Int, e :: Int)
    results = []
    for i in 1:e
        push!(results, maximum(BallsInUrns(m, n)))
    end
    return results
end # MaxAmountOfBalls

# e - liczba eksperymentów, m powinno być równe n
function EmptyUrns(m :: Int, n :: Int, e :: Int)
    results = []
    for i in 1:e
        A = BallsInUrns(m, n)
        empty = 0
        for j in 1:n
            if A[j] == 0
                empty += 1
            end
        end
        push!(results, empty)
    end
    return results
end # EmptyUrns

# m - liczba kul, n - liczba urn, e - liczba experymentów
function PlotMaxLoad(m, n, e, ax)
  x = []
  results = []
  expectedValue = []
  experimentalExpectedValue = []
  bounds = []
  bounds2 = []
  for i in 1000:1000:n
    percentages = zeros(20)
    push!(x, i)
    maxLoad = MaxLoad(i, i, e)
    push!(results, maxLoad)
    E = 0
    V² = 0
    for j in 1:length(maxLoad)
        E += maxLoad[j]
        percentages[maxLoad[j]] += 1
    end
    E = E / length(maxLoad)
    for j in 1:length(maxLoad)
        V² += (maxLoad[j] - E)^2
    end
    V² = V² / length(maxLoad)
    push!(experimentalExpectedValue, E)
    push!(expectedValue, ((1 + 0.57)*log(i)) / (log(log(i))))
    push!(bounds, [E + 2*sqrt(V²), E - 2*sqrt(V²)])
    push!(bounds2, [log(i) / (log(log(i))), (3*log(i)) / (log(log(i)))])
    len = length(maxLoad)
    println("Liczba urn: ", i)
    outsideBoundsCounter = 0
    for k in 1:length(percentages)
        if percentages[k] != 0
            if k < E - 2*sqrt(V²) || k > E + 2*sqrt(V²)
                outsideBoundsCounter += percentages[k]
            end
            println(k, " - ", percentages[k] / len)
        end
    end
    println("Dane poza ograniczeniami: ", outsideBoundsCounter/len)
  end
  ax[:plot](x, results, color="red", "o")
  ax[:plot](x, expectedValue, label="Teoretyczna wart. oczekiwana", color="blue", "-")
  ax[:plot](x, experimentalExpectedValue, label="Wyznaczona eksperymentalnie wart. oczekiwana", color="black", "-")
  ax[:plot](x, bounds, label="Ograniczenia z nier. Czebyszewa", color="red", "--")
  ax[:plot](x, bounds2, color="green", "--")
  ax[:legend](loc="best")

  grid("on")
  xlabel("Liczba urn")
  ylabel("Max load")
  title("Max Load")
end # PlotMaxAmountOfBalls

# m - liczba kul, n - liczba urn, e - liczba experymentów
function PlotEmptyUrns(m, n, e, ax)
  x = []
  results = []
  expectedValue = []
  experimentalExpectedValue = []
  bounds = []
  for i in 1000:1000:n
    push!(x, i)
    emptyUrns = EmptyUrns(m, i, e)
    push!(results, emptyUrns)
    E = 0
    V² = 0
    for j in 1:length(emptyUrns)
        E += emptyUrns[j]
    end
    E = E / length(emptyUrns)
    for j in 1:length(emptyUrns)
        V² += (emptyUrns[j] - E)^2
    end
    V² = V² / length(emptyUrns)
    push!(experimentalExpectedValue, E)
    push!(expectedValue, i*(1 - (1/i))^m)
    push!(bounds, [E + 2*sqrt(V²), E - 2*sqrt(V²)])
  end
  ax[:plot](x, results, color="red", "+")
  ax[:plot](x, expectedValue, label="Teoretyczna wart. oczekiwana", color="blue", "-")
  ax[:plot](x, experimentalExpectedValue, label="Wyznaczona eksperymentalnie wart. oczekiwana", color="black", "-")
  ax[:plot](x, bounds, label="Ograniczenia z nier. Czebyszewa", color="red", "--")
  grid("on")
  xlabel("Liczba urn")
  ylabel("Liczba pustych urn")
  annotate(string("Liczba kul: ", m),
	xy=[1;0],
	xycoords="axes fraction",
	xytext=[-10,10],
	textcoords="offset points",
	fontsize=15.0,
	ha="right",
	va="bottom")
  ax[:legend](loc="best")
  title("Liczba pustych urn")
end # PlotEmptyUrns

fig, ax = PyPlot.subplots()
PlotMaxLoad(10000, 10000, 1000, ax)
# fig, ax = PyPlot.subplots()
# PlotEmptyUrns(2000, 10000, 1000, ax)
# PlotEmptyUrns(5000, 10000, 1000, ax)
# PlotEmptyUrns(20000, 10000, 1000, ax)
