// ClientDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER0	0
static HHOOK g_Hook;

void DebugOutput(LPCWSTR lpStr)
{
	::OutputDebugStringW(lpStr);
	::OutputDebugStringW(L"\n");
}

void DebugErrorMessageOutput(DWORD error)
{
	DWORD err = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessage(				//エラー表示文字列作成
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	TCHAR hoge[512] = { 0 };
	_stprintf_s(hoge, _T("ERROR: %d(0x%08x) : %s"), err, err, (LPTSTR)lpMsgBuf);
	::OutputDebugStringW(hoge);
	::OutputDebugStringW(L"\n");
}

// CClientDlg ダイアログ
CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CClientDlg::~CClientDlg()
{
	if (m_pDummyWindow != nullptr)
	{
		delete m_pDummyWindow;
	}
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CClientDlg::OnBnClickedButton2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CClientDlg メッセージ ハンドラー

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	SetTimer(ID_TIMER0, 100, NULL);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		// 共有メモリをポーリング
		case ID_TIMER0:
			// 表示用にポーリング
			PolingSharedMemory();

			// コマンド処理用にポーリング
			// 冗長だけど後でコードをわけたりするときのためにこのままにしておく。
			PolingCreateCommand();
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CClientDlg::PolingSharedMemory()
{
	long size = m_SharedData.GetTotalSize();
	{
		byte* bytes = new byte[size];
		TCHAR* buff = new TCHAR[3 * size + 1];

		if (m_SharedData.ReadToEnd(bytes) == size)
		{
			for (int i = 0; i < size; i++)
			{
				wsprintf(buff + 3 * i, _T("%02x "), bytes[i]);
			}
			GetDlgItem(IDC_STATIC1)->SetWindowTextW(buff);
		}

		delete buff;
		delete bytes;
	}
}

void CClientDlg::PolingCreateCommand()
{
	byte bytes[sizeof(int)] = { 0 };
	m_SharedData.Read(bytes, E_MAP_CreateWindowCommand);

	if (*(int*)bytes == 0)
	{
		byte zeros[4] = { 0 };
		m_SharedData.Write((byte*)zeros, E_MAP_CreateWindowAnswer);
	}
	else
	{
		if (m_HasCreateCommand == false)
		{
			m_HasCreateCommand = true;
		}
	}
}

HWND CClientDlg::CreateChild()
{
	byte bytes[sizeof(int)] = { 0 };
	m_SharedData.Read(bytes, E_MAP_WindowHandle);
	m_hWndFromWPF = (HWND)(*(int*)bytes);
	if (m_hWndFromWPF != NULL)
	{
		CWnd* pWnd = CWnd::FromHandle(m_hWndFromWPF);

		TCHAR buff[256] = { 0 };
		RECT rect1 = { 0, 0, 260, 260 };
		RECT rect2 = { 10, 10, 120, 30 };

		if (m_pDummyWindow != NULL)
		{
			delete m_pDummyWindow;
		}
		m_pDummyWindow = new DummyView();
		if (m_pButton != NULL)
		{
			delete m_pButton;
		}
		m_pButton = new CButton();

		m_pDummyWindow->Create(NULL, _T("DummyWindow"), WS_VISIBLE | WS_CHILD, rect1, pWnd, 1005);
		m_pButton->Create(_T("Button1"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rect2, m_pDummyWindow, 1003);
		_stprintf_s(buff, _T("0x%08x"), (int)m_pButton->GetSafeHwnd());
		m_pButton->SetWindowTextW(buff);

		return m_pDummyWindow->GetSafeHwnd();
	}

	return 0;
}

// WriteAnswer ボタン
void CClientDlg::OnBnClickedButton1()
{
	if (m_ChildHandle != NULL)
	{
		int handle = (int)m_ChildHandle;
		m_SharedData.Write((byte*)handle, E_MAP_CreateWindowAnswer);
	}
}

// CreateWindow ボタン
void CClientDlg::OnBnClickedButton2()
{
	if (m_HasCreateCommand)
	{
		m_ChildHandle = CreateChild();

		TCHAR buff[11] = { 0 };
		wsprintf(buff, _T("0x%08x"), (int)m_ChildHandle);
		GetDlgItem(IDC_STATIC2)->SetWindowTextW(buff);
	}
}
