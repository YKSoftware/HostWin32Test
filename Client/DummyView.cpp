// DummyView.cpp : 実装ファイル
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



// DummyView メッセージ ハンドラー




void DummyView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	::OutputDebugStringW(L"押されました。\n");

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
