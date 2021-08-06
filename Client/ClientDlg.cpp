
// ClientDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "define.h"

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
	ON_MESSAGE(WM_USER, &CClientDlg::OnUser)
	ON_MESSAGE(WM_USER_DESTROY, &CClientDlg::OnUserDestroy)
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
		for (size_t i = 0; i < m_pTestViews.size(); ++i)
		{
			if (m_pTestViews[i]->GetParentHwnd() == parentHandle)
			{
				// 既に同じ親に対して描画しているものが存在する場合はそれと入れ替える
				m_pTestViews[i]->DestroyWindow();
				delete m_pTestViews[i];
				m_pTestViews[i] = new TestView();
				m_pTestViews[i]->Create(pParent);
				return (LRESULT)m_pTestViews[i]->GetSafeHwnd();
			}
		}
		// 新規に作成する
		TestView* pView = new TestView();
		pView->Create(pParent);
		m_pTestViews.push_back(pView);
		return (LRESULT)pView->GetSafeHwnd();
	}

	return 0;
}

afx_msg LRESULT CClientDlg::OnUserDestroy(WPARAM wParam, LPARAM lParam)
{
	::OutputDebugStringW(_T("CClientDlg::OnUserDestroy()\n"));

	for (size_t i = 0; i < m_pTestViews.size(); ++i)
	{
		if (m_pTestViews[i]->GetParentHwnd() == (HWND)lParam)
		{
			m_pTestViews[i]->DestroyWindow();
			delete m_pTestViews[i];
			m_pTestViews.erase(m_pTestViews.begin() + i);
			break;
		}
	}

	return 0;
}
