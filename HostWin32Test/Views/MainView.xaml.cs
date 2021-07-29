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

            this.Dispatcher.BeginInvoke((Action)(async () =>
            {
                var win32Host = await (App.Current as App).CreateWin32ControlAsync();
                this.win32ContentHost.Content = win32Host;
            }));
        }
    }
}
