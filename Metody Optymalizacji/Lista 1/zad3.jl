# Bartosz Rzepkowski
# Metdoy optymalizacji - Lista 1
# Zadanie 3
using JuMP
using GLPKMathProgInterface

model = Model(solver = GLPKSolverLP()) # Zastosuj solver GLPKSolverLP
#------Variables------
@variable(model, B1 >= 0)
@variable(model, B2 >= 0)
@variable(model, K >= 0)
@variable(model, PS >= 0)
@variable(model, DPO >= 0)
@variable(model, CPO >= 0)
@variable(model, x₁ >= 0)
@variable(model, x₂ >= 0)
@variable(model, x₃ >= 0)
@variable(model, x₄ >= 0)
@variable(model, x₅ >= 0)
@variable(model, x₆ >= 0)
@variable(model, x₇ >= 0)
@variable(model, x₈ >= 0)

#------Objective-------
@objective(model,Min, 1300*B1 + 10*B1 + 1500*B2 + 10*B2 + 20*x₁ + 20*x₃) # Funkcja celu -> minimalizacja kosztów produkcji paliw

#-----Constraints------
@constraint(model, 0.15*B1 + 0.1*B2 + 0.5*K == PS)
@constraint(model, x₅ + x₇ + 0.2*K == DPO)
@constraint(model, x₆ + x₈ + 0.06*K +x₂ + x₄ + 0.15*B1 + 0.25*B2 == CPO)
@constraint(model, x₁ + x₃ == K)
@constraint(model, x₁ + x₂ == 0.15*B1)
@constraint(model, x₃ + x₄ == 0.2*B2)
@constraint(model, x₅ + x₆ == 0.4*B1)
@constraint(model, x₇ + x₈ == 0.35*B2)
@constraint(model, PS >= 200000)
@constraint(model, DPO >= 400000)
@constraint(model, CPO >= 250000)
@constraint(model, x₅*0.002 + x₇*0.012 + 0.2*(x₁*0.003 + x₃*0.025) <= 0.005*DPO) # Siarka

# print(model)
status = solve(model, suppress_warnings=true)
println("B1 = ", getvalue(B1))
println("B2 = ", getvalue(B2))
println("PS = ", getvalue(PS))
println("DPO = ", getvalue(DPO))
println("CPO = ", getvalue(CPO))
println("x1 = ", getvalue(x₁))
println("x2 = ", getvalue(x₂))
println("x3 = ", getvalue(x₃))
println("x4 = ", getvalue(x₄))
println("x5 = ", getvalue(x₅))
println("x6 = ", getvalue(x₆))
println("x7 = ", getvalue(x₇))
println("x8 = ", getvalue(x₈))
println(getobjectivevalue(model))
