#include "StdAfx.h"
#include "DlgAVSetting.h"
#include "../../include/Util/Util.h"

extern VidyoClientRequestConfiguration reqConfig;

CDlgAVSetting::CDlgAVSetting(void)
	: m_pMixerInfoSpeaker(NULL)
	, m_pMixerInfoMIC(NULL)
	, m_pWaveIn(NULL)
{
	m_iSpeakerTestCount = 0;
	m_pVideoCapture = NULL;
}


CDlgAVSetting::~CDlgAVSetting(void)
{
	
}

LRESULT CDlgAVSetting::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDlgAVSetting::InitWindow()
{
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

	TCHAR szText[MAX_PATH];
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
}

LRESULT CDlgAVSetting::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CDlgAVSetting::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(0,0);

	return WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);
}

LRESULT CDlgAVSetting::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_AUDIO_TEST)
	{
		//VidyoClientRequestVolume volume = {0};
		//VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_VOLUME_AUDIO_IN, &volume, sizeof(volume));
		//m_pProMicTest->SetValue(volume.volume/*m_pWaveIn->GetVolume()*/);
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

LRESULT CDlgAVSetting::OnClose(UINT , WPARAM , LPARAM , BOOL& bHandled)
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

void CDlgAVSetting::OnSelectChanged( TNotifyUI &msg )
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

	InitUI();
}

void CDlgAVSetting::InitUI()
{
	TCHAR szName[128];
	int iError = VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
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

	//输入音量
	CSliderUI* pSliMicVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderMicVolume")));
	VidyoClientRequestVolume volume = {0};
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_VOLUME_AUDIO_IN, &volume, sizeof(volume));
	pSliMicVolume->SetValue(volume.volume);

	if (reqConfig.enableAudioAGC) pSliMicVolume->SetValue(0);
	pSliMicVolume->SetEnabled(!reqConfig.enableAudioAGC);

	COptionUI* pOptAudioAGC = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAudioAGC")));
	pOptAudioAGC->Selected(reqConfig.enableAudioAGC && true);

	//输出音量
	CSliderUI* pSliVolume = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderSpeakerVolume")));
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_VOLUME_AUDIO_OUT, &volume, sizeof(volume));
	pSliVolume->SetValue(volume.volume);

	COptionUI* poptCapVideo720P = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optCapVideo720P")));
	poptCapVideo720P->Selected( (reqConfig.videoPreferences == VIDYO_CLIENT_VIDEO_PREFERENCES_ADVANCED_720p30)?true:false );
}

void CDlgAVSetting::OnBtnTestMic()
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

void CDlgAVSetting::OnBtnTestSpeaker()
{
	PlaySound(TEXT("test.wav"),NULL,SND_FILENAME|SND_ASYNC);

	SetTimer(m_hWnd, TIMER_SPEAKER_TEST, 200, NULL);
}

void CDlgAVSetting::OnBtnTestCamera(TCHAR* deviceName)
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
		m_pVideoCapture->OpenCamera(m_hWnd, deviceName, 40, 130, 220, 150);
		project_language_loadstring(_T("SETTING"), _T("btnTestCameraClose"), szText, MAX_PATH);
	}
	pbtnTestCamera->SetText(szText);
}

void CDlgAVSetting::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == _T("btnClose") ) 
		{
			Close(0);
		}
		else if ( msg.pSender->GetName() == _T("btnTestMic") )
		{
			VidyoClientInEventPrecallTestMicrophone testMic = {VIDYO_CLIENT_DEVICE_TEST_START};
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_PRECALL_TEST_MICROPHONE, &testMic, sizeof(testMic));
			//OnBtnTestMic();
			SetTimer(m_hWnd, TIMER_AUDIO_TEST, 16, NULL);
		}
		else if ( msg.pSender->GetName() == _T("btnTestCamera") )
		{
			TCHAR szName[128];
			ChangUtf8ToText((char*)(reqConfig.cameras[reqConfig.currentCamera]), szName);
			OnBtnTestCamera(szName);
		}
		else if ( msg.pSender->GetName() == _T("btnTestSpeaker") )
		{
			OnBtnTestSpeaker();
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

			if (m_pVideoCapture)
			{
				TCHAR szName[128];
				ChangUtf8ToText((char*)(reqConfig.cameras[reqConfig.currentCamera]), szName);
				m_pVideoCapture->OpenCamera(m_hWnd, szName, 40, 130, 220, 150);
			}
		}
		VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	}
}