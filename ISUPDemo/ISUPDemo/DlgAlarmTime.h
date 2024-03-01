#pragma once
#include "afxwin.h"


// CDlgAlarmTime dialog

class CDlgAlarmTime : public CDialog
{
    DECLARE_DYNAMIC(CDlgAlarmTime)

public:
    CDlgAlarmTime(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgAlarmTime();

    // Dialog Data
    enum { IDD = IDD_DLG_ALARM_TIME };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()

public:
    void SetAlarmTime();
    void GetAlarmTime();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnOk();
    afx_msg void OnBnClickedBtnCancel();

public:
    CString m_strDlgTitle;
    BYTE m_byWeekday;
    NET_EHOME_ALARM_TIME_CFG m_struAlarmTimeCfg;

public:
    CComboBox m_cmbAlarmInWeekDay;
    BYTE m_byHour11;
    BYTE m_byMin11;
    BYTE m_byHour12;
    BYTE m_byMin12;
    BYTE m_byHour21;
    BYTE m_byMin21;
    BYTE m_byHour22;
    BYTE m_byMin22;
    BYTE m_byHour31;
    BYTE m_byMin31;
    BYTE m_byHour32;
    BYTE m_byMin32;
    BYTE m_byHour41;
    BYTE m_byMin41;
    BYTE m_byHour42;
    BYTE m_byMin42;
    BYTE m_byHour51;
    BYTE m_byMin51;
    BYTE m_byHour52;
    BYTE m_byMin52;
    BYTE m_byHour61;
    BYTE m_byMin61;
    BYTE m_byHour62;
    BYTE m_byMin62;
    BYTE m_byHour71;
    BYTE m_byMin71;
    BYTE m_byHour72;
    BYTE m_byMin72;
    BYTE m_byHour81;
    BYTE m_byMin81;
    BYTE m_byHour82;
    BYTE m_byMin82;
    BYTE m_byTimeNum;
};
