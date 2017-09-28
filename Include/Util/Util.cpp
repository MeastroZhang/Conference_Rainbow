
#pragma once

#include "StdAfx.h"
#include "Util.h"
#include "Registry.h"
#include <Shlobj.h>
#include <CommDlg.h>

#define INI_CONFIG	_T("CONF")

#define LAN_EN_US	_T("en_US.ini")
#define LAN_ZH_CN	_T("zh_CN.ini")
#define LAN_ZH_TW	_T("zh_TW.ini")
//---------------------------------------------------------------------------------------
//将UTF8字符串解码成通用字符串
wstring UTF82UNCODE(LPCSTR lpBuff, int nLen)
{
	int nCont = MultiByteToWideChar(CP_UTF8, 0, lpBuff, nLen, NULL, 0);

	WCHAR* szTemp = new WCHAR[nCont+1];
	ZeroMemory(szTemp, sizeof(WCHAR)*(nCont + 1));

	MultiByteToWideChar(CP_UTF8, 0, lpBuff, nLen, szTemp, nCont);

	wstring strResult;
	strResult.clear();
	strResult.append(szTemp, nCont);

	delete[]szTemp;
	szTemp = NULL;

	return strResult;
}

//改变文本格式为UTf8
char *ChangTextToUtf8(LPCTSTR szText,char* szTemp)
{
#ifdef _UNICODE
	int nCount = WideCharToMultiByte(CP_UTF8, 0, szText, -1, NULL, 0, NULL, NULL) ;

	char* szChang = new char[nCount+1];
	ZeroMemory(szChang, sizeof(char)*(nCount + 1));

	WideCharToMultiByte(CP_UTF8, 0, szText, -1, szChang, nCount, NULL, NULL);

	strcpy(szTemp, szChang);

	delete[]szChang;
	szChang = NULL;
#else
	strcpy(szTemp, szText);
#endif

	return szTemp;
}

//改变文本格式由UTf8转回
TCHAR *ChangUtf8ToText(const char* szText,TCHAR *szTemp)
{
#ifdef _UNICODE
	int nCount = MultiByteToWideChar(CP_UTF8, 0, szText, -1, NULL, 0) ;

	WCHAR* szChang = new WCHAR[nCount+1];
	ZeroMemory(szChang, sizeof(WCHAR)*(nCount + 1));

	MultiByteToWideChar(CP_UTF8, 0, szText, -1, szChang, nCount);

	_tcscpy(szTemp, szChang);

	delete[]szChang;
	szChang = NULL;
#else
	strcpy(szTemp, szText);
#endif

	return szTemp;
}

//改变文本格式，有UTF8转为GBK
LPSTR ChangUtf8ToGBK(LPCSTR lpBuff, LPSTR lpTemp)
{
#ifdef _UNICODE
	int nCount = MultiByteToWideChar(CP_UTF8, 0, lpBuff, -1, NULL, 0) ;

	WCHAR* szChang = new WCHAR[nCount+1];
	ZeroMemory(szChang, sizeof(WCHAR)*(nCount + 1));

	MultiByteToWideChar(CP_UTF8, 0, lpBuff, -1, szChang, nCount);

	nCount = WideCharToMultiByte(CP_ACP, 0, szChang, -1, NULL, 0, NULL, NULL);       
	char *strGBK = new char[nCount+1];       
	WideCharToMultiByte(CP_ACP, 0, szChang, -1, strGBK, nCount, NULL, NULL);

	strcpy(lpTemp, strGBK);

	delete[]szChang;
	szChang = NULL;
	delete[]strGBK;
	strGBK = NULL;
#else
	strcpy(lpTemp, lpBuff);
#endif

	return lpTemp;
}

short int HexChar2Dec(char ch)
{
	if ('0'<=ch && ch<='9')
	{
		return short(ch-'0');
	}
	else if ('a'<=ch && ch<='f')
	{
		return short(ch-'a')+10;
	}
	else if ('A'<=ch && ch<='F')
	{
		return short(ch-'A')+10;
	}
	else
		return -1;
}

LPTSTR GetFilePath( LPCTSTR file, LPTSTR path)
{
	int i, len = (int)_tcslen((LPTSTR)file);

	for(i =len-1; i >-1; i--)
		if(file[i] ==_T('\\') || file[i] ==_T('/') || file[i] ==_T(':')) break;
	if(i ==0) _tcscpy(path, _T("."));
	else
	{
		_tcscpy(path, file);
		path[i] =0;
	}
	return path;
}

LPCTSTR GetFileName(LPCTSTR lpFileName,TCHAR *init_file)
{
	TCHAR sztemp[SZTEMP_LENTH], sztemp2[SZTEMP_LENTH];
	GetModuleFileName(NULL, sztemp2, sizeof(sztemp2));
	GetFilePath(sztemp2, sztemp);
	_tcscat(sztemp, lpFileName);
	wsprintf(init_file,_T("%s"), sztemp);
	return	init_file;
}

int ReadSetupIniInt(LPCTSTR lpcszSec,LPCTSTR lpcszKey,int nDefValue)
{
	return ::GetPrivateProfileInt(lpcszSec, lpcszKey, nDefValue, INI_CONFIG);
}

void ReadSetupIniString(LPCTSTR lpcszSec,LPCTSTR lpcszKey,LPCTSTR lpcszDefValue,TCHAR* lpValue,int nMax)
{
	TCHAR szPath[MAX_PATH];
	GetFileName(INI_CONFIG, szPath);
	::GetPrivateProfileString(lpcszSec, lpcszKey, lpcszDefValue, lpValue, nMax, szPath);
}

void WriteIniInt(LPCTSTR lpcszSec, LPCTSTR lpcszKey, int nValue)
{
	TCHAR	szValue[51];
	wsprintf(szValue, _T("%d"), nValue);
	::WritePrivateProfileString(lpcszSec, lpcszKey, szValue, PATH_CONFIG);
}

void WriteIniString(LPCTSTR lpcszSec, LPCTSTR lpcszKey, LPCTSTR lpcszValue)
{
	::WritePrivateProfileString(lpcszSec, lpcszKey, lpcszValue, PATH_CONFIG);
}

int ReadIniInt(LPCTSTR lpcszSec,LPCTSTR lpcszKey,int nDefValue)
{
	return ::GetPrivateProfileInt(lpcszSec, lpcszKey, nDefValue, PATH_CONFIG);
}

void ReadIniString(LPCTSTR lpcszSec,LPCTSTR lpcszKey,LPCTSTR lpcszDefValue,TCHAR* lpValue,int nMax)
{
	::GetPrivateProfileString(lpcszSec,lpcszKey,lpcszDefValue,lpValue,nMax,PATH_CONFIG);
}

void project_language_loadstring(LPCTSTR project, LPCTSTR key, TCHAR* szValue, int max)
{
	TCHAR szPath[MAX_PATH];			// 语言包文件路径

	ZeroMemory(szValue, sizeof(TCHAR)*max);
	ZeroMemory(szPath, sizeof(TCHAR)*MAX_PATH);

	TCHAR *p = NULL;
	::GetModuleFileName(NULL, szPath, sizeof(szPath));
	p = _tcsrchr(szPath, '\\');	
	if (p)
	{
		_tcscpy(p, _T("\\language\\"));
	}

	int iLanType = ReadIniInt(_T("SETTING"), _T("CurrentLanguage"), 1);
	switch (iLanType)
	{
	case 0:
		_tcscat(szPath, LAN_EN_US);
		break;
	case 2:
		_tcscat(szPath, LAN_ZH_TW);
		break;
	default:
		_tcscat(szPath, LAN_ZH_CN);
		break;
	}

	::GetPrivateProfileString(project, key, _T(""), szValue, max-1, szPath);
}

void SaveWindowCopy(LPCTSTR lpAPPName, LPCSTR lpFileName, int iCapHeight, int iWidth, int iHeight, HWND hWnd /*= NULL*/)
{
	if (hWnd == NULL) hWnd = ::FindWindowW(NULL, lpAPPName);

	POINT pt = {0, 0};
	ClientToScreen(hWnd, &pt);

	// create hdc and hbmp
	HDC hdcScreen = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, iWidth, iHeight);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hbmp);
	BitBlt(hdc, 0, 0, iWidth, iHeight, hdcScreen, pt.x, pt.y+iCapHeight, SRCCOPY);
	hbmp = (HBITMAP)SelectObject(hdc, hOldBitmap);

	SaveBitmapToFile(hbmp, lpFileName);

	DeleteDC(hdc);
	DeleteObject(hbmp);
	ReleaseDC(NULL, hdcScreen);
}

void SaveWindowPrint(HWND hWnd, LPCSTR lpFileName)
{
	if (hWnd == NULL)
		hWnd = GetDesktopWindow();

	RECT rc;
	::GetWindowRect(hWnd, &rc);

	// create hdc and hbmp
	HDC hdcScreen = GetDC(NULL);
	HDC hdc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, 180, 100/*rc.right-rc.left, rc.bottom-rc.top*/);  
	SelectObject(hdc, hbmp);

	PrintWindow(hWnd, hdc, PW_CLIENTONLY);

	SaveBitmapToFile(hbmp, lpFileName);

	DeleteDC(hdc); 
	DeleteObject(hbmp); 
	ReleaseDC(NULL, hdcScreen);
}
 
int SaveBitmapToFile(HBITMAP hBitmap, LPCSTR lpFileName) //hBitmap 为刚才的屏幕位图句柄
{      
	HDC     hDC;			//设备描述表
	int     iBits;			//当前显示分辨率下每个像素所占字节数
	WORD    wBitCount;		//位图中每个像素所占字节数
	
	//定义调色板大小， 位图中像素字节大小 ，  位图文件大小 ， 写入文件字节数
	DWORD				dwPaletteSize=0, dwBmBitsSize, dwDIBSize, dwWritten;
	BITMAP				Bitmap;
	BITMAPFILEHEADER	bmfHdr;				//位图属性结构
	BITMAPINFOHEADER	bi;					//位图文件头结构
	LPBITMAPINFOHEADER	lpbi;				//位图信息头结构 
	HANDLE				fh, hDib, hPal;		//指向位图信息头结构
	HPALETTE			hOldPal=NULL;		//定义文件，分配内存句柄，调色板句柄
 
	//计算位图文件每个像素所占字节数
	hDC = CreateDCA("DISPLAY",NULL,NULL,NULL);
	iBits = 16/*GetDeviceCaps(hDC, BITSPIXEL)*GetDeviceCaps(hDC, PLANES)*/;
	DeleteDC(hDC);
	if (iBits <= 1) wBitCount = 1;
	else if (iBits <= 4) wBitCount = 4;
	else if (iBits <= 8) wBitCount = 8;
	else if (iBits <= 24) wBitCount = 24;
	else wBitCount = 32;
	
	if (wBitCount <= 8) dwPaletteSize=(1<<wBitCount)*sizeof(RGBQUAD);	 //计算调色板大小
 
	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = Bitmap.bmWidth;
	bi.biHeight          = Bitmap.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount        = wBitCount;
	bi.biCompression     = BI_RGB;
	bi.biSizeImage       = 0;
	bi.biXPelsPerMeter   = 0;
	bi.biYPelsPerMeter   = 0;
	bi.biClrUsed         = 0;
	bi.biClrImportant    = 0;
 
	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)/32)*4*Bitmap.bmHeight;	 //为位图内容分配内存
	//计算位图大小分解一下(解释一下上面的语句)
	//每个扫描行所占的字节数应该为4的整数倍，具体算法为:
	//int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
	//if((Bitmap.bmWidth*wBitCount) % 32)
	//biWidth++; //不是整数倍的加1
	//biWidth *= 4;//到这里，计算得到的为每个扫描行的字节数。
	//dwBmBitsSize = biWidth * Bitmap.bmHeight;//得到大小

	hDib	= GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi	= (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi	= bi;
	// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//创建位图文件    
	fh = CreateFileA(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh==INVALID_HANDLE_VALUE) return FALSE;

	// 设置位图文件头
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);	// 写入位图文件头
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize , &dwWritten, NULL);	// 写入位图文件其余内容
 
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;
}

void OpenPath(HWND hWnd, LPCTSTR szPath)
{
	TCHAR szBuffer[MAX_PATH] = {0};
	OPENFILENAME ofn= {0}; 
	ofn.lStructSize = sizeof(ofn); 
	ofn.hwndOwner = hWnd; 
	ofn.lpstrFilter = _T("bmp(*.bmp)\0*.bmp\0AllFiles(*.*)\0*.*\0");//要选择的文件后缀 
	ofn.lpstrInitialDir = szPath;//默认的文件路径 
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区 
	ofn.nMaxFile = sizeof(szBuffer)/sizeof(*szBuffer); 
	ofn.nFilterIndex = 0; 
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&ofn);
}

void SetPath(LPCTSTR szTitle, LPWSTR szPath)
{
	BROWSEINFO bi; 
	ZeroMemory(&bi,sizeof(BROWSEINFO)); 
	bi.hwndOwner = NULL; 
	bi.pszDisplayName = NULL; 

	bi.lpszTitle = szTitle; 
	bi.ulFlags = BIF_BROWSEINCLUDEFILES; 
	LPITEMIDLIST idl = SHBrowseForFolder(&bi); 
	if (NULL == idl) 
	{ 
		return; 
	} 
	SHGetPathFromIDList(idl,szPath); 
}



bool is_valid_domain_name(const string& str)
{  
	static const string tlds="|com|net|org|edu|gov|int|mil|ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|arpa|at|au|ax|az|ba|bb|bd|be|bf|bg|bh|bi|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cu|cv|cx|cy|cz|de|dj|dk|dm|do|dz|ec|ee|eg|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kp|kr|kw|ky|kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|me|mg|mh|mk|ml|mm|mn|mo|mp|mq|ms|mt|mu|mv|mw|mx|my|mz|na|nc|ne|nf|ng|ni|nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|rs|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|sk|sl|sm|sn|so|sr|st|su|sv|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|za|zm|zw|aw|as|mr|bl|bq|cw|eh|mf|ss|sx|um|aaa|aarp|abarth|abb|abbott|abbvie|abc|able|abogado|abudhabi|academy|accenture|accountant|accountants|acer|aco|active|actor|adac|ads|adult|aeg|aero|aetna|afamilycompany|afl|africa|africamagic|agakhan|agency|aig|aigo|airbus|airforce|airtel|akdn|alcon|alfaromeo|alibaba|alipay|allfinanz|allfinanzberater|allfinanzberatung|allstate|ally|alsace|alstom|amazon|americanexpress|americanfamily|amex|amfam|amica|amp|amsterdam|analytics|and|android|anquan|ansons|anthem|antivirus|aol|apartments|app|apple|aquarelle|aquitaine|arab|aramco|archi|architect|are|army|art|arte|asda|asia|associates|astrium|athleta|attorney|auction|audi|audible|audio|auspost|author|auto|autoinsurance|autos|avery|avianca|axa|axis|azure|baby|baidu|banamex|bananarepublic|band|bank|banque|bar|barcelona|barclaycard|barclays|barefoot|bargains|baseball|basketball|bauhaus|bayern|bbb|bbc|bbt|bbva|bcg|bcn|beats|beauty|beer|beknown|bentley|berlin|best|bestbuy|bet|bharti|bible|bid|bike|bing|bingo|bio|biz|black|blackfriday|blanco|blockbuster|blog|bloomberg|bloomingdales|blue|bms|bmw|bnl|bnpparibas|boats|boehringer|bofa|bom|bond|boo|book|booking|boots|bosch|bostik|boston|bot|boutique|box|bradesco|bridgestone|broadway|broker|brother|brussels|budapest|bugatti|buick|build|builders|business|buy|buzz|bway|bzh|cab|cadillac|cafe|cal|call|calvinklein|cam|camera|camp|canalplus|cancerresearch|canon|capetown|capital|capitalone|caravan|cards|care|career|careers|caremore|carinsurance|cars|cartier|casa|case|caseih|cash|cashbackbonus|casino|cat|catalonia|catering|catholic|cba|cbn|cbre|cbs|ceb|center|ceo|cern|cfa|cfd|chanel|changiairport|channel|charity|chartis|chase|chat|chatr|cheap|chesapeake|chevrolet|chevy|chintai|city|chk|chloe|christmas|chrome|chrysler|church|cialis|cimb|cipriani|circle|cisco|citadel|citi|citic|cityeats|claims|cleaning|click|clinic|clinique|clothing|club|clubmed|coach|codes|coffee|college|cologne|com|comcast|commbank|community|company|compare|computer|comsec|condos|connectors|construction|consulting|contact|contractors|cooking|cookingchannel|cool|coop|corsica|country|coupon|coupons|courses|credit|creditcard|creditunion|cricket|crown|crs|cruise|cruises|csc|cuisinella|cymru|cyou|dabur|dad|dance|data|date|dating|datsun|day|dclk|dds|deal|dealer|deals|degree|delivery|dell|delmonte|deloitte|delta|democrat|dental|dentist|desi|design|deutschepost|dhl|diamonds|diet|digikey|digital|direct|directory|discount|discover|dish|diy|dnb|dnp|docomo|docs|doctor|dodge|dog|doha|domains|doosan|dot|dotafrica|download|drive|dstv|dtv|dubai|duck|dunlop|duns|dupont|durban|dvag|dvr|dwg|earth|eat|eco|ecom|edeka|education|edu|email|emerck|emerson|energy|engineer|engineering|enterprises|epost|epson|equipment|ericsson|erni|esq|est|estate|esurance|etisalat|eurovision|eus|events|everbank|exchange|expert|exposed|express|extraspace|fage|fail|fairwinds|faith|family|fan|fans|farm|farmers|fashion|fast|fedex|feedback|ferrari|ferrero|fiat|fidelity|fido|film|final|finance|financial|financialaid|finish|fire|firestone|firmdale|fish|fishing|fitness|flickr|flights|flir|florist|flowers|fls|flsmidth|fly|foo|food|foodnetwork|football|ford|forex|forsale|forum|foundation|fox|free|fresenius|frl|frogans|frontdoor|frontier|ftr|fujitsu|fujixerox|fun|fund|furniture|futbol|fyi|gai|gal|gallery|gallo|gallup|game|games|gap|garden|garnier|gay|gbiz|gcc|gdn|gea|gecompany|ged|gent|genting|george|ggee|gift|gifts|gives|giving|glade|glass|gle|glean|global|globalx|globo|gmail|gmbh|gmc|gmo|gmx|godaddy|gold|goldpoint|golf|goo|goodhands|goodyear|goog|google|gop|got|gotv|grainger|graphics|gratis|gree|green|gripe|grocery|group|guardian|guardianlife|guardianmedia|gucci|guge|guide|guitars|guru|hair|halal|hamburg|hangout|haus|hbo|hdfc|hdfcbank|health|healthcare|heart|heinz|help|helsinki|here|hermes|hgtv|hilton|hiphop|hisamitsu|hitachi|hiv|hkt|hockey|holdings|holiday|homedepot|homegoods|homes|homesense|honda|honeywell|horse|host|hosting|hoteis|hotel|hoteles|hotels|hotmail|house|how|htc|hughes|hyatt|hyundai|ibm|icbc|ice|icu|idn|ieee|ifm|iinet|ikano|imamat|imdb|immo|immobilien|indians|industries|infiniti|info|infosys|infy|ing|ink|institute|insurance|insure|intel|international|intuit|investments|ipiranga|ira|irish|iselect|islam|ismaili|ist|istanbul|itau|itv|iveco|iwc|jaguar|java|jcb|jcp|jeep|jetzt|jewelry|jio|jlc|jll|jmp|jnj|jobs|joburg|jot|joy|jpmorgan|jpmorganchase|jprs|juegos|juniper|justforu|kaufen|kddi|kerastase|kerryhotels|kerrylogisitics|kerryproperties|ketchup|kfh|kia|kid|kids|kiehls|kim|kinder|kindle|kitchen|kiwi|koeln|komatsu|konami|kone|kosher|kpmg|kpn|krd|kred|ksb|kuokgroup|kyknet|kyoto|lacaixa|ladbrokes|lamborghini|lamer|lancaster|lancia|lancome|land|landrover|lanxess|lat|latino|latrobe|lawyer|lds|lease|leclerc|lefrak|legal|lego|lexus|lgbt|liaison|lidl|life|lifeinsurance|lifestyle|lighting|lightning|like|lilly|limited|limo|lincoln|linde|link|lipsy|live|livestrong|living|lixil|llc|loan|loans|locker|locus|loft|lol|london|loreal|lotte|lotto|love|lpl|lplfinancial|ltd|ltda|lundbeck|lupin|luxe|luxury|macys|madrid|maif|maison|makeup|man|management|mango|map|market|marketing|markets|marriott|marshalls|maserati|mattel|maybelline|mba|mcd|mcdonalds|mckinsey|media|medical|meet|melbourne|meme|memorial|men|menu|meo|merck|merckmsd|metlife|miami|microsoft|mih|mii|mini|mint|mit|mitek|mitsubishi|mlb|mls|mnet|mobi|mobile|mobily|moda|moe|mom|monash|money|monster|montblanc|mopar|mormon|mortgage|moscow|moto|motorcycles|mov|movie|movistar|mozaic|mrmuscle|mrporter|mtn|mtpc|mtr|multichoice|museum|music|mutual|mutualfunds|mutuelle|mzansimagic|nab|nadex|nagoya|name|naspers|nationwide|natura|navy|nba|nec|net|netaporter|netbank|netflix|network|neustar|new|newholland|news|next|nextdirect|nexus|nfl|ngo|nhk|nico|nike|nikon|ninja|nissan|nissay|nokia|northlandinsurance|northwesternmutual|norton|now|nowruz|nowtv|nra|nrw|ntt|nyc|obi|observer|off|okinawa|olayan|olayangroup|oldnavy|ollo|olympus|omega|ong|onl|online|onyourside|ooo|open|oracle|orange|org|organic|orientexpress|origins|osaka|otsuka|ott|overheidnl|ovh|page|pamperedchef|panasonic|panerai|paris|pars|partners|parts|party|passagens|patagonia|patch|pay|payu|pccw|persiangulf|pets|pfizer|pharmacy|phd|philips|phone|photo|photography|photos|physio|piaget|pics|pictet|pictures|pid|pin|ping|pink|pioneer|piperlime|pitney|pizza|place|play|playstation|plumbing|plus|pnc|pohl|poker|politie|polo|porn|post|pramerica|praxi|press|prime|pro|prod|productions|prof|progressive|promo|properties|property|protection|pru|prudential|pub|pwc|qpon|qtel|quebec|quest|qvc|racing|radio|raid|ram|read|realestate|realtor|realty|recipes|red|redken|redstone|redumbrella|rehab|reise|reisen|reit|ren|rent|rentals|repair|report|republican|rest|restaurant|retirement|review|reviews|rexroth|rich|richardli|ricoh|rightathome|ril|rio|rip|rmit|rocher|rocks|rockwool|rodeo|rogers|roma|room|rsvp|rugby|ruhr|run|rwe|ryukyu|saarland|safe|safety|safeway|sakura|sale|salon|samsclub|samsung|sandvik|sandvikcoromant|sanofi|sap|sapo|sapphire|sarl|sas|save|saxo|sbi|sbs|sca|scb|schaeffler|schedule|schmidt|scholarhips|scholarships|schule|schwarz|schwarzgroup|science|scjohnson|scor|scot|search|seat|security|seek|select|sener|services|ses|seven|sew|sex|sexy|sfr|shangrila|sharp|shell|shia|shiksha|shirriam|shoes|shop|shopping|shopyourway|shouji|show|showtime|shriram|silk|sina|singles|ski|skin|skolkovo|sky|skydrive|skype|sling|smart|smile|sncf|soccer|social|softbank|software|sohu|solar|solutions|song|sony|soy|spa|space|spiegel|sport|sports|spot|spreadbetting|srt|stada|staples|star|starhub|statebank|statefarm|statoil|stc|stcgroup|stockholm|storage|store|stream|stroke|studio|study|style|sucks|supersport|supplies|supply|support|surf|surgery|suzuki|svr|swatch|swiftcover|swiss|symantec|systems|sydney|tab|taipei|talk|taobao|target|tata|tatamotors|tatar|tattoo|tax|taxi|tci|tdk|team|technology|tel|telecity|telefonica|temasek|tennis|terra|teva|thai|thd|theater|theatre|theguardian|thehartford|tiaa|tickets|tienda|tiffany|tips|tires|tirol|tjmaxx|tjx|tkmaxx|tmall|today|tokyo|tools|top|toray|toshiba|total|tour|tours|town|toyota|toys|trade|tradershotels|trading|training|transformers|translations|transunion|travel|travelchannel|travelers|travelersinsurance|travelguard|trust|trv|tube|tui|tunes|tushu|tvs|ubank|ubs|uconnect|ultrabook|ummah|unicom|unicorn|university|uno|uol|ups|vacations|vana|vanguard|vanish|vegas|ventures|verisign|versicherung|vet|viajes|video|vig|viking|villas|vin|vip|virgin|visa|vision|vista|vistaprint|viva|vivo|vlaanderen|vodka|volkswagen|volvo|vons|vote|voting|voto|voyage|vuelos|wales|walmart|walter|wang|wanggou|warman|watch|watches|weather|weatherchannel|web|webcam|weber|webjet|webs|website|wed|wedding|weibo|weir|whoswho|wien|wiki|williamhill|wilmar|windows|wine|winners|wme|wolterskluwer|world|woodside|work|works|wow|wtc|wtf|xbox|xerox|xfinity|xihuan|xin|xn--11b4c3d|xn--1ck2e1b|xn--1qqw23a|xn--30rr7y|xn--3bst00m|xn--3ds443g|xn--3e0b707e|xn--3oq18vl8pn36a|xn--3pxu8k|xn--42c2d9a|xn--45brj9c|xn--45q11c|xn--4gbrim|xn--4gq48lf9j|xn--54b7fta0cc|xn--55qw42g|xn--55qx5d|xn--55qx5d8y0buji4b870u|xn--5su34j936bgsg|xn--5tzm5g|xn--6frz82g|xn--6qq986b3x1|xn--6qq986b3xl|xn--6rtwn|xn--80adxhks|xn--80ao21a|xn--80aqecdr1a|xn--80asehdb|xn--80aswg|xn--8y0a063a|xn--90a3ac|xn--9et52u|xn--9krt00a|xn--b4w605ferd|xn--bck1b9a5dre4c|xn--c1avg|xn--c1yn36f|xn--c2br7g|xn--cck2b3b|xn--cckwcxetd|xn--cg4bki|xn--clchc0ea0b2g2a9gcd|xn--czr694b|xn--czrs0t|xn--czru2d|xn--d1acj3b|xn--dkwm73cwpn|xn--eckvdtc9d|xn--efvy88h|xn--estv75g|xn--fct429k|xn--fes124c|xn--fhbei|xn--fiq228c5hs|xn--fiq64b|xn--fiQ64b|xn--fiQ64B|xn--fiqs8s|xn--fiqz9s|xn--fjq720a|xn--flw351e|xn--fpcrj9c3d|xn--fzc2c9e2c|xn--fzys8d69uvgm|xn--g2xx48c|xn--gckr3f0f|xn--gecrj9c|xn--gk3at1e|xn--h2brj9c|xn--hdb9cza1b|xn--hxt035cmppuel|xn--hxt035czzpffl|xn--hxt814e|xn--i1b6b1a6a2e|xn--imr513n|xn--io0a7i|xn--j1aef|xn--j1amh|xn--j6w193g|xn--j6w470d71issc|xn--jlq480n2rg|xn--jlq61u9w7b|xn--jvr189m|xn--kcrx77d1x4a|xn--kcrx7bb75ajk3b|xn--kprw13d|xn--kpry57d|xn--kpu716f|xn--kput3i|xn--lgbbat1ad8j|xn--mgb9awbf|xn--mgba3a3ejt|xn--mgba3a4f16a|xn--mgba7c0bbn0a|xn--mgbaakc7dvf|xn--mgbaam7a8h|xn--mgbab2bd|xn--mgbai9azgqp6j|xn--mgbayh7gpa|xn--mgbb9fbpob|xn--mgbbh1a71e|xn--mgbc0a9azcg|xn--mgbca7dzdo|xn--mgberp4a5d4ar|xn--mgbi4ecexp|xn--mgbt3dhd|xn--mgbv6cfpo|xn--mgbx4cd0ab|xn--mk1bu44c|xn--mxtq1m|xn--ngbc5azd|xn--ngbe9e0a|xn--ngbrx|xn--node|xn--nqv7f|xn--nqv7fs00ema|xn--nyqy26a|xn--o3cw4h|xn--ogbpf8fl|xn--otu796d|xn--p1acf|xn--p1ai|xn--pbt977c|xn--pgb3ceoj|xn--pgbs0dh|xn--pssy2u|xn--q9jyb4c|xn--qcka1pmc|xn--rhqv96g|xn--rovu88b|xn--s9brj9c|xn--ses554g|xn--t60b56a|xn--tckwe|xn--tiq49xqyj|xn--tqq33ed31aqia|xn--unup4y|xn--vermgensberater-ctb|xn--vermgensberatung-pwb|xn--vhquv|xn--vuq861b|xn--w4r85el8fhu5dnra|xn--w4rs40l|xn--wgbh1c|xn--wgbl6a|xn--xhq521b|xn--xkc2al3hye2a|xn--xkc2dl3a5ee0h|xn--yfro4i67o|xn--ygbi2ammx|xn--zfr164b|xn—3ds443g|xn—fiq228c5hs|xperia|xxx|xyz|yachts|yahoo|yamaxun|yandex|yellowpages|yodobashi|yoga|yokohama|you|youtube|yun|zappos|zara|zero|zip|zippo|zone|zuerich|zulu|";  
	const char* s=str.c_str();  
	int segsize=0,i,ix=str.length();  
	if (s[0]=='.' || s[ix-1]=='.' || ix>253)   
		return false;  
	for(i=0,segsize=0; i<ix; i++)  
	{  
		if (s[i]=='.')  
		{  
			if (segsize==0) //fail for abc..com  
				return false;  
			segsize=0;  
		}  
		else if ( ('0' <= s[i] && s[i] <='9')   
			|| ('a' <= s[i] && s[i] <='z')   
			|| ('A' <= s[i] && s[i] <='Z')   
			|| (s[i]=='-' && segsize!=0 && i+1<ix && s[i+1]!='.')  
			)  
		{  
			segsize++;  
		}  
		else  
			return false; //invalid char...  

		if (segsize>63)  
			return false;  
	}  

	stringstream ss;  
	ss << "|" << str.substr(ix-segsize) << "|" ;//get last domain segment  

	if (tlds.find(ss.str())==string::npos)  
		return false;  

	return true;  
} 

void AddAutoRun()
{
	HKEY   hKey; 
	TCHAR pFileName[MAX_PATH];
	HMODULE GetModH = GetModuleHandle(NULL);
	//得到当前执行文件的全路径
	GetModuleFileNameW(NULL, (LPWCH)pFileName, MAX_PATH);
	char szPath[MAX_PATH];
	ChangTextToUtf8(pFileName, szPath);
	int iLen = strlen(szPath);
	//找到系统的启动项 
	LPCTSTR lpRun = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
	//打开启动项Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0L, KEY_QUERY_VALUE, &hKey);
	if(lRet== ERROR_SUCCESS)
	{
		//添加注册
		RegSetValueEx(hKey, L"Conference",0,REG_SZ,(const BYTE *)szPath,iLen);
		RegCloseKey(hKey); 
	}
}

void DeleteAutoRun()
{
	HKEY   hKey; 
	char pFileName[MAX_PATH] = {0}; 
	//得到程序自身的全路径 
	DWORD dwRet = GetModuleFileNameW(NULL, (LPWCH)pFileName, MAX_PATH); 
	//找到系统的启动项 
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); 
	//打开启动项Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0L, KEY_ALL_ACCESS, &hKey);
	if(lRet== ERROR_SUCCESS)
	{
		//删除注册
		RegDeleteValue(hKey,_T("Conference"));
		RegCloseKey(hKey); 
	}
}