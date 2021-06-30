using System;
using System.Linq;

namespace HostWin32Test.Models
{
    public struct Win32DataMapItem
    {
        public Win32DataMapIndexes ID { get; set; }
        public int Index { get; set; }
        public byte[] Data { get; set; }
        public int Size { get { return this.Data.Length; } }
    }

    public class Win32DataMap
    {
        static Win32DataMap()
        {
            var index = 0;
            Items = Enumerable.Range(0, Enum.GetValues(typeof(Win32DataMapIndexes)).Cast<int>().Max(x => x) + 1)
                               .Select(x => (Win32DataMapIndexes)x)
                               .Select(x =>
                                {
                                    var size = x.GetAttribute<SizeAttribute>().Size;
                                    var item = new Win32DataMapItem()
                                    {
                                        ID = x,
                                        Index = index,
                                        Data = new byte[size],
                                    };
                                    index += size;
                                    return item;
                                })
                               .ToArray();
        }

        public static Win32DataMapItem GetMap(Win32DataMapIndexes index)
        {
            return Win32DataMap.Items[(int)index];
        }

        public static Win32DataMapItem[] Items { get; private set; }
    }
}
