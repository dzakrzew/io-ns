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
        # Pre-processing: Total length is a multiple of 512 bits.
        # length in bytes
        length = len(input_file) * 8

        input_file += b"\x80"
        input_file += b"\x00" * (-(len(input_file) + 8) % 64)
        input_file += struct.pack("<Q", length)

        # Split message into 512-bit chunks.
        message_chunks = []
        for i in range(0, len(input_file), 64):
            message_chunks.append(input_file[i: i + 64])
        self._process(message_chunks)

    def bytes(self):
        # return final hash as bytes
        return struct.pack("<4L", *self.words)

    def hexdigest(self):
        # return final hash as hexdigest
        return "".join(f"{value:02x}" for value in self.bytes())

    def _process(self, chunks):
        for chunk in chunks:
            X, h = list(struct.unpack("<16I", chunk)), self.words.copy()

            # Round 1.
            Xi = [3, 7, 11, 19]
            for n in range(16):
                i, j, k, l = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = n, Xi[n % 4]
                hn = h[i] + MD4.F(h[j], h[k], h[l]) + X[K]
                h[i] = MD4.left_rotate(hn & MD4.mask, S)

            # Round 2.
            Xi = [3, 5, 9, 13]
            for n in range(16):
                i, j, k, l = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = n % 4 * 4 + n // 4, Xi[n % 4]
                hn = h[i] + MD4.G(h[j], h[k], h[l]) + X[K] + 0x5a827999
                h[i] = MD4.left_rotate(hn & MD4.mask, S)

            # Round 3.
            Xi = [3, 9, 11, 15]
            Ki = [0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15]
            for n in range(16):
                i, j, k, l = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = Ki[n], Xi[n % 4]
                hn = h[i] + MD4.H(h[j], h[k], h[l]) + X[K] + 0x6ed9eba1
                h[i] = MD4.left_rotate(hn & MD4.mask, S)

            self.words = [((v + n) & MD4.mask) for v, n in zip(self.words, h)]



