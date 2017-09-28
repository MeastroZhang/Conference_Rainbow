#include "StdAfx.h"
#include "MsgCommon.h"

MsgCommon::MsgCommon(eMsgWndType msgType)
{
	m_eMsgType = msgType;
}


MsgCommon::~MsgCommon(void)
{
}

LRESULT MsgCommon::ResponseDefaultKeyEvent(WPARAM wParam)
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

void MsgCommon::InitWindow()
{
	CLabelUI* labMsg = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labMsg")));
	CButtonUI* pbtnOK	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnOK")));

	TCHAR szText[MAX_PATH];
	TCHAR szKey[64] = {0};
	switch(m_eMsgType)
	{
	case MSG_ROOMLOCKED:
		memcpy(szKey, L"labRoomLocked", sizeof(szKey));
		break;
	case MSG_WRONGPIN:
		memcpy(szKey, L"labWrongPin", sizeof(szKey));
		break;
	case MSG_RECORD_DISABLE:
		memcpy(szKey, L"labRecordDisable", sizeof(szKey));
		break;
	}
	project_language_loadstring(_T("MSGWND"), szKey, szText, MAX_PATH);
	labMsg->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnOK"), szText, MAX_PATH);
	pbtnOK->SetText(szText);
}

void MsgCommon::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		Close(0);
	}
}