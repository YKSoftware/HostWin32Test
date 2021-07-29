namespace HostWin32Test.Views
{
    using System;
    using YKToolkit.Controls;

    /// <summary>
    /// MainView.xaml の相互作用ロジック
    /// </summary>
    public partial class MainView : Window
    {
        public MainView()
        {
            InitializeComponent();
        }

        //private async void Button_Click(object sender, System.Windows.RoutedEventArgs e)
        //{
        //    var win32Host = await (App.Current as App).CreateWin32ControlAsync();
        //    this.win32ContentHost.Content = win32Host;
        //}
    }
}
