#include "stdafx.h"



CWebSocketProxyConnection::CWebSocketProxyConnection()
{
	m_WebSocketStatus = WEB_SOCKET_STATUS_NONE;
}


CWebSocketProxyConnection::~CWebSocketProxyConnection()
{
}
void CWebSocketProxyConnection::OnConnection(bool IsSucceed)
{
	m_NeedDelayClose = false;
	m_KeepAliveTimer.SaveTime();
	m_KeepAliveCount = 0;
	m_RecentPingDelay = 0;
	m_RecvCount = 0;
	m_RecvFlow = 0;

	if (IsSucceed)
	{
		PrintDOSDebugLog(_T("�յ������������ӣ�IP=%s"), GetRemoteAddress().GetIPString());
		m_Status = STATUS_CONNECTED;
		m_pService->AcceptConnection(this);

		//SendProtocolOption();
	}
	else
	{
		PrintDOSLog(_T("���ӳ�ʼ��ʧ�ܣ�IP=%s"), GetRemoteAddress().GetIPString());
		m_Status = STATUS_DISCONNECTED;
		m_pService->QueryDestoryConnection(this);
	}
}

bool CWebSocketProxyConnection::OnMessage(MSG_ID_TYPE MsgID, WORD MsgFlag, const void* pData, MSG_LEN_TYPE DataLen)
{
	m_TotalMsgSendCount++;

	if ((m_Config.MsgCompressType != MSG_COMPRESS_NONE) && ((MsgFlag & DOS_MESSAGE_FLAG_NO_COMPRESS) == 0))
	{
		//ѹ��
		LPCVOID pNewData = CompressMsg(pData, DataLen);
		if (pNewData == NULL)
		{
			PrintDOSLog(_T("��Ϣ0x%Xѹ��ʧ��"), MsgID);
			return false;
		}
		if (pNewData != pData)
		{
			MsgFlag |= DOS_MESSAGE_FLAG_COMPRESSED;
			pData = pNewData;
		}
	}
	return SendWebSocketFrame(WEB_SOCKET_OP_CODE_BINARY_DATA, MsgID, MsgFlag, 0, pData, DataLen);
}

void CWebSocketProxyConnection::OnRecvData(const BYTE * pData, UINT DataSize)
{
	m_KeepAliveCount = 0;
	m_KeepAliveTimer.SaveTime();
	switch (m_WebSocketStatus)
	{
	case WEB_SOCKET_STATUS_NONE:
		ProcessHTTPMsg(pData, DataSize);
		break;
	case WEB_SOCKET_STATUS_ACCEPTED:
		ProcessWebSocketData(pData, DataSize);
		break;
	}
}


void CWebSocketProxyConnection::OnKeepAliveMsg(UINT Timestamp, UINT RecentDelay)
{

}
void CWebSocketProxyConnection::OnKeepAliveAckMsg(UINT Timestamp)
{

}
void CWebSocketProxyConnection::SendKeepAliveMsg()
{
	SendWebSocketPingMsg();
}
void CWebSocketProxyConnection::SendProtocolOption()
{
	PROTOCOL_OPTION Data;
	Data.Flag = 0;
	if (m_Config.MsgEnCryptType != MSG_ENCRYPT_NONE)
		Data.Flag |= PROTOCOL_OPTION_FLAG_UP_MSG_USE_ENCRYPT;

	SendWebSocketFrame(WEB_SOCKET_OP_CODE_BINARY_DATA, DSM_PROTOCOL_OPTION, DOS_MESSAGE_FLAG_SYSTEM_MESSAGE,0, &Data, sizeof(PROTOCOL_OPTION));

	PrintDOSDebugLog(_T("�ѷ���Э������0x%X"), Data.Flag);
}




void CWebSocketProxyConnection::ProcessHTTPMsg(const BYTE * pData, UINT DataSize)
{
	UINT Ptr = 0;
	HTTP_REQUEST_PARSE_STATUS ParseStatus = HRPS_NONE;
	while (Ptr < DataSize)
	{
		switch (ParseStatus)
		{
		case HRPS_NONE:
			if (pData[Ptr] == '\r')
				ParseStatus = HRPS_RETURN1;
			break;
		case HRPS_RETURN1:
			if (pData[Ptr] == '\n')
				ParseStatus = HRPS_NEW_LINE1;
			else
				ParseStatus = HRPS_NONE;
			break;
		case HRPS_NEW_LINE1:
			if (pData[Ptr] == '\r')
				ParseStatus = HRPS_RETURN2;
			else
				ParseStatus = HRPS_NONE;
			break;
		case HRPS_RETURN2:
			if (pData[Ptr] == '\n')
			{
				//���������Ѿ�����
				((BYTE *)pData)[Ptr] = 0;
				((BYTE *)pData)[Ptr - 1] = 0;
				OnHTTPRequest((const char *)pData);
				break;
			}
			else
			{
				ParseStatus = HRPS_NONE;
			}
			break;
		}
		Ptr++;
	}
}

void CWebSocketProxyConnection::OnHTTPRequest(const char * szRequest)
{
	PrintDOSLog(_T("%s"), szRequest);

	CEasyArray<char *> Lines;
	ParseStringLines((char *)szRequest, Lines);
	CEasyString GetField;
	CEasyString UpgradeField;
	CEasyString ConnectionField;
	CEasyString SecWebSocketKeyField;
	CEasyString SecWebSocketVersionField;
	CEasyString SecWebSocketExtensionsField;
	CEasyString HostField;
	CEasyString OriginField;
	for (UINT i = 0; i < Lines.GetCount(); i++)
	{
		if (strncmp(Lines[i], "GET ", 4) == 0)
		{
			GetField = Lines[i] + 4;
		}
		else if (strncmp(Lines[i], "Upgrade:", 8) == 0)
		{
			UpgradeField = Lines[i] + 8;
			UpgradeField.Trim();
		}
		else if (strncmp(Lines[i], "Connection:", 11) == 0)
		{
			ConnectionField = Lines[i] + 11;
			ConnectionField.Trim();
		}
		else if (strncmp(Lines[i], "Sec-WebSocket-Key:", 18) == 0)
		{
			SecWebSocketKeyField = Lines[i] + 18;
			SecWebSocketKeyField.Trim();
		}
		else if (strncmp(Lines[i], "Sec-WebSocket-Version:", 18) == 0)
		{
			SecWebSocketVersionField = Lines[i] + 22;
			SecWebSocketVersionField.Trim();
		}
		else if (strncmp(Lines[i], "Sec-WebSocket-Extensions:", 25) == 0)
		{
			SecWebSocketExtensionsField = Lines[i] + 25;
			SecWebSocketExtensionsField.Trim();
		}
		else if (strncmp(Lines[i], "Host:", 5) == 0)
		{
			HostField = Lines[i] + 5;
			HostField.Trim();
		}
		else if (strncmp(Lines[i], "Origin:", 7) == 0)
		{
			OriginField = Lines[i] + 7;
			OriginField.Trim();
		}
	}
	if (!GetField.IsEmpty() && UpgradeField.CompareNoCase("websocket") == 0 && ConnectionField.CompareNoCase("Upgrade") == 0 &&
		SecWebSocketVersionField == "13" && !SecWebSocketKeyField.IsEmpty())
	{
		SecWebSocketKeyField = SecWebSocketKeyField + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		CSHA1 sha1;
		BYTE HashBin[20];

		sha1.Update((BYTE*)SecWebSocketKeyField.GetBuffer(), SecWebSocketKeyField.GetLength());
		sha1.Final();
		sha1.GetHash(HashBin);
		CEasyString HashStr = CBase64::Encode(HashBin, 20);
		SendHTTPRespond(101, HashStr);
		m_WebSocketStatus = WEB_SOCKET_STATUS_ACCEPTED;
	}
	else
	{
		SendHTTPRespond(403, "Not Support");
	}
}

void CWebSocketProxyConnection::SendHTTPRespond(int Code, LPCSTR szContent)
{
	char Buff[4096];
	switch (Code)
	{
	case 101:
		sprintf_s(Buff, 4096, "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n",
			szContent);
		break;
	case 403:
		sprintf_s(Buff, 4096, "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: %u\r\n\r\n%s",
			strlen(szContent), szContent);
		break;
	default:
		sprintf_s(Buff, 4096, "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: %u\r\n\r\n%s",
			strlen(szContent), szContent);
		break;
	}
	Send(Buff, strlen(Buff));
	if (Code != 101)
	{
		QueryDisconnect(1000);
	}
}

void CWebSocketProxyConnection::ParseStringLines(char * szStr, CEasyArray<char *>& StrLines)
{
	char * szLine = szStr;
	while (*szStr)
	{
		if (*szStr == '\r' || *szStr == '\n')
		{
			*szStr = 0;
			if (szStr - szLine > 1)
				StrLines.Add(szLine);
			szLine = szStr + 1;
		}
		szStr++;
	}
	if (szStr - szLine > 1)
		StrLines.Add(szLine);
}

void CWebSocketProxyConnection::ProcessWebSocketData(const BYTE * pData, UINT DataSize)
{
	if (m_AssembleBuffer.PushBack(pData, DataSize))
	{
		while (m_AssembleBuffer.GetUsedSize() >= 2)
		{
			BYTE Header1, Header2;
			size_t Pos = 0;
			m_AssembleBuffer.Peek(Pos, &Header1, sizeof(Header1));
			m_AssembleBuffer.Peek(Pos, &Header2, sizeof(Header2));
			BYTE OPCode = Header1 & 0x0F;
			BYTE ReserveCode = (Header1 >> 4) & 0x07;
			bool IsFinFrame = (Header1 & 0x80) != 0;
			bool HaveMask = (Header2 & 0x80) != 0;
			UINT64 DataLen = Header2 & 0x7F;
			if (DataLen == 126)
			{
				m_AssembleBuffer.Peek(Pos, &DataLen, sizeof(WORD));
				DataLen = ntohs(DataLen);
			}
			else if (DataLen == 127)
			{
				m_AssembleBuffer.Peek(Pos, &DataLen, sizeof(UINT64));
				DataLen = __ntohll(DataLen);
			}
			BYTE MaskKey[4];
			if (HaveMask)
			{
				m_AssembleBuffer.Peek(Pos, MaskKey, sizeof(BYTE) * 4);
			}
			if (m_AssembleBuffer.GetUsedSize() >= Pos + DataLen)
			{
				OnWebSocketFrame(OPCode, IsFinFrame, HaveMask, MaskKey, ((BYTE*)m_AssembleBuffer.GetBuffer()) + Pos, DataLen);
				m_AssembleBuffer.PopFront(NULL, Pos + DataLen);
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		Disconnect();
		PrintDOSLog(_T("DOSLib"), _T("�������(%d)ƴ��������������ӶϿ���"), GetID());
	}
}

void CWebSocketProxyConnection::OnWebSocketFrame(BYTE OPCode, bool IsFinFrame, bool HaveMask, BYTE * MaskKey, BYTE* pData, UINT DataLen)
{
	//PrintDOSDebugLog("Frame:%u %u %s %s Mask=%02X %02X %02X %02X", 
	//	OPCode, DataLen, IsFinFrame ? "true" : "false", HaveMask ? "true" : "false", MaskKey[0], MaskKey[1], MaskKey[2], MaskKey[3]);
	switch (OPCode)
	{
	case WEB_SOCKET_OP_CODE_CONTINUOUS_DATA:
	case WEB_SOCKET_OP_CODE_TEXT_DATA:
	case WEB_SOCKET_OP_CODE_BINARY_DATA:
		{
			if (HaveMask)
			{
				for (UINT i = 0; i < DataLen; i++)
				{
					pData[i] = pData[i] ^ MaskKey[i % 4];
				}
			}
			if (DataLen >= sizeof(WS_MESSAGE_HEAD))
			{
				WS_MESSAGE_HEAD* pHeader = (WS_MESSAGE_HEAD*)pData;
				ProcessClientMsg(pHeader->MsgID, pHeader->MsgFlag, pHeader->CRC, pData + sizeof(WS_MESSAGE_HEAD), DataLen - sizeof(WS_MESSAGE_HEAD));
			}
		}
		break;
	case WEB_SOCKET_OP_CODE_CLOSE:
		{
			SendWebSocketCloseMsg();
			QueryDisconnect(1000);
		}
		break;
	case WEB_SOCKET_OP_CODE_KEEP_ALIVE_PING:
		{
			m_KeepAliveCount = 0;
			m_KeepAliveTimer.SaveTime();
			SendWebSocketPongMsg();
		}
		break;
	case WEB_SOCKET_OP_CODE_KEEP_ALIVE_PONG:
	
		break;
	default:
		PrintDOSLog(_T("DOSLib"), _T("�������(%d)δ֪��WebSocket��OPCode(%d)�����ӶϿ���"), GetID(), OPCode);
		Disconnect();
	}
}

void CWebSocketProxyConnection::SendWebSocketCloseMsg()
{
	static BYTE CloseFrame[] = { WEB_SOCKET_OP_CODE_CLOSE | 0x80, 0 };
	if (m_WebSocketStatus == WEB_SOCKET_STATUS_ACCEPTED)
		Send(CloseFrame, sizeof(CloseFrame));
}
void CWebSocketProxyConnection::SendWebSocketPingMsg()
{
	static BYTE PingFrame[] = { WEB_SOCKET_OP_CODE_KEEP_ALIVE_PING | 0x80, 0 };
	if (m_WebSocketStatus == WEB_SOCKET_STATUS_ACCEPTED)
		Send(PingFrame, sizeof(PingFrame));
}
void CWebSocketProxyConnection::SendWebSocketPongMsg()
{
	static BYTE Frame[] = { WEB_SOCKET_OP_CODE_KEEP_ALIVE_PONG | 0x80, 0 };
	if (m_WebSocketStatus == WEB_SOCKET_STATUS_ACCEPTED)
		Send(Frame, sizeof(Frame));
}


bool CWebSocketProxyConnection::SendWebSocketFrame(WEB_SOCKET_OP_CODE OPCode, MSG_ID_TYPE MsgID, WORD MsgFlag, WORD CRC, const void* pData, UINT DataLen)
{
	BYTE FrameHeader[10];
	UINT HeaderSize = 2;
	FrameHeader[0] = OPCode | 0x80;
	UINT MsgLen = DataLen + sizeof(WS_MESSAGE_HEAD);
	if (MsgLen < 126)
	{
		FrameHeader[1] = MsgLen;
	}
	else if (MsgLen < 0x10000)
	{
		FrameHeader[1] = 126;
		WORD Len = htons(MsgLen);
		FrameHeader[2] = Len & 0xFF;
		FrameHeader[3] = (Len >> 8) & 0xFF;
		HeaderSize += 2;
	}
	else
	{
		FrameHeader[1] = 127;
		UINT64 Len = __htonll(MsgLen);
		FrameHeader[2] = Len & 0xFF;
		FrameHeader[3] = (Len >> 8) & 0xFF;
		FrameHeader[4] = (Len >> 16) & 0xFF;
		FrameHeader[5] = (Len >> 24) & 0xFF;
		FrameHeader[6] = (Len >> 32) & 0xFF;
		FrameHeader[7] = (Len >> 40) & 0xFF;
		FrameHeader[8] = (Len >> 48) & 0xFF;
		FrameHeader[9] = (Len >> 56) & 0xFF;
		HeaderSize += 8;
	}

	WS_MESSAGE_HEAD MsgHeader;
	MsgHeader.MsgID = MsgID;
	MsgHeader.MsgFlag = MsgFlag;
	MsgHeader.CRC = CRC;

	LPCVOID DataBuffers[3];
	UINT DataSizes[3];

	DataBuffers[0] = FrameHeader;
	DataSizes[0] = HeaderSize;
	DataBuffers[1] = &MsgHeader;
	DataSizes[1] = sizeof(MsgHeader);
	DataBuffers[2] = pData;
	DataSizes[2] = DataLen;

	
	if (pData && DataLen)
		return SendMulti(DataBuffers, DataSizes, 3);
	else
		return SendMulti(DataBuffers, DataSizes, 2);
}