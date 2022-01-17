import struct
import math


class MD5:
    @staticmethod
    def digest(input_file):

        def F(x, y, z):
            return (x & y) | (~x & z)

        def G(x, y, z):
            return (z & x) | (~z & y)

        def H(x, y, z):
            return x ^ y ^ z

        def I(x, y, z):
            return y ^ (x | ~z)

        def left_rotate(val, n):
            val &= 0xFFFFFFFF
            return (val << n | val >> (32 - n)) & mask

        def bytes():
            # return final hash as bytes
            return struct.pack("<4L", *words)

        width = 32
        mask = 0xFFFFFFFF
        words = [0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476]


        length = len(input_file) * 8
        input_file += b"\x80"
        input_file += b"\x00" * (-(len(input_file) + 8) % 64)
        # 448 bits + padding = 512 bits
        input_file += struct.pack("<Q", length)

        # split message into chunks of 512 bits
        message_chunks = []
        for i in range(0, len(input_file), 64):
            message_chunks.append(input_file[i: i + 64])

        constants = [int(abs(math.sin(i + 1)) * 4294967296) & 0xFFFFFFFF for i in range(64)]

        for chunk in message_chunks:
            # fragments of an original message
            X = list(struct.unpack("<16I", chunk))
            # copy of initial words
            h = words.copy()

            # Round 1.
            Xi = [7, 12, 17, 22]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, T = n, Xi[n % 4], n
                FF = F(h[b], h[c], h[d])
                to_rotate = h[a] + FF + X[K] + constants[T]
                h[a] = (h[b] + left_rotate(to_rotate, S)) & mask

            # Round 2.
            Xi = [5, 9, 14, 20]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, T = (5 * n + 1) % 16, Xi[n % 4], n + 16
                GG = G(h[b], h[c], h[d])
                to_rotate = h[a] + GG + X[K] + constants[T]
                h[a] = (h[b] + left_rotate(to_rotate, S)) & mask

            # Round 3.
            Xi = [4, 11, 16, 23]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, T = (3 * n + 5) % 16, Xi[n % 4], n + 32
                HH = H(h[b], h[c], h[d])
                to_rotate = h[a] + HH + X[K] + constants[T]
                h[a] = (h[b] + left_rotate(to_rotate, S)) & mask

            # Round 4.
            Xi = [6, 10, 15, 21]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S, T = (7 * n) % 16, Xi[n % 4], n + 48
                II = I(h[b], h[c], h[d])
                to_rotate = h[a] + II + X[K] + constants[T]
                h[a] = (h[b] + left_rotate(to_rotate, S)) & mask

            # Create the final message
            words = [((v + n) & mask) for v, n in zip(words, h)]

        return "".join(f"{value:02x}" for value in bytes())






