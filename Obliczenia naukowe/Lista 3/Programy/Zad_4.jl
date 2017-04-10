#Obliczenia naukowe - Lista 3
#Bartosz Rzepkowski - 22.11.2015

push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 3/Programy")
using Metody

function f(x)
  sin(x) - (0.5 * x)^2
end

function pf(x)
  cos(x) - x
end

println(mbisekcji(f, 1.5, 2.0, 0.000005, 0.000005))

println(mstycznych(f, pf, 1.5, 0.000005, 0.000005, 1000000))

println(msiecznych(f, 1.0, 2.0, 0.000005, 0.000005))

