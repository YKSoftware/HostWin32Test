
// ClientDlg.h : ヘッダー ファイル
//

#pragma once
#include <vector>
#include "TestView.h"

using namespace std;

// CClientDlg ダイアログ
class CClientDlg : public CDialogEx
{
// コンストラクション
public:
	CClientDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;
	vector<TestView*> m_pTestViews;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserDestroy(WPARAM wParam, LPARAM lParam);
};
