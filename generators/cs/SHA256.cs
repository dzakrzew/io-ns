using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    public class SHA256
    {
        UInt32 h0 = 0x6a09e667;
        UInt32 h1 = 0xbb67ae85;
        UInt32 h2 = 0x3c6ef372;
        UInt32 h3 = 0xa54ff53a;
        UInt32 h4 = 0x510e527f;
        UInt32 h5 = 0x9b05688c;
        UInt32 h6 = 0x1f83d9ab;
        UInt32 h7 = 0x5be0cd19;

        UInt32[] K = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };


        private UInt32 ROTR(UInt32 word, int bits)
        {
            return (((word) >> (bits)) | ((word) << (32 - (bits))));
        }

        private UInt32 MOD(UInt32 x)
        {
            return ((x) & 0xFFFFFFFF);
        }

        private UInt32 SSIG0(UInt32 x)
        {
            return (ROTR(x, 7) ^ ROTR(x, 18) ^ ((x) >> 3));
        }

        private UInt32 SSIG1(UInt32 x)
        {
            return (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10));
        }

        private UInt32 BSIG0(UInt32 x)
        {
            return (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22));
        }

        private UInt32 BSIG1(UInt32 x)
        {
            return (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25));
        }

        private UInt32 CH(UInt32 x, UInt32 y, UInt32 z)
        {
            return (((x) & (y)) ^ (~(x) & (z)));
        }
        
        private UInt32 MAJ(UInt32 x, UInt32 y, UInt32 z)
        {
            return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
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
                UInt32[] W = new UInt32[64];

                for (int t = 0; t <= 15; t++)
                {
                    W[t] = MOD(blocks[bi][t]);
                }

                for (int t = 16; t < 64; t++)
                {
                    W[t] = MOD(SSIG1(W[t - 2]) + W[t - 7] + SSIG0(W[t - 15]) + W[t - 16]);
                }

                UInt32 a = h0;
                UInt32 b = h1;
                UInt32 c = h2;
                UInt32 d = h3;
                UInt32 e = h4;
                UInt32 f = h5;
                UInt32 g = h6;
                UInt32 h = h7;
                UInt32 temp1;
                UInt32 temp2;

                for (int t = 0; t < 64; t++)
                {
                    temp1 = MOD(h + BSIG1(e) + CH(e, f, g) + K[t] + W[t]);
                    temp2 = MOD(BSIG0(a) + MAJ(a, b, c));
                    h = g;
                    g = f;
                    f = e;
                    e = MOD(d + temp1);
                    d = c;
                    c = b;
                    b = a;
                    a = MOD(temp1 + temp2);
                }

                h0 = MOD(h0 + a);
                h1 = MOD(h1 + b);
                h2 = MOD(h2 + c);
                h3 = MOD(h3 + d);
                h4 = MOD(h4 + e);
                h5 = MOD(h5 + f);
                h6 = MOD(h6 + g);
                h7 = MOD(h7 + h);
            }

            string result = h0.ToString("x4") + h1.ToString("x4") + h2.ToString("x4") + h3.ToString("x4")
                + h4.ToString("x4") + h5.ToString("x4") + h6.ToString("x4") + h7.ToString("x4");
            
            return result;
        }
    }
}
