import struct


class MD4:
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
        lbits, rbits = (val << n) & MD4.mask, val >> (MD4.width - n)
        return lbits | rbits

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

        # Split message into 512-bit chunks.
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
        for chunk in chunks:
            # fragments of an original message
            X = list(struct.unpack("<16I", chunk))
            # copy of initial words
            h = self.words.copy()

            # Round 1.
            Xi = [3, 7, 11, 19]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = n, Xi[n % 4]
                to_rotate = h[a] + MD4.F(h[b], h[c], h[d]) + X[K]
                h[a] = MD4.left_rotate(to_rotate & MD4.mask, S)

            # Round 2.
            Xi = [3, 5, 9, 13]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = n % 4 * 4 + n // 4, Xi[n % 4]
                to_rotate = h[a] + MD4.G(h[b], h[c], h[d]) + X[K] + 0x5A827999
                h[a] = MD4.left_rotate(to_rotate & MD4.mask, S)

            # Round 3.
            Xi = [3, 9, 11, 15]
            Ki = [0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = Ki[n], Xi[n % 4]
                to_rotate = h[a] + MD4.H(h[b], h[c], h[d]) + X[K] + 0x6ED9EBA1
                h[a] = MD4.left_rotate(to_rotate & MD4.mask, S)

            # Create the final message
            self.words = [((v + n) & MD4.mask) for v, n in zip(self.words, h)]



