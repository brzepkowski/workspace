using JuMP
using GLPKMathProgInterface

P = [0 3/10 1/10 3/5;
     1/10 1/10 7/10 1/10;
     1/10 7/10 1/10 1/10;
     9/10 1/10 0 0]

# a)

c = ones(4)

model = Model(solver = GLPKSolverLP())
@variable(model, 0 <= π[1:4] <= 1)
@objective(model, Min, vecdot(c, π))
@constraint(model, [i=1:4], sum(π[j]*P[j, i] for j in 1:4) == π[i])
@constraint(model, sum(π[i] for i in 1:4) == 1)

status = solve(model)
fcelu = getobjectivevalue(model)
println("π = ", getvalue(π))

# b)
