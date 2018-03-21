def simplex1D(f, startingPoint, h, ϵ):
    xₗ = startingPoint
    while h > ϵ:
        xᵣ = xₗ + h
        if f(xₗ) > f(xᵣ):
            xₗ = xᵣ
        else:
            h = h / 2
    return xₗ

def polynomial(x):
    return x**2 + 3*x - 4

min0 = simplex1D(polynomial, -10, 2, 0.1)
print(min0)
