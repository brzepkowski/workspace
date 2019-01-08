import numpy as np
import scipy
import math
import cmath
import time
import sys
from scipy.sparse import kron, identity
from scipy.sparse.linalg import eigsh  # Lanczos routine from ARPACK

spin = 0
m = 0 # Threshold of the basis of subsystem A (if the Hilbert space expands it we are renormalizing the basis).
      # Also number of eigenvectors of the ρₐ from which we will build the truncation operator
number_of_iterations = 0
im = complex(0, 1)
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
        zeros = np.mat(np.zeros((2, 2), dtype = complex))
    elif spin == 1:
        Sx = S1x
        Sy = S1y
        Sz = S1z
        I = np.mat(np.identity(3))
        zeros = np.mat(np.zeros((3, 3), dtype = complex))
    else:
        print("Given value of spin is incorrect (or not supported).")

    # Operators for subsystem A (which will be expanded by adding new sites)
    subsystem_A_Sx = Sx
    subsystem_A_Sy = Sy
    subsystem_A_Sz = Sz
    subsystem_A_I_whole = I # Identity operator, which will grow at the same rate as other operators of subsystem A (it "includes" all sites in the block)
    subsystem_A_I_restr = [] # Identity operator, that does not "include" the last site in the block

    # Initialization of the block's Hamiltonian (empty, without any sites)
    subsystem_A_H = zeros
    subsystem_A_I_whole = I
    subsystem_A_I_restr = [1]

    for i in range(0, number_of_iterations):
        print("Step: ", i)
        # Identity matrices of the whole block and whole block without the right-most site
        subsystem_A_I_whole = np.mat(np.identity(np.shape(subsystem_A_H)[0]))
        subsystem_A_I_restr = np.mat(np.identity(int(np.shape(subsystem_A_H)[0]/2)))

        # Calculating hamiltonian of subsystem A with one site added
        subsystem_A_H = kron(subsystem_A_H, I)
        subsystem_A_H += kron(kron(subsystem_A_I_restr, Sx), I)*kron(subsystem_A_I_whole, Sx)
        subsystem_A_H += kron(kron(subsystem_A_I_restr, Sy), I)*kron(subsystem_A_I_whole, Sy)
        subsystem_A_H += kron(kron(subsystem_A_I_restr, Sz), I)*kron(subsystem_A_I_whole, Sz)
        subsystem_A_I_whole = kron(subsystem_A_I_whole, I)
        subsystem_A_I_restr = kron(subsystem_A_I_restr, I)

        # If the size of the subsystem A exceeds the threshold, begin renormalization
        if np.shape(subsystem_A_H)[0] >= m:
            # Computing the superblock hamiltonian (hamiltonian of subsystem A and symmetrical subsystem B)
            # WARNING: In this algorithm we are using the same hamiltonian for subsystem B as for the subsystem A (in more
            # advanced versions of DMRG these subsystems don't have to be symmetrical).
            subsystem_B_H = subsystem_A_H
            # Hamiltonian of the superblock consisting of subsystems A and B
            superblock_H = kron(subsystem_A_H, subsystem_A_I_whole)
            superblock_H += kron(subsystem_A_I_whole, subsystem_B_H)
            superblock_H += kron(kron(subsystem_A_I_restr, Sx), subsystem_A_I_whole)*kron(subsystem_A_I_whole, kron(Sx, subsystem_A_I_restr))
            superblock_H += kron(kron(subsystem_A_I_restr, Sy), subsystem_A_I_whole)*kron(subsystem_A_I_whole, kron(Sy, subsystem_A_I_restr))
            superblock_H += kron(kron(subsystem_A_I_restr, Sz), subsystem_A_I_whole)*kron(subsystem_A_I_whole, kron(Sz, subsystem_A_I_restr))

            # Diagonalize the superblock hamiltonian
            (eigenvalue,), psi = eigsh(superblock_H, k=1, which="SA")

            # Create the density matrix of the subsystem A (we are using the optimal
            # calculation method, without the need to compute the density matrix of the whole system (subsystems A and B))
            d = np.shape(subsystem_A_H)[0] # d - size of the basis of subsystem A (or length of its state vecotrs)
            rho_A = np.mat(np.zeros((d, d), dtype = complex))
            for k in range(0, d):
                for l in range(0, d):
                    compute_cell_of_rho_matrix(rho_A, psi, d, k, l)

            # Diagonalize the reduced density matrix of the subsystem A
            # WARNING: We won't create or diagonalize the reduced density matrix of the subsystem B in this version of the algorithm
            # The eigenvalues of the ρₐ density matrix are ordered in descending order
            rho_A_eigenvalues, rho_A_eigenvectors = scipy.linalg.eigh(rho_A)

            # Take m most significant eigenvectors of the ρₐ density matrix and construct the truncation operator
            # (last m vectors, because results of the "eigh" function are sorted in the ascending order)
            # Notation: sequence[m:n]  -> from the mth item (inclusive) until the nth item (exclusive)
            truncation_operator = np.mat(rho_A_eigenvectors[:, -m:])

            # Truncate hamiltonian of the A block
            subsystem_A_H = truncation_operator.H * subsystem_A_H * truncation_operator

    eigenvalues, eigenvectors = scipy.linalg.eig(subsystem_A_H)
    print("Eigenvalues: ", eigenvalues)
    min_eigenvalue = min(eigenvalues)
    print("Min eigenvalue: ", min_eigenvalue)
    print("Min energy per site: ", min_eigenvalue / (number_of_iterations + 1))

    print("--- %s seconds ---" % (time.time() - start_time))

if __name__== "__main__":
  main()
