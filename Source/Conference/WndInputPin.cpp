#include "StdAfx.h"
#include "WndInputPin.h"

#include "WSgSoap.h"

CWndInputPin::CWndInputPin(eInputWndType wndType, char* ConferenceID)
{
	m_pRoomID = ConferenceID;
	m_eWndType = wndType;
}


CWndInputPin::~CWndInputPin(void)
{
}

LRESULT CWndInputPin::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndInputPin::InitWindow()
{
	CEditUI* editInputPin	= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editInputPin")));
	CButtonUI* btnCancel	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnCancel")));
	CButtonUI* btnOK	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnOK")));

	TCHAR szText[MAX_PATH];
	if (m_eWndType == ntRoomPin)
	{
		project_language_loadstring(_T("COMMON"), _T("editInputRoomPin"), szText, MAX_PATH);
	} 
	else if (m_eWndType == ntModeratorPin)
	{
		project_language_loadstring(_T("COMMON"), _T("editInputModeratorPin"), szText, MAX_PATH);
	}
	editInputPin->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnCancel"), szText, MAX_PATH);
	btnCancel->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnOK"), szText, MAX_PATH);
	btnOK->SetText(szText);
}

void CWndInputPin::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if ((msg.pSender->GetName() == _T("btnCancel")) || (msg.pSender->GetName() == _T("btnClose")))
		{
			Close(0);
		}
		else if (msg.pSender->GetName() == _T("btnOK"))
		{
			CLabelUI* labWrongPin = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labWrongPin"));
			labWrongPin->SetText(L"");

			CEditUI* editInputPin	= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editInputPin")));
			CDuiString strPin = editInputPin->GetText();
			char szPin[64] = {0};
			ChangTextToUtf8(strPin.GetData(), szPin);

			if (m_eWndType == ntRoomPin)
			{
				if (SoapJoinConference(m_pRoomID, szPin))
				{
					TCHAR szText[MAX_PATH];
					project_language_loadstring(_T("COMMON"), _T("labWrongPin"), szText, MAX_PATH);
					labWrongPin->SetText(szText);
				}
				else
				{
					::PostMessageW(GetParent(GetParent(m_hWnd)), WM_JOIN_PROCESS, NULL, NULL);
					Close(0);
				}
			} 
			else if (m_eWndType == ntModeratorPin)
			{
				CEditUI* editInputPin	= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editInputPin")));
				CDuiString strPin = editInputPin->GetText();
				char szPin[64] = {0};
				ChangTextToUtf8(strPin.GetData(), szPin);
				if (SoapGetLectureModeParticipants(m_pRoomID, NULL, szPin))
				{
					::PostMessageW(GetParent(m_hWnd), WM_MODERATOR_CHANGED, NULL, (LPARAM)szPin);
					Close(0);
				}
				else
				{
					TCHAR szText[MAX_PATH];
					project_language_loadstring(_T("COMMON"), _T("labWrongPin"), szText, MAX_PATH);
					labWrongPin->SetText(szText);
				}
			}

			//Close(0);
		}
	}
	else if( msg.sType == _T("setfocus") ) 
	{
		if ( msg.pSender->GetName() == L"editInputPin" )
		{
			CEditUI* editInputPin	= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editInputPin")));
			editInputPin->SetText(L"");
		}
	}
	else if ( msg.sType == L"textchanged" )
	{
		CLabelUI* labWrongPin = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labWrongPin"));
		labWrongPin->SetText(L"");
	}
}