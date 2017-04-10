#Obliczenia naukowe - Lista 1
#Bartosz Rzepkowski - 18.10.2015r.
function derivative(f, x, h)
  float64((f(float64(x + h)) - f(x)) / h)
end

#f'(x) = sinx + cos3x dla x = 1 oraz h = 2^-n (n = 0, 1, 2, ..., 54)

n = float64(0.0)
while n <= 54.0
  h = float64(1.0 / (2.0^n))
  Result = float64(derivative(sin, float64(1), h) + derivative(cos, float64(3.0), h))
  println("~f'(x) = ", Result, " dla h = ", h, " | 1 + h = ", 1 + h, ", |f'(x) - ~f'(x)| = ", abs(0 - Result))
  n = n + 1.0
end
