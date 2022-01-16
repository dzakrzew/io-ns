using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    public class CRC16 : IHashFunction
    {
        public override string Digest(byte[] input)
        {
            List<UInt16> message = new List<UInt16>();

            for (int i = 0; i < input.Length; i++)
            {
                message.Add(Convert.ToUInt16(input[i]));
            }

            UInt16 crc = 0xFFFF;

            for (int i = 0; i < message.Count; i++)
            {
                UInt16 ch = message[i];

                for (int j = 0; j < 8; j++) {
                    UInt16 b = (UInt16)((ch ^ crc) & 1);

                    crc >>= 1;

                    if (b == 1) {
                        crc = (UInt16)(crc ^ 0x8408);
                    }

                    ch >>= 1;
                }
            }

            string res = (~crc).ToString("x4");

            return res.Substring(res.Length - 4);
        }
    }
}
