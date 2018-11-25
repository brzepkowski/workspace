H = [0 1 0 0 0 1;
     1 0 1 0 0 0;
     0 1 0 1 0 0;
     0 0 1 0 1 0;
     0 0 0 1 0 1;
     1 0 0 0 1 0]

t = 3 # t - całka przeskoku, dla węgla t = 3 eV
eV = 1.60217653 / (10^(19))

n = 6
eigenvalues, eigenvectors = eig(3*H)
# println("Eigenvalues: ", eigenvalues)
# println("Eigenvectors:", eigenvectors)


println("Results:")
for i in 1:n
    println("E: ", eigenvalues[i])
    for j in 1:n
        println(eigenvectors[j,i]^2)
    end
end
