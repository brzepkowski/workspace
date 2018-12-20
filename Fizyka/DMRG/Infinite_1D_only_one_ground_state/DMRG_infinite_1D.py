import numpy as np
import scipy
# import scipy.sparse.linalg
import math
import cmath
import time
import sys
from multiprocessing import Process, Value, Array, Lock, Queue
from scipy.sparse import kron, identity
from scipy.sparse.linalg import eigsh  # Lanczos routine from ARPACK
# import ctypes

spin = 0
m = 0 # Threshold of the basis of subsystem A (if the Hilbert space expands it we are renormalizing the basis)
number_of_iterations = 0
im = complex(0, 1)
# I = np.mat(np.identity(2))
# Matrices for spin = 1/2
S0x = (1/2)*np.mat([[0, 1], [1, 0]])
S0y = (1/2)*np.mat([[0, -im], [im, 0]])
S0z = (1/2)*np.mat([[1, 0],[0, -1]])

# Matrices for spin = 1
S1x = (1/cmath.sqrt(2))*np.mat([[0, 1, 0], [1, 0, 1], [0, 1, 0]])
S1y = (1/cmath.sqrt(2*im))*np.mat([[0, 1, 0], [-1, 0, 1], [0, -1, 0]])
S1z = np.mat([[1, 0, 0], [0, 0, 0], [0, 0, -1]])

Sx = []
Sy = []
Sz = []
H = []

def compute_cell_of_rho_matrix(rho, psi, d, k, l):
    # print("Arguments: ", d, ", ", k, ", ", l)
    value = 0
    for i in range(0, d):
        value += psi[l*d + i]*np.conjugate(psi[(((d*i + k) % d) * d) + math.floor(((d*i) + k)/d)])
    rho[k, l] = value

def main():
    start_time = time.time()
    # spin = float(input('Give type of spin: '))
    if len(sys.argv) != 4:
        print("Incorrect number of arguments.")
        sys.exit(0)
    else:
        spin = float(sys.argv[1])
        m = int(sys.argv[2])
        number_of_iterations = int(sys.argv[3])

    print('Spin: ' + str(spin))

    if spin == 0.5:
        Sx = S0x
        Sy = S0y
        Sz = S0z
        I = np.mat(np.identity(2))
        # H = np.mat(np.zeros((2, 2), dtype = complex))
    elif spin == 1:
        Sx = S1x
        Sy = S1y
        Sz = S1z
        I = np.mat(np.identity(3))
        # H = np.mat(np.zeros((3, 3), dtype = complex))
    else:
        print("Given value of spin is incorrect (or not supported).")

    # Operators for subsystem A (which will be expanded by adding new sites)
    subsystem_A_Sx = Sx
    subsystem_A_Sy = Sy
    subsystem_A_Sz = Sz
    subsystem_A_I = I # Identity operator, which will grow at the same rate as other operators of subsystem A
    subsystem_A_H = subsystem_A_Sx + subsystem_A_Sy + subsystem_A_Sz # Initialization of hamiltonian for subsystem A (initially there is one site in it)

    for i in range(0, number_of_iterations):
        print("Step: ", i)
        # Calculating hamiltonian of subsystem A with added one site
        subsystem_A_H = np.kron(subsystem_A_H, I)
        new_site_Sx = np.kron(subsystem_A_I, Sx)
        new_site_Sy = np.kron(subsystem_A_I, Sy)
        new_site_Sz = np.kron(subsystem_A_I, Sz)
        subsystem_A_I = np.kron(subsystem_A_I, I)
        subsystem_A_H = subsystem_A_H + new_site_Sx + new_site_Sy + new_site_Sz

        # If the size of the subsystem A exceeds threshold begin renormalization
        if np.shape(subsystem_A_H)[0] >= math.sqrt(m):
            # Computing the superblock hamiltonian (hamiltonian of subsystem A and symmetrical subsystem B)
            # WARNING: In this algorithm we are using the same hamiltonian for subsystem B as for the subsystem A (in more
            # advanced versions of DMRG these subsystems don't have to be symmetrical).
            print("Rozmiar A_H NA WEJSCIU: ", np.shape(subsystem_A_H))
            subsystem_B_H = subsystem_A_H
            superblock_H = np.kron(subsystem_A_H, subsystem_B_H) # Hamiltonian of the superblock consisting of subsystems A and B

            # Diagonalize the superblock hamiltonian
            print("Rozmiar: ", np.shape(superblock_H))
            eigenvalues, eigenvectors = scipy.linalg.eig(superblock_H) # Dla m = 4 zwraca psi rozmiaru 16
            # eigenvalues, eigenvectors = eigsh(superblock_H) # Dla m = 4 zwraca psi rozmiaru 6
            print("Zdiagonalizowano superblok")
            # print("Eigenvalues: ", np.sort(eigenvalues))
            min_eigenval_index = np.argmin(eigenvalues)
            # min_eigenval_index2 = np.argmin(eigenvalues2)
            #-------------------------------------------------------------------
            psi = eigenvectors[min_eigenval_index]
            # psi2 = eigenvectors2[min_eigenval_index2]
            # print("1: ", psi)
            # print("2: ", psi2)

            # Create the density matrix of the subsystem A (we are using the optimal
            # calculation method, without the need to compute the density matrix of the whole system (subsystems A and B))
            d = np.shape(subsystem_A_H)[0] # d - size of the basis of subsystem A (or length of its state vecotrs)
            rho_A = np.mat(np.zeros((d, d), dtype = complex))
            q = Queue()
            for k in range(0, d):
                for l in range(0, d):
                    compute_cell_of_rho_matrix(rho_A, psi, d, k, l)

            # Diagonalize the reduced density matrix of the subsystem A
            # WARNING: We won't create or diagonalize the reduced density matrix of the subsystem B in this version of the algorithm
            # The eigenvalues of the ρₐ density matrix are ordered in descending order
            rho_A_eigenvalues, rho_A_eigenvectors = scipy.linalg.eig(rho_A)
            print("Zdiagonalizowano macierz gestosci podsystemu A")

            # Take first m eigenvectors of the ρₐ density matrix and construct the truncation operator
            # Notation: sequence[m:n]  #the mth item until the nth item (exclusive)
            truncation_operator = np.mat(rho_A_eigenvectors[:, 0:m])
            # print(truncation_operator)

            # Truncate all operators
            print("Rozmiar A_H przed: ", np.shape(subsystem_A_H))
            subsystem_A_H = truncation_operator.H * subsystem_A_H * truncation_operator
            print("Rozmiar A_H po: ", np.shape(subsystem_A_H))
            subsystem_A_I = truncation_operator.H * subsystem_A_I * truncation_operator


    eigenvalues, eigenvectors = scipy.linalg.eig(subsystem_A_H)
    print("Eigenvalues: ", eigenvalues)
    min_eigenvalue = min(eigenvalues)
    print("Min eigenvalue: ", min_eigenvalue)
    print("Min energy per site: ", min_eigenvalue / number_of_iterations)
    print("--- %s seconds ---" % (time.time() - start_time))

if __name__== "__main__":
  main()
