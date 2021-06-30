
// ClientDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

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
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CClientDlg::OnBnClickedButton3)
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



// Read �{�^��
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

// Write �{�^��
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
