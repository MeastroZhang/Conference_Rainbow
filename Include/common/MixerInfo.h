#pragma once

// ***************************************************************
//  混音器的信息设置

 #include <mmsystem.h>

class CMixerInfo
{
public:
	CMixerInfo(HWND hWnd,DWORD dwComponentType=4, DWORD dwMinValue = 0, DWORD dwMaxValue = 100);
	virtual ~CMixerInfo(void);

	CDuiString GetDeviceName();				// 取设备名称

	// Volume
	DWORD GetVolumeValue();					// 取音量值
	void SetVolumeValue(DWORD dwValue);		// 设音量值
	BOOL GetVolumeMute();					// 判断是否静音	FALSE 静音	TRUE 有声音
	void SetVolumeMute(BOOL bMute);			// 设置静音		FALSE 静音	TRUE 有声音

private:
	void InitVolumeInfo(DWORD dwComponentType);					// 初始化混音器信息
	DWORD GetValue(DWORD dwComponentType);					// 得到相应类型的音量的大小
	void SetValue(DWORD dwValue, DWORD dwComponentType);	// 设置相应类型的音量的大小

	BOOL GetMute(DWORD dwComponentType);					// 得到相应类型是否静音	
	void SetMute(BOOL bMute, DWORD dwComponentType);		// 设置相应类型是否静音

private:
	HWND		m_hWnd;
	DWORD		m_dwMinValue;				// 最小的音量值	
	DWORD		m_dwMaxValue;				// 最大的音量值					
	HMIXER		m_hMixer;					// 当前混合器的句柄	
	CDuiString     m_strDeviceName;			// 设备名称
	DWORD		m_dwComponentType;			// 应用的类型
};
