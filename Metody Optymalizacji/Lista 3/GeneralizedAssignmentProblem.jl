# Autor - Bartosz Rzepkowski
# Metody optymalizacji - Lista 3
using LightGraphs
using JuMP
using GLPKMathProgInterface
using LatexPrint

function readData(pathToFile)
  readFile = open(pathToFile);
  s = readstring(readFile)
  allData = split(s)

  problemsAmount = parse(Int, allData[1])
  machinesAmount = -1
  jobsAmount = -1
  index = 1
  for i in 1:problemsAmount
    machinesAmount = parse(Int, allData[index + 1])
    jobsAmount = parse(Int, allData[index + 2])
    graph = DiGraph(jobsAmount + machinesAmount)

    J = 1:jobsAmount
    M = jobsAmount+1:jobsAmount+machinesAmount

    costs = Array{Int64}(machinesAmount, jobsAmount)
    resources = Array{Int64}(machinesAmount, jobsAmount)
    capacities = Array{Int64}(machinesAmount)
    index += 2
    # Odczytaj dane o kosztach
    endPoint = index + (jobsAmount * machinesAmount)
    row = 1
    while index < endPoint
      innerIndex = 1
      column = 1
      while innerIndex <= jobsAmount
        costs[row, column] = parse(Int, allData[index + innerIndex])
        column += 1
        innerIndex += 1
      end
      row += 1
      index += jobsAmount
    end
    # Odczytaj dane o zasobach
    endPoint = index + (jobsAmount * machinesAmount)
    row = 1
    while index < endPoint
      innerIndex = 1
      column = 1
      while innerIndex <= jobsAmount
        resources[row, column] = parse(Int, allData[index + innerIndex])
        column += 1
        innerIndex += 1
      end
      row += 1
      index += jobsAmount
    end
    index += 1
    # Odczytaj dane o pojemnościach
    endPoint = index + machinesAmount
    column = 1
    while index < endPoint
      capacities[column] = parse(Int, allData[index])
      column += 1
      index += 1
    end
    index -= 1
    solveIterativeModel(machinesAmount, jobsAmount, J, M, costs, resources, capacities)
  end

  close(readFile)
end # readData

function solveIterativeModel(machinesAmount, jobsAmount, J, M, costs, resources, capacities)

    M = M-jobsAmount

    graph = []
    for i in M
      for j in J
        push!(graph, (i, j))
      end
    end

    Mˈ = [i for i in M]
    Jˈ = [j for j in J]
    F = []

    while (Jˈ != [])

      model = Model(solver = GLPKSolverMIP())

      @variable(model, x[M, J] >= 0)

      @objective(model, Min, sum(costs[i, j]*x[i, j] for (i, j) in graph))

      for j in J
        if (in(j, Jˈ))
          edges = [e for e in graph if e[2] == j]
          # println("j: ", j, ", edges: ", edges)
          @constraint(model, sum(x[i, jˈ] for (i, jˈ) in edges) == 1)
        end
      end

      # println("Maszyny:")
      for i in M
        if (in(i, Mˈ))
          edges = [e for e in graph if e[1] == i]
          # println("i: ", i, ", edges: ", edges)
          @constraint(model, sum(x[iˈ, j]*resources[iˈ, j] for (iˈ, j) in edges) <= capacities[i])
        end
      end

    	status = solve(model)
      solution = getvalue(x)
      # println(solution)

      # --------------- USUWANIE -----------------

      for i in M
        for j in J
          if (solution[i, j] == 0)
            deleteat!(graph, findin(graph, [(i, j)]))
          end
        end
      end

      for i in M
        for j in J
          if (solution[i, j] == 1)
            push!(F, (i, j))
            deleteat!(Jˈ, findin(Jˈ, [j]))
            # println("2) Usuwam j = ", j)
            # println("i: ", i, ", j: ", j)
            # println("przed:", capacities)
            capacities[i] -= resources[i, j]
            # println("po   :", capacities)
          end
        end
      end

      for i in M
        degree = length([e for e in graph if e[1] == 1])
        sum = 0
        for j in J
          sum += solution[i, j]
        end
        if (degree == 1 || (degree == 2 && sum >= 1))
          deleteat!(Mˈ, findin(Mˈ, [i]))
          # println("3) Usuwam i = ", i)
        end
      end

      println("-------GRAPH-------")
      println(graph)
      # println(lap(transpose(graph)))
      # println("--------F----------")
      # println(lap(transpose(F)))
    end
    println("--------F----------")
    println(F)


end # solveIterativeModel

pathToDirectory = "/home/bartas/workspace/Metody\ Optymalizacji/Lista\ 3/"
readData(string(pathToDirectory, "gap1.txt"))
