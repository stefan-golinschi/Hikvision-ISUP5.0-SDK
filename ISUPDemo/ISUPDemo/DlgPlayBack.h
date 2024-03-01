#pragma once
#include "afxcmn.h"
#include "DlgRecordFile.h"
#include "DlgPicFile.h"
#include "DlgFlowInfo.h"
#include "DlgDevLog.h"
#include "DlgAlarmHostLog.h"


// CDlgPlayBack dialog

class CDlgPlayBack : public CDialog
{
    DECLARE_DYNAMIC(CDlgPlayBack)

public:
    CDlgPlayBack(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgPlayBack();

    // Dialog Data
    enum { IDD = IDD_DLG_PLAY_BACK };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CTabCtrl m_tabPlayBack;
    virtual BOOL OnInitDialog();

    void PlayBackWinUpdate();


    CDlgRecordFile m_dlgRecordFile;
    CDlgPicFile    m_dlgPicFile;
    CDlgFlowInfo m_dlgFlowInfo;
    CDlgDevLog m_dlgDevLog;
    CDlgAlarmHostLog m_dlgAlarmHostLog;
    afx_msg void OnTcnSelchangeTabPlayback(NMHDR *pNMHDR, LRESULT *pResult);
};
