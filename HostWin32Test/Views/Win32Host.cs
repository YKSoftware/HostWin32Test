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
            this.HWnd = User32.CreateWindowEx(          // [参考](http://chokuto.ifdef.jp/advanced/function/CreateWindowEx.html)
                User32.WS_EX_LEFT,                      // 拡張ウィンドウスタイル : 一般的な左揃えされたプロパティを持つウィンドウ
                "static",                               // コントロールクラス名 : static はシステム定義済みコントロールのクラス名で、スタティックコントロールを表す。
                "",                                     // ウィンドウ名 : キャプションとして表示される
                User32.WS_CHILD | User32.WS_VISIBLE,    // ウィンドウスタイル : 
                0, 0,                                   // 作成するウィンドウの (x, y) 座標
                260, 260,                               // 作成するウィンドウの幅と高さ
                hwndParent.Handle,                      // 親ウィンドウハンドル
                (IntPtr)User32.HOST_ID,                 // 自分の固有 ID
                IntPtr.Zero,                            // このパラメータは無視されます
                IntPtr.Zero                             // WM_CREATE メッセージの lParam として渡される CREATESTRUCT 構造体へのポインタ。不要な場合は NULL を指定。
                );

            return new HandleRef(this, this.HWnd);
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
    }
}
