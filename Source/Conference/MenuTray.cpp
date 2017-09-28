#include "StdAfx.h"
#include "MenuTray.h"


CMenuTray::CMenuTray(void)
{
}


CMenuTray::~CMenuTray(void)
{
}

void CMenuTray::InitWindow()
{

}

LRESULT CMenuTray::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return TRUE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CMenuTray::Notify(TNotifyUI& msg)
{
}