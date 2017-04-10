#Obliczenia naukowe - Lista 4
#Bartosz Rzepkowski - 6.12.2015

push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 4/Programy")
using Interpolation

# A)
function g(x)
  return e^x
end

Interpolation.rysujNnfx(g, 0.0, 1.0, 5)
Interpolation.rysujNnfx(g, 0.0, 1.0, 10)
Interpolation.rysujNnfx(g, 0.0, 1.0, 15)



# B)

function h(x)
  return (x^2)*sin(x)
end

Interpolation.rysujNnfx(h, -1.0, 1.0, 5)

Interpolation.rysujNnfx(h, -1.0, 1.0, 10)

Interpolation.rysujNnfx(h, -1.0, 1.0, 15)

