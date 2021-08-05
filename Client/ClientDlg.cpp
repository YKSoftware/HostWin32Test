
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

#define WM_USER_SIZECHANGED		WM_USER+1

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
	ON_MESSAGE(WM_USER, &CClientDlg::OnUser)
	ON_MESSAGE(WM_USER_SIZECHANGED, &CClientDlg::OnUserSizechanged)
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



afx_msg LRESULT CClientDlg::OnUser(WPARAM wParam, LPARAM lParam)
{
	HWND parentHandle = (HWND)lParam;
	CWnd* pParent = CWnd::FromHandle(parentHandle);

	if (pParent != NULL)
	{
		if (m_pTestView != NULL)
		{
			delete m_pTestView;
		}
		m_pTestView = new TestView();
		m_pTestView->Create(pParent);
	}
	return (m_pTestView == NULL) ? 0 : (LRESULT)m_pTestView->GetSafeHwnd();
}


afx_msg LRESULT CClientDlg::OnUserSizechanged(WPARAM wParam, LPARAM lParam)
{
	int width = (int)wParam;
	int height = (int)lParam;
	if (m_pTestView != NULL)
	{
		m_pTestView->ChangeSize(width, height);
	}

	return 0;
}
