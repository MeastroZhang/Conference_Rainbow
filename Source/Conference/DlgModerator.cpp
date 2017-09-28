#include "StdAfx.h"
#include "DlgModerator.h"
#include "WSgSoap.h"
#include "MsgCommon.h"

CDlgModerator::CDlgModerator(void)
{
	m_tServerControlStatus.bLocked = FALSE;
	m_tServerControlStatus.bMuteVide = FALSE;
	m_tServerControlStatus.bMuteAudio = FALSE;
	m_tServerControlStatus.bRecord = FALSE;
	m_tServerControlStatus.bRecordPause = FALSE;
	m_tServerControlStatus.lectureMode = VIDYO_CLIENT_LECTURE_MODE_STOP;

	m_iIndexSelected = 0;
	
	m_ptParticipants = NULL;
}


CDlgModerator::~CDlgModerator(void)
{
	
}

LRESULT CDlgModerator::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CDlgModerator::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT CDlgModerator::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_LECTUREMODE)
	{
		UpdateParticipants();
	}

	bHandled = FALSE;
	return 0;
}

void CDlgModerator::SetLectureStage(VidyoClientLectureStage lectureMode)
{
	m_tServerControlStatus.lectureMode = lectureMode;
	UpDataUI();
}

void CDlgModerator::SetModeratorPin(char* szModeratorPin)
{
	memset(m_szModeratorPin, 0, sizeof(m_szModeratorPin));
	memcpy(m_szModeratorPin, szModeratorPin, sizeof(m_szModeratorPin));
}

void CDlgModerator::GetParticipants()
{
	// Release data
	if (m_ptParticipants)
	{
		m_ptParticipants->total = 0;
		delete []m_ptParticipants->participantData;
		m_ptParticipants->participantData = NULL;

		delete m_ptParticipants;
		m_ptParticipants = NULL;
	}
	// Init Data
	m_ptParticipants = new VidyoClientParticipantList;
	m_ptParticipants->total = 0;
	m_ptParticipants->participantData = NULL;

	Sleep(500);
	SoapGetLectureModeParticipants(m_szConferenceID, m_ptParticipants, m_szModeratorPin);
}

void CDlgModerator::UpdateParticipants(unsigned int uiCount /* = 0 */)
{
	GetParticipants();
	if (m_ptParticipants->total > 0)
	{
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
		pList->RemoveAll();
		TCHAR szBuf[MAX_PATH] = {0};
		for (unsigned int i = 0; i < m_ptParticipants->total; i++)
		{
			CDialogBuilder builder;
			CListContainerElementUI* pHorUI = static_cast<CListContainerElementUI*>(builder.Create(_T ("\\DlgModerator\\ListItem.xml")));

			ChangUtf8ToText(m_ptParticipants->participantData[i].displayName, szBuf);
			CLabelUI* pLabItem = static_cast<CLabelUI*>(pHorUI->FindSubControl(L"labDisplayName"));
			pLabItem->SetText(szBuf);

			CButtonUI* pBtnPresent = static_cast<CButtonUI*>(pHorUI->FindSubControl(L"btnPresent"));
			CButtonUI* pBtnNoPresent = static_cast<CButtonUI*>(pHorUI->FindSubControl(L"btnNoPresent"));
			pBtnPresent->SetTag(i);
			pBtnNoPresent->SetTag(i);
			pBtnPresent->SetVisible(m_ptParticipants->participantData[i].presenter && m_ptParticipants->lectureMode);
			pBtnNoPresent->SetVisible(!m_ptParticipants->participantData[i].presenter && m_ptParticipants->lectureMode);

			CButtonUI* pBtnVideo = static_cast<CButtonUI*>(pHorUI->FindSubControl(L"btnVideo"));
			CButtonUI* pBtnVideoClosed = static_cast<CButtonUI*>(pHorUI->FindSubControl(L"btnVideoClosed"));
			pBtnVideo->SetTag(i);
			pBtnVideoClosed->SetTag(i);
			pBtnVideo->SetVisible(m_ptParticipants->participantData[i].video && true);
			pBtnVideoClosed->SetVisible(!m_ptParticipants->participantData[i].video && true);

			CButtonUI* pBtnMic = static_cast<CButtonUI*>(pHorUI->FindSubControl(L"btnMic"));
			CButtonUI* pBtnMicClosed = static_cast<CButtonUI*>(pHorUI->FindSubControl(L"btnMicClosed"));
			pBtnMic->SetTag(i);
			pBtnMicClosed->SetTag(i);
			pBtnMic->SetVisible(m_ptParticipants->participantData[i].audio && true);
			pBtnMicClosed->SetVisible(!m_ptParticipants->participantData[i].audio && true);

			CButtonUI* pBtnKickOut = static_cast<CButtonUI*>(pHorUI->FindSubControl(L"btnKickOut"));
			pBtnKickOut->SetTag(i);

			CButtonUI* pBtnRaiseHand = static_cast<CButtonUI*>(pHorUI->FindSubControl(L"btnRaiseHand"));
			pBtnRaiseHand->SetTag(i);
			pBtnRaiseHand->SetVisible(m_ptParticipants->participantData[i].handRaised && m_ptParticipants->lectureMode);

			pList->Add(pHorUI);
		}
		pList->SelectItem(m_iIndexSelected);
	}
}

void CDlgModerator::UpdataMuteVideo(bool bMuteVideo)
{
	m_tServerControlStatus.bMuteVide = bMuteVideo;
	UpDataUI();
}

void CDlgModerator::UpdataMuteAudio(bool bMuteAudio)
{
	m_tServerControlStatus.bMuteAudio = bMuteAudio;
	UpDataUI();
}

void CDlgModerator::UpDataUI()
{
	CLabelUI* labTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labTitle"));
	TCHAR szText[MAX_PATH];
	if (m_tServerControlStatus.lectureMode < VIDYO_CLIENT_LECTURE_MODE_STOP) 
		project_language_loadstring(_T("MODERATOR"), _T("labTitleLecture"), szText, MAX_PATH);
	else 
		project_language_loadstring(_T("MODERATOR"), _T("labTitleConference"), szText, MAX_PATH);
	labTitle->SetText(szText);

	if (m_tServerControlStatus.lectureMode == VIDYO_CLIENT_LECTURE_MODE_START)
	{
		SetTimer(m_hWnd, TIMER_LECTUREMODE, 2000, NULL);
	}
	else if (m_tServerControlStatus.lectureMode == VIDYO_CLIENT_LECTURE_MODE_STOP)
	{
		KillTimer(m_hWnd, TIMER_LECTUREMODE);
	}
	CButtonUI* btnModeratorMode = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnModeratorMode"));
	CButtonUI* btnConferenceMode = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnConferenceMode"));
	btnModeratorMode->SetVisible((m_tServerControlStatus.lectureMode<VIDYO_CLIENT_LECTURE_MODE_STOP)?true:false);
	btnConferenceMode->SetVisible((m_tServerControlStatus.lectureMode<VIDYO_CLIENT_LECTURE_MODE_STOP)?false:true);

	CButtonUI* btnLock = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnLock"));
	CButtonUI* btnUnLock = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnUnLock"));
	btnLock->SetVisible(!m_tServerControlStatus.bLocked);
	btnUnLock->SetVisible(m_tServerControlStatus.bLocked);

	CButtonUI* btnMuteVideoAll = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnMuteVideoAll"));
	CButtonUI* btnUnMuteVideoAll = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnUnMuteVideoAll"));
	btnMuteVideoAll->SetVisible(!m_tServerControlStatus.bMuteVide);
	btnUnMuteVideoAll->SetVisible(m_tServerControlStatus.bMuteVide);

	CButtonUI* btnMuteAudioAll = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnMuteAudioAll"));
	CButtonUI* btnUnMuteAudioAll = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnUnMuteAudioAll"));
	btnMuteAudioAll->SetVisible(!m_tServerControlStatus.bMuteAudio);
	btnUnMuteAudioAll->SetVisible(m_tServerControlStatus.bMuteAudio);

	CButtonUI* btnRecord = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnRecord"));
	CButtonUI* btnRecordPause = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnRecordPause"));
	CButtonUI* btnRecordRecover = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnRecordRecover"));
	CButtonUI* btnStopRecord = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnStopRecord"));
	btnRecord->SetVisible(!m_tServerControlStatus.bRecord);
	btnStopRecord->SetEnabled(m_tServerControlStatus.bRecord);
	btnRecordPause->SetVisible(!m_tServerControlStatus.bRecordPause && m_tServerControlStatus.bRecord);
	btnRecordRecover->SetVisible(m_tServerControlStatus.bRecordPause && m_tServerControlStatus.bRecord);

	SetForegroundWindow(m_hWnd);
}

void CDlgModerator::ShowUI(bool bShow)
{
	if (!bShow) ReleaseData();
	else UpdateParticipants();
	ShowWindow(bShow);
}

void CDlgModerator::InitData(bool bLocked /* = false */)
{
	m_tServerControlStatus.bLocked = bLocked;
	memset(m_szModeratorPin, 0, sizeof(m_szModeratorPin));

	memset(m_szConferenceID, 0, sizeof(m_szConferenceID));
	SoapGetConferenceID(m_szConferenceID);

	UpDataUI();
	UpdateParticipants();
}

void CDlgModerator::ReleaseData()
{
	m_tServerControlStatus.lectureMode = VIDYO_CLIENT_LECTURE_MODE_STOP;
	m_tServerControlStatus.bLocked = FALSE;
	m_tServerControlStatus.bMuteVide = FALSE;
	m_tServerControlStatus.bMuteAudio = FALSE;
	m_tServerControlStatus.bRecord = FALSE;
	m_tServerControlStatus.bRecordPause = FALSE;

	if (m_ptParticipants)
	{
		if (m_ptParticipants->total)
		{
			delete [] m_ptParticipants->participantData;
			m_ptParticipants->participantData = NULL;
		}

		delete m_ptParticipants;
		m_ptParticipants = NULL;
	}
}

void CDlgModerator::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if ( msg.pSender->GetName() == _T("btnClose") )
		{
			ShowWindow(false);
		}
		else if ( msg.pSender->GetName() == _T("btnMin") )
		{
			ShowWindow(false);
		}
		else if ( msg.pSender->GetName() == L"btnForbidAll" )
		{
			if (SoapDismissAllRaisedHand(m_szConferenceID, m_szModeratorPin))
			{
				UpdateParticipants();
			}
		}
		else if ( msg.pSender->GetName() == L"btnModeratorMode" )
		{
			if (SoapStopLectureMode(m_szConferenceID, m_szModeratorPin))
			{
				UpdateParticipants();
			}
		}
		else if ( msg.pSender->GetName() == L"btnConferenceMode" )
		{
			if (SoapStartLectureMode(m_szConferenceID, m_szModeratorPin))
			{
				UpdateParticipants();
			}
		}
		else if ( msg.pSender->GetName() == L"btnLock" )
		{
			if (SoapLockRoom(m_szConferenceID, m_szModeratorPin))
			{
				m_tServerControlStatus.bLocked = TRUE;
				UpDataUI();
			}
		}
		else if ( msg.pSender->GetName() == L"btnUnLock" )
		{
			if (SoapUnLockRoom(m_szConferenceID, m_szModeratorPin))
			{
				m_tServerControlStatus.bLocked = FALSE;
				UpDataUI();
			}
		}
		else if ( msg.pSender->GetName() == L"btnMuteVideoAll" )
		{
			if (SoapMuteVidyoServerAll(m_szConferenceID, true, m_szModeratorPin))
			{
				m_tServerControlStatus.bMuteVide = TRUE;
				UpdateParticipants();
			}
		}
		else if ( msg.pSender->GetName() == L"btnUnMuteVideoAll" )
		{
			if (SoapMuteVidyoServerAll(m_szConferenceID, false, m_szModeratorPin))
			{
				m_tServerControlStatus.bMuteVide = FALSE;
				UpdateParticipants();
			}
		}
		else if ( msg.pSender->GetName() == L"btnMuteAudioAll" )
		{
			if (SoapMuteAudioServerAll(m_szConferenceID, true, m_szModeratorPin))
			{
				m_tServerControlStatus.bMuteAudio = TRUE;
				UpdateParticipants();
			}
		}
		else if ( msg.pSender->GetName() == L"btnUnMuteAudioAll" )
		{
			if (SoapMuteAudioServerAll(m_szConferenceID, false, m_szModeratorPin))
			{
				m_tServerControlStatus.bMuteAudio = FALSE;
				UpdateParticipants();
			}
		}
		else if ( msg.pSender->GetName() == L"btnRecord" )
		{
			if (SoapStartRecording(m_szConferenceID, m_szModeratorPin))
			{
				m_tServerControlStatus.bRecord = true;
				m_tServerControlStatus.bRecordPause = false;
				UpDataUI();
			}
			else
			{
				MsgCommon* pPopWnd = new MsgCommon(MSG_RECORD_DISABLE);
				pPopWnd->Create(m_hWnd, _T("MsgCommon"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
				pPopWnd->CenterWindow();
				pPopWnd->ShowModal();
			}
		}
		else if ( msg.pSender->GetName() == L"btnRecordPause" )
		{
			GetParticipants();
			if (SoapPauseRecording(m_szConferenceID, m_ptParticipants->RecordID, m_szModeratorPin))
			{
				m_tServerControlStatus.bRecord = true;
				m_tServerControlStatus.bRecordPause = true;
				UpDataUI();
			}
		}
		else if ( msg.pSender->GetName() == L"btnRecordRecover" )
		{
			GetParticipants();
			if (SoapResumeRecording(m_szConferenceID, m_ptParticipants->RecordID, m_szModeratorPin))
			{
				m_tServerControlStatus.bRecord = true;
				m_tServerControlStatus.bRecordPause = false;
				UpDataUI();
			}
		}
		else if ( msg.pSender->GetName() == L"btnStopRecord" )
		{
			GetParticipants();
			if (SoapStopRecording(m_szConferenceID, m_ptParticipants->RecordID, m_szModeratorPin))
			{
				m_tServerControlStatus.bRecord = false;
				m_tServerControlStatus.bRecordPause = false;
				UpDataUI();
			}
		}
		else if ( msg.pSender->GetName() == L"btnPresent" )
		{
			m_iIndexSelected = ((CButtonUI*)msg.pSender)->GetTag();
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
			SoapRemovePresenter(m_szConferenceID,m_ptParticipants->participantData[m_iIndexSelected].participantID, m_szModeratorPin);
			UpdateParticipants();
		}
		else if ( msg.pSender->GetName() == L"btnNoPresent" )
		{
			m_iIndexSelected = ((CButtonUI*)msg.pSender)->GetTag();
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
			SoapSetPresenter(m_szConferenceID,m_ptParticipants->participantData[m_iIndexSelected].participantID, m_szModeratorPin);
			UpdateParticipants();
		}
		else if ( msg.pSender->GetName() == L"btnVideo" )
		{
			m_iIndexSelected = ((CButtonUI*)msg.pSender)->GetTag();
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
			SoapStopVideo(m_szConferenceID,m_ptParticipants->participantData[m_iIndexSelected].participantID, m_szModeratorPin);
			UpdateParticipants();
		}
		else if ( msg.pSender->GetName() == L"btnVideoClosed" )
		{
			m_iIndexSelected = ((CButtonUI*)msg.pSender)->GetTag();
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
			SoapStartVideo(m_szConferenceID,m_ptParticipants->participantData[m_iIndexSelected].participantID, m_szModeratorPin);
			UpdateParticipants();
		}
		else if ( msg.pSender->GetName() == L"btnMic" )
		{
			m_iIndexSelected = ((CButtonUI*)msg.pSender)->GetTag();
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
			SoapStopAudio(m_szConferenceID,m_ptParticipants->participantData[m_iIndexSelected].participantID, m_szModeratorPin);
			UpdateParticipants();
		}
		else if ( msg.pSender->GetName() == L"btnMicClosed" )
		{
			m_iIndexSelected = ((CButtonUI*)msg.pSender)->GetTag();
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
			if (SoapStartAudio(m_szConferenceID,m_ptParticipants->participantData[m_iIndexSelected].participantID, m_szModeratorPin))
				UpdateParticipants();
		}
		else if ( msg.pSender->GetName() == L"btnKickOut" )
		{
			m_iIndexSelected = ((CButtonUI*)msg.pSender)->GetTag();
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
			SoapLeaveConference(m_szConferenceID,m_ptParticipants->participantData[m_iIndexSelected].participantID, m_szModeratorPin);
		}
		else if ( msg.pSender->GetName() == L"btnRaiseHand" )
		{
			m_iIndexSelected = ((CButtonUI*)msg.pSender)->GetTag();
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));
			if (SoapDismissRaisedHand(m_szConferenceID,m_ptParticipants->participantData[m_iIndexSelected].participantID, m_szModeratorPin))
				UpdateParticipants();
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		if (msg.pSender->GetName() == L"optSearch")
		{
			if (((COptionUI*)(msg.pSender))->IsSelected())
			{
				CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(L"editSearch"));
				CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listParticipants"));

				CDuiString strSearch = pEdit->GetText();
				TCHAR szName[256] = {0};
				for (unsigned int i = 0; i < m_ptParticipants->total; i++)
				{
					ChangUtf8ToText(m_ptParticipants->participantData[i].displayName, szName);
					if (!strSearch.Compare(szName))
					{
						pParList->SelectItem(i);
					}
				}
			}
			return;
		}
	}
}