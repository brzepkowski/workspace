#Obliczenia naukowe - Lista 1
#Bartosz Rzepkowski - 18.10.2015r.

#Float16
max = float16(2.0)

while isinf(float16(max)) == false
  if isinf(float16(max * 2)) == true
     break
  else
    max = max * 2
  end
end

a = float16(max / 2)
eta = eps(Float16)

while isinf(float16(max)) == false
  if isinf(float16(max + a)) == false
    max = max + a
    a = a / 2
  elseif isinf(float16(max + a)) == true
    break
  end
end

println("Max = ", max, " realmax(Float16) = ", realmax(Float16))
max == realmax(Float16)

#Float32
max = float32(2.0)

while isinf(float32(max)) == false
  if isinf(float32(max * 2)) == true
     break
  else
    max = max * 2
  end
end

a = float32(max / 2)
eta = eps(Float32)

while isinf(float32(max)) == false
  if isinf(float32(max + a)) == false
    max = max + a
    a = a / 2
  elseif isinf(float32(max + a)) == true
    break
  end
end

println("Max = ", max, " realmax(Float32) = ", realmax(Float32))
max == realmax(Float32)

#Float64
max = float64(2.0)

while isinf(float64(max)) == false
  if isinf(float64(max * 2)) == true
     break
  else
    max = max * 2
  end
end

a = float64(max / 2)
eta = eps(Float64)

while isinf(float64(max)) == false
  if isinf(float64(max + a)) == false
    max = max + a
    a = a / 2
  elseif isinf(float64(max + a)) == true
    break
  end
end

println("Max = ", max, " realmax(Float64) = ", realmax(Float64))
max == realmax(Float64)
