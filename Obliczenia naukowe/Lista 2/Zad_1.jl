#Obliczenia naukowe - Lista 2
#Bartosz Rzepkowski - 8.11.2015r.
x = float64([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float64([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

n = 5
Arr = zeros(Float64, n)
#*************Float64**************
#A)
S = float64(0.0)
x = float64([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float64([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

for i = 1.0 : n
    S = S + float64(x[i]*y[i])
  println(float64(x[i]*y[i]))
end
println("W przód: S = ", S)

#B)
S = float64(0.0)
x = float64([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float64([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

i = n
while i != 0
    S = S + float64(x[i]*y[i])
    println(float64(x[i]*y[i]))
    i = i - 1
end
println("W tył: S = ", S)

#C)
S = float64(0.0)
x = float64([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float64([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

for i = 1.0 : n
  Arr[i] = float64(x[i]*y[i])
end

sort!(Arr)
println(Arr)

Positive = float64(0.0)
Negative = float64(0.0)
i = n

while i != 0
  if Arr[i] > 0
    Positive = float64(Positive + Arr[i])
  end
  i = i - 1.0
end
#println("Positive = ", Positive)

for i = 1 : n
  if Arr[i] < 0
    Negative = float64(Negative + Arr[i])
  end
end
#println("Negative = ", Negative)

Final = float64(Positive + Negative)
println("Final = ", Final)

#D)
S = float64(0.0)
x = float64([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float64([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

PosArr = zeros(Float64, n)
NegArr = zeros(Float64, n)
PosIt = 1.0 #- Iterator of array PosArr
NegIt = 1.0 #- Iterator of array NegArr


for i = 1.0 : n
  if float64(x[i]*y[i]) > 0
    PosArr[PosIt] = float64(x[i]*y[i])
    PosIt = PosIt + 1.0
  elseif float64(x[i]*y[i]) < 0
    NegArr[NegIt] = float64(x[i]*y[i])
    NegIt = NegIt + 1.0
  end
end

sort!(PosArr)
sort!(NegArr)
println(PosArr)
println(NegArr)

Positive = float64(0.0)
Negative = float64(0.0)
i = n
while i != 0
  Negative = float64(Negative + NegArr[i])
  i = i - 1
end
#println("Negative = ", Negative)

for i = 1 : n
    Positive = float64(Positive + PosArr[i])
end
#println("Positive = ", Positive)

Final = float64(Positive + Negative)
println("Final = ", Final)

#***************Float32*************************

x = float32([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float32([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

n = 5
Arr = zeros(Float32, n)
#*************Float32**************
println("Float32")
#A)
S = float32(0.0)
x = float32([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float32([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

for i = 1.0 : n
    S = S + float32(x[i]*y[i])
  println(float32(x[i]*y[i]))
end
println("W przód: S = ", S)

#B)
S = float32(0.0)
x = float32([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float32([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

i = n
while i != 0
    S = S + float32(x[i]*y[i])
    println(float32(x[i]*y[i]))
    i = i - 1
end
println("W tył: S = ", S)

#C)
S = float32(0.0)
x = float32([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float32([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

for i = 1.0 : n
  Arr[i] = float32(x[i]*y[i])
end

sort!(Arr)
println(Arr)

Positive = float32(0.0)
Negative = float32(0.0)
i = n

while i != 0
  if Arr[i] > 0
    Positive = float32(Positive + Arr[i])
  end
  i = i - 1.0
end
#println("Positive = ", Positive)

for i = 1 : n
  if Arr[i] < 0
    Negative = float32(Negative + Arr[i])
  end
end
#println("Negative = ", Negative)

Final = float32(Positive + Negative)
println("Final = ", Final)

#D)
S = float32(0.0)
x = float32([2.718281828, -3.141592654, 1.414213562, 0.577215664, 0.301029995])
y = float32([1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049])

PosArr = zeros(Float32, n)
NegArr = zeros(Float32, n)
PosIt = 1.0 #- Iterator of array PosArr
NegIt = 1.0 #- Iterator of array NegArr


for i = 1.0 : n
  if float32(x[i]*y[i]) > 0
    PosArr[PosIt] = float32(x[i]*y[i])
    PosIt = PosIt + 1.0
  elseif float32(x[i]*y[i]) < 0
    NegArr[NegIt] = float32(x[i]*y[i])
    NegIt = NegIt + 1.0
  end
end

sort!(PosArr)
sort!(NegArr)
println(PosArr)
println(NegArr)

Positive = float32(0.0)
Negative = float32(0.0)
i = n
while i != 0
  Negative = float32(Negative + NegArr[i])
  i = i - 1
end
#println("Negative = ", Negative)

for i = 1 : n
    Positive = float32(Positive + PosArr[i])
end
#println("Positive = ", Positive)

Final = float32(Positive + Negative)
println("Final = ", Final)
