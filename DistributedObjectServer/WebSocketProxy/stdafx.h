// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifdef WIN32
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

#include <afxwin.h>         // MFC ���ĺͱ�׼���

#endif


#include "../../Libs/Utils/Utils.h"
#include "../../Libs/NetLib/NetLib.h"
#include "../../Libs/DOSLib/DOSLib.h"


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�



#ifdef WIN32
#define USE_CRT_DETAIL_NEW
#endif


#define CONFIG_FILE_NAME					"WebSocketProxy.xml"


extern UINT g_LogChannel;

inline void LogWithTag(const char* szTag, const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_NORMAL, szTag, Format, vl);
	va_end(vl);
}

inline void LogDebugWithTag(const char* szTag, const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(g_LogChannel, ILogPrinter::LOG_LEVEL_DEBUG, szTag, Format, vl);
	va_end(vl);
}

#ifdef WIN32
#define Log(_Format, ...)	LogWithTag(__FUNCTION__, _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugWithTag(__FUNCTION__, _Format, ##__VA_ARGS__)
#else
#define Log(_Format, ...)	LogWithTag(__PRETTY_FUNCTION__, _Format, ##__VA_ARGS__)
#define LogDebug(_Format, ...)	LogDebugWithTag(__PRETTY_FUNCTION__, _Format, ##__VA_ARGS__)
#endif





#include "MainConfig.h"
#include "WebSocketProxyConnection.h"
#include "WebSocketProxyService.h"





