#!/usr/bin/python

import sys

# ----------------------------------------------------------------------------
# Crypto4o functions start here
# ----------------------------------------------------------------------------

class GLFSR:
    def __init__(self, polynom, initial_value):
        print "using polynom 0x%X, initial value: 0x%X." % (polynom, initial_value)

        self.polynom = polynom | 1 # | is binary OR
        self.data = initial_value
        tmp = polynom

        self.mask = 1

        while tmp != 0:
            if tmp & self.mask != 0: # & is binary AND (if tmp and mask are not the same number)
                tmp ^= self.mask; # XOR tmp with mask

            if tmp == 0:
                break

            self.mask <<= 1

    def next_state(self):
        self.data <<= 1

        retval = 0

        if self.data & self.mask != 0:
            retval = 1
            self.data ^= self.polynom

        print retval
        return retval


class SPRNG:
    def __init__(self, polynom_d, init_value_d, polynom_c, init_value_c):
        print "GLFSR D0: ",
        self.glfsr_d = GLFSR(polynom_d, init_value_d)
        print "GLFSR C0: ",
        self.glfsr_c = GLFSR(polynom_c, init_value_c)

    def next_byte(self):
        byte = 0
        bitpos = 7

        while True:
            bit_d = self.glfsr_d.next_state()
            bit_c = self.glfsr_c.next_state()

            if bit_c != 0:
                bit_r = bit_d
                byte |= bit_r << bitpos

                bitpos -= - 1

                if bitpos < 0:
                    break

        return byte


# ----------------------------------------------------------------------------
# Crypto4o functions end here
# ----------------------------------------------------------------------------

def main():
    prng = SPRNG(int(sys.argv[3], 16), int(sys.argv[4], 16),
                 int(sys.argv[5], 16), int(sys.argv[6], 16))

    with open(sys.argv[1], "rb") as f, open(sys.argv[2], "wb") as g:
        while True:
            input_ch = f.read(1)
    
            if input_ch == "":
                break
    
            random_ch = prng.next_byte() & 0xff
            g.write(chr(ord(input_ch) ^ random_ch))


if __name__ == '__main__':
    main()
