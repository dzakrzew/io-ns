using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    class Blake2b : IHashFunction
    {
        //Init variables
        List<UInt32> output = new List<UInt32>();
        byte[] b = new byte[128];
        UInt64[] h = new UInt64[8];
        UInt64[] t = new UInt64[2];
        UInt64 c;
        int outlen = 64;
        UInt64[] v = new UInt64[16];

        UInt64[] blake2b_iv = {
            0x6A09E667F3BCC908, 0xBB67AE8584CAA73B,
           0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
           0x510E527FADE682D1, 0x9B05688C2B3E6C1F,
           0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179
        };

        UInt64 Rotr64(UInt64 x, int y)
        {
            UInt64 temp = (x >> y) ^ (x << (64 - y));
            return temp;
        }

        UInt64 Get64(byte b0, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7)
        {
            UInt64 temp = 0;
            temp = (UInt64)b0;
            temp ^= (UInt64)b1 << 8;
            temp ^= (UInt64)b2 << 16;
            temp ^= (UInt64)b3 << 24;
            temp ^= (UInt64)b4 << 32;
            temp ^= (UInt64)b5 << 40;
            temp ^= (UInt64)b6 << 48;
            temp ^= (UInt64)b7 << 56;

            return temp;
        }

        void G(UInt64 a, UInt64 b, UInt64 c, UInt64 d, UInt64 x, UInt64 y)
        {
            v[a] = v[a] + v[b] + x;
            v[d] = Rotr64((v[d] ^ v[a]), 32);
            v[c] = v[c] + v[d];
            v[b] = Rotr64((v[b] ^ v[c]), 24);
            v[a] = v[a] + v[b] + y;
            v[d] = Rotr64((v[d] ^ v[a]), 16);
            v[c] = v[c] + v[d];
            v[b] = Rotr64((v[b] ^ v[c]), 63);
        }

        void compress(int last)
        {
            int[,] sigma = new int[12, 16] {
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
                { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
                { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
                { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
                { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
                { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
                { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
                { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
                { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
                { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
                { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }
            };

            UInt64[] m = new UInt64[16];

            for (int i = 0; i < 8; i++)
            {
                v[i] = h[i];
                v[i + 8] = blake2b_iv[i];
            }

            v[12] ^= t[0];
            v[13] ^= t[1];
            if (last != 0)
                v[14] = ~v[14];

            for (int i = 0; i < 16; i++)
            {
                m[i] = Get64(b[i * 8 + 0], b[i * 8 + 1], b[i * 8 + 2], b[i * 8 + 3],
                            b[i * 8 + 4], b[i * 8 + 5], b[i * 8 + 6], b[i * 8 + 7]);
            }

            for (int i = 0; i < 12; i++)
            {
                G(0, 4, 8, 12, m[sigma[i, 0]], m[sigma[i, 1]]);
                G(1, 5, 9, 13, m[sigma[i, 2]], m[sigma[i, 3]]);
                G(2, 6, 10, 14, m[sigma[i, 4]], m[sigma[i, 5]]);
                G(3, 7, 11, 15, m[sigma[i, 6]], m[sigma[i, 7]]);
                G(0, 5, 10, 15, m[sigma[i, 8]], m[sigma[i, 9]]);
                G(1, 6, 11, 12, m[sigma[i, 10]], m[sigma[i, 11]]);
                G(2, 7, 8, 13, m[sigma[i, 12]], m[sigma[i, 13]]);
                G(3, 4, 9, 14, m[sigma[i, 14]], m[sigma[i, 15]]);
            }

            for (int i = 0; i < 8; ++i)
            {
                h[i] ^= v[i] ^ v[i + 8];
            }
        }

        void update(byte[] chunks)
        {

            for (int i = 0; i < chunks.Length; i++)
            {
                if (c == 128)
                {
                    t[0] += c;
                    if (t[0] < c)
                        t[1]++;
                    compress(0);
                    c = 0;
                }
                b[c++] = (byte)chunks[i];
            }
        }

        int init(byte[] key)
        {
            int keylen = key.Length;
            if (outlen == 0 || outlen > 64 || keylen > 64)
                return -1;

            for (int i = 0; i < 8; i++)
                h[i] = blake2b_iv[i];
            ulong temp = ((ulong)(0x01010000 ^ (keylen << 8) ^ outlen));
            h[0] = h[0] ^ temp;

            t[0] = 0;
            t[1] = 0;
            c = 0;

            for (int i = keylen; i < 128; i++)
                b[i] = 0;
            if (keylen > 0)
            {
                update(key);
                c = 128;
            }

            return 0;
        }

        void finalHash()
        {
            t[0] += c;
            if (t[0] < c)
                t[1]++;

            while (c < 128)
                b[c++] = 0;
            compress(1);

            // little endian convert and store
            for (int i = 0; i < outlen; i++)
                output.Add((uint)((h[i >> 3] >> (8 * (i & 7))) & 0xFF));

        }

        public override string Digest(byte[] message)
        {
            init(new byte[] { });
            update(message);
            finalHash();

            string s = "";
            foreach (UInt32 o in output) {
                if (o.ToString("x").Length < 2)
                    s += "0";
                s += o.ToString("x");
            }

            return s;
        }
    }
}
