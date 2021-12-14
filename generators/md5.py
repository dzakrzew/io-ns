import struct


class MD5:
    @staticmethod
    def F(x, y, z):
        return (x & y) | (~x & z)

    @staticmethod
    def G(x, y, z):
        return (x & y) | (x & z) | (y & z)

    @staticmethod
    def H(x, y, z):
        return x ^ y ^ z

    @staticmethod
    def left_rotate(val, n):
        lbits, rbits = (val << n) & MD5.mask, val >> (MD5.width - n)
        return lbits | rbits



    width = 32
    mask = 0xFFFFFFFF
    words = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476]

    def __init__(self, input_file=None):
        length = len(input_file) + 8

        # Padding is performed as follows: a single "1" bit is appended to the
        # message, and then "0" bits are appended so that the length in bits of
        # the padded message becomes congruent to 448, modulo 512. In all, at
        # least one bit and at most 512 bits are appended.
        input_file += b"\x80"
        input_file += b"\x00" * (-(len(input_file) + 8) % 64)
        input_file += struct.pack("<Q", length)

        # split message into chunks of 512 bits
        message_chunks = []
        for i in range(0, len(input_file), 64):
            message_chunks.append(input_file[i: i + 64])

        self._process(message_chunks)

    def _process(self, chunks):
        for chunk in chunks:
            X, h = list(struct.unpack("<16I", chunk)), self.words.copy()

            # Round 1.
            Xi = [7, 12, 17, 22]
            for n in range(16):
                i, j, k, l = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, W = n, Xi[n % 4], n+1
                print(K, S, W)
                hn = h[i] + MD5.F(h[j], h[k], h[l]) + X[K]
                h[i] = MD5.left_rotate(hn & MD5.mask, S)

            # Round 2.
            Xi = [5, 9, 14, 20]


            # Round 3.
            Xi = [4, 11, 16, 23]


            # Round 4.
            Xi = [6, 10, 15, 21]