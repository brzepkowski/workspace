# Bartosz Rzepkowski
# Metdoy optymalizacji - Lista 2
# Zadanie 1
using JuMP
using GLPKMathProgInterface

function subSets(widths::Array{Int64}, index::Int64, maxWidth::Int64, memory::Array{Int64}, allSubSets::Set{Array{Int64}})

    if (maxWidth > 0)
      if !in(memory, allSubSets)
        push!(allSubSets, memory)
      end
    end

    # If n is 0 then there is 1 solution (do not include any coin)
    if maxWidth == 0
      return 1
    end

    # If n is less than 0 then no solution exists
    if maxWidth < 0
      return 0
    end

    # If there are no coins and n is greater than 0, then no solution exist
    if index <= 0 && maxWidth >= 0
      return 0
    end

    # count is sum of solutions (i) including S[m-1] (ii) excluding S[m-1]
    return subSets(widths, index - 1, maxWidth, copy(memory), allSubSets) + subSets(widths, index, maxWidth - widths[index], append!(copy(memory), widths[index]), allSubSets)
end

function generateSubSets(widths::Array{Int64}, maxWidth::Int64)
  allSubSets = Set{Array{Int64}}()
  subSets(widths, length(widths), maxWidth, Int64[], allSubSets)
  return allSubSets
end

function generateSolution(order::Array{Array{Int64, 1}, 1}, maxWidth::Int64)

  widths = [x[1] for x in order]
  totalBoardsAmounts = [[x[1], 0] for x in order]
  allSubSets = generateSubSets(widths, maxWidth)
  n = length(allSubSets)

  leftovers = zeros(n)
  for i in 1:n
    leftovers[i] = maxWidth
  end

  model = Model(solver = GLPKSolverMIP())
  @variable(model, boards[1:n] >= 0)

  for (index, subSet) in enumerate(allSubSets)
    for element in subSet
      leftovers[index] -= element
    end
  end

  @objective(model, Min, sum(boards[i]*leftovers[i] for i=1:n))

  @constraint(model, [j=1:length(order)], sum(boards[i] for (i, subSet) in enumerate(allSubSets) for element in subSet if element == order[j][1]) >= order[j][2])

  status = solve(model, suppress_warnings=true)
  solution = getvalue(boards)

  index = 1
  for i in allSubSets
    if solution[index] != 0
      println(i, " -> ", solution[index])
    end
    index += 1
  end
end

generateSolution([[7, 110], [5, 120], [3, 80]], 22)
