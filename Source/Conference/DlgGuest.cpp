#include "StdAfx.h"
#include "DlgGuest.h"
#include "../../include/Util/Util.h"

extern VidyoClientRequestConfiguration reqConfig;

CDlgGuest::CDlgGuest(HWND hParent)
	: m_pMixerInfoSpeaker(NULL)
	, m_pMixerInfoMIC(NULL)
	, m_pWaveIn(NULL)
{
	m_hParent = hParent;
	m_strPortal = _T("");
	m_strDisplayName = _T("");
	m_strRoomID = _T("");
	m_bShowAVSetting = false;
	m_bLogin = false;
	m_iSpeakerTestCount = 0;
	m_pVideoCapture = NULL;
}


CDlgGuest::~CDlgGuest()
{
	m_hParent = NULL;
}

LRESULT CDlgGuest::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT CDlgGuest::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_TIMER:		   lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CDlgGuest::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnClose")));

	CButtonUI* pBtnToggle = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnToggle")));
	CButtonUI* pBtnJoin = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnJoin")));
	CEditUI* pEditDisplayName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editDisplayName")));
	CEditUI* pEditUsername = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editUsername")));
	CEditUI* pEditUserpass = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPSW")));
	if (pEditUserpass) pEditUserpass->SetPasswordMode(true);

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("GUEST"), _T("btnToggleLogin"), szText, MAX_PATH);	
	pBtnToggle->SetText(szText);
	project_language_loadstring(_T("GUEST"), _T("btnJoin"), szText, MAX_PATH);	
	pBtnJoin->SetText(szText);
	TCHAR UserName[MAX_PATH];
	DWORD Size = MAX_PATH;
	::GetUserName(UserName,&Size);
	CDuiString strDisplayName;
	strDisplayName.Format(_T("%s"), UserName);
	pEditDisplayName->SetText(strDisplayName);

	TCHAR strText[255]=_T("");
	ReadIniString(_T("USERINFO"), _T("UserName"), _T(""), strText, 255);
	pEditUsername->SetText(strText);
	ReadIniString(_T("USERINFO"), _T("Password"), _T(""), strText, 255);
	pEditUserpass->SetText(strText);

	COptionUI* pOptVideo = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optVideo")));
	COptionUI* pOptAudio = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAudio")));
	CLabelUI* plabMic = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labMic")));
	CLabelUI* plabMicVolume = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labMicVolume")));
	COptionUI* pOptAudioAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAudioAGC")));
	CLabelUI* plabTestMicTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labTestMicTip")));
	CButtonUI* pbtnTestMic = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnTestMic")));
	m_pProMicTest = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("proMicTest")));
	CLabelUI* plabSpeaker = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labSpeaker")));
	CLabelUI* plabSpeakerVolume = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labSpeakerVolume")));
	CLabelUI* plabTestSpeakerTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labTestSpeakerTip")));
	CButtonUI* pbtnTestSpeaker = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnTestSpeaker")));
	m_pProSpeakerTest = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("proSpeakerTest")));

	project_language_loadstring(_T("SETTING"), _T("optVideo"), szText, MAX_PATH);
	pOptVideo->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optAudio"), szText, MAX_PATH);
	pOptAudio->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("labMic"), szText, MAX_PATH);
	plabMic->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("labMicVolume"), szText, MAX_PATH);
	plabMicVolume->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optAutoAdjustMic"), szText, MAX_PATH);
	pOptAudioAGC->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("labTestMicTip"), szText, MAX_PATH);
	plabTestMicTip->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("btnTestMic"), szText, MAX_PATH);
	pbtnTestMic->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("labSpeaker"), szText, MAX_PATH);
	plabSpeaker->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("labSpeakerVolume"), szText, MAX_PATH);
	plabSpeakerVolume->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("labTestSpeakerTip"), szText, MAX_PATH);
	plabTestSpeakerTip->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("btnTestSpeaker"), szText, MAX_PATH);
	pbtnTestSpeaker->SetText(szText);

	CLabelUI* plabCamera = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCamera")));
	CButtonUI* pbtnTestCamera = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnTestCamera")));
	COptionUI* poptCapVideo720P = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCapVideo720P")));

	project_language_loadstring(_T("SETTING"), _T("labCamera"), szText, MAX_PATH);
	plabCamera->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("btnTestCameraOpen"), szText, MAX_PATH);
	pbtnTestCamera->SetText(szText);
	project_language_loadstring(_T("SETTING"), _T("optCapVideo720P"), szText, MAX_PATH);
	poptCapVideo720P->SetText(szText);

	m_pTabAVSetting = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabSetting")));

	if (NULL == m_pMixerInfoMIC)
		m_pMixerInfoMIC = new CMixerInfo(m_hWnd, COMPONENTTYPE_DST_WAVEIN, 0, 100);

	//输入音量
	int nVolumeValue = 0;
	nVolumeValue = m_pMixerInfoMIC->GetVolumeValue();
	CSliderUI* pSliMicVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderMicVolume")));
	if (nVolumeValue>100)
		pSliMicVolume->SetValue(100);
	else
		pSliMicVolume->SetValue(nVolumeValue);

	if (NULL == m_pMixerInfoSpeaker)
		m_pMixerInfoSpeaker = new CMixerInfo(m_hWnd, COMPONENTTYPE_DST_SPEAKERS, 0, 100);

	//输出音量
	nVolumeValue = m_pMixerInfoSpeaker->GetVolumeValue();

	CSliderUI* pSliVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderSpeakerVolume")));
	if (nVolumeValue>100)
		pSliVolume->SetValue(100);
	else
		pSliVolume->SetValue(nVolumeValue);
}

void CDlgGuest::UpdataUI()
{
	CVerticalLayoutUI* pVerAVSetting = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("verAVSetting")));
	pVerAVSetting->SetVisible(m_bShowAVSetting);
	SetWindowPos(m_hWnd, NULL, 0, 0, 350+m_bShowAVSetting*300, 500, SWP_NOMOVE);

	if (m_bShowAVSetting)
	{
		TCHAR szName[128];
		VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));

		COptionUI* pOptAudioAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAudioAGC")));
		pOptAudioAGC->Selected(reqConfig.enableAudioAGC && true);

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

		CSliderUI* pSliMicVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderMicVolume")));
		pSliMicVolume->SetEnabled(!reqConfig.enableAudioAGC);

		COptionUI* poptCapVideo720P = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCapVideo720P")));
		poptCapVideo720P->Selected( (reqConfig.videoPreferences == VIDYO_CLIENT_VIDEO_PREFERENCES_ADVANCED_720p30)?true:false );
	}
	
	CButtonUI* pBtnToggle = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnToggle")));
	TCHAR szText[MAX_PATH];
	if (m_bLogin)
	{
		CVerticalLayoutUI* pVerLoginEdit = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("verLoginEdit")));
		pVerLoginEdit->SetVisible();
		CEditUI* pEditDisplayName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editDisplayName")));
		pEditDisplayName->SetVisible(false);

		project_language_loadstring(_T("GUEST"), _T("btnToggleGuest"), szText, MAX_PATH);	
	}
	else
	{
		CVerticalLayoutUI* pVerLoginEdit = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("verLoginEdit")));
		pVerLoginEdit->SetVisible(false);
		CEditUI* pEditDisplayName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editDisplayName")));
		pEditDisplayName->SetVisible();

		project_language_loadstring(_T("GUEST"), _T("btnToggleLogin"), szText, MAX_PATH);
	}
	pBtnToggle->SetText(szText);
}

LRESULT CDlgGuest::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_AUDIO_TEST && m_pWaveIn)
	{
		m_pProMicTest->SetValue((int)(m_pWaveIn->GetVolume()));
	}

	if (wParam == TIMER_SPEAKER_TEST)
	{
		m_iSpeakerTestCount++;
		m_pProSpeakerTest->SetValue(rand()%100);
		if (m_iSpeakerTestCount == 25)
		{
			m_iSpeakerTestCount = 0;
			KillTimer(m_hWnd, TIMER_SPEAKER_TEST);
			m_pProSpeakerTest->SetValue(0);
		}
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CDlgGuest::OnClose(UINT , WPARAM , LPARAM , BOOL& bHandled)
{
	if (m_pMixerInfoSpeaker)
	{
		delete m_pMixerInfoSpeaker;
		m_pMixerInfoSpeaker = NULL;
	}

	if (m_pMixerInfoMIC)
	{
		delete m_pMixerInfoMIC;
		m_pMixerInfoMIC = NULL;
	}

	if (m_pWaveIn)
	{
		if (m_pWaveIn->IsOpen())
			m_pWaveIn->CloseMic();

		delete m_pWaveIn;
		m_pWaveIn = NULL;
	}

	if (m_pVideoCapture)
	{
		m_pVideoCapture->CloseCamera();
		delete  m_pVideoCapture;
		m_pVideoCapture = NULL;
	}

	bHandled = FALSE;
	return 0;
}

void CDlgGuest::SetGuestInfo(CDuiString strURL, CDuiString strName, CDuiString strID)
{
	m_strRoomID = strID;
	CLabelUI* pLabConfName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labRoomName")));
	CLabelUI* pLabConfID = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labRoomID")));
	pLabConfName->SetText(strName);

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("GUEST"), _T("labRoomID"), szText, MAX_PATH);
	strID.Format(_T("%s%s"), szText, strID);
	pLabConfID->SetText(strID);

	UpdataUI();
}

void CDlgGuest::JoinRoom()
{
	VidyoClientInEventRoomLink roomLink = {0};

	ChangTextToUtf8(m_strDisplayName, roomLink.displayName);
	//memcpy(roomLink.displayName, szName, sizeof(roomLink.displayName));
	ChangTextToUtf8(PORTALURL, roomLink.portalUri);
	//memcpy(roomLink.portalUri, PORTALURL, sizeof(roomLink.portalUri));
	ChangTextToUtf8(m_strRoomID, roomLink.roomKey);
	//memcpy(roomLink.roomKey, m_strRoomID, sizeof(roomLink.roomKey));
	memcpy(roomLink.pin, _T(""), sizeof(roomLink.pin));

	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_ROOM_LINK, &roomLink, sizeof(roomLink)) != VIDYO_FALSE)
	{
		int i = 0;
	}
}

void CDlgGuest::OnSelectChanged( TNotifyUI &msg )
{
	if (msg.pSender->GetName() == _T("optAudioAGC") || msg.pSender->GetName() == _T("optCapVideo720P"))
	{
		return ;
	}

	if (m_pTabAVSetting == NULL) return ;

	if (m_pVideoCapture)
	{
		if (m_pVideoCapture->fPreviewing)
		{
			OnBtnTestCamera(NULL);
		}
	}

	if (msg.pSender->GetName() == _T("optAudio"))
	{
		m_pTabAVSetting->SelectItem(0);
	}
	else if (msg.pSender->GetName() == _T("optVideo"))
	{
		m_pTabAVSetting->SelectItem(1);
	}

	UpdataUI();
}

void CDlgGuest::OnBtnTestCamera(TCHAR* deviceName)
{
	if (NULL == m_pVideoCapture)
		m_pVideoCapture = new CVideoCapture;

	CButtonUI* pbtnTestCamera = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnTestCamera")));
	TCHAR szText[MAX_PATH];
	if (m_pVideoCapture->fPreviewing)
	{
		m_pVideoCapture->CloseCamera();
		delete  m_pVideoCapture;
		m_pVideoCapture = NULL;

		project_language_loadstring(_T("SETTING"), _T("btnTestCameraOpen"), szText, MAX_PATH);
	}
	else
	{
		m_pVideoCapture->OpenCamera(m_hWnd, deviceName, 390, 140, 220, 150);
		project_language_loadstring(_T("SETTING"), _T("btnTestCameraClose"), szText, MAX_PATH);
	}
	pbtnTestCamera->SetText(szText);
}

void CDlgGuest::OnBtnTestMic()
{
	if (m_pWaveIn == NULL)
	{
		m_pWaveIn = new CWaveIn;
	}

	if (m_pWaveIn->IsOpen())
	{
		m_pWaveIn->CloseMic();
		KillTimer(m_hWnd, TIMER_AUDIO_TEST);
		m_pProMicTest->SetValue(0);
	}
	else
	{
		m_pWaveIn->OpenMic(reqConfig.currentMicrophone);
		SetTimer(m_hWnd, TIMER_AUDIO_TEST, 16, NULL);
	}
}

void CDlgGuest::OnBtnTestSpeaker()
{
	PlaySound(TEXT("test.wav"),NULL,SND_FILENAME|SND_ASYNC);

	SetTimer(m_hWnd, TIMER_SPEAKER_TEST, 200, NULL);
}

void CDlgGuest::Notify(TNotifyUI& msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender == m_pCloseBtn ) 
		{
			::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntGuestUI, NULL);
		}
		else if (strControlName == _T("btnToggle"))
		{
			m_bLogin = !m_bLogin;
			UpdataUI();
		}
		else if ( strControlName == _T("btnTestMic") )
		{
			OnBtnTestMic();
			SetTimer(m_hWnd, TIMER_AUDIO_TEST, 16, NULL);
		}
		else if ( strControlName == _T("btnTestCamera") )
		{
			TCHAR szName[128];
			ChangUtf8ToText((char*)(reqConfig.cameras[reqConfig.currentCamera]), szName);
			OnBtnTestCamera(szName);
		}
		else if ( strControlName == _T("btnTestSpeaker") )
		{
			OnBtnTestSpeaker();
		}
		else if ( msg.pSender->GetName() == _T("optAudioAGC") )
		{
			COptionUI* pOptAudioAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAudioAGC")));
			reqConfig.enableAudioAGC = !pOptAudioAGC->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));

			CSliderUI* pSliMicVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderMicVolume")));
			pSliMicVolume->SetEnabled(!reqConfig.enableAudioAGC);
		}
		else if ( msg.pSender->GetName() == _T("optCapVideo720P") )
		{
			COptionUI* poptCapVideo720P = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCapVideo720P")));
			reqConfig.videoPreferences = (!poptCapVideo720P->IsSelected())?VIDYO_CLIENT_VIDEO_PREFERENCES_ADVANCED_720p30:VIDYO_CLIENT_VIDEO_PREFERENCES_BEST_QUALITY;
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
		else if (strControlName == _T("btnJoin"))
		{
			CEditUI* pEditDisplayName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editDisplayName")));
			m_strDisplayName = pEditDisplayName->GetText();
			JoinRoom();
		}
		else if (strControlName == _T("btnHideSetting"))
		{
			CVerticalLayoutUI* pVerAVSetting = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("verAVSetting")));
			m_bShowAVSetting = !m_bShowAVSetting;
			if (m_bShowAVSetting)
			{
				pVerAVSetting->SetVisible();
				UpdataUI();
				::SetWindowPos(m_hWnd, NULL, 0, 0, 650, 500, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
				RECT rc = {0, 0, 350, 200};
				m_PaintManager.SetCaptionRect(rc);
			}
			else
			{
				pVerAVSetting->SetVisible(false);
				::SetWindowPos(m_hWnd, NULL, 0, 0, 350, 500, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
				RECT rc = {0, 0, 0, 200};
				m_PaintManager.SetCaptionRect(rc);
			}
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
			m_pMixerInfoMIC->SetVolumeValue(((CSliderUI*)msg.pSender)->GetValue());
		}
		else if (msg.pSender->GetName() == _T("sliderSpeakerVolume"))
		{
			m_pMixerInfoSpeaker->SetVolumeValue(((CSliderUI*)msg.pSender)->GetValue());
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

			if (m_pVideoCapture)
			{
				TCHAR szName[128];
				ChangUtf8ToText((char*)(reqConfig.cameras[reqConfig.currentCamera]), szName);
				m_pVideoCapture->OpenCamera(m_hWnd, szName, 390, 140, 220, 150);
			}
		}
		VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	}
}