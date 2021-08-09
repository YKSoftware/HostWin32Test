namespace HostWin32Test.Views
{
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

        private void ScrollBar_MouseWheel(object sender, System.Windows.Input.MouseWheelEventArgs e)
        {
            var scrollBar = sender as System.Windows.Controls.Primitives.ScrollBar;
            if (e.Delta < 0)
            {
                var value = scrollBar.Value + scrollBar.LargeChange;
                scrollBar.Value = (value <= scrollBar.Maximum) ? value : scrollBar.Maximum;
            }
            else
            {
                var value = scrollBar.Value - 1;
                scrollBar.Value = (value >= scrollBar.Minimum) ? value : 0;
            }
        }
    }
}
