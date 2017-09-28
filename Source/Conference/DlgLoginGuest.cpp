#include "StdAfx.h"
#include "DlgLoginGuest.h"
#include "WSgSoap.h"
#include "../../include/Util/Util.h"
#include "../../include/VidyoClient/VidyoClient.h"

static VidyoClientInEventLogIn logInParams = {0};

CDlgLoginGuest::CDlgLoginGuest(HWND hParent)
{
	m_hParent = hParent;
}


CDlgLoginGuest::~CDlgLoginGuest(void)
{
	m_hParent = NULL;
}

LRESULT CDlgLoginGuest::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDlgLoginGuest::InitWindow()
{
	//m_pLabCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnClose")));
	//m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnMin")));
	m_pLoginBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnLogin")));
	//m_pEditPortal = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPortal")));
	m_pEditUsername = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editUsername")));
	m_pEditUserpass = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPSW")));
	m_pOptVideo = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optVideo")));
	m_pOptAudio = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optAudio")));
	//m_pBtnFPSW = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnFPSW")));
	if (m_pEditUserpass) m_pEditUserpass->SetPasswordMode(true);

	TCHAR szText[MAX_PATH];
	//project_language_loadstring(_T("LOGINGUEST"), _T("labCaption"), szText, MAX_PATH);
	//m_pLabCaption->SetText(szText);
	/*project_language_loadstring(_T("LOGINGUEST"), _T("btnClose"), szText, MAX_PATH);
	m_pCloseBtn->SetText(szText);*/
	//project_language_loadstring(_T("LOGINGUEST"), _T("btnMin"), szText, MAX_PATH);
	//m_pMinBtn->SetText(szText);
	project_language_loadstring(_T("LOGINGUEST"), _T("btnLogin"), szText, MAX_PATH);	
	m_pLoginBtn->SetText(szText);
	//project_language_loadstring(_T("LOGINGUEST"), _T("btnFPSW"), szText, MAX_PATH);
	//m_pBtnFPSW->SetText(szText);

	TCHAR strText[255]=_T("");
	ReadIniString(_T("USERINFO"), _T("UserName"), _T(""), strText, 255);
	m_pEditUsername->SetText(strText);
	ReadIniString(_T("USERINFO"), _T("Password"), _T(""), strText, 255);
	m_pEditUserpass->SetText(strText);

	int iMute = 0;
	iMute = ReadIniInt(_T("SETTING"), _T("MuteVideoJoin"), 0);
	m_pOptVideo->Selected(iMute && true);
	iMute = ReadIniInt(_T("SETTING"), _T("MuteAudioJoin"), 0);
	m_pOptAudio->Selected(iMute && true);
}

void CDlgLoginGuest::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender == m_pCloseBtn ) 
		{
			::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntLoginUI, NULL);
		}
		else if ( msg.pSender == m_pOptVideo )
		{
			WriteIniInt(_T("SETTING"), _T("MuteVideoJoin"), !m_pOptVideo->IsSelected());
		}
		else if ( msg.pSender == m_pOptAudio )
		{
			WriteIniInt(_T("SETTING"), _T("MuteAudioJoin"), !m_pOptAudio->IsSelected());
		}
		else if ( msg.pSender == m_pLoginBtn )
		{
			Login();
		}
		else if ( msg.pSender->GetName() == _T("btnFPSW") )
		{
			ShellExecute(NULL,_T("open"),_T("http://t.vf365.cn/forgotpassword.html"),NULL,NULL,SW_MAXIMIZE);
		}
	}
}

LRESULT CDlgLoginGuest::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:			lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CDlgLoginGuest::Login()
{
	VidyoClientInEventMute privacy = {FALSE};
	privacy.willMute = m_pOptVideo->IsSelected();
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_VIDEO, &privacy, sizeof(privacy));

	privacy.willMute = m_pOptAudio->IsSelected();
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_AUDIO_IN, &privacy, sizeof(privacy));

	CDuiString strTemp;
	ChangTextToUtf8(PORTALURL, logInParams.portalUri);

	strTemp = m_pEditUsername->GetText();
	WriteIniString(_T("USERINFO"), _T("UserName"), strTemp);
	ChangTextToUtf8(strTemp.GetData(), logInParams.userName);

	strTemp = m_pEditUserpass->GetText();
	WriteIniString(_T("USERINFO"), _T("Password"), strTemp);
	ChangTextToUtf8(strTemp.GetData(), logInParams.userPass);

	logInParams.clientType = VIDYO_CLIENT_CLIENTTYPE_W;

	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_LOGIN, &logInParams, sizeof(logInParams)) != VIDYO_FALSE)
		SoapSetLoginParams(&logInParams);
}