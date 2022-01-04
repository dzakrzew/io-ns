using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    class Adler32
    {
        public const int MOD_ADLER = 65521;

        public string Digest(string input)
        {
            List<UInt32> message = new List<UInt32>();

            for (int i = 0; i < input.Length; i++)
            {
                message.Add(Convert.ToUInt32(input[i]));
            }

            UInt32 length = (UInt32)message.Count;

            UInt32 a = 1;
            UInt32 b = 0;

            for (int i = 0; i < length; i++)
            {
                a = (a + message[i]) % MOD_ADLER;
                b = (b + a) % MOD_ADLER;
            }

            return (((b << 16) | a)).ToString("x4");
        }
    }
}
