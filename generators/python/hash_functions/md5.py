import struct
import math


class MD5:
    @staticmethod
    def F(x, y, z):
        return (x & y) | (~x & z)

    @staticmethod
    def G(x, y, z):
        return (z & x) | (~z & y)

    @staticmethod
    def H(x, y, z):
        return x ^ y ^ z

    @staticmethod
    def I(x, y, z):
        return y ^ (x | ~z)

    @staticmethod
    def left_rotate(val, n):
        val &= 0xFFFFFFFF
        return (val << n | val >> (32 - n)) & MD5.mask

    width = 32
    mask = 0xFFFFFFFF
    words = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476]

    def __init__(self, input_file=None):
        # Padding is performed as follows: a single "1" bit is appended to the
        # message, and then "0" bits are appended so that the length in bits of
        # the padded message becomes congruent to 448, modulo 512. In all, at
        # least one bit and at most 512 bits are appended.
        length = len(input_file) * 8
        input_file += b"\x80"
        input_file += b"\x00" * (-(len(input_file) + 8) % 64)
        # 448 bits + padding = 512 bits
        input_file += struct.pack("<Q", length)

        # split message into chunks of 512 bits
        message_chunks = []
        for i in range(0, len(input_file), 64):
            message_chunks.append(input_file[i: i + 64])
        self.process(message_chunks)

    def bytes(self):
        # return final hash as bytes
        return struct.pack("<4L", *self.words)

    def hexdigest(self):
        # return final hash as hexdigest
        return "".join(f"{value:02x}" for value in self.bytes())

    def process(self, chunks):
        constants = [int(abs(math.sin(i + 1)) * 4294967296) & 0xFFFFFFFF for i in range(64)]

        for chunk in chunks:
            # fragments of an original message
            X = list(struct.unpack("<16I", chunk))
            # copy of initial words
            h = self.words.copy()

            # Round 1.
            Xi = [7, 12, 17, 22]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, T = n, Xi[n % 4], n
                F = MD5.F(h[b], h[c], h[d])
                to_rotate = h[a] + F + X[K] + constants[T]
                h[a] = (h[b] + self.left_rotate(to_rotate, S)) & self.mask

            # Round 2.
            Xi = [5, 9, 14, 20]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, T = (5 * n + 1) % 16, Xi[n % 4], n+16
                G = MD5.G(h[b], h[c], h[d])
                to_rotate = h[a] + G + X[K] + constants[T]
                h[a] = (h[b] + self.left_rotate(to_rotate, S)) & self.mask

            # Round 3.
            Xi = [4, 11, 16, 23]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, T = (3 * n + 5) % 16, Xi[n % 4], n+32
                H = MD5.H(h[b], h[c], h[d])
                to_rotate = h[a] + H + X[K] + constants[T]
                h[a] = (h[b] + self.left_rotate(to_rotate, S)) & self.mask

            # Round 4.
            Xi = [6, 10, 15, 21]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, T = (7 * n) % 16, Xi[n % 4], n+48
                I = MD5.I(h[b], h[c], h[d])
                to_rotate = h[a] + I + X[K] + constants[T]
                h[a] = (h[b] + self.left_rotate(to_rotate, S)) & self.mask

            # Create the final message
            self.words = [((v + n) & MD5.mask) for v, n in zip(self.words, h)]

