
// GetPagesDlg.h : ͷ�ļ�
//

#pragma once
#include "OperateKernel.h"
#include "afxcmn.h"
#include <Tlhelp32.h>
#include <Psapi.h>

enum LOAD_PAGES
{
	PAGES_LOADED,
	PAGES_UNLOADED,
};

// CGetPagesDlg �Ի���
class CGetPagesDlg : public CDialogEx
{
// ����
public:
	CGetPagesDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GETPAGES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	PAEPaging::PDPTE_TT* m_pTT = NULL;
	COperateKernel m_operKer;

	DWORD m_dwPDPTEIdx = -1;	// PDPTE���±�
	DWORD m_dwPDEIdx = -1;		// PDE���±�
	DWORD m_dwCurPID = -1;
	BOOL InitProcessList();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CListCtrl m_lstCtrlProcess;
	CListCtrl m_lstCtrlPDPTE;
	CListCtrl m_lstCtrlPDE;
	CListCtrl m_lstCtrlPTE;
	afx_msg void OnLvnItemchangedGetPagesChoosePID(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChangedPagesPDPTE(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChangedPagesPDE(NMHDR *pNMHDR, LRESULT *pResult);

};
