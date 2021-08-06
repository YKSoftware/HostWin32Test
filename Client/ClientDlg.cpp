
// ClientDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDlg �_�C�A���O

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


// CClientDlg ���b�Z�[�W �n���h���[

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
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
				// ���ɓ����e�ɑ΂��ĕ`�悵�Ă�����̂����݂���ꍇ�͂���Ɠ���ւ���
				m_pTestViews[i]->DestroyWindow();
				delete m_pTestViews[i];
				m_pTestViews[i] = new TestView();
				m_pTestViews[i]->Create(pParent);
				return (LRESULT)m_pTestViews[i]->GetSafeHwnd();
			}
		}
		// �V�K�ɍ쐬����
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
