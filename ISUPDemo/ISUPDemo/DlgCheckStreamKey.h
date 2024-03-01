#pragma once


// CDlgCheckStreamKey 对话框

class CDlgCheckStreamKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgCheckStreamKey)

public:
	CDlgCheckStreamKey(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCheckStreamKey();
    LONG m_lUserID;
    int m_iDeviceIndex;
// 对话框数据
    enum { IDD = IDD_DLG_CHCKE_STREAM_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CString m_csKey;
    afx_msg void OnBnClickedOk();
};
