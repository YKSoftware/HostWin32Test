namespace HostWin32Test
{
    using HostWin32Test.ViewModels;
    using HostWin32Test.Views;
    using System.Windows;

    /// <summary>
    /// App.xaml の相互作用ロジック
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            var w = new MainView() { DataContext = new MainViewModel() };
            w.Show();
        }
    }
}
