namespace HostWin32Test.Views
{
    using System;
    using System.Runtime.InteropServices;
    using System.Windows;
    using System.Windows.Interop;

    public class Win32Host : HwndHost
    {
        public static readonly DependencyProperty HWndProperty = DependencyProperty.Register("HWnd", typeof(IntPtr), typeof(Win32Host), new UIPropertyMetadata(IntPtr.Zero));

        public IntPtr HWnd
        {
            get { return (IntPtr)GetValue(HWndProperty); }
            set { SetValue(HWndProperty, value); }
        }

        #region HwndHost 抽象クラスのメンバ

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            this.HWnd = User32.CreateWindowEx(0, "static", "",
                                              User32.WS_CHILD | User32.WS_VISIBLE,
                                              0, 0,
                                              260, 260,
                                              hwndParent.Handle,
                                              (IntPtr)User32.HOST_ID,
                                              IntPtr.Zero,
                                              0);

            return new HandleRef(this, this.HWnd);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            User32.DestroyWindow(hwnd.Handle);
        }

        #endregion HwndHost 抽象クラスのメンバ
    }
}
