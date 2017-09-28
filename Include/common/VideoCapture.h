#pragma once

#include <windows.h>
#include <dshow.h>

// Macros
#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

// window messages
#define WM_GRAPHNOTIFY  WM_APP+1

class CVideoCapture
{
public:
	CVideoCapture(void);
	~CVideoCapture(void);

	void OpenCamera(HWND hWnd, TCHAR* tchName, int iLeft, int iTop, int iWidth, int iHeight);
	void CloseCamera();
	BOOL StopPreview();
	BOOL StartPreview();

	void TearDownGraph(void);
	BOOL BuildPreviewGraph(int iLeft, int iTop, int iWidth, int iHeight);
	BOOL InitCapFilters(TCHAR* tchName);
	void FreeCapFilters();
	BOOL FindVideoDevice(TCHAR* deviceName, IBaseFilter ** ppSrcFilter);

	BOOL fPreviewing;
private:
	HWND m_hWnd;

	ICaptureGraphBuilder2*	pBuilder;
	IAMVideoCompression* pVC;
	IVideoWindow*	pVW;
	IMediaEventEx*	pME;
	IBaseFilter*	pVCap;
	IGraphBuilder*	pFg;
	IMoniker*		pmVideo;

	BOOL fCCAvail;
	BOOL fPreviewGraphBuilt;
	BOOL fPreviewFaked;
};

