#pragma once

#include "../../include/common/MixerInfo.h"
#include "../../include/common/Wave.h"
#include "../../include/common/VideoCapture.h"

#define COMPONENTTYPE_DST_SPEAKERS			4
#define COMPONENTTYPE_DST_WAVEIN			7

class CDlgAVSetting : public WindowImplBase
{
public:
	CDlgAVSetting(void);
	~CDlgAVSetting(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgSetting\\DlgAVSetting.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Setting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT , WPARAM , LPARAM , BOOL& bHandled);
	void OnSelectChanged( TNotifyUI &msg );

	CWndShadow	m_WndShadow;
	static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
public:
	void InitUI();

	void OnBtnTestCamera(TCHAR* deviceName);
	void OnBtnTestMic();
	void OnBtnTestSpeaker();
private:
	CTabLayoutUI*			m_pTabAVSetting;
	CProgressUI*			m_pProMicTest;
	CProgressUI*			m_pProSpeakerTest;	

	COptionUI*				m_pOptAudioAGC;

	CMixerInfo*				m_pMixerInfoSpeaker;
	CMixerInfo*				m_pMixerInfoMIC;
	CWaveIn*	m_pWaveIn;
	CVideoCapture*	m_pVideoCapture;

	int m_iSpeakerTestCount;
};

