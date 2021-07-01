#pragma once


// DummyView

class DummyView : public CWnd
{
	DECLARE_DYNAMIC(DummyView)

public:
	DummyView();
	virtual ~DummyView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBnClicked();
};


