# Bartosz Rzepkowski
# Metdoy optymalizacji - Lista 2
# Zadanie 2
# Na podstawie programu autorstwa prof. Pawła Zielińskiego
using JuMP
using GLPKMathProgInterface

function singleMachine(p::Vector{Int}, r::Vector{Int}, w::Vector{Float64})

 n = length(p)
 T = maximum(r) + sum(p) + 1 # Długość horyzontu czasowego

 model = Model(solver = GLPKSolverMIP())

 Tasks = 1:n
 Horizon = 1:T

	@variable(model, x[Tasks,Horizon], Bin)

	# Minimalizacja ∑ w[i]c[i], gdzie c jest czsem zakończenia danego zadania
	@objective(model,Min, sum(w[j] * ((t-1) + p[j]) * x[j,t] for j in Tasks, t in Horizon))

	# Dokładnie jeden moment rozpoczęcia j-tego zadania
	for j in Tasks
		@constraint(model,sum(x[j,t] for t in 1:T-p[j]+1) == 1)
	end

	# Moment rozpoczęcia j-tego zadania co najmniej jak moment gotowości rⱼ zadania
	for j in Tasks
		@constraint(model,sum((t-1)*x[j,t] for t in 1:T-p[j]+1) >= r[j])
	end

	# Zadania nie nakladaja się na siebie
	for t in Horizon
		@constraint(model,sum(x[j,s]  for j in Tasks, s in max(1, t-p[j]+1):t)<=1)
	end

	print(model)

	status = solve(model)

	if status==:Optimal
		 return status, getobjectivevalue(model), getvalue(x)
	else
		return status, nothing,nothing
	end

end # singleMachine

# Czasy wykonia
p=[ 3;
    2;
		4;
	  5;
		1]
# Momenty dostepnosci
r=[ 2;
		1;
	  3;
		1;
		0]
# Wagi
w=[ 1.0;
		1.0;
	  1.0;
		1.0;
		1.0]

(status, fcelu, momenty) = singleMachine(p,r,w)

if status==:Optimal
	 println("funkcja celu: ", fcelu)
   println("momenty rozpoczecia zadan: ", momenty)
else
   println("Status: ", status)
end
