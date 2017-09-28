#pragma once

#include "resource.h"
#include <ShellAPI.h>

class CDlgLoginGuest : public WindowImplBase
{
public:
	CDlgLoginGuest(HWND hParent = NULL);
	~CDlgLoginGuest(void);

protected:
	virtual CDuiString GetSkinFolder() { return _T ("skin"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgLogin\\DlgLogin.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Login"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	HWND m_hParent;

private:
	CLabelUI*	m_pLabCaption;
	CButtonUI*	m_pCloseBtn;
	CButtonUI*	m_pMinBtn;
	CButtonUI*	m_pLoginBtn;
	CEditUI*	m_pEditPortal;
	CEditUI*	m_pEditUsername;
	CEditUI*	m_pEditUserpass;
	CButtonUI*	m_pBtnFPSW;
	COptionUI*	m_pOptVideo;
	COptionUI*	m_pOptAudio;

public:
	void Login();
};

