#pragma once

#include "../../include/common/MixerInfo.h"

#define COMPONENTTYPE_DST_SPEAKERS			4
#define COMPONENTTYPE_DST_WAVEIN			7

class CWndSetSpeaker : public WindowImplBase
{
public:
	CWndSetSpeaker(void);
	~CWndSetSpeaker(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndSetSpeaker.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("SpeakerSetting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

public:
	void UpdataUI();
};

