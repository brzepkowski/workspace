#Obliczenia naukowe - Lista 4
#Bartosz Rzepkowski - 6.12.2015

using Interpolation

# ilorazyRoznicowe
x = Float64[3.0, 1.0, 5.0, 6.0]
f = Float64[1.0, -3.0, 2.0, 4.0]
fx = ilorazyRoznicowe(x, f)

x = Float64[3.0, 1.0, 5.0]
fx = ilorazyRoznicowe(x, f)

# warNewton
x = Float64[3.0, 1.0, 5.0, 6.0]
f = Float64[1.0, -3.0, 2.0, 4.0]
fx = ilorazyRoznicowe(x, f)
warNewton(x, fx, 1.0)


fx = [1.0, 2.0, -0.375]
warNewton(x, fx, 1.0)

# rysujNNfx
function z(x)
  return e^x
end

rysujNnfx(z, 0.0, 8.0, 5)

rysujNnfx(z, 0.0, 8.0, -5)
rysujNnfx(z, 0.0, 8.0, 0)
