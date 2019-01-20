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
def initialize_block(H, Sz, Sp, d, threshold):
    I = identity(d)
    #---------------------ADDING 6 SITES OF THE HEXAGON-------------------------
    last_site_Sz = Sz
    last_site_Sp = Sp
    for i in range(0, 5):
        new_site_Sz = kron(identity(np.shape(H)[0]), Sz)
        new_site_Sp = kron(identity(np.shape(H)[0]), Sp)
        new_site_Sm = new_site_Sp.transpose().conjugate()
        last_site_Sz = kron(kron(identity(np.shape(H)[0]/d), Sz), I)
        last_site_Sp = kron(kron(identity(np.shape(H)[0]/d), Sp), I)
        last_site_Sm = last_site_Sp.transpose().conjugate()
        H = kron(H, I)
        H += (last_site_Sz.dot(new_site_Sz)) + (1/2)*(last_site_Sp.dot(new_site_Sm) + last_site_Sm.dot(new_site_Sp))
    # We have to add interaction between the first and the last site, to close the hexagon.
    I_five_sites = identity(d**5)
    # Spin matrices of the first site.
    first_site_Sz = kron(Sz, I_five_sites)
    first_site_Sp = kron(Sp, I_five_sites)
    first_site_Sm = first_site_Sp.transpose().conjugate()

    # Spin matrices of the 6-th site.
    last_site_Sz = kron(I_five_sites, Sz)
    last_site_Sp = kron(I_five_sites, Sp)
    last_site_Sm = last_site_Sp.transpose().conjugate()

    # We are closing the chain (connecting 6-th site with the first one).
    H += (first_site_Sz.dot(last_site_Sz)) + (1/2)*(first_site_Sp.dot(last_site_Sm) + first_site_Sm.dot(last_site_Sp))

    #--------------------ADDING TWO SITES ON THE EDGES--------------------------

    # We are using exponent 5 instead of 3 in the "indentity" function, because we are including the two new sites added on the edges.
    first_added_site_Sz = kron(identity(d**2), kron(Sz, identity(d**5)))
    first_added_site_Sp = kron(identity(d**2), kron(Sp, identity(d**5)))
    first_added_site_Sm = first_added_site_Sp.transpose().conjugate()

    second_added_site_Sz = kron(identity(d**3), kron(Sz, identity(d**4)))
    second_added_site_Sp = kron(identity(d**3), kron(Sp, identity(d**4)))
    second_added_site_Sm = second_added_site_Sp.transpose().conjugate()

    third_added_site_Sz = kron(identity(d**4), kron(Sz, identity(d**3)))
    third_added_site_Sp = kron(identity(d**4), kron(Sp, identity(d**3)))
    third_added_site_Sm = third_added_site_Sp.transpose().conjugate()

    left_site_Sz = kron(identity(d**6), kron(Sz, I))
    left_site_Sp = kron(identity(d**6), kron(Sp, I))
    left_site_Sm = left_site_Sp.transpose().conjugate()

    right_site_Sz = kron(identity(d**6), kron(I, Sz))
    right_site_Sp = kron(identity(d**6), kron(I, Sp))
    right_site_Sm = right_site_Sp.transpose().conjugate()

    H = kron(H, identity(d**2))
    H += left_site_Sz*first_added_site_Sz + (1/2)*(left_site_Sp*first_added_site_Sm + left_site_Sm*first_added_site_Sp)
    H += third_added_site_Sz*right_site_Sz + (1/2)*(third_added_site_Sp*right_site_Sm + third_added_site_Sm*right_site_Sp)

    spin_operators = [] # We will hold here 5 spin operators (for thie initial hexagon system with 2 additional edge-sites).
    spin_operators.append((left_site_Sz, left_site_Sp))
    spin_operators.append((first_added_site_Sz, first_added_site_Sp))
    spin_operators.append((second_added_site_Sz, second_added_site_Sp))
    spin_operators.append((third_added_site_Sz, third_added_site_Sp))
    spin_operators.append((right_site_Sz, right_site_Sp))

    # We have to renormalize even such a small system, because in the second step of the algorithm we are adding 5 sites at once.
    if (np.shape(H)[0] >= threshold):
        size_of_the_subsystem = np.shape(H)[0]
        environment_H = H
        superblock_H = kron(H, identity(size_of_the_subsystem))
        environment_H = kron(identity(size_of_the_subsystem), environment_H)

        (subsystem_left_Sz, subsystem_left_Sp) = spin_operators[0]
        subsystem_left_Sz = kron(subsystem_left_Sz, identity(size_of_the_subsystem))
        subsystem_left_Sp = kron(subsystem_left_Sp, identity(size_of_the_subsystem))
        subsystem_left_Sm = subsystem_left_Sp.transpose().conjugate()

        (subsystem_middle_Sz, subsystem_middle_Sp) = spin_operators[2]
        subsystem_middle_Sz = kron(subsystem_middle_Sz, identity(size_of_the_subsystem))
        subsystem_middle_Sp = kron(subsystem_middle_Sp, identity(size_of_the_subsystem))
        subsystem_middle_Sm = subsystem_middle_Sp.transpose().conjugate()

        (subsystem_right_Sz, subsystem_right_Sp) = spin_operators[4]
        subsystem_right_Sz = kron(subsystem_right_Sz, identity(size_of_the_subsystem))
        subsystem_right_Sp = kron(subsystem_right_Sp, identity(size_of_the_subsystem))
        subsystem_right_Sm = subsystem_right_Sp.transpose().conjugate()


        (environment_left_Sz, environment_left_Sp) = spin_operators[0]
        environment_left_Sz = kron(environment_left_Sz, identity(size_of_the_subsystem))
        environment_left_Sp = kron(environment_left_Sp, identity(size_of_the_subsystem))
        environment_left_Sm = environment_left_Sp.transpose().conjugate()

        (environment_middle_Sz, environment_middle_Sp) = spin_operators[2]
        environment_middle_Sz = kron(environment_middle_Sz, identity(size_of_the_subsystem))
        environment_middle_Sp = kron(environment_middle_Sp, identity(size_of_the_subsystem))
        environment_middle_Sm = environment_middle_Sp.transpose().conjugate()

        (environment_right_Sz, environment_right_Sp) = spin_operators[4]
        environment_right_Sz = kron(environment_right_Sz, identity(size_of_the_subsystem))
        environment_right_Sp = kron(environment_right_Sp, identity(size_of_the_subsystem))
        environment_right_Sm = environment_right_Sp.transpose().conjugate()

        superblock_H += environment_H
        superblock_H += subsystem_left_Sz*environment_left_Sz + (1/2)*(subsystem_left_Sp*environment_left_Sm + subsystem_left_Sm*environment_left_Sp)
        superblock_H += subsystem_middle_Sz*environment_middle_Sz + (1/2)*(subsystem_middle_Sp*environment_middle_Sm + subsystem_middle_Sm*environment_middle_Sp)
        superblock_H += subsystem_right_Sz*environment_right_Sz + (1/2)*(subsystem_right_Sp*environment_right_Sm + subsystem_right_Sm*environment_right_Sp)

        (H, spin_operators) = renormalize_system(H, superblock_H, spin_operators)

    return (H, spin_operators, 8) # 8 stands for the total number of sites in the whole system (in initialization we are creating 6-sited hexagon
                                    # plus we are adding two sites on the edges).

# subsystem_A_H - Hamiltonian representing the system before the enlargement procedure,
# spin_operators - list of spin operators needed in the process of enlargement (we will consequently replace its elements with the new ones
# and also extend this list), total_number_of_sites - number of sites in the system before enlargement,
# d - size of the basis for the given type of spin, Sz and Sp - basic spin operators for the given type of spin,
# i - step of the main loop (or number of row / number of hexagons added in this step).
def odd_step(subsystem_A_H, spin_operators, total_number_of_sites, d, Sz, Sp, i):
    I = identity(d)
    middle_index = math.ceil(len(spin_operators)/2) - 1 # We have to subtract 1, because list is indexed starting from 0.

    #--------------------------ADDING INNER 3 SITES-----------------------------
    first_site_Sz = kron(identity(np.shape(subsystem_A_H)[0]), kron(Sz, kron(I, I)))
    first_site_Sp = kron(identity(np.shape(subsystem_A_H)[0]), kron(Sp, kron(I, I)))
    first_site_Sm = first_site_Sp.transpose().conjugate()

    second_site_Sz = kron(identity(np.shape(subsystem_A_H)[0]), kron(I, kron(Sz, I)))
    second_site_Sp = kron(identity(np.shape(subsystem_A_H)[0]), kron(I, kron(Sp, I)))
    second_site_Sm = second_site_Sp.transpose().conjugate()

    third_site_Sz = kron(identity(np.shape(subsystem_A_H)[0]), kron(I, kron(I, Sz)))
    third_site_Sp = kron(identity(np.shape(subsystem_A_H)[0]), kron(I, kron(I, Sp)))
    third_site_Sm = third_site_Sp.transpose().conjugate()

    middle_left_site_Sz = kron(spin_operators[middle_index - 1][0], identity(d**3))
    middle_left_site_Sp = kron(spin_operators[middle_index - 1][1], identity(d**3))
    middle_left_site_Sm = middle_left_site_Sp.transpose().conjugate()

    middle_right_site_Sz = kron(spin_operators[middle_index + 1][0], identity(d**3))
    middle_right_site_Sp = kron(spin_operators[middle_index + 1][1], identity(d**3))
    middle_right_site_Sm = middle_right_site_Sp.transpose().conjugate()

    # We are adding values comming from 4 interactions between sites (we are adding 3 new sites).
    subsystem_A_H = kron(subsystem_A_H, identity(d**3))
    subsystem_A_H += middle_left_site_Sz*first_site_Sz + \
        (1/2)*(middle_left_site_Sp*first_site_Sm + middle_left_site_Sm*first_site_Sp)
    subsystem_A_H += first_site_Sz*second_site_Sz + (1/2)*(first_site_Sp*second_site_Sm + first_site_Sm*second_site_Sp)
    subsystem_A_H += second_site_Sz*third_site_Sz + (1/2)*(second_site_Sp*third_site_Sm + second_site_Sm*third_site_Sp)
    subsystem_A_H += third_site_Sz*middle_right_site_Sz + \
        (1/2)*(third_site_Sp*middle_right_site_Sm + third_site_Sm*middle_right_site_Sp)

    spin_operators[middle_index - 1] = (first_site_Sz, first_site_Sp)
    spin_operators[middle_index] = (second_site_Sz, second_site_Sp)
    spin_operators[middle_index + 1] = (third_site_Sz, third_site_Sp)

    #------------------ADDING REST OF THE SITES (IN PAIRS)----------------------

    return (subsystem_A_H, spin_operators, total_number_of_sites + 3)


def even_step(subsystem_A_H, spin_operators, total_number_of_sites, d, Sz, Sp, threshold, i):
    I = identity(d)
    middle_index = math.ceil(len(spin_operators)/2) - 1 # We have to subtract 1, because list is indexed starting from 0.

    #--------------------------ADDING INNER 5 SITES-----------------------------

    current_first_site_Sz = kron(spin_operators[middle_index - 2][0], identity(d**5))
    current_first_site_Sp = kron(spin_operators[middle_index - 2][1], identity(d**5))
    current_first_site_Sm = current_first_site_Sp.transpose().conjugate()

    # current_second_site_Sz = kron(spin_operators[middle_index - 1][0], identity(d**5))
    # current_second_site_Sp = kron(spin_operators[middle_index - 1][1], identity(d**5))
    # current_second_site_Sm = current_second_site_Sp.transpose().conjugate()

    # We are naming the next variable "current_third_site", so that it represents the index in the list.
    current_third_site_Sz = kron(spin_operators[middle_index][0], identity(d**5))
    current_third_site_Sp = kron(spin_operators[middle_index][1], identity(d**5))
    current_third_site_Sm = current_third_site_Sp.transpose().conjugate()

    # current_fourth_site_Sz = kron(spin_operators[middle_index + 1][0], identity(d**5))
    # current_fourth_site_Sp = kron(spin_operators[middle_index + 1][1], identity(d**5))
    # current_fourth_site_Sm = current_fourth_site_Sp.transpose().conjugate()

    # Same naming logic as in the above variable.
    current_fifth_site_Sz = kron(spin_operators[middle_index + 2][0], identity(d**5))
    current_fifth_site_Sp = kron(spin_operators[middle_index + 2][1], identity(d**5))
    current_fifth_site_Sm = current_fifth_site_Sp.transpose().conjugate()

    first_site_Sz = kron(identity(np.shape(subsystem_A_H)[0]), kron(Sz, identity(d**4)))
    first_site_Sp = kron(identity(np.shape(subsystem_A_H)[0]), kron(Sp, identity(d**4)))
    first_site_Sm = first_site_Sp.transpose().conjugate()

    second_site_Sz = kron(identity(np.shape(subsystem_A_H)[0]), kron(I, kron(Sz, identity(d**3))))
    second_site_Sp = kron(identity(np.shape(subsystem_A_H)[0]), kron(I, kron(Sp, identity(d**3))))
    second_site_Sm = second_site_Sp.transpose().conjugate()

    third_site_Sz = kron(identity(np.shape(subsystem_A_H)[0]), kron(identity(d**2), kron(Sz, identity(d**2))))
    third_site_Sp = kron(identity(np.shape(subsystem_A_H)[0]), kron(identity(d**2), kron(Sp, identity(d**2))))
    third_site_Sm = third_site_Sp.transpose().conjugate()

    fourth_site_Sz = kron(identity(np.shape(subsystem_A_H)[0]), kron(identity(d**3), kron(Sz, I)))
    fourth_site_Sp = kron(identity(np.shape(subsystem_A_H)[0]), kron(identity(d**3), kron(Sp, I)))
    fourth_site_Sm = fourth_site_Sp.transpose().conjugate()

    fifth_site_Sz = kron(identity(np.shape(subsystem_A_H)[0]), kron(identity(d**4), Sz))
    fifth_site_Sp = kron(identity(np.shape(subsystem_A_H)[0]), kron(identity(d**4), Sp))
    fifth_site_Sm = fifth_site_Sp.transpose().conjugate()

    # We are adding values comming from 7 interactions between sites (we are adding 3 new sites).
    subsystem_A_H = kron(subsystem_A_H, identity(d**5))
    subsystem_A_H += current_first_site_Sz*first_site_Sz + (1/2)*(current_first_site_Sp*first_site_Sm + current_first_site_Sm*first_site_Sp)
    subsystem_A_H += first_site_Sz*second_site_Sz + (1/2)*(first_site_Sp*second_site_Sm + first_site_Sm*second_site_Sp)
    subsystem_A_H += second_site_Sz*third_site_Sz + (1/2)*(second_site_Sp*third_site_Sm + second_site_Sm*third_site_Sp)
    subsystem_A_H += third_site_Sz*current_third_site_Sz + (1/2)*(third_site_Sp*current_third_site_Sm + third_site_Sm*current_third_site_Sp)
    subsystem_A_H += third_site_Sz*fourth_site_Sz + (1/2)*(third_site_Sp*fourth_site_Sm + third_site_Sm*fourth_site_Sp)
    subsystem_A_H += fourth_site_Sz*fifth_site_Sz + (1/2)*(fourth_site_Sp*fifth_site_Sm + fourth_site_Sm*fifth_site_Sp)
    subsystem_A_H += fifth_site_Sz*current_fifth_site_Sz + (1/2)*(fifth_site_Sp*current_fifth_site_Sm + fifth_site_Sm*current_fifth_site_Sp)

    # Update the spin operators
    spin_operators[middle_index - 2] = (first_site_Sz, first_site_Sp)
    spin_operators[middle_index - 1] = (second_site_Sz, second_site_Sp)
    spin_operators[middle_index] = (third_site_Sz, third_site_Sp)
    spin_operators[middle_index + 1] = (fourth_site_Sz, fourth_site_Sp)
    spin_operators[middle_index + 2] = (fifth_site_Sz, fifth_site_Sp)

    total_number_of_sites += 5

    if (np.shape(subsystem_A_H)[0] >= threshold):
        size_of_the_subsystem = np.shape(subsystem_A_H)[0]
        environment_H = subsystem_A_H
        subsystem_A_H = kron(subsystem_A_H, identity(size_of_the_subsystem))
        environment_H = kron(identity(size_of_the_subsystem), environment_H)

        (subsystem_second_site_Sz, subsystem_second_site_Sp) = spin_operators[middle_index - 1]
        subsystem_second_site_Sm = subsystem_second_site_Sp.transpose().conjugate()
        subsystem_second_site_Sz = kron(subsystem_second_site_Sz, identity(size_of_the_subsystem))

        print("subsystem_second_site_Sz: ", subsystem_second_site_Sz)
        print("spin_operators[...]: ", spin_operators[middle_index - 1][0])

        # (subsystem_fourth_site_Sz, subsystem_fourth_site_Sp) = spin_operators[middle_index + 1]
        # subsystem_fourth_site_Sm = subsystem_fourth_site_Sp.transpose().conjugate()




    #------------------ADDING REST OF THE SITES (IN PAIRS)----------------------

    return (subsystem_A_H, spin_operators, total_number_of_sites)

def compute_cell_of_rho_matrix(rho, psi, d, k, l):
    value = 0
    for i in range(0, d):
        value += psi[l*d + i]*np.conjugate(psi[(((d*i + k) % d) * d) + math.floor(((d*i) + k)/d)])
    rho[k, l] = value

# For a given structure (Hamiltonian of the superblock) it will carry out the procedure of renormalization.
def renormalize_system(subsystem_A_H, superblock_H, spin_operators):
    # print("Blok A na wejściu: ", np.shape(subsystem_A_H))
    # print("Superblok na wejściu: ", np.shape(superblock_H))
    #WARNING: Potrzebne inteligentne wyliczanie m na podstawie otrzymywanego błędu (dopiuszczalny błąd powinien być parametrem tej funkcji).
    m = 20
    # Diagonalize the superblock Hamiltonian.
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
    # WARNING: Find the best size of basis, for which the received error is lower than the desired one
    # (probably it will be necesarry to replace argument "m" of the function with error and number_of_iteration).
    truncation_operator = np.array(rho_A_eigenvectors[:, -m:], dtype='d')

    # Truncate hamiltonian and spin operators of the A block.
    subsystem_A_H = truncation_operator.conjugate().transpose().dot(subsystem_A_H.dot(truncation_operator))

    # We need to truncate all spin operators in the dictionary.
    for x in range(0, len(spin_operators)):
        (Sz_op, Sp_op) = spin_operators[x]
        spin_operators[x] = (truncation_operator.conjugate().transpose().dot(Sz_op.dot(truncation_operator)), truncation_operator.conjugate().transpose().dot(Sp_op.dot(truncation_operator)))

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
        d = 2 # d stands for the size of the basis for the given type of spin.
        Sz = S0z
        Sp = S0p
        Sm = S0m
        threshold = 20 # Threshold after which we will begin renormalization.
        # I = identity(2)
        H = np.array([[0, 0], [0, 0]], dtype='d')
    elif spin == 1:
        d = 3 # d stands for the size of the basis for the given type of spin.
        Sz = S1z
        Sp = S1p
        Sm = S1m
        threshold = 20 # Threshold after which we will begin renormalization.
        # I = identity(3)
        H = np.array([[0, 0, 0], [0, 0, 0], [0, 0, 0]], dtype='d')
    else:
        print("Given value of spin is incorrect (or not supported).")
        sys.exit()

    # Initialization of the block's Hamiltonian (first hexagon).
    (subsystem_A_H, spin_operators, total_number_of_sites) = initialize_block(H, Sz, Sp, d, threshold)

    (eigenvalue_0,), psi_0 = eigsh(subsystem_A_H, k=1, which="SA")
    print("Min eigenvalue for hexagon (precise): ", eigenvalue_0)
    print("Min energy per site (for hexagon): ", eigenvalue_0 / total_number_of_sites)

    # Number of iteration tells us about the number of row, we are adding and also how many hexagons are in this row.
    # Each iteration ends with i-hexagons at the bottom of the structure + 2 additional sites at the edges (only such structure
    # makes with the symmetrical environment the full diamond).
    for i in range(2, number_of_iterations + 1):
        print("--->STEP: ", i)
        # if i % 2 == 1:
        #     (subsystem_A_H, spin_operators, total_number_of_sites) = odd_step(subsystem_A_H, spin_operators, total_number_of_sites, d, Sz, Sp, threshold, i)
        #     # (eigenvalue_0,), psi_0 = eigsh(subsystem_A_H, k=1, which="SA")
        #     # print("Min eigenvalue for hexagon (precise): ", eigenvalue_0)
        #     # print("Min energy per site (for hexagon): ", eigenvalue_0 / total_number_of_sites)
        # else:
        #     (subsystem_A_H, spin_operators, total_number_of_sites) = even_step(subsystem_A_H, spin_operators, total_number_of_sites, d, Sz, Sp, threshold, i)


    (eigenvalue,), psi = eigsh(subsystem_A_H, k=1, which="SA")
    print("Min eigenvalue: ", eigenvalue)
    print("Min energy per site: ", eigenvalue / total_number_of_sites)
    # print("--- %s seconds ---" % (time.time() - start_time))

if __name__== "__main__":
  main()
