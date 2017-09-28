#pragma once

#include "WndSetPhoto.h"
#include "WndSetVideo.h"
#include "WndSetMic.h"
#include "WndSetSpeaker.h"
#include "../../include/Util/Util.h"

class CWndToolbar : public WindowImplBase
{
public:
	CWndToolbar(HWND hParent = NULL);
	~CWndToolbar(void);

protected:
	virtual CDuiString GetSkinFolder() { return _T ("skin"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndToolbar.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Toolbar"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	void Notify(TNotifyUI& msg);

	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	HWND m_hParent;
	bool m_bTimer;
	struct _tStartTime
	{
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
	} StartTime;

	CWndSetPhoto*	m_pWndSetPhoto;
	CWndSetVideo*	m_pWndSetVideo;
	CWndSetMic*		m_pWndSetMic;
	CWndSetSpeaker*	m_pWndSetSpeaker;
public:
	void UpDateUI();
	void InitClock();
	void CalculateClock();
	void SetNewChatMsgCount(int iCount);
	void ShowUI(bool bShow = true);

	typedef enum SettingBtnType_
	{
		BTN_NONE,
		BTN_PHOTO,
		BTN_VIDEO,
		BTN_MIC,
		BTN_SPEAKER,
	} SettingBtnType;
	void ShowSettingUI(POINT ptMouse, SettingBtnType btnType = BTN_NONE);
	void HideAllSetUI();

	BOOL m_bSetUIShow;
	BOOL IsSetUIShow();

	tSelfControlStatus	m_tSelfStatus;
	void SetSelfContorlStatus(tSelfControlStatus selfStatus);
	VidyoClientLectureStage	m_eLectureStage;
	void SetLectureStage(VidyoClientLectureStage lectureStage);
	void MuteServerVideo(BOOL bMuteVideo);
	void MuteServerAudio(BOOL bMuteAudio);
private:
	CButtonUI*	m_pBtnViedo;
	CButtonUI*	m_pBtnVideoClosed;
	CButtonUI*	m_pBtnMic;
	CButtonUI*	m_pBtnMicClosed;
	CButtonUI*	m_pBtnSpeaker;
	CButtonUI*	m_pBtnSpeakerClosed;
};

