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
  h = (b-a)/n
  x = zeros(n+1)
  y = zeros(n+1)

  for k = 0: n
    x[k+1] = a + (k*h)
    y[k+1] = f(x[k+1])
  end

  fx = ilorazyRoznicowe(x, y)
  interpolation = zeros(n+1)

  for i = 1: n+1
    interpolation[i] = warNewton(x, fx, x[i])
  end
  plot(x, interpolation)
end

function z(x)
  return e^x
end

rysujNnfx(z, 0.0, 8.0, 50)

end
