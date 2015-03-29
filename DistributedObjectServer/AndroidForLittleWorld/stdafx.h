// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <math.h>

#include "..\..\libs\Utils\Utils.h"
#include "..\..\libs\NetLib\NetLib.h"
#include "..\..\libs\DOSLib\DOSLib.h"
#include "..\..\libs\MFCExtensionLib\MFCExtensionLib.h"

#include "..\LittleWorldProtocol\LittleWorldProtocol.h"

#define CONFIG_FILE_NAME				"AndroidConfig.xml"
#define MAX_LOG_LINE					2000
#define LOG_BUFFER_SIZE					10240
#define EVENT_QUEUE_SIZE				1024
#define SCRITP_THREAD_COUNT				8
#define CONNECT_TIMEOUT					30000
#define MAX_CHAR_LIST_COUNT				512
#define MAX_GROUP_CHAR_LIST_COUNT		2048
#define CHAR_MOVE_STATUS_UPDATE_TIME	100
#define MAX_COMMAND_HISTORY				100
#define SCRIPT_INT_TIMEOUT				(30000)

struct CLIENT_CONFIG
{
	CIPAddress		ServerAddress;
	CEasyString		AccountName;
	CEasyString		Password;
	UINT			FirstConnectDelay;
	UINT			ReconnectDelay;
	bool			AutoReconect;
	CEasyString		ScriptFileName;
};

struct CLIENT_GROUP_CONFIG:public CLIENT_CONFIG
{
	UINT			ClientCount;
	UINT			StartIndex;
};


struct MAP_CHAR_INFO
{
	UINT64			CharID;
	int				RefCount;
	CMoveStatus		MoveStatus;
	CBaseCharInfo	BaseCharInfo;
	MAP_CHAR_INFO()
	{
		CharID=0;
		RefCount=0;
	}
};

#include "resource.h"       // ������

#include "Client.h"
#include "ClientGroup.h"

#include "MapBox.h"
#include "DlgMapView.h"
#include "ClientInfoView.h"
#include "ClientFrm.h"

#include "MainControlView.h"
#include "MainControlFrame.h"

#include "MainFrm.h"


#include "MainApp.h"


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


