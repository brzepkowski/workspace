#Obliczenia naukowe - Lista 1
#Bartosz Rzepkowski - 18.10.2015r.

#Float16
eta = float16(1.0)
while eta > float16(0.0)
  if eta / float16(2.0) == float16(0.0)
    break
  else
    eta = eta / float16(2)
  end
end
println("Eta(Float16) = ", eta, " nextfloat(float16(0.0)) = ", nextfloat(float16(0.0)))

#Float32

eta = float32(1.0)
while eta > float32(0.0)
  if eta / float32(2.0) == float32(0.0)
    break
  else
    eta = eta / float32(2)
  end
end
eta
nextfloat(float32(0.0))

#Without writing "float32" everywhere

eta = float32(1.0)
while eta > 0
  if eta / 2 == 0
    break
  else
    eta = eta / 2
  end
end
println("Eta(Float32) = ", eta, " nextfloat(float32(0.0)) = ", nextfloat(float32(0.0)))


#Float64

eta = float64(1.0)
while eta > 0
  if eta / 2 == 0
    break
  else
    eta = eta / 2
  end
end
println("Eta(Float64) = ", eta, " nextfloat(float64(0.0)) = ", nextfloat(float64(0.0)))


