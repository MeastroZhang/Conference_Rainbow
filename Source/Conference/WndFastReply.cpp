#include "StdAfx.h"
#include "WndFastReply.h"
#include "../../include/Util/Util.h"

CWndFastReply::CWndFastReply(void)
{
}


CWndFastReply::~CWndFastReply(void)
{
}

LRESULT CWndFastReply::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndFastReply::InitWindow()
{
	CDuiString strTemp;
	TCHAR szBuf[MAX_PATH] = {0};
	CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listReply"));
	pParList->RemoveAll();

	for (int i = 0; i < 5; i++)
	{
		CListContainerElementUI* pListItem  = new CListContainerElementUI();
		pListItem->SetFixedHeight(20);
		pListItem->SetManager(&m_PaintManager, NULL, false);
		strTemp.Format(L"element%d", i+1);
		project_language_loadstring(_T("CHATREPLY"), strTemp, szBuf, MAX_PATH);
		CLabelUI* pLabItem = new CLabelUI;
		pLabItem->SetText(szBuf);
		pLabItem->SetAttribute(L"textpadding", L"10,0,0,0");
		pLabItem->SetAttribute(L"textcolor", L"#FF888888");
		pListItem->Add(pLabItem);
		pParList->Add(pListItem);
	}
}

void CWndFastReply::Notify(TNotifyUI& msg)
{
	if ( msg.sType == L"itemselect" )
	{
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("listReply")));
		::PostMessageW(GetParent(m_hWnd), WM_CHAT_REPLY, pList->GetCurSel(), NULL);
		Close(0);
	}
}