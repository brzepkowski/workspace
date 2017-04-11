using JuMP
using GLPKMathProgInterface

model = Model(solver = GLPKSolverLP())
@variable(model, x[1:n]>=0)
@objective(model,Min, vecdot(c,x))
@constraint(model,A*x .==b)
# print(model)
status = solve(model, suppress_warnings=true)

x̂ = Vector{Float64}(n)
x̂ = getvalue(x) # Rozwiązanie obliczone
x = ones(n) # Rozwiązanie dokładne

println("-----GLPK-----")
println("Status = ", status)
println("Wartość funkcji celu = ", getobjectivevalue(model))
println("x̂ = ", x̂)
# ||x||₂ = √(x₁² + x₂² + ... + xₙ²) - norma Euklidesowa
println("Błąd względny = ", norm(x - x̂) / norm(x))
