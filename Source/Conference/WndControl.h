#pragma once
class CWndControl : public WindowImplBase
{
public:
	CWndControl(HWND hParent = NULL);
	~CWndControl(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndControl.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("ControlWnd"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual void OnPrepare();
	virtual	void Notify(TNotifyUI& msg);

	HWND m_hParent;

private:
	CButtonUI*		m_pCloseBtn;
	CTileLayoutUI*	m_pTileShare;
};

