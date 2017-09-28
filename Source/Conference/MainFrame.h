#pragma once

#include <vector>
#include "../../include/Util/Util.h"
#include "DlgConference.h"
#include "DlgLogin.h"
#include "DlgGuest.h"
#include "DlgContacts.h"
#include "WndProcessing.h"
#include "MenuTray.h"
#include "resource.h"

class MainFrame : public WindowImplBase
{
public:
	MainFrame(void);
	~MainFrame(void);

protected:
	virtual CDuiString	GetSkinFolder()				{ return _T ("skin"); };
	virtual CDuiString	GetSkinFile()				{ return _T ("FrameWnd.xml"); };
	virtual LPCTSTR		GetWindowClassName() const	{ return _T ("FrameWnd"); };
	virtual void		InitWindow();
	virtual LRESULT		HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CDlgConference*		m_pDlgConference;
	CDlgLogin*			m_pDlgLogin;
	CDlgGuest*			m_pDlgGuest;
	CDlgContacts*		m_pDlgContacts;
	CWndProcessing*		m_pWndProcessing;

	// UI界面显示调用栈
	vector<eModeType>	m_ModeTypeStack;		// 存放显示的模块堆栈
	int					m_iConferenceUICount;
	int					m_iLoginUICount;
	int					m_iContactsUICount;

	// 保存鼠标位置
	int					m_iMouseXPos;
	int					m_iMouseYPos;

	CTrayIcon			m_trayIcon;
private: 
	void Push(eModeType eType);					// 压栈
	void Pop(eModeType eType);					// 出栈
	void ShowUI();								// 显示模块界面
	void HideAllUI();							// 隐藏所有界面
	void ClearStack();							// 清空栈
	void DestroyAllUI();						// 释放所有的界面

	void CreateConferenceUI();
	void CreateLoginUI();
	void CreateGuestUI();
	void CreateContactsUI();
	void CreateProcessingWnd();

	BOOL ProCmdline();

	//当有新的状态产生时，通知相应窗口
	void NotifyWindow();
public:
	LRESULT OnVidyoMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTrayMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitUI(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowProcess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void InitVidyoSystem();
	void ReleaseVidyoSystem();
	void ShowTrayMenu();
	void TrayLogout();
	void TrayExitConf();
};