#pragma once
class CWndProcessing : public WindowImplBase
{
public:
	CWndProcessing(void);
	~CWndProcessing(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndProcessing.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndProcessing"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CWndShadow	m_WndShadow;
public:
	void ShowUI(bool bShow = true);
	void InitUI();
};

