# Bartosz Rzepkowski
# Metdoy optymalizacji - Lista 2
# Zadanie 1
using JuMP
using GLPKMathProgInterface

# Funkcja generuje wszystkie możliwe podziały deski o standardowej szerokości, a dodatkowo zwraca liczbę możliwych podziałów deski. Wykorzystuje zmienne:
# widths - tablica zawierająca możliwe do wykorzystania szerkości desek
# index - indeks z tablicy widths (mówi o tym, którą szerokość aktualnie rozpatrujemy)
# maxWidth - standardowa szerokość deski
# memory - aktualnie "zapamiętywana" kombinacja podziału deski
# allSubSets - tablica, w której będą przechowywane wszystkie możliwe podziały standardowej deski
function subSets(widths::Array{Int64}, index::Int64, maxWidth::Int64, memory::Array{Int64}, allSubSets::Set{Array{Int64}})

    # Jeśli sprawdzana kombinacja nie przekroczyła szerokością standardowej szerokości deski i nie ma jej jescze w zbiorze wszystkich możliwych kombinacji, to dodaj ją do tego zbioru
    if (maxWidth >= 0)
      if !in(memory, allSubSets)
        push!(allSubSets, memory)
      end
    end

    # Jeśli maxWidth == 0, istnieje rozwiązanie
    if maxWidth == 0
      return 1
    end

    # Jeśli maxWidth < 0, to nie istnieje możliwe rozwiązanie
    if maxWidth < 0
      return 0
    end

    # Jeśli nie mamy już do wykorzystania żadnych podziałów, a maxWidth > 0 nie istnieje już żadne rozwiązanie
    if index <= 0 && maxWidth >= 0
      return 0
    end

    # Zwróć ilość możliwych podziałów 1) bez uwzględniania aktualnie rozpatrywanej szerokości deski + 2) z jej uwzględnieniem
    return subSets(widths, index - 1, maxWidth, copy(memory), allSubSets) + subSets(widths, index, maxWidth - widths[index], append!(copy(memory), widths[index]), allSubSets)
end

# Funkcja zwraca wszystkie możliwe podziały deski o standardowej szerokości dla samych zmiennych:
# widths - tablica zawierająca możliwe do wykorzystania szerkości desek
# maxWidth - standardowa szerokość deski
function generateSubSets(widths::Array{Int64}, maxWidth::Int64)
  allSubSets = Set{Array{Int64}}()
  subSets(widths, length(widths), maxWidth, Int64[], allSubSets)
  return allSubSets
end

# Funkcja generuje rozwiązanie zadania produkcji desek przy minimalizacji odpadów. Zmienne:
# order - tablica tablic, mająca postać [[szerokość_deski, ilość_desek_do_wyprodukowania], ...]
# maxWidth - standardowa szerokość deski
function generateSolution(order::Array{Array{Int64, 1}, 1}, maxWidth::Int64)

  widths = [x[1] for x in order] # Odczytaj z tablicy order pierwszy element pary, żeby mieć do dyspozycji wzsystkie możliwe szerkości desek
  totalBoardsAmounts = [[x[1], 0] for x in order]
  allSubSets = generateSubSets(widths, maxWidth) # Wygeneruj wszystkie możliwe podziały deski o standardowej szerokości
  println(allSubSets)

  n = length(allSubSets) # n = liczba wszystkich możliwych podziałów standardowej deski

  leftovers = zeros(n) # tablica z pozostałościami. Początkowo dla każdej kombinacji jest równa standardowej szerokości deski
  for i in 1:n
    leftovers[i] = maxWidth
  end

  for (index, subSet) in enumerate(allSubSets) # Wygeneruj ostateczne wartości pozostałości z początkowej deski poprzez odjęcie wszystkich szerokości z rozpatrywanej kombinacji
    for element in subSet
      leftovers[index] -= element
    end
  end

  model = Model(solver = GLPKSolverMIP())
  @variable(model, boards[1:n] >= 0) # Zmienne boards są powiązane z tablicą allSubSets. Jeśli np. boards[1] == 2 oznacza to, że należy rozciąć 2 deski o szerkości maxWidth według kombinacji widths[1]

  @objective(model, Min, sum(boards[i]*leftovers[i] for i=1:n)) # Funkcja celu - minimalizowanie pozostałości z rozciętych desek

  # Ograniczenie dla każdego zamówienia - suma wyprodukowanych desek o zadanych długościach nie może być mniejsza od podanych w zamówieniu
  # np. dla pary tablicy order = [[7, 110], ...] musi być w sumie we wszystkich wykorzystanych kombinacjach 110 desek o szerokości 7
  @constraint(model, [j=1:length(order)], sum(boards[i] for (i, subSet) in enumerate(allSubSets), element in subSet if element == order[j][1]) >= order[j][2])

  status = solve(model, suppress_warnings=true)
  solution = getvalue(boards)

  index = 1
  for i in allSubSets
    if solution[index] != 0
      println(i, " -> ", solution[index])
    end
    index += 1
  end
end

generateSolution([[7, 110], [5, 120], [3, 80]], 22)
