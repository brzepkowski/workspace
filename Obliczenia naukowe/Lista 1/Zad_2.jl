#Obliczenia naukowe - Lista 1
#Bartosz Rzepkowski - 18.10.2015r.

macheps = float16(3) * (float16(4)/float16(3) - float16(1)) - float16(1)
println("Macheps dla Float16 = ", macheps, " | eps(Float16) = ", eps(Float16))

macheps = float32(3) * (float32(4)/float32(3) - float32(1)) - float32(1)
println("Macheps dla Float32 = ", macheps, " | eps(Float32) = ", eps(Float32))

macheps = float64(3) * (float64(4)/float64(3) - float64(1)) - float64(1)
println("Macheps dla Float64 ", macheps, " | eps(Float64= ", eps(Float64))
