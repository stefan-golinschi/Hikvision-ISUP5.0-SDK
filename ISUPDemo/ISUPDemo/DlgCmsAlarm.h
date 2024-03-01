#pragma once
#include "afxcmn.h"


// CDlgCmsAlarm 对话框

class CDlgCmsAlarm : public CDialog
{
	DECLARE_DYNAMIC(CDlgCmsAlarm)

public:
	CDlgCmsAlarm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCmsAlarm();

// 对话框数据
	enum { IDD = IDD_DLG_CMS_ALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    DWORD m_wPort;
    CIPAddressCtrl m_IPAddr;
    afx_msg void OnBnClickedBtnOpen();
    NET_EHOME_AMS_ADDRESS m_struAmsAddr;
    int m_iDeviceIndex;
    afx_msg void OnBnClickedBtnClose();
};
