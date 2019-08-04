using Base.LinAlg.svd

psi = (1/sqrt(2))*[1 0 0 0 0 0 0 1]
rho = psi.*transpose(conj(psi))
U, D, V = svd(rho)
show(rho)
# show(U)
# show(diagm(D))
# show(V')
# print(U*diagm(D)*V')
