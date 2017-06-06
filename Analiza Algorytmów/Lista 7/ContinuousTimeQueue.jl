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

function continuousTimeQueue(λ, μ, n)
  G = Exponential(1/λ)
  H = Exponential(1/μ)
  queues = []
  priorityQueue = [] # Needed to keep track of consecutive events
  clientId = 1
  timesInqueues = []

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
  while t < 5
      event = Collections.heappop!(priorityQueue)
      queueId = event.queueId
      t = event.time
      println("t: ", t)
      println("1): ", priorityQueue)

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
      else # client Leaves
        if (queues[queueId] != [])
          nextClient = Collections.heappop!(queues[queueId])
          nextClientLeaves = Event(queueId, nextClient.clientId, Leaves, t + rand(H))
        end
      end
      println("2): ", priorityQueue)
      sleep(2)
  end

  println(priorityQueue)



  # Collections.heappush!(events, e1)

end # continuousTimeQueue

continuousTimeQueue(0.5, 1.0, 5)
