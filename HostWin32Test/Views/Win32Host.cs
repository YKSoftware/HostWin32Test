namespace HostWin32Test.Views
{
    using HostWin32Test.Models;
    using System;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Threading;
    using System.Windows;
    using System.Windows.Interop;

    public class Win32Host : HwndHost
    {
        public Win32Host()
            : this(IntPtr.Zero)
        {
        }

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

        [DllImport("user32.dll")]
        public static extern int GetWindowThreadProcessId(IntPtr hWnd, out int lpdwProcessId);

        [DllImport("user32.dll")]
        public static extern int GetWindowThreadProcessId(HandleRef hWnd, out int lpdwProcessId);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        public static extern int GetCurrentThreadId();

        #region HwndHost 抽象クラスのメンバ

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            int processId = 0;
            var threadId = GetWindowThreadProcessId(hwndParent.Handle, out processId);

            var bytes = BitConverter.GetBytes(hwndParent.Handle.ToInt32());
            this._win32Data.Write(bytes, Win32DataMapIndexes.WindowHandle);

            // Cpp に CreateWindow() に相当する処理を指示する
            //文字列メッセージを送信
            //送信データをByte配列に格納
            COPYDATASTRUCT cds;
            cds.dwData = 0;        //使用しない
            cds.lpData = "Tomosoft string";//テキストのポインターをセット
            cds.cbData = cds.lpData.Length + 1; //長さをセット
                                                //文字列を送る
            Int32 hWnd = FindWindow(null, "Client");
            this._cppHandle = new IntPtr(SendMessage(hWnd, WM_COPYDATA, 0, ref cds));


            // Cpp からウィンドウハンドルをもらう
            //while (true)
            {
                //var answer = BitConverter.ToInt32(this._win32Data.Read(Win32DataMapIndexes.CreateWindowAnswer).ToArray(), 0);
                //if (answer != 0)
                {
                    //this._cppHandle = new IntPtr(answer);
                    // Cpp への指示を取り下げる
                    this._win32Data.Write(BitConverter.GetBytes((int)0), Win32DataMapIndexes.CreateWindowCommand);
                    //break;
                }
                //Thread.Sleep(200);
            }

            var handleRef = new HandleRef(this, this._cppHandle);
            threadId = GetWindowThreadProcessId(handleRef, out processId);
            var currentThreadId = GetCurrentThreadId();
            return handleRef;
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

        [DllImport("User32.dll", EntryPoint = "FindWindow")]
        public static extern Int32 FindWindow(String lpClassName, String lpWindowName);

        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        public static extern Int32 SendMessage(Int32 hWnd, Int32 Msg, Int32 wParam, ref COPYDATASTRUCT lParam);

        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        public static extern Int32 SendMessage(Int32 hWnd, Int32 Msg, Int32 wParam, Int32 lParam);

        public const Int32 WM_COPYDATA = 0x4A;
        public const Int32 WM_USER = 0x400;

        //COPYDATASTRUCT構造体 
        public struct COPYDATASTRUCT
        {
            public Int32 dwData;      //送信する32ビット値
            public Int32 cbData;   //lpDataのバイト数
            public string lpData;   //送信するデータへのポインタ(0も可能)
        }

        protected IntPtr _cppHandle;
        private Win32Data _win32Data = new Win32Data();
    }
}
