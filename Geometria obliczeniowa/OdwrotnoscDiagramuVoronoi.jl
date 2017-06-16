using JuMP
using GLPKMathProgInterface
using LatexPrint
using VoronoiDelaunay
using Gadfly


tess = DelaunayTessellation()

width = max_coord - min_coord
a= Point2D[Point(min_coord+rand()*width, min_coord+rand()*width) for i in 1:3]
push!(tess, a)

x, y = getplotxy(voronoiedges(tess))
# x = filter(!isnan, x)
# y = filter(!isnan, y)
println("a: ", a)
println("x: ", x)
println("y: ", y)
# set_default_plot_size(15cm, 15cm)
p1 = plot(x=x, y=y, Geom.point, Geom.path, Scale.x_continuous(minvalue=1.0, maxvalue=2.0), Scale.y_continuous(minvalue=1.0, maxvalue=2.0))
# draw(SVG("output.svg", 6inch, 3inch), p)

# plot(x, y, color="red", linewidth=2.0, linestyle="--")

# function solveIterativeModel(machinesAmount, jobsAmount, J, M, costs, resources, capacities)
#
#       model = Model(solver = GLPKSolverMIP())
#       @variable(model, x[M, J] >= 0)
#       @objective(model, Min, sum(costs[i, j]*x[i, j] for (i, j) in graph))
#       @constraint(model, sum(x[i, jˈ] for (i, jˈ) in edges) == 1)
#       @constraint(model, sum(x[iˈ, j]*resources[iˈ, j] for (iˈ, j) in edges) <= capacities[i])
#
#     	status = solve(model)
#       solution = getvalue(x)
#
# end # solveIterativeModel
