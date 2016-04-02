// AndroidForLittleWorld.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMainApp::OnAppAbout)
END_MESSAGE_MAP()


// CMainApp ����

CMainApp::CMainApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMainApp ����

CMainApp theApp;

CMainApp * CMainApp::GetInstance()
{
	return &theApp;
}
// CMainApp ��ʼ��

BOOL CMainApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�

	CExceptionParser::GetInstance()->Init(EXCEPTION_SET_DEFAULT_HANDLER|EXCEPTION_USE_API_HOOK);


	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	AfxInitRichEdit2();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ������ MDI ��ܴ���
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// ��ͼ���ع��� MDI �˵��Ϳ�ݼ���
	//TODO: ��Ӹ��ӳ�Ա�����������ض�
	//	Ӧ�ó��������Ҫ�ĸ��Ӳ˵����͵ĵ���
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_AndroidForLittlTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_AndroidForLittlTYPE));


	


	// �������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();

	m_pMainControlFrame=(CMainControlFrame *)pFrame->CreateNewChild(
		RUNTIME_CLASS(CMainControlFrame), IDR_AndroidForLittlTYPE, m_hMDIMenu, m_hMDIAccel);

	if(m_pMainControlFrame)
		m_pMainControlFrame->SetWindowText("MainControl");
	else
		return FALSE;

	m_Server.StartUp();

	if(!LoadConfig(CONFIG_FILE_NAME))
		return FALSE;

	return TRUE;
}


// CMainApp ��Ϣ�������

int CMainApp::ExitInstance() 
{
	//TODO: �����������ӵĸ�����Դ
	for(UINT i=0;i<m_GroupList.GetCount();i++)
	{
		m_GroupList[i]->SafeTerminate();
		SAFE_RELEASE(m_GroupList[i]);
	}

	m_GroupList.Clear();

	m_Server.ShutDown();

	CLogManager::ReleaseInstance();

	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	return CWinApp::ExitInstance();
}




// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CMainApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMainApp ��Ϣ�������

bool CMainApp::LoadConfig(LPCTSTR szConfigName)
{
	xml_parser Parser;

	if(Parser.parse_file(szConfigName,pug::parse_trim_attribute))
	{
		xml_node Config=Parser.document();
		if(Config.moveto_child("Main"))
		{
			xml_node Group=Config;
			if(Group.moveto_child("Group"))
			{
				CLIENT_GROUP_CONFIG GroupConfig;
				GroupConfig.ServerAddress.SetIP(Group.attribute("ServerIP").getvalue());
				GroupConfig.ServerAddress.SetPort((WORD)Group.attribute("ServerPort"));
				GroupConfig.AccountName=Group.attribute("AccountName").getvalue();
				GroupConfig.Password=Group.attribute("Password").getvalue();
				GroupConfig.FirstConnectDelay=(UINT)Group.attribute("FirstConnectDelay");
				GroupConfig.ReconnectDelay = (UINT)Group.attribute("ReconnectDelay");
				GroupConfig.AutoReconect=Group.attribute("AutoReconect");
				GroupConfig.ScriptFileName=Group.attribute("ScriptFile").getvalue();
				GroupConfig.ClientCount = (UINT)Group.attribute("ClientCount");
				GroupConfig.StartIndex = (UINT)Group.attribute("StartIndex");

				CClientGroup * pGroup=new CClientGroup();
				pGroup->SetIndex(m_GroupList.GetCount());
				if(pGroup->Init(&m_Server,m_pMainControlFrame->GetView(),GroupConfig))
				{
					m_GroupList.Add(pGroup);
					pGroup->Start();
				}
				else
				{
					SAFE_RELEASE(pGroup);
				}
			}
			return true;
		}
		else
		{
			AfxMessageBoxEx(MB_OK,0,"�����ļ�[%s]���Ϸ�",szConfigName);
			return false;
		}

	}
	else
	{
		AfxMessageBoxEx(MB_OK,0,"�޷��������ļ�[%s]",szConfigName);
		return false;
	}
}

UINT CMainApp::AddCommandHistory(LPCTSTR szCommand)
{
	m_CommandHistory.Add(szCommand);
	if(m_CommandHistory.GetCount()>MAX_COMMAND_HISTORY)
		m_CommandHistory.Delete(0);
	return m_CommandHistory.GetCount()+1;
}
LPCTSTR CMainApp::GetCommandHistory(int& Index)
{
	if(Index<0)
		Index=m_CommandHistory.GetCount()-1;
	if(Index>=m_CommandHistory.GetCount())
		Index=0;

	if(Index>=0&&Index<m_CommandHistory.GetCount())
		return m_CommandHistory[Index];

	return NULL;
}

void CMainApp::OnMainControlViewClose()
{
	for(UINT i=0;i<m_GroupList.GetCount();i++)
	{
		m_GroupList[i]->SafeTerminate();
		SAFE_RELEASE(m_GroupList[i]);
	}

	m_GroupList.Clear();
}