#pragma once

#include "../../include/Util/Util.h"

class CWndRightTopBar : public WindowImplBase
{
public:
	CWndRightTopBar(void);
	~CWndRightTopBar(void);

protected:
	virtual CDuiString GetSkinFolder() { return _T ("skin"); };
	virtual CDuiString GetSkinFile()  { return _T ("WndRightTopbar.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndRightTopbar"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual	void Notify(TNotifyUI& msg);

public:
	void ShowUI(bool bShow = true, bool bShowChat = true, bool bFull = true);
	VidyoClientLectureStage	m_eLectureStage;
	void SetLectureStage(VidyoClientLectureStage lectureStage);
	tSelfControlStatus	m_tSelfStatus;
	void SetSelfContorlStatus(tSelfControlStatus selfStatus);
};

