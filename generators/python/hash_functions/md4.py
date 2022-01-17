import struct


class MD4:
    @staticmethod
    def digest(input_file):
        # input_file = input_data

        def F(x, y, z):
            return (x & y) | (~x & z)

        def G(x, y, z):
            return (x & y) | (x & z) | (y & z)

        def H(x, y, z):
            return x ^ y ^ z

        def left_rotate(val, n):
            lbits, rbits = (val << n) & mask, val >> (width - n)
            return lbits | rbits

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

        # Split message into 512-bit chunks.
        message_chunks = []
        for i in range(0, len(input_file), 64):
            message_chunks.append(input_file[i: i + 64])

        for chunk in message_chunks:
            # fragments of an original message
            X = list(struct.unpack("<16I", chunk))
            # copy of initial words
            h = words.copy()

            # Round 1.
            Xi = [3, 7, 11, 19]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = n, Xi[n % 4]
                to_rotate = h[a] + F(h[b], h[c], h[d]) + X[K]
                h[a] = left_rotate(to_rotate & mask, S)

            # Round 2.
            Xi = [3, 5, 9, 13]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = n % 4 * 4 + n // 4, Xi[n % 4]
                to_rotate = h[a] + G(h[b], h[c], h[d]) + X[K] + 0x5A827999
                h[a] = left_rotate(to_rotate & mask, S)

            # Round 3.
            Xi = [3, 9, 11, 15]
            Ki = [0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15]
            for n in range(16):
                a, b, c, d = map(lambda x: x % 4, range(-n, -n + 4))
                K, S = Ki[n], Xi[n % 4]
                to_rotate = h[a] + H(h[b], h[c], h[d]) + X[K] + 0x6ED9EBA1
                h[a] = left_rotate(to_rotate & mask, S)

            # Create the final message
            words = [((v + n) & mask) for v, n in zip(words, h)]

        # return hash
        return "".join(f"{value:02x}" for value in bytes())