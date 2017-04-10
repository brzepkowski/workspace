#Obliczenia naukowe - Lista 1
#Bartosz Rzepkowski - 18.10.2015r.
macheps = float16(1.0)

while float16(1.0) + float16(macheps/2) > float16(1.0)
  macheps = macheps / float16(2.0)
end

println("Macheps dla Float16 = " ,macheps, " | eps(Float16) = " , eps(Float16))

macheps = float32(1.0)

while float32(1.0) + float32(macheps/2) > float32(1.0)
  macheps = macheps / float32(2.0)
end

println("Macheps dla Float32 = " ,macheps, " | eps(Float32) = " , eps(Float32))


macheps = float64(1.0)

while float64(1.0) + float64(macheps/2) > float64(1.0)
  macheps = macheps / float64(2.0)
end

println("Macheps dla Float64 = " ,macheps, " | eps(Float64) = " , eps(Float64))

