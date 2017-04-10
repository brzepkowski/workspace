#Obliczenia naukowe - Lista 5
#Bartosz Rzepkowski - 28.12.2015

push!(LOAD_PATH, "/home/bartas/Julia/Lista\ 5/Programy")
using systems

#1)
A = [2.0 -2.0 0.0; -2.0 0.0 2.0; 0.0 -2.0 0.0]
b = [6.0, 4.0, 2.0]
#Rozwiazanie dokładne x
x = [2.0, -1.0, 4.0]

#Eliminacja Gaussa
x1 = Gauss(A, b, false)[1]
x2 = Gauss(A, b, true)[1]

#Rozwiązanie dwuetapowe
(lu1, ipvt1, err1) = rozkladLU(A, false)

(lu2, ipvt2, err2) = rozkladLU(A, true)

x3 = LUxb(lu1, false, b, ipvt1)
x4 = LUxb(lu2, true, b, ipvt2)

#********Błędy wzgledne macierzy*******
norm(x1-x)/norm(x)
norm(x2-x)/norm(x)
norm(x3-x)/norm(x)
norm(x4-x)/norm(x)

#2)
A = [0.0 2.0 -1.0 -2.0; 2.0 -2.0 4.0 -1.0;
     1.0 1.0 1.0 1.0; -2.0 1.0 -2.0 1.0]

b = [-7.0, 6.0, 10.0, -2.0]

#Rozwiązanie dokładne
x = [1.0, 2.0, 3.0, 4.0]

#Eliminacja Gaussa
x1 = Gauss(A, b, false)[1]
x2 = Gauss(A, b, true)[1]

#Rozwiązanie dwuetapowe
(lu1, ipvt1, err1) = rozkladLU(A, false)
(lu2, ipvt2, err2) = rozkladLU(A, true)

x3 = LUxb(lu1, false, b, ipvt1)
x4 = LUxb(lu2, true, b, ipvt2)

#********Błędy wzgledne macierzy*******
norm(x1-x)/norm(x)
norm(x2-x)/norm(x)
norm(x3-x)/norm(x)
norm(x4-x)/norm(x)

#3)
A = [2.0 -2.0 0.0; -2.0 0.0 2.0; 0.0 -2.0 0.0]
b = [6.0, 0.0, -2.0]

#Rozwiązanie dokładne
x = [4.0, 1.0, 4.0]

#Eliminacja Gaussa
x1 = Gauss(A, b, false)[1]
x2 = Gauss(A, b, true)[1]

#Rozwiąznie dwuetapowe
(lu1, ipvt1, err1) = rozkladLU(A, false)
(lu2, ipvt2, err2) = rozkladLU(A, true)

x3 = LUxb(lu1, false, b, ipvt1)
x4 = LUxb(lu2, true, b, ipvt2)

#********Błędy wzgledne macierzy*******
norm(x1-x)/norm(x)
norm(x2-x)/norm(x)
norm(x3-x)/norm(x)
norm(x4-x)/norm(x)

