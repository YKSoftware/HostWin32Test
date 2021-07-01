// DummyView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Client.h"
#include "DummyView.h"


// DummyView

IMPLEMENT_DYNAMIC(DummyView, CWnd)

DummyView::DummyView()
{

}

DummyView::~DummyView()
{
}


BEGIN_MESSAGE_MAP(DummyView, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// DummyView ���b�Z�[�W �n���h���[




void DummyView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	::OutputDebugStringW(L"������܂����B\n");

	CWnd::OnLButtonDown(nFlags, point);
}


void DummyView::OnPaint()
{
	CPaintDC DC(this);

	CDC* pDC = this->GetDC();

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	CBrush brush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(10, 10, 240, 240);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}
