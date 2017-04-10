#Obliczenia naukowe - Lista 3
#Bartosz Rzepkowski - 22.11.2015

push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 3")
using Metody

function f(x)
  return x^2 - 4
end

function pf(x)
  return 2*x
end

mbisekcji(f,-3.0, 1.5, 0.00001, 0.00001)
mbisekcji(f,-2.0, 1.5, 0.00001, 0.00001)
mbisekcji(f,1.0, 2.5, 0.00001, 0.00001)
mbisekcji(f,1.0, 1.5, 0.00001, 0.00001) #Error
mbisekcji(f,-3.0, 20.0, 0.00001, 0.00001) #Error

mstycznych(f, pf, -3.0, 0.00001, 0.00001, 1000000)
mstycznych(f, pf, 2.0, 0.00001, 0.00001, 1000000)
mstycznych(f, pf, 1.0, 0.00001, 0.00001, 1000000)
mstycznych(f, pf, 0.0, 0.00001, 0.00001, 1000000) #nothing

msiecznych(f, -3.0, 1.5, 0.00001, 0.00001)
msiecznych(f, 1.0, 1.5, 0.00001, 0.00001) #Error
msiecznych(f, -3.0, 20.0, 0.00001, 0.00001) #Error
msiecznych(f, 1.0, 3.0, 0.00001, 0.00001)

