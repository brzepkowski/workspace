#Obliczenia naukowe - Lista 2
#Bartosz Rzepkowski - 8.11.2015r.
using Polynomials
# A)
P = Poly([2432902008176640000.0, -8752948036761600000.0, 13803759753640704000.0, -12870931245150988800.0,
 8037811822645051776.0, -3599979517947607200.0, 1206647803780373360.0, -311333643161390640.0,
 63030812099294896.0, -10142299865511450.0, 1307535010540395.0, -135585182899530.0,
 11310276995381.0, -756111184500.0, 40171771630.0, -1672280820.0, 53327946.0,
 -1256850.0, 20615.0, -210.0, 1])
P_roots = roots(P)

# Generating Wilkinson's Polynomial from it's roots (using function poly())
p = poly([1.0:20.0]);
Vector = zeros(4)
for k = 1.0: 20.0
  Vector = [P_roots[k], abs(polyval(P, P_roots[k])), abs(polyval(p, P_roots[k])), abs(P_roots[k] - k)]
  println(Vector[1]," ", Vector[2]," ",Vector[3]," ",Vector[4]," ",)
  # println("|P(z", k, ")| = ", abs(polyval(P, P_roots[k])), " |p(z", k, ")| = ",
    #        abs(polyval(p, P_roots[k])), " |z", k, " - k| = ", abs(P_roots[k] - k))
  end

# B)
P = Poly([2432902008176640000.0, -8752948036761600000.0, 13803759753640704000.0, -12870931245150988800.0,
 8037811822645051776.0, -3599979517947607200.0, 1206647803780373360.0, -311333643161390640.0,
 63030812099294896.0, -10142299865511450.0, 1307535010540395.0, -135585182899530.0,
 11310276995381.0, -756111184500.0, 40171771630.0, -1672280820.0, 53327946.0,
 -1256850.0, 20615.0, -210.0-(1/2^(23)), 1])
P_roots = roots(P)

# Generating Wilkinson's Polynomial from it's roots (using function poly())
p = poly([1.0:20.0]);
Vector = zeros(4)
for k = 1.0: 20.0
   Vector = [P_roots[k], abs(polyval(P, P_roots[k])), abs(polyval(p, P_roots[k])), abs(P_roots[k] - k)]
    println(Vector[1]," , ", Vector[2]," , ",Vector[3]," , ",Vector[4])
    #println("|P(z", k, ")| = ", abs(polyval(P, P_roots[k])), " |p(z", k, ")| = ",
     #       abs(polyval(p, P_roots[k])), " |z", k, " - k| = ", abs(P_roots[k] - k))
  end

