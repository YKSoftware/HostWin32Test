namespace HostWin32Test.Models
{
    using System.Collections.Generic;
    using System.IO.MemoryMappedFiles;
    using System.Linq;
    using System.Threading;

    /// <summary>
    /// 共有メモリの機能を提供します。
    /// </summary>
    public static class SharedMemory
    {
        /// <summary>
        /// 静的なコンストラクタ
        /// </summary>
        static SharedMemory()
        {
            // readonly フィールドの初期化
            var last = Win32DataMap.Items.Last();
            _memorySize = last.Index + last.Size;

            TryInitialize();
        }

        private static bool TryInitialize()
        {
            // 排他処理用のミューテックスを作成
            if (Mutex.TryOpenExisting(_mutexName, out _mutex) == false)
            {
                _mutex = new Mutex(false, _mutexName);
            }

            // 共有メモリを生成する
            // ただし既に存在する場合はそれを開く
            _sharedFile = MemoryMappedFile.CreateOrOpen(_sharedMemoryName, _memorySize);
            _accessor = _sharedFile.CreateViewAccessor();

            return _accessor != null;
        }

        /// <summary>
        /// 読み込みます。
        /// </summary>
        /// <param name="index">読み込むデータの先頭インデックスを指定します。</param>
        /// <param name="size">読み込むデータのサイズを指定します。</param>
        /// <returns>読み込んだデータを返します。</returns>
        public static IEnumerable<byte> Read(int index, int size)
        {
            if ((_accessor == null) && (TryInitialize() == false))
                yield return 0;

            try
            {
                if (_mutex.WaitOne(_waitTime))
                {
                    for (var i = 0; i < size; i++)
                    {
                        // 共有メモリサイズを逸脱しないように注意
                        var position = index + i;
                        if (position >= _memorySize)
                            break;

                        yield return _accessor.ReadByte(position);
                    }
                }
            }
            finally
            {
                _mutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// 書き込みます。
        /// </summary>
        /// <param name="bytes">書き込むデータ配列を指定します。</param>
        /// <param name="index">書き込み先のインデックスを指定します。</param>
        public static void Write(byte[] bytes, int index)
        {
            if ((_accessor == null) && (TryInitialize() == false))
                return;

            try
            {
                if (_mutex.WaitOne(_waitTime))
                {
                    for (var i = 0; i < bytes.Length; i++)
                    {
                        // 共有メモリサイズを逸脱しないように注意
                        var position = index + i;
                        if (position >= _memorySize)
                            break;

                        _accessor.Write(position, bytes[i]);
                    }
                }
            }
            finally
            {
                _mutex.ReleaseMutex();
            }
        }

        /// <summary>
        /// リソースを破棄します。
        /// </summary>
        public static void Dispose()
        {
            if (_mutex != null)
            {
                _mutex.Dispose();
                _mutex = null;
            }
            if (_sharedFile != null)
            {
                _sharedFile.Dispose();
                _sharedFile = null;
            }
            if (_accessor != null)
            {
                _accessor.Dispose();
                _accessor = null;
            }
        }

        private static Mutex _mutex;
        private const string _mutexName = "HostWin32Test.Mutex01";

        private static MemoryMappedFile _sharedFile;
        private static MemoryMappedViewAccessor _accessor;
        private const string _sharedMemoryName = "HostWin32Test.Memory01";
        private static readonly long _memorySize;
        private const int _waitTime = 500;
    }
}
