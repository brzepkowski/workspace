#Obliczenia naukowe - Lista 5
#Bartosz Rzepkowski - 28.12.2015

module systems
export Gauss
export rozkladLU
export LUxb

function Gauss(A::Matrix{Float64}, b::Vector{Float64}, pivot::Bool)
  n = length(b)
  x = zeros(n)
  err = 0
  macheps = eps(1.0)

  #**************Bez wybou elementu głównego***********
  if pivot == false
      for k = 1: n-1
        # Sprawdzenie, czy element główny jest mniejszy niż macheps
        if abs(A[k, k]) < macheps
          err = 1
        println("Błąd!!!   A[",k,", ",k, "] = ", A[k,k])
        end
        #---------------------------------------------------------
        for i = k + 1: n
          I = A[i, k] / A[k, k]
          for j = k: n
            A[i, j] = A[i, j] - I*A[k, j]
          end
        b[i] = b[i] - I*b[k]
        end
      end
    #Sprawdzenie A[n, n], czy jest równe 0
    if abs(A[n, n]) < macheps
      err = 1
      println("Błąd!!!   A[",n,", ",n, "] = ", A[n,n])
    end
    #-------------------------------------
    #****Obliczenie ostatecznego wyniku*****
    for i = n: -1 : 1
      #Suma wcześniejszych x*a
      s = float64(0.0)
      for j = i+1: n
        s = s + A[i,j]*x[j]
      end
      #------------------------
      x[i] = (b[i] - s)/A[i,i]
    end
    return x, err

  #**********Wybór częściowy elementu głównego*********
  else
    for k = 1: n-1 # k - numer wiersza, dla ktorego bedziemy szukali wiersza zastępczego
      max = 0 # max będzie pamiętał numer wiersza, w którym jest największa wartość
      for i = k+1: n
        if abs(A[i, k]) > abs(A[k, k])
          max = i
        end
      end
      if max != 0
        #Zamiana wierszy
        for i = 1: n
          buffer = A[k, i]
          A[k, i] = A[max, i]
          A[max, i] = buffer
        end
        buffer = b[k]
        b[k] = b[max]
        b[max] = buffer
      end
      # Sprawdzenie, czy element główny jest mniejszy niż macheps
        if abs(A[k, k]) < macheps
          err = 1
        println("Błąd!!!   A[",k,", ",k, "] = ", A[k,k])
        end
        #---------------------------------------------------------
        for i = k + 1: n
          I = A[i, k] / A[k, k]
          for j = k: n
            A[i, j] = A[i, j] - I*A[k, j]
          end
        b[i] = b[i] - I*b[k]
        end
    end
    #Sprawdzenie A[n, n], czy jest równe 0
    if abs(A[n, n]) < macheps
      err = 1
      println("Błąd!!!   A[",n,", ",n, "] = ", A[n,n])
    end
    #-------------------------------------
    #***Obliczenie ostatecznego wyniku*****
    for i = n: -1 : 1
      #Suma wcześniejszych x*a
      s = float64(0.0)
      for j = i+1: n
        s = s + A[i,j]*x[j]
      end
      #------------------------
      x[i] = (b[i] - s)/A[i,i]
    end
    return x, err
  end
end


function rozkladLU(A::Matrix{Float64}, pivot::Bool)
  n = size(A, 1)
  lu = A   #lu jest tą samą tablicą co A (wejściowa tablica A zostanie przekształcona w tablicę "lu")
  err = 0
  macheps = eps(1.0)

  #Utworzenie tablicy ipvt i ustalenie jej elementów na 1, 2, ..., n
  ipvt = Array(Int, n)
  for i = 1: n
    ipvt[i] = i
  end

  #***************Bez wyboru elementu głównego**************
  if pivot == false
      for k = 1: n-1
        # Sprawdzenie, czy element główny jest mniejszy niż macheps
        if abs(lu[k, k]) < macheps
          err = 1
          println("Błąd!!!   A[",k,", ",k, "] = ", A[k,k])
        end
        #---------------------------------------------------------
        for i = k + 1: n
          I = lu[i, k] / lu[k, k]
          for j = k: n
            lu[i, j] = lu[i, j] - I*lu[k, j]
          end
        lu[i, k] = I
        end
      end
    #Sprawdzenie A[n, n], czy jest równe 0
    if abs(lu[n, n]) < macheps
      err = 1
      println("Błąd!!!   A[",n,", ",n, "] = ", A[n,n])
    end
    #-------------------------------------
    return lu, ipvt, err

  #************Wybór częściowy elementu głównego*************
  else
    for k = 1: n-1  # k - numer wiersza, dla ktorego bedziemy szukali wiersza zastępczego
      max = 0       # max będzie pamiętał numer wiersza, w którym jest największa wartość
      for i = k+1: n
        if abs(lu[i, k]) > abs(lu[k, k])
          max = i
        end
      end
      if max != 0
        #Zamiana wierszy
        for i = 1: n
          buffer = lu[k, i]
          lu[k, i] = lu[max, i]
          lu[max, i] = buffer
        end
        # Permutacje
          buffer = ipvt[max]
          ipvt[max] = ipvt[k]
          ipvt[k] = buffer
      end
      # Sprawdzenie, czy element główny jest mniejszy niż macheps
        if abs(lu[k, k]) < macheps
          err = 1
        println("Błąd!!!   A[",k,", ",k, "] = ", A[k,k])
        end
        #---------------------------------------------------------
        for i = k + 1: n
          I = lu[i, k] / lu[k, k]
          for j = k: n
            lu[i, j] = lu[i, j] - I*lu[k, j]
          end
        lu[i, k] = I
        end
    end
    #Sprawdzenie A[n, n], czy jest równe 0
    if abs(lu[n, n]) < macheps
      err = 1
      println("Błąd!!!   A[",n,", ",n, "] = ", A[n,n])
    end
    #-------------------------------------
    return lu, ipvt, err
  end
end


# Funkcja oblicza x z dwóch równań -> Ly = b oraz Ux = y (Ly = Pb w przypadku permutowanej macierzy).
# Wektory x oraz y nie będą tworzone, ponieważ wszystkie dane przechowywane będą w wektorze wejściowym b.
function LUxb(lu::Matrix{Float64}, pivot::Bool, b::Vector{Float64}, ipvt::Vector{Int})
  n = length(b)
  x = b # x jest tym samym wektorem co b.

  #************Bez częściowego wyboru************
  if pivot == false
    # Obliczanie y z równania Ly = b
    for k = 1 : n
      sum = 0.0
      for i = 1: k
        if i == k
          x[k] = x[k] - sum
        else
          sum = sum + lu[k, i]*x[i]
        end
      end
    end
    # -------------------------------
    #Obliczanie x z równania Ux = y
    for k = n : -1: 1
      sum = 0.0
      for i = n : -1: 1
        if i == k
          x[k] = (x[k] - sum)/lu[k, k]
        else
          sum = sum + lu[k, i]*x[i]
        end
      end
    end
    return x

  #*************Wybór częściowy elementu głównego****************
  else
    # Permutowanie wektora prawych stron - b.
    b2 = zeros(n)
    for i = 1: n
      if ipvt[i] != i
        b2[i] = b[ipvt[i]]
      else
        b2[i] = b[i]
      end
    end
    return LUxb(lu, false, b2, ipvt)
  end
end

A = [2.0 -2.0 0.0; -2.0 0.0 2.0; 0.0 -2.0 0.0]
b = [6.0, 0.0, -2.0]

Gauss(A, b, false)
Gauss(A, b, true)

(lu1, ipvt1, err1) = rozkladLU(A, false)
(lu2, ipvt2, err2) = rozkladLU(A, true)


LUxb(lu1, false, b, ipvt1)
LUxb(lu2, true, b, ipvt2)


end
