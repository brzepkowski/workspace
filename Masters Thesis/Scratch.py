# serached_elem is decimal representation of element, that we are searching for
# def grover(circuit, qr, n, searched_elem):
#     bin_searched_elem = list(bin(searched_elem))[2:n+2]
#     iterations = (2**n)**(1/2) # √N = √2ⁿ
#     for i in range(n):
#         circuit.h(qr[i])
#     for it in range(iterations):
#         for (i, elem) in enumerate(reversed(bin_searched_elem)):
#             print(i, " -> ", elem)
#             if elem == '1':
#                 print("Git")
#             else:
#                 print("Nie git

SS = [1 0 0 0;
      0 im 0 0;
      0 0 im 0;
      0 0 0 -1]

IH = [1 1 0 0;
      1 -1 0 0;
      0 0 1 1;
      0 0 1 -1]

# --- CNOT ---
CNOT = [1 0 0 0;
        0 1 0 0;
        0 0 0 1;
        0 0 1 0]

IS = [1 0 0 0;
      0 im 0 0;
      0 0 1 0;
      0 0 0 im]

Hˈ = [1 1 0 0;
      1 -1 0 0;
      0 0 1 1;
      0 0 1 -1]

CZ = [1 0 0 0; 0 1 0 0; 0 0 1 0; 0 0 0 -1]
Xˈ = [0 0 0 1; 0 0 1 0; 0 1 0 0; 1 0 0 0]
HH = [1 1 1 1; 1 -1 1 -1; 1 1 -1 -1; 1 -1 -1 1]
M = [1 0 0 0; 0 -1 0 0; 0 0 -1 0; 0 0 0 -1]
SI = [1 0 0 0; 0 1 0 0; 0 0 im 0; 0 0 0 im]
SS = [1 0 0 0; 0 e^(im*pi/2) 0 0; 0 0 e^(im*pi/2) 0; 0 0 0 e^(im*pi)]
IS = [1 0 0 0; 0 e^(im*pi/2) 0 0; 0 0 1 0; 0 0 0 e^(im*pi/2)]
SI = [1 0 0 0; 0 1 0 0; 0 0 e^(im*pi/2) 0; 0 0 0 e^(im*pi/2)]

# A = 00
# print(SS, '\n','\n')
# print(SS*IH, '\n','\n')
# print(SS*IH*CNOT, '\n','\n')
# print(SS*IH*CNOT*IH, '\n','\n')
# print(SS*IH*CNOT*IH*SS, '\n','\n')

# A = 01
# print(U₀*U₃*U₄*U₃*U₀)

# print(Hˈ * U₄ * Hˈ)
# print(Hˈˈ*Xˈ*Hˈ*CNOT*Hˈ*Xˈ*Hˈˈ)
# print(Hˈˈ*M*Hˈˈ)
# print(IH*CNOT*IH)

H = [1 1; 1 -1]
X = [0 1; 1 0]
Z = [1 0; 0 -1]
# print(H*X*Z*X*H)

# Poniżej tak naprawdę są macierze -Uw i -Us
Us = [1/2 -1/2 -1/2 -1/2; -1/2 1/2 -1/2 -1/2; -1/2 -1/2 1/2 -1/2; -1/2 -1/2 -1/2 1/2]
# Uw = [1 0 0 0; 0 -1 0 0; 0 0 -1 0; 0 0 0 -1]
Uw = [-1 0 0 0; 0 1 0 0; 0 0 1 0; 0 0 0 1]
s = [1/2; 1/2; 1/2; 1/2]
# print(SS*IH*CNOT*IH*SS)

# print(Us*Uw*s)
# print(IH*CZ*IH)
print(HH*CZ*HH)

# print(IS*IH*CNOT*IH*IS)
