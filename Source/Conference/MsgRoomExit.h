#pragma once
class CMsgRoomExit : public WindowImplBase
{
public:
	CMsgRoomExit(void);
	~CMsgRoomExit(void);

protected:
	virtual CDuiString GetSkinFolder() { return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\"); };
	virtual CDuiString GetSkinFile()  { return _T ("MsgRoomExit.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MsgRoomExit"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

