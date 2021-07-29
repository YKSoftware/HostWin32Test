namespace HostWin32Test
{
    using HostWin32Test.ViewModels;
    using HostWin32Test.Views;
    using HostWin32Test.Models;
    using System.Windows;
    using System.Windows.Interop;
    using System;
    using System.Linq;
    using System.Threading;
    using System.Threading.Tasks;

    /// <summary>
    /// App.xaml の相互作用ロジック
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            // とりあえず画面表示してウィンドウハンドルを確定させる
            (new MainView() { DataContext = new MainViewModel() }).Show();

            // WPF のウィンドウハンドルを Cpp へ渡す
            var bytes = BitConverter.GetBytes(new WindowInteropHelper(this.MainWindow).Handle.ToInt32());
            this._win32Data.Write(bytes, Win32DataMapIndexes.WindowHandle);
        }

        protected override void OnExit(ExitEventArgs e)
        {
            base.OnExit(e);

            SharedMemory.Dispose();
        }

        public Task<HwndHost> CreateWin32ControlAsync()
        {
            return Task.Run(() =>
            {
                // Cpp に CreateWindow() に相当する処理を指示する
                this._win32Data.Write(BitConverter.GetBytes((int)1), Win32DataMapIndexes.CreateWindowCommand);

                // Cpp からウィンドウハンドルをもらう
                var cppHandle = IntPtr.Zero;
                while (true)
                {
                    var answer = BitConverter.ToInt32(this._win32Data.Read(Win32DataMapIndexes.CreateWindowAnswer).ToArray(), 0);
                    if (answer != 0)
                    {
                        cppHandle = new IntPtr(answer);
                        // Cpp への指示を取り下げる
                        this._win32Data.Write(BitConverter.GetBytes((int)0), Win32DataMapIndexes.CreateWindowCommand);
                        break;
                    }
                    Thread.Sleep(200);
                }

                // HwndHost コントロールを new して、BuildWindowCore() メソッド内で Cpp からもらったウィンドウハンドルを使う
                return new Win32Host(cppHandle) as HwndHost;
            });
        }

        private Win32Data _win32Data = new Win32Data();
    }
}
