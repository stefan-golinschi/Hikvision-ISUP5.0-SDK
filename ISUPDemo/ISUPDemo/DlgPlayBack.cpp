// DlgPlayBack.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgPlayBack.h"
#include "EHomeDemoDlg.h"


// CDlgPlayBack dialog

CDlgPlayBack *g_pPlayBackDlg = NULL;

IMPLEMENT_DYNAMIC(CDlgPlayBack, CDialog)

CDlgPlayBack::CDlgPlayBack(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgPlayBack::IDD, pParent)
{
    g_pPlayBackDlg = this;
}

CDlgPlayBack::~CDlgPlayBack()
{
}

void CDlgPlayBack::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_PLAYBACK, m_tabPlayBack);
}


BEGIN_MESSAGE_MAP(CDlgPlayBack, CDialog)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PLAYBACK, &CDlgPlayBack::OnTcnSelchangeTabPlayback)
END_MESSAGE_MAP()


// CDlgPlayBack message handlers

BOOL CDlgPlayBack::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    char szLan[1024] = {0};
    g_StringLanType(szLan, "录像文件", "Record file");
    m_tabPlayBack.InsertItem(0, szLan);
    m_dlgRecordFile.Create(IDD_DIALOG_REC_FILE,&m_tabPlayBack);
    m_dlgRecordFile.ShowWindow(SW_HIDE);

    g_StringLanType(szLan, "图片", "Picture file");
    m_tabPlayBack.InsertItem(1, szLan);
    m_dlgPicFile.Create(IDD_DIALOG_PIC_FILE, &m_tabPlayBack);
    m_dlgPicFile.ShowWindow(SW_HIDE);

    g_StringLanType(szLan, "流量", "Flow Value");
    m_tabPlayBack.InsertItem(2, szLan);
    m_dlgFlowInfo.Create(IDD_DLG_FLOW_INFO, &m_tabPlayBack);
    m_dlgFlowInfo.ShowWindow(SW_HIDE);

    g_StringLanType(szLan, "设备日志", "Dev Log");
    m_tabPlayBack.InsertItem(3, szLan);
    m_dlgDevLog.Create(IDD_DLG_DEV_LOG, &m_tabPlayBack);
    m_dlgDevLog.ShowWindow(SW_HIDE);

    g_StringLanType(szLan, "报警主机日志", "Alarm Host Log");
    m_tabPlayBack.InsertItem(4, szLan);
    m_dlgAlarmHostLog.Create(IDD_DLG_ALARM_HOST_LOG, &m_tabPlayBack);
    m_dlgAlarmHostLog.ShowWindow(SW_HIDE);

    CRect rect;

    m_tabPlayBack.GetClientRect(&rect);
    rect.top+=20;
    rect.bottom-=5;
    rect.left+=5;
    rect.right-=5;

    m_dlgRecordFile.MoveWindow(&rect);
    m_dlgPicFile.MoveWindow(&rect);
    m_dlgFlowInfo.MoveWindow(&rect);
    m_dlgDevLog.MoveWindow(&rect);
    m_dlgAlarmHostLog.MoveWindow(&rect);

    PlayBackWinUpdate();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgPlayBack::PlayBackWinUpdate()
{
    int iTabIndex = m_tabPlayBack.GetCurSel();
    switch (iTabIndex)
    {
    case 0:
        m_dlgRecordFile.CheckInitParam();
        m_dlgRecordFile.ShowWindow(SW_SHOW);
        m_dlgPicFile.ShowWindow(SW_HIDE);
        m_dlgFlowInfo.ShowWindow(SW_HIDE);
        m_dlgDevLog.ShowWindow(SW_HIDE);
        m_dlgAlarmHostLog.ShowWindow(SW_HIDE);
        break;
    case 1:
        m_dlgPicFile.CheckInitParam();
        m_dlgRecordFile.ShowWindow(SW_HIDE);
        m_dlgPicFile.ShowWindow(SW_SHOW);
        m_dlgFlowInfo.ShowWindow(SW_HIDE);
        m_dlgDevLog.ShowWindow(SW_HIDE);
        m_dlgAlarmHostLog.ShowWindow(SW_HIDE);
        break;
    case 2:
        m_dlgFlowInfo.CheckInitParam();
        m_dlgRecordFile.ShowWindow(SW_HIDE);
        m_dlgPicFile.ShowWindow(SW_HIDE);
        m_dlgFlowInfo.ShowWindow(SW_SHOW);
        m_dlgDevLog.ShowWindow(SW_HIDE);
        m_dlgAlarmHostLog.ShowWindow(SW_HIDE);
        break;
    case 3:
        m_dlgDevLog.CheckInitParam();
        m_dlgRecordFile.ShowWindow(SW_HIDE);
        m_dlgPicFile.ShowWindow(SW_HIDE);
        m_dlgFlowInfo.ShowWindow(SW_HIDE);
        m_dlgDevLog.ShowWindow(SW_SHOW);
        m_dlgAlarmHostLog.ShowWindow(SW_HIDE);
        break;
    case 4:
        m_dlgAlarmHostLog.CheckInitParam();
        m_dlgRecordFile.ShowWindow(SW_HIDE);
        m_dlgPicFile.ShowWindow(SW_HIDE);
        m_dlgFlowInfo.ShowWindow(SW_HIDE);
        m_dlgDevLog.ShowWindow(SW_HIDE);
        m_dlgAlarmHostLog.ShowWindow(SW_SHOW);
        break;
       
    default:
        break;
    }
}



void CDlgPlayBack::OnTcnSelchangeTabPlayback(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    UN_REFERENCED_PARAMETER(pNMHDR)
    UpdateData(TRUE);
    PlayBackWinUpdate();
    *pResult = 0;
}
