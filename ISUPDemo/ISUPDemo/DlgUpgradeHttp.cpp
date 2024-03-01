// DlgUpgradeHttp.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgUpgradeHttp.h"
#include "afxdialogex.h"
#include "public/cjson/cJson.h"

#define CYCLE_GETPROGRESS_TIMER        1        //get progress timer

// CDlgUpgradeHttp 对话框

IMPLEMENT_DYNAMIC(CDlgUpgradeHttp, CDialogEx)

CDlgUpgradeHttp::CDlgUpgradeHttp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgUpgradeHttp::IDD, pParent)
    , m_csPassword(_T(""))
    , m_dwPort(0)
    , m_csFileName(_T(""))
    , m_sOutput(_T(""))
{
    m_lLoginID = -1;
    m_iDeviceIndex = -1;
}

CDlgUpgradeHttp::~CDlgUpgradeHttp()
{
}

void CDlgUpgradeHttp::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_csPassword);
    DDX_Text(pDX, IDC_EDIT_PORT, m_dwPort);
    DDX_Text(pDX, IDC_EDIT_UPGRADE_FILENAME, m_csFileName);
    DDX_Control(pDX, IDC_UPGRADE_SERVER_IP, m_IPServer);
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_sOutput);
}


BEGIN_MESSAGE_MAP(CDlgUpgradeHttp, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDlgUpgradeHttp::OnBnClickedOk)
    ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BTN_STOP_UPGRADE, &CDlgUpgradeHttp::OnBnClickedBtnStopUpgrade)
END_MESSAGE_MAP()


// CDlgUpgradeHttp 消息处理程序


void CDlgUpgradeHttp::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    DWORD dwIP = 0;
    char sServerIP[16] = { 0 };
    m_IPServer.GetAddress(dwIP);
    memcpy(sServerIP, IPToStr(dwIP), 16);

    CString m_strInputXml;
    m_strInputXml.Format("{\r\n  \"serverIP\": \"%s\",\r\n"\
        "  \"serverPort\": %u,\r\n"\
        "  \"fileUrl\": \"%s\"\r\n}"\
         , sServerIP, m_dwPort, m_csFileName);

    NET_EHOME_PTXML_PARAM struISAPIXML = { 0 };

    char szCmd[129] = { 0 };
    strcpy(szCmd, "POST /ISAPI/System/update/EHome/StartUpdate?format=json");
    char szInBuffer[1500] = { 0 };
    strcpy(szInBuffer, m_strInputXml);
    struISAPIXML.pInBuffer = szInBuffer;
    struISAPIXML.dwInSize = sizeof(szInBuffer);
    struISAPIXML.dwRecvTimeOut = 5 * 1000;

    struISAPIXML.pRequestUrl = szCmd;
    struISAPIXML.dwRequestUrlLen = strlen(szCmd);


    char szOutBuffer[1500] = { 0 };
    struISAPIXML.pOutBuffer = szOutBuffer;
    struISAPIXML.dwOutSize = sizeof(szOutBuffer);

    if (NET_ECMS_ISAPIPassThrough(m_lLoginID, &struISAPIXML))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_ISAPIPassThrough");
        //MessageBox("Start upgrade successful");
        //调整定时器位置，透传成功的情况下才启动定时器获取升级状态
        SetTimer(CYCLE_GETPROGRESS_TIMER, 200, NULL);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_ISAPIPassThrough");
        MessageBox("Start upgrade false");
        //return;
    }

    m_sOutput = szOutBuffer;
   

    UpdateData(FALSE);
}

void CDlgUpgradeHttp::GetProgress()
{
    UpdateData(TRUE);

    NET_EHOME_PTXML_PARAM struISAPIXML = { 0 };

    char szCmd[129] = { 0 };
    strcpy(szCmd, "GET /ISAPI/System/update/EHome/UpgradeStatus?format=json");

    struISAPIXML.dwRecvTimeOut = 5 * 1000;

    struISAPIXML.pRequestUrl = szCmd;
    struISAPIXML.dwRequestUrlLen = strlen(szCmd);

    char szOutBuffer[1500] = { 0 };
    struISAPIXML.pOutBuffer = szOutBuffer;
    struISAPIXML.dwOutSize = sizeof(szOutBuffer);

    if (NET_ECMS_ISAPIPassThrough(m_lLoginID, &struISAPIXML))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_ISAPIPassThrough");
       // MessageBox("Start upgrade successful");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_ISAPIPassThrough");
        //MessageBox("Start upgrade false");
        return;
    }

    int iPercent = 0;

    m_sOutput = szOutBuffer;
    cJSON* pRoot = cJSON_Parse(szOutBuffer);
    if (pRoot == NULL)
    {
        return;
    }

    cJSON* pPercent = cJSON_GetObjectItem(pRoot, "percent");
    if (pPercent == NULL || pPercent->type != cJSON_Number)
    {
        cJSON_Delete(pRoot);
        return ;
    }
    iPercent = pPercent->valueint;

    CString csPercent;
    csPercent.Format("%d%%", iPercent);
    GetDlgItem(IDC_STATIC_PROGRESS)->SetWindowText(csPercent);
    CProgressCtrl *pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROG_UPGRADE);
    pProgCtrl->SetPos(iPercent);

    if (iPercent >= 100)
    {
        KillTimer(CYCLE_GETPROGRESS_TIMER);
    }

    cJSON_Delete(pRoot);
    UpdateData(FALSE);
}


BOOL CDlgUpgradeHttp::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CProgressCtrl *pProgCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROG_UPGRADE);
    pProgCtrl->SetPos(0);
    pProgCtrl->SetRange(0, 100);//设置进度条范围

    char szLan[1024] = { 0 };
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

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}


void CDlgUpgradeHttp::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    switch (nIDEvent)
    {
    case CYCLE_GETPROGRESS_TIMER:
        GetProgress();
        break;
    default:
        break;
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CDlgUpgradeHttp::OnClose()
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    KillTimer(CYCLE_GETPROGRESS_TIMER);
    CDialogEx::OnClose();
}


void CDlgUpgradeHttp::OnBnClickedBtnStopUpgrade()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    NET_EHOME_PTXML_PARAM struISAPIXML = { 0 };

    char szCmd[129] = { 0 };
    strcpy(szCmd, "POST /ISAPI/System/update/EHome/stopUpdate?format=json");

    struISAPIXML.dwRecvTimeOut = 5 * 1000;

    struISAPIXML.pRequestUrl = szCmd;
    struISAPIXML.dwRequestUrlLen = strlen(szCmd);

    char szOutBuffer[1500] = { 0 };
    struISAPIXML.pOutBuffer = szOutBuffer;
    struISAPIXML.dwOutSize = sizeof(szOutBuffer);

    if (NET_ECMS_ISAPIPassThrough(m_lLoginID, &struISAPIXML))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_ISAPIPassThrough");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_ISAPIPassThrough");
        MessageBox("Stop upgrade false");
        return;
    }

    m_sOutput = szOutBuffer;
    UpdateData(FALSE);
    return;
}
