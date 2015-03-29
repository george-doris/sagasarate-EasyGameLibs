#pragma once


class CPlayerCharDBMsgCaller :
	public IPlayerCharDB
{
protected:
	IDistributedObjectOperator *	m_pOperator;
	OBJECT_ID						m_SenderID;
	CEasyArray<OBJECT_ID>			m_TargetIDs;
public:
	CPlayerCharDBMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID TargetID);
	CPlayerCharDBMsgCaller(IDistributedObjectOperator * pOperator,OBJECT_ID SenderID,OBJECT_ID * pTargetIDs,ID_LIST_COUNT_TYPE TargetIDCount);
	~CPlayerCharDBMsgCaller(void);
	
	

	/*************************************************
	������:	AccountLogin
	��;:	
	����:
    szAccountName				
    szPassword				
    szClientIP				
	����ֵ:���ر�����
	*************************************************/
	virtual int AccountLogin(LPCTSTR szAccountName ,LPCTSTR szPassword ,LPCTSTR szClientIP );
	


	/*************************************************
	������:	AccountLogout
	��;:	
	����:
    AccountID				
	����ֵ:���ر�����
	*************************************************/
	virtual int AccountLogout(UINT64 AccountID );
	


	/*************************************************
	������:	GetAccountChar
	��;:	
	����:
    AccountID				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetAccountChar(UINT64 AccountID );
	


	/*************************************************
	������:	CreateChar
	��;:	
	����:
    AccountID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int CreateChar(UINT64 AccountID ,CSmartStruct& CharData );
	


	/*************************************************
	������:	DeleteChar
	��;:	
	����:
    AccountID				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int DeleteChar(UINT64 AccountID ,UINT64 CharID );
	


	/*************************************************
	������:	GetCharData
	��;:	
	����:
    AccountID				
    CharID				
	����ֵ:���ر�����
	*************************************************/
	virtual int GetCharData(UINT64 AccountID ,UINT64 CharID );
	


	/*************************************************
	������:	SaveCharData
	��;:	
	����:
    AccountID				
    CharID				
    CharData				
	����ֵ:���ر�����
	*************************************************/
	virtual int SaveCharData(UINT64 AccountID ,UINT64 CharID ,CSmartStruct& CharData );
	

	bool PackMsgAccountLogin(CSmartStruct& MsgPacket,LPCTSTR szAccountName ,LPCTSTR szPassword ,LPCTSTR szClientIP );
	bool PackMsgAccountLogout(CSmartStruct& MsgPacket,UINT64 AccountID );
	bool PackMsgGetAccountChar(CSmartStruct& MsgPacket,UINT64 AccountID );
	bool PackMsgCreateChar(CSmartStruct& MsgPacket,UINT64 AccountID ,CSmartStruct& CharData );
	bool PackMsgDeleteChar(CSmartStruct& MsgPacket,UINT64 AccountID ,UINT64 CharID );
	bool PackMsgGetCharData(CSmartStruct& MsgPacket,UINT64 AccountID ,UINT64 CharID );
	bool PackMsgSaveCharData(CSmartStruct& MsgPacket,UINT64 AccountID ,UINT64 CharID ,CSmartStruct& CharData );

};
