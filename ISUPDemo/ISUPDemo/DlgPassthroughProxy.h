#pragma once


// CDlgPassthroughProxy 对话框

class CDlgPassthroughProxy : public CDialog
{
	DECLARE_DYNAMIC(CDlgPassthroughProxy)

public:
	CDlgPassthroughProxy(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPassthroughProxy();

// 对话框数据
	enum { IDD = IDD_DLG_PT_PROXY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString m_sSDKListenIP;
    DWORD m_dwSDKListenPort;
    CString m_sHTTPListenIP;
    DWORD m_dwHTTPListenPort;
    LONG m_lListenSDK;
    LONG m_lListenHTTP;
    LONG m_iDeviceIndex;
    LONG m_lLoginID;
    LONG m_iChanIndex;
    afx_msg void OnBnClickedBtnStartPtListen();
    afx_msg void OnBnClickedBtnStopPtListen();
    CString m_csHttpIn;
    CString m_csHttpOut;
    afx_msg void OnBnClickedBtnHttp();
};
