# This program assumes, that we are considering only physical systems with one type of spin
using LightGraphs, SimpleWeightedGraphs

# Tensor product of two square matrices - A and B
function tensorProduct(A, B)
    size_A = size(A)[1]
    size_B = size(B)[1]
    result = zeros(Complex{Float64}, size_A*size_B, size_A*size_B)
    for a_horizontal in 1:size_A
        for a_vertical in 1:size_A
            for b_horizontal in 1:size_B
                for b_vertical in 1:size_B
                    index_0 = ((a_horizontal - 1)*size_B) + b_horizontal
                    index_1 = ((a_vertical - 1)*size_B) + b_vertical
                    result[index_0, index_1] = A[a_horizontal, a_vertical]*B[b_horizontal, b_vertical]
                end
            end
        end
    end
    return result
end # tensorProduct

function printHamiltonian(H)
    n = size(H)[1]
    for i in 1:n
        for j in 1:n
            print(H[i, j], " ")
        end
        println()
    end
end # printHamiltonian

# Two spins
# sources = [1]
# destinations = [2]
# weights = [1]

# Benzene
sources = [1,2,3,4,5,6]
destinations = [2,3,4,5,6,1]
weights = [1,1,1,1,1,1] # TODO we can make three graphs of the same structure,
# but with different weights (for different exchange interaction values - Iˣ, Iʸ, Iᶻ)
graph = SimpleWeightedGraph(sources, destinations, weights)
graph_vertices = vertices(graph)

println("Give the type of spin:")
spin = parse(Float64, readline(STDIN))
println("Spin = ", spin)

# Matrices for spin = 1/2
S0x = (1/2)*[0 1; 1 0]
S0y = (1/2)*[0 -im; im 0]
S0z = (1/2)*[1 0 ; 0 -1]

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

number_of_vertices = length(graph_vertices)
σˣ = []
σʸ = []
σᶻ = []

if spin == 0.5
    σˣ = S0x
    σʸ = S0y
    σᶻ = S0z
    I = eye(2)
    H = zeros(2^(number_of_vertices), 2^(number_of_vertices))
elseif spin == 1
    σˣ = S1x
    σʸ = S1y
    σᶻ = S1z
    I = eye(3)
    H = zeros(3^(number_of_vertices), 3^(number_of_vertices))
elseif spin == 1.5
    σˣ = S2x
    σʸ = S2y
    σᶻ = S2z
    I = eye(4)
    H = zeros(4^(number_of_vertices), 4^(number_of_vertices))
elseif spin == 2
    σˣ = S3x
    σʸ = S3y
    σᶻ = S3z
    I = eye(5)
    H = zeros(5^(number_of_vertices), 5^(number_of_vertices))
elseif spin == 2.5
    σˣ = S4x
    σʸ = S4y
    σᶻ = S4z
    I = eye(6)
    H = zeros(6^(number_of_vertices), 6^(number_of_vertices))
else
    println("Given value of spin is incorrect.")
end

matrices = []
for vertex in graph_vertices
    if vertex == 1
        Sx = σˣ
        Sy = σʸ
        Sz = σᶻ
        for i in 2:number_of_vertices
            Sx = tensorProduct(Sx, I)
            Sy = tensorProduct(Sy, I)
            Sz = tensorProduct(Sz, I)
        end
    else
        Sx = I
        Sy = I
        Sz = I
        for i in 2:(vertex-1)
            Sx = tensorProduct(Sx, I)
            Sy = tensorProduct(Sy, I)
            Sz = tensorProduct(Sz, I)
        end
        Sx = tensorProduct(Sx, σˣ)
        Sy = tensorProduct(Sy, σʸ)
        Sz = tensorProduct(Sz, σᶻ)
        for i in (vertex+1):number_of_vertices
            Sx = tensorProduct(Sx, I)
            Sy = tensorProduct(Sy, I)
            Sz = tensorProduct(Sz, I)
        end
    end
    push!(matrices, [Sx, Sy, Sz])
end

H = []
for edge in edges(graph)
    source = src(edge)
    destination = dst(edge)
    Sx_source = matrices[source][1]
    Sy_source = matrices[source][2]
    Sz_source = matrices[source][3]
    Sx_destination = matrices[destination][1]
    Sy_destination = matrices[destination][2]
    Sz_destination = matrices[destination][3]
    J = weight(edge)
    if H == []
        H = J * Sx_source * Sx_destination
        H += J * Sy_source * Sy_destination
        H += J * Sz_source * Sz_destination
    else
        H += J * Sx_source * Sx_destination
        H += J * Sy_source * Sy_destination
        H += J * Sz_source * Sz_destination
    end
end

n = size(H)[1]
# printHamiltonian(H)
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
