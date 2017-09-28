#include "StdAfx.h"
#include "MsgApplyForbid.h"
#include "../../include/Util/Util.h"

CMsgApplyForbid::CMsgApplyForbid(void)
{
}


CMsgApplyForbid::~CMsgApplyForbid(void)
{
}

LRESULT CMsgApplyForbid::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CMsgApplyForbid::InitWindow()
{
	CLabelUI* labMsgApplyForbid = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labMsgApplyForbid")));
	CButtonUI* pbtnOK	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnOK")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("COMMON"), _T("labMsgApplyForbid"), szText, MAX_PATH);
	labMsgApplyForbid->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnOK"), szText, MAX_PATH);
	pbtnOK->SetText(szText);
}

void CMsgApplyForbid::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		Close(0);
	}
}