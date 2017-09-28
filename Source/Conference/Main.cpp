/********************************************************************
	created:	2016/07/19
	author:		Carl
	e-mail:		carl.zhang@arkenvmc.com
*********************************************************************/
#pragma once
#include "stdafx.h"
#include "MainFrame.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	// Initiation COM
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	// Initiation of the shadow
	CWndShadow::Initialize(hInstance);

	MainFrame* pWnd = new MainFrame;
	pWnd->Create( NULL, _T("FrameWnd"), UI_WNDSTYLE_DIALOG, 0 );
	pWnd->ShowWindow( false );

	CPaintManagerUI::MessageLoop();

	delete pWnd;

	::CoUninitialize();

	return 0;
}