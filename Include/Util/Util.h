#pragma once


#include "../DataDefine.h"
//#include "../../include/VidyoClient/VidyoClient.h"

#include <xstring>
#include <string>
#include <sstream>
#include <atlconv.h>

using namespace std;

//将UTF8字符串解码成通用字符串
wstring UTF82UNCODE(LPCSTR lpBuff, int nLen);
//改变文本格式为UTf8
char *ChangTextToUtf8(LPCTSTR szText,char* szTemp);	
//改变文本格式由UTf8转回
TCHAR *ChangUtf8ToText(const char* szText,TCHAR *szTemp);	
//改变文本格式，有UTF8转为GBK
LPSTR ChangUtf8ToGBK(LPCSTR lpBuff, LPSTR lpTemp);
//
short int HexChar2Dec(char ch);

// get path from full path name
LPTSTR GetFilePath(LPCTSTR file, LPTSTR path);
LPCTSTR GetFileName(LPCTSTR lpFileName,TCHAR *init_file);
int ReadSetupIniInt(LPCTSTR lpcszSec,LPCTSTR lpcszKey,int nDefValue);
void ReadSetupIniString(LPCTSTR lpcszSec,LPCTSTR lpcszKey,LPCTSTR lpcszDefValue,TCHAR* lpValue,int nMax);

void WriteIniInt(LPCTSTR lpcszSec,LPCTSTR lpcszKey,int nValue);
void WriteIniString(LPCTSTR lpcszSec,LPCTSTR lpcszKey,LPCTSTR lpcszValue);
int ReadIniInt(LPCTSTR lpcszSec,LPCTSTR lpcszKey,int nDefValue);
void ReadIniString(LPCTSTR lpcszSec,LPCTSTR lpcszKey,LPCTSTR lpcszDefValue,TCHAR* lpValue,int nMax);

void project_language_loadstring(LPCTSTR project, LPCTSTR key, TCHAR* szValue, int max);

// function: copy window and save
// input: lpAPPName, name of the window to be print and save
// input: lpFileName, saved file name
// input: iCapHeight, top of copy window
// input: iWidth, width of copy window
// input: iHeight, height of copy window
// input: hWnd, HWND of the window to be copy and save
void SaveWindowCopy(LPCTSTR lpAPPName, LPCSTR lpFileName, int iCapHeight, int iWidth, int iHeight, HWND hWnd = NULL);
int SaveBitmapToFile(HBITMAP hBitmap, LPCSTR lpFileName);

// function: print window and save
// input: hWnd, HWND of the window to be print and save
// input: lpFileName, saved file name
void SaveWindowPrint(HWND hWnd, LPCSTR lpFileName);

// open folder
// input:hWnd, owner window
// input:szPath, folder path
void OpenPath(HWND hWnd, LPCTSTR szPath);

// select path
// input:szTitle, dialog title
// input:szPath, get path return
void SetPath(LPCTSTR szTitle, LPWSTR szPath);

// if is a valid domain name
bool is_valid_domain_name(const string& str);

void AddAutoRun();
void DeleteAutoRun();