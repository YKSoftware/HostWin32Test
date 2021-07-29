
// ClientDlg.h : ヘッダー ファイル
//

#pragma once
#include "Win32Data.h"
#include "DummyView.h"

// CClientDlg ダイアログ
class CClientDlg : public CDialogEx
{
// コンストラクション
public:
	CClientDlg(CWnd* pParent = NULL);	// 標準コンストラクター
	~CClientDlg();

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

	bool m_HasCreateCommand = false;
	void PolingSharedMemory();
	void PolingCreateCommand();
	HWND CreateChild();
	DummyView* m_pDummyWindow;
	CButton* m_pButton;
	HWND m_ChildHandle = (HWND)0;

	Win32Data m_SharedData;
	HWND m_hWndFromWPF;
	CButton m_Button1;
	CButton m_Button2;
	int m_Count = 0;
	bool m_IsTimerEnabled = false;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
