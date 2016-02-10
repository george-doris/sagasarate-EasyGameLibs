#include "stdafx.h"


IMPLEMENT_CLASS_INFO_STATIC(CDOSServerThread,CDOSServer);

CDOSServerThread::CDOSServerThread()
{
	FUNCTION_BEGIN;
	m_pSysNetLinkManager=NULL;
	m_pUDPSystemControlPort=NULL;
	m_ConsoleLogLevel=0;
	FUNCTION_END;
}

CDOSServerThread::~CDOSServerThread(void)
{
	FUNCTION_BEGIN;

	FUNCTION_END;
}

void CDOSServerThread::Execute()
{
	FUNCTION_BEGIN;
#ifndef _DEBUG
	CMainGuardThread::GetInstance()->SetTargetThreadID(GetThreadID());
	CMainGuardThread::GetInstance()->Start();
#endif
	while((!m_WantTerminate)&&(OnRun()))
	{
#ifndef _DEBUG
		CMainGuardThread::GetInstance()->MakeKeepAlive();
#endif
	}
#ifndef _DEBUG
	CMainGuardThread::GetInstance()->SafeTerminate();
#endif
	OnBeginTerminate();
	DWORD Time=CEasyTimer::GetTime();
	while(GetTimeToTime(Time,CEasyTimer::GetTime())<SERVER_ENDING_TIME&&OnTerminating())
	{
	}

	FUNCTION_END;
}


BOOL CDOSServerThread::OnStart()
{
	FUNCTION_BEGIN;


	m_TCPRecvBytes=0;
	m_TCPSendBytes=0;
	m_UDPRecvBytes=0;
	m_UDPSendBytes=0;
	m_TCPRecvCount=0;
	m_TCPSendCount=0;
	m_UDPRecvCount=0;
	m_UDPSendCount=0;
	m_CountTimer.SaveTime();

	//װ��ϵͳ����
	CSystemConfig::GetInstance()->LoadConfig(MakeModuleFullPath(NULL,GetConfigFileName()));


	CEasyString LogFileName;
	CEasyString ModulePath=GetModulePath(NULL);

	CServerLogPrinter * pLog;



	LogFileName.Format("%s/Log/%s",(LPCTSTR)ModulePath,g_ProgramName);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_CONSOLE|CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(SERVER_LOG_CHANNEL,pLog);
	SAFE_RELEASE(pLog);

	SetConsoleLogLevel(CSystemConfig::GetInstance()->GetConsoleLogLevel());




	LogFileName.Format("%s/Log/DOSLib",(LPCTSTR)ModulePath);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_DOS_CHANNEL,pLog);
	SAFE_RELEASE(pLog);

	LogFileName.Format("%s/Log/DOSObjectStat",(LPCTSTR)ModulePath);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_DOS_OBJECT_STATE_CHANNEL,pLog);
	SAFE_RELEASE(pLog);

	LogFileName.Format("%s/Log/NetLib",(LPCTSTR)ModulePath);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_NET_CHANNEL,pLog);
	SAFE_RELEASE(pLog);

	LogFileName.Format("%s/Log/DBLib",(LPCTSTR)ModulePath);
	pLog=new CServerLogPrinter(this,CServerLogPrinter::LOM_FILE,
		CSystemConfig::GetInstance()->GetLogLevel(),LogFileName);
	CLogManager::GetInstance()->AddChannel(LOG_DB_ERROR_CHANNEL,pLog);
	SAFE_RELEASE(pLog);



	Log("��ʼ����������,��ǰ�汾:%u.%u.%u.%u",
		g_ProgramVersion[3],
		g_ProgramVersion[2],
		g_ProgramVersion[1],
		g_ProgramVersion[0]);





	m_ESVariableList.Create(128);
	m_ESFactionList.Create(128);
	CESFunctionLib::GetInstance()->AddFunction(&m_ESFactionList);
	m_ESThread.SetVariableList(&m_ESVariableList);
	m_ESThread.SetFunctionList(&m_ESFactionList);
	m_ESThread.SetScript(&m_Script);


	m_ESFactionList.AddCFunction("StartLog",3,this,&CDOSServerThread::StartLog);
	m_ESFactionList.AddCFunction("StopLog",2,this,&CDOSServerThread::StopLog);
	m_ESFactionList.AddCFunction("TestLog",1,this,&CDOSServerThread::TestLog);
	m_ESFactionList.AddCFunction("RebuildUDPControlPort",0,this,&CDOSServerThread::RebuildUDPControlPort);
	m_ESFactionList.AddCFunction("SetConsoleLogLevel",1,this,&CDOSServerThread::SFSetConsoleLogLevel);
	m_ESFactionList.AddCFunction("VerfyMemPool",0,this,&CDOSServerThread::SFVerfyMemPool);

	if(!CDOSServer::OnStart())
		return FALSE;

	//��ʼ��ϵͳ����
	m_pSysNetLinkManager=new CSystemNetLinkManager();
	m_pSysNetLinkManager->SetServerThread(this);

	xml_parser Parser;

	if(Parser.parse_file(MakeModuleFullPath(NULL,GetConfigFileName()),pug::parse_trim_attribute))
	{
		xml_node Config=Parser.document();
		if(Config.moveto_child("SystemLink"))
		{
			if(m_pSysNetLinkManager->Init(this,Config))
			{
				Log("��ʼ��ϵͳ���ӹ������ɹ�");
			}
			else
			{
				Log("��ʼ��ϵͳ���ӹ�����ʧ��");
			}
		}
		else
		{
			Log("���Ϸ���ϵͳ���������ļ�%s",GetConfigFileName());
		}

	}
	else
	{
		Log("δ�ҵ�ϵͳ���������ļ�%s",GetConfigFileName());
	}

	m_pUDPSystemControlPort=new CSystemControlPort();
	if(!m_pUDPSystemControlPort->Init(this))
	{
		Log("��ʼ��UDPϵͳ���ƶ˿�ʧ��");
	}


	m_ServerStatus.Create(SERVER_STATUS_BLOCK_SIZE);

	ULONG64_CONVERTER Version;
	memcpy(Version.Words,g_ProgramVersion,sizeof(ULONG64_CONVERTER));

	CEasyString Temp;

	SetServerStatus(SC_SST_SS_PROGRAM_VERSION,CSmartValue(Version.QuadPart));
	SetServerStatusFormat(SC_SST_SS_PROGRAM_VERSION,"�������汾",SSFT_VERSION);
	SetServerStatusFormat(SC_SST_SS_CYCLE_TIME,"ѭ��ʱ��(����)");
	SetServerStatusFormat(SC_SST_SS_CPU_COST,"CPUռ����",SSFT_PERCENT);
	SetServerStatusFormat(SC_SST_SS_TCP_RECV_FLOW,"TCP��������(Byte/S)",SSFT_FLOW);
	SetServerStatusFormat(SC_SST_SS_TCP_SEND_FLOW,"TCP��������(Byte/S)",SSFT_FLOW);
	SetServerStatusFormat(SC_SST_SS_TCP_RECV_COUNT,"TCP���մ���(��/S)");
	SetServerStatusFormat(SC_SST_SS_TCP_SEND_COUNT,"TCP���ʹ���(��/S)");
	SetServerStatusFormat(SST_SS_OBJECT_COUNT,"ϵͳ������");
	SetServerStatusFormat(SST_SS_ROUTE_IN_MSG_COUNT,"·��������Ϣ��");
	SetServerStatusFormat(SST_SS_ROUTE_IN_MSG_FLOW,"·��������Ϣ����",SSFT_FLOW);
	SetServerStatusFormat(SST_SS_ROUTE_OUT_MSG_COUNT,"·�������Ϣ��");
	SetServerStatusFormat(SST_SS_ROUTE_OUT_MSG_FLOW,"·�������Ϣ����",SSFT_FLOW);
	SetServerStatusFormat(SST_SS_ROUTE_CYCLE_TIME,"·��ѭ��ʱ��(MS)");
	SetServerStatusFormat(SST_SS_ROUTE_MSG_QUEUE_LEN,"·����Ϣ���г���");
	Temp.Format("·��(%u)CPUռ����",GetRouter()->GetThreadID());
	SetServerStatusFormat(SST_SS_ROUTE_CPU_USED_RATE,Temp,SSFT_PERCENT);


	CEasyString CSVLogHeader="CycleTime,CPUUsed,TCPRecvFlow,TCPSendFlow,TCPRecvCount,TCPSendCount,ObjectCount,"
		"RouteInMsgCount,RouteInMsgFlow,RouteOutMsgCount,RouteOutMsgFlow,RouteCycleTime,RouteMsgQueueLen,RouteCPUUsed";

	for (UINT i = 0; i < GetProxyManager()->GetProxyServiceCount(); i++)
	{
		CDOSObjectProxyService * pProxyService = GetProxyManager()->GetProxyService(i);
		if (pProxyService)
		{
			Temp.Format("�������(%u)CPUռ����", pProxyService->GetID());
			SetServerStatusFormat(SST_SS_OBJECT_PROXY_CPU_USED_RATE + i, Temp, SSFT_PERCENT);

			Temp.Format(",Proxy%dCPUUsed", pProxyService->GetID());
			CSVLogHeader += Temp;
		}
	}

	for(UINT i=0;i<GetObjectManager()->GetGroupCount();i++)
	{

		Temp.Format("������%dѭ��ʱ��(MS)",i);
		SetServerStatusFormat(SST_SS_GROUP_CYCLE_TIME+i,Temp);
		Temp.Format("������%d�������Ϣ����",i);
		SetServerStatusFormat(SST_SS_GROUP_MAX_OBJECT_MSG_QUEUE_LEN+i,Temp);
		Temp.Format("������%d(%u)CPUռ����",i,GetObjectManager()->GetGroup(i)->GetThreadID());
		SetServerStatusFormat(SST_SS_GROUP_CPU_USED_RATE+i,Temp,SSFT_PERCENT);
		Temp.Format(",Group%dCycleTime,Group%dMaxMsgQueueLen,Group%dCPUUsed",i,i,i);
		CSVLogHeader+=Temp;
	}

	LogFileName.Format("%s/Log/%s.Status",(LPCTSTR)ModulePath,g_ProgramName);
	CCSVFileLogPrinter * pCSVLog=new CCSVFileLogPrinter();
	pCSVLog->Init(CSystemConfig::GetInstance()->GetLogLevel(),LogFileName,CSVLogHeader);
	CLogManager::GetInstance()->AddChannel(SERVER_STATUS_LOG_CHANNEL,pCSVLog);
	SAFE_RELEASE(pCSVLog);


	m_ThreadPerformanceCounter.Init(GetThreadHandle(),SERVER_INFO_COUNT_TIME);

	Log("�������ɹ�����");

	FUNCTION_END;
	return TRUE;
}

void CDOSServerThread::OnBeginTerminate()
{

}

void CDOSServerThread::OnTerminate()
{
	FUNCTION_BEGIN;

	SAFE_RELEASE(m_pSysNetLinkManager);
	SAFE_RELEASE(m_pUDPSystemControlPort);
	CDOSServer::OnTerminate();
	Log("�������ر�");
	FUNCTION_END;
}

BOOL CDOSServerThread::OnRun()
{
	FUNCTION_BEGIN;

	if(!CBaseServer::OnRun())
		return FALSE;

	if(Update(CSystemConfig::GetInstance()->GetMainThreadProcessLimit())==0)
	{
		DoSleep(1);
	}

	m_ThreadPerformanceCounter.DoPerformanceCount();
	//���������ѭ��ʱ��
	if(m_CountTimer.IsTimeOut(SERVER_INFO_COUNT_TIME))
	{
		m_CountTimer.SaveTime();
		DoServerStat();
	}

	//ִ�п���̨����
	PANEL_MSG * pCommand=CControlPanel::GetInstance()->GetCommand();
	if(pCommand)
	{

		ExecCommand(pCommand->Msg);
		CControlPanel::GetInstance()->ReleaseCommand(pCommand->ID);
	}

	FUNCTION_END;
	return TRUE;
}
int CDOSServerThread::Update(int ProcessPacketLimit)
{
	FUNCTION_BEGIN;
	int Process=0;
	Process+=CDOSServer::Update(ProcessPacketLimit);
	Process+=m_pSysNetLinkManager->Update(ProcessPacketLimit);
	Process+=m_pUDPSystemControlPort->Update(ProcessPacketLimit);
	return Process;
	FUNCTION_END;
	return 0;
}

BOOL CDOSServerThread::OnTerminating()
{
	return FALSE;
}


int CDOSServerThread::GetClientCount()
{
	return 0;
}

LPCTSTR CDOSServerThread::GetConfigFileName()
{
	return SYSTEM_NET_LINK_CONFIG_FILE_NAME;
}

BOOL CDOSServerThread::PrintConsoleLog(int Level,LPCTSTR szLogMsg)
{
	FUNCTION_BEGIN;
	if(m_ConsoleLogLevel&Level)
	{
#ifdef WIN32
		CControlPanel::GetInstance()->PushMsg(szLogMsg);
#else
		printf("%s\n",szLogMsg);
#endif
		if(m_pSysNetLinkManager)
			m_pSysNetLinkManager->SendLogMsg(szLogMsg);
	}
	return TRUE;
	FUNCTION_END;
	return FALSE;
}

void CDOSServerThread::ExecCommand(LPCTSTR szCommand)
{
	FUNCTION_BEGIN;
	int RetCode;
	ES_BOLAN Result;

	Log("ִ������:%s",szCommand);

	RetCode=m_ESThread.PushScript(szCommand);
	if(RetCode)
	{
		Log("�����������:Line=%d,%s",
			m_ESThread.GetLastLine(),
			ESGetErrorMsg(RetCode));
	}
	RetCode=m_ScriptExecutor.ExecScript(m_ESThread);
	if(RetCode)
	{
		Log("�����������:Line=%d,%s",
			m_ESThread.GetLastLine(),
			ESGetErrorMsg(RetCode));
	}
	else
	{
		Log("ִ��������:%s",
			(LPCTSTR)BolanToString(m_ESThread.GetResult()));
	}
	FUNCTION_END;
}

BOOL CDOSServerThread::SetServerStatus(WORD StatusID,const CSmartValue& Value)
{
	FUNCTION_BEGIN;
	if(m_ServerStatus.IDToIndex(StatusID)==CSmartStruct::INVALID_MEMBER_ID)
	{
		m_ServerStatus.AddMember(StatusID,Value);
	}
	else
	{
		m_ServerStatus.GetMember(StatusID).SetValue(Value);
	}
	FUNCTION_END;
	return FALSE;
}

void CDOSServerThread::SetServerStatusFormat(WORD StatusID,LPCTSTR szStatusName,int FormatType)
{
	FUNCTION_BEGIN;
	CControlPanel::GetInstance()->SetServerStatusFormat(StatusID,szStatusName,FormatType);
	FUNCTION_END;
}

void CDOSServerThread::QueryShowDown()
{
	FUNCTION_BEGIN;
	Log("������׼���ر�");
	Terminate();
	FUNCTION_END;
}

bool CDOSServerThread::IsServerTerminated()
{
	FUNCTION_BEGIN;
	return IsTerminated()!=FALSE;
	FUNCTION_END;
	return true;
}

int CDOSServerThread::StartLog(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	CServerLogPrinter * pLog=NULL;
	if(_stricmp(pParams[0].StrValue,"Normal")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(SERVER_LOG_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"Status")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(SERVER_STATUS_LOG_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"DB")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(LOG_DB_ERROR_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"Net")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(LOG_NET_CHANNEL);
	}
	if(pLog)
	{
		int WitchMode=0;
		if(_stricmp(pParams[1].StrValue,"VS")==0)
		{
			WitchMode=CServerLogPrinter::LOM_VS;
		}
		else if(_stricmp(pParams[1].StrValue,"Console")==0)
		{
			WitchMode=CServerLogPrinter::LOM_CONSOLE;
		}
		else if(_stricmp(pParams[1].StrValue,"File")==0)
		{
			WitchMode=CServerLogPrinter::LOM_FILE;
		}
		if(WitchMode)
		{
			UINT Mode=pLog->GetLogMode();
			Mode|=WitchMode;

			int Level=pLog->GetLogLevel();

			CEasyString LogFileName;
			if(pParams[2].StrValue.IsEmpty())
			{
				LogFileName=pLog->GetLogFileName();
			}
			else
			{
				CEasyString ModulePath=GetModulePath(NULL);
				LogFileName.Format("%s/Log/%s",(LPCTSTR)ModulePath,(LPCTSTR)pParams[2].StrValue);
			}


			pLog->SetLogMode(Mode,Level,LogFileName);
			Log("Logģ��[%s],ģʽ[%s]������ѱ�����",
				(LPCTSTR)(pParams[0].StrValue),
				(LPCTSTR)(pParams[1].StrValue));
		}
	}
	FUNCTION_END;
	return 0;
}

int CDOSServerThread::StopLog(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	CServerLogPrinter * pLog=NULL;
	if(_stricmp(pParams[0].StrValue,"Normal")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(SERVER_LOG_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"Status")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(SERVER_STATUS_LOG_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"DB")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(LOG_DB_ERROR_CHANNEL);
	}
	else if(_stricmp(pParams[0].StrValue,"Net")==0)
	{
		pLog=(CServerLogPrinter *)CLogManager::GetInstance()->
			GetChannel(LOG_NET_CHANNEL);
	}
	if(pLog)
	{
		int WitchMode=0;
		if(_stricmp(pParams[1].StrValue,"VS")==0)
		{
			WitchMode=CServerLogPrinter::LOM_VS;
		}
		else if(_stricmp(pParams[1].StrValue,"Console")==0)
		{
			WitchMode=CServerLogPrinter::LOM_CONSOLE;
		}
		else if(_stricmp(pParams[1].StrValue,"File")==0)
		{
			WitchMode=CServerLogPrinter::LOM_FILE;
		}
		if(WitchMode)
		{
			UINT Mode=pLog->GetLogMode();
			Mode&=~WitchMode;
			int Level=pLog->GetLogLevel();
			pLog->SetLogMode(Mode,Level,NULL);
			Log("Logģ��[%s],ģʽ[%s]������ѱ��ر�",
				(LPCTSTR)(pParams[0].StrValue),
				(LPCTSTR)(pParams[1].StrValue));
		}
	}
	FUNCTION_END;
	return 0;
}

int CDOSServerThread::TestLog(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	CServerLogPrinter * pLog=NULL;
	if(_stricmp(pParams[0].StrValue,"Normal")==0)
	{
		Log("Normal");
	}
	else if(_stricmp(pParams[0].StrValue,"Debug")==0)
	{
		LogDebug("Debug");
	}
	

	FUNCTION_END;
	return 0;
}


int CDOSServerThread::RebuildUDPControlPort(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	FUNCTION_BEGIN;
	if(!m_pUDPSystemControlPort->Init(this))
	{
		Log("�ؽ�UDPϵͳ���ƶ˿�ʧ��");
	}
	else
	{
		Log("�ؽ�UDPϵͳ���ƶ˿ڳɹ�");
	}
	FUNCTION_END;
	return 0;
}

int CDOSServerThread::SFSetConsoleLogLevel(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	FUNCTION_BEGIN;

	SetConsoleLogLevel(pParams[0]);
	Log("����̨Log����ȼ�����Ϊ%d",GetConsoleLogLevel());

	FUNCTION_END;
	return 0;
}

int CDOSServerThread::SFVerfyMemPool(CESThread * pESThread,ES_BOLAN* pResult,ES_BOLAN* pParams,int ParamCount)
{
	FUNCTION_BEGIN;

	((CDOSServer *)GetServer())->GetMemoryPool()->Verfy(SERVER_LOG_CHANNEL);

	FUNCTION_END;
	return 0;
}

void CDOSServerThread::DoServerStat()
{
	FUNCTION_BEGIN;
	int ClientCount=GetClientCount();
	float CycleTime=m_ThreadPerformanceCounter.GetCycleTime();
	float CPUCost=m_ThreadPerformanceCounter.GetCPUUsedRate();
	float TCPRecvFlow=(float)m_TCPRecvBytes*1000/SERVER_INFO_COUNT_TIME;
	float TCPSendFlow=(float)m_TCPSendBytes*1000/SERVER_INFO_COUNT_TIME;

	float TCPRecvCount=(float)m_TCPRecvCount*1000/SERVER_INFO_COUNT_TIME;
	float TCPSendCount=(float)m_TCPSendCount*1000/SERVER_INFO_COUNT_TIME;

	UINT ObjectCount=GetObjectManager()->GetObejctCount();

	float RouteInMsgCount=(float)GetRouter()->GetInMsgCount()*1000/SERVER_INFO_COUNT_TIME;
	float RouteInMsgFlow=(float)GetRouter()->GetInMsgFlow()*1000/SERVER_INFO_COUNT_TIME;
	float RouteOutMsgCount=(float)GetRouter()->GetOutMsgCount()*1000/SERVER_INFO_COUNT_TIME;
	float RouteOutMsgFlow=(float)GetRouter()->GetOutMsgFlow()*1000/SERVER_INFO_COUNT_TIME;
	GetRouter()->ResetStatData();

	SetServerStatus(SC_SST_SS_CYCLE_TIME,CSmartValue(CycleTime));
	SetServerStatus(SC_SST_SS_CPU_COST,CSmartValue(CPUCost));
	SetServerStatus(SC_SST_SS_TCP_RECV_FLOW,CSmartValue(TCPRecvFlow));
	SetServerStatus(SC_SST_SS_TCP_SEND_FLOW,CSmartValue(TCPSendFlow));
	SetServerStatus(SC_SST_SS_TCP_RECV_COUNT,CSmartValue(TCPRecvCount));
	SetServerStatus(SC_SST_SS_TCP_SEND_COUNT,CSmartValue(TCPSendCount));
	SetServerStatus(SST_SS_OBJECT_COUNT,CSmartValue(ObjectCount));
	SetServerStatus(SST_SS_ROUTE_IN_MSG_COUNT,CSmartValue(RouteInMsgCount));
	SetServerStatus(SST_SS_ROUTE_IN_MSG_FLOW,CSmartValue(RouteInMsgFlow));
	SetServerStatus(SST_SS_ROUTE_OUT_MSG_COUNT,CSmartValue(RouteOutMsgCount));
	SetServerStatus(SST_SS_ROUTE_OUT_MSG_FLOW,CSmartValue(RouteOutMsgFlow));
	SetServerStatus(SST_SS_ROUTE_CYCLE_TIME,CSmartValue(GetRouter()->GetCycleTime()));
	SetServerStatus(SST_SS_ROUTE_MSG_QUEUE_LEN,CSmartValue(GetRouter()->GetMsgQueueLen()));
	SetServerStatus(SST_SS_ROUTE_CPU_USED_RATE,CSmartValue(GetRouter()->GetCPUUsedRate()));


	CEasyString ProxyStatData;

	for (UINT i = 0; i < GetProxyManager()->GetProxyServiceCount(); i++)
	{
		CEasyString Temp;
		CDOSObjectProxyService * pProxyService = GetProxyManager()->GetProxyService(i);
		if (pProxyService)
		{
			SetServerStatus(SST_SS_OBJECT_PROXY_CPU_USED_RATE + i, CSmartValue(pProxyService->GetCPUUsedRate()));
			Temp.Format(",%g", pProxyService->GetCPUUsedRate());
		}
	}


	CEasyString GroupStatData;

	for(UINT i=0;i<GetObjectManager()->GetGroupCount();i++)
	{
		CEasyString Temp;
		CDOSObjectGroup * pGroup=GetObjectManager()->GetGroup(i);
		if(pGroup)
		{
			SetServerStatus(SST_SS_GROUP_CYCLE_TIME+i,CSmartValue(pGroup->GetCycleTime()));
			SetServerStatus(SST_SS_GROUP_MAX_OBJECT_MSG_QUEUE_LEN+i,CSmartValue(pGroup->GetMaxObjectMsgQueueLen()));
			SetServerStatus(SST_SS_GROUP_CPU_USED_RATE+i,CSmartValue(pGroup->GetCPUUsedRate()));
			Temp.Format(",%g,%d,%g",pGroup->GetCycleTime(),pGroup->GetMaxObjectMsgQueueLen(),pGroup->GetCPUUsedRate());
		}
		else
		{
			Temp=",0,0,0";
		}
		GroupStatData+=Temp;
	}

	CControlPanel::GetInstance()->SetServerStatus(m_ServerStatus.GetData(),m_ServerStatus.GetDataLen());

	CLogManager::GetInstance()->PrintLog(SERVER_STATUS_LOG_CHANNEL,ILogPrinter::LOG_LEVEL_NORMAL,0,
		"%g,%g,%s,%s,%g,%g,%u,%s,%s,%s,%s,%g,%u,%g%s%s",
		CycleTime,
		CPUCost,
		(LPCTSTR)FormatNumberWordsFloat(TCPRecvFlow,true),
		(LPCTSTR)FormatNumberWordsFloat(TCPSendFlow,true),
		TCPRecvCount,
		TCPSendCount,
		ObjectCount,
		(LPCTSTR)FormatNumberWordsFloat(RouteInMsgCount,true),
		(LPCTSTR)FormatNumberWordsFloat(RouteInMsgFlow,true),
		(LPCTSTR)FormatNumberWordsFloat(RouteOutMsgCount,true),
		(LPCTSTR)FormatNumberWordsFloat(RouteOutMsgFlow,true),
		GetRouter()->GetCycleTime(),
		GetRouter()->GetMsgQueueLen(),
		GetRouter()->GetCPUUsedRate(),
		(LPCTSTR)ProxyStatData,
		(LPCTSTR)GroupStatData);

	ResetFluxStat();

	if(CSystemConfig::GetInstance()->IsLogServerObjectUse())
	{
		PrintObjectStatus();
	}
	PrintDOSObjectStatLog(0,"================================================================");
	PrintDOSObjectStatLog(0,"��ʼͳ�ƶ���ʹ�����");
	PrintDOSObjectStatLog(0,"================================================================");
	GetObjectManager()->PrintGroupInfo(LOG_DOS_OBJECT_STATE_CHANNEL);

	UINT AllocCount=((CDOSServer *)GetServer())->GetMemoryPool()->GetAllocCount();
	UINT FreeCount=((CDOSServer *)GetServer())->GetMemoryPool()->GetFreeCount();
	UINT SystemAllocCount=((CDOSServer *)GetServer())->GetMemoryPool()->GetSystemAllocCount();
	UINT SystemFreeCount=((CDOSServer *)GetServer())->GetMemoryPool()->GetSystemFreeCount();

	PrintDOSObjectStatLog(0,"�ڴ�ط�����:%u",AllocCount);
	PrintDOSObjectStatLog(0,"�ڴ���ͷ���:%u,%u",FreeCount,AllocCount-FreeCount);
	PrintDOSObjectStatLog(0,"ϵͳ�ڴ������:%u",SystemAllocCount);
	PrintDOSObjectStatLog(0,"ϵͳ�ڴ��ͷ���:%u,%u",SystemFreeCount,SystemAllocCount-SystemFreeCount);
	FUNCTION_END;
}
