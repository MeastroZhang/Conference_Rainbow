// MainFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "MainFrame.h"
#include "WSgSoap.h"

// Global Variables:
HWND g_hWnd;					// main window handle

typedef struct
{
	VidyoClientOutEvent actionType;
	VidyoVoidPtr		data;
	VidyoUint			paramSize;
	VidyoVoidPtr		param;
} EventData;

typedef struct
{
	const char			*confName;
} ConferenceData;

ConferenceData confData = { "Conference" };

static VidyoClientRequestConfiguration reqConfig = {0};
static VidyoClientInEventLogIn logInParams = {0};
static VidyoClientRequestConnectivityInfo ConnectivityInfo = {0};
static VidyoClientOutEventGroupChat *outEventGroupChat;
static VidyoClientInEventControlCamera panLeft = {0};
static VidyoClientInEventControlCamera panRight = {0};
static VidyoClientInEventControlCamera tiltUp = {0};
static VidyoClientInEventControlCamera tiltDown = {0};
static VidyoClientInEventControlCamera zoomIn = {0};
static VidyoClientInEventControlCamera zoomOut = {0};
static VidyoClientInEventControlCamera stop = {0};
static VidyoClientInEventControlCamera freeze = {0};
static VidyoClientInEventControlCamera unfreeze = {0};
static VidyoClientOutEventIncomingCall *outEventincomingCall;
static VidyoClientOutEventSignedOut *outEventSignedOut;
tSelfControlStatus g_tBtnStatus = {0};

void WINAPI VDAPIConsoleOutEventCB(VidyoClientOutEvent actionType, VidyoVoidPtr param, VidyoUint paramSize, VidyoVoidPtr data)
{
	EventData* event_data	= (EventData*)malloc(sizeof(EventData) + paramSize);
	event_data->actionType	= actionType;
	event_data->param		= param;
	event_data->paramSize	= paramSize;
	event_data->data		= (char*)event_data + sizeof(EventData);
	memcpy(event_data->data, param, paramSize);

	if (PostMessage(g_hWnd, WM_VIDYO, (WPARAM)event_data, NULL) == 0) free(event_data);
}

int numScreen = 1;
RECT rect[3];
BOOL CALLBACK MyMonitorEnumProc(
	HMONITOR hMonitor,  // handle to display monitor
	HDC hdcMonitor,     // handle to monitor DC
	LPRECT lprcMonitor, // monitor intersection rectangle
	LPARAM dwData       // data
	)
{
	static BOOL first = TRUE;

	MONITORINFO	monitorinfo;
	monitorinfo.cbSize = sizeof(MONITORINFO);

	GetMonitorInfo(hMonitor, &monitorinfo);

	if (monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
	{
		if (first)
		{
			first = FALSE;
			numScreen = 1;

			rect[0] = monitorinfo.rcMonitor;
			return TRUE;
		}
		else
		{
			first = TRUE;
			return FALSE;
		}
	}

	rect[numScreen] = monitorinfo.rcMonitor;
	numScreen++;
	return TRUE;
}

MainFrame::MainFrame(void)
	: m_pDlgContacts(NULL)
	, m_pDlgConference(NULL)
	, m_pDlgLogin(NULL)
	, m_pDlgGuest(NULL)
	, m_pWndProcessing(NULL)
{
	m_iConferenceUICount	= 0;
	m_iLoginUICount			= 0;
	m_iContactsUICount		= 0;

	m_iMouseXPos			= 0;
	m_iMouseYPos			= 0;
}

MainFrame::~MainFrame(void)
{
	// delete tray icon
	m_trayIcon.DeleteTrayIcon();

	g_hWnd = NULL;

	ClearStack();
	DestroyAllUI();
	ReleaseVidyoSystem();
}

void MainFrame::InitWindow()
{
	EnumDisplayMonitors(NULL, NULL, MyMonitorEnumProc, NULL);

	CreateConferenceUI();
	CreateLoginUI();
	CreateGuestUI();
	CreateContactsUI();
	CreateProcessingWnd();

	g_hWnd = m_hWnd;
	InitVidyoSystem();

	if (ProCmdline())
		Push(ntGuestUI);
	else
		Push(ntContactsUI);
	ShowUI();

	// 添加系统托盘图标
	m_trayIcon.CreateTrayIcon(m_hWnd, IDR_MAINFRAME, _T("Conference"));
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:				lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:				lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:			lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:			lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:			lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:			lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:			lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:				lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:		lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:			lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_VIDYO:				lRes = OnVidyoMessage(uMsg, wParam, lParam, bHandled); break;
	case WM_WINDOW:				lRes = OnExitUI(uMsg, wParam, lParam, bHandled); break;
	case UIMSG_TRAYICON:		lRes = OnTrayMessage(uMsg, wParam, lParam, bHandled); break;
	case WM_COMMAND:			lRes = OnCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_JOIN_PROCESS:		lRes = OnShowProcess(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

BOOL MainFrame::ProCmdline()
{
	CDuiString strCmd = GetCommandLine();
	int iPos = strCmd.Find(_T("vmc://"));
	if (iPos < 0) return FALSE;

	CDuiString strlink = strCmd.Mid(iPos);

	int iPosID = strlink.ReverseFind('&');
	if (iPosID < 0) return FALSE;
	CDuiString strConfID = strlink.Mid(iPosID);
	strConfID = CDuiString((LPTSTR)strConfID.GetData()+strConfID.Find('=')+1);
	CDuiString strLinkName = strlink.Left(iPosID);

	int iPosName = strLinkName.ReverseFind('?');
	if (iPosName < 0) return FALSE;
	CDuiString strConfName = strLinkName.Mid(iPosName);
	strConfName = CDuiString((LPTSTR)strConfName.GetData()+strConfName.Find('=')+1);
	CDuiString strConfURL = strLinkName.Left(iPosName);

	m_pDlgGuest->SetGuestInfo(strConfURL, strConfName, strConfID);
	return TRUE;
}

void MainFrame::NotifyWindow()
{
	int nCount = m_ModeTypeStack.size();
	if (nCount > 0)
	{
		eModeType peTypeTemp = m_ModeTypeStack.back();

		switch(peTypeTemp)
		{
		case ntConferenceUI:
			{
				if (m_pDlgConference)
				{
					m_pDlgConference->UpdateUI();
				}
			}
			break;
		case ntLoginUI:
			{
				if (m_pDlgLogin)
				{
					//m_pDlgLogin->UpdateUI();
				}
			}
			break;
		default:
			break;
		}
	}
}

void MainFrame::Push(eModeType eType)
{
	//CSingleLock lock(&m_cs, TRUE);
	m_ModeTypeStack.push_back(eType);

	switch(eType)
	{
	case ntConferenceUI:
		m_iConferenceUICount++;
		break;
	case ntLoginUI:
		m_iLoginUICount++;
		break;
	case ntContactsUI:
		m_iContactsUICount++;
		break;
	default:
		break;
	}
}

void MainFrame::Pop(eModeType eType)
{
	//CSingleLock lock(&m_cs, TRUE);

	int nCount = m_ModeTypeStack.size();
	
	if (nCount > 0)
	{
		eModeType peTypeTemp = m_ModeTypeStack.back();

		if (peTypeTemp == eType)
		{
			switch(eType)
			{
			case ntConferenceUI:
				m_iConferenceUICount--;
				if (m_pDlgConference)
				{
					m_pDlgConference->ReleaseUI();
				}
				break;
			case ntLoginUI:
				m_iLoginUICount--;
				break;
			case ntContactsUI:
				m_iContactsUICount--;
				if (m_pDlgContacts)
				{
					m_pDlgContacts->ReleaseUI();
				}
				break;
			default:
				break;
			}

			m_ModeTypeStack.pop_back();
		}
	}
}

void MainFrame::ShowUI()
{
	//CSingleLock lock(&m_cs, TRUE);

	HideAllUI();

	int nCount = m_ModeTypeStack.size();
	if (nCount > 0)
	{
		eModeType peTypeTemp = m_ModeTypeStack.back();

		switch(peTypeTemp)
		{
		case ntConferenceUI:
			{
				if (m_pDlgConference)
				{
					m_pDlgConference->InitUI();
					m_pDlgConference->ShowUI();
				}
			}
			break;
		case ntGuestUI:
			{
				if (m_pDlgGuest)
				{
					m_pDlgGuest->ShowWindow();
				}
			}
			break;
		case ntLoginUI:
			{
				if (m_pDlgLogin)
				{
					m_pDlgLogin->InitUI();
					m_pDlgLogin->ShowWindow();
				}
			}
			break;
		case ntContactsUI:
			{
				if (m_pDlgContacts)
				{
					m_pDlgContacts->ShowUI();
					m_pDlgContacts->Login();
				}
			}
			break;
		case ntProcessing:
			{
				if (m_pWndProcessing)
				{
					m_pWndProcessing->InitUI();
					m_pWndProcessing->ShowUI();
				}
			}
			break;
		default:
			break;
		}
	}
	else
	{
		PostQuitMessage(0);
	}
}

void MainFrame::HideAllUI()
{
	if (m_pDlgLogin)
		m_pDlgLogin->ShowWindow(false);

	if (m_pDlgContacts)
		m_pDlgContacts->ShowWindow(false);

	if (m_pDlgConference)
		m_pDlgConference->ShowWindow(false);

	if (m_pWndProcessing)
		m_pWndProcessing->ShowWindow(false);
}

void MainFrame::ClearStack()
{
	if (m_ModeTypeStack.size())
	{
		m_ModeTypeStack.clear();
	}

	m_iConferenceUICount = 0;
	m_iContactsUICount = 0;
	m_iLoginUICount = 0;
}

void MainFrame::DestroyAllUI()
{
	if (m_pDlgConference)
	{
		m_pDlgConference->Close();
		delete m_pDlgConference;
		m_pDlgConference = NULL;
	}

	if (m_pDlgLogin)
	{
		m_pDlgLogin->Close();
		delete m_pDlgLogin;
		m_pDlgLogin = NULL;
	}

	if (m_pDlgContacts)
	{
		m_pDlgContacts->Close();
		delete m_pDlgContacts;
		m_pDlgContacts = NULL;
	}
}

void MainFrame::CreateConferenceUI()
{
	if (m_pDlgConference == NULL)
	{
		m_pDlgConference = new CDlgConference(m_hWnd);
		// UI_WNDSTYLE_FRAME:TaskBar click minimize, Caption double click maximize.
		m_pDlgConference->Create(NULL, _T("Conference"), UI_WNDSTYLE_DIALOG | WS_MINIMIZEBOX,  NULL, 0, 0, 0, 0);
		if (m_pDlgConference)
		{
			m_pDlgConference->CenterWindow();
			m_pDlgConference->ShowWindow(false);
		}
	}
}

void MainFrame::CreateLoginUI()
{
	if (m_pDlgLogin == NULL)
	{
		m_pDlgLogin = new CDlgLogin(m_hWnd);
		m_pDlgLogin->Create(NULL, _T("Login"), UI_WNDSTYLE_DIALOG | WS_MINIMIZEBOX,  NULL, 0, 0, 0, 0);
		m_pDlgLogin->SetIcon(IDR_MAINFRAME);
		if (m_pDlgLogin)
		{
			m_pDlgLogin->CenterWindow();
			m_pDlgLogin->ShowWindow(false);
		}
	}
}

void MainFrame::CreateGuestUI()
{
	if (m_pDlgGuest == NULL)
	{
		m_pDlgGuest = new CDlgGuest(m_hWnd);
		m_pDlgGuest->Create(NULL, _T("Guest"), UI_WNDSTYLE_DIALOG | WS_MINIMIZEBOX,  NULL, 0, 0, 0, 0);
		m_pDlgGuest->SetIcon(IDR_MAINFRAME);
		if (m_pDlgGuest)
		{
			m_pDlgGuest->CenterWindow();
			m_pDlgGuest->ShowWindow(false);
		}
	}
}

void MainFrame::CreateContactsUI()
{
	if (m_pDlgContacts == NULL)
	{
		m_pDlgContacts = new CDlgContacts(m_hWnd);
		m_pDlgContacts->Create(NULL, _T("Contacts"), UI_WNDSTYLE_DIALOG | WS_MINIMIZEBOX,  NULL, 0, 0, 0, 0);
		if (m_pDlgContacts)
		{
			m_pDlgContacts->CenterWindow();
			m_pDlgContacts->ShowWindow(false);
		}
	}
}

void MainFrame::CreateProcessingWnd()
{
	if (m_pWndProcessing == NULL)
	{
		m_pWndProcessing = new CWndProcessing;
		m_pWndProcessing->Create(NULL, L"Processing", UI_WNDSTYLE_DIALOG, NULL, 0, 0, 0, 0);
		m_pWndProcessing->CenterWindow();
		m_pWndProcessing->ShowWindow(false);
	}
}

void MainFrame::InitVidyoSystem()
{
	VidyoClientLogParams logParams = {0};
	logParams.logBaseFileName = "conference_";
	logParams.pathToLogDir = PATH_LOG;
	logParams.logLevelsAndCategories = ""/*"debug@App debug@AppEvents warning"*/;
	logParams.logSize = 10*1024*1024; // 10M

	VidyoClientProfileParams profileParams = {0};
	profileParams.PROGFILES_DIR   = "\\VMC\\Conference\\";
	profileParams.CONFIG_KEY_PATH = "SOFTWARE\\VMC\\Conference\\";
	profileParams.PRODUCT_NAME    = "Conference";
	profileParams.MYDOCS_DIR      = PATH_DOCS;
	profileParams.VIDYO_DIR       = "\\Conference\\";
	profileParams.portNumber      = 63457;
	//char* szEvent = "http://";
	//profileParams.argvList		  = &szEvent;

	VidyoRect VRect = {0, 30, 1022, 640};
	if (VidyoClientInitialize(VDAPIConsoleOutEventCB, &confData, &logParams) == VIDYO_FALSE) 
	{
		PostQuitMessage(0);
		return;
	}

	if (VidyoClientStart(VDAPIConsoleOutEventCB, &confData, &logParams, 
		m_pDlgConference->GetHWND(), &VRect, NULL, &profileParams, VIDYO_FALSE) == VIDYO_FALSE) 
	{
		PostQuitMessage(0);
		return;
	}
}

void MainFrame::ReleaseVidyoSystem()
{
	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_SIGNOFF, NULL, NULL) != VIDYO_TRUE)
	{
		// Probably still processing. May need to wait a little.
		// handle error
	}

	Sleep(100);

	if (VidyoClientStop() != VIDYO_TRUE)
	{
		// Probably still processing. May need to wait a little.
		// handle error
	}
	else
	{
		Sleep(100);
		VidyoClientUninitialize();
	}
}

void MainFrame::ShowTrayMenu()
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu;
	hMenu = CreatePopupMenu();

	TCHAR szText[MAX_PATH];

	//if (m_ModeTypeStack.size() > 0)
	//{
	//	eModeType peTypeTemp = m_ModeTypeStack.back();
	//	if (peTypeTemp == ntLoginUI)
	//	{
	//		project_language_loadstring(_T("TRAYMENU"), _T("trayOpen"), szText, MAX_PATH);
	//		AppendMenu(hMenu, MF_STRING , ID_TRAY_OPEN, szText);
	//		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	//	}
	//	else
	//	{
	//		project_language_loadstring(_T("TRAYMENU"), _T("trayPsy"), szText, MAX_PATH);
	//		AppendMenu(hMenu, MF_STRING , ID_TRAY_PSW, szText);
	//		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);

	//		if (peTypeTemp == ntContactsUI)
	//		{
	//			project_language_loadstring(_T("TRAYMENU"), _T("trayLogout"), szText, MAX_PATH);
	//			AppendMenu(hMenu, MF_STRING, ID_TRAY_LOGOUT, szText);
	//			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	//		} 
	//		else
	//		{
	//			project_language_loadstring(_T("TRAYMENU"), _T("trayExitConf"), szText, MAX_PATH);
	//			AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT_CONF, szText);
	//			AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	//		}
	//	}
	//}
	project_language_loadstring(_T("TRAYMENU"), _T("trayExit"), szText, MAX_PATH);
	AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, szText);

	//AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);

	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, NULL, m_hWnd, NULL);
}

void MainFrame::TrayLogout()
{
	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_SIGNOFF, NULL, NULL))
	{
		m_pDlgLogin->SetTrayLogout();
	}
}

void MainFrame::TrayExitConf()
{
	if (IsWindowVisible(m_pDlgConference->GetHWND()))
	{
		m_pDlgConference->LeaveConference();
	}
}

LRESULT MainFrame::OnExitUI(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT MainFrame::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case ID_TRAY_OPEN:
		m_pDlgLogin->Login();
		break;
	case ID_TRAY_LOGOUT:
		TrayLogout();
		break;
	case ID_TRAY_EXIT_CONF:
		TrayExitConf();
		break;
	case ID_TRAY_EXIT:
		PostQuitMessage(0);
		break;
	case ID_TRAY_PSW:
		SoapChangePsw();
		break;
	}

	return 0;
}

LRESULT MainFrame::OnShowProcess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Push(ntProcessing);
	ShowUI();

	bHandled = TRUE;
	return 0;
}

LRESULT MainFrame::OnTrayMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (LOWORD(lParam) == WM_LBUTTONDBLCLK) 
	{
		ShowUI();
	}

	if (LOWORD(lParam) == WM_RBUTTONUP)
	{	
		ShowTrayMenu();
	} 

	return 0;
}

LRESULT MainFrame::OnVidyoMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	EventData* event_data = (EventData*)wParam;
	if (event_data)
	{
		if (event_data->data != NULL)
		{
			ConferenceData *confData = (ConferenceData *)event_data->data;
			switch (event_data->actionType)
			{
			case VIDYO_CLIENT_OUT_EVENT_PORTAL_SERVICE:
				{
					break;
				}						
			case VIDYO_CLIENT_OUT_EVENT_LOGIC_STARTED:
				{
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_SIGN_IN:
				{
					VidyoClientOutEventSignIn *outEventSignIn;
					outEventSignIn = (VidyoClientOutEventSignIn *)event_data->data;
					VidyoUint activeEid = outEventSignIn->activeEid;

					// If Endpoint ID is 0, licensing issue
					if (!activeEid) VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_LICENSE, NULL, 0);
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_SIGN_OUT:
				{
					
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_LOGIN:	/* Out event received when login is succesful */
				{
					VidyoClientOutEventLogIn *outEventLogIn;
					outEventLogIn = (VidyoClientOutEventLogIn *)event_data->data;
					break;
				}		
			case VIDYO_CLIENT_OUT_EVENT_LINKED:	/* Out event received when Linkendpoint is successful with VM */
				{
					VidyoClientOutEventLinked *outEventLinked;
					outEventLinked = (VidyoClientOutEventLinked *)event_data->data;
					VidyoUint errorCode;
					errorCode = outEventLinked->error;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_ROOM_LINK:
				{
					VidyoClientOutEventRoomLink	*outEventRoomLink;
					outEventRoomLink = (VidyoClientOutEventRoomLink *)event_data->data;

					if (outEventRoomLink->error != VIDYO_CLIENT_ERROR_NONE)
					{
						if (m_pDlgLogin) 
							m_pDlgLogin->LoginErrorTips(outEventRoomLink->error);
					}

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_SIGNED_IN:	/* Out event received when user is signed in successfully */
				{
					VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONNECTIVITY_INFO, &ConnectivityInfo, sizeof(ConnectivityInfo));
					if (!ConnectivityInfo.guestLogin)
					{
						//Push(ntContactsUI);
						m_pDlgContacts->InitUI();
					}

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_SIGNED_OUT:	/* Out event when user sign out is successful */
				{
					outEventSignedOut = (VidyoClientOutEventSignedOut *)event_data->data;
					//if (m_pDlgLogin) m_pDlgLogin->LoginErrorTips(outEventSignedOut->error);

					//Pop(ntContactsUI);
					//ShowUI();

					if (m_pDlgContacts)
					{
						m_pDlgContacts->SignOut();
					}
					
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_PORTAL_PREFIX:
				{
					VidyoClientOutEventPortalPrefix *outEventPortalPrefix;
					outEventPortalPrefix = (VidyoClientOutEventPortalPrefix *)event_data->data;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MOUSE_DOWN:
				{
					VidyoUint button, count, xPos, yPos;
					VidyoClientOutEventMouse *mouseEvent;
					mouseEvent = (VidyoClientOutEventMouse *)event_data->data;
					button = mouseEvent->button;
					count = mouseEvent->count;
					xPos = mouseEvent->xPos;
					yPos = mouseEvent->yPos;
					if (m_pDlgConference) m_pDlgConference->ShowToolbar(false);
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MOUSE_MOVE:
				{
					VidyoClientOutEventMouse *moveEvent;
					moveEvent = (VidyoClientOutEventMouse *)event_data->data;

					if (m_pDlgConference)
					{
						if ((m_iMouseXPos != moveEvent->xPos) || (m_iMouseYPos != moveEvent->yPos))
						{
							m_iMouseXPos = moveEvent->xPos;
							m_iMouseYPos = moveEvent->yPos;
							m_pDlgConference->ShowToolbar();
						}
					}

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_DEVICE_SELECTION_CHANGED:
				{
					VidyoClientOutEventDeviceSelectionChanged *OutEventDeviceSelectionChanged;
					OutEventDeviceSelectionChanged = (VidyoClientOutEventDeviceSelectionChanged *)event_data->data;

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MUTED_AUDIO_IN:
				{
					VidyoClientOutEventMuted *OutEventMutedIn;
					OutEventMutedIn = (VidyoClientOutEventMuted *)event_data->data;
					g_tBtnStatus.bMuteAudioIn = OutEventMutedIn->isMuted && true;
					NotifyWindow();
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MUTED_AUDIO_OUT:
				{
					VidyoClientOutEventMuted *OutEventMutedOut;
					OutEventMutedOut = (VidyoClientOutEventMuted *)event_data->data;
					g_tBtnStatus.bMuteAudioOut = OutEventMutedOut->isMuted && true;
					NotifyWindow();
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_PREVIEW:
				{
					VidyoClientOutEventPreview *outEventPreview;
					outEventPreview = (VidyoClientOutEventPreview *)event_data->data;

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MUTED_VIDEO:
				{
					VidyoClientOutEventMuted *OutEventMutedVideo;
					OutEventMutedVideo = (VidyoClientOutEventMuted *)event_data->data;
					g_tBtnStatus.bMuteVideo = OutEventMutedVideo->isMuted && true;
					NotifyWindow();
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MUTED_SERVER_AUDIO_IN:	/* Indicate change of mute state of audio input (capture/microphone) device, due to server */
				{
					VidyoClientOutEventMuted *OutEventMutedServerAudioIn;
					OutEventMutedServerAudioIn = (VidyoClientOutEventMuted *)event_data->data;
					g_tBtnStatus.bMuteServerAuidoIn = OutEventMutedServerAudioIn->isMuted && true;
					NotifyWindow();
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MUTED_SERVER_VIDEO:	/* Indicate change of privacy state of video (capture/camera) device, due to server */
				{
					VidyoClientOutEventMuted *OutEventMutedServerVideo;
					OutEventMutedServerVideo = (VidyoClientOutEventMuted *)event_data->data;
					g_tBtnStatus.bMuteServerVideo = OutEventMutedServerVideo->isMuted && true;
					NotifyWindow();
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MEDIA_CONTROL:	/* Server-based media control command details against a media device type */
				{
					VidyoClientOutEventMediaControl *OutEventMediaControl;
					OutEventMediaControl = (VidyoClientOutEventMediaControl *)event_data->data;

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_LICENSE:	/* Report licensing on Portal succes/error */
				{
					VidyoClientOutEventLicense *OutEventLicense;
					OutEventLicense = (VidyoClientOutEventLicense *)event_data->data;

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_CALL_STATE:
				{
					VidyoClientOutEventCallState *OutEventCallState;
					OutEventCallState = (VidyoClientOutEventCallState *)event_data->data;

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_SHARED_SCREEN_DISCONNECTED:
				{
					VidyoClientOutEventAddShare *OutEventAddShare;
					OutEventAddShare = (VidyoClientOutEventAddShare *)event_data->data;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_ADD_SHARE:
				{
					VidyoClientOutEventAddShare *OutEventAddShare;
					OutEventAddShare = (VidyoClientOutEventAddShare *)event_data->data;

					VidyoClientRequestWindowShares Request = {LIST_SHARING_WINDOWS};
					memset(&Request, 0, sizeof(Request));
					if (VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_WINDOW_SHARES, &Request, sizeof(Request)) != VIDYO_FALSE){
						break;
					}

					Request.requestType = TOGGLE_SHARING_WINDOW;
					if (VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_WINDOW_SHARES, &Request, sizeof(Request)) != VIDYO_FALSE){
					}

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_REMOVE_SHARE:
				{
					VidyoClientOutEventRemoveShare *OutEventRemoveShare;
					OutEventRemoveShare = (VidyoClientOutEventRemoveShare *)event_data->data;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_SHARE_CLOSED:
				{
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_RESIZE:
				{
					VidyoClientOutEventResize *OutEventResize;
					OutEventResize = (VidyoClientOutEventResize *)event_data->data;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_ECHO_DETECTED:
				{
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_KEYDOWN:
				{
					VidyoClientOutEventKeyboard *OutEventKeyDown;
					OutEventKeyDown = (VidyoClientOutEventKeyboard *)event_data->data;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_KEYUP:
				{
					VidyoClientOutEventKeyboard *OutEventKeyUp;
					OutEventKeyUp = (VidyoClientOutEventKeyboard *)event_data->data;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_MOUSE_SCROLL:
				{
					VidyoUint button_t1, count_t1, xPos_t1, yPos_t1;
					VidyoClientOutEventMouse *mouseEvent_t1;
					mouseEvent_t1 = (VidyoClientOutEventMouse *)event_data->data;

					button_t1 = mouseEvent_t1->button;
					count_t1 = mouseEvent_t1->count;
					xPos_t1 = mouseEvent_t1->xPos;
					yPos_t1 = mouseEvent_t1->yPos;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_GROUP_CHAT:
				{
					outEventGroupChat = (VidyoClientOutEventGroupChat *)event_data->data;
					m_pDlgConference->RevGroupChat(outEventGroupChat);
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_PRIVATE_CHAT:
				{
					VidyoClientOutEventPrivateChat *outEventPrivateChat;
					outEventPrivateChat = (VidyoClientOutEventPrivateChat *)event_data->data;
					m_pDlgConference->RevPrivateChat(outEventPrivateChat);
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_LECTURE_HAND_CLEARED:
				{
					if (m_pDlgConference) m_pDlgConference->ClearRaisehand();
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_PARTICIPANTS_LIMIT:
				{
					VidyoClientOutEventParticipantsLimit *outLimit;
					outLimit = (VidyoClientOutEventParticipantsLimit *)event_data->data;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_FECC_LIST_UPDATED:
				{
					VidyoClientRequestFeccList requestFeccList = {0};
					if (VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_FECC_LIST, &requestFeccList, sizeof(requestFeccList)) != VIDYO_FALSE) {
						//TRACE(L"VidyoClient sample - SendReq for VIDYO_CLIENT_REQUEST_GET_FECC_LIST failed.");
					}

					if (requestFeccList.numberParticipants == 0) {
						//TRACE(L"VidyoClient sample - No FECC cameras.");
					}

					if (requestFeccList.numberParticipants > 0) {
						strcpy_s(panLeft.uri, sizeof(panLeft.uri), requestFeccList.uri[0]);
						strcpy_s(panRight.uri, sizeof(panRight.uri), requestFeccList.uri[0]);
						strcpy_s(tiltUp.uri, sizeof(tiltUp.uri), requestFeccList.uri[0]);
						strcpy_s(tiltDown.uri, sizeof(tiltDown.uri), requestFeccList.uri[0]);
						strcpy_s(zoomIn.uri, sizeof(zoomIn.uri), requestFeccList.uri[0]);
						strcpy_s(zoomOut.uri, sizeof(zoomOut.uri), requestFeccList.uri[0]);
						strcpy_s(stop.uri, sizeof(stop.uri), requestFeccList.uri[0]);
						strcpy_s(freeze.uri, sizeof(freeze.uri), requestFeccList.uri[0]);
						strcpy_s(unfreeze.uri, sizeof(unfreeze.uri), requestFeccList.uri[0]);
					}
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_INCOMING_CALL:
				{
					outEventincomingCall = (VidyoClientOutEventIncomingCall *)event_data->data;

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_CONFERENCE_ACTIVE:
				{
					Sleep(500); // without sleep will cannot get conference id

					//VidyoClientInEventSetDynamicWatchVideoSource vidyoWatchVideoSource={0};
					//vidyoWatchVideoSource.numParticipants=4;
					//VidyoClientDynamicWatchView watchView={640,480,5};
					//for(int i=0; i<4;i++) vidyoWatchVideoSource.dynamicViews[i]=watchView;
					////send login-event to	VidyoClient
					//VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_SET_DYNAMIC_WATCH_VIDEO_SOURCE,&vidyoWatchVideoSource,sizeof(VidyoClientInEventSetDynamicWatchVideoSource));

					Pop(ntProcessing);

					Push(ntConferenceUI);
					ShowUI();

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_VIDEO_FRAME_RECEIVED:
				{
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_CONFERENCE_ENDED:
				{
					g_tBtnStatus.bMuteVideo = false;
					g_tBtnStatus.bMuteAudioIn = false;
					g_tBtnStatus.bMuteAudioOut = false;
					Pop(ntConferenceUI);
					ShowUI();
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_PARTICIPANTS_CHANGED:
				{
					VidyoClientOutEventParticipantsChanged *OutEventParticipantsChanged;
					OutEventParticipantsChanged = (VidyoClientOutEventParticipantsChanged *)event_data->data;
					if (m_pDlgConference) m_pDlgConference->ParticipantsChanged(OutEventParticipantsChanged->participantCount);
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_LECTURE_MODE_STAGE:
				{
					VidyoClientOutEventLectureModeStage *OutEventLectureModeStage;
					OutEventLectureModeStage = (VidyoClientOutEventLectureModeStage *)event_data->data;

					if (m_pDlgConference)
						m_pDlgConference->SetLectureStage(OutEventLectureModeStage->stage);

					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_LECTURE_MODE_PRESENTER_CHANGED:
				{
					VidyoClientOutEventLectureModePresenter *OutEventLectureModePresenter;
					OutEventLectureModePresenter = (VidyoClientOutEventLectureModePresenter *)event_data->data;
					unsigned int iLen = strlen(OutEventLectureModePresenter->uriOfPresenter);
					m_pDlgConference->SetPresenter(iLen && true);
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_PRECALL_TEST_DEVICE:
				{
					VidyoClientOutEventPrecallTestDevice *OutEventPrecallTestDevice;
					OutEventPrecallTestDevice = (VidyoClientOutEventPrecallTestDevice *)event_data->data;
					break;
				}
			case VIDYO_CLIENT_OUT_EVENT_CLIENT_VERSION_ON_SERVER:
				{
					VidyoClientOutEventClientVersionOnServer *OutEventClientVersion;
					OutEventClientVersion = (VidyoClientOutEventClientVersionOnServer *)event_data->data;
					break;
				}
			}
		}
		free(event_data);
	}
	return 0;
}