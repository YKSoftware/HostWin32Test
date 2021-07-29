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
#define ID_TIMER1	1
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
	ON_BN_CLICKED(IDC_BUTTON3, &CClientDlg::OnBnClickedButton3)
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
		// WPF 側からのコマンドをポーリング
		case ID_TIMER0:
			PolingCreateCommand();
			break;

		case ID_TIMER1:
			TCHAR buff[256] = { 0 };
			_stprintf_s(buff, _T("0x%08x"), ++m_Count);
			GetDlgItem(IDC_STATIC2)->SetWindowTextW(buff);
			m_SharedData.Write((byte*)&m_Count, E_MAP_WatchDogFromWin32);
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CClientDlg::PolingCreateCommand()
{
	byte bytes[sizeof(int)] = { 0 };
	m_SharedData.Read(bytes, E_MAP_CreateWindowCommand);

	if (m_HasCreateCommand == false)
	{
		if (*(int*)bytes != 0)
		{
			m_HasCreateCommand = true;
			HWND childHandle = CreateChild();
			m_SharedData.Write((byte*)(int)childHandle, E_MAP_CreateWindowAnswer);
		}
	}
	else
	{
		if (*(int*)bytes == 0)
		{
			m_HasCreateCommand = false;
			byte zeros[sizeof(int)] = { 0 };
			m_SharedData.Write(zeros, E_MAP_CreateWindowAnswer);
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

		if (m_pDummyWindow != nullptr)
		{
			delete m_pDummyWindow;
		}
		m_pDummyWindow = new DummyView();
		if (m_pButton != nullptr)
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

// Read ボタン
void CClientDlg::OnBnClickedButton1()
{
	if (m_hWndFromWPF != NULL)
		return;

	TCHAR buff[256] = { 0 };
	byte bytes[sizeof(int)] = { 0 };
	if (m_SharedData.Read(bytes, E_MAP_WindowHandle) == sizeof(int))
	{
		m_hWndFromWPF = (HWND)(*(int*)bytes);
		if (m_hWndFromWPF != NULL)
		{
			CWnd* pWnd = CWnd::FromHandle(m_hWndFromWPF);

			//RECT rect2 = { 50, 50, 160, 70 };
			//m_Button2.Create(_T("Button2"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rect2, pWnd, 1004);
			//_stprintf_s(buff, _T("0x%08x"), (int)m_Button2.GetSafeHwnd());
			//m_Button2.SetWindowTextW(buff);

			RECT rect3 = { 0, 0, 260, 260 };
			m_pDummyWindow->Create(NULL, _T("DummyWindow"), WS_VISIBLE | WS_CHILD, rect3, pWnd, 1005);

			RECT rect1 = { 10, 10, 120, 30 };
			m_Button1.Create(_T("Button1"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rect1, m_pDummyWindow, 1003);
			_stprintf_s(buff, _T("0x%08x"), (int)m_Button1.GetSafeHwnd());
			m_Button1.SetWindowTextW(buff);

			GetDlgItem(IDC_BUTTON1)->EnableWindow(false);

			DebugOutput(L"もしかしてフックできるんちゃう？");
			g_Hook = ::SetWindowsHookExW(WH_CALLWNDPROC, (HOOKPROC)CClientDlg::MyHookProc, AfxGetInstanceHandle(), 0);
			DebugErrorMessageOutput(GetLastError());
		}
	}

	_stprintf_s(buff, _T("0x%08x"), (int)m_hWndFromWPF);
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(buff);
}

// Write ボタン
void CClientDlg::OnBnClickedButton2()
{
	if (m_IsTimerEnabled)
	{
		KillTimer(ID_TIMER1);
		GetDlgItem(IDC_BUTTON2)->SetWindowTextW(L"Timer Start");
	}
	else
	{
		SetTimer(ID_TIMER1, 100, NULL);
		GetDlgItem(IDC_BUTTON2)->SetWindowTextW(L"Timer Stop");
	}
	m_IsTimerEnabled = !m_IsTimerEnabled;
}

// WindowHandle Write ボタン
void CClientDlg::OnBnClickedButton3()
{
	TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("0x%08x"), (int)m_hWndFromWPF);
	GetDlgItem(IDC_STATIC3)->SetWindowTextW(buff);
	m_SharedData.Write((byte*)&m_hWndFromWPF, E_MAP_WindowHandleBack);
}

LRESULT CALLBACK CClientDlg::MyHookProc(int nCode, WPARAM wparam, LPARAM lparam)
{
	return ::CallNextHookEx(g_Hook, nCode, wparam, lparam);
}
