namespace HostWin32Test.Models
{
    using System;
    using System.Collections.Generic;

    public class Win32Data
    {
        public IEnumerable<byte> Read(Win32DataMapIndexes index)
        {
            var map = Win32DataMap.GetMap(index);
            return SharedMemory.Read(map.Index, map.Size);
        }

        public void Write(byte[] bytes, Win32DataMapIndexes index)
        {
            var map = Win32DataMap.GetMap(index);
            SharedMemory.Write(bytes, map.Index);
        }
    }
}
