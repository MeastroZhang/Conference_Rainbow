#include "StdAfx.h"
#include "MsgModeratorExit.h"
#include "../../include/Util/Util.h"

CMsgModeratorExit::CMsgModeratorExit(void)
{
}


CMsgModeratorExit::~CMsgModeratorExit(void)
{
}

LRESULT CMsgModeratorExit::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CMsgModeratorExit::InitWindow()
{
	CLabelUI* labModeratorExit1 = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labModeratorExit1")));
	CLabelUI* labModeratorExit2 = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labModeratorExit2")));
	CLabelUI* labModeratorExit3 = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labModeratorExit3")));
	CButtonUI* pbtnCancel	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnCancel")));
	CButtonUI* btnEndConference	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnEndConference")));
	CButtonUI* btnExitConference	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnExitConference")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("MSGWND"), _T("labModeratorExit1"), szText, MAX_PATH);
	labModeratorExit1->SetText(szText);
	project_language_loadstring(_T("MSGWND"), _T("labModeratorExit2"), szText, MAX_PATH);
	labModeratorExit2->SetText(szText);
	project_language_loadstring(_T("MSGWND"), _T("labModeratorExit3"), szText, MAX_PATH);
	labModeratorExit3->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnCancel"), szText, MAX_PATH);
	pbtnCancel->SetText(szText);
	project_language_loadstring(_T("MSGWND"), _T("btnEndConference"), szText, MAX_PATH);
	btnEndConference->SetText(szText);
	project_language_loadstring(_T("MSGWND"), _T("btnExitConference"), szText, MAX_PATH);
	btnExitConference->SetText(szText);
}

void CMsgModeratorExit::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnEndConference"))
		{
			::PostMessageW(GetParent(m_hWnd), WM_END_CONFERENCE, NULL, NULL);
		}
		else if (msg.pSender->GetName() == _T("btnExitConference"))
		{
			::PostMessageW(GetParent(m_hWnd), WM_EXIT_CONFERENCE, NULL, NULL);
		}
		Close(0);
	}
}