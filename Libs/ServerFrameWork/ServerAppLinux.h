/****************************************************************************/
/*                                                                          */
/*      �ļ���:    ServerAppLinux.h                                         */
/*      ��������:  2009��07��06��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#include <signal.h>


class CServerApp
{
protected:
	volatile UINT		m_WantExist;
	IBaseServer *		m_pServer;
public:
	CServerApp(void);
	~CServerApp(void);

public:

	virtual BOOL OnStartUp();
	virtual void OnShutDown();

	virtual int Run();

protected:
	void GetProgamVersion();
	void InitSignals();
	static void OnQuitSignal(int SignalNum, siginfo_t * pSigInfo, void * pContext);
	static void OnExceptionSignal(int SignalNum, siginfo_t * pSigInfo, void * pContext);

};

extern bool g_IsService;