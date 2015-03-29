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
#include "../../libs/DBLib/DBLib.h"

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <math.h>


#ifdef WIN32
#define USE_CRT_DETAIL_NEW
#endif

struct CHAR_CREATE_INFO
{
	UINT64			AccountID;
	UINT64			CharID;
	CEasyString		CharName;
	BYTE			Race;
	BYTE			Gender;
	BYTE			SkinColor;
	BYTE			HairColor;
	BYTE			FaceType;
	BYTE			HairType;
	BYTE			BeardType;
	bool			IsCharBald;
	UINT64			MapID;
	float			PosX;
	float			PosY;
	float			PosHeight;
	float			Direction;
	CHAR_CREATE_INFO()
	{
		Clear();
	}
	void Clear()
	{
		AccountID=0;
		CharID=0;
		CharName.Clear();
		Race=0;
		Gender=0;
		SkinColor=0;
		HairColor=0;
		FaceType=0;
		HairType=0;
		BeardType=0;
		IsCharBald=0;
		MapID=0;
		PosX=0;
		PosY=0;
		PosHeight=0;
		Direction=0;		
	}
};

struct BASE_CHAR_INFO
{
	UINT64			CharID;
	CEasyString		CharName;
	BYTE			Race;
	BYTE			Gender;
	BYTE			SkinColor;
	BYTE			HairColor;
	BYTE			FaceType;
	BYTE			HairType;
	BYTE			BeardType;
	bool			IsCharBald;
	UINT64			MapID;
	float			PosX;
	float			PosY;
	float			PosHeight;
	float			Direction;
	BASE_CHAR_INFO()
	{
		Clear();
	}
	void Clear()
	{
		CharID=0;
		CharName.Clear();
		Race=0;
		Gender=0;
		SkinColor=0;
		HairColor=0;
		FaceType=0;
		HairType=0;
		BeardType=0;
		IsCharBald=0;
		MapID=0;
		PosX=0;
		PosY=0;
		PosHeight=0;
		Direction=0;		
	}
};

struct PLAYER_CHAR_INFO
{
	enum PLAYER_CHAR_INFO_STATUS
	{
		STATUS_SAVED,
		STATUS_SAVING,
		STATUS_NEED_SAVE,
	};
	UINT64				AccountID;
	int					Status;
	BASE_CHAR_INFO		BaseCharInfo;
};

#define CONFIG_FILE_NAME					"LittleWorldDBPlugin.xml"


#define SERVER_LOG_CHANNEL			8001

inline void Log(const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(SERVER_LOG_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,0,Format,vl);
	va_end(vl);
}

inline void LogDebug(const char* Format, ...)
{
	va_list vl;

	va_start(vl, Format);
	CLogManager::GetInstance()->PrintLogVL(SERVER_LOG_CHANNEL,ILogPrinter::LOG_LEVEL_DEBUG,0,Format,vl);
	va_end(vl);
}



#include "../LittleWorldProtocol/LittleWorldProtocol.h"

#include "MainConfig.h"

#include "PlayerCharDataDBAdapter.h"

#include "DBTransAccountLogin.h"
#include "DBTransCreateChar.h"
#include "DBTransGetCharIDSeed.h"
#include "DBTransGetAccountChar.h"
#include "DBTransGetCharData.h"
#include "DBTransSaveCharData.h"

extern CPlayerCharDataDBAdapter * GetPlayerCharDataDBAdapter();

//#define PERFORMANCE_STAT


#ifdef PERFORMANCE_STAT

#define FUNCTION_BEGIN				{static int s_FunctionIndex=0;if(!s_FunctionIndex) s_FunctionIndex=CPerformanceStatistician::GetInstance()->GetFunctionIndex(s_FunctionIndex,__FUNCTION__); CAutoPerformanceCounter tmpPerformanceCounter(s_FunctionIndex);
#define FUNCTION_END				}

#else

#define FUNCTION_BEGIN
#define FUNCTION_END

#endif

