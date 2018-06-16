# import QFT
import math

def get_binary(number, n):
    number_bin = bin(number)[2:]
    l = len(number_bin)
    while l < n:
        number_bin = '0' + number_bin
        l += 1
    return number_bin

def get_reversed_binary(number, n):
    number_bin = get_binary(number, n)
    number_bin_reversed = ''
    for i in reversed(range(n)):
        number_bin_reversed += number_bin[i]
    return number_bin_reversed

def reversed_binary_to_int(bits):
    reversed_binary = ''
    for i in reversed(range(len(bits))):
        reversed_binary += bits[i]
    return int(reversed_binary, 2)

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
def controlled_incrementer(circuit, qr, m, n, starting_index, external_control):
    print("INCREMENTER, n: ", n)
    if n == 1:
        print("Weszło")
        print("control: ", external_control, ", target: ", starting_index)
        CNOT_gate(circuit, qr, m, external_control, starting_index)
    else:
        k = math.ceil(n / 2)
        qubit_map_inverse = {i: 2*i if i < k else 2*(i-k)+1 for i in range(n)} # We can use the same interleaving method as in the CARY_gate,
        qubit_map = {v: k for k, v in qubit_map_inverse.items()} # because the initial position of ancilla and target registers is switched
        # Initial gates (CNOTs and NOTs) before first subtraction widget
        for i in range(1, n - 2, 2):
            extended_CNOT_gate(circuit, qr, m, starting_index + qubit_map[0], starting_index + qubit_map[i], external_control)
        for i in range(2, n - 1, 2):
            extended_NOT_gate(circuit, qr, m, starting_index + qubit_map[i], external_control)
        extended_NOT_gate(circuit, qr, m, starting_index + qubit_map[n - 1], external_control)
        # First Subtraction Widget
        for i in range(0, n - 3, 2):
            extended_UMA_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
        extended_CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2], starting_index + qubit_map[n - 1], external_control)
        for i in reversed(range(0, n - 3, 2)):
            extended_MAJ_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
        # Binary negation of one of the number, which we are subtracting (written on all ancilla qubits except the 0-th one)
        for i in range(2, n - 1, 2):
            extended_NOT_gate(circuit, qr, m, starting_index + qubit_map[i], external_control)
        # Second Subtraction Widget
        for i in range(0, n - 3, 2):
            extended_UMA_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
        extended_CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2], starting_index + qubit_map[n - 1], external_control)
        for i in reversed(range(0, n - 3, 2)):
            extended_MAJ_gate(circuit, qr, m, n, starting_index, starting_index + qubit_map[i], starting_index + qubit_map[i + 1], starting_index + qubit_map[i + 2], external_control)
        # Last CNOT gates controlled on the 0-th qubit
        for i in range(1, n - 2, 2):
            extended_CNOT_gate(circuit, qr, m, starting_index, starting_index + qubit_map[i], external_control)

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
        if i != (n_target - 1):
            if bit == '1':
                CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2 - (2*i)], starting_index + qubit_map[n - 1 - (2*i)])
                NOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2 - (2*i)])
            Toffoli_gate(circuit, qr, m, starting_index + qubit_map[n - 3 - (2*i)], starting_index + qubit_map[n - 2 - (2*i)], starting_index + qubit_map[n - 1 - (2*i)])
        else:
            if bit == '1':
                CNOT_gate(circuit, qr, m, starting_index + qubit_map[n - 2 - (2*i)], starting_index + qubit_map[n - 1 - (2*i)])
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
    print("n: ", n, ", starting_index: ", starting_index)
    if n == 1:
        if a == 1:
            NOT_gate(circuit, qr, m, starting_index)
        print("Koniec rekurencji")
    else:
        a_length = math.ceil(n/2)
        a_binary = get_reversed_binary(a, a_length)
        low_bits_length = math.ceil(a_length/2)
        high_bits_length = math.floor(a_length/2)
        a_low_bits = a_binary[0:low_bits_length] # Numbers stored here and below are in reverse order (binarly)
        a_high_bits = a_binary[low_bits_length:n]
        a_low_int = reversed_binary_to_int(a_low_bits)
        a_high_int = reversed_binary_to_int(a_high_bits)
        print("a_binary: ", a_binary)
        print("low_bits_length: ", low_bits_length)
        print("high_bits_length: ", high_bits_length)
        print("a_low_bits: ", a_low_bits)
        print("a_high_bits: ", a_high_bits)
        print("a_low: ", a_low_int)
        print("a_high", a_high_int)
        # CONTROLLED-INCREMENT-GATE
        controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, starting_index + n)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            CNOT_gate(circuit, qr, m, starting_index + n, starting_index + low_bits_length + i)
        # CARRY-GATE
        CARRY_gate(circuit, qr, m, n, starting_index, a)
        # CONTROLLED-INCREMENT-GATE
        controlled_incrementer(circuit, qr, m, high_bits_length, starting_index + low_bits_length, starting_index + n)
        # CARRY-GATE
        CARRY_gate(circuit, qr, m, n, starting_index, a)
        # MULTIPLE-CNOT-GATE
        for i in range(high_bits_length):
            CNOT_gate(circuit, qr, m, starting_index + n, starting_index + low_bits_length + i)
        ADD_gate(circuit, qr, m, low_bits_length, starting_index, a_low_int)
        ADD_gate(circuit, qr, m, high_bits_length, starting_index + low_bits_length, a_high_int)


# n - number of qubits in register, which will be incremented
# m - total number of quibts used in the circuit
def shor(circuit, qr, cr, m, n, a):
    a = 3
    # incrementer(circuit, qr, n)
    # NOT_gate(circuit, qr, m, 5)
    # NOT_gate(circuit, qr, m, 0)
    # controlled_incrementer(circuit, qr, m, 1, 0, 1)
    # controlled_incrementer(circuit, qr, m, 4, 1, 5)
    # controlled_incrementer(circuit, qr, m, 4, 1, 5)
    # controlled_incrementer(circuit, qr, m, 4, 1, 5)
    # controlled_incrementer(circuit, qr, m, 4, 0, 4)
    # NOT_gate(circuit, qr, m, 0)
    # NOT_gate(circuit, qr, m, 1)
    # NOT_gate(circuit, qr, m, 2)
    # CARRY_gate(circuit, qr, m, 4, 0, a)
    ADD_gate(circuit, qr, m, 4, 0, a)
    # -------------Barrier before measurement------------
    circuit.barrier(qr)
    # measure
    for j in range(0, m):
        circuit.measure(qr[j], cr[j])
