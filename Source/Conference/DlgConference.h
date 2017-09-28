#pragma once

#include "stdafx.h"
#include "WndToolbar.h"
#include "WndRightTopBar.h"
#include "WndControl.h"
#include "DlgSharing.h"
#include "DlgModerator.h"
#include "MsgRoomExit.h"
#include "MsgModeratorExit.h"
#include "MsgApplyForbid.h"
#include "WndInputPin.h"
#include "WndFastReply.h"
#include "DlgPrivateChat.h"
#include "WndLectureMode.h"
#include "../../include/Util/Util.h"

class CDlgConference : public WindowImplBase
{
public:
	CDlgConference(HWND pParent);
	~CDlgConference();

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgConference\\DlgConference.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("р╫йстф"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual	void Notify(TNotifyUI& msg);
	void OnSelectChanged( TNotifyUI &msg );

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnModeratorChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnToolBarMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitConference(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEndConference(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFastReply(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	HWND		m_pParent;
	CWndShadow	m_WndShadow;

	typedef enum ConferenceMode_
	{
		MODE_MODERATOR,
		MODE_DISCUSS,
	} ConferenceMode;

	typedef enum ConferenceAuthority_
	{
		AUTHORITY_COMMON,
		AUTHORITY_MODERATOR,
	} ConferenceAuthority;

	struct _tConferenceInfo
	{
		ConferenceMode confMode;
		ConferenceAuthority confAuthority;
	} tConferenceInfo;

	VidyoClientAccountData* m_ptEntityData;
public:
	void InitUI();
	void ShowUI(bool bShow = true);
	void ReleaseUI();
	void UpdateUI();
	void UpdateLectureUI();
	void UpdataParticipantsList();
	void SetSelfContorlStatus(tSelfControlStatus selfStatus);
	void ClearRaisehand();
	void ShowToolbar(bool bShow = true);
	void UpdateToolbar();
	void ShowShareList();
	void SetLectureStage(VidyoClientLectureStage lectureMode);
	void SetPresenter(bool bHavePresenter);
	void UpdataMuteServerVideo(bool bMuteVideo);
	void UpdataMuteServerAudio(bool bMuteAudio);
	void ParticipantsChanged(unsigned int uiCount);
	void ShowControlWnd();
	void ShowChatWnd();
	void TakePicture();
	void LeaveConference();

	bool					m_bFullScreen;
	CWndToolbar*			m_pWndToolbar;
	CWndRightTopBar*		m_pWndRightTopBar;
	CWndLectureMode*		m_pWndLectureMode;
	CDlgModerator*			m_pDlgModerator;	// Moderator UI
	CDlgPrivateChat*		m_pDlgPrivateChat;

	bool					m_bShowToolBar;		// Show ToolBar Timer
	tSelfControlStatus		m_tBtnStatus;
	VidyoClientLectureStage	m_eLectureStage;
	bool					m_bHavePresenter;

	BOOL					m_bShare;
	void SetShareStatus(bool bShare);
	BOOL GetShareStatus();
	void StartShareScreen();
	void StopShareScreen();
	void SetShareScreenStatus();

	void SendChatMsg();
	void RevGroupChat(VidyoClientOutEventGroupChat* groupChat);
	void RevPrivateChat(VidyoClientOutEventPrivateChat* priviteChat);
	void ShowMsg(LPCTSTR strMsg, LPCTSTR strSender);
private:
	char	m_szConferenceID[64];
	RECT	m_oldRect;
};

