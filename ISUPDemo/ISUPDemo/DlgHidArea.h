#pragma once


// CDlgHidArea 对话框

class CDlgHidArea : public CDialog
{
	DECLARE_DYNAMIC(CDlgHidArea)

public:
	CDlgHidArea(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHidArea();

// 对话框数据
	enum { IDD = IDD_DLG_HID_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    BOOL CheckInitParam();
    int m_iDeviceIndex;
    LONG m_lLoginID;
    DWORD m_dwChannelNo;
    int m_iChanIndex;
    bool m_bEndValid;
    bool m_bStartValid;
    CPoint m_startPoint;
    CPoint m_endPoint;
    bool m_bClear;
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedBtnClear();
    afx_msg void OnBnClickedButtonSet();
    afx_msg void OnBnClickedBtnGet();
};
