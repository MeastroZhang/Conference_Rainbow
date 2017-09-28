#include "StdAfx.h"
#include "WndSetMic.h"
#include "../../include/Util/Util.h"

extern VidyoClientRequestConfiguration reqConfig;

CWndSetMic::CWndSetMic(void)
{
	m_pMixerInfoMIC = NULL;
}


CWndSetMic::~CWndSetMic(void)
{
}

LRESULT CWndSetMic::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndSetMic::InitWindow()
{
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
	CLabelUI* labMic = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labMic")));
	CLabelUI* plabMicVolume = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labMicVolume")));
	COptionUI* optAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAGC")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SETMIC"), _T("labCaption"), szText, MAX_PATH);
	labCaption->SetText(szText);
	project_language_loadstring(_T("SETMIC"), _T("labMic"), szText, MAX_PATH);
	labMic->SetText(szText);
	project_language_loadstring(_T("SETMIC"), _T("labMicVolume"), szText, MAX_PATH);
	plabMicVolume->SetText(szText);
	project_language_loadstring(_T("SETMIC"), _T("optAGC"), szText, MAX_PATH);
	optAGC->SetText(szText);
}

void CWndSetMic::UpdataUI()
{
	TCHAR szName[128] = {0};

	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));

	COptionUI* optAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAGC")));
	optAGC->Selected(reqConfig.enableAudioAGC && true);

	CComboUI* pComboMic = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comMicDevice")));
	pComboMic->RemoveAll();
	for (unsigned int i = 0; i < reqConfig.numberMicrophones; i++)
	{
		CListLabelElementUI* pLabMic = new CListLabelElementUI;
		ChangUtf8ToText((char*)(reqConfig.microphonePhyDeviceName[i]), szName);
		pLabMic->SetText(szName);
		pComboMic->Add(pLabMic);
	}
	pComboMic->SelectItem(reqConfig.currentMicrophone);

	VidyoClientRequestVolume volume = {37};
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_VOLUME_AUDIO_IN, &volume, sizeof(volume));
	CSliderUI* pSliMicVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderMicVolume")));
	pSliMicVolume->SetValue(volume.volume);
	pSliMicVolume->SetEnabled(!reqConfig.enableAudioAGC);
}

void CWndSetMic::Notify(TNotifyUI& msg)
{
	if ( msg.sType == _T("click") )
	{
		if ( msg.pSender->GetName() == _T("optAGC") )
		{
			COptionUI* optAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAGC")));
			reqConfig.enableAudioAGC = !optAGC->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));

			CSliderUI* pSliMicVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderMicVolume")));
			pSliMicVolume->SetEnabled(!reqConfig.enableAudioAGC);
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		int i = ((CComboUI*)msg.pSender)->GetCurSel();
		if (msg.pSender->GetName() == _T("comMicDevice"))
		{
			if (i != reqConfig.currentMicrophone)
				reqConfig.currentMicrophone = i;
		}
		VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	}
	else if (msg.sType == _T("valuechanged"))
	{
		if (msg.pSender->GetName() == _T("sliderMicVolume"))
		{
			VidyoClientRequestVolume volume = {0};
			volume.volume = ((CSliderUI*)msg.pSender)->GetValue();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_VOLUME_AUDIO_IN, &volume, sizeof(volume));
		}
	}
}