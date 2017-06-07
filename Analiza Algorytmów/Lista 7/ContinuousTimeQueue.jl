using Distributions
typealias Comes Val{:Comes}
typealias Leaves Val{:Leaves}

typealias Actions Union{
    Type{Comes},
    Type{Leaves}
}

type Event
  queueId
  clientId
  action
  time
end

Base.isless(a::Event,b::Event) = isless(a.time,b.time)

function continuousTimeQueue(λ, μ, n, T)
  G = Exponential(1/λ)
  H = Exponential(1/μ)
  queues = []
  priorityQueue = [] # Needed to keep track of consecutive events
  clientId = 1
  timesInqueues = []
  clients = [] # Array containing pairs (clientId, beginningTime), which will be necessary to compute average times spent in queues
  timesSpent = []

  # Initiate empty arrays representing all n queues
  for i in 1:n
    push!(queues, [])
  end

  # Compute beginning times of arrivals of clients to the queues
  for k in 1:n
    Collections.heappush!(priorityQueue, Event(k, clientId, Comes, rand(G)))
    clientId += 1
  end

  t = 0
  while t < T
      event = Collections.heappop!(priorityQueue)
      queueId = event.queueId
      t = event.time
      # println("t: ", t)
      # println("1): ", priorityQueue)

      if (t < T)
        if (event.action == Comes)
          newClient = Event(queueId, clientId, Comes, t + rand(G)) # Calculate time of arrival of new client in queue k
          clientId += 1
          Collections.heappush!(priorityQueue, newClient) # Add time of arrival of new client to queue k to the priority queue

          if (queues[queueId] == []) # Add time, when client will be serviced to the priority queue
            clientServiced = Event(queueId, event.clientId, Leaves, t + rand(H))
            Collections.heappush!(priorityQueue, clientServiced)
          else
            push!(queues[queueId], event)
          end

          push!(clients, (event.clientId, event.time))
        else # client Leaves
          if (queues[queueId] != [])
            nextClient = Collections.heappop!(queues[queueId])
            nextClientLeaves = Event(queueId, nextClient.clientId, Leaves, t + rand(H))
          end

          beginningTime = filter(pair -> pair[1] == event.clientId, clients)[1][2]
          # println("Znalazłem!!!: ", beginningTime)
          push!(timesSpent, event.time - beginningTime)
        end
      end
  end

  # println(priorityQueue)

  averageTime = 0
  for time in timesSpent
    averageTime += time
  end
  averageTime = averageTime / length(timesSpent)
  # println(timesSpent)
  println("Średni czas w kolejce: ", averageTime)

end # continuousTimeQueue

# λ, μ, n, T
continuousTimeQueue(0.5, 1.0, 100, 100) # Średni czas: 0.9909238451701395
continuousTimeQueue(0.8, 1.0, 100, 100) # Średni czas: 0.9909238451701395
continuousTimeQueue(0.9, 1.0, 100, 100) # Średni czas: 0.9909238451701395
continuousTimeQueue(0.99, 1.0, 100, 100) # Średni czas: 0.9909238451701395
