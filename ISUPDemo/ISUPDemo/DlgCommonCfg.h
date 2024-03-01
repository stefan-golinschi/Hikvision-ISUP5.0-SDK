#pragma once
#include "afxcmn.h"

#include "SubDlgDeviceCfg.h"
#include "SubDlgNetworkCfg.h"
#include "SubDlgChanCfg.h"
#include "SubDlgAlarmCfg.h"
#include "SubDlgXmlCfg.h"
#include "DlgSubPicCfg.h"

// CDlgCommonCfg dialog

#define SUB_DEVICE_CFG_T    0
#define SUB_NETWORK_CFG_T 1
#define SUB_CHAN_CFG_T  2
#define SUB_ALARM_CFG_T 3
#define SUB_XML_XFG_T 4
#define SUB_PIC_CFG_T 5
#define SUB_ABILITY_T 6

class CDlgCommonCfg : public CDialog
{
    DECLARE_DYNAMIC(CDlgCommonCfg)

public:
    CDlgCommonCfg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgCommonCfg();

    // Dialog Data
    enum { IDD = IDD_DLG_COMMON_CFG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CSubDlgDeviceCfg        m_subDlgDeviceCfg;
    CSubDlgNetworkCfg   m_subDlgNetworkCfg;
    CSubDlgChanCfg            m_subDlgChanCfg;
    CSubDlgAlarmCfg        m_subDlgAlarmCfg;
    CSubDlgXmlCfg            m_subDlgXmlCfg;
    CDlgSubPicCfg                m_subDlgPicCfg;

    int                                m_iConfigType;
    CTabCtrl                        m_tabCommonCfg;

public:
    void    CurCfgUpdate();

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnTcnSelchangeTabCommonCfg(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnBnClickedBtnRefresh();
    afx_msg void OnBnClickedBtnSaveCfg();
};
