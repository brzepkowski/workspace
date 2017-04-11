using JuMP
using GLPKMathProgInterface

function dzwigi_model(n)
  odleglosci = [0.0 42.7 55.1 50.5 34.2 56.3 78.1;
               42.7 0.0 53.2 72.4 77.0 98.7 125.6;
               55.1 53.2 0.0 28.7 86.0 79.0 88.7;
               50.5 72.4 28.7 0.0 62.2 52.6 62.4;
               34.2 77.0 86.0 62.2 0.0 22.0 59.1;
               56.3 98.7 79.0 52.6 22.0 0.0 39.8;
               78.1 125.6 88.7 62.4 59.1 39.8 0.0]

  dzwigi = [7 -2;
            -10 1;
            6 2;
            -4 10;
            5 -4;
            -8 -2;
            0 -1]

  model = Model(solver = GLPKSolverLP())

  @variable(model, transport[1:n, 1:n, 1:2] >= 0)

  @objective(model, Min, sum(transport[i,j,1]*odleglosci[i,j] for i=1:n for j=1:n) + 1.2*sum(transport[i,j,2]*odleglosci[i,j] for i=1:n for j=1:n))

  #@constraint(model, [i=1:n,k=1:2], sum(transport[i,j,k] for j=1:n) - sum(transport[j,i,k] for j=1:n) == dzwigi[i, k])

  println("-------------MODEL----------------")
  println(model)
  println("----------------------------------")

  status = solve(model, suppress_warnings=true)
  println(getvalue(transport))
end

dzwigi_model(7)
