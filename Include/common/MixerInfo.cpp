#include "StdAfx.h"
#include ".\MixerInfo.h"
#include "../../include/Util/Util.h"
 #include <mmsystem.h>

CMixerInfo::CMixerInfo(HWND hWnd,DWORD dwComponentType, DWORD dwMinValue, DWORD dwMaxValue)
{
	m_hWnd					= hWnd;
	m_dwMinValue			= dwMinValue;
	m_dwMaxValue			= dwMaxValue;			
	m_hMixer				= NULL;								
	m_strDeviceName			= _T("");

	switch(dwComponentType)
	{
	case MIXERLINE_COMPONENTTYPE_DST_SPEAKERS:
		m_dwComponentType=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
		InitVolumeInfo(MIXERLINE_COMPONENTTYPE_DST_SPEAKERS);
		break;
	case MIXERLINE_COMPONENTTYPE_DST_WAVEIN:
		m_dwComponentType=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;
		InitVolumeInfo(MIXERLINE_COMPONENTTYPE_DST_WAVEIN);
		break;
	}
}

CMixerInfo::~CMixerInfo(void)
{
	m_hWnd = NULL;
	if (m_hMixer != NULL)
	{	
		//关闭混合器
		::mixerClose(m_hMixer);
		m_hMixer = NULL;
	}
}

CDuiString CMixerInfo::GetDeviceName()
{
	return m_strDeviceName;
}

DWORD CMixerInfo::GetVolumeValue()
{
	return GetValue(m_dwComponentType);
}

void CMixerInfo::SetVolumeValue(DWORD dwValue)
{
	SetValue(dwValue, m_dwComponentType);
}

BOOL CMixerInfo::GetVolumeMute()							// 判断是否静音
{
	return GetMute(m_dwComponentType);
}

void CMixerInfo::SetVolumeMute(BOOL bMute)				// 设置静音
{
	SetMute(bMute, m_dwComponentType);
}

void CMixerInfo::InitVolumeInfo(DWORD dwComponentType)
{	
	UINT		uNumMixers;				// 混合器的数量											
	MIXERCAPS	MixerCaps;				// 当前混合器的性能参数	

	uNumMixers = ::mixerGetNumDevs();	//初始化，获取当前混合设备数量 
	::ZeroMemory(&MixerCaps, sizeof(MIXERCAPS));
	if(uNumMixers !=0)
	{	
		for(unsigned int i = 0; i < uNumMixers; i ++)
		{
			// 打开混合设备
			if (::mixerOpen(&m_hMixer, i, (DWORD)m_hWnd, NULL, /*MIXER_OBJECTF_MIXER |*/ CALLBACK_WINDOW) != MMSYSERR_NOERROR)
				continue ;

			MIXERLINE MixerLine;   
			//结构体的大小   
			MixerLine.cbStruct = sizeof(MixerLine);   
			//录制设备总线   
			MixerLine.dwComponentType   =   dwComponentType;   
			//得到录制总线中的连接数   
			if(mixerGetLineInfo((HMIXEROBJ)m_hMixer, &MixerLine,
				MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE) != MMSYSERR_NOERROR)
				continue; 
			else
				break;
		}

		// 确定混音器设备的能力
		if (::mixerGetDevCaps((UINT)m_hMixer, &MixerCaps, sizeof(MIXERCAPS)) != MMSYSERR_NOERROR)
			return ;
	}

	m_strDeviceName.Format(_T("%s"), MixerCaps.szPname);
}

DWORD CMixerInfo::GetValue(DWORD dwComponentType)
{
	//获得混合器信息
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwComponentType;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!= MMSYSERR_NOERROR)
		return  101;

	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct		= sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID		= mxl.dwLineID;
	mxlc.dwControlType	= MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cControls		= 1;
	mxlc.cbmxctrl		= sizeof(MIXERCONTROL);
	mxlc.pamxctrl		= &mxc;

	//获得混合器线控件
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
		return 101;

	//获取当前音量
	DWORD NewVolume = 0;	
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
	MIXERCONTROLDETAILS mxcd;

	mxcd.cbStruct		= sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID	= mxc.dwControlID;
	mxcd.cChannels		= 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails		= sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails		= &mxcdVolume;

	//获取指定混合器控件 
	if (::mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		return 101;

	NewVolume = (mxcdVolume.dwValue*m_dwMaxValue)/mxc.Bounds.dwMaximum;

	return NewVolume;
}

void  CMixerInfo::SetValue(DWORD dwValue, DWORD dwComponentType)
{
	//获得混合器信息
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwComponentType;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!= MMSYSERR_NOERROR)
		return;

	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct		= sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID		= mxl.dwLineID;
	mxlc.dwControlType	= MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cControls		= 1;
	mxlc.cbmxctrl		= sizeof(MIXERCONTROL);
	mxlc.pamxctrl		= &mxc;

	//获得混合器线控件
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
		return;

	//设置音量
	MIXERCONTROLDETAILS_UNSIGNED NewmxcdVolume = { (dwValue*mxc.Bounds.dwMaximum)/m_dwMaxValue };
	MIXERCONTROLDETAILS mxcd;

	mxcd.cbStruct		= sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID	= mxc.dwControlID;
	mxcd.cChannels		= 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails		= sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails		= &NewmxcdVolume;

	//放置混合器控件 
	if (::mixerSetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		return ;
}

BOOL CMixerInfo::GetMute(DWORD dwComponentType)
{
	//获得混合器信息
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwComponentType;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!= MMSYSERR_NOERROR)
		return FALSE;

	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct		= sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID		= mxl.dwLineID;
	mxlc.dwControlType	= MIXERCONTROL_CONTROLTYPE_MUTE;
	mxlc.cControls		= 1;
	mxlc.cbmxctrl		= sizeof(MIXERCONTROL);
	mxlc.pamxctrl		= &mxc;

	//获得混合器线控件
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
		return FALSE;


	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN mxcMute;

	mxcd.cbStruct		= sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID	= mxc.dwControlID;
	mxcd.cChannels		= 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails		= sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails		= &mxcMute;

	//获取指定混合器控件 
	if (::mixerGetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		return FALSE;

	return !mxcMute.fValue;
}

void CMixerInfo::SetMute(BOOL bMute, DWORD dwComponentType)
{
	//获得混合器信息
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = dwComponentType;
	if (::mixerGetLineInfo((HMIXEROBJ)m_hMixer, &mxl, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE)!= MMSYSERR_NOERROR)
		return;

	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct		= sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID		= mxl.dwLineID;
	mxlc.dwControlType	= MIXERCONTROL_CONTROLTYPE_MUTE;
	mxlc.cControls		= 1;
	mxlc.cbmxctrl		= sizeof(MIXERCONTROL);
	mxlc.pamxctrl		= &mxc;

	//获得混合器线控件
	if (::mixerGetLineControls((HMIXEROBJ)m_hMixer, &mxlc, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE)!= MMSYSERR_NOERROR)
		return;


	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_BOOLEAN mxcMute;

	mxcd.cbStruct		= sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID	= mxc.dwControlID;
	mxcd.cChannels		= 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails		= sizeof(MIXERCONTROLDETAILS_BOOLEAN);
	mxcd.paDetails		= &mxcMute;

	mxcMute.fValue      = !bMute;

	//放置混合器控件 
	if (::mixerSetControlDetails((HMIXEROBJ)m_hMixer, &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR)
		return ;
}