/****************************************************************************/
/*                                                                          */
/*      �ļ���:    DOSProxyManager.cpp                                     */
/*      ��������:  2015��1��6��                                           */
/*      ����:      Sagasarate                                               */
/*                                                                          */
/*      �������Ȩ��Sagasarate(sagasarate@sina.com)����                     */
/*      ����Խ�����������κ���ҵ�ͷ���ҵ�����������                      */
/*      ���뱣���˰�Ȩ����                                                  */
/*                                                                          */
/****************************************************************************/

#include "stdafx.h"


IMPLEMENT_CLASS_INFO_STATIC(CDOSProxyManager, CNameObject);
CDOSProxyManager::CDOSProxyManager()
{
	m_pServer = NULL;
}


CDOSProxyManager::~CDOSProxyManager()
{
	Destory();
}


bool CDOSProxyManager::Initialize(CDOSServer * pServer)
{
	m_pServer = pServer;
	CEasyArray<CLIENT_PROXY_CONFIG>& ProxyConfigs = m_pServer->GetConfig().ClientProxyConfigs;
	//���������Ч��
	for (int i = ProxyConfigs.GetCount() - 1; i >= 0; i--)
	{
		if (ProxyConfigs[i].ProxyType == BROAD_CAST_PROXY_TYPE)
		{
			PrintDOSLog(0, _T("���������벻��Ϊ%d,������ַΪ(%s:%d)�Ĵ���������"), BROAD_CAST_PROXY_TYPE,
				ProxyConfigs[i].ListenAddress.GetIPString(), ProxyConfigs[i].ListenAddress.GetPort());
			ProxyConfigs.Delete(i);
		}
		else
		{
			bool IsListenAddressExist = false;
			bool IsProxyTypeExist = false;
			for (int j = i - 1; j >= 0; j--)
			{
				if (ProxyConfigs[i].ProxyType == ProxyConfigs[j].ProxyType)
				{
					PrintDOSLog(0, _T("�����������ظ�,������ַΪ(%s:%d)�Ĵ���������"), ProxyConfigs[i].ListenAddress.GetIPString(), ProxyConfigs[i].ListenAddress.GetPort());
					ProxyConfigs.Delete(i);
					break;
				}
				else if (ProxyConfigs[i].ListenAddress == ProxyConfigs[j].ListenAddress)
				{
					PrintDOSLog(0, _T("������ַ�ظ�,������ַΪ(%s:%d)�Ĵ���������"), ProxyConfigs[i].ListenAddress.GetIPString(), ProxyConfigs[i].ListenAddress.GetPort());
					ProxyConfigs.Delete(i);
					break;
				}
			}
		}
	}
	if (ProxyConfigs.GetCount())
	{
		m_ProxyServiceList.Resize(ProxyConfigs.GetCount());
		for (UINT i = 0; i < ProxyConfigs.GetCount(); i++)
		{
			m_ProxyServiceList[i] = new CDOSObjectProxyService();
			m_ProxyServiceList[i]->SetID(i + 1);
			if (!m_ProxyServiceList[i]->Init(m_pServer, ProxyConfigs[i]))			
			{
				SAFE_DELETE(m_ProxyServiceList[i]);
			}
		}
	}
	return true;
}
void CDOSProxyManager::Destory()
{
	for (UINT i = 0; i < m_ProxyServiceList.GetCount(); i++)
	{
		if (m_ProxyServiceList[i])
			m_ProxyServiceList[i]->SafeTerminate();
		SAFE_DELETE(m_ProxyServiceList[i]);
	}
	m_ProxyServiceList.Clear();
}

