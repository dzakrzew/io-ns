using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;

namespace HashFunctions
{
    class Program
    {
        static void Main(string[] args)
        {
            Stopwatch sw = new Stopwatch();
            string method = "";
            string file;
            byte[] input = {};
            string[] methods = { 
                "sha1", "sha256", "sha512",
                "adler32", "crc32", "crc16", 
                "md4", "md5", 
                "blake2s", "blake2b"
            };

            for (int i = 0; i < args.Count() - 1; i++) {
                if (args[i] == "--method") {
                    method = args[i + 1];

                    if (!methods.Contains(method)) {
                        Console.WriteLine("Method not implemented");
                        System.Environment.Exit(1);
                    }
                }

                if (args[i] == "--file") {
                    file = args[i + 1];

                    if (!File.Exists(file)) {
                        Console.WriteLine("Input file not exist");
                        System.Environment.Exit(1);
                    }

                    input = File.ReadAllBytes(file);
                }
            }

            SHA1 sha1 = new SHA1();
            SHA256 sha256 = new SHA256();
            SHA512 sha512 = new SHA512();
            Adler32 adler32 = new Adler32();
            CRC32 crc32 = new CRC32();
            CRC16 crc16 = new CRC16();
            MD5 md5 = new MD5();
            MD4 md4 = new MD4();
            Blake2b blake2b = new Blake2b();
            Blake2s blake2s = new Blake2s();
            string digest = "";

            if (method == "sha1") {
                sw.Start();
                digest = sha1.Digest(input);
                sw.Stop();
            }

            if (method == "sha256") {
                sw.Start();
                digest = sha256.Digest(input);
                sw.Stop();
            }

            if (method == "sha512") {
                sw.Start();
                digest = sha512.Digest(input);
                sw.Stop();
            }

            if (method == "adler32") {
                sw.Start();
                digest = adler32.Digest(input);
                sw.Stop();
            }

            if (method == "crc32") {
                sw.Start();
                digest = crc32.Digest(input);
                sw.Stop();
            }

            if (method == "crc16") {
                sw.Start();
                digest = crc16.Digest(input);
                sw.Stop();
            }

            if (method == "md4") {
                sw.Start();
                digest = md4.Digest(input);
                sw.Stop();
            }

            if (method == "md5") {
                sw.Start();
                digest = md5.Digest(input);
                sw.Stop();
            }

            if (method == "blake2b") {
                sw.Start();
                digest = blake2b.Digest(input);
                sw.Stop();
            }

            if (method == "blake2s") {
                sw.Start();
                digest = blake2s.Digest(input);
                sw.Stop();
            }

            Console.WriteLine(
                "{\"hash\": \"" + digest + "\", \"execution_time\": \"" 
                + (sw.Elapsed.TotalMilliseconds / 1000).ToString("0.##########").Replace(',', '.') 
                + "\"}"
            );
        }
    }
}
