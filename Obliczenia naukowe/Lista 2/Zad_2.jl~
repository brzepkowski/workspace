function hilb(n::Int)
# Function generates the Hilbert matrix  A of size n,
#  A (i, j) = 1 / (i + j - 1)
# Inputs:
#	n: size of matrix A, n>0
#
#
# Usage: hilb (10)
#
# Pawel Zielinski
        if n < 1
         error("size n should be > 0")
        end
        A= Array(Float64, n,n)
        for j=1:n, i=1:n
                A[i,j]= 1 / (i + j - 1)
        end
        return A
end

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
Vector = zeros(4)

for i = 1:20
  A = hilb(i)
  x = ones(i)
  b = A * x
  y = inv(A)*b
  x = A \ b
  Vector = [cond(A), rank(A), x, y]
  #println(Vector)
  println(Vector[1], " , ", Vector[2], " , ", Vector[3], " , ", Vector[4])
#  print("Cond(A) = ", cond(A), " Rank(A) = ", rank(A), " Gauss = ", x)
 # x = inv(A)*b
  #println(" A^-1 b = ", x)
end

n = 5
c = float64([1.0, 10.0, 10.0^3, 10.0^7, 10.0^12, 10.0^16])
for i=1:6
  A = matcond(n, c[i])
  x = ones(n)
  b = A * x

  x = A \ b
  #print("Cond(A) = ", cond(A), " Rank(A) = ", rank(A), " Gauss = ", x)
  y = inv(A)*b
  #println(" A^-1 b = ", x)
  Vector = [cond(A), rank(A), x, y]
  #println(Vector)
  println(Vector[1], " , ", Vector[2], " , ", Vector[3], " , ", Vector[4])
end

n = 10
c = float64([1.0, 10.0, 10.0^3, 10.0^7, 10.0^12, 10.0^16])
for i=1:6
  A = matcond(n, c[i])
  x = ones(n)
  b = A * x

  x = A \ b
  #print("Cond(A) = ", cond(A), " Rank(A) = ", rank(A), " Gauss = ", x)
  y = inv(A)*b
  #println(" A^-1 b = ", x)
  Vector = [cond(A), rank(A), x, y]
  #println(Vector)
  println(Vector[1], " , ", Vector[2], " , ", Vector[3], " , ", Vector[4])
end

n = 20
c = float64([1.0, 10.0, 10.0^3, 10.0^7, 10.0^12, 10.0^16])
for i=1:6
  A = matcond(n, c[i])
  x = ones(n)
  b = A * x

  x = A \ b
  #print("Cond(A) = ", cond(A), " Rank(A) = ", rank(A), " Gauss = ", x)
  y = inv(A)*b
  #println(" A^-1 b = ", x)
  Vector = [cond(A), rank(A), x, y]
  #println(Vector)
  println(Vector[1], " , ", Vector[2], " , ", Vector[3], " , ", Vector[4])
end

