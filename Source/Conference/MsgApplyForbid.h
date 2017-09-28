#pragma once
class CMsgApplyForbid : public WindowImplBase
{
public:
	CMsgApplyForbid(void);
	~CMsgApplyForbid(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("MsgApplyForbid.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MsgApplyForbid"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

