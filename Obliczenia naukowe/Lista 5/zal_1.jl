push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 5/Programy")
using systems

function matcond(n::Int, c::Float64)
# Function generates a random square matrix A of size n with
# a given condition number c.
# Inputs:
#	n: size of matrix A, n>1
#	c: condition of matrix A, c>= 1.0
#
# Usage: matcond (10, 100.0);
#
# Pawel Zielinski
        if n < 2
         error("size n should be > 1")
        end
        if c< 1.0
         error("condition number  c of a matrix  should be >= 1.0")
        end
        (U,S,V)=svd(rand(n,n))
        return U*diagm(linspace(1.0,c,n))*V'
end

pivot=true
i=0
while i<2
	n=10  #rozmiar
	c=10.0+19.0*rand() # wskaznik uwarunkowania
	A=matcond(n, c)
	b=A*ones(n)
	println("Dane")
	println("n=", n)
	println("cond=",c)


	if pivot==true
		 println("z czesciowym wyborem")
	 else
		 println("bez wyboru")
	 end
	 println()
	 #println("nacisnij Enter... ")
	 #readline(STDIN)

	 #****test Gaussa************
	 println("Test eliminacji Gaussa")
	 (x,err)=Gauss(A,b,pivot)


	 if err == 0
		 println("brak bledu")
	 elseif err == 1
		 println("Blad !!!!!")
	 else
		 println("nie znany blad!!!")
	 end

	 println()
	 println("obliczone rozwiaznie x")
	 println(x)
	 println()
	 println("||x-dokl||/||dokl||=", norm(x-ones(n))/norm(ones(n)))


	 i+= 1
	 pivot=!pivot

	 println()
	# println("nacisnij Enter... ")
	 #readline(STDIN)
end

