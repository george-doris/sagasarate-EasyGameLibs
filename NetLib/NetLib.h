﻿/****************************************************************************/
/*                                                                          */
/*      文件名:    NetLib.h                                                 */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#pragma once

#define NETLIB_VERSION	"2.0.0.0"

#ifdef WIN32
#include <WinSock2.h>
#include <MSWSock.h>
#include <Ws2tcpip.h>
#include <Iphlpapi.h>

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")
#pragma comment(lib,"Iphlpapi.lib")


typedef int	socklen_t;

#ifdef UNICODE
#define GetHostName	GetHostNameW
#else
#define GetHostName	gethostname
#endif

#else

#ifndef __USE_MISC
#define __USE_MISC
#endif

#include <netdb.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <linux/if.h> 


#define INVALID_SOCKET						(-1)
//#define INVALID_HANDLE_VALUE				(UINT)(-1)
#define SOCKET_ERROR						(-1)
#define DEFAULT_EPOLL_EVENT_WAIT_NUM		(32)


#define closesocket(s)						close(s)
#define InetNtop							inet_ntop
#define InetPton							inet_pton
#define GetAddrInfo							getaddrinfo
#define FreeAddrInfo						freeaddrinfo
#define GetHostName							gethostname

typedef struct addrinfo						ADDRINFOT;
typedef int SOCKET;

#endif


#define MAX_LISTEN_BACKLOG						64

#define DEFAULT_WORK_THREAD_COUNT			4
#define DEFAULT_SERVER_ACCEPT_QUEUE				128
#define DEFAULT_SERVER_RECV_DATA_QUEUE			256
#define DEFAULT_SERVER_SEND_DATA_QUEUE			256
#define DEFAULT_PARALLEL_ACCEPT					64
#define DEFAULT_PARALLEL_RECV					64
#define DEFAULT_EVENT_OBJECT_COUNT				4096
#define DEFAULT_EVENT_ROUTER_COUNT				1024
#define DEFAULT_EVENT_OBJECT_POOL_GROW_SIZE		2048
#define DEFAULT_EVENT_OBJECT_POOL_GROW_LIMIT	32
#define DEFAULT_EVENT_ROUTER_POOL_GROW_SIZE		1024
#define DEFAULT_EVENT_ROUTER_POOL_GROW_LIMIT	32
#define NET_DATA_BLOCK_SIZE						4096
//#define EPOLL_DATA_BLOCK_SIZE					16
#define DEFAULT_SERVER_PROCESS_PACKET_LIMIT		32


#define MAX_EASY_NET_LINK_MSG_SIZE			65535
#define MAX_EASY_NET_LINK_DATA_SIZE			(MAX_EASY_NET_LINK_MSG_SIZE-sizeof(EASY_NET_LINK_MSG_HEAD))

#define EASY_LINK_RECONNECT_TIME			(10*1000)


struct EASY_NET_LINK_MSG_HEAD
{
	DWORD Size;					//消息包大小，包括头部
	DWORD MsgID;				//消息ID
};

struct EASY_NET_LINK_MSG
{
	EASY_NET_LINK_MSG_HEAD	Header;		//消息头部
	char					Data[1];	//消息体数据
};

enum EASY_NET_LINK_MSG_ID
{
	EASY_NET_LINK_MSG_LINK_START,
	EASY_NET_LINK_MSG_KEEP_ALIVE,
	EASY_NET_LINK_MSG_USER_DATA,
	EASY_NET_LINK_MSG_COMPRESSED_USER_DATA,
};

struct EASY_NET_LINK_INFO
{
	UINT ID;
};


inline bool PrintNetLogWithTag(LPCTSTR Tag, LPCTSTR Format, ...)
{
	va_list vl;
	va_start(vl,Format);
	bool ret = CLogManager::GetInstance()->PrintLogVL(LOG_NET_CHANNEL, ILogPrinter::LOG_LEVEL_NORMAL, Tag, Format, vl);
	va_end(vl);
	return ret;
}

inline bool PrintNetDebugLogWithTag(LPCTSTR Tag, LPCTSTR Format, ...)
{
	va_list vl;
	va_start(vl,Format);
	bool ret = CLogManager::GetInstance()->PrintLogVL(LOG_NET_CHANNEL, ILogPrinter::LOG_LEVEL_DEBUG, Tag, Format, vl);
	va_end(vl);
	return ret;
}

#define PrintNetLog(_Format, ...)	PrintNetLogWithTag(_T(__PRETTY_FUNCTION__), _Format, ##__VA_ARGS__)
#define PrintNetDebugLog(_Format, ...)	PrintNetDebugLogWithTag(_T(__PRETTY_FUNCTION__), _Format, ##__VA_ARGS__)

#include "IPAddress.h"
#include "IPPattern.h"
#include "NetAdapterInfos.h"

#include "NetSocket.h"

#include "BaseNetConnection.h"
#include "BaseNetService.h"
#include "BaseNetServer.h"


#ifdef WIN32

#include "IIOCPEventHandler.h"
#include "IOCPEventRouter.h"
#include "OverLappedObject.h"
#include "IOCPThread.h"
#include "IOCPListenThread.h"
#include "NetServerIOCP.h"
#include "NetConnectionIOCP.h"
#include "NetServiceIOCP.h"
#include "NetPUDPServiceIOCP.h"

#else

#include "EpollEventObject.h"
#include "IEpollEventHandler.h"
#include "EpollEventRouter.h"
#include "EpollThread.h"
#include "NetServerEpoll.h"
#include "NetServiceEpoll.h"
#include "NetConnectionEpoll.h"
#include "NetConnectionNoBuffEpoll.h"

#endif

#include "EasyNetLinkManager.h"
#include "EasyNetLinkService.h"
#include "ENLBaseConnection.h"
#include "ENLConnection.h"
#include "EasyNetLink.h"
#include "EasyNetTempLink.h"


#include "DHTDefines.h"
#include "BencodingValue.h"
#include "TRConnection.h"
#include "DHTService.h"
