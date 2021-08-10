namespace HostWin32Test.ViewModels
{
    using System.Collections.Generic;
    using System.Linq;
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
            this._allValues.Add(this._counter);
            VerifyTopIndex();
            RaisePropertyChanged(nameof(this.MaximumIndex));
        }

        /// <summary>
        /// 値コレクションの最終アイテムを削除します。
        /// </summary>
        private void Remove()
        {
            if (this._allValues.Count > 0)
                this._allValues.RemoveAt(this._allValues.Count - 1);
            VerifyTopIndex();
            RaisePropertyChanged(nameof(this.MaximumIndex));
        }

        /// <summary>
        /// 先頭インデックスを正しく設定します。
        /// </summary>
        private void VerifyTopIndex()
        {
            if (this.TopIndex >= this._allValues.Count)
                this.TopIndex = this._allValues.Count - 1;
            else if (this.TopIndex < 0)
                this.TopIndex = 0;

            if ((this.TopIndex - this._allValues.Count) < 2)
                UpdateValues();
        }

        /// <summary>
        /// 表示用値コレクションを更新します。
        /// </summary>
        private void UpdateValues()
        {
            if (this._allValues.Count == 0)
                this.Values = Enumerable.Repeat<int>(0, 2).ToArray();
            else if (this.TopIndex < this._allValues.Count - 1)
                this.Values = this._allValues.Skip(this.TopIndex).Take(2).ToArray();
            else
                this.Values = new int[] { this._allValues.Last(), 0 };
        }

        /// <summary>
        /// 追加コマンドを取得します。
        /// </summary>
        public DelegateCommand AddCommand { get; private set; }

        /// <summary>
        /// 削除コマンドを取得します。
        /// </summary>
        public DelegateCommand RemoveCommand { get; private set; }

        private int[] _values = new int[0];
        /// <summary>
        /// 値コレクションを取得します。
        /// </summary>
        public int[] Values
        {
            get { return this._values; }
            private set
            {
                if (SetProperty(ref this._values, value))
                {
                    RaisePropertyChanged(nameof(this.MaximumIndex));
                }
            }
        }

        private int _topIndex;
        /// <summary>
        /// 現在の先頭インデックスを取得または設定します。
        /// </summary>
        public int TopIndex
        {
            get { return this._topIndex; }
            set
            {
                if (SetProperty(ref this._topIndex, value))
                {
                    UpdateValues();
                }
            }
        }

        /// <summary>
        /// インデックスの最大値を取得します。
        /// </summary>
        public int MaximumIndex { get { return this._allValues.Count - 1; } }

        /// <summary>
        /// 追加した回数をカウント
        /// </summary>
        private int _counter;

        /// <summary>
        /// 全値コレクション
        /// </summary>
        private List<int> _allValues = new List<int>();
    }
}
