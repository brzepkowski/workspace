#Obliczenia naukowe - Lista 1
#Bartosz Rzepkowski - 18.10.2015r.

#A) - Przedział [1,2]
a = float64(1.0)
b = float64(2.0)

delta = float64(nextfloat(a) - a)

k = float64(1.0)
x = float64(1.0)

while x <= float64(2.0)
  x = float64(x + k * delta)
  if x - prevfloat(x) != delta
    println("Delta różna od ", delta, " dla x = ", x, " => delta' = ", x - prevfloat(x))
  end

  #Zwiekszenie k
    k = float64(k + 1.0)
end

#Rozmieszczone równomiernie co 2^-52

#B) - Przedział [1/2, 1]
a = float64(1/2)
b = float64(1.0)

delta = nextfloat(a) - a

#Rozmieszczenie początkowe co 2^-53

k = float64(1.0)
x = float64(0.5)

while x <= float64(1.0)
  x = float64(x + k * delta)
  if x - prevfloat(x) != delta
    println("Delta różna od ", delta, " dla x = ", x, " => delta' = ", x - prevfloat(x))
  end

  #Zwiekszenie k
    k = float64(k + 1.0)
end

#Rozmieszczone równomiernie co 2^-53

#C) - Przedział [2,4]
a = float64(2.0)
b = float64(4.0)

delta = float64(nextfloat(a) - a)

#Rozmieszczone początkowe co 2^-51

k = float64(1.0)
x = float64(2.0)

while x <= float64(4.0)
  x = float64(x + k * delta)
  if x - prevfloat(x) != delta
    println("Delta różna od ", delta, " dla x = ", x, " => delta' = ", x - prevfloat(x))
  end

  #Zwiekszenie k
    k = float64(k + 1.0)
end
