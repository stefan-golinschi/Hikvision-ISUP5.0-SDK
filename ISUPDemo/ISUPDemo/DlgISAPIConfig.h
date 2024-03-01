#pragma once


// CDlgISAPIConfig 对话框

class CDlgISAPIConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgISAPIConfig)

public:
	CDlgISAPIConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgISAPIConfig();

// 对话框数据
	enum { IDD = IDD_DLG_ISAPI_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
    LONG m_lUserID;
    int m_iDeviceIndex;
public:
    CString m_sInput;
    CString m_sOutput;
    CString m_sUrl;
    afx_msg void OnBnClickedBtnCommand();
    virtual BOOL OnInitDialog();
};
