#pragma once
class CMenuTray : public WindowImplBase
{
public:
	CMenuTray(void);
	~CMenuTray(void);

protected:
	virtual CDuiString GetSkinFolder(){return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");};
	virtual CDuiString GetSkinFile(){return _T ("\\MenuTray.xml");};
	virtual LPCTSTR GetWindowClassName() const{return _T("MenuTray");};
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
};

