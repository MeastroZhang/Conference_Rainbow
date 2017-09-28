#include "StdAfx.h"
#include "WndToolbar.h"
#include "WSgSoap.h"

extern tSelfControlStatus g_tBtnStatus;
extern tStartTime g_StartTime;

CWndToolbar::CWndToolbar(HWND hParent)
{
	m_hParent = hParent;
	m_bTimer = false;
	StartTime.wHour = 0;
	StartTime.wMinute = 0;
	StartTime.wSecond = 0;

	m_pWndSetPhoto = NULL;
	m_pWndSetVideo = NULL;
	m_pWndSetMic = NULL;
	m_pWndSetSpeaker = NULL;

	m_bSetUIShow = FALSE;
	m_eLectureStage = VIDYO_CLIENT_LECTURE_MODE_STOP;
}


CWndToolbar::~CWndToolbar(void)
{
	m_hParent = NULL;
}

LRESULT CWndToolbar::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndToolbar::InitWindow()
{
	CButtonUI* pBtnExit = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnExit")));
	m_pBtnViedo = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnVideo")));
	m_pBtnVideoClosed = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnVideoClosed")));
	m_pBtnMic = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnMic")));
	m_pBtnMicClosed = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnMicClosed")));
	m_pBtnSpeaker = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnSpeaker")));
	m_pBtnSpeakerClosed = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnSpeakerClosed")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("TOOLBAR"), _T("btnExit"), szText, MAX_PATH);	
	pBtnExit->SetToolTip(szText);
	project_language_loadstring(_T("TOOLBAR"), _T("btnVideo"), szText, MAX_PATH);
	m_pBtnViedo->SetToolTip(szText);
	project_language_loadstring(_T("TOOLBAR"), _T("btnVideoClosed"), szText, MAX_PATH);
	m_pBtnVideoClosed->SetToolTip(szText);
	project_language_loadstring(_T("TOOLBAR"), _T("btnMic"), szText, MAX_PATH);
	m_pBtnMic->SetToolTip(szText);
	project_language_loadstring(_T("TOOLBAR"), _T("btnMicClosed"), szText, MAX_PATH);
	m_pBtnMicClosed->SetToolTip(szText);
	project_language_loadstring(_T("TOOLBAR"), _T("btnSpeaker"), szText, MAX_PATH);
	m_pBtnSpeaker->SetToolTip(szText);
	project_language_loadstring(_T("TOOLBAR"), _T("btnSpeakerClosed"), szText, MAX_PATH);
	m_pBtnSpeakerClosed->SetToolTip(szText);

	SetTimer(m_hWnd, TIMER_CLOCK, 1000, NULL);

	if (m_pWndSetPhoto == NULL)
	{
		m_pWndSetPhoto = new CWndSetPhoto;
		m_pWndSetPhoto->Create(m_hWnd, _T("SetPhoto"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
	}
	if (m_pWndSetPhoto) m_pWndSetPhoto->ShowWindow(false);

	if (m_pWndSetVideo == NULL)
	{
		m_pWndSetVideo = new CWndSetVideo;
		m_pWndSetVideo->Create(m_hWnd, _T("SetVideo"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
	}
	if (m_pWndSetVideo) m_pWndSetVideo->ShowWindow(false);

	if (m_pWndSetMic == NULL)
	{
		m_pWndSetMic = new CWndSetMic;
		m_pWndSetMic->Create(m_hWnd, _T("SetMic"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
	}
	if (m_pWndSetMic) m_pWndSetMic->ShowWindow(false);

	if (m_pWndSetSpeaker == NULL)
	{
		m_pWndSetSpeaker = new CWndSetSpeaker;
		m_pWndSetSpeaker->Create(m_hWnd, _T("SetSpeaker"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
	}
	if (m_pWndSetSpeaker) m_pWndSetSpeaker->ShowWindow(false);
}

LRESULT CWndToolbar::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CWndToolbar::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_CLOCK)
	{
		CalculateClock();
	}

	bHandled = FALSE;
	return 0;
}

void CWndToolbar::ShowUI(bool bShow /* = true */)
{
	if (bShow)
	{
		RECT   rc; 
		GetClientRect(GetParent(m_hWnd), &rc);
		::SetWindowPos(m_hWnd, NULL, 0, rc.bottom-60, rc.right, rc.bottom, NULL);
		UpDateUI();
	} 
	else
	{
		HideAllSetUI();
	}

	ShowWindow(bShow);
}

void CWndToolbar::HideAllSetUI()
{
	m_pWndSetPhoto->ShowWindow(false);
	m_pWndSetVideo->ShowWindow(false);
	m_pWndSetMic->ShowWindow(false);
	m_pWndSetSpeaker->ShowWindow(false);

	m_bSetUIShow = FALSE;
}

BOOL CWndToolbar::IsSetUIShow()
{
	return m_bSetUIShow;
}

void CWndToolbar::ShowSettingUI(POINT ptMouse, SettingBtnType btnType)
{
	HideAllSetUI();

	RECT rc, rcClient;
	::GetClientRect(m_hWnd, &rc);
	POINT pt = ptMouse;
	ClientToScreen(m_hWnd, &pt);
	switch (btnType)
	{
	case BTN_PHOTO:
		{
			::GetClientRect(m_pWndSetPhoto->GetHWND(), &rcClient);
			SetWindowPos(m_pWndSetPhoto->GetHWND(), NULL, pt.x-rcClient.right/2, rc.top+pt.y-ptMouse.y-rcClient.bottom, rcClient.right, rcClient.bottom, NULL);
			m_pWndSetPhoto->ShowWindow();
			break;
		}
	case BTN_VIDEO:
		{
			::GetClientRect(m_pWndSetVideo->GetHWND(), &rcClient);
			SetWindowPos(m_pWndSetVideo->GetHWND(), NULL, pt.x-rcClient.right/2, rc.top+pt.y-ptMouse.y-rcClient.bottom, rcClient.right, rcClient.bottom, NULL);
			m_pWndSetVideo->UpdataUI();
			m_pWndSetVideo->ShowWindow();
			break;
		}
	case BTN_MIC:
		{
			::GetClientRect(m_pWndSetMic->GetHWND(), &rcClient);
			SetWindowPos(m_pWndSetMic->GetHWND(), NULL, pt.x-rcClient.right/2, rc.top+pt.y-ptMouse.y-rcClient.bottom, rcClient.right, rcClient.bottom, NULL);
			m_pWndSetMic->UpdataUI();
			m_pWndSetMic->ShowWindow();
			break;
		}
	case BTN_SPEAKER:
		{
			::GetClientRect(m_pWndSetSpeaker->GetHWND(), &rcClient);
			SetWindowPos(m_pWndSetSpeaker->GetHWND(), NULL, pt.x-rcClient.right/2, rc.top+pt.y-ptMouse.y-rcClient.bottom, rcClient.right, rcClient.bottom, NULL);
			m_pWndSetSpeaker->UpdataUI();
			m_pWndSetSpeaker->ShowWindow();
			break;
		}
	default:
		{
			break;
		}
	}

	m_bSetUIShow = TRUE;
}

void CWndToolbar::InitClock()
{
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	StartTime.wHour = sys.wHour;
	StartTime.wMinute = sys.wMinute;
	StartTime.wSecond = sys.wSecond;
}

void CWndToolbar::CalculateClock()
{
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	CDuiString strTime;
	if (m_bTimer)
	{
		if ((sys.wSecond-StartTime.wSecond) < 0)
		{
			sys.wMinute -= 1;
			sys.wSecond += 60;
		}
		if ((sys.wMinute-StartTime.wMinute) < 0)
		{
			sys.wHour -= 1;
			sys.wMinute += 60;
		}
		if ((sys.wHour-StartTime.wHour) < 0)
		{
			sys.wHour += 24;
		}
		strTime.Format(_T("%02d:%02d:%02d"), sys.wHour-StartTime.wHour, sys.wMinute-StartTime.wMinute, sys.wSecond-StartTime.wSecond);
		CButtonUI* pBtnTimer = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnTimer")));
		pBtnTimer->SetText(strTime);
	}
	else
	{
		CButtonUI* pBtnClock = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnClock")));
		strTime.Format(_T("%02d:%02d:%02d"), sys.wHour, sys.wMinute, sys.wSecond);
		pBtnClock->SetText(strTime);
	}
}

void CWndToolbar::SetNewChatMsgCount(int iCount)
{

}

void CWndToolbar::UpDateUI()
{
	m_pBtnViedo->SetVisible(!g_tBtnStatus.bMuteVideo);
	m_pBtnVideoClosed->SetVisible(g_tBtnStatus.bMuteVideo);

	m_pBtnMic->SetVisible(!g_tBtnStatus.bMuteAudioIn);
	m_pBtnMicClosed->SetVisible(g_tBtnStatus.bMuteAudioIn);

	m_pBtnSpeaker->SetVisible(!g_tBtnStatus.bMuteAudioOut);
	m_pBtnSpeakerClosed->SetVisible(g_tBtnStatus.bMuteAudioOut);

	m_pBtnViedo->SetEnabled(!g_tBtnStatus.bMuteServerVideo);
	m_pBtnVideoClosed->SetEnabled(!g_tBtnStatus.bMuteServerVideo);
	m_pBtnMic->SetEnabled(!g_tBtnStatus.bMuteServerAuidoIn);
	m_pBtnMicClosed->SetEnabled(!g_tBtnStatus.bMuteServerAuidoIn);
}

void CWndToolbar::SetSelfContorlStatus(tSelfControlStatus selfStatus)
{
	m_tSelfStatus = selfStatus;
	UpDateUI();
}

void CWndToolbar::SetLectureStage(VidyoClientLectureStage lectureStage)
{
	m_eLectureStage = lectureStage;
	UpDateUI();
}

void CWndToolbar::Notify(TNotifyUI& msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{
		if ( strControlName == _T("btnPhotoSetting") )
		{
			if (m_pWndSetPhoto)
			{
				if (!IsWindowVisible(m_pWndSetPhoto->GetHWND()))
					ShowSettingUI(msg.ptMouse, BTN_PHOTO);
				else
					HideAllSetUI();
			}
		}
		else if ( strControlName == _T("btnVideoSetting") )
		{
			if (m_pWndSetVideo)
			{
				if (!IsWindowVisible(m_pWndSetVideo->GetHWND()))
					ShowSettingUI(msg.ptMouse, BTN_VIDEO);
				else
					HideAllSetUI();
			}
		}
		else if ( strControlName == _T("btnMicSetting") )
		{
			if (m_pWndSetMic)
			{
				if (!IsWindowVisible(m_pWndSetMic->GetHWND()))
					ShowSettingUI(msg.ptMouse, BTN_MIC);
				else
					HideAllSetUI();
			}
		}
		else if ( strControlName == _T("btnSpeakerSetting") )
		{
			if (m_pWndSetSpeaker)
			{
				if (!IsWindowVisible(m_pWndSetSpeaker->GetHWND()))
					ShowSettingUI(msg.ptMouse, BTN_SPEAKER);
				else
					HideAllSetUI();
			}
		}
		else if (strControlName == _T("btnVideo"))
		{
			HideAllSetUI();
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = TRUE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_VIDEO, &privacy, sizeof(privacy));
		}
		else if (strControlName == _T("btnVideoClosed"))
		{
			HideAllSetUI();
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = FALSE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_VIDEO, &privacy, sizeof(privacy));
		}
		else if (strControlName == _T("btnMic"))
		{
			HideAllSetUI();
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = TRUE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_AUDIO_IN, &privacy, sizeof(privacy));
		}
		else if (strControlName == _T("btnMicClosed"))
		{
			HideAllSetUI();
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = FALSE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_AUDIO_IN, &privacy, sizeof(privacy));
		}
		else if (strControlName == _T("btnSpeaker"))
		{
			HideAllSetUI();
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = TRUE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_AUDIO_OUT, &privacy, sizeof(privacy));
		}
		else if (strControlName == _T("btnSpeakerClosed"))
		{
			HideAllSetUI();
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = FALSE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_AUDIO_OUT, &privacy, sizeof(privacy));
		}
		else if (strControlName == _T("btnClock"))
		{
			HideAllSetUI();
			m_bTimer = true;
			CButtonUI* pBtnClock = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnClock")));
			CButtonUI* pBtnTimer = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnTimer")));
			pBtnClock->SetVisible(!m_bTimer);
			pBtnTimer->SetVisible(m_bTimer);
			CalculateClock();
		}
		else if (strControlName == _T("btnTimer"))
		{
			HideAllSetUI();
			m_bTimer = false;
			CButtonUI* pBtnClock = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnClock")));
			CButtonUI* pBtnTimer = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnTimer")));
			pBtnClock->SetVisible(!m_bTimer);
			pBtnTimer->SetVisible(m_bTimer);
			CalculateClock();
		}
		else if (strControlName == _T("btnShare"))
		{
			HideAllSetUI();
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_SHARE, NULL);
		}
		else if (strControlName == _T("btnStopShare"))
		{
			HideAllSetUI();
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_STOPSHARE, NULL);
		}
		else if (strControlName == _T("btnChat"))
		{
			HideAllSetUI();
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_CHAT, NULL);
		}
		else if (strControlName == _T("btnPhoto"))
		{
			HideAllSetUI();
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_PHOTO, NULL);
		}
		else if (strControlName == _T("btnRaiseHand"))
		{
			HideAllSetUI();
		}
		else if (strControlName == _T("btnExit"))
		{
			HideAllSetUI();
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_EXIT, NULL);
		}
	}
}