import numpy as np

# f - function, xₗ - left border, xᵣ - right border, ϵ - precission
def bisec(f, xₗ, xᵣ, ϵ):
    xₘ = xₗ + (xᵣ - xₗ)/2
    result = xₘ
    if xᵣ - xₗ <= ϵ:
        return xₘ
    if f(xₗ) * f(xₘ) < 0:
        result = bisec(f, xₗ, xₘ, ϵ)
    elif f(xᵣ) * f(xₘ) < 0:
        result = bisec(f, xₘ, xᵣ, ϵ)
    return result

def polynomial(x):
    return x**2 + 3*x - 4

zero1 = bisec(polynomial, -6, 0, 10**(-3))
zero2 = bisec(np.sin, 0.0, 2*np.pi, 10**(-15))
print(zero1)
print(zero2)
