using JuMP
using GLPKMathProgInterface
using Graphs

function pageRank(graph, α)

  n = num_vertices(graph)
  A = [0.0 for i in 1:n, j in 1:n]
  B = [1.0 for i in 1:n, j in 1:n]

  for i in vertices(graph)
    for neighbor in out_neighbors(i, graph)
      j = vertex_index(neighbor, graph)
      A[i, j] = 1 / out_degree(i, graph)
    end
  end

  P = (1 - α)*A + α*(1/n)*B

  # println(P)

  c = ones(n)

  model = Model(solver = GLPKSolverLP())
  @variable(model, 0 <= π[1:n] <= 1)
  @objective(model, Min, sum(π[i] for i in 1:n))
  @constraint(model, [i=1:n], sum(π[j]*P[j, i] for j in 1:n) == π[i])
  @constraint(model, sum(π[i] for i in 1:n) == 1)

  status = solve(model)
  fcelu = getobjectivevalue(model)
  println(α, " -> ", getvalue(π))

end # pageRank

graph = simple_graph(6)
add_edge!(graph, 1, 1)
# add_edge!(graph, 2, 3)
add_edge!(graph, 2, 5)
add_edge!(graph, 3, 1)
add_edge!(graph, 4, 2)
add_edge!(graph, 4, 5)
add_edge!(graph, 5, 4)
add_edge!(graph, 6, 3)

pageRank(graph, 0.0)
pageRank(graph, 0.15)
pageRank(graph, 0.5)
pageRank(graph, 1.0)
