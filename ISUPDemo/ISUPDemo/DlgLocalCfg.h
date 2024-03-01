#pragma once
#include "afxwin.h"


// CDlgLocalCfg dialog

class CDlgLocalCfg : public CDialog
{
    DECLARE_DYNAMIC(CDlgLocalCfg)

public:
    CDlgLocalCfg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgLocalCfg();

    CComboBox m_comCmsAccess;
    CComboBox m_comAlarmAccess;
    CComboBox m_comStreamAccess;

// Dialog Data
    enum { IDD = IDD_DLG_LOCAL_CFG };
    VOID ModifyLogInfoToFile();
    void ModifySecureInfoToFile();
    void ModifyServerInfoToFile();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnOk();
    afx_msg void OnBtnGetSecureAccessClicked();
    afx_msg void OnBtnSetSecureAccessClicked();

    CString m_strLogPath;
    long m_iLogLevel;
    BOOL m_bAutoDel;
    afx_msg void OnBnClickedBtnCmsAlarm();
    DWORD m_dwKeepLive;
    afx_msg void OnBnClickedBtnRegCfg();
    afx_msg void OnBnClickedBtnAlarmOk();
    DWORD m_dwCount;
    CString m_csAlarmServerIP;
    int m_iAlarmTcpPort;
    int m_iAlarmUdpPort;
    CString m_csPicServerIP;
    int m_iPicServerPort;

    int m_iAlarmType;
    int m_iPicType;
    int m_iAlarmMqttPort;
    afx_msg void OnEnChangeEditLogPath4();
    afx_msg void OnEnChangeEdtTimeCount();
    afx_msg void OnBnClickedBtnGetCompath();
    BOOL m_bHttps;
    afx_msg void OnBnClickedBtnRegCfgCur();
    CString m_strDasConfigFile;
    afx_msg void OnBnClickedBtnDasOk();
    CComboBox m_comboServerType;
    CComboBox m_callBackType;
    CComboBox m_bErrorCodeTest;
    int m_iErrorCode;
    afx_msg void OnBnClickedBtnEnabledError();
    BOOL m_bSmod;
    CString m_strPublicKey;
    CString m_strPrivateKey;
    CString m_strSK;
    CString m_strAK;
    int m_iPoolID;
    afx_msg void OnCbnSelchangeComPicType();
};
