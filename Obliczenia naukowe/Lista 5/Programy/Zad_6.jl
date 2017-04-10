#Obliczenia naukowe - Lista 5
#Bartosz Rzepkowski - 28.12.2015

push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 5/Programy")
using systems

A = [3282825675.08941 -5013081565.65267 3409304728.02911;
     3256050991.27407 439858221.670267 -3005859117.97034;
     -5931951819.47511 4642259422.30978 -948447572.032458]
b = [3231618621.992, 7642010299.1924, -9459784185.83823]
#Rozwiązanie dokladne
x = [3.0, 2.0, 1.0]

#Eliminacja Gausa
x1 = Gauss(A, b, false)[1]
x2 = Gauss(A, b, true)[1]

println(x2)
#Rozwiązanie dwuetapowe
(lu1, ipvt1, err1) = rozkladLU(A, false)
(lu2, ipvt2, err2) = rozkladLU(A, true)

x3 = LUxb(lu1, false, b, ipvt1)
x4 = LUxb(lu2, true, b, ipvt2)

 #********Błędy wzgledne macierzy*******
  println("x1 = ", norm(x1-x)/norm(x))
  println("x2 = ", norm(x2-x)/norm(x))
  println("x3 = ", norm(x3-x)/norm(x))
  println("x4 = ", norm(x4-x)/norm(x))

