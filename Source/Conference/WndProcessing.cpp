#include "StdAfx.h"
#include "WndProcessing.h"
#include "../../include/Util/Util.h"

extern RECT rect[3];
extern int numScreen;

CWndProcessing::CWndProcessing(void)
{
}


CWndProcessing::~CWndProcessing(void)
{
}

LRESULT CWndProcessing::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT CWndProcessing::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CWndProcessing::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(4);
	m_WndShadow.SetPosition(0,0);

	return WindowImplBase::OnCreate(uMsg, wParam, lParam, bHandled);
}

void CWndProcessing::ShowUI(bool bShow /* = true */)
{
	int iScreenIndex = ReadIniInt(L"USERINFO", L"ScreenNo", 0);
	if (numScreen == 2)
	{
		MoveWindow(m_hWnd, rect[1].left, rect[1].top, rect[1].right-rect[1].left, rect[1].bottom-rect[1].top, TRUE);
	}
	ShowWindow(bShow);
}

void CWndProcessing::InitUI()
{
	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("COMMON"), _T("labJoining"), szText, MAX_PATH);
	CLabelUI* labJoining = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labJoining"));
	labJoining->SetText(szText);
}