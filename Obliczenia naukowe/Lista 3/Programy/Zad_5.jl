#Obliczenia naukowe - Lista 3
#Bartosz Rzepkowski - 22.11.2015

push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 3")
using Metody

# 3x = y
# e^x = y
# 3x = e^x
#3x - e^x = 0

function f(x)
  return 3*x - (e^x)
end

println(mbisekcji(f, 0.0, 1.0, 0.00001, 0.00001))

println(mbisekcji(f, 1.0, 2.0, 0.00001, 0.00001))

