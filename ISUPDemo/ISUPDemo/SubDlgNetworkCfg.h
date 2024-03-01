#pragma once
#include "afxwin.h"


// CSubDlgNetworkCfg dialog

class CSubDlgNetworkCfg : public CDialog
{
    DECLARE_DYNAMIC(CSubDlgNetworkCfg)

public:
    CSubDlgNetworkCfg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CSubDlgNetworkCfg();

// Dialog Data
    enum { IDD = IDD_SUB_DLG_NETWORK_CFG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
public:
    BOOL CheckInitParam();
    void CurCfgUpdate();
    void CurCfgSetup();
    afx_msg void OnBnClickedCheckPppoeEnable();

private:
    NET_EHOME_NETWORK_CFG   m_struNetworkCfg;
    int m_iDeviceIndex;
    LONG m_lLoginID;

public:
    CString m_strDevAddr;
    CString m_strMaskAddr;
    CComboBox m_cmbNetType;
    int m_iMTUValue;
    CString m_strMACAddr;
    BOOL m_bPPPoEEnable;
    CString m_strPPPoEUser;
    CString m_strPPPoEAddr;
    CString m_strPPPoEPWD;
    CString m_strGatewayAddr;
    CString m_strMulticastAddr;
    CString m_strDDNS1Addr;
    CString m_strIPResolverAddr;
    int m_iIPResolverPort;
    CString m_strDDNS2Addr;
    CString m_strAlarmHostAddr;
    int m_iAlarmHostPort;
    int m_iSDKPort;
    int m_iHttpPort;
};
