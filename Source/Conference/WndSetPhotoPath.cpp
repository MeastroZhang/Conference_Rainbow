#include "StdAfx.h"
#include "WndSetPhotoPath.h"
#include "../../include/Util/Util.h"

CWndSetPhotoPath::CWndSetPhotoPath(void)
{
}


CWndSetPhotoPath::~CWndSetPhotoPath(void)
{
}

LRESULT CWndSetPhotoPath::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndSetPhotoPath::InitWindow()
{
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
	CLabelUI* labPath = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labPath")));
	CEditUI* editPhoto	= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPhoto")));
	CButtonUI* btnChangePath	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnChangePath")));
	CButtonUI* btnOpen	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnOpen")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SETPHOTOPATH"), _T("labCaption"), szText, MAX_PATH);
	labCaption->SetText(szText);
	project_language_loadstring(_T("SETPHOTOPATH"), _T("labPath"), szText, MAX_PATH);
	labPath->SetText(szText);
	project_language_loadstring(_T("SETPHOTOPATH"), _T("btnChangePath"), szText, MAX_PATH);
	btnChangePath->SetText(szText);
	project_language_loadstring(_T("SETPHOTOPATH"), _T("btnOpen"), szText, MAX_PATH);
	btnOpen->SetText(szText);

	ReadIniString(_T("SETTING"), _T("PhotoPath"), PATH_PHOTO, szText, 255);
	editPhoto->SetText(szText);
}

void CWndSetPhotoPath::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnChangePath"))
		{
			TCHAR szBuffer[MAX_PATH] = {0}; 
			SetPath(NULL, szBuffer);

			if (szBuffer[0] != '\0')
			{
				CEditUI* pEditPath	= static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editPhoto")));
				pEditPath->SetText(szBuffer);
				WriteIniString(_T("SETTING"), _T("PhotoPath"), szBuffer);
			}
		}
		else if (msg.pSender->GetName() == _T("btnOpen"))
		{
			TCHAR szText[255]=_T("");
			ReadIniString(_T("SETTING"), _T("PhotoPath"), PATH_PHOTO, szText, 255);

			OpenPath(m_hWnd, szText);
		}
		else if (msg.pSender->GetName() == _T("btnClose"))
		{
			Close(0);
		}
	}
}