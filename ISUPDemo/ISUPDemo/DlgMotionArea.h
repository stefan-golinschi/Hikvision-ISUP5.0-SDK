#pragma once

#define MOTION_AREA_NUM (8)
#define ROW (18)
#define BlockPerRow (22)

#define MEGA_MAX(a,b) ((a) < (b) ? (b) : (a))
#define MEGA_MIN(a,b) ((a) < (b) ? (a) : (b))
#define MEGA_RANGE_IN(value, vMin, vMax) (MEGA_MIN(MEGA_MAX((value), (vMin)), (vMax)))
// CDlgMotionArea 对话框

class CDlgMotionArea : public CDialog
{
	DECLARE_DYNAMIC(CDlgMotionArea)

public:
	CDlgMotionArea(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMotionArea();

// 对话框数据
	enum { IDD = IDD_DLG_MOTION_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    int m_iDeviceIndex;
    LONG m_lLoginID;
    DWORD m_dwChannelNo;
    int m_iChanIndex;
    CPoint m_startPoint;
    CPoint m_endPoint;
    CRect m_Rect;

    int m_iAreaNum;
    bool m_bStartValid;
    unsigned int m_aiArea[ROW];

    BOOL CheckInitParam();
    void DrawPoint(CPoint &A, CPoint &B);
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedButtonSet();
    virtual BOOL OnInitDialog();

    afx_msg void OnBnClickedBtnGet();
    afx_msg void OnBnClickedBtnClear();
};
