using JuMP
using GLPKMathProgInterface

P = [0 3/10 1/10 3/5;
     1/10 1/10 7/10 1/10;
     1/10 7/10 1/10 1/10;
     9/10 1/10 0 0]

# a)

model = Model(solver = GLPKSolverLP())
@variable(model, 0 <= π[1:4] <= 1)
@objective(model, Min, sum(π[i] for i in 1:4))
@constraint(model, [i=1:4], sum(π[j]*P[j, i] for j in 1:4) == π[i])
@constraint(model, sum(π[i] for i in 1:4) == 1)

status = solve(model)
fcelu = getobjectivevalue(model)
π = getvalue(π)
println("a) π = ", π)

# b)

p = [1 0 0 0]
pˈ = (p*(P^32))[4]
println("b) p = ", pˈ)

# c)

p = [1/4 1/4 1/4 1/4]
pˈ = (p*(P^128))[4]
println("c) p = ", pˈ)

# d)
println("d)")
π = transpose(π)
p = [1 0 0 0]
ϵ = 1/10
for i in 1:3
  pˈ = p*P
  t = 1
  max = 1
  while max > ϵ
    pˈ = pˈ * P
    t += 1
    # Oblicz max
    max = 0
    for (i, element) in enumerate(pˈ)
      if abs(element - π[i]) > max
        max = abs(element - π[i])
      end
    end
  end
  println(i, ") t = ", t)
  ϵ = ϵ/10
end
