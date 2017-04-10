#Obliczenia naukowe - Lista 1
#Bartosz Rzepkowski - 18.10.2015r.

#A)
x = nextfloat(float64(1.0))
eta = nextfloat(float64(1.0)) - float64(1.0)

while float64(x * float64(1/x)) == 1
  x = x + eta
end
bits(x)
#x = 1.000000057228997
#bits(x) = "0011111111110000000000000000000000001111010111001011111100101010"

#B)
x = realmin(Float64)
while float64(x * float64(1/x)) == 1
  x = x + (nextfloat(x) - x)
end
println("x = ", x)
#x = x = 2.225073985845947e-308 Najmniejsza dodatnia

x = nextfloat(-Inf)
while float64(x * float64(1/x)) == 1
  x = x + float64(nextfloat(x) - x)
end
println("x = ", x)

# x = -1.7976931348623157e308 Najmniejsza ujemna
