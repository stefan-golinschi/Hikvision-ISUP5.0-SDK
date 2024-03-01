#pragma once
#include "afxwin.h"


// CSubDlgChanCfg dialog

class CSubDlgChanCfg : public CDialog
{
    DECLARE_DYNAMIC(CSubDlgChanCfg)

public:
    CSubDlgChanCfg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CSubDlgChanCfg();

// Dialog Data
    enum { IDD = IDD_SUB_DLG_CHANNEL_CFG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    void AddResolution();
    void SetResolutionCurSel(DWORD dwData);
    BOOL CheckInitParam();
    void CurCfgUpdate();
    void CurCfgSetup();
    virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeComboCompressionType();

private:
    int m_iDeviceIndex;
    LONG m_lLoginID;
    int m_iChanIndex;
    NET_EHOME_COMPRESSION_COND  m_struCompressionCond;
    NET_EHOME_COMPRESSION_CFG   m_struCompressionCfg;
    NET_EHOME_IMAGE_CFG m_struImageCfg;
    
public:
    CComboBox m_cmbCompressionType;
    CComboBox m_cmbImageQuality;
    CComboBox m_cmbStreamType;
    CComboBox m_cmbResolution;
    CComboBox m_cmbBitrateType;
    CComboBox m_cmbFrameRate;
    CComboBox m_cmbMaxBitrate;
    DWORD m_dwSelfDefBitrate;
    int m_iBright;
    int m_iContrast;
    int m_iSaturation;
    int m_iHue;
    int m_iInteralFrameI;
    int m_iInteralFrameBP;
    afx_msg void OnCbnSelchangeComboMaxBitRate();
    afx_msg void OnSelchangeComboBitRateType();
};
