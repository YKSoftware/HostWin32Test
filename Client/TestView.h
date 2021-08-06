#pragma once


// TestView

class TestView : public CWnd
{
	DECLARE_DYNAMIC(TestView)

public:
	TestView();
	virtual ~TestView();
	BOOL Create(CWnd* pParent);
	void ChangeSize(int width, int height);
	HWND GetParentHwnd();

protected:
	HWND m_ParentHwnd;
	int m_Width = 160;
	int m_Height = 120;
	CButton m_pButton;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClicked();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};


