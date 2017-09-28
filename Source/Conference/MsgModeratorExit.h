#pragma once
class CMsgModeratorExit : public WindowImplBase
{
public:
	CMsgModeratorExit(void);
	~CMsgModeratorExit(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("MsgModeratorExit.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MsgModeratorExit"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

