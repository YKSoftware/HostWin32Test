﻿namespace HostWin32Test.Models
{
    public enum Win32DataMapIndexes
    {
        /// <summary>
        /// WPF がホストする Win32 用コントロールのウィンドウハンドル
        /// </summary>
        [Size(sizeof(int))]
        WindowHandle = 0,

        /// <summary>
        /// CreateWindow 相当の処理をお願いします。
        /// </summary>
        [Size(sizeof(int))]
        CreateWindowCommand,

        /// <summary>
        /// Win32 へのウォッチドッグ用カウンタ
        /// </summary>
        [Size(sizeof(int))]
        WatchDogToWin32,

        // ↑ WPF → Win32 書き込み用
        ///////////////////////////////////////////////////////////////////
        // ↓ WPF ← Win32 読み込み用

        /// <summary>
        /// Win32 へ渡したウィンドウハンドルの確認用
        /// </summary>
        [Size(sizeof(int))]
        WindowHandleBack,

        /// <summary>
        /// CreateWindow 相当の処理をおこないました。
        /// </summary>
        [Size(sizeof(int))]
        CreateWindowAnswer,

        /// <summary>
        /// Win32 からのウォッチドッグ用カウンタ
        /// </summary>
        [Size(sizeof(int))]
        WatchDogFromWin32,
    }
}
