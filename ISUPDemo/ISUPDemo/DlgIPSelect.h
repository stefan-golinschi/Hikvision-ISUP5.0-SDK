#pragma once


// DlgIPSelect 对话框

class DlgIPSelect : public CDialog
{
	DECLARE_DYNAMIC(DlgIPSelect)

public:
	DlgIPSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgIPSelect();

// 对话框数据
	enum { IDD = IDD_DLG_IP_SELECT };


    void GetLocalIP();//只能获取本机ipv4地址
    void GetLocalIPEx();//可以获取本机ipv4和ipv6地址
    void DoGetIP();

protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

    int m_nPort;
    CString m_csIP;

private:
    CComboBox m_cmIP;
};
