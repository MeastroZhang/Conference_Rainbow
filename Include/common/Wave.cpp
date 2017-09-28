#include "StdAfx.h"
#include "Wave.h"

CWaveIn::CWaveIn(void)
{
	m_bIsOpen = false;
	m_dbVolume = 0;

	bTerminateFlage = FALSE;
	m_SampledTimesIndex = 0;

	wfx.nSamplesPerSec = 44100; /* sample rate */
	wfx.wBitsPerSample = 16; /* sample size */
	wfx.nChannels= 2; /* channels*/
	wfx.cbSize = 0; /* size of _extra_ info */
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
}


CWaveIn::~CWaveIn(void)
{
}

HRESULT CWaveIn::OpenMic(UINT deviceID)
{
	m_SampledTimesIndex = 0;

	HRESULT hr;
	hr = waveInGetNumDevs(); 
	if (hr == 0)
		return hr;

	// test for Mic available     
	hr = waveInGetDevCaps (0, &m_WaveInDevCaps, sizeof(WAVEINCAPS)); 
	if ( hr != MMSYSERR_NOERROR)  
		return hr;

	// Open Input    
	hr = waveInOpen( &m_WaveIn, deviceID, &wfx, (DWORD)waveInProc, (DWORD)this, CALLBACK_FUNCTION);  
	if ( hr != MMSYSERR_NOERROR)  
		return hr;

	for(int i = 0; i < NUM_LEVEL_BUFFERS; i++)  
	{  
		_HWWAVEDATA_STRUCT *pWaveStruct = &m_HWWaveStruct[i];  

		pWaveStruct->m_WaveHeader.lpData = (CHAR *)&pWaveStruct->InputBuffer[0];  
		pWaveStruct->m_WaveHeader.dwBufferLength = MAX_SAMPLES *2;  
		pWaveStruct->m_WaveHeader.dwFlags = 0;  
		hr = waveInPrepareHeader( m_WaveIn, &pWaveStruct->m_WaveHeader, sizeof(WAVEHDR) );   
		if ( (hr!= MMSYSERR_NOERROR) || ( pWaveStruct->m_WaveHeader.dwFlags != WHDR_PREPARED) )  
			return hr;

		hr = waveInAddBuffer( m_WaveIn, &pWaveStruct->m_WaveHeader, sizeof(WAVEHDR) );  
		if  (hr!= MMSYSERR_NOERROR)   
			return hr;  
	}  

	bTerminateFlage = false;  
	hr = waveInStart( m_WaveIn );  
	if  (hr!= MMSYSERR_NOERROR)   
		return hr;

	m_bIsOpen = true;

	return hr;
}

void CWaveIn::CloseMic()
{
	bTerminateFlage = TRUE;  
	if (m_WaveIn)   
	{  
		waveInStop(m_WaveIn);  
		waveInReset(m_WaveIn);
		waveInClose(m_WaveIn);  
		m_WaveIn = NULL;  
	}
	m_bIsOpen = false;
}

bool CWaveIn::IsOpen()
{
	return m_bIsOpen;
}

double CWaveIn::GetVolume()
{
	return m_dbVolume;
}

void CWaveIn::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CWaveIn *pSoundIn = (CWaveIn*)dwInstance;
	if (pSoundIn->bTerminateFlage)
		return ;

	switch(uMsg)
	{
	case WIM_DATA:
		{
			MMRESULT result;  
			WAVEHDR *pWaveHeader = (WAVEHDR *)dwParam1;

			// put the buffer back on the queue   
			result = waveInAddBuffer( hwi, pWaveHeader, sizeof(WAVEHDR) );  
			if  (result!= MMSYSERR_NOERROR)   
				break;

			// calculate volume
			SHORT *pSrc = (SHORT*)pWaveHeader->lpData;
			double dbVal = pSrc[0];
			double dbResule = fabs(dbVal);
			for(unsigned int i = 0; i < pWaveHeader->dwBufferLength/2; i++)
			{
				dbVal = pSrc[i];
				pSoundIn->m_dbVolume = max(fabs(dbVal), dbResule);
			}
		}
		break;
	default:
		break;
	}
}


// Class CWaveOut
static CRITICAL_SECTION waveCriticalSection;
static WAVEHDR* waveBlocks;
static volatile int waveFreeBlockCount = BLOCK_COUNT;
static int waveCurrentBlock = 0;

CWaveOut::CWaveOut(HWND hParent)
{
	wfx.nSamplesPerSec = 44100; /* sample rate */
	wfx.wBitsPerSample = 16; /* sample size */
	wfx.nChannels= 2; /* channels*/
	wfx.cbSize = 0; /* size of _extra_ info */
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

	m_dbVolume = 0;

	m_hParent = hParent;
}

CWaveOut::~CWaveOut()
{

}

HRESULT CWaveOut::OpenSpeaker(CDuiString strFileName)
{
	InitializeCriticalSection(&waveCriticalSection);

	char buffer[1024]; /* intermediate buffer for reading */
	waveBlocks = allocateBlocks(BLOCK_SIZE, 20);
	
	HANDLE hFile;
	if((hFile = CreateFile(strFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL)) == INVALID_HANDLE_VALUE)
		return 0;

	// open speaker    
	HRESULT hr;
	hr = waveOutOpen( &m_WaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, (DWORD_PTR)&waveFreeBlockCount, CALLBACK_FUNCTION);  
	if ( hr != MMSYSERR_NOERROR)  
		return hr;

	// playback loop
	while(1) {
		DWORD readBytes;
		if(!ReadFile(hFile, buffer, sizeof(buffer), &readBytes, NULL))
			break;
		if(readBytes == 0)
			break;
		if(readBytes < sizeof(buffer))
			memset(buffer + readBytes, 0, sizeof(buffer) - readBytes);
		writeAudio(m_WaveOut, buffer, sizeof(buffer));
	}

	// wait for all blocks to complete
	while(waveFreeBlockCount < 20)
		Sleep(10);

	// unprepare any blocks that are still prepared
	for(int i = 0; i < waveFreeBlockCount; i++)
	{
		if(waveBlocks[i].dwFlags &WHDR_PREPARED)
			waveOutUnprepareHeader(m_WaveOut,&waveBlocks[i], sizeof(WAVEHDR));

		DeleteCriticalSection(&waveCriticalSection);
		freeBlocks(waveBlocks);
		waveOutClose(m_WaveOut);
		CloseHandle(hFile);
		return 0;
	}

	return hr;
}

double CWaveOut::GetVolume()
{
	return m_dbVolume;
}

void CWaveOut::writeAudio(HWAVEOUT hWaveOut,LPSTR data, int size)
{
	WAVEHDR* current;
	int remain;
	current = &waveBlocks[waveCurrentBlock];
	while(size > 0) {
		// first make sure the header we're going to use is unprepared
		if(current->dwFlags & WHDR_PREPARED)
			waveOutUnprepareHeader(hWaveOut,current, sizeof(WAVEHDR));

		if(size < (int)(BLOCK_SIZE -current->dwUser)) {
			memcpy(current->lpData +current->dwUser, data, size);
			current->dwUser += size;
			break;
		}
		remain = BLOCK_SIZE - current->dwUser;
		memcpy(current->lpData + current->dwUser, data, remain);
		size -= remain;
		data += remain;
		current->dwBufferLength = BLOCK_SIZE;
		waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
		waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));

		// calculate volume
		SHORT *pSrc = (SHORT*)current->lpData;
		double dbVal = pSrc[0];
		double dbResule = fabs(dbVal);
		for(unsigned int i = 0; i < current->dwBufferLength/2; i++)
		{
			dbVal = pSrc[i];
			m_dbVolume = max(fabs(dbVal), dbResule);
		}

		EnterCriticalSection(&waveCriticalSection);
		waveFreeBlockCount--;
		LeaveCriticalSection(&waveCriticalSection);
		
		// wait for a block to become free
		while(!waveFreeBlockCount)
			Sleep(10);

		// point to the next block
		waveCurrentBlock++;
		waveCurrentBlock %= BLOCK_COUNT;
		current = &waveBlocks[waveCurrentBlock];
		current->dwUser = 0;
	}
}

WAVEHDR* CWaveOut::allocateBlocks(int size, int count)
{
	unsigned char* buffer;
	WAVEHDR* blocks;
	DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;

	// allocate memory for the entire set in one go
	void* pBuffer;
	if((pBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, totalBufferSize)) == NULL)
		ExitProcess(1);

	buffer = (unsigned char*)pBuffer;
	// and set up the pointers to each bit
	blocks = (WAVEHDR*)buffer;
	buffer += sizeof(WAVEHDR) * count;
	for(int i = 0; i < count; i++) {
		blocks[i].dwBufferLength = size;
		blocks[i].lpData = (LPSTR)buffer;
		buffer += size;
	}
	return blocks;
}

void CWaveOut::freeBlocks(WAVEHDR* blockArray)
{ 
	// and this is why allocateBlocks works the way it does
	HeapFree(GetProcessHeap(), 0, blockArray);
}

void CWaveOut::CloseSpeaker()
{
	if (m_WaveOut)
	{
		waveOutClose(m_WaveOut);

		m_WaveOut = NULL;
	}
}

void CWaveOut::waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	// pointer to free block counter
	int* freeBlockCounter = (int*)dwInstance;

	// ignore calls that occur due to openining and closing the device.
	if(uMsg != WOM_DONE)
		return;

	EnterCriticalSection(&waveCriticalSection);
	(*freeBlockCounter)++;
	LeaveCriticalSection(&waveCriticalSection);
}