namespace HostWin32Test.Views
{
    using System;
    using System.Runtime.InteropServices;
    using System.Windows;
    using System.Windows.Interop;
    using YKToolkit.Controls;

    public class Win32Host : HwndHost
    {
        public Win32Host()
        {
            this._cppHandle = (IntPtr)User32.FindWindow(null, "Client");

            if (this._cppHandle != IntPtr.Zero)
            {
                this.SizeChanged += OnSizeChanged;
            }
        }

        private void OnSizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (this._childHandle != IntPtr.Zero)
            {
                System.Diagnostics.Debug.WriteLine("SendMessage!!");
                User32.SendMessage((int)this._childHandle, (int)WMs.WM_USER_SIZECHANGED, (int)e.NewSize.Width, (int)e.NewSize.Height);
            }
        }

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            this._wpfHandle = hwndParent.Handle;

            this._cppHostHandle = User32.CreateWindowEx(
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

            if (this._cppHandle != IntPtr.Zero)
            {
                this._childHandle = (IntPtr)User32.SendMessage((int)this._cppHandle, (int)User32.WMs.WM_USER, 0, (int)this._cppHostHandle);
            }

            return new HandleRef(this, this._cppHostHandle);
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            User32.DestroyWindow(hwnd.Handle);
            if (this._cppHandle != IntPtr.Zero)
            {
                User32.SendMessage((int)this._cppHandle, (int)WMs.WM_USER_DESTROY, 0, (int)this._cppHostHandle);
            }
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

                //case (int)User32.WMs.WM_DESTROY:
                    // ここに入る前に DestroyWindowCore() メソッドが実行されて HwndHost 内部はすべて Dispose() されるので
                    // ここには一生入ってきません。
                    //break;
            }

            return base.WndProc(hwnd, msg, wParam, lParam, ref handled);
        }

        private IntPtr _wpfHandle;
        private IntPtr _cppHostHandle;
        private IntPtr _cppHandle;
        private IntPtr _childHandle;

        private const int ID = 1;
        private const string UNKNOWN = "Unknown";
        private const string WPF_HOST = "WPF Host";
        private const string CPP_HOST = "Cpp Host";

        /// <summary>
        /// Cpp 側に伝えるメッセージを表します。
        /// </summary>
        private enum WMs : int
        {
            /// <summary>
            /// サイズ変更
            /// </summary>
            WM_USER_SIZECHANGED = (int)User32.WMs.WM_USER + 1,

            /// <summary>
            /// ウィンドウ破棄
            /// </summary>
            WM_USER_DESTROY,
        }
    }
}
