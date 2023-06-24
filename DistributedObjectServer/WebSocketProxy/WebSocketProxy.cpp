// TestGameServer.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

extern "C" UTILS_DLL_EXPORT bool InitPlugin(UINT PluginID, UINT LogChannel, LPCTSTR ConfigDir, LPCTSTR LogDir);
extern "C" UTILS_DLL_EXPORT IDOSObjectProxyService * GetProxyService();


UINT g_LogChannel = 0;
CWebSocketProxyService* g_pService = NULL;
bool InitPlugin(UINT PluginID, UINT LogChannel, LPCTSTR ConfigDir, LPCTSTR LogDir)
{
	g_LogChannel = LogChannel;

	CEasyString ConfileFileName = ConfigDir;
	ConfileFileName += DIR_SLASH;
	ConfileFileName += CONFIG_FILE_NAME;
	if (!CMainConfig::GetInstance()->LoadConfig(ConfileFileName))
	{
		Log("���������ļ�[%s]ʧ��", CONFIG_FILE_NAME);
		return FALSE;
	}
	return TRUE;
}

IDOSObjectProxyService * GetProxyService()
{
	g_pService = new CWebSocketProxyService();
	return g_pService;
}


