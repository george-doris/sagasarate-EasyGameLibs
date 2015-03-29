// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�

#pragma once

#ifdef WIN32

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0510 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0501	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT



#endif



#ifdef _DEBUG
#define CONFIG_FILE_NAME		"ConfigD.xml"
#else
#define CONFIG_FILE_NAME		"Config.xml"
#endif

#define SERVER_INFO_COUNT_TIME					5000
#define MAX_SERVER_TERMINATE_WAIT_TIME			3000000

#define PLUGIN_INIT_FN_NAME				"InitPlugin"
#define PLUGIN_QUERY_RELEASE_FN_NAME	"QueryReleasePlugin"
#define PLUGIN_RELEASE_FN_NAME			"ReleasePlugin"

#define PLUGIN_LOG_CHANNEL_START	12000

#ifdef WIN32
#define USE_CRT_DETAIL_NEW
#endif

#include "../../Libs/ServerFrameWork/ServerFrameWork.h"

#include "../../Libs/DOSLib/DOSLib.h"

#ifndef WIN32

#include <dlfcn.h>

#endif

#if defined(WIN32)&&(!defined(_DEBUG))

#define OBJECT_EXCEPTION_CATCH_START \
	__try\
	{

#define OBJECT_EXCEPTION_CATCH_END \
	}__except(CExceptionParser::ExceptionPrinter(GetExceptionInformation(),m_ObjectID.ID,__FUNCTION__))\
	{\
		throw;\
	}
#else
#define OBJECT_EXCEPTION_CATCH_START
#define OBJECT_EXCEPTION_CATCH_END
#endif

#include "DistributedObjectOperator.h"
#include "DistributedObjectManager.h"

extern "C" typedef BOOL (*PLUGIN_INIT_FN)(IDistributedObjectManager* pObjectManager,UINT PluginID,UINT LogChannel);
extern "C" typedef void (*PLUGIN_QUERY_RELEASE_FN)();
extern "C" typedef void (*PLUGIN_RELEASE_FN)();

#include "DOSServerThread.h"
#include "MainConfig.h"
#include "MainThread.h"


//#define PERFORMANCE_STAT

#ifdef PERFORMANCE_STAT

#define FUNCTION_BEGIN				{static int s_FunctionIndex=0;if(!s_FunctionIndex) s_FunctionIndex=CPerformanceStatistician::GetInstance()->GetFunctionIndex(s_FunctionIndex,__FUNCTION__); CAutoPerformanceCounter tmpPerformanceCounter(s_FunctionIndex);
#define FUNCTION_END				}

#else

#define FUNCTION_BEGIN
#define FUNCTION_END

#endif
