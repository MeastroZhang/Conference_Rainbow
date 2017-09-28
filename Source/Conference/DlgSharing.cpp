#include "StdAfx.h"
#include "DlgSharing.h"
#include "../../include/Util/Util.h"

static VidyoClientRequestGetWindowsAndDesktops getWindwosAndDesktops = {0};
static VidyoClientRequestWindowShares WindowShares;
CDlgSharing::CDlgSharing(LPVOID pParent)
{
	m_pDlgConference = (CDlgConference*)pParent;
}


CDlgSharing::~CDlgSharing(void)
{
}

LRESULT CDlgSharing::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDlgSharing::InitWindow()
{
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labCaption")));
	CLabelUI* labShared = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labShared")));
	CLabelUI* labBeShare = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labBeShare")));
	CButtonUI* btnStartShare	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnStartShare")));
	CButtonUI* btnStopShareView	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnStopShareView")));
	CButtonUI* btnStopShare	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnStopShare")));
	CButtonUI* btnReShare	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnReShare")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SHARE"), _T("labCaption"), szText, MAX_PATH);
	labCaption->SetText(szText);
	project_language_loadstring(_T("SHARE"), _T("labShared"), szText, MAX_PATH);
	labShared->SetText(szText);
	project_language_loadstring(_T("SHARE"), _T("labBeShare"), szText, MAX_PATH);
	labBeShare->SetText(szText);
	project_language_loadstring(_T("SHARE"), _T("btnStopShareView"), szText, MAX_PATH);
	btnStopShareView->SetText(szText);
	project_language_loadstring(_T("SHARE"), _T("btnStartShare"), szText, MAX_PATH);
	btnStartShare->SetText(szText);
	project_language_loadstring(_T("SHARE"), _T("btnStopShare"), szText, MAX_PATH);
	btnStopShare->SetText(szText);
	project_language_loadstring(_T("SHARE"), _T("btnReShare"), szText, MAX_PATH);
	btnReShare->SetText(szText);
}

void CDlgSharing::UpdataUI()
{
	CButtonUI* btnStartShare	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnStartShare")));
	CButtonUI* btnStopShare	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnStopShare")));
	CButtonUI* btnReShare	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnReShare")));
	if (m_pDlgConference->GetShareStatus())
	{
		btnStopShare->SetVisible();
		btnStartShare->SetVisible(false);
		btnReShare->SetVisible();
	}
	else
	{
		btnStopShare->SetVisible(false);
		btnStartShare->SetVisible();
		btnReShare->SetVisible(false);
	}

	TCHAR szBuf[MAX_PATH] = {0};

	if (VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_WINDOW_SHARES, &WindowShares, sizeof(WindowShares)) == VIDYO_FALSE)
	{
		if (WindowShares.shareList.numApp)
		{
			CHorizontalLayoutUI* pHorTitleShared = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("horTitleShared")));
			CHorizontalLayoutUI* pHorLineShared1 = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("horLineShared1")));
			CHorizontalLayoutUI* pHorTileShared = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("horTileShared")));
			CHorizontalLayoutUI* pHorLineShared2 = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("horLineShared2")));
			pHorTileShared->SetVisible();
			pHorLineShared1->SetVisible();
			pHorTitleShared->SetVisible();
			pHorLineShared2->SetVisible();

			CTileLayoutUI* ptileShared = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("tileShared")));
			RECT rc = {40,20,50,10};
			ptileShared->SetPadding(rc);
			SIZE sz = {180,140};
			ptileShared->SetItemSize(sz);
			ptileShared->RemoveAll();
			ptileShared->SetChildPadding(30);
			ptileShared->SetChildVPadding(30);

			CDuiString strTemp;
			TCHAR szText[MAX_PATH];
			project_language_loadstring(_T("SHARE"), _T("strRemoteShare"), szText, MAX_PATH);
			for (int i = 0; i < WindowShares.shareList.numApp; i++)
			{
				CDialogBuilder builder;
				CContainerUI* pRoot = static_cast<CContainerUI*>(builder.Create(_T ("\\DlgSharing\\ShareItem.xml"), (UINT)0, NULL));
				pRoot->SetTag(i);

				CLabelUI* labShareName = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labShareName")));
				ChangUtf8ToText(WindowShares.shareList.remoteAppName[i+1], szBuf);
				strTemp.Format(_T("%s%s"), szBuf, szText);
				labShareName->SetText(strTemp);

				CButtonUI* btnPicture = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnPicture")));
				btnPicture->SetBkImage(L"\\DlgSharing\\ShareViewItem.png");

				ptileShared->Add(pRoot);
			}
		}
	}

	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_WINDOWS_AND_DESKTOPS, &getWindwosAndDesktops, sizeof(getWindwosAndDesktops));

	CTileLayoutUI* pTileBeShare = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("tileBeShare")));

	RECT rc = {40,20,50,10};
	pTileBeShare->SetPadding(rc);
	SIZE sz = {180,140};
	pTileBeShare->SetItemSize(sz);
	
	pTileBeShare->RemoveAll();
	pTileBeShare->SetChildPadding(30);
	pTileBeShare->SetChildVPadding(30);

	CDuiString strJPGName;
	int iNum = getWindwosAndDesktops.numSystemDesktops;
	int i = 0;
	for ( ; i < iNum; i++ )
	{
		CDialogBuilder builder;
		CContainerUI* pRoot = static_cast<CContainerUI*>(builder.Create(_T ("\\DlgSharing\\ShareItem.xml"), (UINT)0, NULL));
		pRoot->SetTag(i);

		CLabelUI* labShareName = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labShareName")));
		//ChangUtf8ToText(getWindwosAndDesktops.sysDesktopName[i], szBuf);
		CDuiString strScreen;
		strScreen.Format(L"¡¾ÆÁÄ»¹²Ïí¡¿(%d*%d)", getWindwosAndDesktops.sysDesktopRect[i].width, getWindwosAndDesktops.sysDesktopRect[i].height);
		labShareName->SetText(strScreen);

		pTileBeShare->Add(pRoot);
	}

	iNum = getWindwosAndDesktops.numApplicationWindows;
	for( ; i < iNum; i++ )
	{
		HWND hParent = GetParent(m_hWnd);
		if (getWindwosAndDesktops.appWindowId[i] == hParent)
		{
			continue;
		}

		CDialogBuilder builder;
		CContainerUI* pRoot = static_cast<CContainerUI*>(builder.Create(_T ("\\DlgSharing\\ShareItem.xml"), (UINT)0, NULL));
		pRoot->SetTag(i);

		CLabelUI* labShareName = static_cast<CLabelUI*>(pRoot->FindSubControl(_T("labShareName")));
		ChangUtf8ToText(getWindwosAndDesktops.appWindowAppName[i], szBuf);
		labShareName->SetText(szBuf);

		strJPGName.Format(_T("%s\\%d.bmp"), PATH_THUMB, i+1);
		char szJPGName[128];
		ChangTextToUtf8(strJPGName.GetData(), szJPGName);

		CButtonUI* btnPicture = static_cast<CButtonUI*>(pRoot->FindSubControl(_T("btnPicture")));
		SaveWindowPrint(getWindwosAndDesktops.appWindowId[i], szJPGName);
		btnPicture->SetBkImage(strJPGName);

		//SHFILEINFO sfi; 
		//HIMAGELIST hil;

		//ZeroMemory(&sfi,sizeof(sfi)); 
		//hil = (HIMAGELIST)SHGetFileInfo(_T(".zip"), 
		//	FILE_ATTRIBUTE_NORMAL, 
		//	&sfi, 
		//	sizeof(sfi), 
		//	SHGFI_USEFILEATTRIBUTES|SHGFI_ICON);

		pTileBeShare->Add(pRoot);
	}
}

void CDlgSharing::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") )
	{
		UpdataUI();
	}
	else if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			Close(0);
		}
		else if ( msg.pSender->GetName() == L"btnStartShare" || msg.pSender->GetName() == L"btnReShare" )
		{
			unsigned int iIndex = 10000;
			CTileLayoutUI* pTileBeShare = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("tileBeShare")));
			for (int i = 0; i < pTileBeShare->GetCount(); i++)
			{
				CContainerUI* pRoot = (CContainerUI*)pTileBeShare->GetItemAt(i);
				CDuiString strFlag = pRoot->GetText();
				if (!strFlag.Compare(L"selected"))
					iIndex = pRoot->GetTag();
			}

			if (iIndex < 10000)
			{
				bool bShareScreen = false;
				VidyoClientInEventShare inEventShare;
				if (iIndex < getWindwosAndDesktops.numSystemDesktops)
				{
					inEventShare.shareType = VIDYO_CLIENT_CONTENTS_SHARE_TYPE_DESKTOP_WINDOW;
					memcpy(inEventShare.desktopId, getWindwosAndDesktops.sysDesktopId[iIndex], sizeof(inEventShare.desktopId));
					bShareScreen = true;
				} 
				else
				{
					inEventShare.shareType = VIDYO_CLIENT_CONTENTS_SHARE_TYPE_APPLICATION_WINDOW;
					inEventShare.window = getWindwosAndDesktops.appWindowId[iIndex];
					inEventShare.windowSize = getWindwosAndDesktops.appWindowRect[iIndex];
				}
				
				if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_SHARE, &inEventShare, sizeof(inEventShare)))
				{
					::SetForegroundWindow(inEventShare.window);
					m_pDlgConference->SetShareStatus(true);
					if (bShareScreen) m_pDlgConference->StartShareScreen();
					Close(0);
				}
			}
		}
		else if ( msg.pSender->GetName() == L"btnStopShare" )
		{
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_UNSHARE, NULL, NULL);
			m_pDlgConference->SetShareStatus(false);
			Close(0);
		}
		else if ( msg.pSender->GetName() == L"btnStopShareView" )
		{
			WindowShares.requestType = CHANGE_SHARING_WINDOW;
			WindowShares.shareList.newApp = 0;
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_WINDOW_SHARES, &WindowShares, sizeof(WindowShares));
			Close(0);
		}
		else if ( msg.pSender->GetName() == L"btnPicture" )
		{
			CContainerUI* pRoot = (CContainerUI*)((CButtonUI*)msg.pSender->GetParent());

			if (pRoot->GetParent()->GetName() == L"tileBeShare")
			{
				CTileLayoutUI* pTileBeShare = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("tileBeShare")));
				for (int i = 0; i < pTileBeShare->GetCount(); i++)
				{
					CContainerUI* pRoot = (CContainerUI*)pTileBeShare->GetItemAt(i);
					pRoot->SetBkImage(L"\\common\\bgShareItem.png");
					pRoot->SetText(L"");
				}
			}
			else
			{
				CTileLayoutUI* pTileShared = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("tileShared")));
				for (int i = 0; i < pTileShared->GetCount(); i++)
				{
					CContainerUI* pRoot = (CContainerUI*)pTileShared->GetItemAt(i);
					pRoot->SetBkImage(L"\\common\\bgShareItem.png");
					pRoot->SetText(L"");
				}

				WindowShares.requestType = CHANGE_SHARING_WINDOW;
				WindowShares.shareList.newApp = pRoot->GetTag()+1;
				VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_WINDOW_SHARES, &WindowShares, sizeof(WindowShares));
			}

			pRoot->SetBkImage(L"\\common\\bgShareItemSelect.png");
			pRoot->SetText(L"selected");
		}
	}
	else if ( msg.sType == _T("mouseenter") )
	{
		if ( msg.pSender->GetName() == _T("tileBeShare") )
		{
			CTileLayoutUI* pTileBeShare = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(_T("tileBeShare")));
			for (int i = 0; i < pTileBeShare->GetCount(); i++)
			{
				CControlUI* pControl = pTileBeShare->GetItemAt(i);
				if (pControl)
				{
					CContainerUI* pRoot = (CContainerUI*)pControl;
					int iIndex = pRoot->GetTag();

					RECT rc = pControl->GetClientPos();
					if (::PtInRect(&rc, msg.ptMouse))
					{
						CDuiString strSelected = pRoot->GetText();
						if (strSelected.Compare(L"selected"))
						{
							pRoot->SetBkImage(L"\\common\\bgShareItemHot.png");
						}
					}
					else
					{
						CDuiString strSelected = pRoot->GetText();
						if (strSelected.Compare(L"selected"))
						{
							pRoot->SetBkImage(L"\\common\\bgShareItem.png");
						}
					}
				}
			}
		}
	}
}