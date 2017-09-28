#include "StdAfx.h"
#include "WndRightTopBar.h"


CWndRightTopBar::CWndRightTopBar(void)
{
}


CWndRightTopBar::~CWndRightTopBar(void)
{
}

LRESULT CWndRightTopBar::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndRightTopBar::ShowUI(bool bShow /* = true */, bool bShowChat /* = true */, bool bFull /* = true */)
{
	if (bShow)
	{
		int iCount = 0;
		CButtonUI* btnFlgModerator = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnFlgModerator"));
		CButtonUI* btnFlgRaiseHand = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnFlgRaiseHand"));
		CButtonUI* btnNoVideo = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnNoVideo"));
		CButtonUI* btnNoAudio = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnNoAudio"));
		CButtonUI* btnNoSpeaker = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnNoSpeaker"));
		CButtonUI* btnRestore = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"btnRestore"));

		if (m_eLectureStage == VIDYO_CLIENT_LECTURE_MODE_PRESENT)
		{
			iCount ++;
			btnFlgModerator->SetVisible();
		}
		else btnFlgModerator->SetVisible(false);

		if (m_tSelfStatus.bMuteVideo || m_tSelfStatus.bMuteServerVideo)
		{
			iCount ++;
			btnNoVideo->SetVisible();
		}
		else btnNoVideo->SetVisible(false);

		if (m_tSelfStatus.bMuteAudioIn || m_tSelfStatus.bMuteServerAuidoIn)
		{
			iCount ++;
			btnNoAudio->SetVisible();
		}
		else btnNoAudio->SetVisible(false);

		if (m_tSelfStatus.bMuteAudioOut)
		{
			iCount ++;
			btnNoSpeaker->SetVisible();
		}
		else btnNoSpeaker->SetVisible(false);

		if (bFull)
		{
			iCount ++;
			btnRestore->SetVisible();
		}
		else btnRestore->SetVisible(false);

		RECT rc;
		GetClientRect(GetParent(m_hWnd), &rc);
		::SetWindowPos(m_hWnd, NULL, rc.right-iCount*32-10-bShowChat*300, 40, 
			iCount*32, 32, NULL);
	}

	ShowWindow(bShow);
}

void CWndRightTopBar::SetLectureStage(VidyoClientLectureStage lectureStage)
{
	m_eLectureStage = lectureStage;
}

void CWndRightTopBar::SetSelfContorlStatus(tSelfControlStatus selfStatus)
{
	m_tSelfStatus = selfStatus;
}

void CWndRightTopBar::Notify(TNotifyUI& msg)
{
	if (msg.sType == L"click")
	{
		if (msg.pSender->GetName() == L"btnRestore")
		{
			::PostMessageW(GetParent(m_hWnd), WM_SYSCOMMAND, SC_RESTORE, NULL);
		}
	}
}