/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerAppLinux.cpp                                       */
/*      ��������:  2009��09��11��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

#include <execinfo.h>

#define MAX_STACK_LAYERS    20

CServerApp	* m_gAppInstance=NULL;

bool g_IsService = false;



int main(int argc, char *argv[])
{
	if (argc>1)
	{
		if (strcmp(argv[1], "-d") == 0)
		{
			daemon(1, 1);
			g_IsService = true;
		}
	}


	m_gAppInstance->Run();
	return 0;
}




CServerApp::CServerApp(void)
{
	m_WantExist=FALSE;
	m_pServer=NULL;
	m_gAppInstance=this;
}

CServerApp::~CServerApp(void)
{
}



BOOL CServerApp::OnStartUp()
{
	InitSignals();
	return TRUE;
}
void CServerApp::OnShutDown()
{
	if (m_pServer)
		m_pServer->QueryShowDown();
}

int CServerApp::Run()
{
	if(OnStartUp())
	{
		while ((!m_WantExist) && m_pServer && (!m_pServer->IsServerTerminated()))
		{
			DoSleep(1);
		}
		OnShutDown();
	}
	return 0;
}

void CServerApp::InitSignals()
{
	struct sigaction SigAction;

	SigAction.sa_flags = 0;
	SigAction.sa_sigaction = OnQuitSignal;
	if (sigaction(SIGTERM, &SigAction, NULL) != 0)
	{
		PrintImportantLog(0, "ע���ź�SIGTERM����ʧ��%d", errno);
	}
	else
	{
		PrintImportantLog(0, "ע���ź�SIGTERM����");
	}

	SigAction.sa_flags = SA_RESETHAND;
	sigaddset(&SigAction.sa_mask, SIGABRT);
	sigaddset(&SigAction.sa_mask, SIGFPE);
	sigaddset(&SigAction.sa_mask, SIGSEGV);
	sigaddset(&SigAction.sa_mask, SIGILL);
	SigAction.sa_sigaction = OnExceptionSignal;
	if (sigaction(SIGABRT, &SigAction, NULL) != 0)
	{
		PrintImportantLog(0, "ע���ź�SIGABRT����ʧ��%d", errno);
	}
	else
	{
		PrintImportantLog(0, "ע���ź�SIGABRT����");
	}

	if (sigaction(SIGFPE, &SigAction, NULL) != 0)
	{
		PrintImportantLog(0, "ע���ź�SIGFPE����ʧ��%d", errno);
	}
	else
	{
		PrintImportantLog(0, "ע���ź�SIGFPE����");
	}

	if (sigaction(SIGSEGV, &SigAction, NULL) != 0)
	{
		PrintImportantLog(0, "ע���ź�SIGSEGV����ʧ��%d", errno);
	}
	else
	{
		PrintImportantLog(0, "ע���ź�SIGSEGV����");
	}

	if (sigaction(SIGILL, &SigAction, NULL) != 0)
	{
		PrintImportantLog(0, "ע���ź�SIGILL����ʧ��%d", errno);
	}
	else
	{
		PrintImportantLog(0, "ע���ź�SIGILL����");
	}
}


void CServerApp::OnQuitSignal(int SignalNum, siginfo_t * pSigInfo, void * pContext)
{
	PrintImportantLog(0, "�յ��˳��ź�%d��׼���˳�", SignalNum);
	if (m_gAppInstance)
		m_gAppInstance->m_WantExist = TRUE;
}

void CServerApp::OnExceptionSignal(int SignalNum, siginfo_t * pSigInfo, void * pContext)
{	
	void *array[MAX_STACK_LAYERS];

	PrintImportantLog(0, "��ʼ�����ź�%d", SignalNum);
	size_t size = backtrace(array, MAX_STACK_LAYERS);
	PrintImportantLog(0, "�������ջ��С:%d", (int)size);
	CFileLogPrinter * pLog = dynamic_cast<CFileLogPrinter *>(CLogManager::GetInstance()->GetChannel(LOG_IMPORTANT_CHANNEL));
	if (pLog)
	{
		CLinuxFileAccessor * pFile = dynamic_cast<CLinuxFileAccessor *>(pLog->GetFile());
		if (pFile)
		{
			backtrace_symbols_fd(array, MAX_STACK_LAYERS, pFile->GetFileDescriptor());
		}
	}
	
	//char ** strings = backtrace_symbols(array, size);
	//for (size_t i = 0; i < size; i++)
	//{
	//	PrintImportantLog(0, "StackTrace:%s", strings[i]);
	//}
	//free(strings);

	PrintImportantLog(0, "�������ջ���");
}
