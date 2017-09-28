#pragma once
class CWndFastReply : public WindowImplBase
{
public:
	CWndFastReply(void);
	~CWndFastReply(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndFastReply.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndFastReply"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

