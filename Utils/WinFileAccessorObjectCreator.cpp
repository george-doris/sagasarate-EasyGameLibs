﻿/****************************************************************************/
/*                                                                          */
/*      文件名:    WinFileAccessorObjectCreator.cpp                         */
/*      创建日期:  2009年07月06日                                           */
/*      作者:      Sagasarate                                               */
/*                                                                          */
/*      本软件版权归Sagasarate(sagasarate@sina.com)所有                     */
/*      你可以将本软件用于任何商业和非商业软件开发，但                      */
/*      必须保留此版权声明                                                  */
/*                                                                          */
/****************************************************************************/
#include "StdAfx.h"
#include ".\winfileaccessorobjectcreator.h"

IMPLEMENT_CLASS_INFO_STATIC(CWinFileAccessorObjectCreator,IFileObjectCreator);

CWinFileAccessorObjectCreator::CWinFileAccessorObjectCreator(void)
{
}

CWinFileAccessorObjectCreator::~CWinFileAccessorObjectCreator(void)
{
}

IFileAccessor * CWinFileAccessorObjectCreator::CreateFileAccessor()
{
	return MONITORED_NEW(_T("CWinFileAccessorObjectCreator"), CWinFileAccessor);
}