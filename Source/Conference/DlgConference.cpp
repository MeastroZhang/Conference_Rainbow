#include "StdAfx.h"
#include "DlgConference.h"
#include "WSgSoap.h"

static VidyoClientRequestParticipants reqParticipants = {0};
extern tSelfControlStatus g_tBtnStatus;

extern RECT rect[3];
extern int numScreen;

CDlgConference::CDlgConference(HWND pParent)
	: m_pWndToolbar(NULL)
	, m_pWndRightTopBar(NULL)
	, m_pWndLectureMode(NULL)
	, m_bShowToolBar(FALSE)
	, m_pDlgModerator(NULL)
{
	m_pParent = pParent;
	tConferenceInfo.confMode = MODE_DISCUSS;
	tConferenceInfo.confAuthority = AUTHORITY_COMMON;

	m_ptEntityData = NULL;

	m_pDlgPrivateChat = NULL;

	m_eLectureStage = VIDYO_CLIENT_LECTURE_MODE_STOP;
	m_bHavePresenter = false;
	m_bFullScreen = false;
	m_tBtnStatus.bMuteVideo = 0;
	m_tBtnStatus.bMuteAudioIn = 0;
	m_tBtnStatus.bMuteAudioOut = 0;
	m_tBtnStatus.bMuteServerVideo = 0;
	m_tBtnStatus.bMuteServerAuidoIn = 0;
}


CDlgConference::~CDlgConference()
{
	m_pParent = NULL;

	if (m_ptEntityData)
	{
		delete m_ptEntityData;
		m_ptEntityData = NULL;
	}
}

LRESULT CDlgConference::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		return TRUE;
	}
	return FALSE;
}

void CDlgConference::InitWindow()
{
	COptionUI* optGroupChat = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optGroupChat")));
	COptionUI* optPrivateChat = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optPrivateChat")));
	CButtonUI* btnSend = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnSend")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("CONFERENCE"), _T("optGroupChat"), szText, MAX_PATH);
	if (optGroupChat) optGroupChat->SetText(szText);
	project_language_loadstring(_T("CONFERENCE"), _T("optPrivateChat"), szText, MAX_PATH);
	if (optPrivateChat) optPrivateChat->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnSend"), szText, MAX_PATH);
	if (btnSend) btnSend->SetText(szText);

	if (m_pWndToolbar == NULL)
	{
		m_pWndToolbar = new CWndToolbar(m_hWnd);
		m_pWndToolbar->Create(m_hWnd, _T("Toolbar"), UI_WNDSTYLE_CHILD,  NULL, 0, 0, 0, 0);
		m_pWndToolbar->ShowUI(FALSE);
	}

	if (m_pWndRightTopBar == NULL)
	{
		m_pWndRightTopBar = new CWndRightTopBar;
		m_pWndRightTopBar->Create(m_hWnd, L"RigthTopBar", UI_WNDSTYLE_CHILD, NULL, 0, 0, 0, 0);
		m_pWndRightTopBar->ShowUI(false);
	}

	if (m_pWndLectureMode == NULL)
	{
		m_pWndLectureMode = new CWndLectureMode;
		m_pWndLectureMode->Create(m_hWnd, L"LectureMode", UI_WNDSTYLE_CHILD, NULL, 0, 0, 0, 0);
		m_pWndLectureMode->ShowWindow(false);
	}

	if (m_pDlgModerator == NULL)
	{
		m_pDlgModerator = new CDlgModerator;
		m_pDlgModerator->Create(NULL, _T("Moderator"), WS_POPUP | WS_VISIBLE, NULL, 0, 0, 0, 0);
		m_pDlgModerator->CenterWindow();
		m_pDlgModerator->ShowWindow(false);
	}

	if (m_pDlgPrivateChat == NULL)
	{
		m_pDlgPrivateChat = new CDlgPrivateChat;
		m_pDlgPrivateChat->Create(NULL, L"PrivateChat", WS_POPUP | WS_VISIBLE, NULL, 0, 0, 0, 0);
		m_pDlgPrivateChat->CenterWindow();
		m_pDlgPrivateChat->ShowWindow(false);
	}
}

LRESULT CDlgConference::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_TOOLBAR:	   lRes = OnToolBarMsg(uMsg, wParam, lParam, bHandled); break;
	case WM_END_CONFERENCE:		lRes = OnEndConference(uMsg, wParam, lParam, bHandled); break;
	case WM_EXIT_CONFERENCE:	lRes = OnExitConference(uMsg, wParam, lParam, bHandled); break;
	case WM_CHAT_REPLY:	   lRes = OnFastReply(uMsg, wParam, lParam, bHandled); break;
	case WM_MODERATOR_CHANGED:	lRes = OnModeratorChanged(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDlgConference::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(0,0);

	return WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);
}

LRESULT CDlgConference::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_MINIMIZE)
	{
		ShowWindow(false);
		bHandled = TRUE;
		return 0;
	}
	else if (wParam == SC_MAXIMIZE)
	{
		POINT pt = {0,0};
		ClientToScreen(m_hWnd, &pt);
		GetClientRect(m_hWnd, &m_oldRect);
		m_oldRect.left = m_oldRect.left + pt.x;
		m_oldRect.right = m_oldRect.right + pt.x;
		m_oldRect.top = m_oldRect.top + pt.y;
		m_oldRect.bottom = m_oldRect.bottom + pt.y;
		SetWindowPos(m_hWnd, NULL, 0, -30, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)+30, SWP_SHOWWINDOW);
		m_bFullScreen = TRUE;

		if (m_eLectureStage != VIDYO_CLIENT_LECTURE_MODE_STOP)
		{
			//ShowToolbar();
			UpdateLectureUI();
		}

		bHandled = TRUE;
		return 0;
	}
	else if (wParam == SC_RESTORE)
	{
		if (m_bFullScreen) 
		{
			SetWindowPos(m_hWnd, NULL, m_oldRect.left, m_oldRect.top, m_oldRect.right-m_oldRect.left, m_oldRect.bottom-m_oldRect.top, SWP_SHOWWINDOW);
			m_bFullScreen = false;

			if (m_eLectureStage != VIDYO_CLIENT_LECTURE_MODE_STOP)
			{
				//ShowToolbar();
				UpdateLectureUI();
			}
		}
		ShowWindow();
		bHandled = TRUE;
		return 0;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CDlgConference::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LeaveConference();

	bHandled = TRUE;
	return 0;
}

LRESULT CDlgConference::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	VidyoClientInEventResize inEventResize = {0};
	inEventResize.x = 0;
	inEventResize.y = m_PaintManager.GetCaptionRect().bottom;
	inEventResize.width = (VidyoUint)rc.right;
	inEventResize.height = (VidyoUint)(rc.bottom - m_PaintManager.GetCaptionRect().bottom);
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_RESIZE, &inEventResize, sizeof(inEventResize));

	VidyoClientRequestSetBackground reqBackground = {0};
	reqBackground.willBackground = TRUE;
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_BACKGROUND, &reqBackground, sizeof(reqBackground));

	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif

	bHandled = FALSE;
	return 0;
}

LRESULT CDlgConference::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_TOOLBAR)
	{
		if(m_pWndToolbar)
		{
			if (!m_pWndToolbar->IsSetUIShow())
			{
				m_pWndToolbar->ShowUI(FALSE);
				KillTimer(m_hWnd, TIMER_TOOLBAR);
			}
		}
		if (m_pWndRightTopBar)
		{
			if (m_eLectureStage != VIDYO_CLIENT_LECTURE_MODE_LISTEN)
			{
				m_pWndRightTopBar->ShowUI(false);
			}
		}
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CDlgConference::OnModeratorChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	char szPin[64] = {0};
	memcpy(szPin, (char*)lParam, sizeof(szPin));
	m_pDlgModerator->SetModeratorPin(szPin);

	UpdateUI();
	bHandled = TRUE;
	return 0;
}

void CDlgConference::SetShareStatus(bool bShare)
{
	m_bShare = bShare;
}

BOOL CDlgConference::GetShareStatus()
{
	return m_bShare;
}

void CDlgConference::StartShareScreen()
{
	POINT pt = {0,0};
	ClientToScreen(m_hWnd, &pt);
	GetClientRect(m_hWnd, &m_oldRect);
	m_oldRect.left = m_oldRect.left + pt.x;
	m_oldRect.right = m_oldRect.right + pt.x;
	m_oldRect.top = m_oldRect.top + pt.y;
	m_oldRect.bottom = m_oldRect.bottom + pt.y;

	SetShareScreenStatus();
	RECT rc = {0, 0, 600, 1};
	m_PaintManager.SetCaptionRect(rc);
	SetWindowPos(m_hWnd, NULL, 10, GetSystemMetrics(SM_CYSCREEN)-110, 600, 60, NULL);

	m_pWndToolbar->ShowUI(true);
}

void CDlgConference::StopShareScreen()
{
	RECT rc = {0, 0, 0, 0};
	rc.right = m_oldRect.right-m_oldRect.left;
	rc.bottom = 30;
	m_PaintManager.SetCaptionRect(rc);
	SetWindowPos(m_hWnd, NULL, m_oldRect.left, m_oldRect.top, m_oldRect.right-m_oldRect.left, m_oldRect.bottom-m_oldRect.top, NULL);

	m_pWndToolbar->UpDateUI();
	m_pWndToolbar->ShowUI(true);
}

void CDlgConference::SetShareScreenStatus()
{
	m_pWndToolbar->UpDateUI();
}

void CDlgConference::ShowUI(bool bShow /* = true */)
{
	int iScreenIndex = ReadIniInt(L"USERINFO", L"ScreenNo", 0);
	if (numScreen == 2)
	{
		MoveWindow(m_hWnd, rect[1].left, rect[1].top, rect[1].right-rect[1].left, rect[1].bottom-rect[1].top, TRUE);
	}
	ShowWindow(bShow);
}

void CDlgConference::InitUI()
{
	memset(m_szConferenceID, 0, sizeof(m_szConferenceID));
	SoapGetConferenceID(m_szConferenceID);

	if (m_ptEntityData)
	{
		delete m_ptEntityData;
		m_ptEntityData = NULL;
	}
	m_ptEntityData = new VidyoClientAccountData;
	memset(m_ptEntityData, 0, sizeof(VidyoClientAccountData));

	SoapGetEntityByEntityID(m_szConferenceID, m_ptEntityData);

	VidyoClientRequestSessionDisplayInfo reqDisplayInfo;
	if (VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CURRENT_SESSION_DISPLAY_INFO, &reqDisplayInfo, sizeof(reqDisplayInfo)) == VIDYO_FALSE)
	{
		TCHAR tcDisplayText[DISPLAY_CONFERENCE_SIZE];
		ChangUtf8ToText(reqDisplayInfo.sessionDisplayText, tcDisplayText);
		CLabelUI* pLabCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
		if (pLabCaption) pLabCaption->SetText(tcDisplayText);
	}

	VidyoClientInEventPreview inEventPreview;
	if (VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_PREVIEW_MODE , &inEventPreview, sizeof(inEventPreview)) == VIDYO_FALSE)
	{
		VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_PREVIEW, &inEventPreview, sizeof(inEventPreview));
	}

	UpdateUI();

	if (m_pWndToolbar) m_pWndToolbar->InitClock();
	if (m_pDlgModerator) m_pDlgModerator->InitData(m_ptEntityData->roomMode.isRoomLocked && true);
}

void CDlgConference::UpdateUI()
{
	if (m_pWndToolbar != NULL) m_pWndToolbar->ShowUI();

	SetTimer(m_hWnd, TIMER_TOOLBAR, 5000, NULL);
}

void CDlgConference::UpdateLectureUI()
{
	if (((m_eLectureStage==VIDYO_CLIENT_LECTURE_MODE_LISTEN) && m_bHavePresenter) || m_eLectureStage == VIDYO_CLIENT_LECTURE_MODE_STOP)
	{
		m_pWndLectureMode->ShowWindow(false);
		return ;
	}

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	if (m_eLectureStage == VIDYO_CLIENT_LECTURE_MODE_LISTEN)
	{
		SetWindowPos(m_pWndLectureMode->GetHWND(), NULL, 0, 30, rc.right, rc.bottom-90, NULL);
		m_pWndLectureMode->UpdateUI(1);
		ShowToolbar();
		m_bShare = false;
	}
	else if (m_eLectureStage == VIDYO_CLIENT_LECTURE_MODE_PRESENT)
	{
		SetWindowPos(m_pWndLectureMode->GetHWND(), NULL, (rc.right-480)/2, 30, 480, 45, NULL);
		m_pWndLectureMode->UpdateUI(2);
	}
	else if (m_eLectureStage == VIDYO_CLIENT_LECTURE_MODE_ALLOWED_TO_SPEAK)
	{
		SetWindowPos(m_pWndLectureMode->GetHWND(), NULL, (rc.right-480)/2, 30, 480, 45, NULL);
		m_pWndLectureMode->UpdateUI(3);
	}
}

void CDlgConference::ReleaseUI()
{
	if (m_ptEntityData)
	{
		delete m_ptEntityData;
		m_ptEntityData = NULL;
	}

	m_eLectureStage = VIDYO_CLIENT_LECTURE_MODE_STOP;
	m_bHavePresenter = false;

	m_tBtnStatus.bMuteVideo = 0;
	m_tBtnStatus.bMuteAudioIn = 0;
	m_tBtnStatus.bMuteAudioOut = 0;
	m_tBtnStatus.bMuteServerVideo = 0;
	m_tBtnStatus.bMuteServerAuidoIn = 0;

	if (m_pWndToolbar)
	{
		m_pWndToolbar->SetLectureStage(VIDYO_CLIENT_LECTURE_MODE_STOP);
		m_pWndToolbar->ShowUI(false);
	}
	if (m_pDlgModerator) m_pDlgModerator->ShowUI(false);
	if (m_pWndRightTopBar) m_pWndRightTopBar->ShowUI(false);
	if (m_pDlgPrivateChat) m_pDlgPrivateChat->ShowUI(false);
}

void CDlgConference::SetSelfContorlStatus(tSelfControlStatus selfStatus)
{
	if (m_pWndToolbar)
		m_pWndToolbar->SetSelfContorlStatus(selfStatus);
	if (m_pWndRightTopBar)
		m_pWndRightTopBar->SetSelfContorlStatus(selfStatus);
}

void CDlgConference::ClearRaisehand()
{
	if (m_pWndToolbar)
	{
		m_pWndToolbar->UpDateUI();
	}
}

void CDlgConference::SetLectureStage(VidyoClientLectureStage lectureMode)
{
	if (m_pDlgModerator)
		m_pDlgModerator->SetLectureStage(lectureMode);

	if (m_pWndToolbar)
		m_pWndToolbar->SetLectureStage(lectureMode);
	if (m_pWndRightTopBar)
		m_pWndRightTopBar->SetLectureStage(lectureMode);

	m_eLectureStage = lectureMode;
	UpdateLectureUI();
}

void CDlgConference::SetPresenter(bool bHavePresenter)
{
	m_bHavePresenter = bHavePresenter;
	UpdateLectureUI();
}

void CDlgConference::UpdataMuteServerVideo(bool bMuteVideo)
{
	if (m_pDlgModerator)
		m_pDlgModerator->UpdataMuteVideo(bMuteVideo);
}

void CDlgConference::UpdataMuteServerAudio(bool bMuteAudio)
{
	if (m_pDlgModerator)
		m_pDlgModerator->UpdataMuteAudio(bMuteAudio);
	if (m_eLectureStage == VIDYO_CLIENT_LECTURE_MODE_LISTEN && !bMuteAudio)
	{
		SetLectureStage(VIDYO_CLIENT_LECTURE_MODE_ALLOWED_TO_SPEAK);
	}
	if (m_eLectureStage == VIDYO_CLIENT_LECTURE_MODE_ALLOWED_TO_SPEAK && bMuteAudio)
	{
		SetLectureStage(VIDYO_CLIENT_LECTURE_MODE_LISTEN);
	}
}

void CDlgConference::ParticipantsChanged(unsigned int uiCount)
{
	if (m_pDlgModerator)
		m_pDlgModerator->UpdateParticipants(uiCount);
	UpdataParticipantsList();
}

void CDlgConference::UpdataParticipantsList()
{
	int iError = VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_PARTICIPANTS, &reqParticipants, sizeof(reqParticipants));
	if (iError == VIDYO_FALSE)
	{
		TCHAR szBuf[MAX_PATH] = {0};
		CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
		pParList->RemoveAll();

		for (unsigned int i = 0; i < reqParticipants.numberParticipants; i++)
		{
			CListContainerElementUI* pListItem  = new CListContainerElementUI();
			pListItem->SetFixedHeight(30);
			pListItem->SetManager(&m_PaintManager, NULL, false);
			ChangUtf8ToText(reqParticipants.name[i], szBuf);
			CLabelUI* pLabItem = new CLabelUI;
			pLabItem->SetText(szBuf);
			pLabItem->SetAttribute(L"textpadding", L"30,0,0,0");
			pLabItem->SetAttribute(L"font", L"1");
			pListItem->Add(pLabItem);
			pParList->Add(pListItem);
		}
	}
}

void CDlgConference::ShowToolbar(bool bShow /* = true */)
{
	if (m_pWndToolbar == NULL) return;

	m_pWndToolbar->SetSelfContorlStatus(m_tBtnStatus);
	m_pWndToolbar->ShowUI(bShow);

	SetTimer(m_hWnd, TIMER_TOOLBAR, 5000, NULL);
}

void CDlgConference::UpdateToolbar()
{
	if (::IsWindowVisible(m_pWndToolbar->GetHWND()))
	{
		ShowToolbar();
	}
}

void CDlgConference::ShowShareList()
{
	CDlgSharing* pDlgShare = new CDlgSharing(this);
	pDlgShare->Create(m_hWnd, _T("Sharing"), UI_WNDSTYLE_DIALOG, NULL, 0, 0, 0, 0);
	pDlgShare->CenterWindow();
	pDlgShare->ShowModal();
	delete pDlgShare;
}

void CDlgConference::ShowControlWnd()
{
	CWndControl* pWndControl = new CWndControl(NULL);
	pWndControl->Create(NULL, _T("ControlWnd"), UI_WNDSTYLE_DIALOG,  NULL, 0, 0, 0, 0);
	pWndControl->CenterWindow();
	pWndControl->ShowModal();
	delete pWndControl;
}

void CDlgConference::ShowChatWnd()
{
	UpdateUI();
}

void CDlgConference::TakePicture()
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	rc.top = rc.top + m_PaintManager.GetCaptionRect().bottom;
	rc.right = rc.right;

	TCHAR strText[255] = _T("");
	ReadIniString(_T("SETTING"), _T("PhotoPath"), _T(""), strText, 255);
	if (!wcscmp(strText, L""))
	{
		ReadIniString(_T("SETTING"), _T("PhotoPath"), PATH_PHOTO, strText, 255);
		if (_wmkdir(strText) == 0)
			WriteIniString(_T("SETTING"), _T("PhotoPath"), PATH_PHOTO);
	}
	char szText[255] = {0}, szName[128] = {0};
	ChangTextToUtf8(strText, szText);
	ChangUtf8ToGBK(m_ptEntityData->displayName, szName);

	char szPath[512] = {0};
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	sprintf(szPath, "%s\\%s_%d-%02d-%02d_%02d-%02d-%02d.bmp", szText, szName,
		sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
	if (m_pWndToolbar) m_pWndToolbar->ShowUI(false);
	Sleep(100);
	SaveWindowCopy(NULL, szPath, m_PaintManager.GetCaptionRect().bottom, rc.right-rc.left, rc.bottom-rc.top, m_hWnd);
	Sleep(100);
	if (m_pWndToolbar) m_pWndToolbar->ShowUI(true);
}

LRESULT CDlgConference::OnToolBarMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ToolBarMsg msg = (ToolBarMsg)wParam;
	switch (msg)
	{
	case TB_MSG_SHARE:
		ShowShareList();
		break;
	case TB_MSG_STOPSHARE:
		{
			if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_UNSHARE, NULL, NULL))
			{
				StopShareScreen();
				SetShareStatus(false);
			}
		}
		break;
	case TB_MSG_CONTROL:
		ShowControlWnd();
		break;
	case TB_MSG_CHAT:
		ShowChatWnd();
		break;
	case TB_MSG_PHOTO:
		TakePicture();
		break;
	case  TB_MSG_EXIT:
		LeaveConference();
		break;
	}
	return 0;
}

LRESULT CDlgConference::OnEndConference(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SoapDisconnectConferenceAll(m_szConferenceID, "");
	return 0;
}

LRESULT CDlgConference::OnExitConference(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_LEAVE, NULL, 0) == VIDYO_TRUE)
	{
		CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"richGroupChatView"));
		if( pRichEdit == NULL ) return 0;
		pRichEdit->SetText(L"");
	}
	return 0;
}

LRESULT CDlgConference::OnFastReply(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDuiString strTemp;
	TCHAR szBuf[MAX_PATH] = {0};
	strTemp.Format(L"element%d", wParam+1);
	project_language_loadstring(_T("CHATREPLY"), strTemp, szBuf, MAX_PATH);

	ShowMsg(szBuf, L"ÎÒ");

	VidyoClientInEventGroupChat groupChat = {0};
	ChangTextToUtf8(szBuf, (char*)&groupChat);
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_GROUP_CHAT, &groupChat, sizeof(groupChat));

	return 0;
}

void CDlgConference::LeaveConference()
{
	int iSetting = ReadIniInt(L"SETTING", L"LeaveWithConfirm", 1);
	if (iSetting)
	{
		if (m_ptEntityData->canControl)
		{
			CMsgModeratorExit* pPopWnd = new CMsgModeratorExit();
			pPopWnd->Create(m_hWnd, _T("MsgModeratorExit"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
			pPopWnd->CenterWindow();
			pPopWnd->ShowModal();
		}
		else
		{
			CMsgRoomExit* pPopWnd = new CMsgRoomExit;
			pPopWnd->Create(m_hWnd, _T("MsgRoomExit"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
			pPopWnd->CenterWindow();
			pPopWnd->ShowModal();
		}
	}
	else
	{
		VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_LEAVE, NULL, 0);
	}
}

void CDlgConference::RevGroupChat(VidyoClientOutEventGroupChat* groupChat)
{
	TCHAR szName[128] = {0};
	ChangUtf8ToText(groupChat->displayName, szName);

	TCHAR szMsg[1025] = {0};
	ChangUtf8ToText(groupChat->message, szMsg);

	ShowMsg(szMsg, szName);
}

void CDlgConference::RevPrivateChat(VidyoClientOutEventPrivateChat* priviteChat)
{
	if (m_pDlgPrivateChat)
	{
		TCHAR szName[128], szURI[257], szMsg[1025];
		ChangUtf8ToText(priviteChat->displayName, szName);
		ChangUtf8ToText(priviteChat->message, szMsg);
		ChangUtf8ToText(priviteChat->uri, szURI);
		m_pDlgPrivateChat->AddUser(szName, szURI);
		m_pDlgPrivateChat->RevPrivateChat(szMsg, szName);
		
		//m_pDlgPrivateChat->ShowUI();
	}
}

void CDlgConference::ShowMsg(LPCTSTR strMsg, LPCTSTR strSender)
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"richGroupChatView"));
	if( pRichEdit == NULL ) return;
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 0);
	cf.dwEffects = 0;

	if (!pRichEdit->GetText().IsEmpty())
	{
		pRichEdit->SetSel(-1, -1);
		pRichEdit->ReplaceSel(_T("\n"), false);
		pRichEdit->SetSel(-1, -1);
		pRichEdit->ReplaceSel(_T("\n"), false);
	}

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	pRichEdit->ReplaceSel(strSender, false);

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	SYSTEMTIME time;
	::GetLocalTime(&time);
	CDuiString strTime;
	strTime.Format(L":\t(%u/%u/%u %u:%u:%u)\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	pRichEdit->ReplaceSel(strTime, false);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0xcc, 0xCC, 0xCC);
	cf.dwEffects = 0;
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(strMsg, false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();
}

void CDlgConference::SendChatMsg()
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"richGroupChatInput"));
	if( pRichEdit == NULL ) return;
	pRichEdit->SetFocus();
	CDuiString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
	if( sText.IsEmpty() ) return;
	pRichEdit->SetText(_T(""));

	VidyoClientInEventGroupChat groupChat = {0};
	ChangTextToUtf8(sText.GetData(), (char*)&groupChat);
	//int iResult = VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_GROUP_CHAT, &groupChat, sizeof(groupChat));
	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_GROUP_CHAT, &groupChat, sizeof(groupChat)))
	{
		ShowMsg(sText.GetData(), L"ÎÒ");
	}
}

void CDlgConference::OnSelectChanged( TNotifyUI &msg )
{
	if (msg.pSender->GetName() == L"optSearch")
	{
		if (((COptionUI*)(msg.pSender))->IsSelected())
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"editSearch"));
			CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));

			CDuiString strSearch = pEdit->GetText();
			TCHAR szName[256] = {0};
			for (unsigned int i = 0; i < reqParticipants.numberParticipants; i++)
			{
				ChangUtf8ToText(reqParticipants.name[i], szName);
				if (!strSearch.Compare(szName))
				{
					pParList->SelectItem(i);
				}
			}
		}
		return;
	}

	CTabLayoutUI* pTabChat = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabChat")));

	if (pTabChat == NULL) return ;

	if (msg.pSender->GetName() == _T("optGroupChat"))
	{
		pTabChat->SelectItem(0);
	}
	else if (msg.pSender->GetName() == _T("optPrivateChat"))
	{
		pTabChat->SelectItem(1);
		UpdataParticipantsList();
	}
}

void CDlgConference::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == _T("btnClose") ) 
		{
			LeaveConference();
		}
		else if ( msg.pSender->GetName() == _T("btnMin") )
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if ( msg.pSender->GetName() == _T("btnMax") )
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else if ( msg.pSender->GetName() == _T("btnModerator") )
		{
			if (m_pDlgModerator)
			{
				if (!::IsWindowVisible(m_pDlgModerator->GetHWND()))
					m_pDlgModerator->ShowUI();
				::SetForegroundWindow(m_pDlgModerator->GetHWND());
			}
		}
		else if ( msg.pSender->GetName() == L"btnApply" )
		{
			if (!m_ptEntityData->roomMode.hasModeratorPIN)
			{
				CMsgApplyForbid* pPopWnd = new CMsgApplyForbid;
				pPopWnd->Create(m_hWnd, _T("MsgApplyForbid"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
				pPopWnd->CenterWindow();
				pPopWnd->ShowModal();
			}
			else
			{
				CWndInputPin* pPopWnd = new CWndInputPin(ntModeratorPin, m_szConferenceID);
				pPopWnd->Create(m_hWnd, _T("InputModeratorPin"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
				pPopWnd->CenterWindow();
				pPopWnd->ShowModal();
			}
		}
		else if ( msg.pSender->GetName() == L"btnSend" )
		{
			CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"richGroupChatInput"));
			CDuiString strInput = pRichEdit->GetText();
			if (strInput.IsEmpty())
			{
				CWndFastReply* pPopWnd = new CWndFastReply();
				pPopWnd->Create(m_hWnd, _T("FastReply"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);

				RECT rc, rcClient;
				::GetClientRect(m_hWnd, &rc);
				::GetClientRect(pPopWnd->GetHWND(), &rcClient);
				POINT pt = msg.ptMouse;
				ClientToScreen(m_hWnd, &pt);
				SetWindowPos(pPopWnd->GetHWND(), NULL, 
					rc.right-rcClient.right+pt.x-msg.ptMouse.x, 
					rc.bottom-rcClient.bottom+pt.y-msg.ptMouse.y-34,
					rcClient.right, 
					rcClient.bottom, NULL);
				pPopWnd->ShowModal();
			}
			else
				SendChatMsg();
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		OnSelectChanged(msg);
	}
	else if ( msg.sType == L"itemactivate" )
	{
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("listParticipants")));
		if (m_pDlgPrivateChat)
		{
			TCHAR szName[64] = {0};
			TCHAR szURI[257] = {0};
			ChangUtf8ToText(reqParticipants.name[pList->GetCurSel()], szName);
			ChangUtf8ToText(reqParticipants.URI[pList->GetCurSel()], szURI);
			m_pDlgPrivateChat->AddUser(szName, szURI);
			m_pDlgPrivateChat->UpdataUI();
			m_pDlgPrivateChat->ShowWindow();
			SetForegroundWindow(m_pDlgPrivateChat->GetHWND());
		}
	}
}

