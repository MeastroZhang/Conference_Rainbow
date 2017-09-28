#pragma once

#include "../../include/Util/Util.h"

class CDlgModerator : public WindowImplBase
{
public:
	CDlgModerator(void);
	~CDlgModerator(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgModerator\\DlgModerator.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Moderator"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	void InitData(bool bLocked = false);
	void ReleaseData();
	void ShowUI(bool bShow = TRUE);
	void UpDataUI();
	void SetLectureStage(VidyoClientLectureStage lectureMode);
	void GetParticipants();
	void UpdateParticipants(unsigned int uiCount = 0);
	void UpdataMuteVideo(bool bMuteVideo);
	void UpdataMuteAudio(bool bMuteAudio);

	VidyoClientParticipantList*		m_ptParticipants;
	tServerContorlStatus			m_tServerControlStatus;
	char	m_szConferenceID[64];
	char	m_szModeratorPin[64];
	void	SetModeratorPin(char* szModeratorPin);
private:
	int m_iIndexSelected;
};

