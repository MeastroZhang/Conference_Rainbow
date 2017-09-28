#pragma once
class CWndSetPhotoPath : public WindowImplBase
{
public:
	CWndSetPhotoPath(void);
	~CWndSetPhotoPath(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndSetPhotoPath.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndSetPhotoPath"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

