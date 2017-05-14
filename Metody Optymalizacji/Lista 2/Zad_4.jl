using JuMP
using GLPKMathProgInterface
using Graphs

function printSchedule(schedule, Machines, Horizon, p)
  println("-------------------------------------------------")
  for machine in Machines
    print("M", machine, ": ")
    pointer = 1
    for i in schedule
      if i[3] == machine
        while pointer < i[2]
          print("___")
          pointer += 1
        end
        for j in 1:p[i[1]]
          if j == 1 # Pierwszy element
            if (i[1] / 10) < 1
              print("|", i[1], "-")
            else
              print("|", i[1])
            end
          end
          if j == p[i[1]]
            print("--|")
          end
          if j != 1 && j != p[i[1]]
            print("---")
          end
        end
        pointer += p[i[1]]
      end
    end
    println()
  end # Iteration over machines
  print("T:  ")
  for t in Horizon
    t = t-1
    if t < 10
      print("|", t, " ")
    else
      print("|", t)
    end
  end
  println()
  println("-------------------------------------------------")
end #printSchedule

function multipleMachines(p::Vector{Int}, m::Int, r::Vector{Int}, N::Int, graph)

  n = length(p)
  #  n - liczba zadan
  #  p - wektor czasow wykonania zadan
  #  r - wektor momentow dostepnosci zadan
  #  w - wektor wag zadan

  T = sum(p) + 1 # dlugosc horyzontu czasowego

  model = Model(solver = GLPKSolverMIP())

  Task = 1:n
  Horizon = 1:T
  Machines = 1:m

	@variable(model, x[Task, Horizon, Machines], Bin)

	# Funkcja celu
  @objective(model, Min, sum(((t-1) + p[j]) * x[j, t, m] for j in Task, t in Horizon, m in Machines))

	# dokladnie jeden moment rozpoczenia j-tego zadania
	for j in Task
		@constraint(model, sum(x[j,t, m] for t in 1:T-p[j]+1, m in Machines) == 1)
	end

	# moment rozpoczecia j-tego zadan co najmniej jak moment gotowosci rj zadania
	for j in Task
    for neighbor in out_neighbors(j, graph)
	    @constraint(model, sum(x[neighbor, t, m]*(t-1) for t in 1:T-p[neighbor]+1, m in Machines) >= sum(x[j, t, m]*(t-1) for t in 1:T-p[j]+1, m in Machines) + p[j])
    end
	end

  # ograniczenie na wykorzystywane zasoby
  for t in Horizon
    @constraint(model, sum(x[j, t̂, m]*r[j] for j in Task, m in Machines, t̂ in max(1, t-p[j]+1):t) <= N)
  end

	# zadania nie nakladaja sie na siebie
	for t in Horizon
    for m in Machines
		    @constraint(model, sum(x[j, s, m] for j in Task, s in max(1, t-p[j]+1):t) <= 1)
      end
	end

	#print(model) # drukuj model

	status = solve(model) # rozwiaz egzemplarz

  fcelu = getobjectivevalue(model)
  momenty = getvalue(x)

  schedule = []

  if status==:Optimal
  	 println("Funkcja celu: ", fcelu)
     #println("momenty rozpoczecia zadan: ", momenty)
     for i in 1:JuMP.size(momenty, 1)
       for j in 1:JuMP.size(momenty, 2)
         for k in 1:JuMP.size(momenty, 3)
           if momenty[i, j, k] == 1
             push!(schedule, (i, j, k))
             println("[", i, ", ", j, ", ", k, "] -> ", momenty[i, j, k])
           end
         end
       end
     end
   else
     println("Status: ", status)
  end

  printSchedule(schedule, Machines, Horizon, p)

end # multipleMachines

graph = simple_graph(8)
add_edge!(graph, 1, 2)
add_edge!(graph, 1, 3)
#=add_edge!(graph, 1, 4)
add_edge!(graph, 2, 5)
add_edge!(graph, 3, 6)
add_edge!(graph, 4, 6)
add_edge!(graph, 4, 7)
add_edge!(graph, 5, 8)
add_edge!(graph, 6, 8)
add_edge!(graph, 7, 8)
=#
# czasy wykonia j-tego zadania
p = [ 5;
      4;
		  6]
	    #=46;
		  32;
      57;
      15;
      62]=#

# zapotrzebowanie na zasoby
r = [ 9;
      17;
      11]
      #=4;
      13;
      7;
      7;
      17]=#

multipleMachines(p, 3, r, 30, graph)
