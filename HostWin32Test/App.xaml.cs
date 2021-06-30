namespace HostWin32Test
{
    using HostWin32Test.ViewModels;
    using HostWin32Test.Views;
    using HostWin32Test.Models;
    using System.Windows;

    /// <summary>
    /// App.xaml の相互作用ロジック
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            (new MainView() { DataContext = new MainViewModel() }).Show();
        }

        protected override void OnExit(ExitEventArgs e)
        {
            base.OnExit(e);

            SharedMemory.Dispose();
        }
    }
}
