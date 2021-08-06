namespace HostWin32Test.ViewModels
{
    using System.Collections.ObjectModel;
    using YKToolkit.Bindings;

    /// <summary>
    /// MainView に対する ViewModel を表します。
    /// </summary>
    public class MainViewModel : NotificationObject
    {
        /// <summary>
        /// 新しいインスタンスを生成します。
        /// </summary>
        public MainViewModel()
        {
            this.AddCommand = new DelegateCommand(_ => Add());
            this.RemoveCommand = new DelegateCommand(_ => Remove());
        }

        /// <summary>
        /// 値コレクションに値を追加します。
        /// </summary>
        private void Add()
        {
            ++this._counter;
            this.Values.Add(this._counter);
        }

        /// <summary>
        /// 値コレクションの最終アイテムを削除します。
        /// </summary>
        private void Remove()
        {
            if (this.Values.Count > 0)
                this.Values.RemoveAt(this.Values.Count - 1);
        }

        /// <summary>
        /// 追加コマンドを取得します。
        /// </summary>
        public DelegateCommand AddCommand { get; private set; }

        /// <summary>
        /// 削除コマンドを取得します。
        /// </summary>
        public DelegateCommand RemoveCommand { get; private set; }

        /// <summary>
        /// 値コレクションを取得します。
        /// </summary>
        public ObservableCollection<int> Values { get; private set; } = new ObservableCollection<int>();

        /// <summary>
        /// 追加した回数をカウント
        /// </summary>
        private int _counter;
    }
}
