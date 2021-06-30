using System;
using System.Reflection;

namespace HostWin32Test
{
    public static class Extensions
    {
        public static T GetAttribute<T>(this Enum e)
            where T : Attribute
        {
            FieldInfo info = e.GetType().GetField(e.ToString());
            var attribute = info.GetCustomAttribute<T>();
            if (attribute is T)
            {
                return attribute;
            }

            return null;
        }
    }
}
