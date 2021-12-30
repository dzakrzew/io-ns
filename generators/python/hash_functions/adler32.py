class Adler32:
    @staticmethod
    def digest(input_data):
        message = bytearray(input_data)

        MOD_ADLER = 65521
        a = 1
        b = 0
        
        for i in range(len(message)):
            a = (a + message[i]) % MOD_ADLER
            b = (b + a) % MOD_ADLER

        result = (b << 16) | a
        return hex(result)[2:]