#pragma once


// CDlgInfoPubTerCfg 对话框

class CDlgInfoPubTerCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInfoPubTerCfg)

public:
	CDlgInfoPubTerCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInfoPubTerCfg();
	BOOL CheckInitParam();
// 对话框数据
	enum { IDD = IDD_DLG_SETPLAYERPARAM };
	int m_iDeviceIndex;
	LONG m_lLoginID;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSetplayerparam();
	afx_msg void OnBnClickedBtnGetplayerparam();
};
