import numpy as np
import math
import time
import sys
from scipy.sparse import kron, identity
from scipy.sparse.linalg import eigsh  # Lanczos routine from ARPACK

spin = 0
m = 0 # Threshold of the basis of subsystem A (if the Hilbert space expands it, we are renormalizing the basis).
      # Also number of eigenvectors of the ρₐ from which we will build the truncation operator.
number_of_iterations = 0

# Matrices for spin = 1/2.
S0z = (1/2)*np.array([[1, 0],[0, -1]], dtype='d')
S0p = np.array([[0, 1],[0, 0]], dtype='d')
S0m = np.array([[0, 0],[1, 0]], dtype='d')

# Matrices for spin = 1.
S1z = np.array([[1, 0, 0], [0, 0, 0], [0, 0, -1]], dtype='d')
S1p = math.sqrt(2)*np.array([[0, 1, 0], [0, 0, 1], [0, 0, 0]], dtype='d')
S1m = math.sqrt(2)*np.array([[0, 0, 0], [1, 0, 0], [0, 1, 0]], dtype='d')

Sz = []
Sp = []
Sm = []
H = []

def compute_cell_of_rho_matrix(rho, psi, d, k, l):
    value = 0
    for i in range(0, d):
        value += psi[l*d + i]*np.conjugate(psi[(((d*i + k) % d) * d) + math.floor(((d*i) + k)/d)])
    rho[k, l] = value

def main():
    start_time = time.time()
    if len(sys.argv) != 4:
        print("Incorrect number of arguments. They should be passed in format 'SPIN_TYPE TRUNCATION_THRESHOLD NUMBER_OF_ITERATIONS'")
        sys.exit(0)
    else:
        spin = float(sys.argv[1])
        m = int(sys.argv[2])
        number_of_iterations = int(sys.argv[3])

    print('Spin: ' + str(spin))

    if spin == 0.5:
        Sz = S0z
        Sp = S0p
        Sm = S0m
        I = identity(2)
        zeros = np.array([[0, 0], [0, 0]], dtype='d')
    elif spin == 1:
        Sz = S1z
        Sp = S1p
        Sm = S1m
        I = identity(3)
        zeros = np.array([[0, 0, 0], [0, 0, 0], [0, 0, 0]], dtype='d')
    else:
        print("Given value of spin is incorrect (or not supported).")
        sys.exit()

    # Initialization of the block's Hamiltonian (empty, without any sites).
    subsystem_A_H = zeros
    last_site_Sz = Sz
    last_site_Sp = Sp

    for i in range(0, number_of_iterations):
        print("Step: ", i)

        # Identity operator of the same size as the subsystem A.
        subsystem_A_I = identity(np.shape(subsystem_A_H)[0])

        # Operator S₋ is just hermitian conjugate of the S₊ operator.
        last_site_Sm = last_site_Sp.transpose().conjugate()

        # Calculating hamiltonian of subsystem A with one site added.
        subsystem_A_H = kron(subsystem_A_H, I)

        new_site_Sz = kron(subsystem_A_I, Sz)
        new_site_Sp = kron(subsystem_A_I, Sp)
        new_site_Sm = kron(subsystem_A_I, Sm)

        subsystem_A_H += kron(last_site_Sz, I)*new_site_Sz
        subsystem_A_H += (1/2)*kron(last_site_Sp, I)*new_site_Sm
        subsystem_A_H += (1/2)*kron(last_site_Sm, I)*new_site_Sp

        last_site_Sz = new_site_Sz
        last_site_Sp = new_site_Sp
        last_site_Sm = new_site_Sm

        # If the size of the subsystem A exceeds the threshold, begin renormalization.
        if np.shape(subsystem_A_H)[0] >= m:
            #------------------------
            # Computing the superblock hamiltonian (hamiltonian of subsystem A and symmetrical subsystem B).
            # WARNING: In this algorithm we are using the same hamiltonian for subsystem B as for the subsystem A (in more
            # advanced versions of DMRG these subsystems don't have to be symmetrical).
            subsystem_B_H = subsystem_A_H
            # Hamiltonian of the superblock consisting of subsystems A and B.
            subsystem_A_I = identity(np.shape(subsystem_A_H)[0])
            superblock_H = kron(subsystem_A_H, subsystem_A_I)
            superblock_H += kron(subsystem_A_I, subsystem_B_H)
            superblock_H += kron(last_site_Sz, subsystem_A_I)*kron(subsystem_A_I, last_site_Sz)
            superblock_H += (1/2)*kron(last_site_Sp, subsystem_A_I)*kron(subsystem_A_I, last_site_Sm)
            superblock_H += (1/2)*kron(last_site_Sm, subsystem_A_I)*kron(subsystem_A_I, last_site_Sp)

            # Diagonalize the superblock hamiltonian.
            (eigenvalue,), psi = eigsh(superblock_H, k=1, which="SA")

            # Create the density matrix of the subsystem A (we are using the optimal
            # calculation method, without the need to compute the density matrix of the whole system (subsystems A and B)).
            d = np.shape(subsystem_A_H)[0] # d - size of the basis of subsystem A (or length of its state vecotrs).
            rho_A = np.mat(np.zeros((d, d), dtype='d'))
            for k in range(0, d):
                for l in range(0, d):
                    compute_cell_of_rho_matrix(rho_A, psi, d, k, l)

            # Diagonalize the reduced density matrix of the subsystem A.
            # WARNING: We won't create or diagonalize the reduced density matrix of the subsystem B in this version of the algorithm.
            rho_A_eigenvalues, rho_A_eigenvectors = np.linalg.eigh(rho_A)

            # Take m most significant eigenvectors of the ρₐ density matrix and construct the truncation operator
            # (last m vectors, because results of the "eigh" function are sorted in the ascending order).
            # Notation: sequence[m:n]  -> from the mth item (inclusive) until the nth item (exclusive).
            truncation_operator = np.array(rho_A_eigenvectors[:, -m:], dtype='d')

            # Truncate hamiltonian and spin operators of the A block.
            subsystem_A_H = truncation_operator.conjugate().transpose().dot(subsystem_A_H.dot(truncation_operator))
            last_site_Sz = truncation_operator.conjugate().transpose().dot(last_site_Sz.dot(truncation_operator))
            last_site_Sp = truncation_operator.conjugate().transpose().dot(last_site_Sp.dot(truncation_operator))
            #-----------------------------------

    (eigenvalue,), psi = eigsh(subsystem_A_H, k=1, which="SA")
    print("Min eigenvalue: ", eigenvalue)
    print("Min energy per site: ", eigenvalue / (number_of_iterations+1))

    print("--- %s seconds ---" % (time.time() - start_time))

if __name__== "__main__":
  main()
