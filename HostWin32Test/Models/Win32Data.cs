namespace HostWin32Test.Models
{
    using System;
    using System.Linq;
    using System.Timers;

    public class Win32Data : IDisposable
    {
        public Win32Data()
        {
            this._timer = new Timer(100);
            this._timer.Elapsed += OnTimerElapsed;
            this._timer.Start();
        }

        private void OnTimerElapsed(object sender, ElapsedEventArgs e)
        {
            this.Counter = BitConverter.ToInt32(SharedMemory.Read(100, 4).ToArray(), 0);
        }

        public Action<Win32Data> Win32CounterChanged;

        private int _Counter;
        public int Counter
        {
            get { return this._Counter; }
            private set
            {
                if (this._Counter != value)
                {
                    this._Counter = value;
                    RaiseWin32CounterChanged();
                }
            }
        }

        private void RaiseWin32CounterChanged() => this.Win32CounterChanged?.Invoke(this);

        public void Dispose()
        {
            if (this._timer != null)
            {
            this._timer.Elapsed -= OnTimerElapsed;
                this._timer.Dispose();
            }

            GC.SuppressFinalize(this);
        }

        private Timer _timer;
    }
}
