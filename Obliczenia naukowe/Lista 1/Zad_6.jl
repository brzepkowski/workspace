#Obliczenia naukowe - Lista 1
#Bartosz Rzepkowski - 18.10.2015r.
function f(x)
  float64(sqrt(x^2.0 + 1.0) - 1.0)
end

function g(x)
  float64(x^2.0 / (sqrt(x^2.0 + 1.0) + 1.0))
end

for i = 1.0 : 179.0
  x = f(float64(1.0/8.0^i))
  y = g(float64(1.0/8.0^i))
  println("i = ", i, " | f(x) = ", x, " | g(x) = ", y)
end
