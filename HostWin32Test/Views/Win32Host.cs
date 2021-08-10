namespace HostWin32Test.Views
{
    using System;
    using System.Runtime.InteropServices;
    using System.Windows;
    using System.Windows.Interop;
    using System.Windows.Threading;
    using YKToolkit.Controls;

    public class Win32Host : System.Windows.Controls.ContentControl
    {
        public Win32Host()
        {
            this._timer = new DispatcherTimer()
            {
                Interval = TimeSpan.FromMilliseconds(1000),
            };
            EventHandler onTick = null;
            onTick = (_, __) =>
            {
                this._timer.Stop();

                if (SetContent())
                {
                    this._timer.Tick -= onTick;
                    this._timer = null;
                }
                else
                {
                    this._timer.Start();
                }
            };
            this._timer.Tick += onTick;
            this._timer.Start();
        }

        private bool SetContent()
        {
            System.Diagnostics.Debug.WriteLine("SetContent()");
            if (this._win32Host == null)
            {
                var cppHandle = (IntPtr)User32.FindWindow(null, "Client");

                if (cppHandle != IntPtr.Zero)
                {
                    this._win32Host = new Win32HostCore(cppHandle);
                    this.Content = this._win32Host;
                }
            }

            return this.Content != null;
        }

        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register("Value", typeof(int), typeof(Win32Host), new UIPropertyMetadata(0, OnValuePropertyChanged));

        public int Value
        {
            get { return (int)GetValue(ValueProperty); }
            set { SetValue(ValueProperty, value); }
        }

        private static void OnValuePropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var control = d as Win32Host;
            if (control.SetContent())
            {
                control._win32Host.Value = control.Value;
            }
        }

        private DispatcherTimer _timer;
        private Win32HostCore _win32Host;

        private class Win32HostCore : HwndHost
        {
            private int _value;
            public int Value
            {
                get { return this._value; }
                set
                {
                    if (this._value != value)
                    {
                        this._value = value;
                        if (this._childHandle != IntPtr.Zero)
                        {
                            User32.SendMessage((int)this._childHandle, (int)WMs.WM_USER_VALUECHANGED, 0, this._value);
                        }
                    }
                }
            }

            public Win32HostCore(IntPtr cppHandle)
            {
                this._cppHandle = cppHandle;
                this.SizeChanged += OnSizeChanged;
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
                System.Diagnostics.Debug.WriteLine("BuildWindowCore()");
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
                    if (this._childHandle != IntPtr.Zero)
                    {
                        User32.SendMessage((int)this._childHandle, (int)WMs.WM_USER_VALUECHANGED, 0, this._value);
                    }
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

                /// <summary>
                /// 値変更
                /// </summary>
                WM_USER_VALUECHANGED,
            }
        }
    }
}
