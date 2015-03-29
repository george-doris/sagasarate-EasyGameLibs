// DBProxy.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MainApp.h"



LPCTSTR g_ProgramName="DistributedObjectServer";
LPCTSTR g_ServiceName="DOSServer";
LPCTSTR g_ServiceDesc="Distributed Object Server";

// CMainApp

// CMainApp ����

CMainApp::CMainApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_IsConfigLoaded=false;
}


// Ψһ��һ�� CMainApp ����

CMainApp theApp;


// CMainApp ��ʼ��

//BOOL CMainApp::InitInstance()
//{
//	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
//	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
//	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
//	InitCommonControls();
//
//	CWinApp::InitInstance();
//
//	AfxEnableControlContainer();
//
//	// ��׼��ʼ��
//	// ���δʹ����Щ���ܲ�ϣ����С
//	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
//	// ����Ҫ���ض���ʼ������
//	// �������ڴ洢���õ�ע�����
//	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
//	// �����޸�Ϊ��˾����֯��
//	SetRegistryKey(_T("CGAServer"));
//
//	m_pConsoleDlg=new CServerConsoleDlg();
//	m_pConsoleDlg->Create(CServerConsoleDlg::IDD);
//	m_pConsoleDlg->RegisterTrayIcon(g_ProgramName);
//	m_pMainWnd=m_pConsoleDlg;
//
//	if(!CMainThread::GetInstance()->StartUp())
//		return FALSE;
//	
//
//	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
//	// ����������Ӧ�ó������Ϣ�á�
//	return TRUE;
//}
//
//int CMainApp::ExitInstance()
//{
//	
//	CMainThread::GetInstance()->ShutDown(MAX_SERVER_TERMINATE_WAIT_TIME);
//	SAFE_DELETE(m_pConsoleDlg);
//	return 0;
//}

BOOL CMainApp::OnStartUp()
{		
	PrintImportantLog(0,"DistributedObjectServer Start");
	CPerformanceStatistician::GetInstance()->ResetPerformanceStat();


	if(!m_IsConfigLoaded)
	{
		CSystemConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL,CMainThread::GetInstance()->GetConfigFileName()));
		m_IsConfigLoaded=CMainConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL,CONFIG_FILE_NAME));
	}
#ifdef WIN32
	UINT Flag=EXCEPTION_SET_DEFAULT_HANDLER|EXCEPTION_USE_API_HOOK|EXCEPTION_MAKE_DUMP;
	if(CSystemConfig::GetInstance()->IsPreLoadModuleSym())
		Flag|=EXCEPTION_PRE_INIT_SYM;
	if(CSystemConfig::GetInstance()->IsMakeFullDump())
		Flag|=EXCEPTION_MAKE_FULL_DUMP;
	if (CSystemConfig::GetInstance()->IsLogModuleSymStatus())
		Flag |= EXCEPTION_LOG_MODULE_SYM_STATUS;
	
	CExceptionParser::GetInstance()->Init(Flag);
#endif

	if(!m_IsConfigLoaded)
		return FALSE;

	if(!CMainThread::GetInstance()->StartUp(
		CMainConfig::GetInstance()->GetNetConfig().EventObjectPool.StartSize,
		CMainConfig::GetInstance()->GetNetConfig().NetWorkThreadPerCPU,
		CMainConfig::GetInstance()->GetNetConfig().EventRouterPool.StartSize,
		CMainConfig::GetInstance()->GetNetConfig().EventObjectPool.GrowSize,
		CMainConfig::GetInstance()->GetNetConfig().EventObjectPool.GrowLimit,
		CMainConfig::GetInstance()->GetNetConfig().EventRouterPool.GrowSize,
		CMainConfig::GetInstance()->GetNetConfig().EventRouterPool.GrowLimit))
		return FALSE;
	m_pServer=CMainThread::GetInstance();
	return TRUE;
}
void CMainApp::OnShutDown()
{
	CMainThread::GetInstance()->ShutDown(MAX_SERVER_TERMINATE_WAIT_TIME);

	CLogManager::ReleaseInstance();
	CPerformanceStatistician::ReleaseInstance();
}

void CMainApp::OnSetServiceName()
{
	PrintImportantLog(0,"OnSetServiceName");
	if(!m_IsConfigLoaded)
	{
		CSystemConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL,CMainThread::GetInstance()->GetConfigFileName()));
		m_IsConfigLoaded=CMainConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL,CONFIG_FILE_NAME));
	}
	if(m_IsConfigLoaded&&CMainConfig::GetInstance()->HaveServiceName())
	{
		m_lpServiceName=CMainConfig::GetInstance()->GetServiceName();
		m_lpDisplayName=CMainConfig::GetInstance()->GetServiceDesc();
		PrintImportantLog(0,"����������Ϊ:%s/%s",m_lpServiceName,m_lpDisplayName);
	}
}