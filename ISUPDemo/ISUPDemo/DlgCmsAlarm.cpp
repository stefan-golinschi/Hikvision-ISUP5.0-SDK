// DlgCmsAlarm.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgCmsAlarm.h"
#include "afxdialogex.h"
#include "HCISUPAlarm.h"


// CDlgCmsAlarm 对话框

IMPLEMENT_DYNAMIC(CDlgCmsAlarm, CDialog)

CDlgCmsAlarm::CDlgCmsAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCmsAlarm::IDD, pParent)
    , m_wPort(9000)
{

}

CDlgCmsAlarm::~CDlgCmsAlarm()
{
}

void CDlgCmsAlarm::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PORT_TRANSMIT, m_wPort);
    DDX_Control(pDX, IDC_IPADDRESS_TRANSMIT, m_IPAddr);
}


BEGIN_MESSAGE_MAP(CDlgCmsAlarm, CDialog)
    ON_BN_CLICKED(IDC_BTN_OPEN, &CDlgCmsAlarm::OnBnClickedBtnOpen)
    ON_BN_CLICKED(IDC_BTN_CLOSE, &CDlgCmsAlarm::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CDlgCmsAlarm 消息处理程序

BOOL CDlgCmsAlarm::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    DWORD dwIP = ntohl(inet_addr("127.0.0.1"));
    m_IPAddr.SetAddress(dwIP);
    return TRUE;
}

void CDlgCmsAlarm::OnBnClickedBtnOpen()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    DWORD dwIP = 0;
    m_IPAddr.GetAddress(dwIP);
    //ALARM 开启监听
    NET_EHOME_ALARM_LISTEN_PARAM struAlarmListenParam = { 0 };
    memcpy(struAlarmListenParam.struAddress.szIP, IPToStr(dwIP), 16);
    struAlarmListenParam.struAddress.wPort = (WORD)m_wPort;
    struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
    struAlarmListenParam.pUserData = g_pMainDlg;
    struAlarmListenParam.byUseCmsPort = 1;
    LONG lCmsAlarm = -1;
    lCmsAlarm = NET_EALARM_StartListen(&struAlarmListenParam);
    if (-1 == lCmsAlarm)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 3, "SNET_EALARM_StartListen Failed");
    }
    else
    {
        g_lCmsAlarm = lCmsAlarm;
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 3, "SNET_EALARM_StartListen succ");
    }

    m_struAmsAddr.dwSize = sizeof(m_struAmsAddr);
    memcpy(m_struAmsAddr.struAddress.szIP, IPToStr(dwIP), 16);
    m_struAmsAddr.struAddress.wPort = (WORD)m_wPort;
    m_struAmsAddr.byEnable = 1;
    //CMS开始接收
    if (!NET_ECMS_SetSDKLocalCfg(AMS_ADDRESS, &m_struAmsAddr))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "open cms-alarm Failed");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "open cms-alarm  Succ");
    }
}


void CDlgCmsAlarm::OnBnClickedBtnClose()
{
    // TODO:  在此添加控件通知处理程序代码
    if (-1 != g_lCmsAlarm)
    {
        if (!NET_EALARM_StopListen(g_lCmsAlarm))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 3, "NET_EALARM_StopListen Failed");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 3, "NET_EALARM_StopListen Succ");
        }
        g_lCmsAlarm = -1;
    }


    m_struAmsAddr.dwSize = sizeof(m_struAmsAddr);
    m_struAmsAddr.byEnable = 0;
    //CMS开始接收
    if (!NET_ECMS_SetSDKLocalCfg(AMS_ADDRESS, &m_struAmsAddr))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "Close cms-alarm Failed");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "Close cms-alarm Succ");
    }
}
