#pragma once
#include "afxcmn.h"


// CDlgUpgradeHttp 对话框

class CDlgUpgradeHttp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUpgradeHttp)

public:
	CDlgUpgradeHttp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUpgradeHttp();

// 对话框数据
	enum { IDD = IDD_DLG_UPGRADE_HTTP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    int m_iDeviceIndex;
    LONG m_lLoginID;
    void GetProgress();
public:
    CString m_csPassword;
    DWORD m_dwPort;
    CString m_csFileName;
    afx_msg void OnBnClickedOk();
    CIPAddressCtrl m_IPServer;
    CString m_sOutput;
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnClose();
    afx_msg void OnBnClickedBtnStopUpgrade();
};
