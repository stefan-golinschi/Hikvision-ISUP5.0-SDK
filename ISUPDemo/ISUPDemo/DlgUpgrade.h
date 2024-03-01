#pragma once
#include "afxcmn.h"


// CDlgUpgrade 对话框

class CDlgUpgrade : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpgrade)

public:
	CDlgUpgrade(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUpgrade();

// 对话框数据
	enum { IDD = IDD_DLG_UPGRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    BOOL CheckInitParam();
	DECLARE_MESSAGE_MAP()
public:
    CString m_csPassword;
    DWORD m_dwPort;
    CString m_csUserName;
    CString m_csFileName;
    CIPAddressCtrl m_IPServer;
    CComboBox m_cmbDevType;
    int m_iDevIndex;
    CString m_csSubDevIndex;
    CString m_strDevType;
    afx_msg void OnBnClickedOk();
    int m_iDeviceIndex;
    int m_iChannel;
    LONG m_lLoginID;
    virtual BOOL OnInitDialog();
    void SetDevTypeValue();
};
