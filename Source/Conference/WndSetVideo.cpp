#include "StdAfx.h"
#include "WndSetVideo.h"
#include "../../include/Util/Util.h"

extern VidyoClientRequestConfiguration reqConfig;

CWndSetVideo::CWndSetVideo(void)
{
}


CWndSetVideo::~CWndSetVideo(void)
{
}

LRESULT CWndSetVideo::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndSetVideo::InitWindow()
{
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
	CLabelUI* labCamera = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCamera")));
	COptionUI* optSelfView = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optSelfView")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SETVIDEO"), _T("labCaption"), szText, MAX_PATH);
	labCaption->SetText(szText);
	project_language_loadstring(_T("SETVIDEO"), _T("labCamera"), szText, MAX_PATH);
	labCamera->SetText(szText);
	project_language_loadstring(_T("SETVIDEO"), _T("optSelfView"), szText, MAX_PATH);
	optSelfView->SetText(szText);
}

void CWndSetVideo::UpdataUI()
{
	TCHAR szName[128];

	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));

	CComboUI* pComboCamera = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comCameraDevice")));
	pComboCamera->RemoveAll();
	for (unsigned int i = 0; i < reqConfig.numberCameras; i++)
	{
		CListLabelElementUI* pLabCam = new CListLabelElementUI;
		ChangUtf8ToText((char*)(reqConfig.cameras[i]), szName);
		pLabCam->SetText(szName);
		pComboCamera->Add(pLabCam);
	}
	pComboCamera->SelectItem(reqConfig.currentCamera);

	VidyoClientInEventPreview inEventPreview;
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_PREVIEW_MODE , &inEventPreview, sizeof(inEventPreview));

	COptionUI* optSelfView = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optSelfView")));
	optSelfView->Selected(inEventPreview.previewMode && true);
}

void CWndSetVideo::Notify(TNotifyUI& msg)
{
	if ( msg.sType == _T("windowinit") )
	{
		UpdataUI();
	}
	else if ( msg.sType == _T("click") )
	{
		if ( msg.pSender->GetName() == _T("optSelfView") )
		{
			VidyoClientInEventPreview preview = {VIDYO_CLIENT_PREVIEW_MODE_NONE};
			COptionUI* optSelfView = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optSelfView")));
			if ( !optSelfView->IsSelected() )
				preview.previewMode = VIDYO_CLIENT_PREVIEW_MODE_DOCK;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_PREVIEW, &preview, sizeof(preview));
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		int i = ((CComboUI*)msg.pSender)->GetCurSel();
		if (msg.pSender->GetName() == _T("comCameraDevice"))
		{
			if (i != reqConfig.currentCamera)
				reqConfig.currentCamera = i;
		}
		VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	}
}