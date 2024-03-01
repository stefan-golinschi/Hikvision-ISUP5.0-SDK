#pragma once


// CDlgMotionConfig 对话框

class CDlgMotionConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMotionConfig)

public:
	CDlgMotionConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMotionConfig();

// 对话框数据
	enum { IDD = IDD_DLG_MOTION_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    BOOL m_bUseMonition;
    int m_iSensitive;
    afx_msg void OnBnClickedBtnSet();
    BOOL CheckInitParam();

public:
    int m_iDeviceIndex;
    LONG m_lLoginID;
    DWORD m_dwChannelNo;
    int m_iChanIndex;
    afx_msg void OnBnClickedBtnSetHid();
    int m_iHideSensive;
    afx_msg void OnBnClickedBtnGet();
    afx_msg void OnBnClickedBtnGetHid();
    BOOL m_bVideoLost;
    afx_msg void OnBnClickedBtnSetVideo();
    afx_msg void OnBnClickedBtnGetVideo();
    afx_msg void OnBnClickedBtnSetPrivate();
    afx_msg void OnBnClickedBtnGetPrivate();
    BOOL m_bPrivate;
};
