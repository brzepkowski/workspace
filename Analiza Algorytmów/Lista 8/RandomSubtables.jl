function randomSubtables(arr, calls)
  n = length(arr)
  # println(arr)
  calls = 1
  if (n >= 2)
    for k in 1:n
      if (rand() <= 0.5)
        subArr = []
        usedPointers = []
        while (length(usedPointers) < k)
          pointer = rand(collect(1:n))
          while (in(usedPointers, pointer))
            pointer = rand(collect(1:n))
          end
          push!(subArr, arr[pointer])
          push!(usedPointers, pointer)
        end # while
        println(subArr)
        calls += randomSubtables(subArr, calls)
      end
    end
  end
  return calls

end # randomSubtables

function meanNumberOfCalls(n::Int, experiments::Int)
  for k in 1:n
    meanCallsNumber = 0
    for i in 1:experiments
      meanCallsNumber += randomSubtables(collect(1:k), 0)
    end
    meanCallsNumber /= experiments

    println("Średnia liczba wywołań dla n = ", k, " wynosi: ", meanCallsNumber)
  end
end


meanNumberOfCalls(2, 1000)
