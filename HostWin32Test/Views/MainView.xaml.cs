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

        private void Slider_MouseWheel(object sender, System.Windows.Input.MouseWheelEventArgs e)
        {
            var slider = sender as System.Windows.Controls.Slider;
            if (e.Delta < 0)
            {
                var value = slider.Value + slider.LargeChange;
                slider.Value = (value <= slider.Maximum) ? value : slider.Maximum;
            }
            else
            {
                var value = slider.Value - 1;
                slider.Value = (value >= slider.Minimum) ? value : 0;
            }
        }
    }
}
