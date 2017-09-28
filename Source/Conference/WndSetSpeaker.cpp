#include "StdAfx.h"
#include "WndSetSpeaker.h"
#include "../../include/Util/Util.h"

extern VidyoClientRequestConfiguration reqConfig;

CWndSetSpeaker::CWndSetSpeaker(void)
{

}


CWndSetSpeaker::~CWndSetSpeaker(void)
{
}

LRESULT CWndSetSpeaker::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CWndSetSpeaker::InitWindow()
{
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
	CLabelUI* labSpeaker = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labSpeaker")));
	CLabelUI* labSpeakerVolume = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labSpeakerVolume")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SETSPEAKER"), _T("labCaption"), szText, MAX_PATH);
	labCaption->SetText(szText);
	project_language_loadstring(_T("SETSPEAKER"), _T("labSpeaker"), szText, MAX_PATH);
	labSpeaker->SetText(szText);
	project_language_loadstring(_T("SETSPEAKER"), _T("labSpeakerVolume"), szText, MAX_PATH);
	labSpeakerVolume->SetText(szText);
}

void CWndSetSpeaker::UpdataUI()
{
	TCHAR szName[128] = {0};

	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));

	CComboUI* pComboSpeaker = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comSpeakerDevice")));
	pComboSpeaker->RemoveAll();
	for (unsigned int i = 0; i < reqConfig.numberSpeakers; i++)
	{
		CListLabelElementUI* pLabSpeaker = new CListLabelElementUI;
		ChangUtf8ToText((char*)(reqConfig.speakerPhyDeviceName[i]), szName);
		pLabSpeaker->SetText(szName);
		pComboSpeaker->Add(pLabSpeaker);
	}
	pComboSpeaker->SelectItem(reqConfig.currentSpeaker);

	// output volume
	VidyoClientRequestVolume volume = {0};
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_VOLUME_AUDIO_OUT, &volume, sizeof(volume));
	CSliderUI* pSliVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderSpeakerVolume")));
	pSliVolume->SetValue(volume.volume);
}

void CWndSetSpeaker::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("itemselect"))
	{
		int i = ((CComboUI*)msg.pSender)->GetCurSel();
		if (msg.pSender->GetName() == _T("comSpeakerDevice"))
		{
			if (i != reqConfig.currentSpeaker)
				reqConfig.currentSpeaker = i;
		}
		VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	}
	else if (msg.sType == _T("valuechanged"))
	{
		if (msg.pSender->GetName() == _T("sliderSpeakerVolume"))
		{
			VidyoClientRequestVolume volume = {0};
			volume.volume = ((CSliderUI*)msg.pSender)->GetValue();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_VOLUME_AUDIO_OUT, &volume, sizeof(volume));
		}
	}
}