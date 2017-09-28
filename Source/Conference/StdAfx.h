#pragma once

#pragma warning(disable: 4995) 

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>
#include "../../Include/DuiLib/UIlib.h"

#include "../../include/VidyoClient/VidyoClient.h"
#include "../../include/gsoap/soapH.h"
#include "../../include/gsoap/VidyoPortalUserServiceBinding.nsmap"

using namespace DuiLib;

#ifdef _DEBUG
#       pragma comment(lib, "../../lib/DuiLib_d.lib")
#       pragma comment(lib, "../../lib/VidyoClientDll.lib")
#else
#       pragma comment(lib, "../../lib/DuiLib.lib")
#		pragma comment(lib, "../../lib/VidyoClientDll.lib")
#endif