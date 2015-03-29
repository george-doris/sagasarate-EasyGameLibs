#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CClientInfoView ������ͼ

class CClientInfoView : public CFormView
{
	DECLARE_DYNCREATE(CClientInfoView)
protected:
	
protected:
	CClientInfoView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CClientInfoView();

public:
	enum { IDD = IDD_CLIENTINFOVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CDialogItemSorter		m_ItemSorter;
	CRichEditCtrl			m_reLogMsg;
	CCycleBuffer			m_LogBuffer;
	CClient *				m_pClient;
	CDlgMapView				m_DlgMapView;
	int						m_CurCommandHistoryPtr;
public:
	virtual void OnInitialUpdate();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void PushLog(LPCTSTR szMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void SetClient(CClient * pClient)
	{
		m_pClient=pClient;
		m_DlgMapView.SetClient(pClient);
	}

	afx_msg void OnBnClickedShowMap();
	afx_msg void OnBnClickedExec();
	CEdit m_edCommand;
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


