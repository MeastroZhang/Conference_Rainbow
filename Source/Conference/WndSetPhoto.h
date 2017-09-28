#pragma once
class CWndSetPhoto : public WindowImplBase
{
public:
	CWndSetPhoto(void);
	~CWndSetPhoto(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndSetPhoto.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("PhotoSetting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

