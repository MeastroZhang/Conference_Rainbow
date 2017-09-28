#pragma once

class CDlgRoomSetting : public WindowImplBase
{
public:
	CDlgRoomSetting(LPVOID* pRoom);
	~CDlgRoomSetting(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("\\DlgRoomSetting\\DlgRoomSetting.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("RoomSetting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

	VidyoClientAccountData* m_ptRoomData;
public:
	void UpdataUI();
};

