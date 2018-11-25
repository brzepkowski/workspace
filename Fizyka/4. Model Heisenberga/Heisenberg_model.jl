# This program assumes, that we are considering only physical systems with one type of spin
using LightGraphs, SimpleWeightedGraphs

sources = [1,2,3,4,5,6]
destinations = [2,3,4,5,6,1]
weights = [1,1,1,1,1,1] # TODO we can make three graphs of the same structure,
# but with different weights (for different exchange interaction values - Iˣ, Iʸ, Iᶻ)
graph = SimpleWeightedGraph(sources, destinations, weights)

println("Give the type of spin:")
spin = parse(Float64, readline(STDIN))
println("Spin = ", spin)

# Matrices for spin = 1/2
S0x = [1 0; 0 1]
S0y = [0 -im; im 0]
S0z = [1 0 ; 0 -1]

# Matrices for spin = 1
S1x = (1/sqrt(2))*[0 1 0; 1 0 1; 0 1 0]
S1y = (1/sqrt(2im))*[0 1 0; -1 0 1; 0 -1 0]
S1z = [1 0 0; 0 0 0; 0 0 -1]

# Matrices for spin = 3/2
S2x = (1/2)*[0 sqrt(3) 0 0; sqrt(3) 0 2 0; 0 2 0 sqrt(3); 0 0 sqrt(3) 0]
S2y = (1/2im)*[0 sqrt(3) 0 0; -sqrt(3) 0 2 0; 0 -2 0 sqrt(3); 0 0 -sqrt(3) 0]
S2z = [3/2 0 0 0; 0 1/2 0 0; 0 0 -1/2 0; 0 0 0 -3/2]

# Matrices for spin = 2
S3x = (1/2)*[0 2 0 0 0; 2 0 sqrt(6) 0 0; 0 sqrt(6) 0 sqrt(6) 0; 0 0 sqrt(6) 0 2; 0 0 0 2 0]
S3y = (1/2im)*[0 2 0 0 0; -2 0 sqrt(6) 0 0; 0 -sqrt(6) 0 sqrt(6) 0; 0 0 -sqrt(6) 0 2; 0 0 0 -2 0]
S3z = [2 0 0 0 0; 0 1 0 0 0; 0 0 0 0 0; 0 0 0 -1 0; 0 0 0 0 -2]

# Matrices for spin = 5/2
S4x = (1/2)*[0 sqrt(5) 0 0 0 0; sqrt(5) 0 sqrt(8) 0 0 0; 0 sqrt(8) 0 sqrt(9) 0 0; 0 0 sqrt(9) 0 sqrt(8) 0; 0 0 0 sqrt(8) 0 sqrt(5); 0 0 0 0 sqrt(5) 0]
S4y = (1/2im)*[0 sqrt(5) 0 0 0 0; -sqrt(5) 0 sqrt(8) 0 0 0; 0 -sqrt(8) 0 sqrt(9) 0 0; 0 0 -sqrt(9) 0 sqrt(8) 0; 0 0 0 -sqrt(8) 0 sqrt(5); 0 0 0 0 -sqrt(5) 0]
S4z = [5/2 0 0 0 0 0; 0 3/2 0 0 0 0; 0 0 1/2 0 0 0; 0 0 0 -1/2 0 0; 0 0 0 0 -3/2 0; 0 0 0 0 0 -5/2]

σˣ = []
σʸ = []
σᶻ = []

if spin == 0.5
    σˣ = S0x
    σʸ = S0y
    σᶻ = S0z
    H = zeros(2, 2)
elseif spin == 1
    σˣ = S1x
    σʸ = S1y
    σᶻ = S1z
    H = zeros(3, 3)
elseif spin == 1.5
    σˣ = S2x
    σʸ = S2y
    σᶻ = S2z
    H = zeros(4, 4)
elseif spin == 2
    σˣ = S3x
    σʸ = S3y
    σᶻ = S3z
    H = zeros(5, 5)
elseif spin == 2.5
    σˣ = S4x
    σʸ = S4y
    σᶻ = S4z
    H = zeros(6, 6)
else
    println("Given value of spin is incorrect.")
end

# H = [0 0; 0 0] # Hamiltonian
for edge in edges(graph)
    I = weight(edge)
    H += I*σˣ*σˣ
    H += I*σʸ*σʸ
    H += I*σᶻ*σᶻ
    # println("Żródło: ", src(edge), ", cel: ", dst(edge), ", waga: ", weight(edge))
end

println("H = ", H)

n = size(H)[1]
println("n: ", n)
eigenvalues, eigenvectors = eig(H)

println("Results:")
for i in 1:n
    println("E: ", eigenvalues[i])
    for j in 1:n
        println(eigenvectors[j,i]^2)
    end
end

function main(args)
    @show args
    println("Spin: ", spin)
    println("Całka przeskoku: ", exchange_interaction)
end # main
# main(ARGS)
