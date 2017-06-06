typealias Comes Val{:Comes}
typealias Leaves Val{:Leaves}

typealias Actions Union{
    Type{Comes},
    Type{Leaves}
}

type Event
  clientNumber
  action
  time
end

function continuousTimeQueue()
  e1 = Event(1, Comes, 12)
  e2 = Event(2, Comes, 13)
  e3 = Event(3, Comes, 2)
  e4 = Event(4, Comes, 15)
  e5 = Event(5, Comes, 9)
  events = [e1, e2, e3, e4, e5]
  el = [6,21,2,1,4,5]
  # println(el)
  # Collections.heapify!(el)
  # println(el)
  # Collections.heappop!(el)
  # println(el)
  # Collections.heappop!(el)
  # println(el)
  # Collections.heappop!(el)
  # println(el)
  # Collections.heappop!(el)
  # println(el)
  # Collections.heappop!(el)
  # println(el)

  println(events)
  Collections.heapify!(events)
  println(events)
end # continuousTimeQueue

continuousTimeQueue()
