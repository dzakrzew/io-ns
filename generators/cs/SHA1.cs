using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    public class SHA1
    {
        UInt32 h0 = 0x67452301;
        UInt32 h1 = 0xEFCDAB89;
        UInt32 h2 = 0x98BADCFE;
        UInt32 h3 = 0x10325476;
        UInt32 h4 = 0xC3D2E1F0;
        
        private UInt32 MOD(UInt32 x)
        {
            return ((x) & 0xFFFFFFFF);
        }

        private UInt32 f(int t, UInt32 B, UInt32 C, UInt32 D)
        {
            if (0 <= t && t <= 19)
            {
                return MOD((B & C) | ((~B) & D));
            }
            else if (20 <= t && t <= 39)
            {
                return MOD(B ^ C ^ D);
            }
            else if (40 <= t && t <= 59)
            {
                return MOD((B & C) | (B & D) | (C & D));
            }
            else
            {
                return MOD(B ^ C ^ D);
            }
        }

        private UInt32 S(int n, UInt32 X)
        {
            return MOD((X << n) | (X >> (32 - n)));
        }

        private UInt32 K(int t)
        {
            if (0 <= t && t <= 19)
            {
                return 0x5A827999;
            }
            else if (20 <= t && t <= 39)
            {
                return 0x6ED9EBA1;
            }
            else if (40 <= t && t <= 59)
            {
                return 0x8F1BBCDC;
            }
            else
            {
                return 0xCA62C1D6;
            }
        }

        public string Digest(string input)
        {
            List<UInt32> message = new List<UInt32>();

            for (int i = 0; i < input.Length; i++)
            {
                message.Add(Convert.ToUInt32(input[i]));
            }

            UInt32 length = (UInt32)message.Count * 8;

            message.Add(0x80);

            while ((message.Count * 8 + 64) % 512 != 0)
            {
                message.Add(0x00);
            }

            message.Add(0x00);
            message.Add(0x00);
            message.Add(0x00);
            message.Add(0x00);
            message.Add(length >> 24);
            message.Add(length >> 16);
            message.Add(length >> 8);
            message.Add(length);

            List<List<UInt32>> blocks = new List<List<UInt32>>();

            for (int i = 0; i < message.Count * 8 / 512; i++)
            {
                List<UInt32> vtemp = new List<UInt32>();

                for (int j = 0; j < 64; j++)
                {
                    vtemp.Add(message[(i * 64) + j]);
                }

                UInt32[] output = new UInt32[16];

                for (int j = 0; j < 64; j = j + 4)
                {
                    UInt32 temp = 0x00;

                    temp = (UInt32)vtemp[j] << 24;
                    temp |= (UInt32)vtemp[j + 1] << 16;
                    temp |= (UInt32)vtemp[j + 2] << 8;
                    temp |= (UInt32)vtemp[j + 3];

                    output[j / 4] = (UInt32)temp;
                }

                blocks.Add(new List<UInt32>(output));
            }

            for (int bi = 0; bi < blocks.Count; bi++)
            {
                UInt32[] W = new UInt32[80];

                for (int t = 0; t <= 15; t++)
                {
                    W[t] = MOD(blocks[bi][t]);
                }

                for (int t = 16; t < 80; t++)
                {
                    W[t] = S(1, MOD(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]));
                }

                UInt32 a = h0;
                UInt32 b = h1;
                UInt32 c = h2;
                UInt32 d = h3;
                UInt32 e = h4;
                UInt32 temp;

                for (int t = 0; t < 80; t++)
                {
                    temp = MOD(S(5, a) + f(t, b, c, d) + e + W[t] + K(t));
                    e = d;
                    d = c;
                    c = S(30, b);
                    b = a;
                    a = temp;
                }

                h0 = MOD(h0 + a);
                h1 = MOD(h1 + b);
                h2 = MOD(h2 + c);
                h3 = MOD(h3 + d);
                h4 = MOD(h4 + e);
            }

            string result = h0.ToString("x4") + h1.ToString("x4") + h2.ToString("x4") + h3.ToString("x4") + h4.ToString("x4");

            return result;
        }
    }
}
