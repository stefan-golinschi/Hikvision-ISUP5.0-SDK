#pragma once
#include "afxcmn.h"


// CDlgAlarmChanList 对话框

class CDlgAlarmChanList : public CDialog
{
    DECLARE_DYNAMIC(CDlgAlarmChanList)

public:
    CDlgAlarmChanList(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgAlarmChanList();

// 对话框数据
    enum { IDD = IDD_DLG_ALARM_TREE };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnOk();
    afx_msg void OnBnClickedBtnCancel();
    void InitTreeList();
    void GetDataFromTreeList();

public:
    CString m_strDlgTitle;
    BYTE m_byDataType; //0-报警输出，1-录像模拟通道，2-录像数字通道
    BYTE m_byNum;
    BYTE m_byAnalogAlarmOut[MAX_ANALOG_ALARMOUT];
    BYTE m_byAnalogAlarmChan[MAX_ANALOG_CHANNUM];
    BYTE m_byDigitalAlarmChan[MAX_DIGIT_CHANNUM];
    DWORD m_dwDigitalAlarmChan;

public:
    CTreeCtrl m_treeAlarmChan;
    afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnUpdateMenuWirelessInfo(CCmdUI *pCmdUI);
};
