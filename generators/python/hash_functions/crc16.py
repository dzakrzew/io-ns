class CRC16:

    @staticmethod
    def digest(input_data):
        message = bytearray(input_data)
        crc = (0xFFFF)

        for i in range(len(message)):
            ch = int(message[i])

            for j in range(8):
                b = (ch ^ crc) & 1
                crc = crc >> 1
                if b:
                    crc = crc ^ 0x8408
                ch = ch >> 1

        return hex((1 << 16) - 1 - crc)[2:]