#pragma once
#include "afxwin.h"


// CSubDlgAlarmCfg dialog

class CSubDlgAlarmCfg : public CDialog
{
    DECLARE_DYNAMIC(CSubDlgAlarmCfg)

public:
    CSubDlgAlarmCfg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CSubDlgAlarmCfg();

// Dialog Data
    enum { IDD = IDD_SUB_DLG_ALARM_CFG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()

public:
    NET_EHOME_ALARMIN_COND m_struAlarmInCond;
    NET_EHOME_ALARMIN_CFG m_struAlarmInCfg;
    NET_EHOME_ALARM_TIME_COND m_struAlarmTimeCond;
    NET_EHOME_ALARM_TIME_CFG m_struAlarmInTimeCfg;
    NET_EHOME_ALARM_TIME_CFG m_struAlarmOutTimeCfg;
    NET_EHOME_ALARMOUT_CFG m_struAlarmOutCfg;
    NET_EHOME_ALARMOUT_STATUS_CFG m_struAlarmOutStatusCfg;
    NET_EHOME_MANUAL_IOOUT_CTRL m_struManualIOCtrl;
    int m_iDeviceIndex;
    LONG m_lLoginID;

public:
    BOOL CheckInitParam();
    void CurCfgUpdate();
    void CurCfgSetup();
    void AlarmInUpdate();
    void AlarmTimeUpdateIn();
    void AlarmTimeUpdateOut();
    void AlarmOutUpdate();
    void ManualIOCtrl(BOOL bStart);
    afx_msg void OnCbnSelchangeComboAlarminNo();
    afx_msg void OnCbnSelchangeComboPtzChan();
    afx_msg void OnCbnSelchangeComboAlarmoutNo();
    afx_msg void OnBnClickedButtonAlarminTime();
    afx_msg void OnCbnSelchangeComboWeekdayIn();
    afx_msg void OnCbnSelchangeComboWeekdayOut();
    afx_msg void OnBnClickedButtonAlarmoutTime();
    afx_msg void OnBnClickedButtonAnalogalarmOut();
    afx_msg void OnBnClickedButtonAnalogchan();
    afx_msg void OnBnClickedButtonDigitalchan();
    afx_msg void OnCbnSelchangeComboOutputEnable();
    afx_msg void OnCbnSelchangeComboDelaytime();
    virtual BOOL OnInitDialog();

public:
    CComboBox m_cmbAlarmInNo;
    CComboBox m_cmbPTZChan;
    CString m_strAlarmInName;
    CComboBox m_cmbAlarmInType;
    CComboBox m_cmbAlarmOutNo;
    CString m_strAlarmOutName;
    CComboBox m_cmbAlarmOutDelay;
    BOOL m_bAlarmOutStatus;
    CComboBox m_cmbMonitorAlarm;
    CComboBox m_cmbSoundAlarm;
    CComboBox m_cmbEmail;
    CComboBox m_cmbUpload;
    CComboBox m_cmbAlarmOut;
    CComboBox m_cmbUsePreset;
    CComboBox m_cmbCurise;
    CComboBox m_cmbUseTrack;
    int m_iPresetNO;
    int m_iCuriseNO;
    int m_iTrackNO;
    int m_iAnalogAlarmNum;
    int m_iAnalogChanNum;
    int m_iDigitalChanNum;
    CComboBox m_cmbUseAlarmIn;
    CComboBox m_cmbWeekdayIn;
    CComboBox m_cmbWeekdayOut;
    CComboBox m_cmbOutEnable;
    CComboBox m_cmbDelayTime;
    DWORD m_dwDeleyTime;
};
