#pragma once
#include "afxcmn.h"
#include "mapbox.h"


// CDlgMapView �Ի���

class CDlgMapView : public CDialog
{
	DECLARE_DYNAMIC(CDlgMapView)
protected:
	CDialogItemSorter	m_ItemSorter;
	CClient *			m_pClient;	
	CListCtrl			m_lvCharList;
	CMapBox				m_stMapBox;
public:
	CDlgMapView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMapView();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAP_VIEW };

	void SetClient(CClient * pClient)
	{
		m_pClient=pClient;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DrawMap(CPaintDC * pDC);

	void FillCharList();
	void OnClickMap(CPoint Point);
};
