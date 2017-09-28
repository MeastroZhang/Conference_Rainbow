#pragma once

#include "../../include/common/MixerInfo.h"
#include "../../include/common/Wave.h"
#include "../../include/common/VideoCapture.h"

#define COMPONENTTYPE_DST_SPEAKERS			4
#define COMPONENTTYPE_DST_WAVEIN			7

class CDlgGuest : public WindowImplBase
{
public:
	CDlgGuest(HWND hParent = NULL);
	~CDlgGuest();

protected:
	virtual CDuiString GetSkinFolder(){return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");};
	virtual CDuiString GetSkinFile(){return _T ("\\DlgLogin\\DlgGuest.xml");};
	virtual LPCTSTR GetWindowClassName() const{return _T("GuestLogin");};
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT , WPARAM , LPARAM , BOOL& bHandled);
	void OnSelectChanged( TNotifyUI &msg );

	HWND m_hParent;

	bool		m_bShowAVSetting;
	bool		m_bLogin;
private:
	CButtonUI*	m_pCloseBtn;

	CDuiString	m_strPortal;
	CDuiString	m_strDisplayName;
	CDuiString	m_strRoomID;

	CTabLayoutUI*			m_pTabAVSetting;
	CProgressUI*			m_pProMicTest;
	CProgressUI*			m_pProSpeakerTest;	
	COptionUI*				m_pOptAudioAGC;
	CMixerInfo*				m_pMixerInfoSpeaker;
	CMixerInfo*				m_pMixerInfoMIC;
	CWaveIn*				m_pWaveIn;
	CVideoCapture*			m_pVideoCapture;
	int						m_iSpeakerTestCount;

public:
	void UpdataUI();
	void SetGuestInfo(CDuiString strURL, CDuiString strName, CDuiString strID);
	void JoinRoom();

	void OnBtnTestCamera(TCHAR* deviceName);
	void OnBtnTestMic();
	void OnBtnTestSpeaker();
};

