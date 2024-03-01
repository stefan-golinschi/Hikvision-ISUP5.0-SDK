#pragma once
#include "afxwin.h"


// CDlgStreamEncrypt 对话框

class CDlgStreamEncrypt : public CDialog
{
	DECLARE_DYNAMIC(CDlgStreamEncrypt)

public:
	CDlgStreamEncrypt(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStreamEncrypt();
    LONG m_lUserID;
    int m_iDeviceIndex;
// 对话框数据
	enum { IDD = IDD_DLG_STREAM_ENCRYPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    CComboBox m_cmbEncrypt;
    CString m_csNewKey;
    CString m_csOldKey;
};
