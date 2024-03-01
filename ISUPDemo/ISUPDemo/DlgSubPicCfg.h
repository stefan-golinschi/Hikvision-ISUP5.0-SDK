#pragma once
#include "afxwin.h"


// CDlgSubPicCfg 对话框

class CDlgSubPicCfg : public CDialog
{
    DECLARE_DYNAMIC(CDlgSubPicCfg)

public:
    CDlgSubPicCfg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgSubPicCfg();

// 对话框数据
    enum { IDD = IDD_SUB_DLG_PIC_CFG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    DWORD m_dwChannelNo;
    BOOL m_bIsShowChanName;
    CString m_sChanName;
    DWORD m_dwPosChanX;
    DWORD m_dwPosChanY;
    BOOL m_bIsShowOSD;
    DWORD m_dwPosOSDX;
    DWORD m_dwPosOSDY;
    CComboBox m_cmbOSDType;
    CComboBox m_cmbOSDAttrib;
    BOOL m_bIsShowWeek;

    int m_iDeviceIndex;
    LONG m_lLoginID;
    int m_iChanIndex;
    NET_EHOME_PIC_CFG m_struPicCfg;

    BOOL CheckInitParam();
    void CurCfgUpdate();
    void CurCfgSetup();
    virtual BOOL OnInitDialog();
    BOOL m_bShow[4];
    CString m_csString[4];
    int m_iPosX[4];
    int m_iPosY[4];
    afx_msg void OnBnClickedBtnSetOsd();
    afx_msg void OnBnClickedBtnGetOsd();
};
