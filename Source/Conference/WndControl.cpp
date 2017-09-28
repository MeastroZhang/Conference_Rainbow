#include "StdAfx.h"
#include "WndControl.h"
#include "WSgSoap.h"

CWndControl::CWndControl(HWND hParent)
{
	m_hParent = hParent;
}


CWndControl::~CWndControl(void)
{
	m_hParent = NULL;
}

LRESULT CWndControl::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndControl::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnClose")));
	m_pTileShare = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("tileShare")));

	SIZE sz;
	sz.cx = 200;
	sz.cy = 200;
	m_pTileShare->SetItemSize(sz);
}

void CWndControl::OnPrepare()
{
	CButtonUI* pBtnShareScreen = new CButtonUI();
	pBtnShareScreen->SetName(_T("shareScreen"));
	pBtnShareScreen->SetText(_T("关闭全体摄像头"));
	pBtnShareScreen->SetBkColor(0xFF008000);
	m_pTileShare->Add(pBtnShareScreen);

	CButtonUI* pBtnShareStop = new CButtonUI();
	pBtnShareStop->SetName(_T("shareStop"));
	pBtnShareStop->SetText(_T("取消禁止全体摄像头"));
	pBtnShareStop->SetBkColor(0xFF008000);
	m_pTileShare->Add(pBtnShareStop);
}

void CWndControl::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) OnPrepare();
	else if( msg.sType == _T("click") ) 
	{
		if( msg.pSender == m_pCloseBtn ) 
		{
			ShowWindow(false);
		}
		else if ( msg.pSender->GetName() == _T("shareScreen") )
		{
			SoapMuteVidyoServerAll("", true);
		}
		else if ( msg.pSender->GetName() == _T("shareStop") )
		{
			SoapMuteVidyoServerAll("", false);
		}
	}
}