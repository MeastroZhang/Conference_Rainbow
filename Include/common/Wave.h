#pragma once

#include <mmsystem.h>
#include <windows.h>

#define MAX_SAMPLES 8192   //  
#define MAX_VOIE 2  
#define MAX_SIZE_SAMPLES  1  // WORD  
#define MAX_SIZE_INPUT_BUFFER   MAX_SAMPLES*MAX_VOIE*MAX_SIZE_SAMPLES   
#define DEFAULT_CAL_OFFSET 0 // >>>>> depends of you sound card  
#define DEFAULT_CAL_GAIN   1.0

// buffers into which sound data is recorded   
#define NUM_LEVEL_BUFFERS   20

#define BLOCK_SIZE			1024*8
#define BLOCK_COUNT			20;

class CWaveIn
{
public:
	CWaveIn(void);
	~CWaveIn(void);

	HRESULT OpenMic(UINT deviceID);
	void CloseMic();

	bool m_bIsOpen;
	bool IsOpen();

	double m_dbVolume;
	double GetVolume();

protected:
	struct _HWWAVEDATA_STRUCT  
	{  
		WAVEHDR m_WaveHeader;  
		SHORT   InputBuffer[MAX_SIZE_INPUT_BUFFER];  
	}m_HWWaveStruct[NUM_LEVEL_BUFFERS];

	struct _WAVESAMPLED_STRUCT  
	{  
		ULONGLONG mTimesCount;  
		BOOL    bSampleValid;  
		BOOL    bIsReaded;  
		BOOL    bIsCalculating;  
		DWORD   m_SizeRecord;  
		SHORT   InputBuffer[MAX_SIZE_INPUT_BUFFER];  
	}m_SampledData[2];

	HWAVEIN			m_WaveIn;
	WAVEINCAPS		m_WaveInDevCaps;
	WAVEFORMATEX	wfx;

	ULONGLONG m_SampledTimesIndex;

	BOOL bTerminateFlage;

	static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
};

class CWaveOut
{
public:
	CWaveOut(HWND hParent = NULL);
	~CWaveOut();

	HRESULT OpenSpeaker(CDuiString strFileName);
	void writeAudio(HWAVEOUT hWaveOut,LPSTR data, int size);
	WAVEHDR* allocateBlocks(int size, int count);
	void freeBlocks(WAVEHDR* blockArray);
	void CloseSpeaker();

	double m_dbVolume;
	double GetVolume();

protected:
	struct _HWWAVEDATA_STRUCT  
	{  
		WAVEHDR m_WaveHeader;  
		SHORT   InputBuffer[MAX_SIZE_INPUT_BUFFER];  
	}m_HWWaveStruct[NUM_LEVEL_BUFFERS];

	struct _WAVESAMPLED_STRUCT  
	{  
		ULONGLONG mTimesCount;  
		BOOL    bSampleValid;  
		BOOL    bIsReaded;  
		BOOL    bIsCalculating;  
		DWORD   m_SizeRecord;  
		SHORT   InputBuffer[MAX_SIZE_INPUT_BUFFER];  
	}m_SampledData[2];

	HWAVEOUT		m_WaveOut;
	WAVEOUTCAPS		m_WaveOutDevCaps;
	WAVEFORMATEX	wfx;

	HWND	m_hParent;

	static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
};
