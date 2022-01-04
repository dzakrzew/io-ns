using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HashFunctions
{
    class Program
    {
        static void Main(string[] args)
        {
            string input = "hello";
            SHA1 sha1 = new SHA1();
            SHA256 sha256 = new SHA256();
            SHA512 sha512 = new SHA512();
            Adler32 adler32 = new Adler32();

            Console.WriteLine(sha1.Digest(input));
            Console.WriteLine(sha256.Digest(input));
            Console.WriteLine(sha512.Digest(input));
            Console.WriteLine(adler32.Digest(input));

            System.Threading.Thread.Sleep(2000);
        }
    }
}
