﻿/****************************************************************************/
/*                                                                          */
/*      文件名:    SystemControlPort.cpp                                    */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "stdafx.h"

CSystemControlPort::CSystemControlPort(void)
{
	m_pServer=NULL;
}

CSystemControlPort::~CSystemControlPort(void)
{
}

bool CSystemControlPort::Init(CBaseServer * pServer)
{
	if(pServer)
	{
		m_pServer=pServer;
		SetServer(m_pServer->GetServer());
	}

	if(CSystemConfig::GetInstance()->GetUDPControlAddress().GetPort())
	{
		Create(IPPROTO_UDP);
		if(!StartListen(CSystemConfig::GetInstance()->GetUDPControlAddress()))
		{
			Log("SystemControlPort侦听失败%s:%u",
				CSystemConfig::GetInstance()->GetUDPControlAddress().GetIPString(),
				CSystemConfig::GetInstance()->GetUDPControlAddress().GetPort());
			return false;
		}
		Log("SystemControlPort成功侦听于%s:%u",
			CSystemConfig::GetInstance()->GetUDPControlAddress().GetIPString(),
			CSystemConfig::GetInstance()->GetUDPControlAddress().GetPort());
	}
	else
	{
		Log("UDP控制端口未启用");
	}
	return true;
}

void CSystemControlPort::OnStartUp()
{

}

void CSystemControlPort::OnClose()
{

}

void CSystemControlPort::OnRecvData(const CIPAddress& IPAddress, const BYTE * pData, UINT DataSize)
{

	if(m_pServer==NULL)
		return;

	if (DataSize<sizeof(SMSG_HEADER))
		return;

	SMSG * pMsg = (SMSG *)pData;
	DataSize = DataSize - sizeof(SMSG_HEADER);

	switch(pMsg->Header.MsgID)
	{
	case SC_MSG_QUERY_SERVER_STATUS:
		{
			CSmartStruct QueryInfo(pMsg->Data,DataSize,false);
			CSmartStruct ServerStatus(SERVER_STATUS_BLOCK_SIZE);
			for(int i=0;i<QueryInfo.GetMemberCount();i++)
			{
				if(QueryInfo.IndexToID(i)==SC_SST_QSS_STATUS_ID)
				{
					WORD StatusID=QueryInfo.GetMemberByIndex(i);
					ServerStatus.AddMember(StatusID,
						m_pServer->GetServerStatus(StatusID));
				}
			}
			SendMsg(IPAddress,SC_MSG_QUERY_SERVER_STATUS_RESULT,
				ServerStatus.GetData(),ServerStatus.GetDataLen());
		}
		break;
	};
}

void CSystemControlPort::SendMsg(const CIPAddress& Address,WORD Msg,LPCVOID pData,int DataLen)
{

	static char s_SendBuffer[65536];



	CEasyBuffer	SendBuffer(s_SendBuffer,65536);
	SMSG_HEADER MsgHeader;

	MsgHeader.MsgID=Msg;
	MsgHeader.Size=(WORD)(sizeof(SMSG_HEADER)+DataLen);
	SendBuffer.PushBack(&MsgHeader,sizeof(SMSG_HEADER));

	if(DataLen)
		SendBuffer.PushBack(pData,DataLen);
	QueryUDPSend(Address,SendBuffer.GetBuffer(),(UINT)SendBuffer.GetUsedSize());


}
