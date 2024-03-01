// DlgAlarmTime.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgAlarmTime.h"


// CDlgAlarmTime dialog

IMPLEMENT_DYNAMIC(CDlgAlarmTime, CDialog)

CDlgAlarmTime::CDlgAlarmTime(CWnd* pParent /*=NULL*/)
: CDialog(CDlgAlarmTime::IDD, pParent)
, m_byHour11(0)
, m_byMin11(0)
, m_byHour12(0)
, m_byMin12(0)
, m_byHour21(0)
, m_byMin21(0)
, m_byHour22(0)
, m_byMin22(0)
, m_byHour31(0)
, m_byMin31(0)
, m_byHour32(0)
, m_byMin32(0)
, m_byHour41(0)
, m_byMin41(0)
, m_byHour42(0)
, m_byMin42(0)
, m_byHour51(0)
, m_byMin51(0)
, m_byHour52(0)
, m_byMin52(0)
, m_byHour61(0)
, m_byMin61(0)
, m_byHour62(0)
, m_byMin62(0)
, m_byHour71(0)
, m_byMin71(0)
, m_byHour72(0)
, m_byMin72(0)
, m_byHour81(0)
, m_byMin81(0)
, m_byHour82(0)
, m_byMin82(0)
, m_byTimeNum(0)
{
    m_strDlgTitle = _T("");
    m_byWeekday = (BYTE)-1;
    memset(&m_struAlarmTimeCfg, 0, sizeof(m_struAlarmTimeCfg));
    m_struAlarmTimeCfg.dwSize = sizeof(m_struAlarmTimeCfg);
}

CDlgAlarmTime::~CDlgAlarmTime()
{
}

void CDlgAlarmTime::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ALARM_IN_WEEKDAY, m_cmbAlarmInWeekDay);
    DDX_Text(pDX, IDC_EDIT_HOUR11, m_byHour11);
    DDX_Text(pDX, IDC_EDIT_MIN11, m_byMin11);
    DDX_Text(pDX, IDC_EDIT_HOUR12, m_byHour12);
    DDX_Text(pDX, IDC_EDIT_MIN12, m_byMin12);
    DDX_Text(pDX, IDC_EDIT_HOUR21, m_byHour21);
    DDX_Text(pDX, IDC_EDIT_MIN21, m_byMin21);
    DDX_Text(pDX, IDC_EDIT_HOUR22, m_byHour22);
    DDX_Text(pDX, IDC_EDIT_MIN22, m_byMin22);
    DDX_Text(pDX, IDC_EDIT_HOUR31, m_byHour31);
    DDX_Text(pDX, IDC_EDIT_MIN31, m_byMin31);
    DDX_Text(pDX, IDC_EDIT_HOUR32, m_byHour32);
    DDX_Text(pDX, IDC_EDIT_MIN32, m_byMin32);
    DDX_Text(pDX, IDC_EDIT_HOUR41, m_byHour41);
    DDX_Text(pDX, IDC_EDIT_MIN41, m_byMin41);
    DDX_Text(pDX, IDC_EDIT_HOUR42, m_byHour42);
    DDX_Text(pDX, IDC_EDIT_MIN42, m_byMin42);
    DDX_Text(pDX, IDC_EDIT_HOUR51, m_byHour51);
    DDX_Text(pDX, IDC_EDIT_MIN51, m_byMin51);
    DDX_Text(pDX, IDC_EDIT_HOUR52, m_byHour52);
    DDX_Text(pDX, IDC_EDIT_MIN52, m_byMin52);
    DDX_Text(pDX, IDC_EDIT_HOUR61, m_byHour61);
    DDX_Text(pDX, IDC_EDIT_MIN61, m_byMin61);
    DDX_Text(pDX, IDC_EDIT_HOUR62, m_byHour62);
    DDX_Text(pDX, IDC_EDIT_MIN62, m_byMin62);
    DDX_Text(pDX, IDC_EDIT_HOUR71, m_byHour71);
    DDX_Text(pDX, IDC_EDIT_MIN71, m_byMin71);
    DDX_Text(pDX, IDC_EDIT_HOUR72, m_byHour72);
    DDX_Text(pDX, IDC_EDIT_MIN72, m_byMin72);
    DDX_Text(pDX, IDC_EDIT_HOUR81, m_byHour81);
    DDX_Text(pDX, IDC_EDIT_MIN81, m_byMin81);
    DDX_Text(pDX, IDC_EDIT_HOUR82, m_byHour82);
    DDX_Text(pDX, IDC_EDIT_MIN82, m_byMin82);
    DDX_Text(pDX, IDC_EDIT_TIME_NUM, m_byTimeNum);
}

BEGIN_MESSAGE_MAP(CDlgAlarmTime, CDialog)
    ON_BN_CLICKED(IDC_BTN_OK, &CDlgAlarmTime::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgAlarmTime::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgAlarmTime message handlers

void CDlgAlarmTime::SetAlarmTime()
{
    m_cmbAlarmInWeekDay.SetCurSel(m_byWeekday);

    m_byTimeNum = m_struAlarmTimeCfg.bySchedTimeCount;

    m_byHour11 = m_struAlarmTimeCfg.struSchedTime[0].byStartHour;
    m_byHour12 = m_struAlarmTimeCfg.struSchedTime[0].byStopHour;
    m_byMin11 = m_struAlarmTimeCfg.struSchedTime[0].byStartMin;
    m_byMin12 = m_struAlarmTimeCfg.struSchedTime[0].byStopMin;

    m_byHour21 = m_struAlarmTimeCfg.struSchedTime[1].byStartHour;
    m_byHour22 = m_struAlarmTimeCfg.struSchedTime[1].byStopHour;
    m_byMin21 = m_struAlarmTimeCfg.struSchedTime[1].byStartMin;
    m_byMin22 = m_struAlarmTimeCfg.struSchedTime[1].byStopMin;

    m_byHour31 = m_struAlarmTimeCfg.struSchedTime[2].byStartHour;
    m_byHour32 = m_struAlarmTimeCfg.struSchedTime[2].byStopHour;
    m_byMin31 = m_struAlarmTimeCfg.struSchedTime[2].byStartMin;
    m_byMin32 = m_struAlarmTimeCfg.struSchedTime[2].byStopMin;

    m_byHour41 = m_struAlarmTimeCfg.struSchedTime[3].byStartHour;
    m_byHour42 = m_struAlarmTimeCfg.struSchedTime[3].byStopHour;
    m_byMin41 = m_struAlarmTimeCfg.struSchedTime[3].byStartMin;
    m_byMin42 = m_struAlarmTimeCfg.struSchedTime[3].byStopMin;

    m_byHour51 = m_struAlarmTimeCfg.struSchedTime[4].byStartHour;
    m_byHour52 = m_struAlarmTimeCfg.struSchedTime[4].byStopHour;
    m_byMin51 = m_struAlarmTimeCfg.struSchedTime[4].byStartMin;
    m_byMin52 = m_struAlarmTimeCfg.struSchedTime[4].byStopMin;

    m_byHour61 = m_struAlarmTimeCfg.struSchedTime[5].byStartHour;
    m_byHour62 = m_struAlarmTimeCfg.struSchedTime[5].byStopHour;
    m_byMin61 = m_struAlarmTimeCfg.struSchedTime[5].byStartMin;
    m_byMin62 = m_struAlarmTimeCfg.struSchedTime[5].byStopMin;

    m_byHour71 = m_struAlarmTimeCfg.struSchedTime[6].byStartHour;
    m_byHour72 = m_struAlarmTimeCfg.struSchedTime[6].byStopHour;
    m_byMin71 = m_struAlarmTimeCfg.struSchedTime[6].byStartMin;
    m_byMin72 = m_struAlarmTimeCfg.struSchedTime[6].byStopMin;

    m_byHour81 = m_struAlarmTimeCfg.struSchedTime[7].byStartHour;
    m_byHour82 = m_struAlarmTimeCfg.struSchedTime[7].byStopHour;
    m_byMin81 = m_struAlarmTimeCfg.struSchedTime[7].byStartMin;
    m_byMin82 = m_struAlarmTimeCfg.struSchedTime[7].byStopMin;

    UpdateData(FALSE);
}

void CDlgAlarmTime::GetAlarmTime()
{
    UpdateData(TRUE);

    m_struAlarmTimeCfg.struSchedTime[0].byStartHour = m_byHour11;
    m_struAlarmTimeCfg.struSchedTime[0].byStopHour = m_byHour12;
    m_struAlarmTimeCfg.struSchedTime[0].byStartMin = m_byMin11;
    m_struAlarmTimeCfg.struSchedTime[0].byStopMin = m_byMin12;

    m_struAlarmTimeCfg.struSchedTime[1].byStartHour = m_byHour21;
    m_struAlarmTimeCfg.struSchedTime[1].byStopHour = m_byHour22;
    m_struAlarmTimeCfg.struSchedTime[1].byStartMin = m_byMin21;
    m_struAlarmTimeCfg.struSchedTime[1].byStopMin = m_byMin22;

    m_struAlarmTimeCfg.struSchedTime[2].byStartHour = m_byHour31;
    m_struAlarmTimeCfg.struSchedTime[2].byStopHour = m_byHour32;
    m_struAlarmTimeCfg.struSchedTime[2].byStartMin = m_byMin31;
    m_struAlarmTimeCfg.struSchedTime[2].byStopMin = m_byMin32;

    m_struAlarmTimeCfg.struSchedTime[3].byStartHour = m_byHour41;
    m_struAlarmTimeCfg.struSchedTime[3].byStopHour = m_byHour42;
    m_struAlarmTimeCfg.struSchedTime[3].byStartMin = m_byMin41;
    m_struAlarmTimeCfg.struSchedTime[3].byStopMin = m_byMin42;

    m_struAlarmTimeCfg.struSchedTime[4].byStartHour = m_byHour51;
    m_struAlarmTimeCfg.struSchedTime[4].byStopHour = m_byHour52;
    m_struAlarmTimeCfg.struSchedTime[4].byStartMin = m_byMin51;
    m_struAlarmTimeCfg.struSchedTime[4].byStopMin = m_byMin52;

    m_struAlarmTimeCfg.struSchedTime[5].byStartHour = m_byHour61;
    m_struAlarmTimeCfg.struSchedTime[5].byStopHour = m_byHour62;
    m_struAlarmTimeCfg.struSchedTime[5].byStartMin = m_byMin61;
    m_struAlarmTimeCfg.struSchedTime[5].byStopMin = m_byMin62;

    m_struAlarmTimeCfg.struSchedTime[6].byStartHour = m_byHour71;
    m_struAlarmTimeCfg.struSchedTime[6].byStopHour = m_byHour72;
    m_struAlarmTimeCfg.struSchedTime[6].byStartMin = m_byMin71;
    m_struAlarmTimeCfg.struSchedTime[6].byStopMin = m_byMin72;

    m_struAlarmTimeCfg.struSchedTime[7].byStartHour = m_byHour81;
    m_struAlarmTimeCfg.struSchedTime[7].byStopHour = m_byHour82;
    m_struAlarmTimeCfg.struSchedTime[7].byStartMin = m_byMin81;
    m_struAlarmTimeCfg.struSchedTime[7].byStopMin = m_byMin82;
}
BOOL CDlgAlarmTime::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    SetWindowText(m_strDlgTitle);
    SetAlarmTime();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgAlarmTime::OnBnClickedBtnOk()
{
    // TODO: 在此添加控件通知处理程序代码
    GetAlarmTime();
    CDialog::OnOK();
}

void CDlgAlarmTime::OnBnClickedBtnCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialog::OnCancel();
}
