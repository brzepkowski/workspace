# serached_elem is decimal representation of element, that we are searching for
def grover(circuit, qr, n, searched_elem):
    bin_searched_elem = list(bin(searched_elem))[2:n+2]
    iterations = (2**n)**(1/2) # √N = √2ⁿ
    for i in range(n):
        circuit.h(qr[i])
    for it in range(iterations):
        for (i, elem) in enumerate(reversed(bin_searched_elem)):
            print(i, " -> ", elem)
            if elem == '1':
                print("Git")
            else:
                print("Nie git")
