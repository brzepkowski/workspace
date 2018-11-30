# import QFT
import math

def get_binary(number, n):
    number_bin = bin(number)[2:]
    l = len(number_bin)
    while l < n:
        number_bin = '0' + number_bin
        l += 1
    return number_bin

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

# In fact, we will implement MAJ in reverse order, because we want to subtract instead of add
def MAJ_gate(circuit, qr, n, top_qubit, middle_qubit, bottom_qubit):
    Toffoli_gate(circuit, qr, n, top_qubit, middle_qubit, bottom_qubit)
    CNOT_gate(circuit, qr, n, bottom_qubit, top_qubit)
    CNOT_gate(circuit, qr, n, bottom_qubit, middle_qubit)

# In fact, we will implement UMA in reverse order, because we want to subtract instead of add
def UMA_gate(circuit, qr, n, top_qubit, middle_qubit, bottom_qubit):
    CNOT_gate(circuit, qr, n, top_qubit, middle_qubit)
    CNOT_gate(circuit, qr, n, bottom_qubit, top_qubit)
    Toffoli_gate(circuit, qr, n, top_qubit, middle_qubit, bottom_qubit)

# We are using this name to indicate, that initially it was just a CNOT gate,
# but controlled-incrementer needs to add another control qubit to each gate,
# so in fact this gate becomes a Toffoli gate.
# We also leave the notation n, but it will now keep the information about how
# many qubits a circuit has in total (not only how big the register, on which we
# are performing incrementation, is)
# m - total number of qubits in the whole circuit (not only in the incremented register)
def extended_CNOT_gate(circuit, qr, m, control, target, external_control):
    circuit.ccx(qr[control], qr[external_control], qr[target])
    for k in range(m):
        if k != control and k != external_control and k != target:
            circuit.iden(qr[k])

def extended_NOT_gate(circuit, qr, m, target, external_control):
    circuit.cx(qr[external_control], qr[target])
    for k in range(m):
        if k != external_control and k != target:
            circuit.iden(qr[k])

# starting_index is needed, because we are using the last qubit in the register
def extended_Toffoli_gate(circuit, qr, m, n, starting_index, control1, control2, target, external_control):
    Toffoli_gate(circuit, qr, m, control2, starting_index + n - 1, target)
    Toffoli_gate(circuit, qr, m, control1, external_control, starting_index + n - 1)
    Toffoli_gate(circuit, qr, m, control2, starting_index + n - 1, target)
    Toffoli_gate(circuit, qr, m, control1, external_control, starting_index + n - 1)

def extended_MAJ_gate(circuit, qr, m, n, starting_index, top_qubit, middle_qubit, bottom_qubit, external_control):
    extended_Toffoli_gate(circuit, qr, m, n, starting_index, top_qubit, middle_qubit, bottom_qubit, external_control)
    extended_CNOT_gate(circuit, qr, m, bottom_qubit, top_qubit, external_control)
    extended_CNOT_gate(circuit, qr, m, bottom_qubit, middle_qubit, external_control)

def extended_UMA_gate(circuit, qr, m, n, starting_index, top_qubit, middle_qubit, bottom_qubit, external_control):
    extended_CNOT_gate(circuit, qr, m, top_qubit, middle_qubit, external_control)
    extended_CNOT_gate(circuit, qr, m, bottom_qubit, top_qubit, external_control)
    extended_Toffoli_gate(circuit, qr, m, n, starting_index, top_qubit, middle_qubit, bottom_qubit, external_control)

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

# indices of control_qubits and terget_qubit have to be precise (not mapped or with additional starting point)
# n = length of controls array can be arbitrary (for n = 0 we add NOT gate, n = 1, CNOT, n = 2 Toffoli and so on)
def C_n_controlled_NOT_gate(circuit, qr, m, controls, target): # NOT gate with n controls
    n = len(controls)
    if n == 0:
        NOT_gate(circuit, qr, m, target)
    if n == 1:
        CNOT_gate(circuit, qr, m, controls[0], target)
    if n == 2:
        Toffoli_gate(circuit, qr, m, controls[0], controls[1], target)
    else:
        if n - 1 > math.floor((m-1)/2):
            raise Exception("Too many controls in C^n NOT gate for such circuit")
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

# qr - quantum register. It has to interleave qubits, on which we want to act with the ancilla qubits
# e.g. [qr[0], qr[2], qr[4], ...] - ancilla (borrowed) qubits, [qr[1], qr[3], qr[5], ...] - actual qubits,
# which we want to increment
# n - total number of qubits in register, which we will be incrementing (combined ancilla
# and actual qubits), always will be even, because we are using n/2 ancilla qubits (half in total)
# TODO możliwe że trzeba będzie przekazać też całkowitą liczbę qubitów (nie tylko n),
# TODO żeby dodać IDENTITY gates na całej szerokości obwodu
def incrementer(circuit, qr, n):
    # Initial gates (CNOTs and NOTs) before first subtraction widget
    for i in range(1, n - 2, 2):
        CNOT_gate(circuit, qr, n, 0, i)
    for i in range(2, n - 1, 2):
        NOT_gate(circuit, qr, n, i)
    NOT_gate(circuit, qr, n, n - 1)
    # First Subtraction Widget
    for i in range(0, n - 3, 2):
        UMA_gate(circuit, qr, n, i, i + 1, i + 2)
    CNOT_gate(circuit, qr, n, n - 2, n - 1)
    for i in reversed(range(0, n - 3, 2)):
        MAJ_gate(circuit, qr, n, i, i + 1, i + 2)
    # Binary negation of one of the number, which we are subtracting (written on all ancilla qubits except the 0-th one)
    for i in range(2, n - 1, 2):
        NOT_gate(circuit, qr, n, i)
    # Second Subtraction Widget
    for i in range(0, n - 3, 2):
        UMA_gate(circuit, qr, n, i, i + 1, i + 2)
    CNOT_gate(circuit, qr, n, n - 2, n - 1)
    for i in reversed(range(0, n - 3, 2)):
        MAJ_gate(circuit, qr, n, i, i + 1, i + 2)
    # Last CNOT gates controlled on the 0-th qubit
    for i in range(1, n - 2, 2):
        CNOT_gate(circuit, qr, n, 0, i)

# TODO: STARTING POINT!!!
# external_control will be an exact index of control qubit, from which action of the whole gate will
# def controlled_incrementer(circuit, qr, m, n, starting_index, external_control):
#     if n == 1:
#         CNOT_gate(circuit, qr, m, external_control, starting_index)
#     else:
#         k = math.ceil(n / 2)
#         qubit_map_inverse = {i: 2*i if i < k else 2*(i-k)+1 for i in range(n)} # We can use the same interleaving method as in the CARY_gate,
#         qubit_map = {v: k for k, v in qubit_map_inverse.items()} # because the initial position of ancilla and target registers is switched
#         # Initial gates (CNOTs and NOTs) before first subtraction widget
#         for i in range(1, n - 2, 2):
#             extended_CNOT_gate(circuit, qr, m, starting_index + qubit_map[0], starting_index + qubit_map[i], external_control)
#         for i in range(2, n - 1, 2):
#             extended_NOT_gate(circuit, qr, m, starting_index + qubit_map[i], external_control)
#         extended_NOT_gate(circuit, qr, m, starting_index + qubit_map[n - 1], external_control)
#         # First Subtraction Widget
#         for i in range(0, n - 3, 2):
#             extended_UMA_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
#         extended_CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2], starting_index + qubit_map[n - 1], external_control)
#         for i in reversed(range(0, n - 3, 2)):
#             extended_MAJ_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
#         # Binary negation of one of the number, which we are subtracting (written on all ancilla qubits except the 0-th one)
#         for i in range(2, n - 1, 2):
#             extended_NOT_gate(circuit, qr, m, starting_index + qubit_map[i], external_control)
#         # Second Subtraction Widget
#         for i in range(0, n - 3, 2):
#             extended_UMA_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
#         extended_CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2], starting_index + qubit_map[n - 1], external_control)
#         for i in reversed(range(0, n - 3, 2)):
#             extended_MAJ_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
#         # Last CNOT gates controlled on the 0-th qubit
#         for i in range(1, n - 2, 2):
#             extended_CNOT_gate(circuit, qr, m, starting_index, starting_index + qubit_map[i], external_control)

# n always has to be even, because we are passing in it length of combined ancilla and target qubits
# (n/2 target and n/2 ancilla qubits). The only exception is, when n = 1, because this case does not need
# any ancilla qubits (it is just a NOT gate used at the lowest recursion level in the adder)
def controlled_incrementer(circuit, qr, m, n, starting_index, external_control):
    if n > 1 and (n % 2) != 0:
        raise Exception("Wrong number of qubits (n > 1 and ODD)")
    if n == 1:
        CNOT_gate(circuit, qr, m, external_control, starting_index)
    else:
        k = math.ceil(n / 2)
        qubit_map_inverse = {i: 2*i if i < k else 2*(i-k)+1 for i in range(n)} # We can use the same interleaving method as in the CARY_gate,
        qubit_map = {v: k for k, v in qubit_map_inverse.items()} # because the initial position of ancilla and target registers is switched
        # Initial gates (CNOTs and NOTs) before first subtraction widget
        for i in range(1, n - 2, 2):
            Toffoli_gate(circuit, qr, m, starting_index + qubit_map[0], external_control, starting_index + qubit_map[i])
        for i in range(2, n - 1, 2):
            CNOT_gate(circuit, qr, m, external_control, starting_index + qubit_map[i])
        CNOT_gate(circuit, qr, m, external_control, starting_index + qubit_map[n - 1])
        # First Subtraction Widget
        for i in range(0, n - 3, 2):
            extended_UMA_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
        Toffoli_gate(circuit, qr, m, starting_index + qubit_map[n - 2], external_control, starting_index + qubit_map[n - 1])
        for i in reversed(range(0, n - 3, 2)):
            extended_MAJ_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
        # Binary negation of one of the number, which we are subtracting (written on all ancilla qubits except the 0-th one)
        for i in range(2, n - 1, 2):
            CNOT_gate(circuit, qr, m, external_control, starting_index + qubit_map[i])
        # Second Subtraction Widget
        for i in range(0, n - 3, 2):
            extended_UMA_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
        Toffoli_gate(circuit, qr, m, starting_index + qubit_map[n - 2], external_control, starting_index + qubit_map[n - 1])
        for i in reversed(range(0, n - 3, 2)):
            extended_MAJ_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
        # Last CNOT gates controlled on the 0-th qubit
        for i in range(1, n - 2, 2):
            Toffoli_gate(circuit, qr, m, starting_index, external_control, starting_index + qubit_map[i])

# 'controls' has to be at lest 2 elements long
def multiply_controlled_incrementer(circuit, qr, m, n, starting_index, controls):
    if n == 1:
        C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index) # <---
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
        C_n_controlled_NOT_gate(circuit, qr, m, controls, starting_index + qubit_map[n - 1]) #<----
        # First Subtraction Widget
        for i in range(0, n - 3, 2):
            # WARNING: in multiply controlled MAJ and UMA functions controls land at the last position
            multiply_controlled_UMA_gate(circuit, qr, m, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], controls)
        inner_controls = list(controls)
        inner_controls.append(starting_index + qubit_map[n - 2])
        C_n_controlled_NOT_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[n - 1]) # <-----
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
            Toffoli_gate(circuit, qr, m, inner_controls, starting_index + qubit_map[i])

# qr passed to this function does not have form with interleaved target and ancilla qubits,
# so we have to use mapping
# n - size of the register (it will contain n/2 target qubits and n/2 ancilla qubits)
# a - number, which will be added
# TODO: STARTING POINT!!!
# WARNING: We are not going to optimize the circuit on the highest (in the circuit)
# qubits, because of which we will be using n ancilla qubits instead of n - 1. Moreover,
# we need additional qubit to store the carry of the whole procedure, so in fact we will be using n + 1
# ancilla qubits. THE FINAL QUBIT, WHICH WILL STORE THE CARRY, HAS INDEX n!
# WARNING: n ALWAYS has to be even
def CARRY_gate(circuit, qr, m, n, starting_index, a):
    n_target = math.ceil(n/2)
    print("CARRY, n = ", n, ", a: ", a, ", MAX: ", 2**n_target - 1)
    if a > (2**n_target - 1):
        raise Exception("Constant which is supposed to be added in CARRY gate cannot be written using so few bits")
    n_ancilla = math.ceil(n/2)
    k = n_target # We have to remember about the qubit, which will be carrying final carry
    qubit_map_inverse = {i: 2*i if i < k else 2*(i-k)+1 for i in range(n)}
    qubit_map = {v: k for k, v in qubit_map_inverse.items()}
    qubit_map[n] = n
    # print(qubit_map)
    a_binary = get_binary(a, n_target)
    # First ascending gates
    CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 1], starting_index + qubit_map[n])
    for (i, bit) in enumerate(a_binary):
        # print("i: ", i, ", bit: ", bit)
        if i != (n_target - 1):
            if bit == '1':
                CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2 - (2*i)], starting_index + qubit_map[n - 1 - (2*i)])
                NOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2 - (2*i)])
            Toffoli_gate(circuit, qr, m, starting_index + qubit_map[n - 3 - (2*i)], starting_index + qubit_map[n - 2 - (2*i)], starting_index + qubit_map[n - 1 - (2*i)])
        else:
            if bit == '1':
                # CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2 - (2*i)], starting_index + qubit_map[n - 1 - (2*i)])
                CNOT_gate(circuit, qr, m, starting_index + qubit_map[0], starting_index + qubit_map[1])
    for i in reversed(range(n_target - 1)):
        Toffoli_gate(circuit, qr, m, starting_index + qubit_map[n - 3 - (2*i)], starting_index + qubit_map[n - 2 - (2*i)], starting_index + qubit_map[n - 1 - (2*i)])
    CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 1], starting_index + qubit_map[n])
    for i in range(n_target - 1):
        Toffoli_gate(circuit, qr, m, starting_index + qubit_map[n - 3 - (2*i)], starting_index + qubit_map[n - 2 - (2*i)], starting_index + qubit_map[n - 1 - (2*i)])
    for (i, bit) in enumerate(reversed(a_binary)):
        if i == 0:
            if bit == '1':
                CNOT_gate(circuit, qr, m, starting_index + qubit_map[0], starting_index + qubit_map[1])
        else:
            Toffoli_gate(circuit, qr, m, starting_index + qubit_map[n - 3 - (2 * (n_target - i - 1))], starting_index + qubit_map[n - 2 - (2*(n_target - i - 1))], starting_index + qubit_map[n - 1 - (2*(n_target - i - 1))])
            if bit == '1':
                NOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2 - (2*(n_target - i - 1))])
                CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2 - (2*(n_target - i - 1))], starting_index + qubit_map[n - 1 - (2*(n_target - i - 1))])

# a - constant to be added
# m - number of qubits in the whole circuit
# n - size of the whole register on which we will be carrying out adding. The n-th qubit is an ancilla qubit
# (it is the last qubit going into CARRY gate and control qubit for controlled increment gate)
def ADD_gate(circuit, qr, m, n, starting_index, a):
    # print("ADD - n: ", n, ", starting_index: ", starting_index)
    if n == 1:
        if a == 1:
            NOT_gate(circuit, qr, m, starting_index)
        # print("Koniec rekurencji")
    else:
        a_binary = get_binary(a, n)
        low_bits_length = math.ceil(n/2)
        high_bits_length = math.floor(n/2)
        a_low_bits = a_binary[high_bits_length:n] # Numbers stored here and below are in reverse order (binarly)
        a_high_bits = a_binary[0:high_bits_length]
        a_low_int = int(a_low_bits, 2)
        a_high_int = int(a_high_bits, 2)
        # print("a_binary: ", a_binary)
        # print("low_bits_length: ", low_bits_length)
        # print("high_bits_length: ", high_bits_length)
        # print("a_low_bits: ", a_low_bits)
        # print("a_high_bits: ", a_high_bits)
        # print("a_low: ", a_low_int)
        # print("a_high", a_high_int)
        # CONTROLLED-INCREMENT-GATE
        controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, starting_index + n)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            CNOT_gate(circuit, qr, m, starting_index + n, starting_index + low_bits_length + i)
        # CARRY-GATE
        CARRY_gate(circuit, qr, m, n, starting_index, a_low_int)
        # CONTROLLED-INCREMENT-GATE
        controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, starting_index + n)
        # CARRY-GATE
        CARRY_gate(circuit, qr, m, n, starting_index, a_low_int)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            CNOT_gate(circuit, qr, m, starting_index + n, starting_index + low_bits_length + i)
        ADD_gate(circuit, qr, m, low_bits_length, starting_index, a_low_int)
        ADD_gate(circuit, qr, m, high_bits_length, starting_index + low_bits_length, a_high_int)

def controlled_ADD_gate(circuit, qr, m, n, starting_index, a, external_control):
    # print("ADD - n: ", n, ", starting_index: ", starting_index)
    if n == 1:
        if a == 1:
            CNOT_gate(circuit, qr, m, external_control, starting_index) # <-------------------------
    else:
        a_binary = get_binary(a, n)
        low_bits_length = math.ceil(n/2)
        high_bits_length = math.floor(n/2)
        a_low_bits = a_binary[high_bits_length:n] # Numbers stored here and below are in reverse order (binarly)
        a_high_bits = a_binary[0:high_bits_length]
        a_low_int = int(a_low_bits, 2)
        a_high_int = int(a_high_bits, 2)
        multiply_controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, [starting_index + n, external_control])
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            CNOT_gate(circuit, qr, m, starting_index + n, starting_index + low_bits_length + i) # <--- !!!!Teraz tu trzbe zmienić!!!!
        # CARRY-GATE
        CARRY_gate(circuit, qr, m, n, starting_index, a_low_int)
        # CONTROLLED-INCREMENT-GATE
        controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, starting_index + n)
        # CARRY-GATE
        CARRY_gate(circuit, qr, m, n, starting_index, a_low_int)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            CNOT_gate(circuit, qr, m, starting_index + n, starting_index + low_bits_length + i)
        ADD_gate(circuit, qr, m, low_bits_length, starting_index, a_low_int)
        ADD_gate(circuit, qr, m, high_bits_length, starting_index + low_bits_length, a_high_int)

# We need to give length of the desired binary string at least one longer than the number of
# bits necessary to write thoe numbers (because we need a sign bit)
def binarly_subtract(a, b, n):
    a_bin = get_binary(a, n)
    b_bin = get_binary(b, n)
    b_bin_neg = ''
    for i in range(n):
        if b_bin[i] == '0':
            b_bin_neg += '1'
        else:
            b_bin_neg += '0'
    b_neg = int(b_bin_neg, 2) + 1
    a_sub_b = a + b_neg
    a_sub_b_bin = get_binary(a_sub_b, n) # It is possible, that number obtained is larger than n (because inside we are firstly running bin(x))
    return a_sub_b_bin[len(a_sub_b_bin) - n:]

# Here size of the register n must be ODD (we need the same amount of target and ancilla qubits,
# which gives EVEN number and one additional garbage qubit, which gives ODD number)
def ADD_MOD_gate(circuit, qr, m, n, starting_index, a, N):
    N_sub_a = int(binarly_subtract(N, a, math.ceil((n-1)/2)), 2) # In this and below case n must be in fact one digit larger than
    a_sub_N = int(binarly_subtract(a, N, math.ceil((n-1)/2)), 2) # the number of bits necessary to store values of N and a (because we need additional sing bit)
    # COMPARE with N - a (made out of CARRY gate)
    print("N - a: ", N_sub_a)
    print("a - N: ", a_sub_N)
    SWAP_gate(circuit, qr, m, starting_index + (n - 2), starting_index + n)
    CARRY_gate(circuit, qr, m, n - 2, starting_index, N_sub_a) # We are giving n-2 as argument, because we are subtracting
    # garbage qubit (it has index n, so it is kind like outside this register) and one qubit in each register (for sing values), because these cary information about sign, which is not
    # necessary in this CMP gate
    SWAP_gate(circuit, qr, m, starting_index + (n - 2), starting_index + n)
    # Add a or Sub N - a (which is equal to Ad a or Add a - N)
    # ADD_gate(circuit, qr, m, n, starting_index, a)
    # NOT_gate(circuit, qr, m, starting_index + n)
    # ADD_gate(circuit, qr, m, n, starting_index, a_sub_N)
    # NOT_gate(circuit, qr, m, starting_index + n)
    # # COMPARE with a (made out of CARRY gate)
    # SWAP_gate(circuit, qr, m, starting_index + (n - 2), starting_index + n)
    # CARRY_gate(circuit, qr, m, n - 2, starting_index, N_sub_a)
    # SWAP_gate(circuit, qr, m, starting_index + (n - 2), starting_index + n)

# n - number of qubits in register, which will be incremented
# m - total number of quibts used in the circuit
# WARNING: this function can be applied only to EVEN n and also n/2 must be even (it is caused by the contruction
# of the CARRY gate, which needs EVEN number of ancilla and target qubits)
def shor(circuit, qr, cr, m, n, a):
    a = 2
    N = 3
    # incrementer(circuit, qr, 3)
    # incrementer(circuit, qr, 3)
    # incrementer(circuit, qr, 3)
    # incrementer(circuit, qr, 3)
    # incrementer(circuit, qr, 3)
    # NOT_gate(circuit, qr, m, 5)
    # NOT_gate(circuit, qr, m, 0)
    # controlled_incrementer(circuit, qr, m, 3, 1, 0)
    # controlled_incrementer(circuit, qr, m, 4, 1, 0)
    # controlled_incrementer(circuit, qr, m, 4, 1, 0)
    # controlled_incrementer(circuit, qr, m, 4, 1, 0)
    # controlled_incrementer(circuit, qr, m, 4, 1, 5)
    # controlled_incrementer(circuit, qr, m, 4, 1, 5)
    # controlled_incrementer(circuit, qr, m, 4, 0, 4)
    # NOT_gate(circuit, qr, m, 0)
    # NOT_gate(circuit, qr, m, 1) # <-----
    # NOT_gate(circuit, qr, m, 2)
    # NOT_gate(circuit, qr, m, 3)
    # CARRY_gate(circuit, qr, m, 4, 0, a)
    # ADD_gate(circuit, qr, m, m - 1, 0, a)
    # ADD_MOD_gate(circuit, qr, m, m - 1, 0, a, N) # <------
    # C_n_controlled_NOT_gate(circuit, qr, m, [0, 1], 5)
    # -------------Barrier before measurement------------
    circuit.barrier(qr)
    # measure
    for j in range(0, m):
        circuit.measure(qr[j], cr[j])

