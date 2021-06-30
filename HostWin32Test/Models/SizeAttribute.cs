namespace HostWin32Test.Models
{
    using System;

    public class SizeAttribute : Attribute
    {
        public SizeAttribute(int size)
        {
            this.Size = size;
        }

         public int Size { get; set; }
    }
}
