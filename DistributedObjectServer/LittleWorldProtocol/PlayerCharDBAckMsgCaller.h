#pragma once


class CPlayerCharDBAckMsgCaller :
	public IPlayerCharDBAck
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CPlayerCharDBAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CPlayerCharDBAckMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CPlayerCharDBAckMsgCaller(void);
	
	

	/*************************************************
	������:	AccountLoginAck
	��;:	
	����:
    Result				
    AccountID				
    AccountData				
	����ֵ:���ر�����
	*************************************************/
	virtual int AccountLoginAck(int Result ,UINT64 AccountID ,CSmartStruct& AccountData );
	


	/*************************************************
	������:	AccountLogoutAck
	��;:	
	����:
    Result				
    AccountID				
	����ֵ:���ر�����
	*************************************************/
	virtual int AccountLogoutAck(int Result ,UINT64 AccountID );
	


	/*************************************************
	������:	GetAccountCharAck
	��;:	
	����:
    Result				
    AccountID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetAccountCharAck(int Result ,UINT64 AccountID ,CSmartStruct& CharData );
	


	/*************************************************
	������:	CreateCharAck
	��;:	
	����:
    Result				
    AccountID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int CreateCharAck(int Result ,UINT64 AccountID ,CSmartStruct& CharData );
	


	/*************************************************
	������:	DeleteCharAck
	��;:	
	����:
    Result				
    AccountID				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteCharAck(int Result ,UINT64 AccountID ,UINT64 CharID );
	


	/*************************************************
	������:	GetCharDataAck
	��;:	
	����:
    Result				
    AccountID				
    CharID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetCharDataAck(int Result ,UINT64 AccountID ,UINT64 CharID ,CSmartStruct& CharData );
	


	/*************************************************
	������:	SaveCharDataAck
	��;:	
	����:
    Result				
    AccountID				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int SaveCharDataAck(int Result ,UINT64 AccountID ,UINT64 CharID );
	

	bool PackMsgAccountLoginAck(CSmartStruct& MsgPacket,int Result ,UINT64 AccountID ,CSmartStruct& AccountData );
	bool PackMsgAccountLogoutAck(CSmartStruct& MsgPacket,int Result ,UINT64 AccountID );
	bool PackMsgGetAccountCharAck(CSmartStruct& MsgPacket,int Result ,UINT64 AccountID ,CSmartStruct& CharData );
	bool PackMsgCreateCharAck(CSmartStruct& MsgPacket,int Result ,UINT64 AccountID ,CSmartStruct& CharData );
	bool PackMsgDeleteCharAck(CSmartStruct& MsgPacket,int Result ,UINT64 AccountID ,UINT64 CharID );
	bool PackMsgGetCharDataAck(CSmartStruct& MsgPacket,int Result ,UINT64 AccountID ,UINT64 CharID ,CSmartStruct& CharData );
	bool PackMsgSaveCharDataAck(CSmartStruct& MsgPacket,int Result ,UINT64 AccountID ,UINT64 CharID );

};
