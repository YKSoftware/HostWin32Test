
// ClientDlg.h : ヘッダー ファイル
//

#pragma once
#include "Win32Data.h"

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

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	Win32Data m_SharedData;
	HWND m_hWndFromWPF;
	CButton m_Button1;
	CButton m_Button2;
	int m_Count = 0;
	bool m_IsTimerEnabled = false;
	static LRESULT CALLBACK MyHookProc(int nCode, WPARAM wparam, LPARAM lparam);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
