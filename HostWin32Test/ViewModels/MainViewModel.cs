namespace HostWin32Test.ViewModels
{
    using Models;
    using System;
    using System.Linq;
    using YKToolkit.Bindings;

    public class MainViewModel : NotificationObject
    {
        /// <summary>
        /// 新しいインスタンスを生成します。
        /// </summary>
        public MainViewModel()
        {
            this._win32Data = new Win32Data();
            this._win32Data.Win32CounterChanged += OnWin32CounterChanged;

            this.ReadCommand = new DelegateCommand(_ => Read());
            this.WriteCommand = new DelegateCommand(_ => Write());
        }

        /// <summary>
        /// Win32CounterChanged イベントハンドラ
        /// </summary>
        /// <param name="win32Data">イベント発行元</param>
        private void OnWin32CounterChanged(Win32Data win32Data)
        {
            this.Counter = $"0x{win32Data.Counter.ToString("X08")}";
        }

        /// <summary>
        /// 共有メモリを読み込みます。
        /// </summary>
        private void Read()
        {
            var bytes = SharedMemory.Read(50, 4).ToArray();
            this.WindowHandleFromCpp = $"0x{BitConverter.ToInt32(bytes, 0).ToString("X08")}";
        }

        /// <summary>
        /// 共有メモリに書き込みます。
        /// </summary>
        private void Write()
        {
            var handle = this.HWnd.ToInt32();
            var bytes = BitConverter.GetBytes(handle);
            SharedMemory.Write(bytes, 0);

            this.WindowHandle = $"0x{handle.ToString("X08")}";
        }

        /// <summary>
        /// 共有メモリ読込コマンドを取得します。
        /// </summary>
        public DelegateCommand ReadCommand { get; private set; }

        /// <summary>
        /// 共有メモリ書き込みコマンドを取得します。
        /// </summary>
        public DelegateCommand WriteCommand { get; private set; }

        private IntPtr _hWnd;
        /// <summary>
        /// Win32 をホストするコントロールのウィンドウハンドルを取得または設定します。
        /// </summary>
        public IntPtr HWnd
        {
            get { return this._hWnd; }
            set { SetProperty(ref this._hWnd, value); }
        }

        private string _windowHandle;
        /// <summary>
        /// ウィンドウハンドルの文字列を取得します。
        /// </summary>
        public string WindowHandle
        {
            get { return this._windowHandle; }
            private set { SetProperty(ref this._windowHandle, value); }
        }

        private string _windowHandleFromCpp;
        /// <summary>
        /// C++ から取得したウィンドウハンドルの文字列を取得します。
        /// </summary>
        public string WindowHandleFromCpp
        {
            get { return this._windowHandleFromCpp; }
            private set { SetProperty(ref this._windowHandleFromCpp, value); }
        }

        private string _counter;
        /// <summary>
        /// カウンタ値文字列を取得します。
        /// </summary>
        public string Counter
        {
            get { return this._counter; }
            private set { SetProperty(ref this._counter, value); }
        }

        private Win32Data _win32Data;
    }
}
