H = [0 1 0 0 0 1;
     1 0 1 0 0 0;
     0 1 0 1 0 0;
     0 0 1 0 1 0;
     0 0 0 1 0 1;
     1 0 0 0 1 0]
t = 3 # t - całka przeskoku, dla węgla t = 3 eV
eV = 1.60217653 / (10^(19))

eigenvalues, eigenvectors = eig(3*eV*H)
println("Eigenvalues: ", eigenvalues)
println("Eigenvectors:", eigenvectors)

println("|ci|^2:")
for (i, eigenvalue) in enumerate(eigenvalues)
    println(i , " - ", eigenvalue^2)
end
