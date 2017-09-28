#include "StdAfx.h"
#include "DlgPrivateChat.h"
#include "WndFastReply.h"

CDlgPrivateChat::CDlgPrivateChat(void)
{
	m_ptUserList = new tChatUserList;
	m_ptUserList->iNum = 0;
	m_ptUserList->Users = new tChatUser[10];

	m_ptMsgList = new tChatMsgList;
	m_ptMsgList->iMsgNum = 0;
	m_ptMsgList->tMsg = new tChatMsg[1000];
}


CDlgPrivateChat::~CDlgPrivateChat(void)
{
	delete []m_ptUserList->Users;
	m_ptUserList->Users = NULL;
	delete m_ptUserList;
	m_ptUserList = NULL;

	delete []m_ptMsgList->tMsg;
	m_ptMsgList->tMsg = NULL;
	delete m_ptMsgList;
	m_ptMsgList = NULL;
}

LRESULT CDlgPrivateChat::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT CDlgPrivateChat::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_CHAT_REPLY:	   lRes = OnFastReply(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CDlgPrivateChat::InitWindow()
{
	CButtonUI* btnSend = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnSend")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("COMMON"), _T("btnSend"), szText, MAX_PATH);
	btnSend->SetText(szText);
}

void CDlgPrivateChat::AddUser(LPCTSTR szName, LPCTSTR szURL)
{
	for (int i = 0; i < m_ptUserList->iNum; i++)
	{
		if (!wcscmp(szURL, m_ptUserList->Users[i].UserURL))
			return ;
	}
	if (m_ptUserList->iNum < 10)
	{
		memcpy(m_ptUserList->Users[m_ptUserList->iNum].UserName, szName, sizeof(m_ptUserList->Users[m_ptUserList->iNum].UserName));
		memcpy(m_ptUserList->Users[m_ptUserList->iNum].UserURL, szURL, sizeof(m_ptUserList->Users[m_ptUserList->iNum].UserURL));
		m_ptUserList->iNum++;
	}
}

void CDlgPrivateChat::UpdataUI()
{
	TCHAR szBuf[129] = {0};
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labCaption"));
	if (m_ptUserList->iNum == 1)
	{
		labCaption->SetText(m_ptUserList->Users[0].UserName);

		CVerticalLayoutUI* pVerUI = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"verList"));
		pVerUI->SetVisible(false);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 444, 512, SWP_NOMOVE | SWP_SHOWWINDOW);
		RECT rc = {0,0,0,44};
		m_PaintManager.SetCaptionRect(rc);
	}
	else
	{
		CVerticalLayoutUI* pVerUI = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(L"verList"));
		pVerUI->SetVisible();
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 614, 512, SWP_NOMOVE | SWP_SHOWWINDOW);
		RECT rc = {170,0,0,44};
		m_PaintManager.SetCaptionRect(rc);

		CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listChatUser"));
		pParList->RemoveAll();
		for (int i = 0; i < m_ptUserList->iNum; i++)
		{
			CListContainerElementUI* pListItem  = new CListContainerElementUI();
			pListItem->SetFixedHeight(40);
			pListItem->SetManager(&m_PaintManager, NULL, false);
			CLabelUI* pLabItem = new CLabelUI;
			pLabItem->SetText(m_ptUserList->Users[i].UserName);
			pLabItem->SetAttribute(L"textpadding", L"30,0,0,0");
			pListItem->Add(pLabItem);
			pParList->Add(pListItem);
		}
		pParList->SelectItem(m_ptUserList->iNum-1);
	}
	ShowPrivateMsg();
}

void CDlgPrivateChat::ShowUI(bool bShow /* = true */)
{
	if (!bShow)
	{
		
	}
	ShowWindow(bShow);
}

void CDlgPrivateChat::ShowPrivateMsg()
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"richGroupChatView"));
	if( pRichEdit == NULL ) return;
	pRichEdit->SetText(L"");

	CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listChatUser"));
	int iIndex = pParList->GetCurSel();
	if (iIndex < 0) iIndex = 0;

	TCHAR szName[129] = {0};
	TCHAR szMsg[1025] = {0};
	TCHAR szTime[128] = {0};
	for (int i = 0; i < m_ptMsgList->iMsgNum; i++)
	{
		if (!wcscmp(m_ptMsgList->tMsg[i].szName, m_ptUserList->Users[iIndex].UserName))
		{
			if (m_ptMsgList->tMsg[i].bSend)
				ShowMsg(m_ptMsgList->tMsg[i].szMsg, L"ÎÒ", m_ptMsgList->tMsg[i].szTime);
			else
				ShowMsg(m_ptMsgList->tMsg[i].szMsg, m_ptMsgList->tMsg[i].szName, m_ptMsgList->tMsg[i].szTime);
		}
	}
}

void CDlgPrivateChat::ShowMsg(LPCTSTR strMsg, LPCTSTR strSender, LPCTSTR strTime)
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"richGroupChatView"));
	if( pRichEdit == NULL ) return;
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 0);
	cf.dwEffects = 0;

	if (!pRichEdit->GetText().IsEmpty())
	{
		pRichEdit->SetSel(-1, -1);
		pRichEdit->ReplaceSel(_T("\n"), false);
		pRichEdit->SetSel(-1, -1);
		pRichEdit->ReplaceSel(_T("\n"), false);
	}

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	pRichEdit->ReplaceSel(strSender, false);

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	pRichEdit->ReplaceSel(strTime, false);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0xcc, 0xCC, 0xCC);
	cf.dwEffects = 0;
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(strMsg, false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();
}

void CDlgPrivateChat::RevPrivateChat(LPCTSTR strMsg, LPCTSTR strSender, BOOL bSend)
{
	m_ptMsgList->tMsg[m_ptMsgList->iMsgNum].bSend = bSend;
	memcpy(m_ptMsgList->tMsg[m_ptMsgList->iMsgNum].szName, strSender, sizeof(m_ptMsgList->tMsg[m_ptMsgList->iMsgNum].szName));
	memcpy(m_ptMsgList->tMsg[m_ptMsgList->iMsgNum].szMsg, strMsg, sizeof(m_ptMsgList->tMsg[m_ptMsgList->iMsgNum].szMsg));

	SYSTEMTIME time;
	::GetLocalTime(&time);
	CDuiString strTime;
	strTime.Format(L":\t(%u/%u/%u %u:%u:%u)\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	memcpy(m_ptMsgList->tMsg[m_ptMsgList->iMsgNum].szTime, strTime.GetData(), sizeof(m_ptMsgList->tMsg[m_ptMsgList->iMsgNum].szTime));

	m_ptMsgList->iMsgNum++;

	ShowPrivateMsg();
}

LRESULT CDlgPrivateChat::OnFastReply(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDuiString strTemp;
	TCHAR szBuf[MAX_PATH] = {0};
	strTemp.Format(L"element%d", wParam+1);
	project_language_loadstring(_T("CHATREPLY"), strTemp, szBuf, MAX_PATH);

	CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listChatUser"));
	int iIndex = pParList->GetCurSel();
	if (iIndex < 0) iIndex = 0;

	RevPrivateChat(szBuf, m_ptUserList->Users[iIndex].UserName, TRUE);

	VidyoClientInEventPrivateChat privateChat = {0};
	ChangTextToUtf8(szBuf, (char*)&privateChat.message);
	ChangTextToUtf8(m_ptUserList->Users[iIndex].UserURL, (char*)&privateChat.uri);
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_PRIVATE_CHAT, &privateChat, sizeof(privateChat));

	return 0;
}

void CDlgPrivateChat::SendChatMsg()
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"richGroupChatInput"));
	if( pRichEdit == NULL ) return;
	pRichEdit->SetFocus();
	CDuiString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
	if( sText.IsEmpty() ) return;
	pRichEdit->SetText(_T(""));

	CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listChatUser"));
	int iIndex = pParList->GetCurSel();
	if (iIndex < 0) iIndex = 0;

	RevPrivateChat(sText.GetData(), m_ptUserList->Users[iIndex].UserName, TRUE);

	VidyoClientInEventPrivateChat privateChat = {0};
	ChangTextToUtf8(sText.GetData(), (char*)&privateChat.message);
	ChangTextToUtf8(m_ptUserList->Users[iIndex].UserURL, (char*)&privateChat.uri);
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_PRIVATE_CHAT, &privateChat, sizeof(privateChat));
}

void CDlgPrivateChat::ClearUserList()
{
	m_ptUserList->iNum = 0;
	CListUI* pParList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listChatUser"));
	pParList->SelectItem(0);
}

void CDlgPrivateChat::Notify(TNotifyUI& msg)
{
	if ( msg.sType == L"click" )
	{
		if ( msg.pSender->GetName() == L"btnClose" )
		{
			ClearUserList();
			ShowWindow(false);
		}
		else if ( msg.pSender->GetName() == L"btnMin" )
		{
			ShowWindow(false);
		}
		else if ( msg.pSender->GetName() == L"btnSend" )
		{
			CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"richGroupChatInput"));
			CDuiString strInput = pRichEdit->GetText();
			if (strInput.IsEmpty())
			{
				CWndFastReply* pPopWnd = new CWndFastReply();
				pPopWnd->Create(m_hWnd, _T("FastReply"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);

				RECT rc, rcClient;
				::GetClientRect(m_hWnd, &rc);
				::GetClientRect(pPopWnd->GetHWND(), &rcClient);
				POINT pt = msg.ptMouse;
				ClientToScreen(m_hWnd, &pt);
				SetWindowPos(pPopWnd->GetHWND(), NULL, 
					rc.right-rcClient.right+pt.x-msg.ptMouse.x, 
					rc.bottom-rcClient.bottom+pt.y-msg.ptMouse.y-34,
					rcClient.right, 
					rcClient.bottom, NULL);
				pPopWnd->ShowModal();
			}
			else
				SendChatMsg();
		}
	}
	else if ( msg.sType == L"itemselect" )
	{
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(L"listChatUser"));
		int iIndex = pList->GetCurSel();
		if (iIndex < 0) iIndex = 0;
		
		CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"labCaption"));
		pLabel->SetText(m_ptUserList->Users[iIndex].UserName);

		ShowPrivateMsg();
	}
}