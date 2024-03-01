#pragma once

// CDlgInfoPublish 对话框

class CDlgInfoPublish : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInfoPublish)

public:
	CDlgInfoPublish(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInfoPublish();
    BOOL CheckInitParam();
// 对话框数据
	enum { IDD = IDD_DLG_INFOPUBLISH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    int m_iDeviceIndex;
    LONG m_lLoginID;
	afx_msg void OnBnClickedBtnTerGroundLight();
    CString m_terminalName;
    afx_msg void OnBnClickedBtnSetTerminalName();
    DWORD m_dwServerPort;
    DWORD m_dwClientFd;
    CString m_strPicKmsAddr;
    afx_msg void OnBnClickedBtnScreenShot();
    UINT m_dwCommunityFd;
    UINT m_dwTransIdentify;
    CString m_strPicIdentify;
    CString m_strRetPicKmsAddr;
    afx_msg void OnBnClickedBtnGetTerminalWorkStatus();
    afx_msg void OnBnClickedBtnGetTerminalName();
    afx_msg void OnBnClickedBtnScreenShotSecond();
};
