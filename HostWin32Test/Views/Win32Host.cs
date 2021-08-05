namespace HostWin32Test.Views
{
    using System;
    using System.Runtime.InteropServices;
    using System.Windows.Interop;
    using YKToolkit.Controls;

    public class Win32Host : HwndHost
    {
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            this._wpfHandle = hwndParent.Handle;

            var cppHostHandle = User32.CreateWindowEx(
                (int)User32.WSs.WS_EX_LEFT,
                "static",
                "",
                (int)(User32.WSs.WS_CHILD | User32.WSs.WS_VISIBLE | User32.WSs.WS_CLIPCHILDREN),
                0, 0,
                260, 260,
                hwndParent.Handle,
                (IntPtr)ID,
                IntPtr.Zero,
                IntPtr.Zero
                );

            var cppHandle = User32.FindWindow(null, "Client");
            this._childHandle = (IntPtr)User32.SendMessage(cppHandle, (int)User32.WMs.WM_USER, 0, (int)cppHostHandle);

            return new HandleRef(this, cppHostHandle);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            User32.DestroyWindow(hwnd.Handle);
        }

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            var targetName = UNKNOWN;
            if (hwnd == this._wpfHandle) targetName = WPF_HOST;
            else if (hwnd == this.Handle) targetName = CPP_HOST;

            switch (msg)
            {
                case (int)User32.WMs.WM_PAINT:
                    System.Diagnostics.Debug.WriteLine($"[{targetName}] WM_PAINT");
                    break;
            }

            return base.WndProc(hwnd, msg, wParam, lParam, ref handled);
        }

        private IntPtr _wpfHandle;
        private IntPtr _childHandle;

        private const int ID = 1;
        private const string UNKNOWN = "Unknown";
        private const string WPF_HOST = "WPF Host";
        private const string CPP_HOST = "Cpp Host";
    }
}
