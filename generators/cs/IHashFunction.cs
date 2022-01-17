namespace HashFunctions
{
    public abstract class IHashFunction
    {
        public abstract string Digest(byte[] input);
    }
}
