// ChildFrm.h : CChildFrame ��Ľӿ�
//


#pragma once



class CClientFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CClientFrame)
public:
	CClientFrame();

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	
	CClientInfoView * GetView()
	{
		return m_pView;
	}

	

// ʵ��
public:
	// ��ܹ���������ͼ��
	CClientInfoView *		m_pView;
	
	virtual ~CClientFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
