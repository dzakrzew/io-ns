class SHA1:

    @staticmethod
    def digest(input_data):

        def f(t, B: int, C: int, D: int):
            if 0 <= t and t <= 19:
                return ((B & C) | ((~B) & D)) % 2**32
            elif 20 <= t and t <= 39:
                return (B ^ C ^ D) % 2**32
            elif 40 <= t and t <= 59:
                return ((B & C) | (B & D) | (C & D)) % 2**32
            elif 60 <= t and t <= 79:
                return (B ^ C ^ D) % 2**32
        
        def S(n: int, X: int):
            return ((X << n) | (X >> 32 - n)) % 2 ** 32

        def K(t):
            if 0 <= t and t <= 19:
                return 0x5A827999
            elif 20 <= t and t <= 39:
                return 0x6ED9EBA1
            elif 40 <= t and t <= 59:
                return 0x8F1BBCDC
            elif 60 <= t and t <= 79:
                return 0xCA62C1D6
        

        message = bytearray(input_data)
        length = len(message) * 8 # number of bytes in message
        message.append(0x80) # append 10000000

        while (len(message) * 8 + 64) % 512 != 0:
            message.append(0x00) # append zeros to get multiply of 512

        message += length.to_bytes(8, 'big') # append the initial length of message

        blocks = []

        for i in range(0, len(message), 64):
            blocks.append(message[i:i+64])
        
        h0 = 0x67452301
        h1 = 0xEFCDAB89
        h2 = 0x98BADCFE
        h3 = 0x10325476
        h4 = 0xC3D2E1F0

        for block in blocks:
            w = []
            d = int(len(block) / 16)

            for i in range(0, len(block), d):
                w.append(block[i : i + d])

            for t in range(16, 80):
                xa = int.from_bytes(w[t-3], 'big')
                xb = int.from_bytes(w[t-8], 'big')
                xc = int.from_bytes(w[t-14], 'big')
                xd = int.from_bytes(w[t-16], 'big')
                w.append(S(1, (xa ^ xb ^ xc ^ xd) % 2**32).to_bytes(4, 'big'))
            
            A = h0
            B = h1
            C = h2
            D = h3
            E = h4

            TEMP = None

            for t in range(0, 80):
                TEMP = (S(5, A) + f(t, B, C, D) + E + int.from_bytes(w[t], 'big') + K(t)) % 2**32
                E = D
                D = C
                C = S(30, B)
                B = A
                A = TEMP

            h0 = (h0 + A) % 2**32
            h1 = (h1 + B) % 2**32
            h2 = (h2 + C) % 2**32
            h3 = (h3 + D) % 2**32
            h4 = (h4 + E) % 2**32

        result = h0.to_bytes(4, 'big') + h1.to_bytes(4, 'big') + h2.to_bytes(4, 'big') + h3.to_bytes(4, 'big') + h4.to_bytes(4, 'big')
        return result.hex()
