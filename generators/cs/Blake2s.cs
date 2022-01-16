using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    class Blake2s {
        //Init variables
        List<UInt32> output = new List<UInt32>();
        byte[] b = new byte[64];
        UInt32[] h = new UInt32[8];
        UInt32[] t = new UInt32[2];
        UInt32 c;
        int outlen = 32;
        UInt32[] v = new UInt32[16];

        UInt32[] blake2s_iv = {
            0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
        };

        UInt32 Rotr32(UInt32 x, int y)
        {
            UInt32 temp = (x >> y) ^ (x << (32 - y));
            return temp;
        }

        UInt32 Get32(byte b0, byte b1, byte b2, byte b3)
        {
            UInt32 temp = 0;
            temp = (UInt32)b0;
            temp ^= (UInt32)b1 << 8;
            temp ^= (UInt32)b2 << 16;
            temp ^= (UInt32)b3 << 24;

            return temp;
        }

        void G(UInt32 a, UInt32 b, UInt32 c, UInt32 d, UInt32 x, UInt32 y)
        {
            v[a] = v[a] + v[b] + x;
            v[d] = Rotr32((v[d] ^ v[a]), 16);
            v[c] = v[c] + v[d];
            v[b] = Rotr32((v[b] ^ v[c]), 12);
            v[a] = v[a] + v[b] + y;
            v[d] = Rotr32((v[d] ^ v[a]), 8);
            v[c] = v[c] + v[d];
            v[b] = Rotr32((v[b] ^ v[c]), 7);
        }

        void compress(int last)
        {
            int[,] sigma = new int[10, 16] {
                { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
                { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
                { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
                { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
                { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
                { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
                { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
                { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
                { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
                { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 }
            };

            UInt32[] m = new UInt32[16];

            for (int i = 0; i < 8; i++)
            {
                v[i] = h[i];
                v[i + 8] = blake2s_iv[i];
            }

            v[12] ^= t[0];
            v[13] ^= t[1];
            if (last != 0)
                v[14] = ~v[14];

            for (int i = 0; i < 16; i++)
            {
                m[i] = Get32(b[i * 4 + 0], b[i * 4 + 1], b[i * 4 + 2], b[i * 4 + 3]);
            }

            for (int i = 0; i < 10; i++)
            {
                G(0, 4, 8, 12, m[sigma[i, 0]], m[sigma[i,1]]);
                G(1, 5, 9, 13, m[sigma[i,2]], m[sigma[i,3]]);
                G(2, 6, 10, 14, m[sigma[i,4]], m[sigma[i,5]]);
                G(3, 7, 11, 15, m[sigma[i,6]], m[sigma[i,7]]);
                G(0, 5, 10, 15, m[sigma[i,8]], m[sigma[i,9]]);
                G(1, 6, 11, 12, m[sigma[i,10]], m[sigma[i,11]]);
                G(2, 7, 8, 13, m[sigma[i,12]], m[sigma[i,13]]);
                G(3, 4, 9, 14, m[sigma[i,14]], m[sigma[i,15]]);
            }

            for (int i = 0; i < 8; ++i)
            {
                h[i] ^= v[i] ^ v[i + 8];
            }
        }

        void update(string chunks)
        {

            for (int i = 0; i < chunks.Length; i++)
            {
                if (c == 64)
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

        int init(string key)
        {
            int keylen = key.Length;
            if (outlen == 0 || outlen > 32 || keylen > 32)
                return -1;

            for (int i = 0; i < 8; i++)
                h[i] = blake2s_iv[i];
            h[0] = (uint)(h[0] ^ 0x01010000 ^ (keylen << 8) ^ outlen);

            t[0] = 0;
            t[1] = 0;
            c = 0;

            for (int i = keylen; i < 64; i++)
                b[i] = 0;
            if (keylen > 0)
            {
                update(key);
                c = 64;
            }

            return 0;
        }

        void finalHash()
        {
            t[0] += c;
            if (t[0] < c)
                t[1]++;

            while (c < 64)
                b[c++] = 0;
            compress(1);

            // little endian convert and store
            for (int i = 0; i < outlen; i++)
                output.Add((h[i >> 2] >> (8 * (i & 3))) & 0xFF);

        }

        public Blake2s(string key, string message) {
            init(key);
            update(message);
            finalHash();
        }

        public string Digest()
        {
            string s = "";
            foreach (UInt32 o in output)
                s += o.ToString("x");

            return s;
        }
    }
}
