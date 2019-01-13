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

# This function creates Hamiltonian for the initial benzene structure (hexagon).
# H - empty matrix (its size depends from the type of spin), Sz - spin Z matrix, Sp - spin S₊ matrix
def initialize_block(H, Sz, Sp):
    I = identity(np.shape(H)[0])
    last_site_Sz = Sz
    last_site_Sp = Sp
    for i in range(0, 5):
        new_site_Sz = kron(identity(np.shape(H)[0]), Sz)
        new_site_Sp = kron(identity(np.shape(H)[0]), Sp)
        new_site_Sm = new_site_Sp.transpose().conjugate()
        last_site_Sz = kron(last_site_Sz, I)
        last_site_Sp = kron(last_site_Sp, I)
        last_site_Sm = last_site_Sp.transpose().conjugate()
        H = kron(H, I)
        H += (last_site_Sz*new_site_Sz) + (1/2)*(last_site_Sp*new_site_Sm + last_site_Sm*new_site_Sp)
    I_whole = identity(np.shape(H)[0]) # Here I will be the size of 5-site chain (or hexagon without the energy beween the last and first sites).
    last_site_Sz = kron(Sz, I_whole)
    last_site_Sp = kron(Sp, I_whole)
    last_site_Sm = last_site_Sp.transpose().conjugate()
    new_site_Sz = kron(I_whole, Sz)
    new_site_Sp = kron(I_whole, Sp)
    new_site_Sm = new_site_Sp.transpose().conjugate()
    H = kron(H, I)
    H += (last_site_Sz*new_site_Sz) + (1/2)*(last_site_Sp*new_site_Sm + last_site_Sm*new_site_Sp)
    return H

def compute_cell_of_rho_matrix(rho, psi, d, k, l):
    value = 0
    for i in range(0, d):
        value += psi[l*d + i]*np.conjugate(psi[(((d*i + k) % d) * d) + math.floor(((d*i) + k)/d)])
    rho[k, l] = value

# number_of_sites - number of sites in the 1D spin chain,
# spin - type of spin, H - initial zeroed Hamiltonian, Sz - spin z operator,
# Sp - spin + operator, I - identity matrix of size corresponding to the spin type.
# TODO: Replace this version with the finite size DMRG.
def generate_1d_spin_chain(number_of_sites, spin, H, Sz, Sp, I, m):
    Sm = Sp.transpose().conjugate()
    subsystem_A_H = H # H that was passed to this function is just zeroed matrix of appropriate size.
    last_site_Sz = Sz # Sz is spin z matrix appropriate for given spin type.
    last_site_Sp = Sp # Sz is spin S₊ matrix appropriate for given spin type.

    spin_operators = {} # This dictionary will hold spin operators for all sites.
                        # This dictionar will be indexed with numbers (corresponding to the number of site).
                        # Each entry will hold pair of operators (for Sz and Sp operators).
                        # We need to hold operators for all sites, because some of them
                        # will be needed in the process of adding this 1D chain to the previous structure
                        # and alter some other will become the edge spin operators needed in the next step of the iteration.
    spin_operators[1] = (Sz, Sp)

    # Although the initial Hamiltonian is empty it represents system with one site
    # (in the Heisenberg model only interactions BETWEEN spins are added to the initial Hamiltonian).
    for i in range(2, number_of_sites + 1): # TODO: <---- sprawdzić zakres!!! | Here we are just saying, that we began with one site and we are adding the second one and so on).
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

        last_site_Sz = kron(last_site_Sz, I)
        subsystem_A_H += last_site_Sz*new_site_Sz
        subsystem_A_H += (1/2)*kron(last_site_Sp, I)*new_site_Sm
        subsystem_A_H += (1/2)*kron(last_site_Sm, I)*new_site_Sp

        last_site_Sz = new_site_Sz
        last_site_Sp = new_site_Sp
        last_site_Sm = new_site_Sm

        # Expand all spin operators from the dictionary (we added new site,
        # so we need to include this information in these spin operators (using kronecker product)).
        for key in spin_operators.keys(): # Reaching the keys of dictionary.
            (Sz_op, Sp_op) = spin_operators[key] # Reaching every element in the tuple.
            spin_operators[key] = (kron(Sz_op, I), kron(Sp_op, I))

        # If the size of the subsystem A exceeds the threshold, begin renormalization.
        if np.shape(subsystem_A_H)[0] >= m:
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
            # print("Rho: ", np.shape(rho_A))
            rho_A_eigenvalues, rho_A_eigenvectors = np.linalg.eigh(rho_A)

            # Take m most significant eigenvectors of the ρₐ density matrix and construct the truncation operator
            # (last m vectors, because results of the "eigh" function are sorted in the ascending order).
            # Notation: sequence[m:n]  -> from the mth item (inclusive) until the nth item (exclusive).
            truncation_operator = np.array(rho_A_eigenvectors[:, -m:], dtype='d')

            # Truncate hamiltonian and spin operators of the A block.
            # print("truncation_operator: ", np.shape(truncation_operator))
            # print("subsystem_A_H: ", np.shape(subsystem_A_H))
            # print("last_site_Sz: ", np.shape(last_site_Sz))
            # print("last_site_Sp: ", np.shape(last_site_Sp))
            subsystem_A_H = truncation_operator.conjugate().transpose().dot(subsystem_A_H.dot(truncation_operator))
            last_site_Sz = truncation_operator.conjugate().transpose().dot(last_site_Sz.dot(truncation_operator))
            last_site_Sp = truncation_operator.conjugate().transpose().dot(last_site_Sp.dot(truncation_operator))

            # We need to truncate all spin operators in the dictionary.
            for key in spin_operators.keys(): # Reaching the keys of dictionary.
                (Sz_op, Sp_op) = spin_operators[key] # Reaching every element in the tuple.
                spin_operators[key] = (truncation_operator.conjugate().transpose().dot(Sz_op.dot(truncation_operator)), truncation_operator.conjugate().transpose().dot(Sp_op.dot(truncation_operator)))

        # Add spin operators of the last site to the dictionary (note, that the new operator is already truncated,
        # so we do not need to truncate it as in the above loop).
        spin_operators[i] = (last_site_Sz, last_site_Sp)

    return (subsystem_A_H, spin_operators)
#-------------------------------------------------------------------------------


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
        H = np.array([[0, 0], [0, 0]], dtype='d')
    elif spin == 1:
        Sz = S1z
        Sp = S1p
        Sm = S1m
        I = identity(3)
        H = np.array([[0, 0, 0], [0, 0, 0], [0, 0, 0]], dtype='d')
    else:
        print("Given value of spin is incorrect (or not supported).")
        sys.exit()

    # Initialization of the block's Hamiltonian (empty, without any sites).
    subsystem_A_H = initialize_block(H, Sz, Sp)
    # print("H: ", subsystem_A_H)

    (new_chain, new_chain_spin_operators) = generate_1d_spin_chain(7, spin, H, Sz, Sp, I, m)
    # (new_chain, new_chain_spin_operators) = generate_1d_spin_chain(100, spin, H, Sz, Sp, I, m)
    # print("new_chain_H: ", new_chain)
    print("Operators: ", new_chain_spin_operators.keys())



    # last_site_Sz = Sz
    # last_site_Sp = Sp

    # for i in range(0, number_of_iterations):
    #     print("Step: ", i)
    #
    #     # Identity operator of the same size as the subsystem A.
    #     subsystem_A_I = identity(np.shape(subsystem_A_H)[0])
    #
    #     # Operator S₋ is just hermitian conjugate of the S₊ operator.
    #     last_site_Sm = last_site_Sp.transpose().conjugate()
    #
    #     # Calculating hamiltonian of subsystem A with one site added.
    #     subsystem_A_H = kron(subsystem_A_H, I)
    #
    #     new_site_Sz = kron(subsystem_A_I, Sz)
    #     new_site_Sp = kron(subsystem_A_I, Sp)
    #     new_site_Sm = kron(subsystem_A_I, Sm)
    #
    #     subsystem_A_H += kron(last_site_Sz, I)*new_site_Sz
    #     subsystem_A_H += (1/2)*kron(last_site_Sp, I)*new_site_Sm
    #     subsystem_A_H += (1/2)*kron(last_site_Sm, I)*new_site_Sp
    #
    #     subsystem_A_I = kron(subsystem_A_I, I)
    #     last_site_Sz = new_site_Sz
    #     last_site_Sp = new_site_Sp
    #     last_site_Sm = new_site_Sm
    #
    #     # If the size of the subsystem A exceeds the threshold, begin renormalization.
    #     if np.shape(subsystem_A_H)[0] >= m:
    #         #------------------------
    #         # Computing the superblock hamiltonian (hamiltonian of subsystem A and symmetrical subsystem B).
    #         # WARNING: In this algorithm we are using the same hamiltonian for subsystem B as for the subsystem A (in more
    #         # advanced versions of DMRG these subsystems don't have to be symmetrical).
    #         subsystem_B_H = subsystem_A_H
    #         # Hamiltonian of the superblock consisting of subsystems A and B.
    #         subsystem_A_I = identity(np.shape(subsystem_A_H)[0])
    #         superblock_H = kron(subsystem_A_H, subsystem_A_I)
    #         superblock_H += kron(subsystem_A_I, subsystem_B_H)
    #         superblock_H += kron(last_site_Sz, subsystem_A_I)*kron(subsystem_A_I, last_site_Sz)
    #         superblock_H += (1/2)*kron(last_site_Sp, subsystem_A_I)*kron(subsystem_A_I, last_site_Sm)
    #         superblock_H += (1/2)*kron(last_site_Sm, subsystem_A_I)*kron(subsystem_A_I, last_site_Sp)
    #
    #         # Diagonalize the superblock hamiltonian.
    #         (eigenvalue,), psi = eigsh(superblock_H, k=1, which="SA")
    #
    #         # Create the density matrix of the subsystem A (we are using the optimal
    #         # calculation method, without the need to compute the density matrix of the whole system (subsystems A and B)).
    #         d = np.shape(subsystem_A_H)[0] # d - size of the basis of subsystem A (or length of its state vecotrs).
    #         rho_A = np.mat(np.zeros((d, d), dtype='d'))
    #         for k in range(0, d):
    #             for l in range(0, d):
    #                 compute_cell_of_rho_matrix(rho_A, psi, d, k, l)
    #
    #         # Diagonalize the reduced density matrix of the subsystem A.
    #         # WARNING: We won't create or diagonalize the reduced density matrix of the subsystem B in this version of the algorithm.
    #         rho_A_eigenvalues, rho_A_eigenvectors = np.linalg.eigh(rho_A)
    #
    #         # Take m most significant eigenvectors of the ρₐ density matrix and construct the truncation operator
    #         # (last m vectors, because results of the "eigh" function are sorted in the ascending order).
    #         # Notation: sequence[m:n]  -> from the mth item (inclusive) until the nth item (exclusive).
    #         truncation_operator = np.array(rho_A_eigenvectors[:, -m:], dtype='d')
    #
    #         # Truncate hamiltonian and spin operators of the A block.
    #         subsystem_A_H = truncation_operator.conjugate().transpose().dot(subsystem_A_H.dot(truncation_operator))
    #         last_site_Sz = truncation_operator.conjugate().transpose().dot(last_site_Sz.dot(truncation_operator))
    #         last_site_Sp = truncation_operator.conjugate().transpose().dot(last_site_Sp.dot(truncation_operator))
    #         #-----------------------------------
    #
    # (eigenvalue,), psi = eigsh(subsystem_A_H, k=1, which="SA")
    # print("Min eigenvalue: ", eigenvalue)
    # print("Min energy per site: ", eigenvalue / (number_of_iterations+1))
    #
    # print("--- %s seconds ---" % (time.time() - start_time))

if __name__== "__main__":
  main()
