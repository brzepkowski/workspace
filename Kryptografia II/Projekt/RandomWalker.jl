using LightGraphs


function randomWalker(N, d, l)
  S = collect(1:N)
  println(S[randperm(length(S))])
  graph = DiGraph(N)

  println(graph)
end

randomWalker(10, 2, 2)
