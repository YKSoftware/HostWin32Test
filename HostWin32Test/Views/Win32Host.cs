namespace HostWin32Test.Views
{
    using System;
    using System.Runtime.InteropServices;
    using System.Windows.Interop;

    public class Win32Host : HwndHost
    {
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            throw new NotImplementedException();
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            throw new NotImplementedException();
        }
    }
}
