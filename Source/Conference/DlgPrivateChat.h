#pragma once

#include "../../include/Util/Util.h"

class CDlgPrivateChat : public WindowImplBase
{
public:
	CDlgPrivateChat(void);
	~CDlgPrivateChat(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgPrivateChat\\DlgPrivateChat.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("DlgPrivateChat"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnFastReply(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	tChatUserList*	m_ptUserList;
	tChatMsgList*	m_ptMsgList;
	void AddUser(LPCTSTR szName, LPCTSTR szURL);
	void UpdataUI();

	void ClearUserList();

	void SendChatMsg();
	void ShowMsg(LPCTSTR strMsg, LPCTSTR strSender, LPCTSTR strTime);
	void RevPrivateChat(LPCTSTR strMsg, LPCTSTR strSender, BOOL bSend = FALSE);

	void ShowUI(bool bShow = true);
	void ShowPrivateMsg();
};

