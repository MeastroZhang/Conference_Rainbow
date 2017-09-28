#include "StdAfx.h"
#include "WndLectureMode.h"
#include "../../include/Util/Util.h"

CWndLectureMode::CWndLectureMode(void)
{
}


CWndLectureMode::~CWndLectureMode(void)
{
}

LRESULT CWndLectureMode::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT CWndLectureMode::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CWndLectureMode::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_LECTURE_NOTE)
	{
		ShowWindow(false);
		KillTimer(m_hWnd, TIMER_LECTURE_NOTE);
	}

	bHandled = FALSE;
	return 0;
}

void CWndLectureMode::UpdateUI(BYTE bFlag)
{
	CLabelUI* pLab = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labLectureMode"));
	TCHAR szText[MAX_PATH];
	CDuiString strTemp;
	strTemp.Format(L"labLecture%d", bFlag);
	project_language_loadstring(_T("COMMON"), strTemp, szText, MAX_PATH);
	pLab->SetText(szText);
	ShowWindow();

	KillTimer(m_hWnd, TIMER_LECTURE_NOTE);
	if (bFlag > 1)
	{
		SetTimer(m_hWnd, TIMER_LECTURE_NOTE, 1000, NULL);
	}
}