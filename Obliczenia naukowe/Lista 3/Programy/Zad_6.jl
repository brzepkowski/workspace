#Obliczenia naukowe - Lista 3
#Bartosz Rzepkowski - 22.11.2015

push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 3")
using Metody

function f(x)
  return (e^(1-x)) - 1
end

function pf(x)
  return -e^(1-x)
end

function g(x)
  return x * (e^(-x))
end

function gf(x)
  return (-e^(-x)) * (x-1)
end

#f(x) - Metoda bisekcji
println(mbisekcji(f, 0.0, 3.0, 0.00001, 0.00001))
println(mbisekcji(f, -200.0, 300.0, 0.00001, 0.00001))
#g(x) - Metoda bisekcji
println(mbisekcji(g, -2.0, 3.0, 0.00001, 0.00001))
println(mbisekcji(g, -20.0, 30.0, 0.00001, 0.00001))

#f(x) - Metoda Newtona
println(mstycznych(f, pf, 0.0, 0.00001, 0.00001, 1000000))
println(mstycznych(f, pf, -10.0, 0.00001, 0.00001, 1000000))
#g(x) - Metoda Newtona
println(mstycznych(g, gf, -1.0, 0.00001, 0.00001, 1000000))
println(mstycznych(g, gf, 1.0, 0.00001, 0.00001, 1000000))
println(mstycznych(g, gf, -20.0, 0.00001, 0.00001, 1000000))

#f(x) - Metoda siecznych
println(msiecznych(f, 0.0, 2.0, 0.00001, 0.00001))
println(msiecznych(f, -10.0, 10.0, 0.00001, 0.00001))
#g(x) - Metoda siecznych
println(msiecznych(g, -1.0, 1.0, 0.00001, 0.00001))
println(msiecznych(g, -1.0, 2.0, 0.00001, 0.00001))


