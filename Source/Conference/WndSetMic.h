#pragma once

#include "../../include/common/MixerInfo.h"

#define COMPONENTTYPE_DST_SPEAKERS			4
#define COMPONENTTYPE_DST_WAVEIN			7

class CWndSetMic : public WindowImplBase
{
public:
	CWndSetMic(void);
	~CWndSetMic(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndSetMic.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MicSetting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

public:
	void UpdataUI();

private:
	CMixerInfo*				m_pMixerInfoMIC;
};

