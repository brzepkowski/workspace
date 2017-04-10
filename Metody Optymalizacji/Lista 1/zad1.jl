using JuMP
using GLPKMathProgInterface
using Clp

function generateVariables(n)
  A = Matrix{Float64}(n, n)
  b = Vector{Float64}(n)
  c = Vector{Float64}(n)

  # Wypełnij macierz Hilberta A wartościami
  for i in 1:n
    for j in 1:n
      A[i, j] = 1 / (i + j - 1)
    end
  end

  # Wypełnij wektory b i c wartościami
  for i in 1:n
    sum = 0
    for j in 1:n
      sum = sum + (1 / (i + j - 1))
    end
    b[i] = sum
    c[i] = sum
  end

  return (A, b, c)
end

function hilbertGLPK(n)
  (A, b, c) = generateVariables(n)

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
end

function hilbertCLP(n)
  (A, b, c) = generateVariables(n)

  model = Model(solver = ClpSolver())
	@variable(model, x[1:n]>=0)
	@objective(model,Min, vecdot(c,x))
  @constraint(model,A*x .==b)
	# print(model)
  status = solve(model, suppress_warnings=true)

  x̂ = Vector{Float64}(n)
  x̂ = getvalue(x) # Rozwiązanie obliczone
  x = ones(n) # Rozwiązanie dokładne

  println("-----CLP-----")
  println("Status = ", status)
  println("Wartość funkcji celu = ", getobjectivevalue(model))
  println("x̂ = ", x̂)
  # ||x||₂ = √(x₁² + x₂² + ... + xₙ²) - norma Euklidesowa
  println("Błąd względny = ", norm(x - x̂) / norm(x))
end

hilbertGLPK(10)
hilbertCLP(10)
