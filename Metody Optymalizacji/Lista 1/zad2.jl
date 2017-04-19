# Bartosz Rzepkowski
# Metdoy optymalizacji - Lista 1
# Zadanie 2
using JuMP
using GLPKMathProgInterface

function generuj_wartosci()
  odleglosci = [0.0 42.7 55.1 50.5 34.2 56.3 81.1;
               42.7 0.0 53.2 72.4 77.0 98.7 125.3;
               55.1 53.2 0.0 28.7 86.0 79.0 89.3;
               50.5 72.4 28.7 0.0 62.2 52.6 62.4;
               34.2 77.0 86.0 62.2 0.0 22.0 59.4;
               56.3 98.7 79.0 52.6 22.0 0.0 35.7;
               81.1 125.3 89.3 62.4 59.4 35.7 0.0]

  dzwigi_nadmiar = [7 0;
                    0 1;
                    6 2;
                    0 10;
                    5 0;
                    0 0;
                    0 0]

  dzwigi_niedobor = [0 2;
                    10 0;
                    0 0;
                    4 0;
                    0 4;
                    8 2;
                    0 1]
  return (odleglosci, dzwigi_nadmiar, dzwigi_niedobor)
end

function dzwigi_model(odleglosci, dzwigi_nadmiar, dzwigi_niedobor, n)
  model = Model(solver = GLPKSolverLP()) # Stwórz model do rozwiązania

  @variable(model, transport[1:n, 1:n, 1:2] >= 0) # Dodaj do modelu zmienną będącą tablicą n x n x 2, w której będą przechowywane wiadomości o transportowanych dźwigach

  @objective(model, Min, sum(transport[i,j,1]*odleglosci[i,j] for i=1:n for j=1:n) + 1.2*sum(transport[i,j,2]*odleglosci[i,j] for i=1:n for j=1:n)) # Funkcja celu -> minimalizacja kosztów transportu

  #Nadmiar
  @constraint(model, [i=1:n, k=1:2], sum(transport[i, j, k] for j=1:n) == dzwigi_nadmiar[i,k]) # Liczba wysłanych z danego misata dźwigów musi być równa liczbie dźwigów nadmiarowych znajdujących się w nim
  #Niedobór
  @constraint(model, [i=1:n], sum(transport[j, i, 2] for j=1:n) >= dzwigi_niedobor[i,2]) # Liczba dźwigów typu 2 wysłanych do innych miast może być większa niż niedobór w danych miastach (bo można nimi zastępować też dźwigi typu 1)
  @constraint(model, [i=1:n], sum(transport[j, i, 1] for j=1:n) + sum(transport[j, i, 2] for j=1:n) == dzwigi_niedobor[i,2] + dzwigi_niedobor[i,1]) # Suma dźwigów odebranych przez miasta musi być równa niedoborom w tych miastach


  println("-------------MODEL----------------")
  println(model)
  println("----------------------------------")

  status = solve(model, suppress_warnings=true)
  transport2 = getvalue(transport)
  for k in 1:2
    for i in 1:n
      for j in 1:n
        print(transport2[i, j, k], ", ")
      end
      println()
    end
    println("--------------")
  end
end

(odleglosci, dzwigi_nadmiar, dzwigi_niedobor) = generuj_wartosci()
dzwigi_model(odleglosci, dzwigi_nadmiar, dzwigi_niedobor, 7)
