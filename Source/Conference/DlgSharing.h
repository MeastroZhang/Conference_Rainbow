#pragma once

#include "DlgConference.h"
class CDlgConference;
class CDlgSharing : public WindowImplBase
{
public:
	CDlgSharing(LPVOID pParent);
	~CDlgSharing(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgSharing\\DlgShare.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Sharing"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

public:
	void UpdataUI();

	CDlgConference*	m_pDlgConference;
};

