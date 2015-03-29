#pragma once


class CCharQueryCenterAckMsgCaller :
	public ICharQueryCenterAck
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CCharQueryCenterAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CCharQueryCenterAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CCharQueryCenterAckMsgCaller(void);
	
	

	/*************************************************
	������:	RegisterCharAck
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int RegisterCharAck(int Result );
	


	/*************************************************
	������:	UnregisterCharAck
	��;:	
	����:
    Result				
	����ֵ:���ر�����
	*************************************************/
	virtual int UnregisterCharAck(int Result );
	

	bool PackMsgRegisterCharAck(CSmartStruct& MsgPacket,int Result );
	bool PackMsgUnregisterCharAck(CSmartStruct& MsgPacket,int Result );

};
