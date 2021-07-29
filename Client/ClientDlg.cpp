// ClientDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER0	0
static HHOOK g_Hook;

void DebugOutput(LPCWSTR lpStr)
{
	::OutputDebugStringW(lpStr);
	::OutputDebugStringW(L"\n");
}

void DebugErrorMessageOutput(DWORD error)
{
	DWORD err = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessage(				//�G���[�\��������쐬
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	TCHAR hoge[512] = { 0 };
	_stprintf_s(hoge, _T("ERROR: %d(0x%08x) : %s"), err, err, (LPTSTR)lpMsgBuf);
	::OutputDebugStringW(hoge);
	::OutputDebugStringW(L"\n");
}

// CClientDlg �_�C�A���O
CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CClientDlg::~CClientDlg()
{
	if (m_pDummyWindow != nullptr)
	{
		delete m_pDummyWindow;
	}
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
	ON_WM_TIMER()
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
	SetTimer(ID_TIMER0, 100, NULL);

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

void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		// ���L���������|�[�����O
		case ID_TIMER0:
			// �\���p�Ƀ|�[�����O
			PolingSharedMemory();

			// �R�}���h�����p�Ƀ|�[�����O
			// �璷�����ǌ�ŃR�[�h���킯���肷��Ƃ��̂��߂ɂ��̂܂܂ɂ��Ă����B
			PolingCreateCommand();
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CClientDlg::PolingSharedMemory()
{
	long size = m_SharedData.GetTotalSize();
	{
		byte* bytes = new byte[size];
		TCHAR* buff = new TCHAR[3 * size + 1];

		if (m_SharedData.ReadToEnd(bytes) == size)
		{
			for (int i = 0; i < size; i++)
			{
				wsprintf(buff + 3 * i, _T("%02x "), bytes[i]);
			}
			GetDlgItem(IDC_STATIC1)->SetWindowTextW(buff);
		}

		delete buff;
		delete bytes;
	}
}

void CClientDlg::PolingCreateCommand()
{
	byte bytes[sizeof(int)] = { 0 };
	m_SharedData.Read(bytes, E_MAP_CreateWindowCommand);

	if (*(int*)bytes == 0)
	{
		byte zeros[4] = { 0 };
		m_SharedData.Write((byte*)zeros, E_MAP_CreateWindowAnswer);
	}
	else
	{
		if (m_HasCreateCommand == false)
		{
			m_HasCreateCommand = true;
		}
	}
}

HWND CClientDlg::CreateChild()
{
	byte bytes[sizeof(int)] = { 0 };
	m_SharedData.Read(bytes, E_MAP_WindowHandle);
	m_hWndFromWPF = (HWND)(*(int*)bytes);
	if (m_hWndFromWPF != NULL)
	{
		CWnd* pWnd = CWnd::FromHandle(m_hWndFromWPF);

		TCHAR buff[256] = { 0 };
		RECT rect1 = { 0, 0, 260, 260 };
		RECT rect2 = { 10, 10, 120, 30 };

		if (m_pDummyWindow != NULL)
		{
			delete m_pDummyWindow;
		}
		m_pDummyWindow = new DummyView();
		if (m_pButton != NULL)
		{
			delete m_pButton;
		}
		m_pButton = new CButton();

		m_pDummyWindow->Create(NULL, _T("DummyWindow"), WS_VISIBLE | WS_CHILD, rect1, pWnd, 1005);
		m_pButton->Create(_T("Button1"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, rect2, m_pDummyWindow, 1003);
		_stprintf_s(buff, _T("0x%08x"), (int)m_pButton->GetSafeHwnd());
		m_pButton->SetWindowTextW(buff);

		return m_pDummyWindow->GetSafeHwnd();
	}

	return 0;
}

// WriteAnswer �{�^��
void CClientDlg::OnBnClickedButton1()
{
	if (m_ChildHandle != NULL)
	{
		int handle = (int)m_ChildHandle;
		m_SharedData.Write((byte*)handle, E_MAP_CreateWindowAnswer);
	}
}

// CreateWindow �{�^��
void CClientDlg::OnBnClickedButton2()
{
	if (m_HasCreateCommand)
	{
		m_ChildHandle = CreateChild();

		TCHAR buff[11] = { 0 };
		wsprintf(buff, _T("0x%08x"), (int)m_ChildHandle);
		GetDlgItem(IDC_STATIC2)->SetWindowTextW(buff);
	}
}
