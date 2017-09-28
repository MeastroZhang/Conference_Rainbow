#include "StdAfx.h"
#include "DlgRoomSetting.h"
#include "WSgSoap.h"
#include "../../include/Util/Util.h"

CDlgRoomSetting::CDlgRoomSetting(LPVOID* pRoom)
{
	m_ptRoomData = (VidyoClientAccountData*)pRoom;
}


CDlgRoomSetting::~CDlgRoomSetting(void)
{
}

LRESULT CDlgRoomSetting::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDlgRoomSetting::InitWindow()
{
	COptionUI* pOptRoomVideo = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optRoomVideo")));
	COptionUI* pOptRoomAudio = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optRoomAudio")));
	CLabelUI* pLabRoomPin = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labRoomPin")));
	CLabelUI* pLabControlPin = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labControlPin")));
	CLabelUI* pLabRoomLink = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labRoomLink")));
	CButtonUI* pbtnCopy	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnCopy")));
	CButtonUI* pbtnChange	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnChange")));
	CButtonUI* pbtnCancel	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnCancel")));
	CButtonUI* pbtnOK	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnOK")));
	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("ROOMSETTING"), _T("optRoomVideo"), szText, MAX_PATH);
	pOptRoomVideo->SetText(szText);
	project_language_loadstring(_T("ROOMSETTING"), _T("optRoomAudio"), szText, MAX_PATH);
	pOptRoomAudio->SetText(szText);
	project_language_loadstring(_T("ROOMSETTING"), _T("labRoomPin"), szText, MAX_PATH);
	pLabRoomPin->SetText(szText);
	project_language_loadstring(_T("ROOMSETTING"), _T("labControlPin"), szText, MAX_PATH);
	pLabControlPin->SetText(szText);
	project_language_loadstring(_T("ROOMSETTING"), _T("labRoomLink"), szText, MAX_PATH);
	pLabRoomLink->SetText(szText);
	project_language_loadstring(_T("ROOMSETTING"), _T("btnCopy"), szText, MAX_PATH);
	pbtnCopy->SetText(szText);
	project_language_loadstring(_T("ROOMSETTING"), _T("btnChange"), szText, MAX_PATH);
	pbtnChange->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnCancel"), szText, MAX_PATH);
	pbtnCancel->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnOK"), szText, MAX_PATH);
	pbtnOK->SetText(szText);
}

void CDlgRoomSetting::UpdataUI()
{
	char szRoomProfile[128] = {0};
	if (SoapGetRoomProfile(m_ptRoomData->entityID, szRoomProfile))
	{
		if (strlen(szRoomProfile))
		{
			TCHAR tczProfile[128];
			ChangUtf8ToText(szRoomProfile, tczProfile);
			CDuiString strProfile;
			strProfile.Format(_T("%s"), tczProfile);
			if (!strProfile.Compare(_T("AudioOnly")))
			{
				COptionUI* pOptRoomVideo = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optRoomVideo")));
				pOptRoomVideo->Selected(true);
			}
			else if (!strProfile.Compare(_T("VideoOnly")))
			{
				COptionUI* pOptRoomAudio = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optRoomAudio")));
				pOptRoomAudio->Selected(true);
			}
			else if (!strProfile.Compare(_T("NoAudioAndVideo")))
			{
				COptionUI* pOptRoomVideo = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optRoomVideo")));
				pOptRoomVideo->Selected(true);
				COptionUI* pOptRoomAudio = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optRoomAudio")));
				pOptRoomAudio->Selected(true);
			}
		}
	}

	TCHAR szBuf[MAX_PATH] = {0};
	CEditUI* pEditRoomPin = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editRoomPin")));
	ChangUtf8ToText(m_ptRoomData->roomMode.roomPIN, szBuf);
	pEditRoomPin->SetText(szBuf);
	CEditUI* pEditControlPin = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editControlPin")));
	ChangUtf8ToText(m_ptRoomData->roomMode.moderatorPIN, szBuf);
	pEditControlPin->SetText(szBuf);
	CEditUI* pEditRoomLink = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editRoomLink")));
	ChangUtf8ToText(m_ptRoomData->roomMode.roomURL, szBuf);
	CDuiString strRoomURL;
	strRoomURL.Format(_T("%s"), szBuf);
	int iPos = strRoomURL.Find(_T("key="));
	pEditRoomLink->SetText(strRoomURL.Mid(iPos+4));
}

void CDlgRoomSetting::Notify(TNotifyUI& msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if ( msg.sType == _T("windowinit") )
	{
		UpdataUI();
	}
	else if( msg.sType == _T("click") ) 
	{
		if( strControlName == _T("btnClose") ) 
		{
			Close(0);
		}
		else if ( strControlName == _T("btnCopy") )
		{
			if (OpenClipboard(m_hWnd))
			{
				HGLOBAL clipbuffer;
				char * buffer;
				EmptyClipboard();
				clipbuffer = GlobalAlloc(GMEM_DDESHARE, sizeof(m_ptRoomData->roomMode.roomURL));
				buffer = (char*)GlobalLock(clipbuffer);
				strcpy(buffer, m_ptRoomData->roomMode.roomURL);
				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();
			}
		}
		else if ( strControlName == _T("btnChange") )
		{
			if (SoapRemoveRoomURL(m_ptRoomData->entityID))
			{
				if (SoapCreateRoomURL(m_ptRoomData->entityID))
				{
					char szRoomURL[128] = {0};
					SoapGetRoomAccessOptions(m_ptRoomData->entityID, szRoomURL);
					memcpy(m_ptRoomData->roomMode.roomURL, szRoomURL, sizeof(szRoomURL));

					UpdataUI();
				}
			}
		}
		else if ( strControlName == _T("btnCancel") )
		{
			Close(0);
		}
		else if ( strControlName == _T("btnOK") )
		{
			COptionUI* pOptRoomVideo = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optRoomVideo")));
			COptionUI* pOptRoomAudio = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("optRoomAudio")));
			char szProfile[64] = {0};
			if (pOptRoomVideo->IsSelected())
			{
				if (pOptRoomAudio->IsSelected())
					memcpy(szProfile, "NoAudioAndVideo", strlen("NoAudioAndVideo"));
				else
					memcpy(szProfile, "AudioOnly", strlen("NoAudioAndVideo"));
			}
			else
			{
				if (pOptRoomAudio->IsSelected())
					memcpy(szProfile, "VideoOnly", strlen("NoAudioAndVideo"));
			}

			if (strlen(szProfile) == 0)
				SoapRemoveRoomProfile(m_ptRoomData->entityID);
			else
				SoapSetRoomProfile(m_ptRoomData->entityID, szProfile);

			CEditUI* pEditRoomPin = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editRoomPin")));
			CDuiString strRoomPin = pEditRoomPin->GetText();
			char szRoomPin[17] = {0};
			if (strRoomPin.GetLength())
			{
				ChangTextToUtf8(strRoomPin.GetData(), szRoomPin);
				if (SoapCreateRoomPin(m_ptRoomData->entityID, szRoomPin))
				{
					m_ptRoomData->roomMode.hasPIN = true;
					memcpy(m_ptRoomData->roomMode.roomPIN, szRoomPin, strlen(szRoomPin));
				}
			}
			else
			{
				if (SoapRemoveRoomPin(m_ptRoomData->entityID))
				{
					m_ptRoomData->roomMode.hasPIN = false;
					memset(m_ptRoomData->roomMode.roomPIN, 0, sizeof(m_ptRoomData->roomMode.roomPIN));
				}
			}

			CEditUI* pEditControlPin = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editControlPin")));
			CDuiString strControlPin = pEditControlPin->GetText();
			char szModeratorPin[17] = {0};
			if (strControlPin.GetLength())
			{
				ChangTextToUtf8(strControlPin.GetData(), szModeratorPin);
				if (SoapCreateModeratorPin(m_ptRoomData->entityID, szModeratorPin))
				{
					m_ptRoomData->roomMode.hasModeratorPIN = true;
					memcpy(m_ptRoomData->roomMode.moderatorPIN, szModeratorPin, strlen(szModeratorPin));
				}
			}
			else
			{
				if (SoapRemoveModeratorPin(m_ptRoomData->entityID))
				{
					m_ptRoomData->roomMode.hasModeratorPIN = false;
					memset(m_ptRoomData->roomMode.moderatorPIN, 0, sizeof(m_ptRoomData->roomMode.moderatorPIN));
				}
			}
		}
	}
}