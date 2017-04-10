#Obliczenia naukowe - Lista 4
#Bartosz Rzepkowski - 6.12.2015

push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 4/Programy")
using Interpolation

# A)

function g(x)
  return abs(x)
end

Interpolation.rysujNnfx(g, -1.0, 1.0, 5)
Interpolation.rysujNnfx(g, -1.0, 1.0, 10)
Interpolation.rysujNnfx(g, -1.0, 1.0, 15)


# B)

function h(x)
  return 1 / (1 + x^2)
end

Interpolation.rysujNnfx(h, -5.0, 5.0, 5)
Interpolation.rysujNnfx(h, -5.0, 5.0, 10)
Interpolation.rysujNnfx(h, -5.0, 5.0, 15)

