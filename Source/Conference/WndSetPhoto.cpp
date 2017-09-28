#include "StdAfx.h"
#include "WndSetPhoto.h"
#include "../../include/Util/Util.h"
#include "WndSetPhotoPath.h"

CWndSetPhoto::CWndSetPhoto(void)
{
}


CWndSetPhoto::~CWndSetPhoto(void)
{
}

LRESULT CWndSetPhoto::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndSetPhoto::InitWindow()
{
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
	CButtonUI* btnOpenFile = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnOpenFile")));
	CButtonUI* btnSetPath = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnSetPath")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SETPHOTO"), _T("labCaption"), szText, MAX_PATH);
	labCaption->SetText(szText);
	project_language_loadstring(_T("SETPHOTO"), _T("btnOpenFile"), szText, MAX_PATH);
	btnOpenFile->SetText(szText);
	project_language_loadstring(_T("SETPHOTO"), _T("btnSetPath"), szText, MAX_PATH);
	btnSetPath->SetText(szText);
}

void CWndSetPhoto::Notify(TNotifyUI& msg)
{
	if ( msg.sType == _T("click") )
	{
		if ( msg.pSender->GetName() == _T("btnOpenFile") )
		{
			TCHAR szText[255]=_T("");
			ReadIniString(_T("SETTING"), _T("PhotoPath"), PATH_PHOTO, szText, 255);

			OpenPath(m_hWnd, szText);
		}
		else if ( msg.pSender->GetName() == _T("btnSetPath") )
		{
			CWndSetPhotoPath* pPopWnd = new CWndSetPhotoPath();
			pPopWnd->Create(GetParent(GetParent(GetParent(m_hWnd))), _T("SetPhotoPath"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
			pPopWnd->CenterWindow();
			pPopWnd->ShowModal();
		}
	}
}