
// ClientDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "Win32Data.h"

// CClientDlg �_�C�A���O
class CClientDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CClientDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};