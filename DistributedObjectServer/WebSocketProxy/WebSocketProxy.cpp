// TestGameServer.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

extern "C" UTILS_DLL_EXPORT bool InitPlugin(UINT PluginID, UINT LogChannel);
extern "C" UTILS_DLL_EXPORT IDOSObjectProxyService * GetProxyService();
extern "C" UTILS_DLL_EXPORT IDOSObjectProxyConnection * CreateProxyConnection();



UINT g_LogChannel = 0;

bool InitPlugin(UINT PluginID, UINT LogChannel)
{
	g_LogChannel = LogChannel;

	if (!CMainConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL, CONFIG_FILE_NAME)))
	{
		Log("���������ļ�[%s]ʧ��", CONFIG_FILE_NAME);
		return FALSE;
	}
	return TRUE;
}

IDOSObjectProxyService * GetProxyService()
{
	return NULL;
}

IDOSObjectProxyConnection * CreateProxyConnection()
{
	//_CrtSetBreakAlloc(155);	

	return new CWebSocketProxyConnection();
	
}

