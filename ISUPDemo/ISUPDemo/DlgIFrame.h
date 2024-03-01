#pragma once


// CDlgIFrame 对话框

class CDlgIFrame : public CDialog
{
	DECLARE_DYNAMIC(CDlgIFrame)

public:
	CDlgIFrame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgIFrame();

// 对话框数据
	enum { IDD = IDD_DLG_IFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    LONG m_lUserID;
    int m_iDeviceIndex;
    int m_iChanIndex;
    int m_iChannelNo;
    CComboBox m_cmbStreamType;
    afx_msg void OnBnClickedOk();
};
