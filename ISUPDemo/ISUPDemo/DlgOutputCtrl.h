#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgOutputCtrl dialog
class CPTZButton : public CButton
{
    // Construction
public:
    CPTZButton();

    // Attributes
public:
    int m_iSubBtnIndex;
    LONG m_lPlayHandle;//Player Handle
    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPTZButton)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CPTZButton();

    // Generated message map functions
protected:
    //{{AFX_MSG(CPTZButton)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

class CDlgOutputCtrl : public CDialog
{
    DECLARE_DYNAMIC(CDlgOutputCtrl)

public:
    CDlgOutputCtrl(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgOutputCtrl();

// Dialog Data
    enum { IDD = IDD_DLG_OUTPUT_CTRL };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    NET_EHOME_IMAGE_CFG m_struImageCfg;
public:
    afx_msg void OnBnClickedBtnPresetSetup();
    CComboBox m_comboSpeed;
    CComboBox m_comboPresetIndex;

    CPTZButton m_btnPtzUp;
    CPTZButton m_btnPtzDown;
    CPTZButton m_btnPtzLeft;
    CPTZButton m_btnPtzRight;
    CPTZButton m_btnPtzLeftUp;
    CPTZButton m_btnPtzRightUp;
    CPTZButton m_btnPtzLeftDown;    
    CPTZButton m_btnPtzRightDown;
    CPTZButton m_btnZoomIn;//zoom in
    CPTZButton m_btnZoomOut;//zoom out
    CPTZButton m_btnFocusNear;//focus near
    CPTZButton m_btnFocusFar;//focus far
    CPTZButton m_btnIrisOpen;//iris open
    CPTZButton m_btnIrisClose;//iris close
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnPresetAdjust();
    afx_msg void OnBnClickedBtnPresetDelete();
    afx_msg void OnBnClickedBtnLightOn();
    afx_msg void OnBnClickedBtnLightOff();
    afx_msg void OnBnClickedBtnWiperOn();

    afx_msg void OnBnClickedBtnWiperOff();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    CSliderCtrl m_sliderBrightness;
    CSliderCtrl m_sliderContrast;
    CSliderCtrl m_sliderSaturation;
    CSliderCtrl m_sliderHue;
    afx_msg void OnBnClickedBtnImageRefresh();
};
