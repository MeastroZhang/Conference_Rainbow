#include "StdAfx.h"
#include "MsgRoomExit.h"
#include "../../include/Util/Util.h"

CMsgRoomExit::CMsgRoomExit(void)
{
}


CMsgRoomExit::~CMsgRoomExit(void)
{
}

LRESULT CMsgRoomExit::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CMsgRoomExit::InitWindow()
{
	CLabelUI* labExit = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labExit")));
	CButtonUI* pbtnCancel	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnCancel")));
	CButtonUI* pbtnOK	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnOK")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("MSGWND"), _T("labExit"), szText, MAX_PATH);
	labExit->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnCancel"), szText, MAX_PATH);
	pbtnCancel->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnOK"), szText, MAX_PATH);
	pbtnOK->SetText(szText);
}

void CMsgRoomExit::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnOK"))
		{
			::PostMessageW(GetParent(m_hWnd), WM_EXIT_CONFERENCE, NULL, NULL);
		}
		Close(0);
	}
}