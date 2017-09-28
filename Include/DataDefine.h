#pragma once

#define WM_VIDYO						WM_USER+0x0001
#define WM_WINDOW						WM_USER+0x0002
#define WM_WINDOW_LOGIN					WM_USER+0x0003
#define WM_WINDOW_CONTACTS				WM_USER+0x0004
#define WM_WINDOW_CONFERENCE			WM_USER+0x0005
#define WM_WINDOW_SETTING				WM_USER+0x0006
#define WM_TRAY							WM_USER+0x0007
#define WM_TOOLBAR						WM_USER+0x0008
#define WM_SPEAKER_TEST					WM_USER+0x0009
#define WM_EXIT_CONFERENCE				WM_USER+0x000a
#define WM_END_CONFERENCE				WM_USER+0x000b
#define WM_CHAT_REPLY					WM_USER+0x000c
#define WM_JOIN_PROCESS					WM_USER+0x000d
#define WM_MODERATOR_CHANGED			WM_USER+0x000e

#define PORTALURL	_T("http://t.vf365.cn")
#define PATH_DOCS	"C:\\Users\\Public"
#define PATH_LOG	"C:\\Users\\Public\\Conference\\Log"
#define PATH_PHOTO	L"C:\\Users\\Public\\Conference\\PhotoGraph"
#define PATH_THUMB	L"C:\\Users\\Public\\Conference\\Thumb"
#define PATH_CONFIG	L"C:\\Users\\Public\\Conference\\config.ini"

//转化临时字符串的长度
#define SZTEMP_LENTH			(1024)

#define TIMER_TOOLBAR			1001
#define TIMER_SEARCH			1002
#define TIMER_CLOCK				1003
#define TIMER_AUDIO_TEST		1004
#define TIMER_SPEAKER_TEST		1005
#define TIMER_LECTUREMODE		1006	// lecture mode refresh status
#define TIMER_LECTURE_NOTE		1007	// lecture mode reminder window close timer

typedef enum eMsgWndType_
{
	MSG_COMMON,
	MSG_WRONGPIN,
	MSG_ROOMLOCKED,
	MSG_RECORD_DISABLE,
} eMsgWndType;

typedef enum ClientStatusType_
{
	CLIENTSTATUSNOLOG,		//未登录
	CLIENTSTATUSLOGINED,	//已登录
	CLIENTSTATUSMEETING,	//会议中
} ClientStatusType;

typedef enum eLoginError_
{
	ERROR_USERNAME_NULL = 1,
	ERROR_PSW_NULL,
	ERROR_ROOMID_NULL,
	ERROR_NAME_NULL,
} eLoginError;

typedef enum ToolBarMsg_
{	
	TB_MSG_SHARE = 1,
	TB_MSG_STOPSHARE,
	TB_MSG_CONTROL,
	TB_MSG_CHAT,
	TB_MSG_PHOTO,
	TB_MSG_EXIT,
} ToolBarMsg;

typedef struct _tChatUser
{
	TCHAR	UserName[129];
	TCHAR	UserURL[257];
} tChatUser;

typedef struct _tChatUserList
{
	int			iNum;
	tChatUser*	Users;
} tChatUserList;

typedef struct _tChatMsg
{
	BOOL	bSend;
	TCHAR	szName[129];
	TCHAR	szMsg[1025];
	TCHAR	szTime[128];
} tChatMsg;

typedef struct _tChatMsgList
{
	int			iMsgNum;
	tChatMsg*	tMsg;
} tChatMsgList;

// get video replay library
typedef struct _tRecordLibrary
{
	char	token[128];
	char	URL[128];
} tRecordLibrary;

typedef struct _tStartTime
{
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
} tStartTime;

// self control status
typedef struct _tSelfControlStatus
{
	bool	bMuteVideo;
	bool	bMuteAudioIn;
	bool	bMuteAudioOut;
	bool	bMuteServerVideo;
	bool	bMuteServerAuidoIn;
} tSelfControlStatus;

// server control status
typedef struct _tServerControlStatus
{
	bool	bLocked;
	bool	bMuteVide;
	bool	bMuteAudio;
	bool	bRecord;
	bool	bRecordPause;
	VidyoClientLectureStage	lectureMode;
} tServerContorlStatus;

typedef enum _eModeType 
{	ntConferenceUI = 0, 
	ntGuestUI,
	ntLoginUI,
	ntSetting,
	ntContactsUI,
	ntProcessing
} eModeType;

typedef enum _eBtnLoginWin 
{	ntCloseBtn = 0,
	ntMinBtn,
	ntLoginBtn
} eBtnLoginWin;

typedef enum _eInputWndType
{
	ntRoomPin = 0,
	ntModeratorPin,
} eInputWndType;