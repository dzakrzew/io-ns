using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    public class CRC32 : IHashFunction
    {
        public override string Digest(byte[] input)
        {
            List<UInt32> message = new List<UInt32>();

            for (int i = 0; i < input.Length; i++)
            {
                message.Add(Convert.ToUInt32(input[i]));
            }

            UInt32 crc = 0xFFFFFFFF;

            for (int i = 0; i < message.Count; i++)
            {
                UInt32 ch = message[i];

                for (int j = 0; j < 8; j++) {
                    UInt32 b = (ch ^ crc) & 1;

                    crc >>= 1;

                    if (b == 1) {
                        crc = crc ^ 0xEDB88320;
                    }

                    ch >>= 1;
                }
            }

            return (~crc).ToString("x8").PadLeft(8, '0');
        }
    }
}
