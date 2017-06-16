function queue(λ, μ, T)
  queue = Array{Array{Int}}(1)
  queue[1] = Int64[]

  index = 1

  for t in 1:T
    λˈ = rand()
    μˈ = rand()

    if λˈ < λ
      index += 1
      if index > length(queue)
        push!(queue, Int64[])
      else
        # queue[index][1] += 1
      end
      append!(queue[index], t)
    end
    if μˈ < μ
      if index > 1
        index -= 1
      end
      # queue[index][1] += 1
      push!(queue[index], t)
    end
  end
  println("Długość kolejki: ", length(queue))
  printStats(queue)

end # queue

function printStats(M) # M is array of arrays with all data. Each cell in sub-array contains the number of step in which this state "was active"
  for (i, state) in enumerate(M)
    print(i, ") Czas: ", length(state))
    steps = Int64[]
    average = 0
    if length(state) > 1
      for j in 2:length(state)
        push!(steps, state[j] - state[j-1])
      end
      for stepLength in steps
        average += stepLength
      end
      average = average / length(steps)
    end
    # println(", Długość kroku: ", state, " -> ", steps, " -> ", average)
    println(", Średnia długość kroku: ", average)
  end
end

queue(0.3, 0.4, 100000)
