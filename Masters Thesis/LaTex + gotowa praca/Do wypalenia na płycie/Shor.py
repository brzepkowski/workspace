import math
from math import trunc
from fractions import Fraction

def get_binary(number, n):
    number_bin = bin(number)[2:]
    l = len(number_bin)
    while l < n:
        number_bin = '0' + number_bin
        l += 1
    return number_bin

def get_twos_complement_binary(number, n):
    number_bin = bin(number)[2:]
    l = len(number_bin)
    while l < n:
        number_bin = '0' + number_bin
        l += 1
    number_bin_bit_neg = ''
    for (i, bit) in enumerate(number_bin):
        if bit == '0':
            number_bin_bit_neg += '1'
        else:
            number_bin_bit_neg += '0'
    carry = 1
    number_bin_neg = ''
    for (i, bit) in enumerate(reversed(number_bin_bit_neg)):
        if carry == 1 and bit == '0':
            number_bin_neg = '1' + number_bin_neg
            carry = 0
        elif carry == 1 and bit == '1':
            number_bin_neg = '0' + number_bin_neg
        elif carry == 0:
            number_bin_neg = bit + number_bin_neg
    return number_bin_neg

def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)

def Toffoli_gate(circuit, qr, n, control1, control2, target):
    circuit.ccx(qr[control1], qr[control2], qr[target])
    for k in range(n):
        if k != control1 and k != control2 and k != target:
            circuit.iden(qr[k])

def CNOT_gate(circuit, qr, n, control, target):
    circuit.cx(qr[control], qr[target])
    for k in range(n):
        if k != control and k != target:
            circuit.iden(qr[k])

def NOT_gate(circuit, qr, n, target):
    circuit.x(qr[target])
    for k in range(n):
        if k != target:
            circuit.iden(qr[k])

def SWAP_gate(circuit, qr, n, target1, target2):
    circuit.swap(qr[target1], qr[target2])
    for k in range(n):
        if k != target1 and k != target2:
            circuit.iden(qr[k])

def Hadamard_gate(circuit, qr, n, target):
    circuit.h(qr[target])
    for k in range(n):
        if k != target:
            circuit.iden(qr[k])

def CU1_INV_gate(circuit, qr, n, theta, control, target):
    circuit.cu1(theta, qr[control], qr[target]).inverse()
    for k in range(n):
        if k != control and k != target:
            circuit.iden(qr[k])

# def C_n_controlled_SWAP_gate(circuit, qr, m, controls, target1, target2):
#     inner_controls = list(controls)
#     inner_controls.append(target1)
#     C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, target2)
#     inner_controls = list(controls)
#     inner_controls.append(target2)
#     C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, target1)
#     inner_controls = list(controls)
#     inner_controls.append(target1)
#     C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, target2)

# indices of control_qubits and terget_qubit have to be precise (not mapped or with additional starting point)
# n = length of controls array can be arbitrary (for n = 0 we add NOT gate, n = 1, CNOT, n = 2 Toffoli and so on)
def C_n_controlled_NOT_gate(circuit, qr, m, controls, target): # NOT gate with n controls
    n = len(controls)
    if (n - 1) > (math.floor((m-1)/2)+1): # +1 added for safety
        raise Exception("Too many controls in C^n NOT gate for such circuit")
    if n == 0:
        NOT_gate(circuit, qr, m, target)
    if n == 1:
        CNOT_gate(circuit, qr, m, controls[0], target)
    if n == 2:
        Toffoli_gate(circuit, qr, m, controls[0], controls[1], target)
    else:
        all_initial_indices = list(controls) # We are making copy of control qubits
        all_initial_indices.append(target)
        temp_control = target + 1
        temp_target = target
        used_indices = []
        for (i, control) in enumerate(controls):
            if i == n - 2:
                Toffoli_gate(circuit, qr, m, control, controls[n - 1], temp_target)
            elif i < n - 2:
                while temp_control in all_initial_indices:
                    if temp_control + 1 == m:
                        temp_control = 0
                    else:
                        temp_control += 1
                Toffoli_gate(circuit, qr, m, temp_control, control, temp_target)
                temp_target = temp_control
                used_indices.append(temp_control)
                temp_control += 1
        k = len(used_indices)
        for i in range(2, n - 1):
            if i == 2:
                Toffoli_gate(circuit, qr, m, controls[n - 3], used_indices[k - 1], used_indices[k - 2])
            else:
                Toffoli_gate(circuit, qr, m, controls[n - i - 1], used_indices[k - i + 1], used_indices[k - i])
        for i in range(n-1):
            if i == 0:
                Toffoli_gate(circuit, qr, m, controls[0], used_indices[0], target)
            elif i < n - 2:
                Toffoli_gate(circuit, qr, m, controls[i], used_indices[i], used_indices[i - 1])
            else:
                Toffoli_gate(circuit, qr, m, controls[n - 2], controls[n - 1], used_indices[k - 1])
        for i in range(2, n - 1):
            if i == 2:
                Toffoli_gate(circuit, qr, m, controls[n - 3], used_indices[k - 1], used_indices[k - 2])
            else:
                Toffoli_gate(circuit, qr, m, controls[n - i - 1], used_indices[k - i + 1], used_indices[k - i])

# In fact, we will implement MAJ in reverse order, because we want to subtract instead of add
# 'controls' is an array of controls (needs to have length at least 2)
def multiply_controlled_MAJ_gate(circuit, qr, m, top_qubit, middle_qubit, bottom_qubit, controls):
    first_controls = list(controls)
    first_controls.append(top_qubit)
    first_controls.append(middle_qubit)
    second_controls = list(controls)
    second_controls.append(bottom_qubit)
    third_controls = list(controls)
    third_controls.append(bottom_qubit)
    C_n_controlled_NOT_gate(circuit, qr, m, first_controls, bottom_qubit)
    C_n_controlled_NOT_gate(circuit, qr, m, second_controls, top_qubit)
    C_n_controlled_NOT_gate(circuit, qr, m, third_controls, middle_qubit)

# In fact, we will implement UMA in reverse order, because we want to subtract instead of add
# 'controls' is an array of controls (needs to have length at least 2)
def multiply_controlled_UMA_gate(circuit, qr, m, top_qubit, middle_qubit, bottom_qubit, controls):
    first_controls = list(controls)
    first_controls.append(top_qubit)
    second_controls = list(controls)
    second_controls.append(bottom_qubit)
    third_controls = list(controls)
    third_controls.append(top_qubit)
    third_controls.append(middle_qubit)
    C_n_controlled_NOT_gate(circuit, qr, m, first_controls, middle_qubit)
    C_n_controlled_NOT_gate(circuit, qr, m, second_controls, top_qubit)
    C_n_controlled_NOT_gate(circuit, qr, m, third_controls, bottom_qubit)

# Inverse Quantum Fourier Transform
def inv_qft(circuit, qr, m, n, starting_index):
    π = math.pi
    theta = 2 * π / float(2**n)
    for i in reversed(range(n)):
        Hadamard_gate(circuit, qr, m, starting_index + i)
        for j in range(i):
            CU1_INV_gate(circuit, qr, m, theta, starting_index + i, starting_index + j)

# WARNING: n always has to be even, because we are passing in it length of combined ancilla and target qubits
# (n/2 target and n/2 ancilla qubits). The only exception is, when n = 1, because this case does not need
# any ancilla qubits (it is just a NOT gate used at the lowest recursion level in the adder)
# Passed register has NOT INTERLEAVED ancilla and target qubits
def multiply_controlled_incrementer(circuit, qr, m, n, starting_index, controls):
    if n > 1 and (n % 2) != 0:
        raise Exception("Wrong number of qubits (n > 1 and ODD)")
    if n == 1:
        C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index)
    else:
        k = math.ceil(n / 2)
        qubit_map_inverse = {i: 2*i if i < k else 2*(i-k)+1 for i in range(n)} # We can use the same interleaving method as in the CARY_gate,
        qubit_map = {v: k for k, v in qubit_map_inverse.items()} # because the initial position of ancilla and target registers is switched
        # Initial gates (CNOTs and NOTs) before first subtraction widget
        for i in range(1, n - 2, 2):
            inner_controls = list(controls)
            inner_controls.append(starting_index + qubit_map[0])
            C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[i])
        for i in range(2, n - 1, 2):
            C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index + qubit_map[i])
        C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index + qubit_map[n - 1])
        # First Subtraction Widget
        for i in range(0, n - 3, 2):
            multiply_controlled_UMA_gate(circuit, qr, m, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], controls)
        inner_controls = list(controls)
        inner_controls.append(starting_index + qubit_map[n - 2])
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1])
        for i in reversed(range(0, n - 3, 2)):
            multiply_controlled_MAJ_gate(circuit, qr, m, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], controls)
        # Binary negation of one of the number, which we are subtracting (written on all ancilla qubits except the 0-th one)
        for i in range(2, n - 1, 2):
            C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index + qubit_map[i])
        # Second Subtraction Widget
        for i in range(0, n - 3, 2):
            multiply_controlled_UMA_gate(circuit, qr, m, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], controls)
        inner_controls = list(controls)
        inner_controls.append(starting_index + qubit_map[n - 2])
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1])
        for i in reversed(range(0, n - 3, 2)):
            multiply_controlled_MAJ_gate(circuit, qr, m, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], controls)
        # Last CNOT gates controlled on the 0-th qubit
        for i in range(1, n - 2, 2):
            inner_controls = list(controls)
            inner_controls.append(starting_index)
            C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[i])

# qr passed to this function does not have form with interleaved target and ancilla qubits,
# so we have to use mapping
# n - size of the register (it will contain n/2 target qubits and n/2 ancilla qubits)
# a - number, which will be added
# WARNING: We are not going to optimize the circuit on the highest (in the circuit)
# qubits, because of which we will be using n ancilla qubits instead of n - 1. Moreover,
# we need additional qubit to store the carry of the whole procedure, so in fact we will be using n + 1
# ancilla qubits. THE FINAL QUBIT, WHICH WILL STORE THE CARRY, HAS INDEX n!
# WARNING: n ALWAYS has to be even
def multiply_controlled_CARRY_gate(circuit, qr, m, n, starting_index, a, controls):
    n_target = math.ceil(n/2)
    print("CARRY, n: ", n, ", a: ", a, ", MAX: ", 2**n_target - 1)
    if a > (2**n_target - 1):
        raise Exception("Constant which is supposed to be added in CARRY gate cannot be written using so few bits")
    n_ancilla = math.ceil(n/2)
    k = n_target # We have to remember about the qubit, which will be carrying final carry
    qubit_map_inverse = {i: 2*i if i < k else 2*(i-k)+1 for i in range(n)}
    qubit_map = {v: k for k, v in qubit_map_inverse.items()}
    qubit_map[n] = n
    if a >= 0:
        a_binary = get_binary(a, n_target)
    else:
        a_binary = get_twos_complement_binary(-a, n_target)
    # First ascending gates
    inner_controls = list(controls)
    inner_controls.append(starting_index + qubit_map[n - 1])
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n])
    for (i, bit) in enumerate(a_binary):
        if i != (n_target - 1):
            if bit == '1':
                inner_controls = list(controls)
                inner_controls.append(starting_index + qubit_map[n - 2 - (2*i)])
                C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1 - (2*i)])
                C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index + qubit_map[n - 2 - (2*i)])
            inner_controls = list(controls)
            inner_controls.append(starting_index + qubit_map[n - 3 - (2*i)])
            inner_controls.append(starting_index + qubit_map[n - 2 - (2*i)])
            C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1 - (2*i)])
        else:
            if bit == '1':
                inner_controls = list(controls)
                inner_controls.append(starting_index + qubit_map[0])
                C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[1])
    for i in reversed(range(n_target - 1)):
        inner_controls = list(controls)
        inner_controls.append(starting_index + qubit_map[n - 3 - (2*i)])
        inner_controls.append(starting_index + qubit_map[n - 2 - (2*i)])
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1 - (2*i)])
    inner_controls = list(controls)
    inner_controls.append(starting_index + qubit_map[n - 1])
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n])
    for i in range(n_target - 1):
        inner_controls = list(controls)
        inner_controls.append(starting_index + qubit_map[n - 3 - (2*i)])
        inner_controls.append(starting_index + qubit_map[n - 2 - (2*i)])
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1 - (2*i)])
    for (i, bit) in enumerate(reversed(a_binary)):
        if i == 0:
            if bit == '1':
                inner_controls = list(controls)
                inner_controls.append(starting_index + qubit_map[0])
                C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[1])
        else:
            inner_controls = list(controls)
            inner_controls.append(starting_index + qubit_map[n - 3 - (2 * (n_target - i - 1))])
            inner_controls.append(starting_index + qubit_map[n - 2 - (2*(n_target - i - 1))])
            C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1 - (2*(n_target - i - 1))])
            if bit == '1':
                C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index + qubit_map[n - 2 - (2*(n_target - i - 1))])
                inner_controls = list(controls)
                inner_controls.append(starting_index + qubit_map[n - 2 - (2*(n_target - i - 1))])
                C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1 - (2*(n_target - i - 1))])

# a - constant to be added
# m - number of qubits in the whole circuit
# n - size of the whole register on which we will be carrying out adding. The n-th qubit is an ancilla qubit
# (it is the last qubit going into CARRY gate and control qubit for controlled increment gate further in the ADD_MOD fuction),
# so in fact we are using n+1 qubits in total
def multiply_controlled_ADD_gate(circuit, qr, m, n, starting_index, a, controls):
    if a > ((2**n) - 1):
        raise Exception("Constant which is supposed to be ADDed cannot be written using so few bits")
    if n == 1:
        if a == 1:
            C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index)
    else:
        if a >= 0:
            a_binary = get_binary(a, n)
        else:
            a_binary = get_twos_complement_binary(-a, n)
        low_bits_length = math.ceil(n/2)
        high_bits_length = math.floor(n/2)
        a_low_bits = a_binary[high_bits_length:n] # Numbers stored here and below are in reverse order (binarly)
        a_high_bits = a_binary[0:high_bits_length]
        a_low_int = int(a_low_bits, 2)
        a_high_int = int(a_high_bits, 2)
        inner_controls = list(controls)
        inner_controls.append(starting_index + n)
        multiply_controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, inner_controls)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            inner_controls = list(controls)
            inner_controls.append(starting_index + n)
            C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + low_bits_length + i)
        # CARRY-GATE
        multiply_controlled_CARRY_gate(circuit, qr, m, n, starting_index, a_low_int, controls)
        # CONTROLLED-INCREMENT-GATE
        inner_controls = list(controls)
        inner_controls.append(starting_index + n)
        multiply_controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, inner_controls)
        # CARRY-GATE
        multiply_controlled_CARRY_gate(circuit, qr, m, n, starting_index, a_low_int, controls)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            inner_controls = list(controls)
            inner_controls.append(starting_index + n)
            C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + low_bits_length + i)
        multiply_controlled_ADD_gate(circuit, qr, m, low_bits_length, starting_index, a_low_int, controls)
        multiply_controlled_ADD_gate(circuit, qr, m, high_bits_length, starting_index + low_bits_length, a_high_int, controls)

# This function is going to be launched only once (on the top level of the recursion)
def multiply_controlled_ADD_gate_2(circuit, qr, m, n, starting_index, a, controls):
    if a > ((2**n) - 1):
        raise Exception("Constant which is supposed to be ADDed cannot be written using so few bits")
    if a >= 0:
        a_binary = get_binary(a, n)
    else:
        a_binary = get_twos_complement_binary(-a, n)
    if n == 3:
        a_2 =  int(a_binary[1:], 2)
        multiply_controlled_ADD_gate(circuit, qr, m, 2, starting_index, a_2, controls)
    else:
        low_bits_length = math.ceil(n/2)
        high_bits_length = math.floor(n/2)
        a_low_bits = a_binary[high_bits_length:n] # Numbers stored here and below are in reverse order (binarly)
        a_high_bits = a_binary[1:high_bits_length]
        high_bits_length -= 1
        a_low_int = int(a_low_bits, 2)
        a_high_int = int(a_high_bits, 2)
        inner_controls = list(controls)
        inner_controls.append(starting_index + n)
        multiply_controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, inner_controls)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            inner_controls = list(controls)
            inner_controls.append(starting_index + n)
            C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + low_bits_length + i)
        # CARRY-GATE
        multiply_controlled_CARRY_gate(circuit, qr, m, n, starting_index, a_low_int, controls)
        # CONTROLLED-INCREMENT-GATE
        inner_controls = list(controls)
        inner_controls.append(starting_index + n)
        multiply_controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, inner_controls)
        # CARRY-GATE
        multiply_controlled_CARRY_gate(circuit, qr, m, n, starting_index, a_low_int, controls)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            inner_controls = list(controls)
            inner_controls.append(starting_index + n)
            C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + low_bits_length + i)
        multiply_controlled_ADD_gate(circuit, qr, m, low_bits_length, starting_index, a_low_int, controls)
        multiply_controlled_ADD_gate(circuit, qr, m, high_bits_length, starting_index + low_bits_length, a_high_int, controls)

# n - size of the whole register (ancilla and target qubits), has to be even. Target qubits are above the ancilla ones
def CMP_2_gate(circuit, qr, m, n, starting_index, a, controls):
    sub_register_length = math.ceil(n/3)
    if a >= 0:
        a_binary = get_binary(a, sub_register_length)
    else:
        a_binary = get_twos_complement_binary(-a, sub_register_length)
    target = sub_register_length - 1
    # Initial NOT gates
    for (i, bit) in enumerate(reversed(a_binary)):
        if bit == '1':
            NOT_gate(circuit, qr, m, starting_index + sub_register_length + i)
    inner_controls = list(controls)
    inner_controls.append(starting_index + 2*sub_register_length - 1)
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + target)
    # -----Toffoli and C^3 NOT gates-----
    # Ascending gates
    inner_controls = list(controls)
    inner_controls.append(starting_index)
    inner_controls.append(starting_index + sub_register_length)
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length)
    for i in range(1,sub_register_length - 1):
        inner_controls = list(controls)
        inner_controls.append(starting_index + i)
        inner_controls.append(starting_index + sub_register_length + i)
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length + i)
        inner_controls = list(controls)
        inner_controls.append(starting_index + 2*sub_register_length + i - 1)
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length + i)
    # Last CNOT gate
    inner_controls = list(controls)
    inner_controls.append(starting_index + 3*sub_register_length - 2)
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + target)
    # Descending gates
    for i in reversed(range(1,sub_register_length - 1)):
        inner_controls = list(controls)
        inner_controls.append(starting_index + 2*sub_register_length + i - 1)
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length + i)
        inner_controls = list(controls)
        inner_controls.append(starting_index + i)
        inner_controls.append(starting_index + sub_register_length + i)
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length + i)
    inner_controls = list(controls)
    inner_controls.append(starting_index)
    inner_controls.append(starting_index + sub_register_length)
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length)
    # -------------------------------
    # Initial NOT gates
    for (i, bit) in enumerate(reversed(a_binary)):
        if bit == '1':
            NOT_gate(circuit, qr, m, starting_index + sub_register_length + i)
    inner_controls = list(controls)
    inner_controls.append(starting_index + 2*sub_register_length - 1)
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + target)
    # -----Toffoli and C^3 NOT gates-----
    # Ascending gates
    inner_controls = list(controls)
    inner_controls.append(starting_index)
    inner_controls.append(starting_index + sub_register_length)
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length)
    for i in range(1,sub_register_length - 1):
        inner_controls = list(controls)
        inner_controls.append(starting_index + i)
        inner_controls.append(starting_index + sub_register_length + i)
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length + i)
        inner_controls = list(controls)
        inner_controls.append(starting_index + 2*sub_register_length + i - 1)
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length + i)
    # Last CNOT gate
    inner_controls = list(controls)
    inner_controls.append(starting_index + 3*sub_register_length - 2)
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + target)
    # Descending gates
    for i in reversed(range(1,sub_register_length - 1)):
        inner_controls = list(controls)
        inner_controls.append(starting_index + 2*sub_register_length + i - 1)
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length + i)
        inner_controls = list(controls)
        inner_controls.append(starting_index + i)
        inner_controls.append(starting_index + sub_register_length + i)
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length + i)
    inner_controls = list(controls)
    inner_controls.append(starting_index)
    inner_controls.append(starting_index + sub_register_length)
    C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + 2*sub_register_length)
    # -------------------------------

# # n - size of the whole register (it consists of n qubits for number 'b', n borrowed qubits,
# # 1 borrowed qubit (necessary for the ADD gates) and one more zeroed qubit, which
# # will be control qubit for controlled ADD gates)
# WARNING: n always has to be EVEN and be exactly equal to 2n + 2, where n is the number of bits, on which a
# twos complement representation of a number can be written
# This function uses one zeroed qubit instead of borrowed one.
# Moreover, it will use one qubit outside the register to compute the second CMP operation
def multiply_controlled_ADD_MOD_gate(circuit, qr, m, n, starting_index, a, N, controls):
    if (n % 2) != 0:
        raise Exception("Size of the register n cannot be ODD")
    # -------First CMP gate------
    multiply_controlled_CARRY_gate(circuit, qr, m, n - 2, starting_index, -(N - a), controls)
    SWAP_gate(circuit, qr, m, starting_index + (n - 2), starting_index + (n - 1))
    inner_controls = list(controls)
    inner_controls.append(starting_index + (n - 1))
    twos_complement_bin_length = len(bin(-(a-N))[2:]) + 2 
    multiply_controlled_ADD_gate_2(circuit, qr, m, twos_complement_bin_length, starting_index, a-N, inner_controls)
    NOT_gate(circuit, qr, m, starting_index + (n - 1))
    inner_controls = list(controls)
    inner_controls.append(starting_index + (n - 1))
    twos_complement_bin_length = len(bin(a)[2:]) + 2
    multiply_controlled_ADD_gate_2(circuit, qr, m, twos_complement_bin_length, starting_index, a, inner_controls)
    NOT_gate(circuit, qr, m, starting_index + (n - 1))
    # ------Second CMP(a) gate-------
    SWAP_gate(circuit, qr, m, starting_index + (n - 1), starting_index + math.ceil(n/2) - 1)
    CMP_2_gate(circuit, qr, m, n + math.ceil(n/2), starting_index, -a, controls)

# x - array containing qubits of number x (it will be exponent in a^x mod N)
def MULT_MOD(circuit, qr, m, n, starting_index, a, N, x, controls):
    x_len = len(x)
    for i in range(x_len):
        inner_controls = list(controls)
        inner_controls.append(x[i])
        multiply_controlled_ADD_MOD_gate(circuit, qr, m, n, starting_index, (a*(2**i)) % N, N, inner_controls)
    for i in range(x_len):
        SWAP_gate(circuit, qr, m, starting_index + i, x[i])
    a_inverse = 1
    while a_inverse < N and (a_inverse*a) % N != 1:
        a_inverse += 1
    a_inverse_neg = N - a_inverse
    for i in range(x_len):
        inner_controls = list(controls)
        inner_controls.append(x[i])
        multiply_controlled_ADD_MOD_gate(circuit, qr, m, n, starting_index, (a_inverse_neg*(2**i)) % N, N, inner_controls)

# x_1 and x_2 are arrays storing locations of x (it is stored in two registers)
def EXP_MOD(circuit, qr, m, n, starting_index, a, N, x_1, x_2):
    for i in range(len(x_2)):
        MULT_MOD(circuit, qr, m, n, starting_index, a**(2**i), N, x_1, [x_2[i]])

def continued_fractions(binary_result):
    m = len(binary_result)
    phi_approx = 0.0
    for (i, bit) in enumerate(reversed(binary_result)):
        if bit == '1':
            phi_approx += 2**i
    divider = 1
    if phi_approx > 0:
        while phi_approx > 1:
            phi_approx = phi_approx / 10 # We are multiplying by 10, because we don't want to loose places in vector for unnecessary zeros
            divider += 1
    phi_approx *= 10
    components = []
    for i in range(m):
        if i < m - 1:
            integer, fractional = divmod(phi_approx, 1)
            if integer > 0:
                components.append(int(integer))
                phi_approx -= integer
                if (trunc(phi_approx*(10**m))/(10**m))  > 0:
                    phi_approx = 1 / phi_approx
                else:
                    break
            else:
                phi_approx = int(phi_approx)
                components.append(phi_approx)
                break
        else:
            phi_approx = int(phi_approx)
            components.append(phi_approx)
    print(components)
    last_index = len(components) - 1
    while components[last_index] == 0 and last_index >= 0:
        last_index -= 1
    if last_index >= 0:
        buffer = Fraction(1, components[last_index])
        for (i, component) in enumerate(reversed(components[:last_index])):
            buffer = Fraction(component, 1) + buffer
            buffer = Fraction(1, 1) / buffer
        # buffer = Fraction(buffer.numerator * divider, buffer.denominator)
        buffer = Fraction(buffer.numerator * 10**(n - divider - 1), buffer.denominator)
        result = buffer.numerator
    else:
        result = 0
    return result

# n - number of qubits in register, which will be incremented
# m - total number of quibts used in the circuit
# WARNING: this function can be applied only to EVEN n and also n/2 must be even (it is caused by the contruction
# of the CARRY gate, which needs EVEN number of ancilla and target qubits)
def shor_quantum_subroutine(circuit, qr, cr, m, a, N):
    # n = len(bin(N)[2:])
    # print("n: ", n)
    # first_x_register_starting_index = (n + 2) * 3
    # second_x_register_starting_index = ((n + 2) * 3) + n
    # for i in range(n):
    #     Hadamard_gate(circuit, qr, m, first_x_register_starting_index + i)
    # for i in range(n):
    #     Hadamard_gate(circuit, qr, m, second_x_register_starting_index + i)
    # first_controls = []
    # for i in range(n):
    #     first_controls.append(first_x_register_starting_index + i)
    # second_controls = []
    # for i in range(n):
    #     second_controls.append(second_x_register_starting_index + i)
    # EXP_MOD(circuit, qr, m, (n + 2)*2, 0, a, N, first_controls, second_controls)
    # inv_qft(circuit, qr, m, n, second_x_register_starting_index)


    # a = 2
    # N = 3
    #---------------------------
    # NOT_gate(circuit, qr, m, 12)
    NOT_gate(circuit, qr, m, 13)
    # MULT_MOD(circuit, qr, m, 8, 0, a, N, [12, 13], [])
    # NOT_gate(circuit, qr, m, 14)
    NOT_gate(circuit, qr, m, 15)
    EXP_MOD(circuit, qr, m, 8, 0, a, N, [12, 13], [14, 15])
    # inv_qft(circuit, qr, m, 4, 0)

    # -------------Barrier before measurement------------
    circuit.barrier(qr)
    # measure
    for j in range(0, m):
        circuit.measure(qr[j], cr[j])
