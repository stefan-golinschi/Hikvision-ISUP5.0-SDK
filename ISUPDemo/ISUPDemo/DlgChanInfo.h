#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgChanInfo 对话框

class CDlgChanInfo : public CDialog
{
    DECLARE_DYNAMIC(CDlgChanInfo)

public:
    CDlgChanInfo(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgChanInfo();

// 对话框数据
    enum { IDD = IDD_DLG_CHANNEL_INFO };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CIPAddressCtrl m_ipAddress;
    CComboBox m_cmProtocol;
    CComboBox m_cmStreamType;
    int m_iDeviceIndex;
    int m_iChannelIndex;
    afx_msg void OnBnClickedBtnSure();
    DWORD m_wPort;  
    BOOL m_bEncrypt;
    CComboBox m_cmStreamFormat;
};
