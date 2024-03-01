#pragma once

#include "HCISUPCMS.h"
#include "afxwin.h"
// CSubDlgDeviceCfg dialog

class CSubDlgDeviceCfg : public CDialog
{
    DECLARE_DYNAMIC(CSubDlgDeviceCfg)

public:
    CSubDlgDeviceCfg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CSubDlgDeviceCfg();

// Dialog Data
    enum { IDD = IDD_SUB_DLG_DEVICE_CFG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    BOOL CheckInitParam();
    void CurCfgUpdate();
    void CurCfgSetup();
    CString GetDVRType(DWORD dwDVRType);
    void AddAudioType();
    void SetResolutionCurSel(DWORD dwData);

private:
    int m_iDeviceIndex;
    LONG    m_lLoginID;
    NET_EHOME_DEVICE_INFO   m_struDevInfo;
    NET_EHOME_VERSION_INFO  m_struVerInfo;
    NET_EHOME_DEVICE_CFG    m_struDevCfg;

public:
    CString m_strDeviceName;
    BOOL m_bCycleRecord;
    int m_iAnalogChanNum;
    int m_iTotalChanNum;
    int m_iVoiceNum;
    CComboBox m_cmbAudioType;
    CString m_strDeviceType;
    int m_iStartChanNum;
    CString m_strCellphoneNO;
    int m_iAlarmInputNum;
    int m_iAlarmOutputNum;
    int m_iDiskNum;
    int m_iZeroChanNum;
    CString m_strDeviceSerialNO;
    CString m_doaoioi;
    int m_iRS232Num;
    int m_iRS485Num;
    int m_iNetportNum;
    int m_iAuxoutNum;
    CComboBox m_cmbMajorScale;
    CComboBox m_cmbMinorScale;
    int m_iTotalAlarmInputNum;
    int m_iTotalAlarmOutputNum;
    int m_iZeroStartChanNum;
    int m_iMaxDigitalNum;
    CString m_strSIMSerialNO;
    int m_iDeviceNO;
    CComboBox m_cmbSmartType;
};
