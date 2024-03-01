#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"


// CDlgFlowInfo 对话框

class CDlgFlowInfo : public CDialog
{
    DECLARE_DYNAMIC(CDlgFlowInfo)

public:
    CDlgFlowInfo(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgFlowInfo();

// 对话框数据
    enum { IDD = IDD_DLG_FLOW_INFO };

    BOOL CheckInitParam();
    static UINT GetFileThread(LPVOID pParam);
    afx_msg void OnBnClickedBtnSearch();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    LONG m_iDeviceIndex;
    LONG m_lLoginID;
    BOOL m_bSearching;
    BOOL m_bQuit;
    LONG m_iChanIndex;
    LONG m_lFileHandle;
    HANDLE m_hFileThread;
    DWORD m_dwFileNum;
    NET_EHOME_FINDCOND m_struFindCond;

    CComboBox m_cmbFlowType;
    CListCtrl m_listFlowInfo;
    COleDateTime m_dateStart;
    COleDateTime m_timeStart;
    COleDateTime m_dateStop;
    COleDateTime m_timeStop;
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    int m_bTimeType;
};
