#include "stdafx.h"



CTRConnection::CTRConnection()
{
	m_pParent = NULL;
	m_SearchID = 0;
	m_AssembleBuffer.Create(65536);
	m_SendBuffer.Create(4096);
	m_FinishHandshake = 0;
	m_MetaDataSize = 0;
	m_CurMetaDataPiece = 0;
}


CTRConnection::~CTRConnection()
{
}

bool CTRConnection::Init(CDHTService * pParent, UINT SearchID, const NODE_ID& PeerID, const NODE_ID& InfoHash, const CIPAddress& Address)
{
	m_pParent = pParent;
	m_SearchID = SearchID;
	m_PeerID = PeerID;
	m_InfoHash = InfoHash;	
	m_AssembleBuffer.SetUsedSize(0);
	m_SendBuffer.SetUsedSize(0);
	m_FinishHandshake = 0;
	m_MetaDataMsgID = 0;
	m_MetaDataSize = 0;
	m_CurMetaDataPiece = 0;
	m_LogTag.Format(_T("DHT(%d)"), GetID());
	PrintNetDebugLog(m_LogTag, _T("��������%s"), Address.GetAddressString());
	SetServer(pParent->GetServer());
	if (Connect(Address,20000))
		return true;
	return false;
}

void CTRConnection::OnConnection(BOOL IsSucceed)
{
	if (IsSucceed)
	{
		PrintNetLog(m_LogTag, _T("����%s�ɹ�����������1"), GetRemoteAddress().GetAddressString());
		SendHandshake1();
	}
	else
	{
		PrintNetLog(m_LogTag, _T("����%sʧ��"), GetRemoteAddress().GetAddressString());
	}
}
void CTRConnection::OnDisconnection()
{
	PrintNetLog(m_LogTag, _T("����%s�Ͽ�"), GetRemoteAddress().GetAddressString());
}

void CTRConnection::OnRecvData(const BYTE * pData, UINT DataSize)
{
	m_AssembleBuffer.PushBack(pData, DataSize);
	if (m_FinishHandshake == 0)
	{
		if (m_AssembleBuffer.GetUsedSize() >= 68)
		{
			if (memcmp(((BYTE *)m_AssembleBuffer.GetBuffer()) + 1, "BitTorrent protocol", 19) == 0)
			{
				BYTE * pInfoHash = (BYTE *)m_AssembleBuffer.GetBuffer() + 28;
				BYTE * pPeerID = (BYTE *)m_AssembleBuffer.GetBuffer() + 48;
				memcpy(m_RemotePeerID.NodeID, pPeerID, NODE_ID_BYTE_COUNT);
				m_AssembleBuffer.PopFront(NULL, 68);				
				CEasyString PeerStr((char *)pPeerID, NODE_ID_BYTE_COUNT);
				PrintNetDebugLog(m_LogTag, _T("�յ�����1��Ӧ:%s"), (LPCTSTR)PeerStr);
				m_FinishHandshake = 1;
				SendHandshake2();
			}
		}
	}	
	else
	{
		
		while (m_AssembleBuffer.GetUsedSize() >= sizeof(UINT))
		{
			UINT MsgSize = 0;
			UINT PeekPtr = 0;
			m_AssembleBuffer.Peek(PeekPtr, &MsgSize, sizeof(MsgSize));
			MsgSize = ntohl(MsgSize);
			if (m_AssembleBuffer.GetUsedSize() >= MsgSize + sizeof(UINT))
			{
				BYTE MsgID, ExtMsgID;
				m_AssembleBuffer.Peek(PeekPtr, &MsgID, sizeof(MsgID));
				m_AssembleBuffer.Peek(PeekPtr, &ExtMsgID, sizeof(ExtMsgID));
				if (MsgID == 20)
				{
					OnMsg(((BYTE *)m_AssembleBuffer.GetBuffer()) + sizeof(UINT) + 2, MsgSize - 2);
				}		
				else
				{
					PrintNetDebugLog(m_LogTag, _T("����δ֪��ϢMsgID=%u,ExtMsgID=%u,Size=%u"), MsgID, ExtMsgID, MsgSize);
				}
				m_AssembleBuffer.PopFront(NULL, MsgSize + sizeof(UINT));
			}
			else
			{
				break;
			}
		}
	}
}
bool CTRConnection::SendHandshake1()
{
	char Buff[68];

	ZeroMemory(Buff, 68);
	Buff[0] = 19;
	memcpy(Buff + 1, "BitTorrent protocol", 19);
	Buff[25] = 0x10;
	memcpy(Buff + 28, m_InfoHash.NodeID, 20);
	memcpy(Buff + 48, m_PeerID.NodeID, 20);
	return Send(Buff, 68) != FALSE;
}
bool CTRConnection::SendHandshake2()
{
	char * pBuff = (char *)m_SendBuffer.GetBuffer();
	UINT BuffSize = m_SendBuffer.GetBufferSize();
	UINT BuffPtr = 0;

	UINT * MsgSize = (UINT *)(pBuff + BuffPtr);
	BuffPtr += sizeof(UINT);
	BuffSize -= sizeof(UINT);

	*(pBuff + BuffPtr) = 20;
	BuffPtr++;
	BuffSize--;

	*(pBuff + BuffPtr) = 0;
	BuffPtr++;
	BuffSize--;

	int Len = _snprintf_s(pBuff + BuffPtr, BuffSize, BuffSize, "d1:md11:ut_metadatai3eee");
	BuffPtr += Len;
	BuffSize -= Len;

	*MsgSize = htonl(BuffPtr - sizeof(UINT));
	return Send(pBuff, BuffPtr) != FALSE;
}
bool CTRConnection::SendRequest(UINT Piece)
{
	char * pBuff = (char *)m_SendBuffer.GetBuffer();
	UINT BuffSize = m_SendBuffer.GetBufferSize();
	UINT BuffPtr = 0;

	UINT * MsgSize = (UINT *)(pBuff + BuffPtr);
	BuffPtr += sizeof(UINT);
	BuffSize -= sizeof(UINT);

	*(pBuff + BuffPtr) = 20;
	BuffPtr++;
	BuffSize--;

	*(pBuff + BuffPtr) = m_MetaDataMsgID;
	BuffPtr++;
	BuffSize--;

	int Len = _snprintf_s(pBuff + BuffPtr, BuffSize, BuffSize, "d8:msg_typei0e5:piecei%uee", Piece);
	BuffPtr += Len;
	BuffSize -= Len;

	*MsgSize = htonl(BuffPtr - sizeof(UINT));
	return Send(pBuff, BuffPtr) != FALSE;
}

void CTRConnection::OnMsg(BYTE * pData, UINT DataSize)
{
	CBencodingValue MsgData;

	UINT ExtDataSize = DataSize;
	const BYTE * pExtData = MsgData.Parser(pData, ExtDataSize);

	if (MsgData.GetType() != BENCODING_TYPE_DICTIONARY)
	{
		PrintNetLog(m_LogTag, _T("1�յ����Ϸ�����Ϣ��ʽ����%s�Ͽ�"), GetRemoteAddress().GetAddressString());
		Disconnect();
	}

	if (m_FinishHandshake == 1)
	{
		if (MsgData.HaveDictValue("m"))
		{
			CBencodingValue& m = MsgData.GetDictValue("m");
			if (m.GetType() == BENCODING_TYPE_DICTIONARY)
			{
				for (UINT i = 0; i < m.GetDictCount(); i++)
				{
					BENCODING_VALUE_PAIR& Pair = m.GetDictPair(i);
					if (Pair.Value->GetType() == BENCODING_TYPE_INTEGER)
					{
						CEasyString Temp = Pair.Key;
						PrintNetDebugLog(m_LogTag, _T("��չ��Ϣ%s=%d"), (LPCTSTR)Temp, Pair.Value->GetIntValue());
						if (Pair.Key == "ut_metadata")
						{
							m_MetaDataMsgID = (int)Pair.Value->GetIntValue();
						}
					}
				}				
			}			

			if (MsgData.HaveDictValue("metadata_size"))
			{
				CBencodingValue& MetaDataSize = MsgData.GetDictValue("metadata_size");
				m_MetaDataSize = (UINT)MetaDataSize.GetIntValue();
				if (m_MetaDataSize < MAX_TORRENT_SIZE)
				{					
					PrintNetDebugLog(m_LogTag, _T("MetaDataSize=%u"), m_MetaDataSize);
					m_MetaDataBuffer.Create(m_MetaDataSize);
				}
				else
				{
					PrintNetDebugLog(m_LogTag, _T("metadata_size=%u��С�쳣"), m_MetaDataSize);
					m_MetaDataSize = 0;
				}
			}

			PrintNetDebugLog(m_LogTag, _T("�յ�����2����ʼ��������"));
			m_FinishHandshake = 2;
			SendRequest(m_CurMetaDataPiece);
		}
		else
		{
			PrintNetLog(m_LogTag, _T("2�յ����Ϸ�����Ϣ��ʽ����%s�Ͽ�"), GetRemoteAddress().GetAddressString());
			Disconnect();
		}
	}
	else
	{
		if (MsgData.HaveDictValue("msg_type"))
		{
			int MsgType = (int)MsgData.GetDictValue("msg_type").GetIntValue();
			if (MsgType == 1)
			{
				if (m_CurMetaDataPiece == 0 && MsgData.HaveDictValue("total_size"))
				{
					m_MetaDataSize = (UINT)MsgData.GetDictValue("total_size").GetIntValue();	
					if (m_MetaDataSize < MAX_TORRENT_SIZE)
					{
						m_MetaDataBuffer.Create(m_MetaDataSize);
					}
					else
					{
						PrintNetDebugLog(m_LogTag, _T("total_size=%u��С�쳣"), m_MetaDataSize);
						m_MetaDataSize = 0;
					}
				}
				if (m_MetaDataSize)
				{
					PrintNetLog(m_LogTag, _T("�յ�MetaData Size=%d"), ExtDataSize);
					m_MetaDataBuffer.PushBack(pExtData, ExtDataSize);
					m_CurMetaDataPiece++;
					if (m_MetaDataBuffer.GetUsedSize() < m_MetaDataSize)
					{
						SendRequest(m_CurMetaDataPiece);
					}
					else
					{
						PrintNetLog(m_LogTag, _T("MetaData������ϣ�����%s�Ͽ�"), GetRemoteAddress().GetAddressString());
						Disconnect();
						m_pParent->OnTorrent(m_SearchID, (BYTE *)m_MetaDataBuffer.GetBuffer(), m_MetaDataBuffer.GetUsedSize());
					}
				}
				else
				{
					PrintNetLog(m_LogTag, _T("Э���쳣������%s�Ͽ�"), GetRemoteAddress().GetAddressString());
					Disconnect();
				}
			}
			else if (MsgType == 2)
			{
				PrintNetLog(m_LogTag, _T("���󱻾ܾ�������%s�Ͽ�"), GetRemoteAddress().GetAddressString());
				Disconnect();
			}
			else
			{
				PrintNetLog(m_LogTag, _T("�쳣MsgType=%d"), MsgType);
			}
		}
		else
		{
			PrintNetLog(m_LogTag, _T("3�յ����Ϸ�����Ϣ��ʽ����%s�Ͽ�"), GetRemoteAddress().GetAddressString());
			Disconnect();
		}
	}
	
}