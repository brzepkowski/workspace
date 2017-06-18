using JuMP
using GLPKMathProgInterface
using LatexPrint
using VoronoiDelaunay
using Gadfly
using LightGraphs
using Mosek

tess = DelaunayTessellation()

width = max_coord - min_coord
a = Point2D[Point(min_coord+rand()*width, min_coord+rand()*width) for i in 1:5]
push!(tess, a)

x, y = getplotxy(voronoiedgeswithoutgenerators(tess))
# println("a: ", a)

primalPointsX = []
primalPointsY = []
for aˈ in a
  push!(primalPointsX, VoronoiDelaunay.getx(aˈ))
  push!(primalPointsY, VoronoiDelaunay.gety(aˈ))
end

vertices = []
edges = []

# ------Find all vertices, give them corresponding int numbers and create graph---------
for edge in voronoiedgeswithoutgenerators(tess)
  a = (getx(geta(edge)), VoronoiDelaunay.gety(geta(edge)))
  b = (getx(getb(edge)), VoronoiDelaunay.gety(getb(edge)))
  push!(vertices, a)
  push!(vertices, b)
end

vertices = union(vertices)
vertPoint2Int = Dict{Tuple{Float64, Float64}, Int}()
vertInt2Point = Dict{Int, Tuple{Float64, Float64}}()
for (i, vertex) in enumerate(vertices)
  vertPoint2Int[vertex] = i
  vertInt2Point[i] = vertex
end

graph = Graph(length(vertices))
# ---------------------Add edges to graph---------------------------------------

for edge in voronoiedgeswithoutgenerators(tess)
  a = vertPoint2Int[(VoronoiDelaunay.getx(geta(edge)), VoronoiDelaunay.gety(geta(edge)))]
  b = vertPoint2Int[(VoronoiDelaunay.getx(getb(edge)), VoronoiDelaunay.gety(getb(edge)))]
  add_edge!(graph, a, b)
end

# ------------------------------------------------------------------------------
verticesRemoved = []
verticesDegrees = Dict{Int, Int}()
for vertex in 1:length(vertices)
  verticesDegrees[vertex] = degree(graph, vertex)
  if (degree(graph, vertex) < 3)
    push!(verticesRemoved, vertex)
  end
end

verticesToCheck = []

for i in 1:length(vertices)
  for j in neighbors(graph, i)
    if (in(j, verticesRemoved))
      verticesDegrees[i] -= 1
    end
  end
end

for i in 1:length(vertices)
  print("(", i, " => ", vertInt2Point[i], "), ")
end
println()
println("Stopnie: ")
for i in 1:length(vertices)
  print("(", i, " => ", verticesDegrees[i] , "), ")
end
println()

for i in 1:length(vertices)
  if (verticesDegrees[i] >= 3)
    push!(verticesToCheck, i)
  end
end

println("Do sprawdzenia: ", verticesToCheck)


# -------------------Uporzadkowanie przed Solverem------------------------------
n = length(vertices)
model = Model(solver = MosekSolver())
@variable(model, x[1:n, 1:3, 1:2] >= 0)
for i in 1:n
  @NLobjective(model, Min, sum(x[i, j, k] for i in 1:n, j in 1:3, k in 1:2))
end

# println(typeof(model))
for v in verticesToCheck
  println(v, " - ", neighbors(graph, v))
end
# println(typeof(x))

function distance(x1::Float64, y1::Float64, x2::JuMP.Variable, y2::JuMP.Variable)
  return (x1 - x2)^2 + (y1 - y2)^2
end

visitedVertices = []

function addConstraints(model::JuMP.Model, vertex::Int, x::Array{JuMP.Variable,3})
  println("Weszlo(1): ", vertex, " => ", vertInt2Point[vertex])
  a = Float64(vertInt2Point[vertex][1])
  b = Float64(vertInt2Point[vertex][2])
  @constraint(model, distance(a, b, x[vertex, 1, 1], x[vertex, 1, 2]) == distance(a, b, x[vertex, 2, 1], x[vertex, 2, 2]))
  @constraint(model, distance(a, b, x[vertex, 2, 1], x[vertex, 2, 2]) == distance(a, b, x[vertex, 3, 1], x[vertex, 3, 2]))
  @constraint(model, distance(a, b, x[vertex, 3, 1], x[vertex, 3, 2]) == distance(a, b, x[vertex, 1, 1], x[vertex, 1, 2]))

  push!(visitedVertices, vertex)

  area1 = 1
  area2 = 3

  for i in neighbors(graph, vertex)
    if (verticesDegrees[i] >= 3)
      addConstraints(model, i, vertex, area1, area2)
      area1 = area1 + 1 % 3
      area2 = area2 + 1 % 3
    end
  end
end

function addConstraints(model::JuMP.Model, vertex::Int, previousVertex::Int, prevArea1::Int, prevArea2::Int)
    println("Weszlo(2): ", vertex, " => ", vertInt2Point[vertex])
    a = Float64(vertInt2Point[vertex][1])
    b = Float64(vertInt2Point[vertex][2])

    @constraint(model, distance(a, b, x[vertex, 1, 1], x[vertex, 1, 2]) == distance(a, b, x[vertex, 2, 1], x[vertex, 2, 2]))
    @constraint(model, distance(a, b, x[vertex, 2, 1], x[vertex, 2, 2]) == distance(a, b, x[vertex, 3, 1], x[vertex, 3, 2]))
    @constraint(model, distance(a, b, x[vertex, 3, 1], x[vertex, 3, 2]) == distance(a, b, x[vertex, 1, 1], x[vertex, 1, 2]))

    @constraint(model, x[vertex, 1, 1] == x[previousVertex, prevArea1, 1])
    @constraint(model, x[vertex, 1, 2] == x[previousVertex, prevArea1, 2])
    @constraint(model, x[vertex, 2, 1] == x[previousVertex, prevArea2, 1])
    @constraint(model, x[vertex, 2, 2] == x[previousVertex, prevArea2, 2])

    push!(visitedVertices, vertex)

    area1 = 1
    area2 = 3

    for i in neighbors(graph, vertex)
      if (verticesDegrees[i] >= 3 && !in(i, visitedVertices))
        addConstraints(model, i, vertex, area1, area2)
        area1 = area1 + 1 % 3
        area2 = area2 + 1 % 3
      end
    end
end

addConstraints(model, verticesToCheck[1], x)

# println(model)

status = solve(model)
println(getvalue(x))

# p1 = plot(layer(x=x, y=y, Geom.point, Geom.path),
#       layer(x = primalPointsX, y = primalPointsY, Geom.point),
#       Scale.x_continuous(minvalue=0.0, maxvalue=5.0),
#       Scale.y_continuous(minvalue=0.0, maxvalue=5.0))
