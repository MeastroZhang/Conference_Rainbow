#pragma once

#include "resource.h"
#include <ShellAPI.h>

class CDlgLogin : public WindowImplBase
{
public:
	CDlgLogin(HWND hParent = NULL);
	~CDlgLogin();

protected:
	virtual CDuiString GetSkinFolder(){return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");};
	virtual CDuiString GetSkinFile(){return _T ("\\DlgLogin\\DlgLogin.xml");};
	virtual LPCTSTR GetWindowClassName() const{return _T("Login");};
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	HWND m_hParent;

	bool		m_bLogin;
	CWndShadow	m_WndShadow;
private:
	CButtonUI*	m_pCloseBtn;
	CButtonUI*	m_pLoginBtn;
	CButtonUI*	m_pBtnJoin;
	CEditUI*	m_pEditUsername;
	CEditUI*	m_pEditUserpass;
	CButtonUI*	m_pBtnFPSW;
	CLabelUI*	m_pLabLoginError;

public:
	void Login();
	void LoginErrorTips(unsigned int errorType);
	void InitUI();

	void JoinRoom();

	bool m_bTrayLogout;
	void SetTrayLogout(bool bTrayLogout = true);
};

