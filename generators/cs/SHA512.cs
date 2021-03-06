using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    public class SHA512 : IHashFunction
    {
        UInt64 h0 = 0x6a09e667f3bcc908;
        UInt64 h1 = 0xbb67ae8584caa73b;
        UInt64 h2 = 0x3c6ef372fe94f82b;
        UInt64 h3 = 0xa54ff53a5f1d36f1;
        UInt64 h4 = 0x510e527fade682d1;
        UInt64 h5 = 0x9b05688c2b3e6c1f;
        UInt64 h6 = 0x1f83d9abfb41bd6b;
        UInt64 h7 = 0x5be0cd19137e2179;

        UInt64[] K = {
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
        };

        private UInt64 ROTR(UInt64 word, int bits)
        {
            return (((word) >> (bits)) | ((word) << (64 - (bits))));
        }

        private UInt64 MOD(UInt64 x)
        {
            return ((x) & 0xFFFFFFFFFFFFFFFF);
        }

        private UInt64 SSIG0(UInt64 x)
        {
            return (ROTR(x, 1) ^ ROTR(x, 8) ^ ((x) >> 7));
        }

        private UInt64 SSIG1(UInt64 x)
        {
            return (ROTR(x, 19) ^ ROTR(x, 61) ^ ((x) >> 6));
        }

        private UInt64 BSIG0(UInt64 x)
        {
            return (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39));
        }

        private UInt64 BSIG1(UInt64 x)
        {
            return (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41));
        }

        private UInt64 CH(UInt64 x, UInt64 y, UInt64 z)
        {
            return (((x) & (y)) ^ (~(x) & (z)));
        }

        private UInt64 MAJ(UInt64 x, UInt64 y, UInt64 z)
        {
            return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
        }

        private string hex(UInt64 n) {
            return n.ToString("x8").PadLeft(16, '0');
        }

        public override string Digest(byte[] input)
        {
            List<UInt64> message = new List<UInt64>();

            for (int i = 0; i < input.Length; i++)
            {
                message.Add(Convert.ToUInt64(input[i]));
            }

            UInt64 length = (UInt64)message.Count * 8;

            message.Add(0x80);

            while ((message.Count * 8 + 128) % 1024 != 0)
            {
                message.Add(0x00);
            }
            
            for (int i = 0; i < 8; i++)
            {
                message.Add(0x00);
            }

            message.Add(length >> 56);
            message.Add(length >> 48);
            message.Add(length >> 40);
            message.Add(length >> 32);
            message.Add(length >> 24);
            message.Add(length >> 16);
            message.Add(length >> 8);
            message.Add(length);

            List<List<UInt64>> blocks = new List<List<UInt64>>();

            for (int i = 0; i < message.Count * 8 / 1024; i++)
            {
                List<UInt64> vtemp = new List<UInt64>();

                for (int j = 0; j < 128; j++)
                {
                    vtemp.Add(message[(i * 128) + j]);
                }

                UInt64[] output = new UInt64[16];

                for (int j = 0; j < 128; j = j + 8)
                {
                    UInt64 temp = 0x00;

                    temp = (UInt64)vtemp[j] << 56;
                    temp |= (UInt64)vtemp[j + 1] << 48;
                    temp |= (UInt64)vtemp[j + 2] << 40;
                    temp |= (UInt64)vtemp[j + 3] << 32;
                    temp |= (UInt64)vtemp[j + 4] << 24;
                    temp |= (UInt64)vtemp[j + 5] << 16;
                    temp |= (UInt64)vtemp[j + 6] << 8;
                    temp |= (UInt64)vtemp[j + 7];

                    output[j / 8] = (UInt64)temp;
                }

                blocks.Add(new List<UInt64>(output));
            }

            for (int bi = 0; bi < blocks.Count; bi++)
            {
                UInt64[] W = new UInt64[80];

                for (int t = 0; t <= 15; t++)
                {
                    W[t] = MOD(blocks[bi][t]);
                }

                for (int t = 16; t < 80; t++)
                {
                    W[t] = MOD(SSIG1(W[t - 2]) + W[t - 7] + SSIG0(W[t - 15]) + W[t - 16]);
                }

                UInt64 a = h0;
                UInt64 b = h1;
                UInt64 c = h2;
                UInt64 d = h3;
                UInt64 e = h4;
                UInt64 f = h5;
                UInt64 g = h6;
                UInt64 h = h7;
                UInt64 temp1;
                UInt64 temp2;

                for (int t = 0; t < 80; t++)
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

            string result = this.hex(h0) + this.hex(h1) + this.hex(h2) + this.hex(h3)
                + this.hex(h4) + this.hex(h5) + this.hex(h6) + this.hex(h7);

            return result;
        }
    }
}