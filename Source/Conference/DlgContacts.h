#pragma once

#include "stdafx.h"

class CDlgContacts : public WindowImplBase
{
public:
	CDlgContacts(HWND hParent = NULL);
	~CDlgContacts(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgContacts\\DlgContacts.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("р╫йстф"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

	HWND m_hParent;
	CWndShadow	m_WndShadow;
public:
	void ShowUI(bool bShow = true);
	void Login();
	void SignOut();
	void InitUI();
	void UpdateUI();
	void CalculateTime();
	void UpdataCaption();
	void UpdataContactsList(bool bSearch = false);
	void ReleaseUI();

	VidyoClientAccountData* m_ptAccountData;
	VidyoClientAccountList* m_ptAccountList;

	void OnBtnSearch(bool bClick = true);
private:
	CLabelUI*		m_pLabCaption;
	COptionUI*		m_pOptSearch;
	CButtonUI*		m_pBtnSetting;
	CEditUI*		m_pEditSearch;
	CTileLayoutUI*	m_pTileRoom;
};

