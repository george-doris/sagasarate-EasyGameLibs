/****************************************************************************/
/*                                                                          */
/*      �ļ���:    EasyTimer.h                                              */
/*      ��������:  2010��02��09��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once


#define MAXTIME	(UINT32(0xffffffff))

inline UINT32	GetTimeToTime(UINT32 t1,UINT32 t2 )
{
	return (t1<=t2?(t2-t1):(MAXTIME-t1+t2));
}

class CEasyTimer
{
protected:
	UINT32	m_dwSavedTime;
	UINT32	m_dwTimeoutTime;
public:
	CEasyTimer():m_dwSavedTime(0),m_dwTimeoutTime(0)
	{

	}
	CEasyTimer(const CEasyTimer& Timer)
	{
		m_dwSavedTime=Timer.m_dwSavedTime;
		m_dwTimeoutTime=Timer.m_dwTimeoutTime;
	}
	CEasyTimer& operator=(const CEasyTimer& Timer)
	{
		m_dwSavedTime=Timer.m_dwSavedTime;
		m_dwTimeoutTime=Timer.m_dwTimeoutTime;
		return *this;
	}
#ifdef WIN32
	static inline UINT32	GetTime()
	{
		return GetTickCount();
	}
#else
	static inline UINT32	GetTime()
	{
		struct timeval CurTime;
		gettimeofday(&CurTime,NULL);
		return CurTime.tv_sec*1000+CurTime.tv_usec/1000;
	}	
#endif
	void	SaveTime(UINT32 dwCurTime)
	{
		m_dwSavedTime = dwCurTime;
	}
	void	SaveTime()
	{
		SaveTime(CEasyTimer::GetTime());
	}
	static BOOL	IsTimeOut( UINT32 starttime, UINT32 timeout )
	{
		UINT32	dwTime = CEasyTimer::GetTime();
		if( GetTimeToTime(starttime, dwTime) >= timeout )
		{
			return TRUE;
		}
		return FALSE;
	}
	BOOL	IsTimeOut( UINT32 dwTimeOut )
	{
		UINT32	dwTime = CEasyTimer::GetTime();
		if( GetTimeToTime(m_dwSavedTime, dwTime)>= dwTimeOut )
		{
			return TRUE;
		}
		return FALSE;
	}
	void	SetTimeOut( UINT32	dwTimeOut)
	{
		m_dwSavedTime = CEasyTimer::GetTime();
		m_dwTimeoutTime = dwTimeOut;
	}
	BOOL	IsTimeOut()
	{
		UINT32	dwTime = CEasyTimer::GetTime();
		if( GetTimeToTime(m_dwSavedTime, dwTime)>= m_dwTimeoutTime )
			return TRUE;
		return FALSE;
	}
	void Reset(UINT32 dwCurTime)
	{
		m_dwSavedTime=dwCurTime;
	}
	void Reset()
	{
		Reset(CEasyTimer::GetTime());
	}
	UINT32	GetLeftTime(UINT32 dwCurTime)
	{
		UINT32 dwTime = GetTimeToTime( m_dwSavedTime, dwCurTime );
		if( dwTime >= m_dwTimeoutTime )return 0;
		return (m_dwTimeoutTime - dwTime);
	}
	UINT32	GetLeftTime()
	{
		return GetLeftTime(CEasyTimer::GetTime());
	}
	UINT32 GetPastTime(UINT32 dwCurTime)
	{		
		return GetTimeToTime( m_dwSavedTime, dwCurTime );
	}
	UINT32 GetPastTime()
	{
		return GetPastTime(CEasyTimer::GetTime());
	}
	UINT32	GetTimeOut(){ return m_dwTimeoutTime;}
	UINT32	GetSavedTime(){ return m_dwSavedTime;}

};
