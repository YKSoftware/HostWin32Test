
// ClientDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg ダイアログ



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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



// Read ボタン
void CClientDlg::OnBnClickedButton1()
{
	if (m_hWndFromWPF != NULL)
		return;

	TCHAR buff[256] = { 0 };
	byte bytes[sizeof(int)] = { 0 };
	if (m_SharedMemory.Read(bytes, 0, sizeof(int)) == sizeof(int))
	{
		m_hWndFromWPF = (HWND)(*(int*)bytes);
		if (m_hWndFromWPF != NULL)
		{

			CWnd* pWnd = CWnd::FromHandle(m_hWndFromWPF);
			RECT rect1 = { 10, 10, 120, 30 };
			m_Button1.Create(_T("Button1"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rect1, pWnd, 1003);
			_stprintf_s(buff, _T("0x%08x"), (int)m_Button1.GetSafeHwnd());
			m_Button1.SetWindowTextW(buff);

			RECT rect2 = { 50, 50, 160, 70 };
			m_Button2.Create(_T("Button2"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rect2, pWnd, 1004);
			_stprintf_s(buff, _T("0x%08x"), (int)m_Button2.GetSafeHwnd());
			m_Button2.SetWindowTextW(buff);

			GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
		}
	}

	_stprintf_s(buff, _T("0x%08x"), (int)m_hWndFromWPF);
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(buff);
}

// Write ボタン
void CClientDlg::OnBnClickedButton2()
{
	TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("0x%08x"), ++m_Count);
	GetDlgItem(IDC_STATIC2)->SetWindowTextW(buff);
	m_SharedMemory.Write((byte*)&m_Count, 100, sizeof(int));
}


void CClientDlg::OnBnClickedButton3()
{
	TCHAR buff[256] = { 0 };
	_stprintf_s(buff, _T("0x%08x"), (int)m_hWndFromWPF);
	GetDlgItem(IDC_STATIC3)->SetWindowTextW(buff);
	m_SharedMemory.Write((byte*)&m_hWndFromWPF, 50, sizeof(int));
}
