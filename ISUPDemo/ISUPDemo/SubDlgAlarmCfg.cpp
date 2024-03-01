// SubDlgAlarmCfg.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "SubDlgAlarmCfg.h"
#include "DlgAlarmTime.h"
#include "DlgAlarmChanList.h"
#include "Public/Convert/Convert.h"


// CSubDlgAlarmCfg dialog

IMPLEMENT_DYNAMIC(CSubDlgAlarmCfg, CDialog)

CSubDlgAlarmCfg::CSubDlgAlarmCfg(CWnd* pParent /*=NULL*/)
: CDialog(CSubDlgAlarmCfg::IDD, pParent)
, m_iDeviceIndex(-1)
, m_lLoginID(-1)
, m_strAlarmInName(_T(""))
, m_strAlarmOutName(_T(""))
, m_bAlarmOutStatus(FALSE)
, m_iPresetNO(0)
, m_iCuriseNO(0)
, m_iTrackNO(0)
, m_iAnalogAlarmNum(0)
, m_iAnalogChanNum(0)
, m_iDigitalChanNum(0)
, m_dwDeleyTime(0)
{
    memset(&m_struAlarmInCond, 0, sizeof(m_struAlarmInCond));
    m_struAlarmInCond.dwSize = sizeof(m_struAlarmInCond);
    memset(&m_struAlarmInCfg, 0, sizeof(m_struAlarmInCfg));
    m_struAlarmInCfg.dwSize = sizeof(m_struAlarmInCfg);
    memset(&m_struAlarmTimeCond, 0, sizeof(m_struAlarmTimeCond));
    m_struAlarmTimeCond.dwSize = sizeof(m_struAlarmTimeCond);
    memset(&m_struAlarmInTimeCfg, 0, sizeof(m_struAlarmInTimeCfg));
    m_struAlarmInTimeCfg.dwSize = sizeof(m_struAlarmInTimeCfg);
    memset(&m_struAlarmOutTimeCfg, 0, sizeof(m_struAlarmOutTimeCfg));
    m_struAlarmOutTimeCfg.dwSize = sizeof(m_struAlarmOutTimeCfg);
    memset(&m_struAlarmOutCfg, 0, sizeof(m_struAlarmOutCfg));
    m_struAlarmOutCfg.dwSize = sizeof(m_struAlarmOutCfg);
    memset(&m_struAlarmOutStatusCfg, 0, sizeof(m_struAlarmOutStatusCfg));
    m_struAlarmOutStatusCfg.dwSize = sizeof(m_struAlarmOutStatusCfg);
    memset(&m_struManualIOCtrl, 0, sizeof(m_struManualIOCtrl));
    m_struManualIOCtrl.dwSize = sizeof(m_struManualIOCtrl);
}

CSubDlgAlarmCfg::~CSubDlgAlarmCfg()
{
}

void CSubDlgAlarmCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ALARMIN_NO, m_cmbAlarmInNo);
    DDX_Control(pDX, IDC_COMBO_PTZ_CHAN, m_cmbPTZChan);
    DDX_Text(pDX, IDC_EDIT_ALARMIN_NAME, m_strAlarmInName);
    DDX_Control(pDX, IDC_COMBO_ALARMIN_TYPE, m_cmbAlarmInType);
    DDX_Control(pDX, IDC_COMBO_ALARMOUT_NO, m_cmbAlarmOutNo);
    DDX_Text(pDX, IDC_EDIT_ALARMOUT_NAME, m_strAlarmOutName);
    DDX_Control(pDX, IDC_COMBO_ALARMOUT_DELAY, m_cmbAlarmOutDelay);
    DDX_Check(pDX, IDC_CHECK_ALARMOUT_STATUS, m_bAlarmOutStatus);
    DDX_Control(pDX, IDC_COMBO_ALARM_MONITOR, m_cmbMonitorAlarm);
    DDX_Control(pDX, IDC_COMBO_ALARM_VOICE, m_cmbSoundAlarm);
    DDX_Control(pDX, IDC_COMBO_EMAIL, m_cmbEmail);
    DDX_Control(pDX, IDC_COMBO_UPLOAD, m_cmbUpload);
    DDX_Control(pDX, IDC_COMBO_ALARM_OUT, m_cmbAlarmOut);
    DDX_Control(pDX, IDC_COMBO_USEPRESET, m_cmbUsePreset);
    DDX_Control(pDX, IDC_COMBO_CURISE, m_cmbCurise);
    DDX_Control(pDX, IDC_COMBO_USETRACK, m_cmbUseTrack);
    DDX_Text(pDX, IDC_EDIT_NO_PRESET, m_iPresetNO);
    DDX_Text(pDX, IDC_EDIT_NO_CURISE, m_iCuriseNO);
    DDX_Text(pDX, IDC_EDIT_NO_TRACK, m_iTrackNO);
    DDX_Text(pDX, IDC_EDIT_ANALOGALARM_NUM, m_iAnalogAlarmNum);
    DDX_Text(pDX, IDC_EDIT_ANALOGCHAN_NUM, m_iAnalogChanNum);
    DDX_Text(pDX, IDC_EDIT_DIGITALCHAN_NUM, m_iDigitalChanNum);
    DDX_Control(pDX, IDC_COMBO_USEALARMIN, m_cmbUseAlarmIn);
    DDX_Control(pDX, IDC_COMBO_WEEKDAY_IN, m_cmbWeekdayIn);
    DDX_Control(pDX, IDC_COMBO_WEEKDAY_OUT, m_cmbWeekdayOut);
    DDX_Control(pDX, IDC_COMBO_OUTPUT_ENABLE, m_cmbOutEnable);
    DDX_Control(pDX, IDC_COMBO_DELAYTIME, m_cmbDelayTime);
    DDX_Text(pDX, IDC_EDIT_DELAYTIME, m_dwDeleyTime);
}


BEGIN_MESSAGE_MAP(CSubDlgAlarmCfg, CDialog)
    ON_CBN_SELCHANGE(IDC_COMBO_ALARMIN_NO, &CSubDlgAlarmCfg::OnCbnSelchangeComboAlarminNo)
    ON_CBN_SELCHANGE(IDC_COMBO_PTZ_CHAN, &CSubDlgAlarmCfg::OnCbnSelchangeComboPtzChan)
    ON_CBN_SELCHANGE(IDC_COMBO_ALARMOUT_NO, &CSubDlgAlarmCfg::OnCbnSelchangeComboAlarmoutNo)
    ON_BN_CLICKED(IDC_BUTTON_ALARMIN_TIME, &CSubDlgAlarmCfg::OnBnClickedButtonAlarminTime)
    ON_CBN_SELCHANGE(IDC_COMBO_WEEKDAY_IN, &CSubDlgAlarmCfg::OnCbnSelchangeComboWeekdayIn)
    ON_CBN_SELCHANGE(IDC_COMBO_WEEKDAY_OUT, &CSubDlgAlarmCfg::OnCbnSelchangeComboWeekdayOut)
    ON_BN_CLICKED(IDC_BUTTON_ALARMOUT_TIME, &CSubDlgAlarmCfg::OnBnClickedButtonAlarmoutTime)
    ON_BN_CLICKED(IDC_BUTTON_ANALOGALARM_OUT, &CSubDlgAlarmCfg::OnBnClickedButtonAnalogalarmOut)
    ON_BN_CLICKED(IDC_BUTTON_ANALOGCHAN, &CSubDlgAlarmCfg::OnBnClickedButtonAnalogchan)
    ON_BN_CLICKED(IDC_BUTTON_DIGITALCHAN, &CSubDlgAlarmCfg::OnBnClickedButtonDigitalchan)
    ON_CBN_SELCHANGE(IDC_COMBO_OUTPUT_ENABLE, &CSubDlgAlarmCfg::OnCbnSelchangeComboOutputEnable)
    ON_CBN_SELCHANGE(IDC_COMBO_DELAYTIME, &CSubDlgAlarmCfg::OnCbnSelchangeComboDelaytime)
END_MESSAGE_MAP()


// CSubDlgAlarmCfg message handlers

BOOL CSubDlgAlarmCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    
    // TODO:  Add extra initialization here
    CRect rc(0, 0, 0, 0);
    GetParent()->GetClientRect(&rc);
    ((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
    MoveWindow(&rc);

    m_cmbDelayTime.SetCurSel(1);
    m_dwDeleyTime = 5;
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSubDlgAlarmCfg::CheckInitParam()
{
    char szLan[1024] = {0};
    m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (m_iDeviceIndex < 0)
    {
        g_StringLanType(szLan, "请先选择一个设备!", "please select a device first!");
        g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, 0, szLan);
        return FALSE;
    }
    m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    if (m_lLoginID < 0)
    {
        g_StringLanType(szLan, "设备未注册!", "device offline!");
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, szLan);
        return FALSE;
    }
    m_cmbAlarmInNo.ResetContent();
    m_cmbAlarmOutNo.ResetContent();
    m_cmbPTZChan.ResetContent();
    unsigned int i = 0;
    char szAlarmInName[32] = {0};
    for (i = 0; i < g_struDeviceInfo[m_iDeviceIndex].dwAlarmInNum; i++)
    {
        sprintf(szAlarmInName, "AlarmIn %d", i + 1);    //IP报警输入，设备不支持配置
        m_cmbAlarmInNo.AddString(szAlarmInName);
    }
    char szAlarmOutName[32] = {0};
    for (i = 0; i < g_struDeviceInfo[m_iDeviceIndex].dwAlarmOutNum; i++)
    {
        sprintf(szAlarmOutName, "AlarmOut %d", i + 1);
        m_cmbAlarmOutNo.AddString(szAlarmOutName);  //IP报警输出，设备不支持配置
    }
    char szPTZChan[32] = {0};
    for (i = 0; i < g_struDeviceInfo[m_iDeviceIndex].dwAnalogChanNum; i++)
    {
        sprintf(szPTZChan, "Channel %d", i + 1);
        m_cmbPTZChan.AddString(szPTZChan);
    }
    //然后添加IP通道
    for (; i < g_struDeviceInfo[m_iDeviceIndex].dwDeviceChanNum; i++)
    {
        sprintf(szPTZChan, "IPChannel %d", i - g_struDeviceInfo[m_iDeviceIndex].dwAnalogChanNum + 1);
        m_cmbPTZChan.AddString(szPTZChan);
    }
    m_cmbAlarmInNo.SetCurSel(0);
    m_cmbAlarmOutNo.SetCurSel(0);
    m_cmbPTZChan.SetCurSel(0);
    m_cmbWeekdayIn.SetCurSel(0);
    m_cmbWeekdayOut.SetCurSel(0);
    UpdateData(FALSE);
    return TRUE;
}

void CSubDlgAlarmCfg::CurCfgUpdate()
{
    if (!CheckInitParam())
    {
        return ;
    }
    UpdateData(TRUE);
    AlarmInUpdate();
    AlarmOutUpdate();
    UpdateData(FALSE);
}

void CSubDlgAlarmCfg::CurCfgSetup()
{
    UpdateData(TRUE);
    NET_EHOME_CONFIG struCfg = {0};
    //设置报警输入
    m_struAlarmInCond.dwSize = sizeof(m_struAlarmInCond);
    m_struAlarmInCond.dwAlarmInNum = m_cmbAlarmInNo.GetCurSel() + 1;
    m_struAlarmInCond.dwPTZChan = m_cmbPTZChan.GetCurSel() + 1;
    struCfg.pCondBuf = &m_struAlarmInCond;
    struCfg.dwCondSize = sizeof(m_struAlarmInCond);
    memset(m_struAlarmInCfg.sAlarmInName, 0, sizeof(m_struAlarmInCfg.sAlarmInName));//原先的数据不清空，没被替换完的数据会被设置到设备里面
    memcpy(m_struAlarmInCfg.sAlarmInName, m_strAlarmInName.GetBuffer(0), m_strAlarmInName.GetLength());
    m_struAlarmInCfg.byAlarmInType = (BYTE)m_cmbAlarmInType.GetCurSel();
    m_struAlarmInCfg.byUseAlarmIn = (BYTE)m_cmbUseAlarmIn.GetCurSel();
    m_struAlarmInCfg.struLinkageType.byMonitorAlarm = (BYTE)m_cmbMonitorAlarm.GetCurSel();
    m_struAlarmInCfg.struLinkageType.bySoundAlarm = (BYTE)m_cmbSoundAlarm.GetCurSel();
    m_struAlarmInCfg.struLinkageType.byUpload = (BYTE)m_cmbUpload.GetCurSel();
    m_struAlarmInCfg.struLinkageType.byEmail = (BYTE)m_cmbEmail.GetCurSel();
    m_struAlarmInCfg.struLinkageType.byAlarmout = (BYTE)m_cmbAlarmOut.GetCurSel();
    m_struAlarmInCfg.struLinkageType.struPTZLinkage.byUseCurise = (BYTE)m_cmbCurise.GetCurSel();
    m_struAlarmInCfg.struLinkageType.struPTZLinkage.byUsePreset = (BYTE)m_cmbUsePreset.GetCurSel();
    m_struAlarmInCfg.struLinkageType.struPTZLinkage.byUseTrack = (BYTE)m_cmbUseTrack.GetCurSel();
    m_struAlarmInCfg.struLinkageType.struPTZLinkage.wCuriseNo = (WORD)m_iCuriseNO;
    m_struAlarmInCfg.struLinkageType.struPTZLinkage.wPresetNo = (WORD)m_iPresetNO;
    m_struAlarmInCfg.struLinkageType.struPTZLinkage.wTrackNo = (WORD)m_iTrackNO;

    //需要将字符串字段转换成UTF-8
    DWORD dwConvertLen = 0;
    A2UTF8((char*)m_struAlarmInCfg.sAlarmInName, (char*)m_struAlarmInCfg.sAlarmInName, NAME_LEN, &dwConvertLen);

    struCfg.pInBuf = &m_struAlarmInCfg;
    struCfg.dwInSize = sizeof(m_struAlarmInCfg);

    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_ALARMIN_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_ALARMIN_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_ALARMIN_CFG");
    }
    //设置报警输入布防时间
    m_struAlarmTimeCond.dwSize = sizeof(m_struAlarmTimeCond);
    m_struAlarmTimeCond.byAlarmType = 3;
    m_struAlarmTimeCond.dwChannel = m_cmbAlarmInNo.GetCurSel() + 1;
    m_struAlarmTimeCond.byWeekday = (BYTE)m_cmbWeekdayIn.GetCurSel();

    //目前没有要进行字符转换的字段

    struCfg.pCondBuf = &m_struAlarmTimeCond;
    struCfg.dwCondSize = sizeof(m_struAlarmTimeCond);
    struCfg.pInBuf = &m_struAlarmInTimeCfg;
    struCfg.dwInSize = sizeof(m_struAlarmInTimeCfg);
    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_ALARM_TIME_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_ALARM_TIME_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_ALARM_TIME_CFG");
    }
    //设置报警输出
    DWORD dwAlarmOutNo = m_cmbAlarmOutNo.GetCurSel() + 1;
    struCfg.pCondBuf = &dwAlarmOutNo;
    struCfg.dwCondSize = sizeof(dwAlarmOutNo);
    memset(m_struAlarmOutCfg.sAlarmOutName, NAME_LEN, 0);
    char szName[NAME_LEN] = { 0 };
    memcpy(szName, m_strAlarmOutName.GetBuffer(0), min(m_strAlarmOutName.GetLength(), NAME_LEN - 1));
    m_struAlarmOutCfg.wAlarmOutDelay = (WORD)m_cmbAlarmOutDelay.GetCurSel();

    //需要将字符串字段转换成UTF-8
    A2UTF8(szName, (char*)m_struAlarmOutCfg.sAlarmOutName, NAME_LEN, &dwConvertLen);

    struCfg.pInBuf = &m_struAlarmOutCfg;
    struCfg.dwInSize = sizeof(m_struAlarmOutCfg);
    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_ALARMOUT_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_ALARMOUT_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_ALARMOUT_CFG");
    }
    ////设置报警输出状态 不支持设置，注释掉
    //m_struAlarmOutStatusCfg.byAlarmOutStatus = (BYTE)m_bAlarmOutStatus;

    ////目前没有要进行字符转换的字段

    //struCfg.pInBuf = &m_struAlarmOutStatusCfg;
    //struCfg.dwInSize = sizeof(m_struAlarmOutStatusCfg);
    //if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_ALARMOUT_STATUS_CFG, &struCfg, sizeof(struCfg)))
    //{
    //    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_ALARMOUT_STATUS_CFG");
    //}
    //else
    //{
    //    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_ALARMOUT_STATUS_CFG");
    //}
    //设置报警输出布防时间
    m_struAlarmTimeCond.dwSize = sizeof(m_struAlarmTimeCond);
    m_struAlarmTimeCond.byAlarmType = 4;
    m_struAlarmTimeCond.dwChannel = m_cmbAlarmOutNo.GetCurSel() + 1;
    m_struAlarmTimeCond.byWeekday = (BYTE)m_cmbWeekdayOut.GetCurSel();

    //目前没有要进行字符转换的字段

    struCfg.pCondBuf = &m_struAlarmTimeCond;
    struCfg.dwCondSize = sizeof(m_struAlarmTimeCond);
    struCfg.pInBuf = &m_struAlarmOutTimeCfg;
    struCfg.dwInSize = sizeof(m_struAlarmOutTimeCfg);
    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_ALARM_TIME_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_ALARM_TIME_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_ALARM_TIME_CFG");
    }
}

void CSubDlgAlarmCfg::AlarmInUpdate()
{
    NET_EHOME_CONFIG struCfg = {0};
    //报警输入
    m_struAlarmInCond.dwSize = sizeof(m_struAlarmInCond);
    m_struAlarmInCond.dwAlarmInNum = m_cmbAlarmInNo.GetCurSel() + 1;
    m_struAlarmInCond.dwPTZChan = m_cmbPTZChan.GetCurSel() + 1;
    struCfg.pCondBuf = &m_struAlarmInCond;
    struCfg.dwCondSize = sizeof(m_struAlarmInCond);
    struCfg.pOutBuf = &m_struAlarmInCfg;
    struCfg.dwOutSize = sizeof(m_struAlarmInCfg);
    memset(&m_struAlarmInCfg, 0 , sizeof(m_struAlarmInCfg));
    m_struAlarmInCfg.dwSize = sizeof(m_struAlarmInCfg);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_ALARMIN_CFG, &struCfg, sizeof(struCfg)))
    {
        //需要将sAlarmInName转换成GB2312
        DWORD dwConvertLen = 0;
        UTF82A((char*)m_struAlarmInCfg.sAlarmInName, (char*)m_struAlarmInCfg.sAlarmInName, NAME_LEN, &dwConvertLen);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_ALARMIN_CFG");
        m_strAlarmInName = m_struAlarmInCfg.sAlarmInName;
        m_cmbAlarmInType.SetCurSel(m_struAlarmInCfg.byAlarmInType);
        m_cmbUseAlarmIn.SetCurSel(m_struAlarmInCfg.byUseAlarmIn);
        m_cmbMonitorAlarm.SetCurSel(m_struAlarmInCfg.struLinkageType.byMonitorAlarm);
        m_cmbSoundAlarm.SetCurSel(m_struAlarmInCfg.struLinkageType.bySoundAlarm);
        m_cmbUpload.SetCurSel(m_struAlarmInCfg.struLinkageType.byUpload);
        m_cmbEmail.SetCurSel(m_struAlarmInCfg.struLinkageType.byEmail);
        m_cmbAlarmOut.SetCurSel(m_struAlarmInCfg.struLinkageType.byAlarmout);
        m_cmbUsePreset.SetCurSel(m_struAlarmInCfg.struLinkageType.struPTZLinkage.byUsePreset);
        m_cmbUseTrack.SetCurSel(m_struAlarmInCfg.struLinkageType.struPTZLinkage.byUseTrack);
        m_cmbCurise.SetCurSel(m_struAlarmInCfg.struLinkageType.struPTZLinkage.byUseCurise);
        m_iPresetNO = m_struAlarmInCfg.struLinkageType.struPTZLinkage.wPresetNo;
        m_iCuriseNO = m_struAlarmInCfg.struLinkageType.struPTZLinkage.wCuriseNo;
        m_iTrackNO = m_struAlarmInCfg.struLinkageType.struPTZLinkage.wTrackNo;
        m_iAnalogAlarmNum = m_struAlarmInCfg.struLinkageType.struAlarmOut.dwAnalogAlarmOutNum;
        m_iAnalogChanNum = m_struAlarmInCfg.struRecordChan.byAnalogChanNum;
        m_iDigitalChanNum = m_struAlarmInCfg.struRecordChan.wDigitChanNum;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_ALARMIN_CFG");
    }
    //报警输入布防时间
    AlarmTimeUpdateIn();
}

void CSubDlgAlarmCfg::AlarmTimeUpdateIn()
{
    NET_EHOME_CONFIG struCfg = {0};
    //报警输入布防时间
    m_struAlarmTimeCond.dwSize = sizeof(m_struAlarmTimeCond);
    m_struAlarmTimeCond.byAlarmType = 3;
    m_struAlarmTimeCond.dwChannel = m_cmbAlarmInNo.GetCurSel() + 1;
    m_struAlarmTimeCond.byWeekday = (BYTE)m_cmbWeekdayIn.GetCurSel();
    struCfg.pCondBuf = &m_struAlarmTimeCond;
    struCfg.dwCondSize = sizeof(m_struAlarmTimeCond);
    struCfg.pOutBuf = &m_struAlarmInTimeCfg;
    struCfg.dwOutSize = sizeof(m_struAlarmInTimeCfg);
    memset(&m_struAlarmInTimeCfg, 0 , sizeof(m_struAlarmInTimeCfg));
    m_struAlarmInTimeCfg.dwSize = sizeof(m_struAlarmInTimeCfg);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_ALARM_TIME_CFG, &struCfg, sizeof(struCfg)))
    {
        //没有需要进行字符转码的字段
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_ALARM_TIME_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_ALARM_TIME_CFG");
    }
}

void CSubDlgAlarmCfg::AlarmTimeUpdateOut()
{
    NET_EHOME_CONFIG struCfg = {0};
    //报警输出布防时间
    m_struAlarmTimeCond.dwSize = sizeof(m_struAlarmTimeCond);
    m_struAlarmTimeCond.byAlarmType = 4;
    m_struAlarmTimeCond.dwChannel = m_cmbAlarmOutNo.GetCurSel() + 1;
    m_struAlarmTimeCond.byWeekday = (BYTE)m_cmbWeekdayOut.GetCurSel();
    struCfg.pCondBuf = &m_struAlarmTimeCond;
    struCfg.dwCondSize = sizeof(m_struAlarmTimeCond);
    struCfg.pOutBuf = &m_struAlarmOutTimeCfg;
    struCfg.dwOutSize = sizeof(m_struAlarmOutTimeCfg);
    memset(&m_struAlarmOutTimeCfg, 0 , sizeof(m_struAlarmOutTimeCfg));
    m_struAlarmOutTimeCfg.dwSize = sizeof(m_struAlarmOutTimeCfg);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_ALARM_TIME_CFG, &struCfg, sizeof(struCfg)))
    {
        //没有需要进行字符转码的字段
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_ALARM_TIME_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_ALARM_TIME_CFG");
    }
}

void CSubDlgAlarmCfg::AlarmOutUpdate()
{
    NET_EHOME_CONFIG struCfg = {0};
    //报警输出
    DWORD dwAlarmOutNo = m_cmbAlarmOutNo.GetCurSel() + 1;
    struCfg.pCondBuf = &dwAlarmOutNo;
    struCfg.dwCondSize = sizeof(dwAlarmOutNo);
    struCfg.pOutBuf = &m_struAlarmOutCfg;
    struCfg.dwOutSize = sizeof(m_struAlarmOutCfg);
    memset(&m_struAlarmOutCfg, 0, sizeof(m_struAlarmOutCfg));
    m_struAlarmOutCfg.dwSize = sizeof(m_struAlarmOutCfg);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_ALARMOUT_CFG, &struCfg, sizeof(struCfg)))
    {
        //需要将sAlarmOutName转换成GB2312
        DWORD dwConvertLen = 0;
        UTF82A((char*)m_struAlarmOutCfg.sAlarmOutName, (char*)m_struAlarmOutCfg.sAlarmOutName, NAME_LEN, &dwConvertLen);
        
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_ALARMOUT_CFG");
        m_strAlarmOutName = m_struAlarmOutCfg.sAlarmOutName;
        m_cmbAlarmOutDelay.SetCurSel(m_struAlarmOutCfg.wAlarmOutDelay);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_ALARMOUT_CFG");
    }
    ////报警输出状态 设备不支持注释掉
    //struCfg.pOutBuf = &m_struAlarmOutStatusCfg;
    //struCfg.dwOutSize = sizeof(m_struAlarmOutStatusCfg);
    //memset(&m_struAlarmOutStatusCfg, 0, sizeof(m_struAlarmOutStatusCfg));
    //m_struAlarmOutStatusCfg.dwSize = sizeof(m_struAlarmOutStatusCfg);
    //if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_ALARMOUT_STATUS_CFG, &struCfg, sizeof(struCfg)))
    //{
    //    //目前没有要进行字符转换的字段
    //    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_ALARMOUT_STATUS_CFG");
    //    m_bAlarmOutStatus = m_struAlarmOutStatusCfg.byAlarmOutStatus;
    //}
    //else
    //{
    //    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_ALARMOUT_STATUS_CFG");
    //}
    //报警输出布防时间
    UpdateData(FALSE);
    AlarmTimeUpdateOut();
}

void CSubDlgAlarmCfg::OnCbnSelchangeComboAlarminNo()
{
    // TODO: Add your control notification handler code here
    AlarmInUpdate();
}

void CSubDlgAlarmCfg::OnCbnSelchangeComboPtzChan()
{
    // TODO: Add your control notification handler code here
    AlarmInUpdate();
}

void CSubDlgAlarmCfg::OnCbnSelchangeComboAlarmoutNo()
{
    // TODO: Add your control notification handler code here
    AlarmOutUpdate();
}

void CSubDlgAlarmCfg::ManualIOCtrl(BOOL bStart)
{
    //if (!CheckInitParam())
    //{
    //    return;
    //}
    UpdateData(TRUE);
    m_struManualIOCtrl.dwSize = sizeof(m_struManualIOCtrl);
    m_struManualIOCtrl.dwChan = m_cmbAlarmOutNo.GetCurSel() + 1;
    m_struManualIOCtrl.byAction = (BYTE)bStart;
    if (m_cmbDelayTime.GetCurSel() == 0)
    {
        m_struManualIOCtrl.dwDelayTime = 0;
    }
    else
    {
        m_struManualIOCtrl.dwDelayTime = m_dwDeleyTime;
    }
    NET_EHOME_REMOTE_CTRL_PARAM struRemoteCtrl = {0};
    struRemoteCtrl.dwSize = sizeof(struRemoteCtrl);
    struRemoteCtrl.lpInbuffer = &m_struManualIOCtrl;
    struRemoteCtrl.dwInBufferSize = sizeof(m_struManualIOCtrl);

    //目前没有要进行字符转换的字段

    if (NET_ECMS_RemoteControl(m_lLoginID, NET_EHOME_MANUAL_IOOUT, &struRemoteCtrl))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_MANUAL_IOOUT");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_MANUAL_IOOUT");
    }
}

void CSubDlgAlarmCfg::OnBnClickedButtonAlarminTime()
{
    // TODO: 在此添加控件通知处理程序代码
    CDlgAlarmTime dlgAlarmTime;
    char szDlgTitle[1024] = {0};
    g_StringLanType(szDlgTitle, _T("报警输入布防时间"), _T("Alarm Input Time"));
    dlgAlarmTime.m_strDlgTitle = szDlgTitle;
    dlgAlarmTime.m_byWeekday = (BYTE)m_cmbWeekdayIn.GetCurSel();
    memcpy(&dlgAlarmTime.m_struAlarmTimeCfg, &m_struAlarmInTimeCfg, sizeof(m_struAlarmInTimeCfg));
    if (dlgAlarmTime.DoModal() != IDOK)
    {
        return;
    }
    memcpy(&m_struAlarmInTimeCfg, &dlgAlarmTime.m_struAlarmTimeCfg, sizeof(m_struAlarmInTimeCfg));
}

void CSubDlgAlarmCfg::OnCbnSelchangeComboWeekdayIn()
{
    // TODO: 在此添加控件通知处理程序代码
    AlarmTimeUpdateIn();
}

void CSubDlgAlarmCfg::OnCbnSelchangeComboWeekdayOut()
{
    // TODO: 在此添加控件通知处理程序代码
    AlarmTimeUpdateOut();
}

void CSubDlgAlarmCfg::OnBnClickedButtonAlarmoutTime()
{
    // TODO: 在此添加控件通知处理程序代码
    CDlgAlarmTime dlgAlarmTime;
    char szDlgTitle[1024] = {0};
    g_StringLanType(szDlgTitle, _T("报警输出布防时间"), _T("Alarm Output Time"));
    dlgAlarmTime.m_strDlgTitle = szDlgTitle;
    dlgAlarmTime.m_byWeekday = (BYTE)m_cmbWeekdayOut.GetCurSel();
    memcpy(&dlgAlarmTime.m_struAlarmTimeCfg, &m_struAlarmOutTimeCfg, sizeof(m_struAlarmOutTimeCfg));
    if (dlgAlarmTime.DoModal() != IDOK)
    {
        return;
    }
    memcpy(&m_struAlarmOutTimeCfg, &dlgAlarmTime.m_struAlarmTimeCfg, sizeof(m_struAlarmOutTimeCfg));
}

void CSubDlgAlarmCfg::OnBnClickedButtonAnalogalarmOut()
{
    // TODO: 在此添加控件通知处理程序代码
    CDlgAlarmChanList dlgList;
    char szDlgTitle[1024] = {0};
    g_StringLanType(szDlgTitle, _T("模拟报警输出"), _T("Analog Alarm Output"));
    dlgList.m_strDlgTitle = szDlgTitle;
    dlgList.m_byDataType = 0;
    dlgList.m_byNum = (BYTE)m_struAlarmInCfg.struLinkageType.struAlarmOut.dwAnalogAlarmOutNum;
    memcpy(dlgList.m_byAnalogAlarmOut, m_struAlarmInCfg.struLinkageType.struAlarmOut.byAnalogAlarmOut, MAX_ANALOG_ALARMOUT);
    if (dlgList.DoModal() != IDOK)
    {
        return;
    }
    memcpy(m_struAlarmInCfg.struLinkageType.struAlarmOut.byAnalogAlarmOut, dlgList.m_byAnalogAlarmOut, MAX_ANALOG_ALARMOUT);
}

void CSubDlgAlarmCfg::OnBnClickedButtonAnalogchan()
{
    // TODO: 在此添加控件通知处理程序代码
    CDlgAlarmChanList dlgList;
    char szDlgTitle[1024] = {0};
    g_StringLanType(szDlgTitle, _T("录像模拟通道"), _T("Record Analog Channel"));
    dlgList.m_strDlgTitle = szDlgTitle;
    dlgList.m_byDataType = 1;
    dlgList.m_byNum = m_struAlarmInCfg.struRecordChan.byAnalogChanNum;
    memcpy(dlgList.m_byAnalogAlarmChan, m_struAlarmInCfg.struRecordChan.byAnalogChan, MAX_ANALOG_CHANNUM);
    if (dlgList.DoModal() != IDOK)
    {
        return;
    }
    memcpy(m_struAlarmInCfg.struRecordChan.byAnalogChan, dlgList.m_byAnalogAlarmChan, MAX_ANALOG_CHANNUM);
}

void CSubDlgAlarmCfg::OnBnClickedButtonDigitalchan()
{
    // TODO: 在此添加控件通知处理程序代码
    CDlgAlarmChanList dlgList;
    char szDlgTitle[1024] = {0};
    g_StringLanType(szDlgTitle, _T("录像数字通道"), _T("Record Digital Channel"));
    dlgList.m_strDlgTitle = szDlgTitle;
    dlgList.m_byDataType = 2;
    dlgList.m_byNum = (BYTE)m_struAlarmInCfg.struRecordChan.wDigitChanNum;
    memcpy(dlgList.m_byDigitalAlarmChan, m_struAlarmInCfg.struRecordChan.byDigitChan, MAX_DIGIT_CHANNUM);
    if (dlgList.DoModal() != IDOK)
    {
        return;
    }
    memcpy(m_struAlarmInCfg.struRecordChan.byDigitChan, dlgList.m_byDigitalAlarmChan, MAX_DIGIT_CHANNUM);
}

void CSubDlgAlarmCfg::OnCbnSelchangeComboOutputEnable()
{
    // TODO: 在此添加控件通知处理程序代码
    ManualIOCtrl(m_cmbOutEnable.GetCurSel());
}

void CSubDlgAlarmCfg::OnCbnSelchangeComboDelaytime()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_cmbDelayTime.GetCurSel() == 0)
    {
        GetDlgItem(IDC_EDIT_DELAYTIME)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_DELAYTIME)->EnableWindow(TRUE);
    }
    ManualIOCtrl(m_cmbOutEnable.GetCurSel());
}

