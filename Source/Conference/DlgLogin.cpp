#include "StdAfx.h"
#include "DlgLogin.h"
#include "WSgSoap.h"
#include "../../include/Util/Util.h"
#include "../../include/VidyoClient/VidyoClient.h"

static VidyoClientInEventLogIn logInParams = {0};
VidyoClientRequestConfiguration reqConfig = {0};
static VidyoClientRequestVolume requestVolume = {0};

CDlgLogin::CDlgLogin(HWND hParent)
{
	m_hParent = hParent;
	m_bLogin = true;
	m_bTrayLogout = false;
}

CDlgLogin::~CDlgLogin()
{
	m_hParent = NULL;
}

LRESULT CDlgLogin::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CDlgLogin::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(0,0);

	return WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);
}

LRESULT CDlgLogin::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntLoginUI, NULL);

	bHandled = TRUE;
	return 0;
}

void CDlgLogin::InitWindow()
{
	CButtonUI* pBtnToggle = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnToggle")));
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnClose")));
	CEditUI* pEditRoomID = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editRoomID")));
	CEditUI* pEditDisplayName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editDisplayName")));
	m_pBtnJoin = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnJoin")));
	m_pLoginBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnLogin")));
	if (m_pLoginBtn) m_pLoginBtn->SetEnabled(false);
	m_pEditUsername = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editUsername")));
	m_pEditUserpass = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPSW")));

	m_pBtnFPSW = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnFPSW")));
	m_pLabLoginError = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labLoginError")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("LOGIN"), _T("btnToggleLogin"), szText, MAX_PATH);	
	pBtnToggle->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("editRoomID"), szText, MAX_PATH);	
	pEditRoomID->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("btnJoin"), szText, MAX_PATH);	
	m_pBtnJoin->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("btnLogin"), szText, MAX_PATH);	
	m_pLoginBtn->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("btnFPSW"), szText, MAX_PATH);
	m_pBtnFPSW->SetText(szText);

	TCHAR UserName[MAX_PATH];
	DWORD Size = MAX_PATH;
	::GetUserName(UserName,&Size);
	CDuiString strDisplayName;
	strDisplayName.Format(_T("%s"), UserName);
	pEditDisplayName->SetText(strDisplayName);

	TCHAR strText[255]=_T("");
	TCHAR szTemp[MAX_PATH];
	project_language_loadstring(_T("LOGIN"), _T("editUserName"), szTemp, MAX_PATH);
	ReadIniString(_T("USERINFO"), _T("UserName"), szTemp, strText, 255);
	m_pEditUsername->SetText(strText);
	project_language_loadstring(_T("LOGIN"), _T("editPsw"), szTemp, MAX_PATH);
	ReadIniString(_T("USERINFO"), _T("Password"), szTemp, strText, 255);
	m_pEditUserpass->SetText(strText);
	if (m_pEditUserpass->GetText().Compare(szTemp))
	{
		m_pEditUserpass->SetPasswordMode(true);
	}
}

void CDlgLogin::JoinRoom()
{
	VidyoClientInEventRoomLink roomLink = {0};

	CEditUI* pEditRoomID = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editRoomID")));
	CEditUI* pEditDisplayName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editDisplayName")));
	CDuiString strRoomID = pEditRoomID->GetText();
	if (strRoomID.IsEmpty())
	{
		LoginErrorTips(ERROR_ROOMID_NULL);
		return ;
	}
	CDuiString strName = pEditDisplayName->GetText();
	if (strName.IsEmpty())
	{
		LoginErrorTips(ERROR_NAME_NULL);
		return ;
	}
	ChangTextToUtf8(strName, roomLink.displayName);
	//memcpy(roomLink.displayName, szName, sizeof(roomLink.displayName));
	ChangTextToUtf8(PORTALURL, roomLink.portalUri);
	//memcpy(roomLink.portalUri, PORTALURL, sizeof(roomLink.portalUri));
	ChangTextToUtf8(strRoomID, roomLink.roomKey);
	//memcpy(roomLink.roomKey, m_strRoomID, sizeof(roomLink.roomKey));
	memcpy(roomLink.pin, "", sizeof(roomLink.pin));

	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_ROOM_LINK, &roomLink, sizeof(roomLink)) != VIDYO_FALSE)
	{
		m_pBtnJoin->SetEnabled(false);
		TCHAR szText[MAX_PATH];
		project_language_loadstring(_T("LOGIN"), _T("btnJoining"), szText, MAX_PATH);
		m_pBtnJoin->SetText(szText);
	}
}

LRESULT CDlgLogin::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		if (m_pLoginBtn->IsVisible()) 
		{
			m_pLoginBtn->SetFocus();
			m_pLoginBtn->Activate();
		}
		if (m_pBtnJoin->IsVisible()) 
		{
			m_pBtnJoin->SetFocus();
			m_pBtnJoin->Activate();
		}
			
		return TRUE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CDlgLogin::Login()
{
	CDuiString strUsername, strPsw;
	ChangTextToUtf8(PORTALURL, logInParams.portalUri);

	strUsername = m_pEditUsername->GetText();
	if (strUsername.IsEmpty())
	{
		LoginErrorTips(ERROR_USERNAME_NULL);
		return ;
	}
	ChangTextToUtf8(strUsername.GetData(), logInParams.userName);

	strPsw = m_pEditUserpass->GetText();
	if (strPsw.IsEmpty())
	{
		LoginErrorTips(ERROR_PSW_NULL);
		return;
	}
	ChangTextToUtf8(strPsw.GetData(), logInParams.userPass);

	logInParams.clientType = VIDYO_CLIENT_CLIENTTYPE_W;
	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_LOGIN, &logInParams, sizeof(logInParams)) != VIDYO_FALSE)
	{
		m_pLoginBtn->SetEnabled(false);
		TCHAR szText[MAX_PATH];
		project_language_loadstring(_T("LOGIN"), _T("btnLogining"), szText, MAX_PATH);
		m_pLoginBtn->SetText(szText);
		SoapSetLoginParams(&logInParams);
		WriteIniString(_T("USERINFO"), _T("UserName"), strUsername);
		WriteIniString(_T("USERINFO"), _T("Password"), strPsw);
	}
}

void CDlgLogin::LoginErrorTips(unsigned int errorTpye)
{
	bool bBtnLogin = false;
	bool bLabLoginErrro = false;
	TCHAR szText[MAX_PATH] = {0};
	if (errorTpye)
	{
		switch (errorTpye)
		{
		case ERROR_USERNAME_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("usernameNull"), szText, MAX_PATH);
			break;
		case ERROR_PSW_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("PswNull"), szText, MAX_PATH);
			break;
		case ERROR_ROOMID_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("RoomIDNull"), szText, MAX_PATH);
			break;
		case ERROR_NAME_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("nameNull"), szText, MAX_PATH);
			break;
		case VIDYO_CLIENT_ERROR_HTTP_408_TIMED_OUT:
			project_language_loadstring(_T("LOGINERROR"), _T("httpTimeOut"), szText, MAX_PATH);
			break;
		case VIDYO_CLIENT_ERROR_HTTP_401_UNAUTHORIZED:
			project_language_loadstring(_T("LOGINERROR"), _T("unAuthorized"), szText, MAX_PATH);
			break;
		case VIDYO_CLIENT_ERROR_INTERNAL_COULD_NOT_LINK:
			project_language_loadstring(_T("LOGINERROR"), _T("couldNotLink"), szText, MAX_PATH);
			break;
		case VIDYO_CLIENT_ERROR_SOAP_RESOURCE_NOT_AVAILABLE:
			project_language_loadstring(_T("LOGINERROR"), _T("serverError"), szText, MAX_PATH);
			break;
		case VIDYO_CLIENT_ERROR_SOAP_CANNOT_JOIN_GUEST:
			project_language_loadstring(_T("LOGINERROR"), _T("soapCannotJoinGuest"), szText, MAX_PATH);
			break;
		}
		bLabLoginErrro = true;
	}
	else
	{
		bBtnLogin = true;
		bLabLoginErrro = false;
	}
	m_pLabLoginError->SetVisible(bLabLoginErrro);
	m_pLabLoginError->SetText(szText);
	m_pLoginBtn->SetEnabled(bBtnLogin);
	project_language_loadstring(_T("LOGIN"), _T("btnLogin"), szText, MAX_PATH);
	m_pLoginBtn->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("btnJoin"), szText, MAX_PATH);
	m_pBtnJoin->SetText(szText);
}

void CDlgLogin::SetTrayLogout(bool bTrayLogout /* = true */)
{
	m_bTrayLogout = bTrayLogout;
}

void CDlgLogin::InitUI()
{
	CButtonUI* pBtnToggle = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnToggle")));
	TCHAR szText[MAX_PATH];
	if (m_bLogin)
	{
		project_language_loadstring(_T("LOGIN"), _T("btnToggleJoin"), szText, MAX_PATH);
		pBtnToggle->SetText(szText);
	}
	else
	{
		project_language_loadstring(_T("LOGIN"), _T("btnToggleLogin"), szText, MAX_PATH);
		pBtnToggle->SetText(szText);
		project_language_loadstring(_T("LOGIN"), _T("btnJoin"), szText, MAX_PATH);
		m_pBtnJoin->SetText(szText);
	}

	CVerticalLayoutUI* pVerJoinEdit = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("verJoinEdit")));
	pVerJoinEdit->SetVisible(!m_bLogin);
	CVerticalLayoutUI* pVerLoginEdit = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("verLoginEdit")));
	pVerLoginEdit->SetVisible(m_bLogin);
	m_pBtnJoin->SetVisible(!m_bLogin);
	m_pBtnJoin->SetEnabled(!m_bLogin);
	m_pLoginBtn->SetVisible(m_bLogin);
	m_pLoginBtn->SetEnabled(m_bLogin);

	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	if (reqConfig.enableAutoLogIn)
	{
		TCHAR szTemp[MAX_PATH];
		project_language_loadstring(_T("LOGIN"), _T("editUserName"), szTemp, MAX_PATH);
		if (m_bLogin && m_pLabLoginError->GetText().IsEmpty()
			&& !m_pEditUsername->GetText().IsEmpty()
			&& !m_pEditUserpass->GetText().IsEmpty()
			&& m_pEditUsername->GetText().Compare(szTemp)
			&& !m_bTrayLogout)
			Login();
	}
}

void CDlgLogin::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender == m_pCloseBtn ) 
		{
			::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntLoginUI, NULL);
		}
		else if ( msg.pSender == m_pLoginBtn )
		{
			LoginErrorTips(NULL);
			Login();
		}
		else if ( msg.pSender == m_pBtnJoin )
		{
			JoinRoom();
		}
		else if ( msg.pSender->GetName() == _T("btnFPSW") )
		{
			ShellExecute(NULL,_T("open"),_T("http://t.vf365.cn/forgotpassword.html"),NULL,NULL,SW_MAXIMIZE);
		}
		else if (msg.pSender->GetName() == _T("btnToggle"))
		{
			m_bLogin = !m_bLogin;
			LoginErrorTips(NULL);
			InitUI();
		}
	}
	else if ( msg.sType == L"textchanged" )
	{
		LoginErrorTips(NULL);
		if (m_bLogin)
		{
			m_pLoginBtn->SetEnabled();
			TCHAR szText[MAX_PATH];
			project_language_loadstring(_T("LOGIN"), _T("btnLogin"), szText, MAX_PATH);
			m_pLoginBtn->SetText(szText);
		} 
		else
		{
			m_pBtnJoin->SetEnabled();
			TCHAR szText[MAX_PATH];
			project_language_loadstring(_T("LOGIN"), _T("btnJoin"), szText, MAX_PATH);
			m_pBtnJoin->SetText(szText);
		}

		if (msg.pSender->GetName() == L"editPSW")
		{
			m_pEditUserpass->SetPasswordMode(true);
		} 
	}
	else if ( msg.sType == L"setfocus" )
	{
		if ( msg.pSender->GetName() == L"editUsername" )
		{
			TCHAR szTemp[MAX_PATH];
			project_language_loadstring(_T("LOGIN"), _T("editUserName"), szTemp, MAX_PATH);
			if (!((CEditUI*)msg.pSender)->GetText().Compare(szTemp))
			{
				((CEditUI*)msg.pSender)->SetText(L"");
			}
		}
		else if ( msg.pSender->GetName() == L"editPSW" )
		{
			TCHAR szTemp[MAX_PATH];
			project_language_loadstring(_T("LOGIN"), _T("editPSW"), szTemp, MAX_PATH);
			if (!((CEditUI*)msg.pSender)->GetText().Compare(szTemp))
			{
				((CEditUI*)msg.pSender)->SetText(L"");
			}
		}
	}
}