class SHA512:

    @staticmethod
    def digest(input_data):
        K = [
            0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
            0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
            0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
            0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
            0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
            0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
            0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
            0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
            0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
            0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
            0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
            0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
            0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
            0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
            0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
            0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
            0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
            0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
            0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
            0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817,
        ]

        h0 = 0x6a09e667f3bcc908
        h1 = 0xbb67ae8584caa73b
        h2 = 0x3c6ef372fe94f82b
        h3 = 0xa54ff53a5f1d36f1
        h4 = 0x510e527fade682d1
        h5 = 0x9b05688c2b3e6c1f
        h6 = 0x1f83d9abfb41bd6b
        h7 = 0x5be0cd19137e2179

        def ROTR(num: int, shift: int, size: int = 64):
            return (num >> shift) | (num << size - shift)

        def SSIG0(num: int):
            num = (ROTR(num, 1) ^ ROTR(num, 8) ^ (num >> 7))
            return num

        def SSIG1(num: int):
            num = (ROTR(num, 19) ^ ROTR(num, 61) ^ (num >> 6))
            return num

        def BSIG0(num: int):
            num = (ROTR(num, 28) ^ ROTR(num, 34) ^ ROTR(num, 39))
            return num

        def BSIG1(num: int):
            num = (ROTR(num, 14) ^ ROTR(num, 18) ^ ROTR(num, 41))
            return num

        def CH(x: int, y: int, z: int):
            return (x & y) ^ (~x & z)

        def MAJ(x: int, y: int, z: int):
            return (x & y) ^ (x & z) ^ (y & z)

        # Step 1: Convert to binary
        message = bytearray(input_data)

        length = len(message) * 8 # number of bytes in message

        message.append(0x80) # append 10000000

        while (len(message) * 8 + 128) % 1024 != 0:
            message.append(0x00) # append zeros to get multiply of 1024

        message += length.to_bytes(16, 'big') # append the initial length of message

        blocks = [] # 1024-bits = 128 bytes chunks of message
        for i in range(0, len(message), 128):
            blocks.append(message[i:i+128])

        for message_block in blocks:
            message_schedule = []

            for t in range(0, 80):
                if t <= 15:
                    message_schedule.append(bytes(message_block[t*8 : (t*8) + 8]))
                else:
                    term1 = SSIG1(int.from_bytes(message_schedule[t-2], 'big'))
                    term2 = int.from_bytes(message_schedule[t-7], 'big')
                    term3 = SSIG0(int.from_bytes(message_schedule[t-15], 'big'))
                    term4 = int.from_bytes(message_schedule[t-16], 'big')

                    schedule = ((term1 + term2 + term3 + term4) % (2**64)).to_bytes(8, 'big')
                    message_schedule.append(schedule)

            a = h0
            b = h1
            c = h2
            d = h3
            e = h4
            f = h5
            g = h6
            h = h7

            for t in range(80):
                t1 = ((h + BSIG1(e) + CH(e, f, g) + K[t] + int.from_bytes(message_schedule[t], 'big')) % 2**64)
                t2 = (BSIG0(a) + MAJ(a, b, c)) % 2**64
                h = g
                g = f
                f = e
                e = (d + t1) % 2**64
                d = c
                c = b
                b = a
                a = (t1 + t2) % 2**64

            h0 = (h0 + a) % 2**64
            h1 = (h1 + b) % 2**64
            h2 = (h2 + c) % 2**64
            h3 = (h3 + d) % 2**64
            h4 = (h4 + e) % 2**64
            h5 = (h5 + f) % 2**64
            h6 = (h6 + g) % 2**64
            h7 = (h7 + h) % 2**64

        result = h0.to_bytes(8, 'big') + h1.to_bytes(8, 'big') + h2.to_bytes(8, 'big') + h3.to_bytes(8, 'big') + h4.to_bytes(8, 'big') + h5.to_bytes(8, 'big') + h6.to_bytes(8, 'big') + h7.to_bytes(8, 'big')

        return result.hex()