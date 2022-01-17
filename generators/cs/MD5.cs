using System;
using System.Collections.Generic;

namespace HashFunctions
{
    public class MD5
    {
        UInt32 mask = 0xFFFFFFFF;
        int width = 32;

        List<UInt32> words = new List<UInt32> { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };

        private UInt32 F(UInt32 x, UInt32 y, UInt32 z)
        {
            return (x & y) | (~x & z);
        }

        private UInt32 G(UInt32 x, UInt32 y, UInt32 z)
        {
            return (z & x) | (~z & y);
        }

        private UInt32 H(UInt32 x, UInt32 y, UInt32 z)
        {
            return (x ^ y ^ z);
        }
        private UInt32 I(UInt32 x, UInt32 y, UInt32 z)
        {
            return y ^ (x | ~z);
        }

        private UInt32 leftRotate(UInt64 val, int n)
        {
            UInt32 lbits = (UInt32)((val << n) & mask);
            UInt32 rbits = (UInt32)(val >> (width - n));
            return lbits | rbits;
        }

        public void init(string input)
        {
            List<UInt32> message = new List<UInt32>();

            for (int i = 0; i < input.Length; i++)
            {
                message.Add(Convert.ToUInt32(input[i]));
            }

            UInt64 length = (UInt64)(message.Count * 8);

            message.Add(0x80);

            while ((message.Count * 8 + 64) % 512 != 0)
            {
                message.Add(0x00);
            }

            // Add padding
            for (int i = 0; i <= 56; i += 8)
                message.Add((uint)(length >> i));

            List<List<UInt32>> message_chunks = new List<List<UInt32>>();

            for (int i = 0; i < message.Count * 8 / 512; i++)
            {
                List<UInt32> vtemp = new List<UInt32>();

                for (int j = 0; j < 64; j++)
                {
                    vtemp.Add(message[(i * 64) + j]);
                }

                message_chunks.Add(vtemp);
            }

            process(message_chunks);
        }

        public string Digest(string message)
        {
            init(message);

            string s = "";
            foreach (UInt32 word in words)
            {
                byte[] bytes = BitConverter.GetBytes(word);
                string retval = "";
                foreach (byte b in bytes)
                    retval += b.ToString("x2");
                s += retval;
            }

            return s;
        }

        private void process(List<List<UInt32>> chunks)
        {
            foreach (List<UInt32> chunk in chunks)
            {
                List<UInt32> X = new List<UInt32>();
                List<UInt32> h = new List<UInt32>(words);

                //prepare constants
                List<UInt32> constants = new List<UInt32>();
                for (int i = 0; i < 64; i++)
                {
                    UInt64 c = ((ulong)(Math.Abs((double)Math.Sin(i + 1)) * 4294967296));
                    c = c & mask;
                    constants.Add((uint)c);
                }

                //bytes to int
                for (int j = 0; j < 64; j = j + 4)
                {
                    UInt32 temp = 0x00;

                    temp = (UInt32)chunk[j + 3] << 24;
                    temp |= (UInt32)chunk[j + 2] << 16;
                    temp |= (UInt32)chunk[j + 1] << 8;
                    temp |= (UInt32)chunk[j + 0];

                    X.Add(temp);
                }

                List<UInt32> Xi = new List<UInt32> { 7, 12, 17, 22 };
                // round 1
                for (int n = 0; n < 16; n++)
                {
                    //modulo w C++ działa inaczej na negatywnych intach niż python dlatego potrzebne są takie komplikacje
                    int a = (4 + ((-n + 0) % 4)) % 4;
                    int b = (4 + ((-n + 1) % 4)) % 4;
                    int c = (4 + ((-n + 2) % 4)) % 4;
                    int d = (4 + ((-n + 3) % 4)) % 4;

                    int K = n;
                    int S = (int)Xi[n % 4];
                    int T = n;


                    //wartości tej zmiennej wykraczają poza 32 bity, dlatego trzeba je rzutować na ullong
                    UInt64 to_rotate = h[a];
                    to_rotate += X[K];
                    to_rotate += F(h[b], h[c], h[d]);
                    to_rotate += constants[T];

                    h[a] = (h[b] + leftRotate((to_rotate & mask), S)) & mask;
                }

                Xi = new List<UInt32> { 5, 9, 14, 20 };
                // round 2
                for (int n = 0; n < 16; n++)
                {
                    int a = (4 + ((-n + 0) % 4)) % 4;
                    int b = (4 + ((-n + 1) % 4)) % 4;
                    int c = (4 + ((-n + 2) % 4)) % 4;
                    int d = (4 + ((-n + 3) % 4)) % 4;

                    double te = (5 * n + 1) % 16;
                    int K = (int)Math.Floor(te);
                    int S = (int)Xi[n % 4];
                    int T = n + 16;

                    UInt64 to_rotate = h[a];
                    to_rotate += X[K];
                    to_rotate += G(h[b], h[c], h[d]);
                    to_rotate += constants[T];

                    h[a] = (h[b] + leftRotate((to_rotate & mask), S)) & mask;
                }

                Xi = new List<UInt32> { 4, 11, 16, 23 };
                // round 3
                for (int n = 0; n < 16; n++)
                {
                    int a = (4 + ((-n + 0) % 4)) % 4;
                    int b = (4 + ((-n + 1) % 4)) % 4;
                    int c = (4 + ((-n + 2) % 4)) % 4;
                    int d = (4 + ((-n + 3) % 4)) % 4;

                    int K = (3 * n + 5) % 16;
                    int S = (int)Xi[n % 4];
                    int T = n + 32;

                    UInt64 to_rotate = h[a];
                    to_rotate += X[K];
                    to_rotate += H(h[b], h[c], h[d]);
                    to_rotate += constants[T];

                    h[a] = (h[b] + leftRotate((to_rotate & mask), S)) & mask;
                }

                Xi = new List<UInt32> { 6, 10, 15, 21 };
                // round 4
                for (int n = 0; n < 16; n++)
                {
                    int a = (4 + ((-n + 0) % 4)) % 4;
                    int b = (4 + ((-n + 1) % 4)) % 4;
                    int c = (4 + ((-n + 2) % 4)) % 4;
                    int d = (4 + ((-n + 3) % 4)) % 4;

                    int K = (7 * n) % 16;
                    int S = (int)Xi[n % 4];
                    int T = n + 48;

                    UInt64 to_rotate = h[a];
                    to_rotate += X[K];
                    to_rotate += I(h[b], h[c], h[d]);
                    to_rotate += constants[T];

                    h[a] = (h[b] + leftRotate((to_rotate & mask), S)) & mask;
                }

                words[0] = (words[0] + h[0]) & mask;
                words[1] = (words[1] + h[1]) & mask;
                words[2] = (words[2] + h[2]) & mask;
                words[3] = (words[3] + h[3]) & mask;
            }
        }
    }
}