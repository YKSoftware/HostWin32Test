namespace HostWin32Test.Views
{
    using System;
    using System.Runtime.InteropServices;
    using System.Windows;
    using System.Windows.Interop;

    public class Win32Host : HwndHost
    {
        public Win32Host(IntPtr cppHandle)
        {
            this._cppHandle = cppHandle;
        }

        public static readonly DependencyProperty HWndProperty = DependencyProperty.Register("HWnd", typeof(IntPtr), typeof(Win32Host), new UIPropertyMetadata(IntPtr.Zero));

        public IntPtr HWnd
        {
            get { return (IntPtr)GetValue(HWndProperty); }
            set { SetValue(HWndProperty, value); }
        }

        #region HwndHost 抽象クラスのメンバ

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            return new HandleRef(this, this._cppHandle);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            User32.DestroyWindow(hwnd.Handle);
        }

        #endregion HwndHost 抽象クラスのメンバ

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch (msg)
            {
                case (int)User32.WMs.WM_PAINT:
                    System.Diagnostics.Debug.WriteLine($"WM_PAINT to 0x{hwnd.ToInt32().ToString("X08")}");
                    break;

                case (int)User32.WMs.WM_COMMAND:
                    var id = (uint)wParam.ToInt32() & 0x0000ffff;
                    System.Diagnostics.Debug.WriteLine(id);
                    break;
            }

            return base.WndProc(hwnd, msg, wParam, lParam, ref handled);
        }

        protected IntPtr _cppHandle;
    }
}
