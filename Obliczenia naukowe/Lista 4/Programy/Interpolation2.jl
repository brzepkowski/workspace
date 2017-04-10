#Obliczenia naukowe - Lista 4
#Bartosz Rzepkowski - .2015
 Pkg.pin("Winston", v"0.11.0")
Pkg.add("Winston")
module Interpolation
using Winston

export ilorazyRoznicowe
export warNewton
export rysujNnfx

function ilorazyRoznicowe(x::Vector{Float64}, f::Vector{Float64})
  fx = copy(f)
  for I = 2 : length(x)
    for J = length(x) : -1: I
      fx[J] = (fx[J] - fx[J-1])/(x[J] - x[J-I+1])
    end
  end
  return fx
end

x = Float64[3.0, 1.0, 5.0, 6.0]
f = Float64[1.0, -3.0, 2.0, 4.0]
fx = ilorazyRoznicowe(x, f)

println(ilorazyRoznicowe(x, f))

function warNewton (x::Vector{Float64}, fx::Vector{Float64}, t::Float64)
  nt = fx[length(fx)]
  for K = length(fx)-1:-1: 1
     nt = fx[K] + (t - x[K])*nt
  end
  return nt
end

warNewton(x, fx, 1.0)

function rysujNnfx(f,a::Float64,b::Float64,n::Int)
        if n <= 0
                error("Stopien wielomianu musi byc wiekszy niz 0")
        end
        h = (b-a)/n
        x = zeros(n)
        y = zeros(n)

        l = 2*((n+1)^2)

        for i in 1:n
                x[i] = a + (i-1)*h
                y[i] = f(x[i])
        end

        fx = ilorazyRoznicowe(x,y)

        interpolation = Array(Float64,l)
        h = (b-a)/l
        Xi = Array(Float64,l)
        Y1 = Array(Float64,l)
        for i in 1:length(Xi)
                Xi[i] = a + (i-1)*h
                Y1[i] = f(Xi[i])
                interpolation[i] = warNewton(x,fx,Xi[i])
        end

        plot(Xi,Y1,"r",Xi,interpolation,"g-")

end

rysujNnfx(z, 0.0, 8.0, 50)

end
