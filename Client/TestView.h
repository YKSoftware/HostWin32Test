#pragma once


// TestView

class TestView : public CWnd
{
	DECLARE_DYNAMIC(TestView)

public:
	TestView();
	virtual ~TestView();
	BOOL Create(CWnd* pParent);
	HWND GetParentHwnd();

protected:
	HWND m_ParentHwnd;
	int m_Width = 160;
	int m_Height = 120;
	int m_Value = 0;
	CButton m_Button;
	CStatic m_Static;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClicked();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
protected:
	afx_msg LRESULT OnUserSizechanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserValuechanged(WPARAM wParam, LPARAM lParam);
};


