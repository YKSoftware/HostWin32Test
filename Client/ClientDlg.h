
// ClientDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "TestView.h"

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
	TestView* m_pTestView;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserSizechanged(WPARAM wParam, LPARAM lParam);
};
