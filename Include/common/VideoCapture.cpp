#include "StdAfx.h"
#include "VideoCapture.h"

CVideoCapture::CVideoCapture(void)
{
	m_hWnd = NULL;
	pBuilder = NULL;
	pmVideo = NULL;
	pFg = NULL;
	pVCap = NULL;
	pVC = NULL;
	fPreviewGraphBuilt = FALSE;
	fPreviewFaked = FALSE;
	fPreviewing = FALSE;
}

CVideoCapture::~CVideoCapture(void)
{
}

void CVideoCapture::OpenCamera(HWND hWnd, TCHAR* tchName, int iLeft, int iTop, int iWidth, int iHeight)
{
	m_hWnd = hWnd;

	if (fPreviewing) StopPreview();

	if(fPreviewGraphBuilt) TearDownGraph();

	FreeCapFilters();
	InitCapFilters(tchName);
	BuildPreviewGraph(iLeft, iTop, iWidth, iHeight);
	StartPreview();
}

void CVideoCapture::CloseCamera()
{
	StopPreview();
	TearDownGraph();
}

void CVideoCapture::TearDownGraph()
{
	SAFE_RELEASE(pME);

	if(pVW)
	{
		// stop drawing in our window, or we may get wierd repaint effects
		pVW->put_Owner(NULL);
		pVW->put_Visible(OAFALSE);
		pVW->Release();
		pVW = NULL;
	}

	if(pVCap)
		SAFE_RELEASE(pBuilder);

	fPreviewGraphBuilt = FALSE;
	fPreviewFaked = FALSE;
}

// build the preview graph!
BOOL CVideoCapture::BuildPreviewGraph(int iLeft, int iTop, int iWidth, int iHeight)
{
	HRESULT hr;

	// We don't have the necessary capture filters
	if(pVCap == NULL) return FALSE;

	// Render the preview pin - even if there is not preview pin, the capture
	// graph builder will use a smart tee filter and provide a preview.
	hr = pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Interleaved, pVCap, NULL, NULL);
	if(hr == VFW_S_NOPREVIEWPIN)
	{
		// preview was faked up for us using the (only) capture pin
		fPreviewFaked = TRUE;
	}
	else if(hr != S_OK)
	{
		// maybe it's DV?
		hr = pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pVCap, NULL, NULL);
		if(hr == VFW_S_NOPREVIEWPIN)
		{
			// preview was faked up for us using the (only) capture pin
			fPreviewFaked = TRUE;
		}
		else if(hr != S_OK)
		{
			fPreviewGraphBuilt = FALSE;
			return FALSE;
		}
	}

	// Get the preview window to be a child of our app's window
	hr = pFg->QueryInterface(IID_IVideoWindow, (void **)&pVW);
	if(hr == NOERROR)
	{
		pVW->put_Owner((OAHWND)m_hWnd);    // We own the window now
		pVW->put_WindowStyle(WS_CHILD);    // you are now a child

		// give the preview window all our space but where the status bar is
		pVW->SetWindowPosition(iLeft, iTop, iWidth, iHeight);
		pVW->put_Visible(OATRUE);
	}

	// make sure we process events while we're previewing!
	hr = pFg->QueryInterface(IID_IMediaEventEx, (void **)&pME);
	if(hr == NOERROR)
	{
		pME->SetNotifyWindow((OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0);
	}

	// All done.
	fPreviewGraphBuilt = TRUE;
	return TRUE;
}

// create the capture filters of the graph.  We need to keep them loaded from
// the beginning, so we can set parameters on them and have them remembered
//
BOOL CVideoCapture::InitCapFilters(TCHAR* tchName)
{
	HRESULT hr = S_OK;

	// make builder
	hr = CoCreateInstance((REFCLSID)CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, (REFIID)IID_ICaptureGraphBuilder2, (LPVOID*)&pBuilder);
	if(hr != S_OK) return FALSE;

	// First, we need a Video Capture filter, and some interfaces
	pVCap = NULL;
	FindVideoDevice(tchName, &pVCap);

	if(pVCap == NULL)
	{
		goto InitCapFiltersFail;
	}

	// make a filter graph, give it to the graph builder and put the video capture filter in the graph
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pFg);
	if(hr != S_OK)
	{
		goto InitCapFiltersFail;
	}

	hr = pBuilder->SetFiltergraph(pFg);
	if(hr != S_OK)
	{
		goto InitCapFiltersFail;
	}

	// Add the video capture filter to the graph with its friendly name
	hr = pFg->AddFilter(pVCap, tchName);
	if(hr != S_OK)
	{
		goto InitCapFiltersFail;
	}

	return TRUE;

InitCapFiltersFail:
	FreeCapFilters();
	return FALSE;
}

// all done with the capture filters and the graph builder
//
void CVideoCapture::FreeCapFilters()
{
	SAFE_RELEASE(pFg);
	if( pBuilder )
	{
		delete pBuilder;
		pBuilder = NULL;
	}
	SAFE_RELEASE(pVCap);
	SAFE_RELEASE(pVC);
}

// Start previewing
//
BOOL CVideoCapture::StartPreview()
{
	// way ahead of you
	if(fPreviewing)
		return TRUE;

	if(!fPreviewGraphBuilt)
		return FALSE;

	// run the graph
	IMediaControl *pMC = NULL;
	HRESULT hr = pFg->QueryInterface(IID_IMediaControl, (void **)&pMC);
	if(SUCCEEDED(hr))
	{
		hr = pMC->Run();
		if(FAILED(hr))
		{
			pMC->Stop();
		}
		pMC->Release();
	}
	if(FAILED(hr)) return FALSE;

	fPreviewing = TRUE;
	return TRUE;
}

BOOL CVideoCapture::StopPreview()
{
	// stop the graph
	IMediaControl *pMC = NULL;
	HRESULT hr = pFg->QueryInterface(IID_IMediaControl, (void **)&pMC);
	if(SUCCEEDED(hr))
	{
		hr = pMC->Stop();
		pMC->Release();
	}
	if(FAILED(hr)) return FALSE;

	fPreviewing = FALSE;
	return TRUE;
}

BOOL CVideoCapture::FindVideoDevice(TCHAR* deviceName, IBaseFilter ** ppSrcFilter)
{
	HRESULT hr;

	IBaseFilter * pSrc = NULL;
	IMoniker* pMoniker =NULL;
	ICreateDevEnum *pDevEnum =NULL;
	IEnumMoniker *pClassEnum = NULL;

	if (!ppSrcFilter)
	{
		return FALSE;
	}

	// Create the system device enumerator
	hr = CoCreateInstance (CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **) &pDevEnum);
	if (FAILED(hr)) return FALSE;

	// Create an enumerator for the video capture devices
	hr = pDevEnum->CreateClassEnumerator (CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
	if (FAILED(hr)) return FALSE;

	// If there are no enumerators for the requested type, then 
	// CreateClassEnumerator will succeed, but pClassEnum will be NULL.
	if (pClassEnum == NULL) return FALSE;

	// Use the first video capture device on the device list.
	// Note that if the Next() call succeeds but there are no monikers,
	// it will return S_FALSE (which is not a failure).  Therefore, we
	// check that the return code is S_OK instead of using SUCCEEDED() macro.
	bool bFind = FALSE;
	if (SUCCEEDED(hr))
	{
		while (hr = pClassEnum->Next(1, &pMoniker, NULL), S_OK == hr)
		{
			IPropertyBag* pBag = NULL;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (LPVOID*)&pBag);
			if (SUCCEEDED(hr))
			{
				VARIANT var;
				var.vt = VT_BSTR;
				hr = pBag->Read(L"FriendlyName", &var, NULL);
				if(hr == NOERROR && !lstrcmpW(var.bstrVal, deviceName))
				{
					bFind = TRUE;
					hr = pMoniker->BindToObject(0,0,IID_IBaseFilter, (LPVOID*)&pSrc);
					pBag->Release();
					break;
				}
				pBag->Release();
			}
		}
	}

	// Copy the found filter pointer to the output parameter.
	if (SUCCEEDED(hr) && bFind)
	{
		*ppSrcFilter = pSrc;
		(*ppSrcFilter)->AddRef();
	}

	SAFE_RELEASE(pSrc);
	SAFE_RELEASE(pMoniker);
	SAFE_RELEASE(pDevEnum);
	SAFE_RELEASE(pClassEnum);

	return TRUE;
}