// TestView.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Client.h"
#include "TestView.h"

// TestView

IMPLEMENT_DYNAMIC(TestView, CWnd)

TestView::TestView()
{
}

TestView::~TestView()
{
}

HWND TestView::GetParentHwnd()
{
	return m_ParentHwnd;
}

BOOL TestView::Create(CWnd* pParent)
{
	m_ParentHwnd = pParent->GetSafeHwnd();
	return CWnd::Create(NULL, _T("TestView"), WS_CHILD | WS_VISIBLE, CRect(0, 0, m_Width, m_Height), pParent, 1);
}

void TestView::ChangeSize(int width, int height)
{
	::OutputDebugStringW(_T("ChangeSize()\n"));
	m_Width = width;
	m_Height = height;
	SetWindowPos(NULL, 0, 0, m_Width, m_Height, SWP_NOMOVE | SWP_NOZORDER);
}

BEGIN_MESSAGE_MAP(TestView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(1001, &TestView::OnBnClicked)
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void TestView::OnPaint()
{
	::OutputDebugStringW(_T("OnPaint()\n"));

	CPaintDC DC(this);

	CDC* pDC = this->GetDC();

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	CBrush brush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(0, 0, m_Width, m_Height);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}


void TestView::OnSize(UINT nType, int cx, int cy)
{
	::OutputDebugStringW(_T("OnSize()\n"));
	CWnd::OnSize(nType, cx, cy);
}


int TestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pButton.Create(_T("Click me."), WS_VISIBLE, CRect(20, 20, 100, 50), this, 1001);

	return 0;
}

void TestView::OnBnClicked()
{
	::OutputDebugStringW(_T("ボタンが押されました。\n"));
}


void TestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	::OutputDebugStringW(_T("OnLButtonDown()\n"));

	CWnd::OnLButtonDown(nFlags, point);
}


void TestView::OnDestroy()
{
	::OutputDebugStringW(_T("TestView::OnDestroy()\n"));
	CWnd::OnDestroy();
}
