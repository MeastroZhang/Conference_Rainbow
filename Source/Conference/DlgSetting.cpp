#include "StdAfx.h"
#include "DlgSetting.h"
#include "../../include/Util/Util.h"
#include <atlimage.h>

extern VidyoClientRequestConfiguration reqConfig;

CDlgSetting::CDlgSetting(HWND hParent)
{
	m_hParent = hParent;
}


CDlgSetting::~CDlgSetting(void)
{
	m_hParent = NULL;
}

LRESULT CDlgSetting::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDlgSetting::InitWindow()
{
	m_pOptCommon = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCommon")));
	m_pOptDevice = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optDevice")));
	m_pOptUpdate = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optUpdate"));
	m_pOptCommit = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCommit")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SETTING"), _T("optCommon"), szText, MAX_PATH);
	m_pOptCommon->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optDevice"), szText, MAX_PATH);
	m_pOptDevice->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optUpdate"), szText, MAX_PATH);
	m_pOptUpdate->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optCommit"), szText, MAX_PATH);
	m_pOptCommit->SetText(szText);

	m_pTabSetting = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabSetting")));

	CLabelUI* labAudio = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labAudio"));
	project_language_loadstring(_T("SETTING"), _T("labAudio"), szText, MAX_PATH);
	labAudio->SetText(szText);
	CLabelUI* labMic = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labMic"));
	project_language_loadstring(_T("SETTING"), _T("labMic"), szText, MAX_PATH);
	labMic->SetText(szText);
	CLabelUI* labMicVolume = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labMicVolume"));
	project_language_loadstring(_T("SETTING"), _T("labMicVolume"), szText, MAX_PATH);
	labMicVolume->SetText(szText);
	COptionUI* optAudioAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optAudioAGC"));
	project_language_loadstring(_T("SETTING"), _T("optAudioAGC"), szText, MAX_PATH);
	optAudioAGC->SetText(szText);
	CLabelUI* labSpeaker = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labSpeaker"));
	project_language_loadstring(_T("SETTING"), _T("labSpeaker"), szText, MAX_PATH);
	labSpeaker->SetText(szText);
	CLabelUI* labSpeakerVolume = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labSpeakerVolume"));
	project_language_loadstring(_T("SETTING"), _T("labSpeakerVolume"), szText, MAX_PATH);
	labSpeakerVolume->SetText(szText);
	CLabelUI* labVideo = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labVideo"));
	project_language_loadstring(_T("SETTING"), _T("labVideo"), szText, MAX_PATH);
	labVideo->SetText(szText);
	CLabelUI* labCamera = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labCamera"));
	project_language_loadstring(_T("SETTING"), _T("labCamera"), szText, MAX_PATH);
	labCamera->SetText(szText);
	COptionUI* optCapVideo720P = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optCapVideo720P"));
	project_language_loadstring(_T("SETTING"), _T("optCapVideo720P"), szText, MAX_PATH);
	optCapVideo720P->SetText(szText);

	CLabelUI* labCommon = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labCommon"));
	project_language_loadstring(_T("SETTING"), _T("labCommon"), szText, MAX_PATH);
	labCommon->SetText(szText);
	CLabelUI* labApply = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labApply"));
	project_language_loadstring(_T("SETTING"), _T("labApply"), szText, MAX_PATH);
	labApply->SetText(szText);
	COptionUI* optStartWithSystem = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optStartWithSystem"));
	project_language_loadstring(_T("SETTING"), _T("optStartWithSystem"), szText, MAX_PATH);
	optStartWithSystem->SetText(szText);
	COptionUI* optHideToTray = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optHideToTray"));
	project_language_loadstring(_T("SETTING"), _T("optHideToTray"), szText, MAX_PATH);
	optHideToTray->SetText(szText);
	COptionUI* optCopyUrl = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optCopyUrl"));
	project_language_loadstring(_T("SETTING"), _T("optCopyUrl"), szText, MAX_PATH);
	optCopyUrl->SetText(szText);
	COptionUI* optLeaveWithConfirm = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optLeaveWithConfirm"));
	project_language_loadstring(_T("SETTING"), _T("optLeaveWithConfirm"), szText, MAX_PATH);
	optLeaveWithConfirm->SetText(szText);
	CLabelUI* labShare = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labShare"));
	project_language_loadstring(_T("SETTING"), _T("labShare"), szText, MAX_PATH);
	labShare->SetText(szText);
	COptionUI* optDBScreen = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optDBScreen"));
	project_language_loadstring(_T("SETTING"), _T("optDBScreen"), szText, MAX_PATH);
	optDBScreen->SetText(szText);
	COptionUI* optFullScreen = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optFullScreen"));
	project_language_loadstring(_T("SETTING"), _T("optFullScreen"), szText, MAX_PATH);
	optFullScreen->SetText(szText);
	COptionUI* optShareFullScreen = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optShareFullScreen"));
	project_language_loadstring(_T("SETTING"), _T("optShareFullScreen"), szText, MAX_PATH);
	optShareFullScreen->SetText(szText);
	COptionUI* optShareAutoSize = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optShareAutoSize"));
	project_language_loadstring(_T("SETTING"), _T("optShareAutoSize"), szText, MAX_PATH);
	optShareAutoSize->SetText(szText);

	CLabelUI* labUpdate = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labUpdate"));
	project_language_loadstring(_T("SETTING"), _T("labUpdate"), szText, MAX_PATH);
	labUpdate->SetText(szText);
	CLabelUI* labLocalVersion = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labLocalVersion"));
	project_language_loadstring(_T("SETTING"), _T("labLocalVersion"), szText, MAX_PATH);
	TCHAR szVersion[MAX_PATH];
	CDuiString sVersion;
	ReadSetupIniString(L"VERSION", L"ver", NULL, szVersion, MAX_PATH);
	sVersion.Format(L"%s%s", szText, szVersion);
	labLocalVersion->SetText(sVersion);
	CLabelUI* labCurentVersion = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labCurentVersion"));
	project_language_loadstring(_T("SETTING"), _T("labCurentVersion"), szText, MAX_PATH);
	labCurentVersion->SetText(szText);
	CButtonUI* btnUpdate = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnUpdate"));
	project_language_loadstring(_T("SETTING"), _T("btnUpdate"), szText, MAX_PATH);
	btnUpdate->SetText(szText);

	CLabelUI* labCommit = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labCommit"));
	project_language_loadstring(_T("SETTING"), _T("labCommit"), szText, MAX_PATH);
	labCommit->SetText(szText);
	CLabelUI* labTips = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labTips"));
	project_language_loadstring(_T("SETTING"), _T("labTips"), szText, MAX_PATH);
	labTips->SetText(szText);
	CLabelUI* labTips1 = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labTips1"));
	project_language_loadstring(_T("SETTING"), _T("labTips1"), szText, MAX_PATH);
	labTips1->SetText(szText);
	CLabelUI* labTips2 = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labTips2"));
	project_language_loadstring(_T("SETTING"), _T("labTips2"), szText, MAX_PATH);
	labTips2->SetText(szText);
	CLabelUI* labTips3 = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labTips3"));
	project_language_loadstring(_T("SETTING"), _T("labTips3"), szText, MAX_PATH);
	labTips3->SetText(szText);
}

LRESULT CDlgSetting::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDlgSetting::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(0,0);

	return WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);
}

void CDlgSetting::OnSelectChanged( TNotifyUI &msg )
{
	if (m_pTabSetting == NULL)
	{
		return ;
	}

	if (msg.pSender->GetName() == _T("optDevice"))
	{
		m_pTabSetting->SelectItem(0);
	}
	else if (msg.pSender->GetName() == _T("optCommon"))
	{
		m_pTabSetting->SelectItem(1);
	}
	else if ( msg.pSender->GetName() == L"optUpdate")
	{
		TCHAR szText[MAX_PATH];
		CLabelUI* labCurentVersion = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labCurentVersion"));
		project_language_loadstring(_T("SETTING"), _T("labCurentVersion"), szText, MAX_PATH);
		labCurentVersion->SetText(szText);

		m_pTabSetting->SelectItem(2);
	}
	else if (msg.pSender->GetName() == _T("optCommit"))
	{
		m_pTabSetting->SelectItem(3);
	}

	InitUI();
}

void CDlgSetting::InitUI()
{
	COptionUI* optStartWithSystem = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optStartWithSystem"));
	int iSetting = ReadIniInt(L"SETTING", L"AutoStart", 0);
	optStartWithSystem->Selected((bool)iSetting);
	COptionUI* optHideToTray = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optHideToTray"));
	iSetting = ReadIniInt(L"SETTING", L"HideToTray", 1);
	optHideToTray->Selected((bool)iSetting);
	COptionUI* optLeaveWithConfirm = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optLeaveWithConfirm"));
	iSetting = ReadIniInt(L"SETTING", L"LeaveWithConfirm", 1);
	optLeaveWithConfirm->Selected((bool)iSetting);
	COptionUI* optDBScreen = static_cast<COptionUI*>(m_PaintManager.FindControl(L"optDBScreen"));
	iSetting = ReadIniInt(L"SETTING", L"DBScreen", 1);
	optDBScreen->Selected((bool)iSetting);

	TCHAR szName[MAX_PATH];
	string sName = "Same as system";
	CDuiString sDevName;
	int iError = VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	CComboUI* pComboMic = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comMicDevice")));
	pComboMic->RemoveAll();
	for (unsigned int iMic = 0; iMic < reqConfig.numberMicrophones; iMic++)
	{
		CListLabelElementUI* pLabMic = new CListLabelElementUI;
		if (!memcmp(reqConfig.microphones[iMic], sName.c_str(), sName.length()))
		{
			project_language_loadstring(_T("SETTING"), _T("labDev0"), szName, MAX_PATH);
		} 
		else
		{
			ChangUtf8ToText((char*)(reqConfig.microphones[iMic]), szName);
		}
		sDevName.Format(L"%s", szName);
		pLabMic->SetText(sDevName);
		pComboMic->Add(pLabMic);
	}
	pComboMic->SelectItem(reqConfig.currentMicrophone);

	CComboUI* pComboSpeaker = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comSpeakerDevice")));
	pComboSpeaker->RemoveAll();
	for (unsigned int iSpk = 0; iSpk < reqConfig.numberSpeakers; iSpk++)
	{
		CListLabelElementUI* pLabSpeaker = new CListLabelElementUI;
		if (!memcmp(reqConfig.speakers[iSpk], sName.c_str(), sName.length()))
		{
			project_language_loadstring(_T("SETTING"), _T("labDev0"), szName, MAX_PATH);
		} 
		else
		{
			ChangUtf8ToText((char*)(reqConfig.speakers[iSpk]), szName);
		}
		sDevName.Format(L"%s", szName);
		pLabSpeaker->SetText(sDevName);
		pComboSpeaker->Add(pLabSpeaker);
	}
	pComboSpeaker->SelectItem(reqConfig.currentSpeaker);

	CComboUI* pComboCamera = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comCameraDevice")));
	pComboCamera->RemoveAll();
	for (unsigned int iCam = 0; iCam < reqConfig.numberCameras; iCam++)
	{
		CListLabelElementUI* pLabCam = new CListLabelElementUI;
		ChangUtf8ToText((char*)(reqConfig.cameras[iCam]), szName);
		pLabCam->SetText(szName);
		pComboCamera->Add(pLabCam);
	}
	pComboCamera->SelectItem(reqConfig.currentCamera);

	//输入音量
	CSliderUI* pSliMicVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderMicVolume")));
	VidyoClientRequestVolume volumeIn = {0};
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_VOLUME_AUDIO_IN, &volumeIn, sizeof(volumeIn));
	pSliMicVolume->SetValue(volumeIn.volume);

	if (reqConfig.enableAudioAGC) pSliMicVolume->SetValue(0);
	pSliMicVolume->SetEnabled(!reqConfig.enableAudioAGC);

	COptionUI* pOptAudioAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAudioAGC")));
	pOptAudioAGC->Selected(reqConfig.enableAudioAGC && true);

	//输出音量
	CSliderUI* pSliVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderSpeakerVolume")));
	VidyoClientRequestVolume volumeOut = {0};
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_VOLUME_AUDIO_OUT, &volumeOut, sizeof(volumeOut));
	pSliVolume->SetValue(volumeOut.volume);

	COptionUI* poptCapVideo720P = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCapVideo720P")));
	poptCapVideo720P->Selected( (reqConfig.videoPreferences == VIDYO_CLIENT_VIDEO_PREFERENCES_ADVANCED_720p30)?true:false );
}

void CDlgSetting::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == _T("btnClose") ) 
		{
			Close(0);
		}
		else if (msg.pSender->GetName() == L"optDBScreen")
		{
			COptionUI* optDBScreen = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optDBScreen")));
			WriteIniInt(L"SETTING", L"DBScreen", optDBScreen->IsSelected());
		}
		else if (msg.pSender->GetName() == L"optLeaveWithConfirm")
		{
			COptionUI* optLeaveWithConfirm = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optLeaveWithConfirm")));
			WriteIniInt(L"SETTING", L"LeaveWithConfirm", !optLeaveWithConfirm->IsSelected());
		}
		else if (msg.pSender->GetName() == L"optHideToTray")
		{
			COptionUI* optHideToTray = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optHideToTray")));
			WriteIniInt(L"SETTING", L"HideToTray", !optHideToTray->IsSelected());
		}
		else if (msg.pSender->GetName() == L"optStartWithSystem")
		{
			COptionUI* optStartWithSystem = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optStartWithSystem")));
			WriteIniInt(L"SETTING", L"AutoStart", !optStartWithSystem->IsSelected());

			//(!optStartWithSystem->IsSelected())?AddAutoRun():DeleteAutoRun();
		}
		else if ( msg.pSender->GetName() == _T("optAudioAGC") )
		{
			COptionUI* pOptAudioAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAudioAGC")));
			reqConfig.enableAudioAGC = !pOptAudioAGC->IsSelected();
			if (VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig)) == VIDYO_FALSE)
			{
				VidyoClientRequestVolume volume = {0};
				VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_VOLUME_AUDIO_IN, &volume, sizeof(volume));
				unsigned int iVolume = (reqConfig.enableAudioAGC)?0:volume.volume;
				CSliderUI* pSliMicVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderMicVolume")));
				pSliMicVolume->SetEnabled(!reqConfig.enableAudioAGC);
				pSliMicVolume->SetValue(iVolume);
			}
		}
		else if ( msg.pSender->GetName() == _T("optCapVideo720P") )
		{
			COptionUI* poptCapVideo720P = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCapVideo720P")));
			reqConfig.videoPreferences = (!poptCapVideo720P->IsSelected())?VIDYO_CLIENT_VIDEO_PREFERENCES_ADVANCED_720p30:VIDYO_CLIENT_VIDEO_PREFERENCES_BEST_QUALITY;
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
		else if ( msg.pSender->GetName() == L"btnUpdate" )
		{
			TCHAR szExePath[MAX_PATH];
			GetFileName(L"\\AutoUpdater\\AutoUpdater.exe", szExePath);
			ShellExecute(NULL,_T("open"),szExePath,NULL,NULL,SW_SHOW);
			::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntSetting, NULL);
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		OnSelectChanged(msg);
	}
	else if (msg.sType == _T("valuechanged"))
	{
		if (msg.pSender->GetName() == _T("sliderMicVolume"))
		{
			VidyoClientRequestVolume volume = {0};
			volume.volume = ((CSliderUI*)msg.pSender)->GetValue();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_VOLUME_AUDIO_IN, &volume, sizeof(volume));
		}
		else if (msg.pSender->GetName() == _T("sliderSpeakerVolume"))
		{
			VidyoClientRequestVolume volume = {0};
			volume.volume = ((CSliderUI*)msg.pSender)->GetValue();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_VOLUME_AUDIO_OUT, &volume, sizeof(volume));
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
		else if (msg.pSender->GetName() == _T("comSpeakerDevice"))
		{
			if (i != reqConfig.currentSpeaker)
				reqConfig.currentSpeaker = i;
		}
		else if (msg.pSender->GetName() == _T("comCameraDevice"))
		{
			if (i != reqConfig.currentCamera)
				reqConfig.currentCamera = i;
		}
		VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	}
}