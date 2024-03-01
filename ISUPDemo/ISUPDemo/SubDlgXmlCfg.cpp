// SubDlgXmlCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "SubDlgXmlCfg.h"
#include "DlgInfoPublish.h"
//#include "DlgInfoDiffusionAddition.h"

#include "Public/Convert/Convert.h"
char strTmp[2049] = { 0 };
DWORD lTmp = 0;
BOOL CALLBACK  asynchronousFun(LPNET_EHOME_ASYNC_RESP_CB_DATA lpData, void *pUser)
{
    CSubDlgXmlCfg* dlg = static_cast<CSubDlgXmlCfg*> (pUser);
    if (NULL == dlg)
    {
        return FALSE;
    }


    NET_EHOME_ASYNC_RESP_CB_DATA* struData = (NET_EHOME_ASYNC_RESP_CB_DATA*)lpData;
    memset(strTmp, 0, 2048);
    UTF82A((char*)struData->pOutBuffer, strTmp, sizeof(strTmp), &lTmp);
    int dwErrorNo = struData->dwErrorNo;
    if (struData->pOutBuffer==NULL&&dwErrorNo==0)
    {
        AfxMessageBox(_T("ERROR"));
    }
    if (struData->pOutBuffer!=NULL&&dwErrorNo==0)
    {
        ::PostMessage(dlg->m_hWnd, WM_ASYNC_RESP, NULL, (LPARAM)strTmp);
    }
    
    return TRUE;
}


BOOL CALLBACK LongConfigCallback(LONG lHandle, LONG_LINK_MSG enMsg, void *pOutBuffer, DWORD dwOutLen, void *pUser)
{
    CSubDlgXmlCfg* pDlg = (CSubDlgXmlCfg*)pUser;
    pDlg->ProcessData(lHandle, enMsg, pOutBuffer, dwOutLen);
    return true;
}

void CSubDlgXmlCfg::ProcessData(LONG lHandle, LONG_LINK_MSG enMsg, void *pOutBuffer, DWORD dwOutLen)
{
    UN_REFERENCED_PARAMETER(dwOutLen);
    UN_REFERENCED_PARAMETER(pOutBuffer);
    if (enMsg == LONG_CFG_CREATED)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 0, "LONG_CFG_CREATED,iHande[%d]", lHandle);
    }
    else if (enMsg == LONG_CFG_CREATE_FAIL)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 0, "LONG_CFG_CREATE_FAIL,iHande[%d]", lHandle);
    }
    else if (enMsg == LONG_CFG_DATA)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 0, "LONG_CFG_DATA,iHande=%d,buffsize[%d]", lHandle, dwOutLen);
          g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 0, "data:bufsize[%d]", dwOutLen);
    }
    else if (enMsg == LONG_CFG_TERMINATE)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 0, "LONG_CFG_TERMINATE,iHande[%d]", lHandle);
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 0, "others,iHande[%d]", lHandle);
    }
}

// CSubDlgXmlCfg 对话框

IMPLEMENT_DYNAMIC(CSubDlgXmlCfg, CDialog)

CSubDlgXmlCfg::CSubDlgXmlCfg(CWnd* pParent /*=NULL*/)
    : CDialog(CSubDlgXmlCfg::IDD, pParent)
    , m_strInputXml(_T(""))
    , m_strOutputXml(_T(""))
    , m_strCmd(_T(""))
    , m_csCmd(_T(""))
    , m_csDetail(_T(""))
    , m_csCmdName(_T(""))
    , m_csType(_T(""))
    , m_strIP(_T(""))
    , m_dwSeq(0)
    , m_dwTimeID(0)
    , m_dwCheduleID(0)
    , m_strPlayTime(_T(""))
{
    m_iDeviceIndex = -1;
    m_lLoginID = -1;
    memset(&m_struXmlCfg, 0, sizeof(m_struXmlCfg));
}

CSubDlgXmlCfg::~CSubDlgXmlCfg()
{
}

void CSubDlgXmlCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_XML_INPUT, m_strInputXml);
    DDX_Text(pDX, IDC_EDIT_XML_OUTPUT, m_strOutputXml);
    DDX_Text(pDX, IDC_EDIT_CMD, m_strCmd);
    DDX_CBString(pDX, IDC_COMBO_CMD, m_csCmd);
    DDX_CBString(pDX, IDC_COMBO_CMD_DETAIL, m_csDetail);
    DDX_Text(pDX, IDC_STATIC_CMD, m_csCmdName);
    DDX_Control(pDX, IDC_COMBO_CMD_DETAIL, m_cmbDetail);
    DDX_Control(pDX, IDC_COMBO_CMD, m_cmbCmd);
    DDX_CBString(pDX, IDC_COMBO_TYPE, m_csType);
    DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
#ifndef DEMO_LAN_EN
    DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
    DDX_Text(pDX, IDC_EDIT_SEQ, m_dwSeq);
    DDX_Text(pDX, IDC_EDIT_TIMEID, m_dwTimeID);
    DDX_Text(pDX, IDC_EDIT_SCHEID, m_dwCheduleID);
    DDX_Text(pDX, IDC_EDIT_PLAY_TIME, m_strPlayTime);
#endif
}


BEGIN_MESSAGE_MAP(CSubDlgXmlCfg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_XML_CONTROL, &CSubDlgXmlCfg::OnBnClickedButtonXmlControl)
    ON_BN_CLICKED(IDC_BUTTON_XML_CONFIG, &CSubDlgXmlCfg::OnBnClickedButtonXmlConfig)
    ON_CBN_SELCHANGE(IDC_COMBO_CMD, &CSubDlgXmlCfg::OnSelchangeComboCmd)
    ON_CBN_SELCHANGE(IDC_COMBO_CMD_DETAIL, &CSubDlgXmlCfg::OnSelchangeComboCmdDetail)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CSubDlgXmlCfg::OnSelchangeComboType)
//    ON_CBN_SETFOCUS(IDC_COMBO_CMD_DETAIL, &CSubDlgXmlCfg::OnCbnSetfocusComboCmdDetail)
    ON_MESSAGE(WM_ASYNC_RESP, &CSubDlgXmlCfg::OnWMAsyncResp)
    ON_BN_CLICKED(IDC_BTN_SHOT, &CSubDlgXmlCfg::OnBnClickedBtnShot)
    ON_BN_CLICKED(IDC_BTN_RELEASE, &CSubDlgXmlCfg::OnBnClickedBtnRelease)
    ON_BN_CLICKED(IDC_BTN_MATERIAL, &CSubDlgXmlCfg::OnBnClickedBtnMaterial)
    ON_BN_CLICKED(IDC_BTN_INSERT, &CSubDlgXmlCfg::OnBnClickedBtnInsert)
    ON_BN_CLICKED(IDC_BTN_UPGRATE, &CSubDlgXmlCfg::OnBnClickedBtnUpgrate)
    ON_BN_CLICKED(IDC_BTN_INSERT_STOP, &CSubDlgXmlCfg::OnBnClickedBtnInsertStop)
    //ON_BN_CLICKED(IDC_BTN_GET_TERMINAL_INFO, &CSubDlgXmlCfg::OnBnClickedBtnGetTerminalInfo)
    //ON_BN_CLICKED(IDC_BTN_INFO_DIFFUSION_ADDITION, &CSubDlgXmlCfg::OnBnClickedBtnInfoDiffusionAddition)
    //ON_BN_CLICKED(IDC_BUTTON_BINARY_CONFIG, &CSubDlgXmlCfg::OnBnClickedButtonBinaryConfig)
	ON_BN_CLICKED(IDC_BTN_INFOPUBLSIH, &CSubDlgXmlCfg::OnBnClickedBtnInfopublsih)
END_MESSAGE_MAP()


// CSubDlgXmlCfg 消息处理程序

BOOL CSubDlgXmlCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CRect rc(0, 0, 0, 0);
    GetParent()->GetClientRect(&rc);
    ((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
    MoveWindow(&rc);
    
    m_cmbCmd.InsertString(0, "GETDEVICECONFIG");
    m_cmbCmd.InsertString(1, "SETDEVICECONFIG");
    m_cmbCmd.InsertString(2, "CONTROL");
    m_cmbCmd.SetCurSel(0);
    InitGetParam(BASIC);
    InitCmdType();
    UpdateData(FALSE);


    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

LRESULT CSubDlgXmlCfg::OnWMAsyncResp(WPARAM wParam, LPARAM lParam)
{
    UN_REFERENCED_PARAMETER(wParam)
        char* pTemp = (char*)lParam;

    m_strOutputXml = pTemp;
    UpdateData(FALSE);
    return 0;
}

void CSubDlgXmlCfg::CurCfgUpdate()
{
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    if (m_strCmd.IsEmpty())
    {
        //没有参数,不执行操作,不需要打印错误日志
        //g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    if (m_strInputXml.IsEmpty())
    {
        //没有参数,不执行操作,不需要打印错误日志
        //g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    memset(&m_struXmlCfg, 0, sizeof(m_struXmlCfg));
    char szCmd[32] = {0};
    strcpy(szCmd, m_strCmd);
    char szInBuffer[1500] = {0};
    strcpy(szInBuffer, m_strInputXml);
    char szOutBuffer[1024*10] = {0};
    m_struXmlCfg.pCmdBuf = szCmd;
    m_struXmlCfg.dwCmdLen = sizeof(szCmd);
    m_struXmlCfg.pInBuf = szInBuffer;
    m_struXmlCfg.dwInSize  = sizeof(szInBuffer);
    m_struXmlCfg.pOutBuf = szOutBuffer;
    m_struXmlCfg.dwOutSize = sizeof(szOutBuffer);

    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)m_struXmlCfg.pCmdBuf, (char*)m_struXmlCfg.pCmdBuf, m_struXmlCfg.dwCmdLen, &m_struXmlCfg.dwCmdLen);
    A2UTF8((char*)m_struXmlCfg.pInBuf, (char*)m_struXmlCfg.pInBuf, m_struXmlCfg.dwInSize, &m_struXmlCfg.dwInSize);

    if (NET_ECMS_XMLConfig(m_lLoginID, &m_struXmlCfg, sizeof(m_struXmlCfg)))
    {
        //需要将字符串字段转换成GB2312
        UTF82A((char*)m_struXmlCfg.pOutBuf, m_struXmlCfg.dwOutSize, (char*)m_struXmlCfg.pOutBuf, m_struXmlCfg.dwOutSize, &m_struXmlCfg.dwOutSize);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
        m_strOutputXml = szOutBuffer;
        UpdateData(FALSE);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
    }
}

void CSubDlgXmlCfg::CurCfgSetup()
{
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    memset(&m_struXmlCfg, 0, sizeof(m_struXmlCfg));
    char szCmd[32] = {0};
    strcpy(szCmd, m_strCmd);
    char szInBuffer[1500] = {0};
    strcpy(szInBuffer, m_strInputXml);
    char szOutBuffer[1500] = {0};
    m_struXmlCfg.pCmdBuf = szCmd;
    m_struXmlCfg.dwCmdLen = sizeof(szCmd);
    m_struXmlCfg.pInBuf = szInBuffer;
    m_struXmlCfg.dwInSize  = sizeof(szInBuffer);
    m_struXmlCfg.pOutBuf = szOutBuffer;
    m_struXmlCfg.dwOutSize = sizeof(szOutBuffer);

    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)m_struXmlCfg.pCmdBuf,(char*)m_struXmlCfg.pCmdBuf, 32, &m_struXmlCfg.dwCmdLen);
    A2UTF8((char*)m_struXmlCfg.pInBuf, (char*)m_struXmlCfg.pInBuf, 1500, &m_struXmlCfg.dwInSize);

    if (NET_ECMS_XMLConfig(m_lLoginID, &m_struXmlCfg, sizeof(m_struXmlCfg)))
    {
        //需要将字符串字段转换成GB2312
        UTF82A((char*)m_struXmlCfg.pOutBuf, (char*)m_struXmlCfg.pOutBuf, m_struXmlCfg.dwOutSize, &m_struXmlCfg.dwOutSize);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
        m_strOutputXml = szOutBuffer;
        UpdateData(FALSE);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
    }
}

BOOL CSubDlgXmlCfg::CheckInitParam()
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
    return TRUE;
}

void CSubDlgXmlCfg::OnBnClickedButtonXmlControl()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);

    NET_EHOME_XML_REMOTE_CTRL_PARAM struXMLCfg = {0};
    memset(&struXMLCfg, 0, sizeof(struXMLCfg));
    char szCmd[32] = { 0 };
    DWORD dwCmdLen = 0;
    strcpy(szCmd, m_strCmd);
    char szInBuffer[1500] = { 0 };
    strcpy(szInBuffer, m_strInputXml);
    struXMLCfg.lpInbuffer = szInBuffer;
    struXMLCfg.dwInBufferSize = sizeof(szInBuffer);

    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)szCmd, (char*)szCmd, 32, &dwCmdLen);
    A2UTF8((char*)struXMLCfg.lpInbuffer, (char*)struXMLCfg.lpInbuffer, 1500, &struXMLCfg.dwInBufferSize);

    struXMLCfg.dwSize = sizeof(NET_EHOME_XML_REMOTE_CTRL_PARAM);
    struXMLCfg.dwRecvTimeOut = 5 * 1000;
    if (NET_ECMS_XMLRemoteControl(m_lLoginID, &struXMLCfg, sizeof(struXMLCfg)))
    {

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLRemoteControl");
        UpdateData(FALSE);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLRemoteControl");
    }
}


void CSubDlgXmlCfg::OnBnClickedButtonXmlConfig()
{
    // TODO:  在此添加控件通知处理程序代码
    
//     NET_EHOME_INPUT_PLAN struSwitchPlanSet;
//     NET_EHOME_INPUT_PLAN struSwitchPlanGet;
//     memset(&struSwitchPlanSet, 0, sizeof(struSwitchPlanSet));
//     memset(&struSwitchPlanGet, 0, sizeof(struSwitchPlanGet));
//     NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
//     struSendData.dwSize = sizeof(struSendData);
//     struSendData.lpInBuffer = &struSwitchPlanSet;
//     struSendData.dwInBufferSize = sizeof(struSwitchPlanSet);
// 
//     NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
//     struRecvData.dwSize = sizeof(struRecvData);
//     struRecvData.lpOutBuffer = &struSwitchPlanGet;
//     struRecvData.dwOutBufferSize = sizeof(struSwitchPlanGet);
//     if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_GET_INPUTPLAN, &struSendData, &struRecvData))
//     {
//         DWORD dwErr = NET_ECMS_GetLastError();
//     }

    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
    struSendData.dwSize = sizeof(struSendData);
    char sInput[15*1024] = { 0 };
    strcpy(sInput, m_strInputXml);
    //strcpy(sInput, "{\"cmd\":1001,\"data\":{\"ip\":\"10.100.60.26\",\"port\":123}}");
    struSendData.lpInBuffer = sInput;
    struSendData.dwInBufferSize = strlen(sInput);
    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
    struRecvData.dwSize = sizeof(struRecvData);
    char sOutput[1024] = { 0 };
    struRecvData.lpOutBuffer = sOutput;
    struRecvData.dwOutBufferSize = sizeof(sOutput);
    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)struSendData.lpInBuffer, (char*)struSendData.lpInBuffer, 15 * 1024, &struSendData.dwInBufferSize);
    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_TRANS_WITH_RET, &struSendData, &struRecvData))
    {
        DWORD dwErr = NET_ECMS_GetLastError();
    }

    //return;

    if (!CheckInitParam())
    {
        return;
    }

    //const int dwLen = 8 * 1024 - 68 - 4;
    //char pBuf[8124] = { 0 };
    //memset(pBuf, 65, 8124);

    ////if (NET_ECMS_TranBuf(m_lLoginID, 7 * 1024, pBuf))
    ////{
    ////    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLRemoteControl");
    ////    UpdateData(FALSE);
    ////}

    ////return;


    //NET_EHOME_LONG_CFG_INPUT struCfg = { 0 };
    //struCfg.fnDataCallBack = LongConfigCallback;
    //struCfg.pUser = this;
    //int iHandle = NET_ECMS_LongConfigCreate(m_lLoginID, &struCfg);
    //if (iHandle < 0)
    //{
    //    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_LongConfigCreate");
    //    return;
    //}

    //NET_EHOME_LONG_CFG_SEND struSend = { 0 };
    //struSend.pDataBuffer = pBuf;
    //struSend.dwDataLen = dwLen;

    ////for (int i = 0; i <= 100 * 128; i++)
    ////{
    ////    if (!NET_ECMS_LongConfigSend(iHandle, &struSend))
    ////    {
    ////        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_LongConfigSend");
    ////    }
    ////}

    //memset(pBuf, 70, 8124);
    //struSend.dwDataLen = 8000;
    //if (!NET_ECMS_LongConfigSend(iHandle, &struSend))
    //{
    //    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_LongConfigSend");
    //}

    //Sleep(3000);
    //if (!NET_ECMS_LongConfigDestory(iHandle))
    //{
    //    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_LongConfigDestory");
    //}

    //return;

    //if (!CheckInitParam())
    //{
    //   return;
    //}
    UpdateData(TRUE);
    if (strcmp(m_csCmd, "GETDEVICECONFIG") == 0 || strcmp(m_csCmd, "SETDEVICECONFIG") == 0)
    {
        memset(&m_struXmlCfg, 0, sizeof(m_struXmlCfg));
        char szCmd[64] = { 0 };
        strcpy(szCmd, m_csCmd);
        char szInBuffer[1500] = { 0 };
        strcpy(szInBuffer, m_strInputXml);
        char szOutBuffer[1024*10] = { 0 };
        m_struXmlCfg.pCmdBuf = szCmd;
        m_struXmlCfg.dwCmdLen = strlen(szCmd);
        m_struXmlCfg.pInBuf = szInBuffer;
        m_struXmlCfg.dwInSize = sizeof(szInBuffer);
        m_struXmlCfg.pOutBuf = szOutBuffer;
        m_struXmlCfg.dwOutSize = sizeof(szOutBuffer);
        char szStatusBuf[1024*10] = { 0 };
        m_struXmlCfg.pStatusBuf = szStatusBuf;
        m_struXmlCfg.dwStatusSize = sizeof(szStatusBuf);

        //需要将字符串字段转换成UTF-8
        A2UTF8((char*)m_struXmlCfg.pCmdBuf, (char*)m_struXmlCfg.pCmdBuf, 64, &m_struXmlCfg.dwCmdLen);
        A2UTF8((char*)m_struXmlCfg.pInBuf, (char*)m_struXmlCfg.pInBuf, 1500, &m_struXmlCfg.dwInSize);
        int nIndex =1;
       if (nIndex==1)
       {
           if (NET_ECMS_XMLConfig(m_lLoginID, &m_struXmlCfg, sizeof(m_struXmlCfg)))
           {
               //需要将字符串字段转换成GB2312
               UTF82A((char*)m_struXmlCfg.pOutBuf, (char*)m_struXmlCfg.pOutBuf, m_struXmlCfg.dwOutSize, &m_struXmlCfg.dwOutSize);

               g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
               m_strOutputXml = szOutBuffer;
               UpdateData(FALSE);
           }
           else
           {
               m_strOutputXml = szOutBuffer;
               g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
           }
       }
       else if (nIndex==2)
       {
           NET_ECMS_SetXmlConfigResponseCB(asynchronousFun, this);
           DWORD h = (DWORD)-1;
           if (NET_ECMS_XMLConfigEx(m_lLoginID, &m_struXmlCfg, &h))
           {
               //需要将字符串字段转换成GB2312
               /*UTF82A((char*)m_struXmlCfg.pOutBuf, (char*)m_struXmlCfg.pOutBuf, m_struXmlCfg.dwOutSize, &m_struXmlCfg.dwOutSize);

               g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
               m_strOutputXml = szOutBuffer;*/

               UpdateData(FALSE);
           }
           else
           {
               g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfigEx");
           }

       }
    }
    else if (strcmp(m_csCmd, "CONTROL") == 0)
    {
        NET_EHOME_XML_REMOTE_CTRL_PARAM struXMLCfg = { 0 };
        char szInBuffer[1500] = { 0 };
        strcpy(szInBuffer, m_strInputXml);
        struXMLCfg.lpInbuffer = szInBuffer;
        struXMLCfg.dwInBufferSize = sizeof(szInBuffer);
        //DWORD dwSeq = 0;
        char szOutBuffer[102400] = { 0 };
        struXMLCfg.lpOutBuffer = szOutBuffer;
        struXMLCfg.dwOutBufferSize = 102400;
        char szStatus[102400] = { 0 };
        struXMLCfg.lpStatusBuffer = szStatus;
        struXMLCfg.dwStatusBufferSize = 102400;

        //需要将字符串字段转换成UTF-8
        A2UTF8((char*)struXMLCfg.lpInbuffer, (char*)struXMLCfg.lpInbuffer, 1500, &struXMLCfg.dwInBufferSize);

        struXMLCfg.dwSize = sizeof(NET_EHOME_XML_REMOTE_CTRL_PARAM);
        struXMLCfg.dwRecvTimeOut = 30 * 1000;
        if (NET_ECMS_XMLRemoteControl(m_lLoginID, &struXMLCfg, sizeof(struXMLCfg)))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLRemoteControl");
            if (m_csDetail == "MANUALSNAP")
            {
                int iSequence = *(int*)(szOutBuffer);
                m_strOutputXml.Format("%d", iSequence);
            }
            else
            {
                m_strOutputXml = szOutBuffer;
            }
            UpdateData(FALSE);
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLRemoteControl");
        }

    }
}

void CSubDlgXmlCfg::InitGetParam(CMD_TYPE enumType)
{
    m_cmbDetail.ResetContent();

    if (enumType == BASIC)
    {
        m_cmbDetail.AddString("GetServerInfoPara"); //设备基本参数
        m_cmbDetail.AddString("GetServerInfo"); //获取设备信息
        m_cmbDetail.AddString("GetVersionPara"); //获取版本信息


        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "获取设备基本参数", "Get Server Info Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetServerInfoPara</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (enumType == VIDEO_OSD)
    {
        m_cmbDetail.AddString("GetPicPara"); //OSD显示参数
        m_cmbDetail.AddString("GetShowStringPara"); //OSD字符叠加参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "OSD显示参数", "Get OSD Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetPicPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (enumType == MOVE_DETECTION)
    {
        m_cmbDetail.AddString("GetMotionPara"); //移动侦测使能
        m_cmbDetail.AddString("GetMotionArea"); //移动侦测布防
        m_cmbDetail.AddString("GetAlarmDeploymentTime");//布防时间
        m_cmbDetail.AddString("GetAlarmTriggerRecord"); //通道录像参数
        m_cmbDetail.AddString("GetAlarmLinkageType"); //联动参数
        m_cmbDetail.AddString("GetAlarmTriggerCapture");//抓拍参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "移动侦测使能", "Get motion param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetMotionPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (enumType == HIDE_ALARM)
    {
        m_cmbDetail.AddString("GetHideAlarmPara"); //遮挡使能
        m_cmbDetail.AddString("GetHideAlarmArea"); //遮挡区域
        m_cmbDetail.AddString("GetAlarmDeploymentTime");//布防时间
        m_cmbDetail.AddString("GetAlarmLinkageType"); //联动参数
        m_cmbDetail.AddString("GetAlarmTriggerCapture");//抓拍参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "遮挡使能", "Get hide alarm param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetHideAlarmPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (enumType == VIDEO_LOST)
    {
        m_cmbDetail.AddString("GetVILostPara"); //视频丢失使能
        m_cmbDetail.AddString("GetAlarmDeploymentTime");//布防时间
        m_cmbDetail.AddString("GetAlarmLinkageType"); //联动参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "视频丢失使能", "Get video lost param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVILostPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");

    }
    else if (enumType == HIDE_PRIVATE)
    {
        m_cmbDetail.AddString("GetHidePara"); //隐私使能
        m_cmbDetail.AddString("GetHideArea");//隐私区域
        m_cmbDetail.AddString("GetAlarmLinkageType"); //联动参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "视频丢失使能", "Get video lost param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVILostPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (enumType == VIDEO_STATUS)
    {
        m_cmbDetail.AddString("GetChanVideoStatus"); //通道视频状态

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "通道视频状态", "Get chan video status");
        m_csCmdName.Format("%s", szLan);

    }
    else if (enumType == INFRARED_WIFI_CALL)
    {
        m_cmbDetail.AddString("GetAlarmLinkageType"); //联动参数
        m_cmbDetail.AddString("GetAlarmTriggerRecord"); //通道录像参数
        m_cmbDetail.AddString("GetAlarmTriggerCapture");//抓拍参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "联动参数", "Get alarm linkage type");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetAlarmLinkageType</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (enumType == Alarm)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetAlarmLinkageType");
        m_cmbDetail.AddString("GetAlarmTriggerRecord");
        m_cmbDetail.AddString("GetAlarmTriggerCapture");
        m_cmbDetail.AddString("GetAlarmInPara");
        m_cmbDetail.AddString("GetAlarmOutPara");
        m_cmbDetail.AddString("GetAlarmDeploymentTime");
        m_cmbDetail.AddString("GetAlarmOutStatus");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == CAPTUREPIC)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetCapturePicPlanPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == RS)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetRS485Para");
        m_cmbDetail.AddString("GetRS232Para");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == RECORD)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetRecordPlanPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == CMSINFO)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetCMSInfoPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == GPS)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetGPSInfoPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == OPERATOR_CODE)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetOperateCode");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == ZERO_CHAN)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetZeroChannelInfo");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == DISK_FORMAT)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetFormatDiskProgress");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == PREVIEW)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetPreviewPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == ABILITY)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetDevAbility");
        m_cmbDetail.AddString("GetDevVCAAbility");
        m_cmbDetail.AddString("GetDevChannelVCAAbility");
        m_cmbDetail.AddString("GetVCAFaceSnapCapa");
        m_cmbDetail.AddString("GetVCAFaceSnapRuleCapa");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == PASSENGER_PARAMETER)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetPassengerPara");
        m_cmbDetail.AddString("GetPassengerOSDPara");
        m_cmbDetail.AddString("GetPassengerRegionPara");
        m_cmbDetail.AddString("GetPassengerLinePara");
        m_cmbDetail.AddString("GetPassengerSetupPara");
        m_cmbDetail.AddString("GetCalibrationPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == VCA)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetVCACapCtrlCfg");
        m_cmbDetail.AddString("GetVCACtrlCfg");
        m_cmbDetail.AddString("GetVCAVersion");
        m_cmbDetail.AddString("GetVCAFaceSnapCfg");
        m_cmbDetail.AddString("GetVCAFaceSnapRuleCfg");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == RECORD_STATUS)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetRecordStatus");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == NET_STATUS)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetNetStatus");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == DIAL_MODE)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetDialMode");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == ACS)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("GetCardWeekPlan");
        m_cmbDetail.AddString("GetCardWeekPlanCapabilities");
        m_cmbDetail.AddString("GetCardWeekPlanTime");
        m_cmbDetail.AddString("GetCardWeekPlanTimeCapabilities");
        m_cmbDetail.AddString("GetCardHolidayPlan");
        m_cmbDetail.AddString("GetCardHolidayPlanCapabilities");
        m_cmbDetail.AddString("GetCardHolidayGroup");
        m_cmbDetail.AddString("GetCardHolidayGroupCapabilities");
        m_cmbDetail.AddString("GetCardTemplate");
        m_cmbDetail.AddString("GetCardTemplateCapabilities");
        m_cmbDetail.AddString("GetDoorStatusWeekPlan");
        m_cmbDetail.AddString("GetDoorStatusWeekPlanCapabilities");
        m_cmbDetail.AddString("GetDoorStatusWeekPlanTime");
        m_cmbDetail.AddString("GetDoorStatusWeekPlanTimeCapabilities");
        m_cmbDetail.AddString("GetDoorStatusHolidayPlan");
        m_cmbDetail.AddString("GetDoorStatusHolidayPlanCapabilities");
        m_cmbDetail.AddString("GetDoorStatusHolidayGroup");
        m_cmbDetail.AddString("GetDoorStatusHolidayGroupCapabilities");
        m_cmbDetail.AddString("GetDoorStatusTemplate");
        m_cmbDetail.AddString("GetDoorStatusTemplateCapabilities");
        m_cmbDetail.AddString("GetReaderVertifyWeekPlan");
        m_cmbDetail.AddString("GetReaderVertifyWeekPlanCapabilities");
        m_cmbDetail.AddString("GetReaderVertifyWeekPlanTime");
        m_cmbDetail.AddString("GetReaderVertifyWeekPlanTimeCapabilities");
        m_cmbDetail.AddString("GetReaderVertifyHolidayPlan");
        m_cmbDetail.AddString("GetReaderVertifyHolidayPlanCapabilities");
        m_cmbDetail.AddString("GetReaderVertifyHolidayGroup");
        m_cmbDetail.AddString("GetReaderVertifyHolidayGroupCapabilities");
        m_cmbDetail.AddString("GetReaderVertifyTemplate");
        m_cmbDetail.AddString("GetReaderVertifyTemplateCapabilities");
        m_cmbDetail.AddString("GetCardCfg");
        m_cmbDetail.AddString("GetCardCfgCapabilities");
        m_cmbDetail.AddString("GetCardRightCfg");
        m_cmbDetail.AddString("GetCardRightCfgCapabilities");
        m_cmbDetail.AddString("GetCardPasswdOpenDoorCfg");
        m_cmbDetail.AddString("GetCardPasswdOpenDoorCfgCapabilities");
        m_cmbDetail.AddString("GetHostWorkStatus");
        m_cmbDetail.AddString("GetHostWorkStatusCapabilities");
        m_cmbDetail.AddString("GetCardReaderWorkStatus");
        m_cmbDetail.AddString("GetCardReaderWorkStatusCapabilities");
        m_cmbDetail.AddString("GetAlarmInStatus");
        m_cmbDetail.AddString("GetAlarmInStatusCapabilities");
        m_cmbDetail.AddString("GetAlarmOutStatus");
        m_cmbDetail.AddString("GetAlarmOutStatusCapabilities");
        m_cmbDetail.AddString("GetDoorCfg");
        m_cmbDetail.AddString("GetDoorCfgCapabilities");
        m_cmbDetail.AddString("GetDoorWorkStatus");
        m_cmbDetail.AddString("GetDoorWorkStatusCapabilities");
        m_cmbDetail.AddString("GetDoorWorkStatusPlan");
        m_cmbDetail.AddString("GetDoorWorkStatusPlanCapabilities");
        m_cmbDetail.AddString("GetCardReaderCfg");
        m_cmbDetail.AddString("GetCardReaderCfgCapabilities");
        m_cmbDetail.AddString("GetCardReaderVCACfg");
        m_cmbDetail.AddString("GetCardReaderVCACfgCapabilities");
        m_cmbDetail.AddString("GetReaderVertifyPlan");
        m_cmbDetail.AddString("GetReaderVertifyPlanCapabilities");
        m_cmbDetail.AddString("GetGroupCfg");
        m_cmbDetail.AddString("GetGroupCfgCapabilities");
        m_cmbDetail.AddString("GetMultiCardCfg");
        m_cmbDetail.AddString("GetMultiCardCfgCapabilities");
        m_cmbDetail.AddString("GetMultiCardGroupCfg");
        m_cmbDetail.AddString("GetMultiCardGroupCfgCapabilities");
        m_cmbDetail.AddString("GetAntiSneakCfg");
        m_cmbDetail.AddString("GetAntiSneakCfgCapabilities");
        m_cmbDetail.AddString("GetAntiSneakReaderCfg");
        m_cmbDetail.AddString("GetAntiSneakReaderCfgCapabilities");
        m_cmbDetail.AddString("GetMultiDoorInterlockCfg");
        m_cmbDetail.AddString("GetMultiDoorInterlockCfgCapabilities");
        m_cmbDetail.AddString("GetMultiDoorInterlockGroupCfg");
        m_cmbDetail.AddString("GetMultiDoorInterlockGroupCfgCapabilities");
        m_cmbDetail.AddString("GetAntiSneakMultiHostCfg");
        m_cmbDetail.AddString("GetAntiSneakMultiHostCfgCapabilities");
        m_cmbDetail.AddString("GetAntiSneakMultiHostReaderCfg");
        m_cmbDetail.AddString("GetAntiSneakMultiHostCardReaderCfgCapabilities");
        m_cmbDetail.AddString("GetCaseStatus");
        m_cmbDetail.AddString("GetCaseStatusCapabilities");
        m_cmbDetail.AddString("GetEventLinkCfg");
        m_cmbDetail.AddString("GetEventLinkCfgCapabilities");
        m_cmbDetail.AddString("GetEventLinkOpenDoorCfg");
        m_cmbDetail.AddString("GetEventLinkOpenDoorCfgCapabilities");
        m_cmbDetail.AddString("GetCaseSensorLinkAlarmOut");
        m_cmbDetail.AddString("GetCaseSensorLinkAlarmOutCapabilities");
        m_cmbDetail.AddString("GetEventCardLinkBuzzer");
        m_cmbDetail.AddString("GetEventCardLinkBuzzerCapabilities");
        m_cmbDetail.AddString("GetEventCardLinkDoor");
        m_cmbDetail.AddString("GetEventCardLinkDoorCapabilities");
        m_cmbDetail.AddString("GetEventCardLinkAlarmout");
        m_cmbDetail.AddString("GetEventCardLinkAlarmoutCapabilities");
        m_cmbDetail.AddString("GetEventCardLinkAlarmIn");
        m_cmbDetail.AddString("GetEventCardLinkAlarmInCapabilities");
        m_cmbDetail.AddString("GetFingerPrint");
        m_cmbDetail.AddString("GetFingerPrintCapabilities");
        m_cmbDetail.AddString("GetDelFingerPrintByCardNoCapabilities");
        m_cmbDetail.AddString("GetDelFingerPrintByReaderIDCapabilities");
        m_cmbDetail.AddString("GetPhoneCfg");
        m_cmbDetail.AddString("GetPhoneCfgCapabilities");
        m_cmbDetail.AddString("GetSMSRightCfg");
        m_cmbDetail.AddString("GetSMSRightCfgCapabilities");
        m_cmbDetail.AddString("GetWirelessNetWorkCfg");
        m_cmbDetail.AddString("GetWirelessNetWorkCfgCapabilities");
        m_cmbDetail.AddString("GetWirelessNetCenterCfg");
        m_cmbDetail.AddString("GetWirelessNetCenterCfgCapabilities");
        m_cmbDetail.AddString("GetReportCenterCfg");
        m_cmbDetail.AddString("GetReportCenterCfgCapabilities");
        m_cmbDetail.AddString("GetServerInfoPara");
        m_cmbDetail.AddString("GetServerInfoParaCapabilities");
        m_cmbDetail.AddString("GetNetPara");
        m_cmbDetail.AddString("GetNetParaCapabilities");
        m_cmbDetail.AddString("GetAlarmInParam");
        m_cmbDetail.AddString("GetAlarmInParamCapabilities");
        m_cmbDetail.AddString("GetDSTCfg");
        m_cmbDetail.AddString("GetDSTCfgCapabilities");
        m_cmbDetail.AddString("GetSecurityCfg");
        m_cmbDetail.AddString("GetSecurityCfgCapabilities");
        m_cmbDetail.AddString("GetCMSInfoPara");
        m_cmbDetail.AddString("GetCMSInfoParaCapabilities");
        m_cmbDetail.AddString("GetAlarmHostNetCenterCfg");
        m_cmbDetail.AddString("GetAlarmHostNetCenterCfgCapabilities");
        m_cmbDetail.AddString("GetACSCfg");
        m_cmbDetail.AddString("GetACSCfgCapabilities");
        m_cmbDetail.AddString("GetCardUserInfo");
        m_cmbDetail.AddString("GetCardUserInfoCapabilities");
        m_cmbDetail.AddString("GetEventCardLinkCapturePic");
        m_cmbDetail.AddString("GetEventCardLinkCapturePicCapabilities");
        m_cmbDetail.AddString("GetFaceDetectRuleCfg");
        m_cmbDetail.AddString("GetFaceDetectRuleCfgCapabilities");
        m_cmbDetail.AddString("GetContinousSnapCfgCapabilities");
        m_cmbDetail.AddString("GetUploadPicCfgCapabilities");
        m_cmbDetail.AddString("GetDelPictureCfgCapabilities");
        m_cmbDetail.AddString("GetACSFunctionCapabilities");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
     else if (enumType == WIRELESS_INFO)
     {
         m_cmbDetail.ResetContent();
         m_cmbDetail.AddString("GetWirelessInfoPara");
         m_cmbDetail.SetCurSel(0);
         OnSelchangeComboCmdDetail();
     }
}

void CSubDlgXmlCfg::InitSetParam(CMD_TYPE enumType)
{
    m_cmbDetail.ResetContent();
    if (enumType == BASIC)
    {
        m_cmbDetail.AddString("SetServerInfoPara"); //设备基本参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "设置设备基本参数", "Set Server Info Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetServerInfoPara</ConfigCmd>\r\n"\
            "<ConfigXML>\r\n"\
            "<SERVERINFOPARA>\r\n"\
            "<ServerName>asdfghjkasdfghjkasdfghjkasdfghj</ServerName>\r\n"\
            "<ServerID>250</ServerID >\r\n"\
            "<RecycleRecord>1</RecycleRecord >\r\n"\
            "<ServerType>2233</ServerType >\r\n"\
            "<ChannelNum>32</ChannelNum >\r\n"\
            "<HardDiskNum>1</HardDiskNum >\r\n"\
            "<AlarmInNum>17</AlarmInNum >\r\n"\
            "<AlarmOutNum>5</AlarmOutNum >\r\n"\
            "<RS232Num>1</RS232Num >\r\n"\
            "<RS485Num>1</RS485Num >\r\n"\
            "<NetworkPortNum>2</NetworkPortNum >\r\n"\
            "<AuxoutNum>3</AuxoutNum >\r\n"\
            "<AudioNum>2</AudioNum >\r\n"\
            "<SerialNumber>1620160326CCRR586037487WCVU </SerialNumber >\r\n"\
            "<MajorScale>0</MajorScale>\r\n"\
            "<MinorScale>0</MinorScale>\r\n"\
            "</SERVERINFOPARA>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (enumType == VIDEO_OSD)
    {
        m_cmbDetail.AddString("SetPicPara"); //OSD显示参数
        m_cmbDetail.AddString("SetShowStringPara"); //OSD字符叠加

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "OSD字符叠加", "Set OSD show string");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetPicPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<CHANPICINFO>\r\n"
            "<ChannelName>Camera 01</ChannelName>\r\n"
            "<IsShowChanName>1</IsShowChanName>\r\n"
            "<ChannelNameXPos>512</ChannelNameXPos>\r\n"
            "<ChannelNameYPos>512</ChannelNameYPos>\r\n"
            "<IsShowOSD>1</IsShowOSD>\r\n"
            "<OSDXPos>0</OSDXPos>\r\n"
            "<OSDYPos>32</OSDYPos>\r\n"
            "<OSDType>2</OSDType>\r\n"
            "<OSDAttrib>4</OSDAttrib>\r\n"
            "<IsShowWeek>1</IsShowWeek>\r\n"
            "</CHANPICINFO>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else if (enumType == MOVE_DETECTION)
    {
        m_cmbDetail.AddString("SetMotionPara"); //移动侦测使能
        m_cmbDetail.AddString("SetMotionArea"); //移动侦测布防
        m_cmbDetail.AddString("SetAlarmDeploymentTime");//布防时间
        m_cmbDetail.AddString("SetAlarmTriggerRecord"); //通道录像参数
        m_cmbDetail.AddString("SetAlarmLinkageType"); //联动参数
        m_cmbDetail.AddString("SetAlarmTriggerCapture");//抓拍参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "移动侦测使能", "Set motion param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetMotionPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<MOTIONCFG>\r\n"
            "<IsUseMotion>0</IsUseMotion>\r\n"
            "<Sensitive>0</Sensitive>\r\n"
            "</MOTIONCFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else if (enumType == HIDE_ALARM)
    {
        m_cmbDetail.AddString("SetHideAlarmPara"); //遮挡使能
        m_cmbDetail.AddString("SetHideAlarmArea"); //遮挡区域
        m_cmbDetail.AddString("SetAlarmDeploymentTime");//布防时间
        m_cmbDetail.AddString("SetAlarmLinkageType"); //联动参数
        m_cmbDetail.AddString("SetAlarmTriggerCapture");//抓拍参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "遮挡使能", "Set hide alarm param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetHideAlarmPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<HIDEALARMCFG>\r\n"
            "<Sensitive>1</Sensitive>\r\n"
            "</HIDEALARMCFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else if (enumType == VIDEO_LOST)
    {
        m_cmbDetail.AddString("SetVILostPara"); //视频丢失使能
        m_cmbDetail.AddString("SetAlarmDeploymentTime");//布防时间
        m_cmbDetail.AddString("SetAlarmLinkageType"); //联动参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "视频丢失使能", "Set video lost param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetVILostPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<VILOSTCFG>\r\n"
            "<IsUseVILost>1</IsUseVILost>\r\n"
            "</VILOSTCFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else if (enumType == HIDE_PRIVATE)
    {
        m_cmbDetail.AddString("SetHidePara"); //隐私使能
        m_cmbDetail.AddString("SetHideArea");//隐私区域
        m_cmbDetail.AddString("SetAlarmLinkageType"); //联动参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "隐私使能", "Set hide param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetHideAlarmPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<HIDECFG>\r\n"
            "<IsUseHide>1</IsUseHide>\r\n"
            "<HIDECFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else if (enumType == INFRARED_WIFI_CALL)
    {
        m_cmbDetail.AddString("SetAlarmLinkageType"); //联动参数
        m_cmbDetail.AddString("SetAlarmTriggerRecord"); //通道录像参数
        m_cmbDetail.AddString("SetAlarmTriggerCapture");//抓拍参数

        m_cmbDetail.SetCurSel(0);
        char szLan[64] = { 0 };
        g_StringLanType(szLan, "联动参数", "Get alarm linkage type");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetAlarmLinkageType</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigXML>\r\n"
            "<LINKAGETYPE>\r\n"
            "<MonitorAlarm>0</MonitorAlarm>\r\n"
            "<SoundAlarm>0</SoundAlarm>\r\n"
            "<Upload>0</Upload>\r\n"
            "<Alarmout>0</Alarmout>\r\n"
            "<Email>0</Email>\r\n"
            "<ALARMOUT>\r\n"
            "<AnalogAlarmOut1>0</AnalogAlarmOut1>\r\n"
            "<AnalogAlarmOut2>0</AnalogAlarmOut2>\r\n"
            "</ALARMOUT>\r\n"
            "</LINKAGETYPE>\r\n"
            "</ConfigXML>\r\n"
            "</Params>\r\n");
    }
    else if (enumType == Alarm)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetAlarmLinkageType");
        m_cmbDetail.AddString("SetAlarmTriggerRecord");
        m_cmbDetail.AddString("SetAlarmTriggerCapture");
        m_cmbDetail.AddString("SetAlarmInPara");
        m_cmbDetail.AddString("SetAlarmOutPara");
        m_cmbDetail.AddString("SetAlarmDeploymentTime");
        m_cmbDetail.AddString("SetAlarmOutStatus");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == CAPTUREPIC)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetCapturePicPlanPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == RS)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetRS485Para");
        m_cmbDetail.AddString("SetRS232Para");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == RECORD)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetRecordPlanPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == CMSINFO)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetCMSInfoPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == GPS)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetGPSInfoPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == ZERO_CHAN)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetZeroChannelInfo");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == DISK_FORMAT)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetFormatDisk");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == PREVIEW)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetPreviewPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == PASSENGER_PARAMETER)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetPassengerPara");
        m_cmbDetail.AddString("SetPassengerOSDPara");
        m_cmbDetail.AddString("SetPassengerRegionPara");
        m_cmbDetail.AddString("SetPassengerLinePara");
        m_cmbDetail.AddString("SetPassengerSetupPara");
        m_cmbDetail.AddString("SetCalibrationPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == VCA)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetVCACtrlCfg");
        m_cmbDetail.AddString("SetVCAFaceSnapCfg");
        m_cmbDetail.AddString("SetVCAFaceSnapRuleCfg");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == DIAL_MODE)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetDialMode");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == ACS)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetCardWeekPlan");
        m_cmbDetail.AddString("SetCardWeekPlanTime");
        m_cmbDetail.AddString("SetCardHolidayPlan");
        m_cmbDetail.AddString("SetCardHolidayGroup");
        m_cmbDetail.AddString("SetCardTemplate");
        m_cmbDetail.AddString("SetDoorStatusWeekPlan");
        m_cmbDetail.AddString("SetDoorStatusWeekPlanTime");
        m_cmbDetail.AddString("SetDoorStatusHolidayPlan");
        m_cmbDetail.AddString("SetDoorStatusHolidayGroup");
        m_cmbDetail.AddString("SetDoorStatusTemplate");
        m_cmbDetail.AddString("SetReaderVertifyWeekPlan");
        m_cmbDetail.AddString("SetReaderVertifyWeekPlanTime");
        m_cmbDetail.AddString("SetReaderVertifyHolidayPlan");
        m_cmbDetail.AddString("SetReaderVertifyHolidayGroup");
        m_cmbDetail.AddString("SetReaderVertifyTemplate");
        m_cmbDetail.AddString("SetCardCfg");
        m_cmbDetail.AddString("SetCardRightCfg");
        m_cmbDetail.AddString("SetCardPasswdOpenDoorCfg");
        m_cmbDetail.AddString("SetAlarmInStatus");
        m_cmbDetail.AddString("SetAlarmOutStatus");
        m_cmbDetail.AddString("SetDoorCfg");
        m_cmbDetail.AddString("SetDoorWorkStatusPlan");
        m_cmbDetail.AddString("SetCardReaderCfg");
        m_cmbDetail.AddString("SetCardReaderVCACfg");
        m_cmbDetail.AddString("SetReaderVertifyPlan");
        m_cmbDetail.AddString("SetGroupCfg");
        m_cmbDetail.AddString("SetMultiCardCfg");
        m_cmbDetail.AddString("SetMultiCardGroupCfg");
        m_cmbDetail.AddString("SetAntiSneakCfg");
        m_cmbDetail.AddString("SetAntiSneakReaderCfg");
        m_cmbDetail.AddString("SetMultiDoorInterlockCfg");
        m_cmbDetail.AddString("SetMultiDoorInterlockGroupCfg");
        m_cmbDetail.AddString("SetAntiSneakMultiHostCfg");
        m_cmbDetail.AddString("SetAntiSneakMultiHostReaderCfg");
        m_cmbDetail.AddString("SetCaseStatus");
        m_cmbDetail.AddString("SetEventLinkCfg");
        m_cmbDetail.AddString("SetEventLinkOpenDoorCfg");
        m_cmbDetail.AddString("SetCaseSensorLinkAlarmOut");
        m_cmbDetail.AddString("SetEventCardLinkBuzzer");
        m_cmbDetail.AddString("SetEventCardLinkDoor");
        m_cmbDetail.AddString("SetEventCardLinkAlarmout");
        m_cmbDetail.AddString("SetEventCardLinkAlarmIn");
        m_cmbDetail.AddString("SetFingerPrintCfg");
        m_cmbDetail.AddString("DelFingerPrintByCardNo");
        m_cmbDetail.AddString("DelFingerPrintByReaderID");
        m_cmbDetail.AddString("SetPhoneCfg");
        m_cmbDetail.AddString("SetSMSRightCfg");
        m_cmbDetail.AddString("SetWirelessNetWorkCfg");
        m_cmbDetail.AddString("SetWirelessNetCenterCfg");
        m_cmbDetail.AddString("SetReportCenterCfg");
        m_cmbDetail.AddString("SetServerInfoPara");
        m_cmbDetail.AddString("SetNetPara");
        m_cmbDetail.AddString("SetAlarmInParam");
        m_cmbDetail.AddString("SetDSTCfg");
        m_cmbDetail.AddString("SetSecurityCfg");
        m_cmbDetail.AddString("SetCMSInfoPara");
        m_cmbDetail.AddString("SetAlarmHostNetCenterCfg");
        m_cmbDetail.AddString("SetACSCfg");
        m_cmbDetail.AddString("SetCardUserInfo");
        m_cmbDetail.AddString("SetEventCardLinkCapturePic");
        m_cmbDetail.AddString("SetFaceDetectRuleCfg");
        m_cmbDetail.AddString("SetContinousSnapCfg");
        m_cmbDetail.AddString("SetUploadPicCfg");
        m_cmbDetail.AddString("DelPictureCfg");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
    else if (enumType == WIRELESS_INFO)
    {
        m_cmbDetail.ResetContent();
        m_cmbDetail.AddString("SetWirelessInfoPara");
        m_cmbDetail.SetCurSel(0);
        OnSelchangeComboCmdDetail();
    }
}

void CSubDlgXmlCfg::InitControlParam()
{
    m_cmbDetail.ResetContent();
    m_cmbDetail.AddString("PTZCONTROL"); //云镜控制 
    m_cmbDetail.AddString("MANUALSNAP"); //手动抓图
    m_cmbDetail.AddString("GETDEVICEWORKSTATUS");//获取设备工作状态
    m_cmbDetail.AddString("PTZPRESETCONTROL");   //预置点设置
    m_cmbDetail.AddString("PZIN");               //3D框放大缩写  
    m_cmbDetail.AddString("PTRACK");             //3D框放大居中 
    m_cmbDetail.AddString("GETIMAGEPARAM");      //获取图像质量参数
    m_cmbDetail.AddString("SETIMAGEPARAM");      //设置图像质量参数
    m_cmbDetail.AddString("SETDEFENSESTATUS");   //远程布撤防
    m_cmbDetail.AddString("ALARMHOSTCTRL");      //远程控制报警主机
    m_cmbDetail.AddString("ADJUSTTIME");         //手动校时
    m_cmbDetail.AddString("RECSTART");           //手动开启录像
    m_cmbDetail.AddString("RECSTOP");            //手动关闭录像
    m_cmbDetail.AddString("MANUALAIO");          //触发设备IO输出
    m_cmbDetail.AddString("UPLOADPIC");          //下载图片
    m_cmbDetail.AddString("ISSUEINFORMATION");   //下发显示信息
    m_cmbDetail.AddString("RESETPDCOSD");        //客流量清零
    m_cmbDetail.AddString("RESTARTIVSLIB");      //重启算法库
    m_cmbDetail.AddString("SETCLOUDSERVERINFO");    //云存储服务器参数
    m_cmbDetail.AddString("SETBAYONETPOLICEINFO");  //卡扣服务器参数
    m_cmbDetail.AddString("SETENFORCESERVERINFO");  //取证服务器参数
    m_cmbDetail.AddString("GATECONTROL"); //远程控门
    m_cmbDetail.AddString("CLEARACSPARAM"); //清空门禁相关参数

    m_cmbDetail.SetCurSel(0);
    char szLan[64] = { 0 };
    g_StringLanType(szLan, "云镜控制", "Ptz control");
    m_csCmdName.Format("%s", szLan);
    m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"\
        "<PPVSPMessage>\r\n"\
        "<Version>4.0</Version>\r\n"\
        "<Sequence>8757</Sequence >\r\n"\
        "<CommandType>REQUEST</CommandType>\r\n"\
        "<Method>CONTROL</Method>\r\n"\
        "<Command>PTZCONTROL</Command>\r\n"\
        "<Params>\r\n"\
        "<Channel>1</Channel >\r\n"\
        "<PTZCmd>UP</PTZCmd>\r\n"\
        "<Action>Stop</Action>\r\n"\
        "<Speed>0</Speed >\r\n"\
        "</Params>\r\n"\
        "</PPVSPMessage>\r\n");
}


void CSubDlgXmlCfg::OnSelchangeComboCmd()
{
    // TODO:  在此添加控件通知处理程序代码

    int nCurSel = m_cmbCmd.GetCurSel();
    if (nCurSel == CB_ERR)
    {
        return;
    }
    UpdateData(TRUE);
    m_strInputXml = "";
    m_strOutputXml = "";
    m_csCmdName = "";

    if (strcmp(m_csCmd, "GETDEVICECONFIG") == 0) //获取参数
    {
        InitGetParam(m_enumType);
    }
    else if (strcmp(m_csCmd, "SETDEVICECONFIG") == 0) //设置参数
    {
        InitSetParam(m_enumType);
    }
    else if (strcmp(m_csCmd, "CONTROL") == 0) //控制
    {
        InitControlParam();
    }
    UpdateData(FALSE);
}


void CSubDlgXmlCfg::OnSelchangeComboCmdDetail()
{
    // TODO:  在此添加控件通知处理程序代码
    int nCurSel = m_cmbDetail.GetCurSel();
    if (nCurSel == CB_ERR)
    {
        return;
    }
    UpdateData(TRUE);
    m_strInputXml = "";
    m_strOutputXml = "";
    m_csCmdName = "";
    char szLan[64] = { 0 };
    //获取
    if (strcmp(m_csDetail, "GetServerInfoPara") == 0)
    {
        g_StringLanType(szLan, "获取设备基本参数", "Get Server Info Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetServerInfoPara</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetServerInfo") == 0)
    {
        g_StringLanType(szLan, "获取设备信息", "Get Server Info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetServerInfo</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVersionPara") == 0)
    {
        g_StringLanType(szLan, "获取版本信息", "Get Version Info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVersionPara</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetPicPara") == 0)
    {
        g_StringLanType(szLan, "OSD显示参数", "Get OSD Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetPicPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetShowStringPara") == 0)
    {
        g_StringLanType(szLan, "OSD字符叠加", "Get OSD show string");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetShowStringPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetMotionPara") == 0)
    {
        g_StringLanType(szLan, "移动侦测使能", "Get motion param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetMotionPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }

    else if (strcmp(m_csDetail, "GetMotionArea") == 0)
    {
        g_StringLanType(szLan, "移动侦测布防", "Get motion area");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetMotionArea</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetAlarmDeploymentTime") == 0)
    {
        g_StringLanType(szLan, "布防时间", "Get alarm deployment time");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetAlarmDeploymentTime</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigParam3>1</ConfigParam3>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetAlarmTriggerRecord") == 0)
    {
        g_StringLanType(szLan, "通道录像参数", "Get alarm trigger record param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetAlarmTriggerRecord</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetAlarmLinkageType") == 0)
    {
        g_StringLanType(szLan, "联动参数", "Get alarm linkage type");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetAlarmLinkageType</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetAlarmTriggerCapture") == 0)
    {
        g_StringLanType(szLan, "抓拍参数", "Get alarm trigger param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetAlarmTriggerCapture</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetHideAlarmPara") == 0)
    {
        g_StringLanType(szLan, "遮挡使能", "Get hide alarm param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetHideAlarmPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetHideAlarmArea") == 0)
    {
        g_StringLanType(szLan, "遮挡区域", "Get hide alarm param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetHideAlarmArea</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVILostPara") == 0)
    {
        g_StringLanType(szLan, "视频丢失使能", "Get video lost param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVILostPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetHidePara") == 0)
    {
        g_StringLanType(szLan, "隐私使能", "Get hide param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetHidePara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetHideArea") == 0)
    {
        g_StringLanType(szLan, "隐私区域", "Get hide area");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetHideArea</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetChanVideoStatus") == 0)
    {
        g_StringLanType(szLan, "通道视频状态", "Get chan video status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetChanVideoStatus</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetAlarmInPara") == 0)
    {
        g_StringLanType(szLan, "报警输入参数", "Alarm In Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetAlarmInPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetAlarmOutPara") == 0)
    {
        g_StringLanType(szLan, "报警输出参数", "Alarm Out Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetAlarmOutPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetAlarmOutStatus") == 0)
    {
        g_StringLanType(szLan, "报警输出状态", "Alarm Out Status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetAlarmOutStatus</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetCapturePicPlanPara") == 0)
    {
        g_StringLanType(szLan, "定时抓图", "Capture Picture");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetCapturePicPlanPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetRS485Para") == 0)
    {
        g_StringLanType(szLan, "RS485 配置", "RS485 Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetRS485Para</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetRS232Para") == 0)
    {
        g_StringLanType(szLan, "RS232 配置", "RS232 Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetRS232Para</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetRecordPlanPara") == 0)
    {
        g_StringLanType(szLan, "录像计划", "Record Plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetRecordPlanPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetCMSInfoPara") == 0)
    {
        g_StringLanType(szLan, "设备注册信息", "CMS Info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetCMSInfoPara</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetGPSInfoPara") == 0)
    {
        g_StringLanType(szLan, "GPS 信息", "GPS Info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetGPSInfoPara</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetOperateCode") == 0)
    {
        g_StringLanType(szLan, "操作密码", "Operate Code");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetOperateCode</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetZeroChannelInfo") == 0)
    {
        g_StringLanType(szLan, "零通道配置", "Zero Channel");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetZeroChannelInfo</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetFormatDiskProgress") == 0)
    {
        g_StringLanType(szLan, "硬盘格式化", "Disk Format");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetFormatDiskProgress</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetPreviewPara") == 0)
    {
        g_StringLanType(szLan, "预览循序配置", "Preview Order");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetPreviewPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetDevAbility") == 0)
    {
        g_StringLanType(szLan, "设备软件能力", "Software ability");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetDevAbility</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetDevVCAAbility") == 0)
    {
        g_StringLanType(szLan, "设备智能能力", "VCA ability");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetDevVCAAbility</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetDevChannelVCAAbility") == 0)
    {
        g_StringLanType(szLan, "设备通道智能能力", "VCA channel ability");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetDevChannelVCAAbility</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVCAFaceSnapCapa") == 0)
    {
        g_StringLanType(szLan, "人脸抓拍能力", "VCA  face snap ability");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVCAFaceSnapCapa</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVCAFaceSnapRuleCapa") == 0)
    {
        g_StringLanType(szLan, "人脸抓拍规则能力", "VCA  face snap rule ability");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVCAFaceSnapRuleCapa</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetPassengerPara") == 0)
    {
        g_StringLanType(szLan, "客流量使能参数", "Passenger enable cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetPassengerPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetPassengerOSDPara") == 0)
    {
        g_StringLanType(szLan, "客流量OSD参数", "Passenger OSD cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetPassengerOSDPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetPassengerRegionPara") == 0)
    {
        g_StringLanType(szLan, "客流量区域参数", "Passenger region cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetPassengerRegionPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetPassengerLinePara") == 0)
    {
        g_StringLanType(szLan, "客流量划线参数", "Passenger line cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetPassengerLinePara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetPassengerSetupPara") == 0)
    {
        g_StringLanType(szLan, "客流量安装配置参数", "Passenger setup cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetPassengerSetupPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetCalibrationPara") == 0)
    {
        g_StringLanType(szLan, "客流量标定参数", "Passenger calibration cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetCalibrationPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVCACapCtrlCfg") == 0)
    {
        g_StringLanType(szLan, "智能控制信息能力参数", "VCA cap control cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVCACapCtrlCfg</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVCACtrlCfg") == 0)
    {
        g_StringLanType(szLan, "智能控制信息参数", "VCA control cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVCACtrlCfg</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVCAVersion") == 0)
    {
        g_StringLanType(szLan, "获取算法库版本", "Get VCA version");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVCAVersion</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVCAFaceSnapCfg") == 0)
    {
        g_StringLanType(szLan, "人脸抓拍参数", "VCA face snap cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVCAFaceSnapCfg</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetVCAFaceSnapRuleCfg") == 0)
    {
        g_StringLanType(szLan, "人脸抓拍规则配置", "VCA face snap rule cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetVCAFaceSnapRuleCfg</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetRecordStatus") == 0)
    {
        g_StringLanType(szLan, "获取设备月录像分布状态", "Get record status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetRecordStatus</ConfigCmd>\r\n"\
            //通道号
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            //月份
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            //年份
            "<ConfigParam3>2016</ConfigParam3>\r\n"\
            //保留
            "<ConfigParam4>0</ConfigParam4>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetNetStatus") == 0)
    {
        g_StringLanType(szLan, "获取设备当前网络状态", "Get device net status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetNetStatus</ConfigCmd>\r\n"\
            "<ConfigParam1>0</ConfigParam1>\r\n"\
            "<ConfigParam2>0</ConfigParam2>\r\n"\
            "<ConfigParam3>0</ConfigParam3>\r\n"\
            "<ConfigParam4>0</ConfigParam4>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "GetDialMode") == 0)
    {
        g_StringLanType(szLan, "获取设备拨号制式", "Get dial mode");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetDialMode</ConfigCmd>\r\n"\
            "<ConfigParam1>0</ConfigParam1>\r\n"\
            "<ConfigParam2>0</ConfigParam2>\r\n"\
            "<ConfigParam3>0</ConfigParam3>\r\n"\
            "<ConfigParam4>0</ConfigParam4>\r\n"\
            "</Params>\r\n");
    }

    if (strcmp(m_csDetail, "GetCardWeekPlan") == 0)
    {
        g_StringLanType(szLan, "获取卡权限周计划使能", "Get Card week plan status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2-ConfigParam4——无效
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardWeekPlanCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡权限周计划使能能力", "Get capabilities of Card week plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardWeekPlanTime") == 0)
    {
        g_StringLanType(szLan, "获取卡权限周计划时间", "Get Card week plan time");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2——WeekDay（xs:integer 0-6）ConfigParam3-ConfigParam4——无效
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardWeekPlanTimeCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡权限周计划时间能力", "Get capabilities of Card week plan time");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardHolidayPlan") == 0)
    {
        g_StringLanType(szLan, "获取卡权限假日计划", "Get Card holiday plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayPlanNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardHolidayPlanCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡权限假日计划能力", "Get capabilities of Card holiday plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardHolidayGroup") == 0)
    {
        g_StringLanType(szLan, "获取卡权限假日组计划", "Get Card holiday group plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayGroupNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardHolidayGroupCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡权限假日组计划能力", "Get capabilities of Card holiday group plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardTemplate") == 0)
    {
        g_StringLanType(szLan, "获取卡权限计划模板", "Get Card template plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardTemplateNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardTemplateCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡权限计划模板能力", "Get capabilities of Card template plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusWeekPlan") == 0)
    {
        g_StringLanType(szLan, "获取门状态周计划使能", "Get door status week plan status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2-ConfigParam4——无效
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusWeekPlanCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门状态周计划使能能力", "Get capabilities of door status week plan status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusWeekPlanTime") == 0)
    {
        g_StringLanType(szLan, "获取门状态周计划时间", "Get door status week plan time");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2——WeekDay（xs:integer 0-6）ConfigParam3-ConfigParam4——无效
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusWeekPlanTimeCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门状态周计划时间能力", "Get capabilities of door status week plan time");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusHolidayPlan") == 0)
    {
        g_StringLanType(szLan, "获取门状态假日计划", "Get door status holiday plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayPlanNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusHolidayPlanCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门状态假日计划能力", "Get capabilities of door status holiday plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusHolidayGroup") == 0)
    {
        g_StringLanType(szLan, "获取门状态假日组计划", "Get door status holiday group plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayGroupNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusHolidayGroupCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门状态假日计划能力", "Get capabilities of door status holiday group plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusTemplate") == 0)
    {
        g_StringLanType(szLan, "获取门状态计划模板", "Get door status template plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorStatusTemplateNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorStatusTemplateCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门状态计划模板能力", "Get capabilities of door status template plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyWeekPlan") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式周计划使能", "Get reader verification mode week plan status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2-ConfigParam4——无效
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyWeekPlanCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式周计划使能能力", "Get capabilities of reader verification mode week plan status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyWeekPlanTime") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式周计划时间", "Get reader verification mode week plan time");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2——WeekDay（xs:integer 0-6）ConfigParam3-ConfigParam4——无效
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyWeekPlanTimeCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式周计划时间能力", "Get capabilities of reader verification mode week plan time");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail,"0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyHolidayPlan") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式假日计划", "Get reader verification mode holiday plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayPlanNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyHolidayPlanCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式假日计划能力", "Get capabilities of reader verification mode holiday plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyHolidayGroup") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式假日组计划", "Get reader verification mode holiday group plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayGroupNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyHolidayGroupCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式假日计划能力", "Get capabilities of reader verification mode holiday group plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyTemplate") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式计划模板", "Get reader verification mode template plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——ReaderVertifyTemplateNo（xs:integer编号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyTemplateCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证方式计划模板能力", "Get capabilities of reader verification mode template plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardCfg") == 0)
    {
        g_StringLanType(szLan, "获取卡参数", "Get card parameters");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardNo（卡号xs：string 32字符串类型）ConfigParam2——CheckCardNo是否校验卡号（xs:boolean, 1,0）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "123124345155364563456345634563", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡参数能力", "Get capabilities of card parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardRightCfg") == 0)
    {
        g_StringLanType(szLan, "获取卡权限", "Get card rights");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardNo（卡号xs：string 32字符串类型）ConfigParam2——CheckCardNo是否校验卡号（xs:boolean, 1,0）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "123124345155364563456345634563", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardRightCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡权限能力", "Get capabilities of card rights");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardPasswdOpenDoorCfg") == 0)
    {
        g_StringLanType(szLan, "获取卡密码开门使能", "Get the status of open door with card password");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardNo（卡号xs：string 32字符串类型）ConfigParam2——CheckCardNo是否校验卡号（xs:boolean, 1,0）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "123124345155364563456345634563", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardPasswdOpenDoorCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡密码开门使能能力", "Get capabilities of open door with card password");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetHostWorkStatus") == 0)
    {
        g_StringLanType(szLan, "获取主机工作状态", "Get host work status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetHostWorkStatusCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取主机工作状态能力", "Get capabilities of host work status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardReaderWorkStatus") == 0)
    {
        g_StringLanType(szLan, "获取读卡器工作状态", "Get card reader work status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——GroupNo（xs:integer组号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardReaderWorkStatusCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器工作状态能力", "Get capabilities of card reader work status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAlarmInStatus") == 0)
    {
        g_StringLanType(szLan, "获取报警输入状态", "Get input alarm status");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——AlarmInNo（xs:integer报警输出通道号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAlarmInStatusCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取报警输入状态能力", "Get capabilities of input alarm status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAlarmOutStatus") == 0)
    {
        g_StringLanType(szLan, "获取报警输出状态", "Get output alarm status");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——Channel（通道号从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAlarmOutStatusCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取报警输出状态能力", "Get capabilities of output alarm status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorCfg") == 0)
    {
        g_StringLanType(szLan, "获取门参数", "Get door configure parameters");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer门编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门参数能力", "Get capabilities of door parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorWorkStatus") == 0)
    {
        g_StringLanType(szLan, "获取门状态", "Get door work status");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer门编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorWorkStatusCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门状态能力", "Get capabilities of door work status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorWorkStatusPlan") == 0)
    {
        g_StringLanType(szLan, "获取门状态计划", "Get door work plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer门编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDoorWorkStatusPlanCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门状态计划能力", "Get capabilities of door work plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardReaderCfg") == 0)
    {
        g_StringLanType(szLan, "获取读卡器参数", "Get card reader configuration");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardReaderNo（xs:integer读卡器编号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardReaderCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器参数能力", "Get capabilities of card reader configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyPlan") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证计划", "Get card reader verify plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardReaderNo（xs:integer读卡器编号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReaderVertifyPlanCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器验证计划能力", "Get capabilities of card reader verify plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetGroupCfg") == 0)
    {
        g_StringLanType(szLan, "获取群组配置参数", "Get group configuration");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——GroupNo（xs:integer群组编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetGroupCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取群组配置参数能力", "Get capabilities of group configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetMultiCardCfg") == 0)
    {
        g_StringLanType(szLan, "获取多重卡认证模式参数", "Get multi-card verification mode parameters");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer门编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetMultiCardCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取多重卡认证模式能力", "Get capabilities of multi-card verification mode parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetMultiCardGroupCfg") == 0)
    {
        g_StringLanType(szLan, "获取多重卡认证群组参数", "Get multi-card verification group parameters");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer门编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetMultiCardGroupCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取多重卡认证群组能力", "Get capabilities of multi-card verification group parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAntiSneakCfg") == 0)
    {
        g_StringLanType(szLan, "获取反潜回主机参数", "Get host anti sneak parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAntiSneakCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取反潜回主机参数能力", "Get capabilities of host anti sneak parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAntiSneakReaderCfg") == 0)
    {
        g_StringLanType(szLan, "获取反潜回主机读卡器参数", "Get anti sneak host card reader parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——ReaderID（xs:integer读卡器编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAntiSneakReaderCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取反潜回主机读卡器参数能力", "Get capabilities of host anti sneak card reader parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetMultiDoorInterlockCfg") == 0)
    {
        g_StringLanType(szLan, "获取多门互锁参数", "Get multi-door interlock parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetMultiDoorInterlockCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取多门互锁参数能力", "Get capabilities of multi-door interlock parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetMultiDoorInterlockGroupCfg") == 0)
    {
        g_StringLanType(szLan, "获取多门互锁组参数", "Get multi-door interlock group parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——GroupNo（xs:integer互锁编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetMultiDoorInterlockGroupCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取多门互锁组参数能力", "Get capabilities of multi-door interlock group parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAntiSneakMultiHostCfg") == 0)
    {
        g_StringLanType(szLan, "获取跨主机反潜回参数", "Get multi-host anti sneak parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAntiSneakMultiHostCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取跨主机反潜回参数能力", "Get capabilities of multi-host anti sneak parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAntiSneakMultiHostReaderCfg") == 0)
    {
        g_StringLanType(szLan, "获取跨主机反潜回读卡器参数", "Get multi-host anti sneak card reader parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——ReaderInfoNo（xs:integer读卡器信息编号,范围 1-16）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAntiSneakMultiHostCardReaderCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取跨主机反潜回读卡器参数能力", "Get capabilities of multi-host anti sneak card reader parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCaseStatus") == 0)
    {
        g_StringLanType(szLan, "获取事件触发器状态", "Get case status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CaseSensorNo（事件触发器编号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCaseStatusCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件触发器状态能力", "Get capabilities of case status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventLinkCfg") == 0)
    {
        g_StringLanType(szLan, "获取事件触发器联动蜂鸣器配置", "Get event linkage buzzer configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CaseSensorNo（事件触发器编号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventLinkCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件触发器联动蜂鸣器能力", "Get capabilities of event linkage buzzer configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventLinkOpenDoorCfg") == 0)
    {
        g_StringLanType(szLan, "获取事件触发器联动门状态配置", "Get event linkage door status configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CaseSensorNo（事件触发器编号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventLinkOpenDoorCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件触发器联动门状态能力", "Get capabilities of event linkage door status configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCaseSensorLinkAlarmOut") == 0)
    {
        g_StringLanType(szLan, "获取事件触发器联动报警输出配置", "Get event linkage alarm output configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CaseSensorNo（事件触发器编号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCaseSensorLinkAlarmOutCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件触发器联动报警输出能力", "Get capabilities of event linkage alarm output configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkBuzzer") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动蜂鸣器配置", "Get card and event linkage buzzer configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkBuzzerCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动蜂鸣器能力", "Get capabilities of card and event linkage buzzer configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkDoor") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动门动作参数", "Get card and event linkage door configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkDoorCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动门动作能力", "Get capabilities of card and event linkage door configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkAlarmout") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动报警输出参数", "Get card and event linkage alarm output configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkAlarmoutCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动报警输出能力", "Get capabilities of card and event linkage alarm output configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkAlarmIn") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动防区", "Get card and event linkage alarm input configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkAlarmInCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动防区能力", "Get capabilities of card and event linkage alarm input configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetFingerPrint") == 0)
    {
        g_StringLanType(szLan, "获取指纹信息", "Get finger print");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CardNo（卡号，xs:string 32个字节）ConfigParam2——FingerNo（xs:integer 手指编号）ConfigParam3——readerNo（xs:integer读卡器编号）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1234567890", "1", "1", "0");
    }
    else if (strcmp(m_csDetail, "GetFingerPrintCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取指纹信息能力", "Get capabilities of finger print");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDelFingerPrintByCardNoCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取按卡号删除指纹信息能力", "Get capabilities of delete finger print by card number");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDelFingerPrintByReaderIDCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取按读卡器删除指纹信息能力", "Get capabilities of delete finger print by card reader");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetPhoneCfg") == 0)
    {
        g_StringLanType(szLan, "获取手机白名单信息", "Get phone white list");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WhiteListNo（xs:integer手机白名单编号）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetPhoneCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取手机白名单信息能力", "Get capabilities of phone white list");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetSMSRightCfg") == 0)
    {
        g_StringLanType(szLan, "获取手机白名单权限配置", "Get access rights configuration for the phone white list");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WhiteListNo（xs:integer手机白名单编号）ConfigParam2——GroupNo(xs:integer 组号，从1开始)
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetSMSRightCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取手机白名单权限配置能力", "Get capabilities of access rights configuration for the phone white list");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetWirelessNetWorkCfg") == 0)
    {
        g_StringLanType(szLan, "获取无线网络配置", "Get wireless network configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetWirelessNetWorkCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取无线网络配置能力", "Get capabilities of wireless network configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetWirelessNetCenterCfg") == 0)
    {
        g_StringLanType(szLan, "获取无线网络中心GPRS配置", "Get wireless network GPRS center configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——NetCenterNo（xs:integer网络中心编号）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetWirelessNetCenterCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取无线网络中心GPRS能力", "Get capabilities of wireless network GPRS center configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReportCenterCfg") == 0)
    {
        g_StringLanType(szLan, "获取报告上传方式配置", "Get report upload configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CenterGroupNo（xs:integer中心组序号）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetReportCenterCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取报告上传方式能力", "Get capabilities of report upload configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetNetPara") == 0)
    {
        g_StringLanType(szLan, "获取网络参数", "Get net parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetNetParaCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取网络参数能力", "Get capabilities of net parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAlarmInParam") == 0)
    {
        g_StringLanType(szLan, "获取报警输入参数（防区参数）", "Get alarm input parameters");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——RegionNo（xs:integer防区号，从0开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAlarmInParamCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取报警输入参数能力（防区参数）", "Get capabilities of alarm input parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDSTCfg") == 0)
    {
        g_StringLanType(szLan, "获取夏令时参数", "Get DST parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDSTCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取夏令时参数能力", "Get capabilities of DST parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetSecurityCfg") == 0)
    {
        g_StringLanType(szLan, "获取安全认证参数", "Get security authentication parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetSecurityCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取安全认证参数能力", "Get capabilities of security authentication parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAlarmHostNetCenterCfg") == 0)
    {
        g_StringLanType(szLan, "获取网络中心参数", "Get network center parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——NetCenterNo（xs:integer网络中心编号）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetAlarmHostNetCenterCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取网络中心参数能力", "Get capabilities of network center parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetACSCfg") == 0)
    {
        g_StringLanType(szLan, "获取门禁主机参数", "Get ACS host parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetACSCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门禁主机参数能力", "Get capabilities of ACS host parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardUserInfo") == 0)
    {
        g_StringLanType(szLan, "获取卡号关联用户信息参数", "Get card number associated user information parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——cardNo（xs:string 32字节卡号）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1234567890", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardUserInfoCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取卡号关联用户信息参数能力", "Get capabilities of card number associated user information");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkCapturePic") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动抓拍参数", "Get card number and event linkage capture parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetEventCardLinkCapturePicCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取事件及卡号联动抓拍能力", "Get capabilities of card number and event linkage capture");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetFaceDetectRuleCfg") == 0)
    {
        g_StringLanType(szLan, "获取人脸检测参数", "Get face detection parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——channelNo（xs:integer通道号）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetFaceDetectRuleCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取人脸检测参数能力", "Get capabilities of face detection parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetContinousSnapCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取触发抓拍参数能力", "Get capabilities of continuous capture parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    if (strcmp(m_csDetail, "GetUploadPicCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取底图上传能力", "Get capabilities of picture upload");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetDelPictureCfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取删除底图能力", "Get capabilities of delete picture");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetACSFunctionCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取门禁主机功能能力集", "Get capabilities of ACS host");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetServerInfoParaCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取设备基本信息能力", "Get capabilities of server information parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCMSInfoParaCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取设备注册信息能力", "Get capabilities of CMS information parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardReaderVCACfg") == 0)
    {
        g_StringLanType(szLan, "获取读卡器智能相关参数", "Get card reader VCA configuration");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardReaderNo（xs:integer读卡器编号，从1开始）
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0");
    }
    else if (strcmp(m_csDetail, "GetCardReaderVCACfgCapabilities") == 0)
    {
        g_StringLanType(szLan, "获取读卡器智能相关参数能力", "Get capabilities of card reader VCA configuration");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(GETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0");
    }

    //设置
    else if (strcmp(m_csDetail, "SetReaderVertifyPlan") == 0)
    {
        g_StringLanType(szLan, "设置读卡器验证计划", "Set reader verify plan");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CardReaderNo（xs:integer读卡器编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<ReaderVertifyPlan>\r\n"\
            "<templateNo>1</templateNo>\r\n"\
            "</ReaderVertifyPlan>\r\n");
    }
    else if (strcmp(m_csDetail, "DelPictureCfg") == 0)
    {
        g_StringLanType(szLan, "删除底图", "Delete picture");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<DelPictureCfg>\r\n"\
            "<PicUrl>http://10.64.61.98:8088/image/vrb2/i2/41fe3e8b5e564aff84b9c27cd854b138/00004?key=3efd&offset=625175202&high=30618</PicUrl>\r\n"\
            "</DelPictureCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetUploadPicCfg") == 0)
    {
        g_StringLanType(szLan, "设置底图上传", "Set picture upload parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<UploadPicCfg>\r\n"\
            "<enableUp>1</enableUp>\r\n"\
            "<useType>1</useType>\r\n"\
            "<sequence>1</sequence>\r\n"\
            "<BasemapCfg>\r\n"\
            "<sourWidth>100</sourWidth>\r\n"\
            "<sourHeight>100</sourHeight>\r\n"\
            "</BasemapCfg>\r\n"\
            "<PicUrl>http://10.64.61.98:8088/image/vrb2/i2/41fe3e8b5e564aff84b9c27cd854b138/00004?key=3efd&offset=625175202&high=30618</PicUrl>\r\n"\
            "</UploadPicCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetContinousSnapCfg") == 0)
    {
        g_StringLanType(szLan, "设置触发抓拍参数", "Set continuous capture parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<ContinousSnapCfg>\r\n"\
            "<relatedDriveWay>1</relatedDriveWay>\r\n"\
            "<snapTimes>3</snapTimes>\r\n"\
            "<snapWaitTime>100</snapWaitTime>\r\n"\
            "<IntervalTimeList>\r\n"\
            "<intervalTime>100</intervalTime>\r\n"\
            "</IntervalTimeList>\r\n"\
            "<JpegParam>\r\n"\
            "<pictureSize>0xff</pictureSize>\r\n"\
            "<pictureQuality>best</pictureQuality>\r\n"\
            "</JpegParam>\r\n"\
            "</ContinousSnapCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetFaceDetectRuleCfg") == 0)
    {
        g_StringLanType(szLan, "设置人脸检测参数", "Set face detection parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——channelNo（xs:integer通道号）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<FaceDetectRuleCfg>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<cardNo>123123123</cardNo>\r\n"\
            "<VcaPolygon>\r\n"\
            "<pointNum>3</pointNum>\r\n"\
            "<PosList>\r\n"\
            "<Pos>\r\n"\
            "<XCoordinate>40</XCoordinate>\r\n"\
            "<XCoordinate>50</XCoordinate>\r\n"\
            "</Pos>\r\n"\
            "</PosList>\r\n"\
            "</VcaPolygon>\r\n"\
            "<sensitivity>2</sensitivity>\r\n"\
            "<SizeFilter>\r\n"\
            "<active>1</active>\r\n"\
            "<mode>1</mode>\r\n"\
            "<MiniRect>\r\n"\
            "<XCoordinate>6</XCoordinate>\r\n"\
            "<XCoordinate>6</XCoordinate>\r\n"\
            "<width>5</width>\r\n"\
            "<height>5</height>\r\n"\
            "</MiniRect>\r\n"\
            "<MaxRect>\r\n"\
            "<XCoordinate>6</XCoordinate>\r\n"\
            "<XCoordinate>6</XCoordinate>\r\n"\
            "<width>10</width>\r\n"\
            "<height>10</height>\r\n"\
            "</MaxRect>\r\n"\
            "</SizeFilter>\r\n"\
            "</FaceDetectRuleCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetEventCardLinkCapturePic") == 0)
    {
        g_StringLanType(szLan, "设置事件及卡号联动抓拍", "Set card number and event linkage capture parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<EventCardLinkCapturePic>\r\n"\
            "<type>1</type>\r\n"\
            "<cardNo>123123123</cardNo>\r\n"\
            "<mainEvent>1</mainEvent>\r\n"\
            "<subEvent>2</subEvent>\r\n"\
            "<employeeNo>123</employeeNo>\r\n"\
            "<sourceID>1</sourceID>\r\n"\
            "<capturePic>1</capturePic>\r\n"\
            "</EventCardLinkCapturePic>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardUserInfo") == 0)
    {
        g_StringLanType(szLan, "设置卡号关联用户信息参数", "Set card number associated user information parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——cardNo（xs:string 32字节卡号）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1234567890", "0", "0", "0", "<CardUserInfo>\r\n"\
            "<userName>test</userName>\r\n"\
            "</CardUserInfo>\r\n");
    }
    else if (strcmp(m_csDetail, "SetACSCfg") == 0)
    {
        g_StringLanType(szLan, "设置门禁主机参数", "Set ACS host parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<AcsCfg>\r\n"\
            "<rs485Backup>1</rs485Backup>\r\n"\
            "<showCapPic>1</showCapPic>\r\n"\
            "<showCardNo>1</showCardNo>\r\n"\
            "<showUserInfo>1</showUserInfo>\r\n"\
            "<overlayUserInfo>1</overlayUserInfo>\r\n"\
            "<voicePrompt>1</voicePrompt>\r\n"\
            "<uploadCapPic>1</uploadCapPic>\r\n"\
            "<saveCapPic>1</saveCapPic>\r\n"\
            "<inputCardNo>1</inputCardNo>\r\n"\
            "<protocol>Private</protocol>\r\n"\
            "</AcsCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmHostNetCenterCfg") == 0)
    {
        g_StringLanType(szLan, "网络中心配置", "Set network center parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——NetCenterNo（xs:integer网络中心编号）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<AlarmHostNetCenterCfg>\r\n"\
            // 需要使用有效的IP、端口和域名等，这里只是示例
            "<ip>192.168.0.2</ip>\r\n"\
            "<port>8888</port>\r\n"\
            "<domainName>www.justest.cn</domainName>\r\n"\
            "<reportProto>ehome</reportProto>\r\n"\
            "<deviceID>123</deviceID>\r\n"\
            "</AlarmHostNetCenterCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetSecurityCfg") == 0)
    {
        g_StringLanType(szLan, "安全认证配置", "Set security authentication");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<SecurityCfg>\r\n"\
            "<level>open</level>\r\n"\
            "<cmdCheckLevel>1</cmdCheckLevel>\r\n"\
            "<loginLevel>1</loginLevel>\r\n"\
            "<webAuthentication>digest</webAuthentication>\r\n"\
            "<rtspAuthentication>basic</rtspAuthentication>\r\n"\
            "<telnetServer>0</telnetServer>\r\n"\
            "<SSHServer>1</SSHServer>\r\n"\
            "</SecurityCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetDSTCfg") == 0)
    {
        g_StringLanType(szLan, "夏令时配置", "Set DST");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<DSTCfg>\r\n"\
            "<enable>1</enable>\r\n"\
            "<DSTBias>1</DSTBias>\r\n"\
            "<startMonth>1</startMonth>\r\n"\
            "<startWeekNo>1</startWeekNo>\r\n"\
            "<startWeekDate>1</startWeekDate>\r\n"\
            "<startHour>1</startHour>\r\n"\
            "<startMin>1</startMin>\r\n"\
            "<stopMonth>2</stopMonth>\r\n"\
            "<stopWeekNo>1</stopWeekNo>\r\n"\
            "<stopWeekDate>1</stopWeekDate>\r\n"\
            "<stopHour>1</stopHour>\r\n"\
            "<stopMin>1</stopMin>\r\n"\
            "</DSTCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmInParam") == 0)
    {
        g_StringLanType(szLan, "报警输入参数配置（防区参数配置）", "Set alarm input parameters(sector)");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——RegionNo（xs:integer防区号，从0开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<RegionParam>\r\n"\
            "<name>test sector</name>\r\n"\
            "<detectorType>SMOKE_DETECTOR</detectorType>\r\n"\
            "<alarmType>1</alarmType>\r\n"\
            "<uploadAlarmRecoveryReport>1</uploadAlarmRecoveryReport>\r\n"\
            "<delayTime>10</delayTime>\r\n"\
            "<sensitivity>1</sensitivity>\r\n"\
            "<arrayBypass>0</arrayBypass>\r\n"\
            "<jointSubSystemNo>1</jointSubSystemNo>\r\n"\
            "<moduleStatus>1</moduleStatus>\r\n"\
            "<moduleAddr>1</moduleAddr>\r\n"\
            "<moduleChan>1</moduleChan>\r\n"\
            "<moduleType>1</moduleType>\r\n"\
            "<zoneIndex>1</zoneIndex>\r\n"\
            "</RegionParam>\r\n");
    }
    else if (strcmp(m_csDetail, "SetNetPara") == 0)
    {
        g_StringLanType(szLan, "配置网络参数", "Set net parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<NETINFO>\r\n"\
            "<NetInterface>5</NetInterface>\r\n"\
            "<ServerIP>192.0.0.64</ServerIP>\r\n"\
            "<ServerPort>8000</ServerPort>\r\n"\
            "<ServerMaskIP>255.255.255.0</ServerMaskIP>\r\n"\
            "<ServerGateWayIP>192.0.0.1</ServerGateWayIP>\r\n"\
            "<MultiCastIP>0.0.0.0</MultiCastIP>\r\n"\
            "<MACAddr>8ce748cfbeba(only read)</MACAddr>\r\n"\
            "<HTTPPort>80</HTTPPort>\r\n"\
            "<MTU>81500</MTU>\r\n"\
            "<DNSServer1IP>8.8.8.8</DNSServer1IP>\r\n"\
            "<DNSServer2IP>8.8.8.8</DNSServer2IP>\r\n"\
            "<IPResolver>0.0.0.0</IPResolver>\r\n"\
            "<IPResolverPort>0</IPResolverPort>\r\n"\
            "<IsUsePPPOE>0</IsUsePPPOE>\r\n"\
            "<PPPOEUserName>set what you get(only read)</PPPOEUserName>\r\n"\
            "<PPPOEUserPsw>12345(change what you want)</PPPOEUserPsw>\r\n"\
            "<PPPOEResolverIP>set what you get(only read)</PPPOEResolverIP>\r\n"\
            "</NETINFO>\r\n");
    }
    else if (strcmp(m_csDetail, "SetReportCenterCfg") == 0)
    {
        g_StringLanType(szLan, "配置报告上传方式", "Set report upload configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CenterGroupNo（xs:integer中心组序号）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<ReportCenterCfg>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<dataType>1</dataType>\r\n"\
            "<AlarmModeList>\r\n"\
            "<alarmMode>N1</alarmMode>\r\n"\
            "</AlarmModeList>\r\n"\
            "</ReportCenterCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetWirelessNetCenterCfg") == 0)
    {
        g_StringLanType(szLan, "配置无线网络中心GPRS", "Set wireless network GPRS center configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——NetCenterNo（xs:integer网络中心编号）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<WirelessNetCenterCfg>\r\n"\
            // 需要使用有效的IP、端口和域名等，这里只是示例
            "<ip>192.168.0.2</ip>\r\n"\
            "<port>8888</port>\r\n"\
            "<domainName>www.justest.cn</domainName>\r\n"\
            "<reportProto>ehome</reportProto>\r\n"\
            "<deviceID>495816011</deviceID>\r\n"\
            "</WirelessNetCenterCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetWirelessNetWorkCfg") == 0)
    {
        g_StringLanType(szLan, "配置无线网络", "Configure wireless network");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<WirelessNetWorkCfg>\r\n"\
            "<APNName>test</APNName>\r\n"\
            "<userName>test user</userName>\r\n"\
            "<passwd>MTIzNDU=</passwd>\r\n"\
            "<reconnTime>10</reconnTime>\r\n"\
            "<overTime>30</overTime>\r\n"\
            "<detectLinkTime>10</detectLinkTime>\r\n"\
            "<SIMNum>18966666666</SIMNum>\r\n"\
            "<SIMIp>192.168.0.2</SIMIp>\r\n"\
            "</WirelessNetWorkCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetSMSRightCfg") == 0)
    {
        g_StringLanType(szLan, "配置手机白名单权限", "Set access rights for the phone white list");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WhiteListNo（xs:integer手机白名单编号）ConfigParam2——GroupNo(xs:integer 组号，从1开始)
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0", "<SMSRight>\r\n"\
            "<openDoorNo>1,2,3</openDoorNo>\r\n"\
            "<closeDoorNo>1,2,3</closeDoorNo>\r\n"\
            "<normalOpenDoorNo>1,2,3</normalOpenDoorNo>\r\n"\
            "<normalCloseDoorNo>1,2,3</normalCloseDoorNo>\r\n"\
            "<armChannelNo>1,2,3</armChannelNo>\r\n"\
            "<disarmChannelNo>1,2,3</disarmChannelNo>\r\n"\
            "</SMSRight>\r\n");
    }
    else if (strcmp(m_csDetail, "SetPhoneCfg") == 0)
    {
        g_StringLanType(szLan, "设置手机白名单信息", "Set phone white list");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WhiteListNo（xs:integer手机白名单编号）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<PhoneCfg>\r\n"\
            "<whiteList>1</whiteList>\r\n"\
            "<perssion>4</perssion>\r\n"\
            "</PhoneCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "DelFingerPrintByReaderID") == 0)
    {
        g_StringLanType(szLan, "按读卡器删除指纹信息", "Delete finger print by card reader");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——ReaderID（xs:integer读卡器编号，从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<FingerPrintByCardReader>\r\n"\
            "<enable>1</enable>\r\n"\
            "</FingerPrintByCardReader>\r\n");
    }
    else if (strcmp(m_csDetail, "DelFingerPrintByCardNo") == 0)
    {
        g_StringLanType(szLan, "按卡号删除指纹信息", "Delete finger print by card number");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CardNo（xs:string 32字节卡号）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1234567890", "0", "0", "0", "<FingerPrintByCard>\r\n"\
            "<readerNo>1,2,3</readerNo>\r\n"\
            "<fingerNo>1,2,3</fingerNo>\r\n"\
            "</FingerPrintByCard>\r\n");
    }
    else if (strcmp(m_csDetail, "SetFingerPrintCfg") == 0)
    {
        g_StringLanType(szLan, "添加指纹信息", "Set finger print");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardNo（xs:string 32字节卡号）ConfigParam2——FingerNo（xs:integer 指纹编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1234567890", "1", "0", "0", "<FingerPrintCfg>\r\n"\
            "<readerNo>1,2,3</readerNo>\r\n"\
            "<fingerType>0</fingerType>\r\n"\
            "<fingerData>300naXhcs?xh銄M&榵邔i&笀恝?葂寡(`N?&站A%XXI藨8婾藵貎*啸萺>討8奒刭q:?HhB峤坔候Exx给i垁6</fingerData>\r\n"\
            "</FingerPrintCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetEventCardLinkAlarmout") == 0)
    {
        g_StringLanType(szLan, "配置事件及卡号联动报警输出", "Set card and event linkage alarm output configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<EventCardLinkAlarmout>\r\n"\
            "<type>1</type>\r\n"\
            "<cardNo>123123123</cardNo>\r\n"\
            "<mainEvent>1</mainEvent>\r\n"\
            "<subEvent>2</subEvent>\r\n"\
            "<employeeNo>123</employeeNo>\r\n"\
            "<sourceID>1</sourceID>\r\n"\
            "<alarmOut>01011110000000000000000</alarmOut>\r\n"\
            "</EventCardLinkAlarmout>\r\n");
    }
    else if (strcmp(m_csDetail, "SetEventCardLinkAlarmIn") == 0)
    {
        g_StringLanType(szLan, "配置事件及卡号联动防区", "Set card and event linkage alarm input configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<GetEventCardLinkAlarmIn>\r\n"\
            "<type>1</type>\r\n"\
            "<cardNo>123123123</cardNo>\r\n"\
            "<mainEvent>1</mainEvent>\r\n"\
            "<subEvent>2</subEvent>\r\n"\
            "<employeeNo>123</employeeNo>\r\n"\
            "<sourceID>1</sourceID>\r\n"\
            "<alarmInSetup>1,2,3</alarmInSetup>\r\n"\
            "</GetEventCardLinkAlarmIn>\r\n");
    }
    else if (strcmp(m_csDetail, "SetEventCardLinkDoor") == 0)
    {
        g_StringLanType(szLan, "配置事件及卡号联动门动作", "Set card and event linkage door configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<EventCardLinkDoor>\r\n"\
            "<type>1</type>\r\n"\
            "<cardNo>123123123</cardNo>\r\n"\
            "<mainEvent>1</mainEvent>\r\n"\
            "<subEvent>2</subEvent>\r\n"\
            "<employeeNo>123</employeeNo>\r\n"\
            "<sourceID>1</sourceID>\r\n"\
            "<open>1,2</open>\r\n"\
            "<close>3,4</close>\r\n"\
            "<normalOpen>5,6</normalOpen>\r\n"\
            "<normalClose>7,8</normalClose>\r\n"\
            "</EventCardLinkDoor>\r\n");
    }
    else if (strcmp(m_csDetail, "SetEventCardLinkBuzzer") == 0)
    {
        g_StringLanType(szLan, "配置事件及卡号联动蜂鸣器", "Set card and event linkage buzzer configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——EventID（xs：integer 事件ID编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<EventCardLinkBuzzer>\r\n"\
            "<type>1</type>\r\n"\
            "<cardNo>123123123</cardNo>\r\n"\
            "<mainEvent>1</mainEvent>\r\n"\
            "<subEvent>2</subEvent>\r\n"\
            "<employeeNo>123</employeeNo>\r\n"\
            "<sourceID>1</sourceID>\r\n"\
            "<enabledMainDevBuzzer>1</enabledMainDevBuzzer>\r\n"\
            "<readerBuzzer>0xffffffff</readerBuzzer>\r\n"\
            "</EventCardLinkBuzzer>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCaseSensorLinkAlarmOut") == 0)
    {
        g_StringLanType(szLan, "配置事件触发器联动报警输出", "Set event linkage alarm output configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CaseSensorNo（事件触发器编号，从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<CaseSensorLinkAlarmout>\r\n"\
            "<alarmOut>01011110000000000000000</alarmOut>\r\n"\
            "</CaseSensorLinkAlarmout>\r\n");
    }
    else if (strcmp(m_csDetail, "SetEventLinkOpenDoorCfg") == 0)
    {
        g_StringLanType(szLan, "配置事件触发器联动门状态", "Set event linkage door status configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CaseSensorNo（事件触发器编号，从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<CaseSensorLinkDoor>\r\n"\
            "<open>1,2,3</open>\r\n"\
            "<close>4,5,6</close>\r\n"\
            "</CaseSensorLinkDoor>\r\n");
    }
    else if (strcmp(m_csDetail, "SetEventLinkCfg") == 0)
    {
        g_StringLanType(szLan, "配置事件触发器联动蜂鸣器", "Set event linkage buzzer configuration");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CaseSensorNo（事件触发器编号，从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<CaseSensorLinkBuzzer>\r\n"\
            "<enabledMainDevBuzzer>1</enabledMainDevBuzzer>\r\n"\
            "<readerBuzzer>1,2,3</readerBuzzer>\r\n"\
            "</CaseSensorLinkBuzzer>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCaseStatus") == 0)
    {
        g_StringLanType(szLan, "设置事件触发器状态", "Set case status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——CaseSensorNo（事件触发器编号，从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<CaseStatus>\r\n"\
            "<caseIn>1</caseIn>\r\n"\
            "</CaseStatus>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAntiSneakMultiHostReaderCfg") == 0)
    {
        g_StringLanType(szLan, "设置跨主机反潜回读卡器参数", "Set multi-host anti sneak card reader parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——ReaderInfoNo（xs:integer读卡器信息编号,范围 1-16）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<ReaderMultiHost>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<antiSnealHostNo>1</antiSnealHostNo>\r\n"\
            "<readerID>1</readerID>\r\n"\
            "<ReaderInfo>\r\n"\
            "<followAntiSnealHostNo>0,1,2</followAntiSnealHostNo>\r\n"\
            "<followUpCardReader>1,2,3</followUpCardReader>\r\n"\
            "</ReaderInfo>\r\n"\
            "</ReaderMultiHost>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAntiSneakMultiHostCfg") == 0)
    {
        g_StringLanType(szLan, "设置跨主机反潜回参数", "Set multi-host anti sneak parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<AntiSneakMultiHost>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<HostDevInfoList>\r\n"\
            "<HostDevInfo>\r\n"\
            "<ip>192.168.0.11</ip>\r\n"\
            "<port>9111</port>\r\n"\
            "<firstHost>1</firstHost>\r\n"\
            "<hostNo>1</hostNo>\r\n"\
            "</HostDevInfo>\r\n"\
            "</HostDevInfoList>\r\n"\
            "<StartReader>\r\n"\
            "<hostNo>1</hostNo>\r\n"\
            "<readerID>1</readerID>\r\n"\
            "</StartReader>\r\n"\
            "</AntiSneakMultiHost>\r\n");
    }
    else if (strcmp(m_csDetail, "SetMultiDoorInterlockGroupCfg") == 0)
    {
        g_StringLanType(szLan, "设置多门互锁组", "Set multi-door interlock group parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——GroupNo（xs:integer互锁编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<InterlockGroup>\r\n"\
            "<doorNo>1,2,3,4</doorNo>\r\n"\
            "</InterlockGroup>\r\n");
    }
    else if (strcmp(m_csDetail, "SetMultiDoorInterlockCfg") == 0)
    {
        g_StringLanType(szLan, "设置多门互锁", "Set multi-door interlock parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<MultiDoorInterlock>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "</MultiDoorInterlock>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAntiSneakReaderCfg") == 0)
    {
        g_StringLanType(szLan, "反潜回主机读卡器参数配置", "Set anti sneak host card reader parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——ReaderID（xs:integer读卡器编号, 从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<AntiSneakReader>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<followUpReader>1,2,3</followUpReader>\r\n"\
            "</AntiSneakReader>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAntiSneakCfg") == 0)
    {
        g_StringLanType(szLan, "反潜回主机配置", "Set anti sneak parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "0", "0", "0", "0", "<AntiSneak>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<startReaderNo>1</startReaderNo>\r\n"\
            "</AntiSneak>\r\n");
    }
    else if (strcmp(m_csDetail, "SetMultiCardGroupCfg") == 0)
    {
        g_StringLanType(szLan, "多重卡认证群组配置", "Set multi-card verification group parameters");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer门编号,从1开始）ConfigParam2——GroupNo（xs:integer群组编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0", "<MultiCardGroupCfg>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<offlineVerify>0</offlineVerify>\r\n"\
            "<templateNo>1</templateNo>\r\n"\
            "<GroupList>\r\n"\
            "<Group>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<memberNum>10</memberNum>\r\n"\
            "<sequenceNo>1</sequenceNo>\r\n"\
            "<groupNo>0xffffffff</groupNo>\r\n"\
            "</Group>\r\n"\
            "</GroupList>\r\n"\
            "</MultiCardGroupCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetMultiCardCfg") == 0)
    {
        g_StringLanType(szLan, "多重卡认证模式配置", "Set multi-card verification mode parameters");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer门编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<MultiCardCfg>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<intervalTime>20</intervalTime>\r\n"\
            "</MultiCardCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetGroupCfg") == 0)
    {
        g_StringLanType(szLan, "群组参数配置", "Configure group parameters");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——GroupNo（xs:integer群组编号, 从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<GroupCfg>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<enabledValidPeriod>1</enabledValidPeriod>\r\n"\
            "<startTime>2017-01-01T00:00:00</startTime>\r\n"\
            "<endTime>2018-01-01T00:00:00</endTime>\r\n"\
            "<timeType>0</timeType>\r\n"\
            "<groupName>JustTest</groupName>\r\n"\
            "</GroupCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardReaderCfg") == 0)
    {
        g_StringLanType(szLan, "设置读卡器参数", "Configure the card reader");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardReaderNo（xs:integer读卡器编号，从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<CardReaderCfg>\r\n"\
            "<enabled>1</enabled>\r\n"\
            // 读卡器类型，1-DS-K110XM/MK/C/CK，2-DS-K192AM/AMP，3-DS-K192BM/BMP，4-DS-K182AM/AMP，5-DS-K182BM/BMP, 6-DS-K182AMF/ACF，7-韦根或485不在线，
            // 8- DS-K1101M/MK，9- DS-K1101C/CK，10- DS-K1102M/MK/M-A，11- DS-K1102C/CK，12- DS-K1103M/MK，13- DS-K1103C/CK，14- DS-K1104M/MK，15- DS-K1104C/CK，
            // 16- DS-K1102S/SK/S-A，17- DS-K1102G/GK，18- DS-K1100S-B，19- DS-K1102EM/EMK，20- DS-K1102E/EK，21- DS-K1200EF，22- DS-K1200MF，23- DS-K1200CF，
            // 24- DS-K1300EF，25- DS-K1300MF，26- DS-K1300CF，27- DS-K1105E，28- DS-K1105M，29- DS-K1105C，30- DS-K182AMF，31- DS-K196AMF，32-DS-K194AMP
            "<cardReaderType>1</cardReaderType>\r\n"\
            "<okLedPolarity>1</okLedPolarity>\r\n"\
            "<errorLedPolarity>1</errorLedPolarity>\r\n"\
            "<buzzerPolarity>1</buzzerPolarity>\r\n"\
            "<swipeInterval>10</swipeInterval>\r\n"\
            "<pressTimeout>5</pressTimeout>\r\n"\
            "<failAlarmEnabled>1</failAlarmEnabled>\r\n"\
            "<maxReadCardFailNum>10</maxReadCardFailNum>\r\n"\
            "<tamperCheckEnabled>1</tamperCheckEnabled>\r\n"\
            "<offlineCheckTime>30</offlineCheckTime>\r\n"\
            // 指纹识别等级，1 - 1 / 10误认率，2 - 1 / 100误认率，3 - 1 / 1000误认率，4 - 1 / 10000误认率，5 - 1 / 100000误认率，6 - 1 / 1000000误认率，
            // 7 - 1 / 10000000误认率，8 - 1 / 100000000误认率，9 - 3 / 100误认率，10 - 3 / 1000误认率，11 - 3 / 10000误认率，12 - 3 / 100000误认率，
            // 13 - 3 / 1000000误认率，14 - 3 / 10000000误认率，15 - 3 / 100000000误认率，16 - Automatic Normal, 17 - Automatic Secure, 18 - Automatic More Secure
            "<fingerPrintCheckLevel>6</fingerPrintCheckLevel>\r\n"\
            "<buzzerTime>10</buzzerTime>\r\n"\
            "</CardReaderCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardReaderVCACfg") == 0)
    {
        g_StringLanType(szLan, "设置读卡器智能相关参数", "Configure the card reader VCA");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardReaderNo（xs:integer读卡器编号，从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<CardReaderVCACfg>\r\n"\
            "<faceQuality>10</faceQuality>\r\n"\
            "</CardReaderVCACfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetDoorWorkStatusPlan") == 0)
    {
        g_StringLanType(szLan, "设置门状态计划", "Set door work plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer门编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<DoorStatusPlan>\r\n"\
            "<templateNo>1</templateNo>\r\n"\
            "</DoorStatusPlan>\r\n");
    }
    else if (strcmp(m_csDetail, "SetDoorCfg") == 0)
    {
        g_StringLanType(szLan, "设置门参数", "Set door configuration");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorNo（xs:integer 门编号,从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<DoorCfg>\r\n"\
            "<doorName>JustTest</doorName>\r\n"\
            "<magneticType>0</magneticType>\r\n"\
            "<openButtonType>0</openButtonType>\r\n"\
            "<openDuration>5</openDuration>\r\n"\
            "<disabledOpenDuration>20</disabledOpenDuration>\r\n"\
            "<magneticAlarmTimeout>30</magneticAlarmTimeout>\r\n"\
            "<doorLockEnabled>1</doorLockEnabled>\r\n"\
            "<leaderCardEnabled>1</leaderCardEnabled>\r\n"\
            "<leaderCardMode>1</leaderCardMode>\r\n"\
            "<leaderCardOpenDuration>60</leaderCardOpenDuration>\r\n"\
            "<stressPassword>MTIzNDU=</stressPassword>\r\n"\
            "<superPassword>MTIzNDU=</superPassword>\r\n"\
            "<unlockPassword>MTIzNDU=</unlockPassword>\r\n"\
            "</DoorCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmOutStatus") == 0)
    {
        g_StringLanType(szLan, "设置报警输出状态", "Set output alarm status");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——Channel（通道号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<ALARMOUTSTATUSINFO>\r\n"\
            "<AlarmOutStatus>1</AlarmOutStatus>\r\n"\
            "</ALARMOUTSTATUSINFO>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmInStatus") == 0)
    {
        g_StringLanType(szLan, "设置报警输入状态", "Set input alarm status");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——AlarmInNo（xs:integer报警输出通道号，从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<AlarmInStatus>\r\n"\
            "<setUpStatus>1</setUpStatus>\r\n"\
            "<alarmIn>1</alarmIn>\r\n"\
            "</AlarmInStatus>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardPasswdOpenDoorCfg") == 0)
    {
        g_StringLanType(szLan, "设置卡密码开门使能", "Set the status of open door with card password");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "123124345155364563456345634563", "0", "0", "0", "<CardPasswdOpenDoorCfg>\r\n"\
            "<valid>1</valid>\r\n"\
            "<password>MTIzNDU=</password>\r\n"\
            "</CardPasswdOpenDoorCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardRightCfg") == 0)
    {
        g_StringLanType(szLan, "设置卡权限", "Set card rights");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "123124345155364563456345634563", "0", "0", "0", "<CardRight>\r\n"\
            // 门权限，最大32个门
            "<DoorRightList>\r\n"\
            "<DoorRight>\r\n"\
            "<doorNo>1</doorNo>\r\n"\
            // 门权限计划模板编号，单门最大4个计划模板
            "<planNo>1, 2</planNo>\r\n"\
            "</DoorRight>\r\n"\
            "</DoorRightList>\r\n"\
            "</CardRight>\r\n");
    }
    else if(strcmp(m_csDetail, "SetCardCfg") == 0)
    {
        g_StringLanType(szLan, "设置卡参数", "Set card parameters");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "123124345155364563456345634563", "0", "0", "0", "<CardCfg>\r\n"\
            // xs:boolean 置为0表示删除此卡
            "<valid>1</valid>\r\n"\
            // xs:integer,卡类型，1-普通卡，2-残疾人卡，3-黑名单卡，4-巡更卡，5-胁迫卡，6-超级卡，7-来宾卡 8-解除卡
            "<cardType>1</cardType>\r\n"\
            "<leaderCardEnabled>1</leaderCardEnabled>\r\n"\
            "<validTimeEnabled>1</validTimeEnabled>\r\n"\
            "<ValidTime>\r\n"\
            "<startTime>2017-01-01T00:00:00</startTime>\r\n"\
            "<endTime>2018-01-01T00:00:00</endTime>\r\n"\
            "</ValidTime>\r\n"\
            "<password>MTIzNDU=</password>"
            "<maxSwipeTime>0</maxSwipeTime>\r\n"\
            "<swipeTime>10</swipeTime>\r\n"\
            "<groupNo>1</groupNo>\r\n"\
            "</CardCfg>\r\n");
    }
    else if(strcmp(m_csDetail, "SetReaderVertifyTemplate") == 0)
    {
        g_StringLanType(szLan, "设置读卡器验证方式计划模板", "Set reader verification mode template plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorStatusTemplateNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<Template>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<name>JustTest</name>\r\n"\
            "<weekPlanNo>1</weekPlanNo>\r\n"\
            // 假日组，最多16个
            "<HGNo>1,2</HGNo>\r\n"\
            "</Template>\r\n");
    }
    else if (strcmp(m_csDetail, "SetReaderVertifyHolidayGroup") == 0)
    {
        g_StringLanType(szLan, "设置读卡器验证方式假日组计划", "Set reader verification mode holiday group plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayGroupNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<HolidayGroup>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<groupName>JustTest</groupName>\r\n"\
            // 假日列表，最多16个
            "<holidayNo>1,2</holidayNo>\r\n"\
            "</HolidayGroup>\r\n");
    }
    else if (strcmp(m_csDetail, "SetReaderVertifyHolidayPlan") == 0)
    {
        g_StringLanType(szLan, "设置读卡器验证方式假日计划", "Set reader verification mode holiday plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayPlanNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<HolidayPlan>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<startDate>2017-01-01T00:00:00</startDate>\r\n"\
            "<endDate>2017-01-07T00:00:00</endDate>\r\n"\
            // 时间段，最多8个
            "<TimeList>\r\n"\
            "<Time>\r\n"\
            "<startTime>2017-01-01T08:00:00</startTime>\r\n"\
            "<endTime>2017-01-01T12:00:00</endTime>\r\n"\
            // 该时间段下读卡器验证方式 0-无效，1-休眠，2-刷卡+密码，3-刷卡,4-刷卡或密码,5-指纹，6-指纹+密码，7-指纹或刷卡，8-指纹+刷卡，9-指纹+刷卡+密码（无先后顺序）
            "<verifyMode>2</verifyMode>>\r\n"\
            "</Time>\r\n"\
            "</TimeList>\r\n"\
            "</HolidayPlan>\r\n");
    }
    else if (strcmp(m_csDetail, "SetReaderVertifyWeekPlanTime") == 0)
    {
        g_StringLanType(szLan, "设置读卡器验证方式周计划时间", "Set reader verification mode week plan time status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2——WeekDay（xs:integer 0-6）ConfigParam3-ConfigParam4——无效
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0", "<WeekPlanTimeList>\r\n"\
            "<WeekPlanTime>\r\n"\
            "<startTime>2017-01-01T08:00:00</startTime>\r\n"\
            "<endTime>2017-01-01T09:30:00</endTime>\r\n"\
            // 该时间段下读卡器验证方式 0-无效，1-休眠，2-刷卡+密码，3-刷卡,4-刷卡或密码,5-指纹，6-指纹+密码，7-指纹或刷卡，8-指纹+刷卡，9-指纹+刷卡+密码（无先后顺序）
            "<verifyMode>2</verifyMode>>\r\n"\
            "</WeekPlanTime>\r\n"\
            "</WeekPlanTimeList>\r\n");
    }
    else if (strcmp(m_csDetail, "SetReaderVertifyWeekPlan") == 0)
    {
        g_StringLanType(szLan, "设置读卡器验证方式周计划使能", "Set reader verification mode week plan status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2-ConfigParam4——无效
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<WeekPlanCfg>\r\n<enabled>1</enabled>\r\n</WeekPlanCfg>\r\n");
    }
    if (strcmp(m_csDetail, "SetDoorStatusTemplate") == 0)
    {
        g_StringLanType(szLan, "设置门状态计划模板", "Set door status template plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——DoorStatusTemplateNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<Template>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<name>JustTest</name>\r\n"\
            "<weekPlanNo>1</weekPlanNo>\r\n"\
            // 假日组，最多16个
            "<HGNo>1,2</HGNo>\r\n"\
            "</Template>\r\n");
    }
    else if (strcmp(m_csDetail, "SetDoorStatusHolidayGroup") == 0)
    {
        g_StringLanType(szLan, "设置门状态假日组计划", "Set door status holiday group plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayGroupNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<HolidayGroup>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<groupName>JustTest</groupName>\r\n"\
            // 假日列表，最多16个
            "<holidayNo>1,2</holidayNo>\r\n"\
            "</HolidayGroup>\r\n");
    }
    else if (strcmp(m_csDetail, "SetDoorStatusHolidayPlan") == 0)
    {
        g_StringLanType(szLan, "设置门状态假日计划", "Set door status holiday plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayPlanNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<HolidayPlan>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<startDate>2017-01-01T00:00:00</startDate>\r\n"\
            "<endDate>2017-01-07T00:00:00</endDate>\r\n"\
            // 时间段，最多8个
            "<TimeList>\r\n"\
            "<Time>\r\n"\
            "<startTime>2017-01-01T08:00:00</startTime>\r\n"\
            "<endTime>2017-01-01T12:00:00</endTime>\r\n"\
            // 该时间段下门状态，0-无效，1-休眠，2-常开状态，3-常闭状态
            "<doorStatus>2</doorStatus>\r\n"\
            "</Time>\r\n"\
            "</TimeList>\r\n"\
            "</HolidayPlan>\r\n");
    }
    else if (strcmp(m_csDetail, "SetDoorStatusWeekPlanTime") == 0)
    {
        g_StringLanType(szLan, "设置门状态周计划时间", "Set door status week plan time status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2——WeekDay（xs:integer 0-6）ConfigParam3-ConfigParam4——无效
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0", "<WeekPlanTimeList>\r\n"\
            "<WeekPlanTime>\r\n"\
            "<startTime>2017-01-01T08:00:00</startTime>\r\n"\
            "<endTime>2017-01-01T09:30:00</endTime>\r\n"\
            // 该时间段下门状态，0-无效，1-休眠，2-常开状态，3-常闭状态
            "<doorStatus>2</doorStatus>\r\n"\
            "</WeekPlanTime>\r\n"\
            "</WeekPlanTimeList>\r\n");
    }
    else if (strcmp(m_csDetail, "SetDoorStatusWeekPlan") == 0)
    {
        g_StringLanType(szLan, "设置门状态周计划使能", "Set door status week plan status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2-ConfigParam4——无效
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<WeekPlanCfg>\r\n<enabled>1</enabled>\r\n</WeekPlanCfg>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardTemplate") == 0)
    {
        g_StringLanType(szLan, "设置卡权限计划模板", "Set card template plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——CardTemplateNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<Template>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<name>JustTest</name>\r\n"\
            "<weekPlanNo>1</weekPlanNo>\r\n"\
            // 假日组，最多16个
            "<HGNo>1,2</HGNo>\r\n"\
            "</Template>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardHolidayGroup") == 0)
    {
        g_StringLanType(szLan, "设置卡权限假日组计划", "Set card holiday group plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayGroupNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<HolidayGroup>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<groupName>JustTest</groupName>\r\n"\
            // 假日列表，最多16个
            "<holidayNo>1,2</holidayNo>\r\n"\
            "</HolidayGroup>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardHolidayPlan") == 0)
    {
        g_StringLanType(szLan, "设置卡权限假日计划", "Set card holiday plan");
        m_csCmdName.Format("%s", szLan);
        //ConfigParam1——HolidayPlanNo（xs:integer编号从1开始）
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<HolidayPlan>\r\n"\
            "<enabled>1</enabled>\r\n"\
            "<startDate>2017-01-01T00:00:00</startDate>\r\n"\
            "<endDate>2017-01-07T00:00:00</endDate>\r\n"\
            // 时间段，最多8个
            "<TimeList>\r\n"\
            "<Time>\r\n"\
            "<startTime>2017-01-01T08:00:00</startTime>\r\n"\
            "<endTime>2017-01-01T12:00:00</endTime>\r\n"\
            "</Time>\r\n"\
            "</TimeList>\r\n"\
            "</HolidayPlan>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardWeekPlanTime") == 0)
    {
        g_StringLanType(szLan, "设置卡权限周计划时间", "Set card week plan time status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2——WeekDay（xs:integer 0-6）ConfigParam3-ConfigParam4——无效
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "1", "0", "0", "<WeekPlanTimeList>\r\n"\
            "<WeekPlanTime>\r\n"\
            "<startTime>2017-01-01T08:00:00</startTime>\r\n"\
            "<endTime>2017-01-01T09:30:00</endTime>\r\n"\
            "</WeekPlanTime>\r\n"\
            "</WeekPlanTimeList>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCardWeekPlan") == 0)
    {
        g_StringLanType(szLan, "设置卡权限周计划使能", "Set card week plan status");
        m_csCmdName.Format("%s", szLan);
        // ConfigParam1——WeekPlanNo（xs:integer编号从1开始）ConfigParam2-ConfigParam4——无效
        m_strInputXml.Format(SETDEVICECONFIG_XML_PATTERN, m_csDetail, "1", "0", "0", "0", "<WeekPlanCfg>\r\n<enabled>1</enabled>\r\n</WeekPlanCfg>\r\n");
    }
    else  if(strcmp(m_csDetail, "SetServerInfoPara") == 0)
    {
        g_StringLanType(szLan, "设置设备基本信息", "set basic info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetServerInfoPara</ConfigCmd>\r\n"\
            "<ConfigXML>\r\n"\
            "<SERVERINFOPARA>\r\n"\
            "<ServerName>asdfghjkasdfghjkasdfghjkasdfghj</ServerName>\r\n"\
            "<ServerID>250</ServerID >\r\n"\
            "<RecycleRecord>1</RecycleRecord >\r\n"\
            "<ServerType>2233</ServerType >\r\n"\
            "<ChannelNum>32</ChannelNum >\r\n"\
            "<HardDiskNum>1</HardDiskNum >\r\n"\
            "<AlarmInNum>17</AlarmInNum >\r\n"\
            "<AlarmOutNum>5</AlarmOutNum >\r\n"\
            "<RS232Num>1</RS232Num >\r\n"\
            "<RS485Num>1</RS485Num >\r\n"\
            "<NetworkPortNum>2</NetworkPortNum >\r\n"\
            "<AuxoutNum>3</AuxoutNum >\r\n"\
            "<AudioNum>2</AudioNum >\r\n"\
            "<SerialNumber>1620160326CCRR586037487WCVU </SerialNumber >\r\n"\
            "<MajorScale>0</MajorScale>\r\n"\
            "<MinorScale>0</MinorScale>\r\n"\
            "</SERVERINFOPARA>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else  if (strcmp(m_csDetail, "SetPicPara") == 0)
    {
        g_StringLanType(szLan, "OSD显示参数", "Set OSD param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetPicPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<CHANPICINFO>\r\n"
            "<ChannelName>Camera 01</ChannelName>\r\n"
            "<IsShowChanName>1</IsShowChanName>\r\n"
            "<ChannelNameXPos>512</ChannelNameXPos>\r\n"
            "<ChannelNameYPos>512</ChannelNameYPos>\r\n"
            "<IsShowOSD>1</IsShowOSD>\r\n"
            "<OSDXPos>0</OSDXPos>\r\n"
            "<OSDYPos>32</OSDYPos>\r\n"
            "<OSDType>2</OSDType>\r\n"
            "<OSDAttrib>4</OSDAttrib>\r\n"
            "<IsShowWeek>1</IsShowWeek>\r\n"
            "</CHANPICINFO>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else  if (strcmp(m_csDetail, "SetShowStringPara") == 0)
    {
        g_StringLanType(szLan, "OSD字符叠加", "Set OSD show string");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetShowStringPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<SHOWSTRINGINFO>\r\n"
            "<STRINGINFO>\r\n"
            "<String/>\r\n"
            "<IsShowString>0</IsShowString>\r\n"
            "<XPos>0</XPos>\r\n"
            "<YPos>0</YPos>\r\n"
            "</STRINGINFO>\r\n"
            "<STRINGINFO>\r\n"
            "<String/>\r\n"
            "<IsShowString>0</IsShowString>\r\n"
            "<XPos>0</XPos>\r\n"
            "<YPos>0</YPos>\r\n"
            "</STRINGINFO>\r\n"
            "<STRINGINFO>\r\n"
            "<String/>\r\n"
            "<IsShowString>0</IsShowString >\r\n"
            "<XPos>0</XPos>\r\n"
            "<YPos>0</YPos>\r\n"
            "</STRINGINFO>\r\n"
            "<STRINGINFO>\r\n"
            "<String/>\r\n"
            "<IsShowString>0 </IsShowString >\r\n"
            "<XPos>0 </XPos>\r\n"
            "<YPos>0 </YPos>\r\n"
            "</STRINGINFO>\r\n"
            "<STRINGINFO>\r\n"
            "<String/>\r\n"
            "<IsShowString>0 </IsShowString >\r\n"
            "<XPos>0</XPos>\r\n"
            "<YPos>0</YPos>\r\n"
            "</STRINGINFO>\r\n"
            "<STRINGINFO>\r\n"
            "<String/>\r\n"
            "<IsShowString>0</IsShowString >\r\n"
            "<XPos>0</XPos>\r\n"
            "<YPos>0</YPos>\r\n"
            "</STRINGINFO>\r\n"
            "<STRINGINFO>\r\n"
            "<String/>\r\n"
            "<IsShowString>0</IsShowString >\r\n"
            "<XPos>0</XPos>\r\n"
            "<YPos>0</YPos>\r\n"
            "</STRINGINFO>\r\n"
            "<STRINGINFO>\r\n"
            "<String/>\r\n"
            "<IsShowString>0 </IsShowString >\r\n"
            "<XPos>0</XPos>\r\n"
            "<YPos>0</YPos>\r\n"
            "</STRINGINFO>\r\n"
            "</SHOWSTRINGINFO>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }

    else if (strcmp(m_csDetail, "SetMotionPara") == 0)
    {
        g_StringLanType(szLan, "移动侦测使能", "Set motion param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetMotionPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<MOTIONCFG>\r\n"
            "<IsUseMotion>0</IsUseMotion>\r\n"
            "<Sensitive>0</Sensitive>\r\n"
            "</MOTIONCFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }


    else if (strcmp(m_csDetail, "SetMotionArea") == 0)
    {
        g_StringLanType(szLan, "移动侦测布防", "Set motion area");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetMotionArea</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<MOTIONAREACFG>\r\n"
            "<Row>18</Row>\r\n"
            "<BlockPerRow>22</BlockPerRow>\r\n"
            "<PictureWidth>0</PictureWidth>\r\n"
            "<PictureHeight>0</PictureHeight>\r\n"
            "<AREAS>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "<Mask>0</Mask>\r\n"
            "</AREAS>\r\n"
            "</MOTIONAREACFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmDeploymentTime") == 0)
    {
        g_StringLanType(szLan, "布防时间", "Set alarm deployment time");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetAlarmDeploymentTime</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigParam3>1</ConfigParam3>\r\n"\
            "<ConfigXML>\r\n"
            "<ALARMTIME>\r\n"
            "<TIME>\r\n"
            "<StartTime>00:00</StartTime>\r\n"
            "<StopTime>24:00</StopTime>\r\n"
            "</TIME>\r\n"
            "<TIME>\r\n"
            "<StartTime>00:00</StartTime>\r\n"
            "<StopTime>00:00</StopTime>\r\n"
            "</TIME>\r\n"
            "<TIME>\r\n"
            "<StartTime>00:00</StartTime>\r\n"
            "<StopTime>00:00</StopTime>\r\n"
            "</TIME>\r\n"
            "<TIME>\r\n"
            "<StartTime>00:00</StartTime>\r\n"
            "<StopTime>00:00</StopTime>\r\n"
            "</TIME>\r\n"
            "</ALARMTIME>\r\n"
            "</ConfigXML>\r\n"
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmTriggerRecord") == 0)
    {
        g_StringLanType(szLan, "通道录像参数", "Set alarm trigger record param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetAlarmTriggerRecord</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigXML>\r\n"
            "<RECORDCHAN>\r\n"
            "<AnalogChan1>1</AnalogChan1>\r\n"
            "<AnalogChan2>1</AnalogChan2>\r\n"
            "<DigitChan1>1</DigitChan1>\r\n"
            "</RECORDCHAN>\r\n"
            "</ConfigXML>\r\n"
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmLinkageType") == 0)
    {
        g_StringLanType(szLan, "联动参数", "Set alarm linkage type");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetAlarmLinkageType</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigXML>\r\n"
            "<LINKAGETYPE>\r\n"
            "<MonitorAlarm>0</MonitorAlarm>\r\n"
            "<SoundAlarm>0</SoundAlarm>\r\n"
            "<Upload>0</Upload>\r\n"
            "<Alarmout>0</Alarmout>\r\n"
            "<Email>0</Email>\r\n"
            "<ALARMOUT>\r\n"
            "<AnalogAlarmOut1>0</AnalogAlarmOut1>\r\n"
            "<AnalogAlarmOut2>0</AnalogAlarmOut2>\r\n"
            "</ALARMOUT>\r\n"
            "</LINKAGETYPE>\r\n"
            "</ConfigXML>\r\n"
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmTriggerCapture") == 0)
    {
        g_StringLanType(szLan, "抓拍参数", "Set alarm trigger param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetAlarmTriggerCapture</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigXML>\r\n"
            "<CAPTURECONFIG>\r\n"
            "<UseCapture>0</UseCapture>\r\n"
            "<SendTo>1</SendTo>\r\n"
            "<Interval>5</Interval>\r\n"
            "<Resolution>1</Resolution>\r\n"
            "<Quality>60</Quality>\r\n"
            "<Frequency>1</Frequency>\r\n"
            "<CAPTURECHAN>\r\n"
            "<AnalogChan1>0</AnalogChan1>\r\n"
            "<AnalogChan2>0</AnalogChan2>\r\n"
            "<AnalogChan3>0</AnalogChan3>\r\n"
            "<AnalogChan4>0</AnalogChan4>\r\n"
            "</CAPTURECHAN>\r\n"
            "</CAPTURECONFIG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetHideAlarmPara") == 0)
    {
        g_StringLanType(szLan, "遮挡使能", "Set hide alarm param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetHideAlarmPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<HIDEALARMCFG>\r\n"
            "<Sensitive>1</Sensitive>\r\n"
            "</HIDEALARMCFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else if (strcmp(m_csDetail, "SetHideAlarmArea") == 0)
    {
        g_StringLanType(szLan, "遮挡区域", "Set hide alarm area");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetHideAlarmArea</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<HIDEALARMAREACFG>\r\n"
            "<AREARECT>\r\n"
            "<AREALEFT>1</AREALEFT>\r\n"
            "<AREATOP>2</AREATOP>\r\n"
            "<AREARIGHT>3</AREARIGHT>\r\n"
            "<AREABOTTOM>4</AREABOTTOM>\r\n"
            "</AREARECT>\r\n"
            "</HIDEALARMAREACFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else if (strcmp(m_csDetail, "SetVILostPara") == 0)
    {
        g_StringLanType(szLan, "视频丢失使能", "Set video lost param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"
            "<ConfigCmd>SetVILostPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"
            "<VILOSTCFG>\r\n"
            "<IsUseVILost>1</IsUseVILost>\r\n"
            "</VILOSTCFG>\r\n"
            "</ConfigXML>\r\n"
            "</Params>");
    }
    else if (strcmp(m_csDetail, "SetAlarmInPara") == 0)
    {
        g_StringLanType(szLan, "报警输入参数", "Alarm In Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetAlarmInPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigXML>\r\n"\
            "<ALARMININFO>\r\n"\
            "<AlarmInName>AlarmTest</AlarmInName>\r\n"\
            "<AlarmInType>0</AlarmInType>\r\n"\
            "<IsUseAlarmIn>1</IsUseAlarmIn>\r\n"\
            "<LINKAGETYPE>\r\n"\
            "<MonitorAlarm>1</MonitorAlarm>\r\n"\
            "<SoundAlarm>1</SoundAlarm>\r\n"\
            "<Upload>1</Upload>\r\n"\
            "<Alarmout>1</Alarmout>\r\n"\
            "<Email>1</Email>\r\n"\
            "<ALARMOUT>\r\n"\
            "<AnalogAlarmOut1>1</AnalogAlarmOut1>\r\n"\
            "<AnalogAlarmOut2>1</AnalogAlarmOut2>\r\n"\
            "</ALARMOUT>\r\n"\
            "<PTZLINKAGE>\r\n"\
            "<IsUsePreset>1</IsUsePreset>\r\n"\
            "<PresetNo>1</PresetNo>\r\n"\
            "<IsUseCurise>1</IsUseCurise>\r\n"\
            "<CuriseNo>1</CuriseNo>\r\n"\
            "<IsUseTrack>1</IsUseTrack>\r\n"\
            "<TrackNo>1</TrackNo>\r\n"\
            "</PTZLINKAGE>\r\n"\
            "</LINKAGETYPE>\r\n"\
            "<RECORDCHAN>\r\n"\
            "<AnalogChan1>1</AnalogChan1>\r\n"\
            "<AnalogChan2>1</AnalogChan2>\r\n"\
            "<DigitChan1>2</DigitChan1>\r\n"\
            "<DigitChan2>2</DigitChan2>\r\n"\
            "</RECORDCHAN>\r\n"\
            "</ALARMININFO>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmOutPara") == 0)
    {
        g_StringLanType(szLan, "报警输出参数", "Alarm Out Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetAlarmOutPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<ALARMOUTINFO>\r\n"\
            "<AlarmOutName>AlarmTest</AlarmOutName>\r\n"\
            "<AlarmOutDelay>0</AlarmOutDelay>\r\n"\
            "</ALARMOUTINFO>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetAlarmOutStatus") == 0)
    {
        g_StringLanType(szLan, "报警输出状态", "Alarm Out Status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetAlarmOutStatus</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<ALARMOUTSTATUSINFO>\r\n"\
            "<AlarmOutStatus>1</AlarmOutStatus>\r\n"\
            "</ALARMOUTSTATUSINFO>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetExceptionPara") == 0)
    {
        g_StringLanType(szLan, "异常报警联动", "Exception Alarm");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetExceptionPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<EXCEPTIONINFO> \r\n"\
            "<LINKAGETYPE>\r\n"\
            "<MonitorAlarm>0</MonitorAlarm>\r\n"\
            "<SoundAlarm>0</SoundAlarm>\r\n"\
            "<Upload>0</Upload> \r\n"\
            "<Alarmout>0</Alarmout>\r\n"\
            "<Email>0</Email>\r\n"\
            "<ALARMOUT>\r\n"\
            "<AnalogAlarmOut1>0</AnalogAlarmOut1>\r\n"\
            "<AnalogAlarmOut2>0</AnalogAlarmOut2>\r\n"\
            "</ALARMOUT>\r\n"\
            "</LINKAGETYPE>\r\n"\
            "</EXCEPTIONINFO>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCapturePicPlanPara") == 0)
    {
        g_StringLanType(szLan, "定时抓图", "Capture Picture");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetCapturePicPlanPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigXML>\r\n"\
            "<CAPTUREPLANCFG>\r\n"\
            "<UseCapture></UseCapture>\r\n"\
            "<SendTo>1</SendTo>\r\n"\
            "<CaptureType>2</CaptureType>\r\n"\
            "<Interval>30</Interval>\r\n"\
            "<Resolution>1</Resolution>\r\n"\
            "<Quality>1</Quality>\r\n"\
            "<Frequency>1</Frequency>\r\n"\
            "<CAPTUREPLAN>\r\n"\
            "<TimeZoneIdx>0</TimeZoneIdx>\r\n"\
            "<TimePoint>HH:MI HH:MI HH:MI HH:MI HH:MI HH:MI HH:MI HH:MI</TimePoint>\r\n"\
            "<TIME>\r\n"\
            "<StartTime>HH:MI</StartTime>\r\n"\
            "<StopTime>HH:MI</StopTime>\r\n"\
            "</TIME>\r\n"\
            "</CAPTUREPLAN>\r\n"\
            "</CAPTUREPLANCFG>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetRS485Para") == 0)
    {
        g_StringLanType(szLan, "RS485 配置", "RS485 Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetRS485Para</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<RS485INFO>\r\n"\
            "<BaudRate>0</BaudRate>\r\n"\
            "<DataBit>0</DataBit>\r\n"\
            "<StopBit>0</StopBit>\r\n"\
            "<Parity>0</Parity>\r\n"\
            "<FlowControl>0</FlowControl>\r\n"\
            "<DecoderType>1</DecoderType>\r\n"\
            "<DecoderAddr>0</DecoderAddr>\r\n"\
            "</RS485INFO>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetRS232Para") == 0)
    {
        g_StringLanType(szLan, "RS232 配置", "RS232 Param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetRS232Para</ConfigCmd>\r\n"\
            "<ConfigXML>\r\n"\
            "<RS232INFO>\r\n"\
            "<BaudRate>0</BaudRate>\r\n"\
            "<DataBit>0</DataBit> \r\n"\
            "<StopBit>0</StopBit>\r\n"\
            "<Parity>0</Parity>	\r\n"\
            "<FlowControl>0</FlowControl>\r\n"\
            "<WorkMode>0</WorkMode>	\r\n"\
            "</RS232INFO>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetRecordPlanPara") == 0)
    {
        g_StringLanType(szLan, "录像计划", "Record Plan");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetRecordPlanPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigXML>\r\n"\
            "<RECORDTIMECFG>\r\n"\
            "<IsUseRecord>1</IsUseRecord>\r\n"\
            "<RecordTime>1</RecordTime>\r\n"\
            "<PreRecordTime>1</PreRecordTime>\r\n"\
            "<RecorderDuration>1</RecorderDuration>\r\n"\
            "<RedundancyRec>1</RedundancyRec>\r\n"\
            "<AudioRec>1</AudioRec>\r\n"\
            "<RECORDPLAY>\r\n"\
            "<IsAllDayRecord>1</IsAllDayRecord>\r\n"\
            "<AllDayRecordType>1</AllDayRecordType>\r\n"\
            "<TimeZoneIdx>0</TimeZoneIdx>\r\n"\
            "<TIME>\r\n"\
            "<RecordType>1</RecordType>\r\n"\
            "<StartTime>HH:MI</StartTime>\r\n"\
            "<StopTime>HH:MI</StopTime>	\r\n"\
            "</TIME>\r\n"\
            "</RECORDPLAY>\r\n"\
            "</RECORDTIMECFG>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCMSInfoPara") == 0)
    {
        g_StringLanType(szLan, "设备注册信息", "CMS Info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetCMSInfoPara</ConfigCmd>\r\n"\
            "<ConfigXML>\r\n"\
            "<CMSINFOPARA>\r\n"\
            "<CMSIP>172.9.14.251</CMSIP>\r\n"\
            "<CMSPort>7660</CMSPort>\r\n"\
            "<DeviceID>test123</DeviceID>\r\n"\
            "</CMSINFOPARA>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetGPSInfoPara") == 0)
    {
        g_StringLanType(szLan, "GPS 信息", "GPS Info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetGPSInfoPara</ConfigCmd>\r\n"\
            "<ConfigXML>\r\n"\
            "<GPSINFOPARA>\r\n"\
            "<GPSTransInterval>10</GPSTransInterval>\r\n"\
            "<MaxSpeed>10</MaxSpeed>\r\n"\
            "<MinSpeed>0</MinSpeed>\r\n"\
            "<Enable>1</Enable>\r\n"\
            "</GPSINFOPARA>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetZeroChannelInfo") == 0)
    {
        g_StringLanType(szLan, "零通道配置", "Zero Channel");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetZeroChannelInfo</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<ZEROCHANNELINFO>\r\n"\
            "<IsEnable>1</IsEnable>\r\n"\
            "<VideoBitRate>1</VideoBitRate>\r\n"\
            "<MaxBitRate>1</MaxBitRate>\r\n"\
            "<VideoFrameRate>1</VideoFrameRate>\r\n"\
            "</ZEROCHANNELINFO>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetFormatDisk") == 0)
    {
        g_StringLanType(szLan, "硬盘格式化", "Disk Format");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetFormatDisk</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetPreviewPara") == 0)
    {
        g_StringLanType(szLan, "预览循序配置", "Preview Order");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetPreviewPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<PREVIEWINFO>\r\n"\
            "<PrevMode>1</PrevMode>\r\n"\
            "<EnableAudio>1</EnableAudio>\r\n"\
            "<SwitchInter>1</SwitchInter>\r\n"\
            "<PREVORDER>\r\n"\
            "<Order>01-02-03-04-ff-ff-ff-...-ff</Order>\r\n"\
            "</PREVORDER>\r\n"\
            "</PREVIEWINFO>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetPassengerPara") == 0)
    {
        g_StringLanType(szLan, "客流量使能参数", "Passenger enable cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetPassengerPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<PASSENGERCFG>\r\n"\
            "<Enable>0</Enable>\r\n"\
            "</PASSENGERCFG>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetPassengerOSDPara") == 0)
    {
        g_StringLanType(szLan, "客流量OSD参数", "Passenger OSD cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetPassengerOSDPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<PASSENGEROSDCFG>\r\n"\
            "<Enable>1</Enable>\r\n"\
            "<OSDPos>0,0</OSDPos>\r\n"\
            "</PASSENGEROSDCFG>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetPassengerRegionPara") == 0)
    {
        g_StringLanType(szLan, "客流量区域参数", "Passenger region cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetPassengerRegionPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<PASSENGERREGIONCFG>\r\n"\
            "<Sensitivity>1</Sensitivity>\r\n"\
            "<SpaceSpeed>1</SpaceSpeed>\r\n"\
            "<TimeSpeed>1</TimeSpeed>\r\n"\
            "<CountSpeed>1</CountSpeed>\r\n"\
            "<DetectType>1</DetectType>\r\n"\
            "<SizeCorrect>1</SizeCorrect>\r\n"\
            "<PointNum>3</PointNum>\r\n"\
            "<RegionCoordinatesList>200,200 800,200 500,500</RegionCoordinatesList>\r\n"\
            "<Direction>500,200 500,500</Direction>\r\n"\
            "</PASSENGERREGIONCFG>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetPassengerLinePara") == 0)
    {
        g_StringLanType(szLan, "客流量划线参数", "Passenger line cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetPassengerLinePara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigParam2>1</ConfigParam2>\r\n"\
            "<ConfigXML>\r\n"\
            "<PASSENGERLINECFG>\r\n"\
            "<Sensitivity>1</Sensitivity>\r\n"\
            "<SpaceSpeed>1</SpaceSpeed>\r\n"\
            "<TimeSpeed>1</TimeSpeed>\r\n"\
            "<CountSpeed>1</CountSpeed>\r\n"\
            "<DetectType>1</DetectType>\r\n"\
            "<SizeCorrect>1</SizeCorrect>\r\n"\
            "<LineCoordinatesList>300,400 700,400</LineCoordinatesList>\r\n"\
            "<Direction>500,200 500,500</Direction>\r\n"\
            "</PASSENGERLINECFG>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetPassengerSetupPara") == 0)
    {
        g_StringLanType(szLan, "客流量安装配置参数", "Passenger setup cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetPassengerSetupPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<PASSENGERSETUPCFG>\r\n"\
            "<SetupHeight>500</SetupHeight>\r\n"\
            "<ViewAngle>0</ViewAngle>\r\n"\
            "<HorDistance>500</HorDistance>\r\n"\
            "<FocalLength>50</FocalLength>\r\n"\
            "</PASSENGERSETUPCFG>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetCalibrationPara") == 0)
    {
        g_StringLanType(szLan, "客流量标定参数", "Passenger calibration cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetCalibrationPara</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<CALIBRATIONCFG>\r\n"\
            "<Enable>1</Enable>\r\n"\
            "<DemarcationLineList>\r\n"\
            "<DemarcationLine>300,300 600,600</DemarcationLine>\r\n"\
            "</DemarcationLineList>\r\n"\
            "</CALIBRATIONCFG>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetVCACtrlCfg") == 0)
    {
        g_StringLanType(szLan, "智能控制信息参数", "VCA control cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetVCACtrlCfg</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<VCACtrlCfg>\r\n"\
            "<VCAEnable>1</VCAEnable>\r\n"\
            "<VCAType>1</VCAType>\r\n"\
            "<streamWithVCA>1</streamWithVCA>\r\n"\
            "<mode>0</mode>\r\n"\
            "<controlType>1</controlType>\r\n"\
            "</VCACtrlCfg>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetVCAFaceSnapCfg") == 0)
    {
        g_StringLanType(szLan, "人脸抓拍参数", "VCA face snap cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetVCAFaceSnapCfg</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<VCAFaceSnapCfg>\r\n"\
            "<snapTime>1</snapTime>\r\n"\
            "<snapInterval>25</snapInterval>\r\n"\
            "<snapThreshold>1</snapThreshold>\r\n"\
            "<generateRate>1</generateRate>\r\n"\
            "<sensitive>1</sensitive>\r\n"\
            "<referenceBright>1</referenceBright>\r\n"\
            "<matchType>1</matchType>\r\n"\
            "<matchThreshold>50</matchThreshold>\r\n"\
            "<JPEGPara>\r\n"\
            "<picSize>255</picSize>\r\n"\
            "<picQuality>1</picQuality>\r\n"\
            "</JPEGPara>\r\n"\
            "<faceExposureMinDuratio>60</faceExposureMinDuratio>\r\n"\
            "<faceExposureMode>1</faceExposureMode>\r\n"\
            "<backgroundPic>1</backgroundPic>\r\n"\
            "</VCAFaceSnapCfg>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetVCAFaceSnapRuleCfg") == 0)
    {
        g_StringLanType(szLan, "人脸抓拍规则配置", "VCA face snap rule cfg");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetVCAFaceSnapRuleCfg</ConfigCmd>\r\n"\
            "<ConfigParam1>1</ConfigParam1>\r\n"\
            "<ConfigXML>\r\n"\
            "<VCAFaceSnapRuleCfg>\r\n"\
            "<VCASingleFaceSnapCfg>\r\n"\
            "<VCASingleFaceSnapCfg>\r\n"\
            "<active>1</active>\r\n"\
            "<autoROIEnable>1</autoROIEnable>\r\n"\
            "<VCASizeFilter>\r\n"\
            "<active>0</active>\r\n"\
            "<mode>0</mode>\r\n"\
            "<miniRect>0,123,200,200</miniRect>\r\n"\
            "<maxRect>0,123,200,200</maxRect>\r\n"\
            "</VCASizeFilter>\r\n"\
            "<VcaPolygon>\r\n"\
            "<pointNum>0</pointNum>\r\n"\
            "<pos>123,321</pos>\r\n"\
            "</VcaPolygon>\r\n"\
            "</VCASingleFaceSnapCfg>\r\n"\
            "</VCASingleFaceSnapCfg>\r\n"\
            "</VCAFaceSnapRuleCfg>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetDialMode") == 0)
    {
        g_StringLanType(szLan, "设置设备拨号制式", "Set dial mode");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetDialMode</ConfigCmd>\r\n"\
            "<ConfigParam1>0</ConfigParam1>\r\n"\
            "<ConfigParam2>0</ConfigParam2>\r\n"\
            "<ConfigParam3>0</ConfigParam3>\r\n"\
            "<ConfigParam4>0</ConfigParam4>\r\n"\
            "<ConfigXML>\r\n"\
            "<ModeInfo>\r\n"\
            "<Mode>2</Mode>\r\n"\
            "</ModeInfo>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }

    //控制参数
    if (strcmp(m_csDetail, "PTZCONTROL") == 0)
    {
        g_StringLanType(szLan, "云镜控制", "PTZ control");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>8757</Sequence >\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>PTZCONTROL</Command>\r\n"\
            "<Params>\r\n"\
            "<Channel>1</Channel >\r\n"\
            "<PTZCmd>UP</PTZCmd>\r\n"\
            "<Action>Stop</Action>\r\n"\
            "<Speed>0</Speed >\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>\r\n");
    }
    else if (strcmp(m_csDetail, "MANUALSNAP") == 0)
    {
        g_StringLanType(szLan, "手动抓图", "manual snap");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>11561</Sequence >\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>MANUALSNAP</Command>\r\n"\
            "<Params>\r\n"\
            "<Chan>1</Chan>\r\n"\
            "<SnapType>0</SnapType>\r\n"\
            "<SendTo>1</SendTo >\r\n"\
            "<Resolution>6</Resolution>\r\n"\
            "<Frequency>1</Frequency>\r\n"\
            "<Quality>60 </Quality>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "GETDEVICEWORKSTATUS") == 0)
    {
        g_StringLanType(szLan, "获取设备工作状态", "get device work status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>QUERY</Method>\r\n"\
            "<Command>GETDEVICEWORKSTATUS</Command>\r\n"\
            "<Params>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "PTZPRESETCONTROL") == 0)
    {
        g_StringLanType(szLan, "预置点操作", "set ptz preset");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>PTZPRESETCONTROL</Command>\r\n"\
            "<Params>\r\n"\
            "<Channel>1 </Channel >\r\n"\
            "<PTZPresetCmd>SET_PRESET</PTZPresetCmd>\r\n"\
            "<PresetIndex>1 </PresetIndex >\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "PTRACK") == 0)
    {
        g_StringLanType(szLan, "3D点击居中", "placed in center");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>PTRACK</Command>\r\n"\
            "<Params>\r\n"\
            "<Channel>1</Channel>\r\n"\
            "<AreaX>150</AreaX>\r\n"\
            "<AreaY>114</AreaY>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "PZIN") == 0)
    {
        g_StringLanType(szLan, "3D框选放大缩小", "Zoom in /Zoom out");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>PZIN</Command>\r\n"\
            "<Params>\r\n"\
            "<Channel>1</Channel>\r\n"\
            "<PTZCmd>PZIN</PTZCmd>\r\n"\
            "<Action>0</Action>\r\n"\
            "<AreaX>150</AreaX>\r\n"\
            "<AreaY>114</AreaY>\r\n"\
            "<AreaWidth>227</AreaWidth>\r\n"\
            "<AreaHeight>183</AreaHeight>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "GETIMAGEPARAM") == 0)
    {
        g_StringLanType(szLan, "获取图像质量参数", "get image param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>PARAM</Method>\r\n"\
            "<Command>GETIMAGEPARAM</Command>\r\n"\
            "<Params>\r\n"\
            "<Channel>1</Channel>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "SETIMAGEPARAM") == 0)
    {
        g_StringLanType(szLan, "设置图像质量参数", "set image param");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>PARAM</Method>\r\n"\
            "<Command>SETIMAGEPARAM</Command>\r\n"\
            "<Params>\r\n"\
            "<Channel>1</Channel>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "SETDEFENSESTATUS") == 0)
    {
        g_StringLanType(szLan, "远程布撤防", "");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>SETDEFENSESTATUS</Command>\r\n"\
            "<Params>\r\n"\
            "<IsEnable>1</IsEnable>\r\n"\
            "<ArmDelayTime>60</ArmDelayTime>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "ALARMHOSTCTRL") == 0)
    {
        g_StringLanType(szLan, "远程控制报警主机", "alarmhost control");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>ALARMHOSTCTRL</Command>\r\n"\
            "<Params>\r\n"\
            "<CtrlType>3</CtrlType>\r\n"\
            "<AlarmNo>000000000000000000000000000000</AlarmNo>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "ADJUSTTIME") == 0)
    {
        g_StringLanType(szLan, "校时", "adjust time");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>ADJUSTTIME</Command>\r\n"\
            "<Params>\r\n"\
            "<TimeZone>0</TimeZone>\r\n"\
            "<Time>2016-08-28 08:00:00</Time>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "RECSTART") == 0)
    {
        g_StringLanType(szLan, "手动开启录像", "start  record");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>RECSTART</Command>\r\n"\
            "<Params>\r\n"\
            "<ChannelIndex>1</ChannelIndex>\r\n"\
            "<RecDelayTime>60</RecDelayTime>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "RECSTOP") == 0)
    {
        g_StringLanType(szLan, "手动停止录像", "stop  record");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>RECSTOP</Command>\r\n"\
            "<Params>\r\n"\
            "<ChannelIndex>1</ChannelIndex>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "MANUALAIO") == 0)
    {
        g_StringLanType(szLan, "触发设备IO输出", "trigger");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>MANUALAIO</Command>\r\n"\
            "<Params>\r\n"\
            "<Action>START</Action>\r\n"\
            "<Chan>1</Chan>\r\n"\
            "<DelayTime>30</DelayTime>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "UPLOADPIC") == 0)
    {
        g_StringLanType(szLan, "下载图片文件", "download picture");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>UPLOADPIC</Command>\r\n"\
            "<Params>\r\n"\
            "<Chan>1</Chan>\r\n"\
            "<Filename>ch01_00000251000300</Filename>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "ISSUEINFORMATION") == 0)
    {
        g_StringLanType(szLan, "下发消息", "upload display info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>ISSUEINFORMATION</Command>\r\n"\
            "<Params>\r\n"\
            "<InfoID>this_is_msg_ID</InfoID>\r\n"\
            "<InfoLevel>1</InfoLevel>\r\n"\
            "<NeedConfirm>0</NeedConfirm>\r\n"\
            "<Info>This Info is UTF-8 encoded</Info>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "RESETPDCOSD") == 0)
    {
        g_StringLanType(szLan, "客流量清零", "reset PDC");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>RESETPDCOSD</Command>\r\n"\
            "<Params>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "RESTARTIVSLIB") == 0)
    {
        g_StringLanType(szLan, "重启算法库", "restart IVS lib");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>RESTARTIVSLIB</Command>\r\n"\
            "<Params>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "SETCLOUDSERVERINFO") == 0)
    {
        g_StringLanType(szLan, "云存储服务器参数", "Cloud server info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>SETCLOUDSERVERINFO</Command>\r\n"\
            "<Params>\r\n"\
            "<CloudServer>172.0.0.1</CloudServer>\r\n"\
            "<CloudLoginPort>6001</CloudLoginPort>\r\n"\
            "<CloudManageHttpPort>8009</CloudManageHttpPort>\r\n"\
            "<CloudTransDataPort>9001</CloudTransDataPort>\r\n"\
            "<CloudCmdPort>8008</CloudCmdPort>\r\n"\
            "<CloudHeartBeatPort>9999</CloudHeartBeatPort>\r\n"\
            "<CloudStorageHttpPort>8083</CloudStorageHttpPort>\r\n"\
            "<CloudUsername>username</CloudUsername>\r\n"\
            "<CloudPassword>password</CloudPassword>\r\n"\
            "<CloudPoolId>1</CloudPoolId>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "SETBAYONETPOLICEINFO") == 0)
    {
        g_StringLanType(szLan, "卡口服务器参数", "Bayonet police info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>SETBAYONETPOLICEINFO</Command>\r\n"\
            "<Params>\r\n"\
            "<BayonetPoliceServerIp>172.0.0.1</BayonetPoliceServerIp>\r\n"\
            "<BayonetPoliceServerPort>7661</BayonetPoliceServerPort>\r\n"\
            "<PassPlateUploadType>1</PassPlateUploadType>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "SETENFORCESERVERINFO") == 0)
    {
        g_StringLanType(szLan, "取证服务器参数", "Enforce server info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>4.0</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<Command>SETENFORCESERVERINFO</Command>\r\n"\
            "<Params>\r\n"\
            "<EnforceDataServerIp>172.0.0.1</EnforceDataServerIp>\r\n"\
            "<EnforceDataServerPort>8888</EnforceDataServerPort>\r\n"\
            "<EnforceDataUploadType>1</EnforceDataUploadType>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "GATECONTROL") == 0)
    {
        g_StringLanType(szLan, "远程控门", "Remote gate control");
        m_csCmdName.Format("%s", szLan);
        // Cmd:OPEN,CLOSE,ALWAYSOPEN,ALWAYSCLOSE
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>2.5</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Command>GATECONTROL</Command>\r\n"\
            "<Params>\r\n"\
            "<DoorNo>1</DoorNo>\r\n"\
            "<Cmd>OPEN</Cmd>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "CLEARACSPARAM") == 0)
    {
        g_StringLanType(szLan, "清空参数", "Clear parameters");
        m_csCmdName.Format("%s", szLan);
        // paramType:1-19
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>2.5</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<Method>CONTROL</Method>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Command>CLEARACSPARAM</Command>\r\n"\
            "<Params>\r\n"\
            "<paramType>16</paramType>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "QUERYDEVICELOG") == 0)
    {
        g_StringLanType(szLan, "查询设备日志", "Search device log");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>2.5</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<Method>QUERY</Method>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Command>QUERYDEVICELOG</Command>\r\n"\
            "<Params>\r\n"\
            "<MajorType>1</MajorType>\r\n"\
            "<MinorType>0x76</MinorType>\r\n"\
            "<StartTime>2017-01-01 10:49:54</StartTime>\r\n"\
            "<StopTime>2017-10-01 10:49:54</StopTime>\r\n"\
            "<ChunkSize>10</ChunkSize>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    else if (strcmp(m_csDetail, "QUERYALARMHOSTSTATUS") == 0)
    {
        g_StringLanType(szLan, "查询报警主机状态", "Query AlarmHost status");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n"\
            "<PPVSPMessage>\r\n"\
            "<Version>2.5</Version>\r\n"\
            "<Sequence>1</Sequence>\r\n"\
            "<Method>QUERY</Method>\r\n"\
            "<CommandType>REQUEST</CommandType>\r\n"\
            "<Command>QUERYALARMHOSTSTATUS</Command>\r\n"\
            "<Params>\r\n"\
            "<AlarmType>1</AlarmType>\r\n"\
            "</Params>\r\n"\
            "</PPVSPMessage>");
    }
    if (strcmp(m_csDetail, "GetWirelessInfoPara") == 0)
    {
        g_StringLanType(szLan, "无线网络信息", "Wireless Info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>GetWirelessInfoPara</ConfigCmd>\r\n"\
            "</Params>\r\n");
    }
    else if (strcmp(m_csDetail, "SetWirelessInfoPara") == 0)
    {
        g_StringLanType(szLan, "无线网络信息", "Wireless Info");
        m_csCmdName.Format("%s", szLan);
        m_strInputXml.Format("<Params>\r\n"\
            "<ConfigCmd>SetWirelessInfoPara</ConfigCmd>\r\n"\
            "<ConfigXML>\r\n"\
            "<WIRELESSINFOPARA>\r\n"\
            "<Enable>1</Enable>\r\n"\
            "<InfoTransInterval>10</InfoTransInterval>\r\n"\
            "</WIRELESSINFOPARA>\r\n"\
            "</ConfigXML>\r\n"\
            "</Params>\r\n");
    }
    UpdateData(FALSE);
}


void CSubDlgXmlCfg::InitCmdType()
{
    m_cmbType.AddString("Basic");
    m_cmbType.AddString("Video OSD");
    m_cmbType.AddString("Move Detection");
    m_cmbType.AddString("Hide Alarm");
    m_cmbType.AddString("Video Lost");
    m_cmbType.AddString("Hide Private");
    m_cmbType.AddString("Video Status");
    m_cmbType.AddString("Infrared Wifi Call");
    m_cmbType.AddString("Alarm");
    m_cmbType.AddString("CapturePic");
    m_cmbType.AddString("RS");
    m_cmbType.AddString("Record");
    m_cmbType.AddString("CMSInfo");
    m_cmbType.AddString("GPS");
    m_cmbType.AddString("Zero Chan");
    m_cmbType.AddString("Format Disk");    
    m_cmbType.AddString("Preview");
    m_cmbType.AddString("Ability");
    m_cmbType.AddString("Passenger Parameter");
    m_cmbType.AddString("VCA");
    m_cmbType.AddString("Record Status");
    m_cmbType.AddString("Net Status");
    m_cmbType.AddString("Dial Mode");
    m_cmbType.AddString("ACS");
    m_cmbType.AddString("Operate Code");
    m_cmbType.AddString("Wireless Info");
    m_cmbType.SetCurSel(0);
}

void CSubDlgXmlCfg::OnSelchangeComboType()
{
    // TODO:  在此添加控件通知处理程序代码

    m_cmbCmd.ResetContent();

    UpdateData(TRUE);
    if (strcmp(m_csType, "Basic") == 0)
    {
        m_enumType = BASIC;
     
    }
    else if (strcmp(m_csType, "Video OSD") == 0)
    {
        m_enumType = VIDEO_OSD;
    }
    else if (strcmp(m_csType, "Move Detection") == 0)
    {       
        m_enumType = MOVE_DETECTION;
    }
    else if (strcmp(m_csType, "Hide Alarm") == 0)
    {
        m_enumType = HIDE_ALARM;
    }
    else if (strcmp(m_csType, "Video Lost") == 0)
    {
        m_enumType = VIDEO_LOST;
    }
    else if (strcmp(m_csType, "Hide Private") == 0)
    {
        m_enumType = HIDE_PRIVATE;
    }
    else if (strcmp(m_csType, "Video Status") == 0)
    {
        m_enumType = VIDEO_STATUS;
    }
    else if (strcmp(m_csType, "Infrared Wifi Call") == 0)
    {
        m_enumType = INFRARED_WIFI_CALL;
    }
    else if (strcmp(m_csType, "Alarm") == 0)
    {
        m_enumType = Alarm;
    }
    else if (strcmp(m_csType, "CapturePic") == 0)
    {
        m_enumType = CAPTUREPIC;
    }
    else if (strcmp(m_csType, "RS") == 0)
    {
        m_enumType = RS;
    }
    else if (strcmp(m_csType, "Record") == 0)
    {
        m_enumType = RECORD;
    }
    else if (strcmp(m_csType, "CMSInfo") == 0)
    {
        m_enumType = CMSINFO;
    }
    else if (strcmp(m_csType, "GPS") == 0)
    {
        m_enumType = GPS;
    }
    else if (strcmp(m_csType, "Zero Chan") == 0)
    {
        m_enumType = ZERO_CHAN;
    }
    else if (strcmp(m_csType, "Format Disk") == 0)
    {
        m_enumType = DISK_FORMAT;
    }
    else if (strcmp(m_csType, "Preview") == 0)
    {
        m_enumType = PREVIEW;
    }
    else if (strcmp(m_csType, "Ability") == 0)
    {
        m_enumType = ABILITY;
    }
    else if (strcmp(m_csType, "Passenger Parameter") == 0)
    {
        m_enumType = PASSENGER_PARAMETER;
    }
    else if (strcmp(m_csType, "VCA") == 0)
    {
        m_enumType = VCA;
    }
    else if (strcmp(m_csType, "Record Status") == 0)
    {
        m_enumType = RECORD_STATUS;
    }
    else if (strcmp(m_csType, "Net Status") == 0)
    {
        m_enumType = NET_STATUS;
    }
    else if (strcmp(m_csType, "Dial Mode") == 0)
    {
        m_enumType = DIAL_MODE;
    } 
    else if (strcmp(m_csType, "ACS") == 0)
    {
        m_enumType = ACS;
    }
    else if (strcmp(m_csType, "Operate Code") == 0)
    {
        m_enumType = OPERATOR_CODE;
    }
    else if (strcmp(m_csType, "Wireless Info") == 0)
    {
        m_enumType = WIRELESS_INFO;
    }

    m_cmbCmd.AddString("GETDEVICECONFIG");
    m_cmbCmd.AddString("SETDEVICECONFIG");
    m_cmbCmd.AddString("CONTROL");
    m_cmbCmd.SetCurSel(0);
    InitGetParam(m_enumType);

    UpdateData(FALSE);
}


void CSubDlgXmlCfg::OnBnClickedBtnShot()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    NET_EHOME_SCREEN_SHOT struShot = { 0 };
    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
    struSendData.dwSize = sizeof(struSendData);
    struSendData.lpInBuffer = &struShot;
    struSendData.dwInBufferSize = sizeof(struShot);
    struShot.dwServerPort = 7661;
    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
    struRecvData.dwSize = sizeof(struRecvData);
    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_GET_SCREENSHOT, &struSendData, &struRecvData))
    {
        DWORD dwErr = NET_ECMS_GetLastError();
        AfxMessageBox("NET_EHOME_S2C_GET_SCREENSHOT failed, err[%d]", dwErr);
    }
    else
    {
        AfxMessageBox("NET_EHOME_S2C_GET_SCREENSHOT success");
    }
}


void CSubDlgXmlCfg::OnBnClickedBtnRelease()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    NET_EHOME_POST_SCHEDULE struRelease = { 0 };
    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
    struSendData.dwSize = sizeof(struSendData);
    struSendData.lpInBuffer = &struRelease;
    struSendData.dwInBufferSize = sizeof(struRelease);
    strcpy((char*)struRelease.byServerIP, m_strIP);
    struRelease.dwServerPort = 7661;
    struRelease.dwScheduleID = m_dwCheduleID;
    struRelease.dwScheduleSeq = m_dwSeq;
    struRelease.dwTimingSchedualID = m_dwTimeID;
    memcpy(struRelease.byEffectiveTime, m_strPlayTime, strlen(m_strPlayTime));
    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
    struRecvData.dwSize = sizeof(struRecvData);
    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_POST_SCHEDULE, &struSendData, &struRecvData))
    {
        DWORD dwErr = NET_ECMS_GetLastError();
        AfxMessageBox("NET_EHOME_S2C_POST_SCHEDULE failed, err[%d]", dwErr);
    }
    else
    {
        AfxMessageBox("NET_EHOME_S2C_POST_SCHEDULE success");
    }
}


void CSubDlgXmlCfg::OnBnClickedBtnMaterial()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    NET_EHOME_REPLACE_MATERIAL struMaterial = { 0 };
    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
    struSendData.dwSize = sizeof(struSendData);
    struSendData.lpInBuffer = &struMaterial;
    struSendData.dwInBufferSize = sizeof(struMaterial);
    strcpy((char*)struMaterial.szServerIP, m_strIP);
    struMaterial.dwServerPort = 7661;
    struMaterial.dwMaterialId = m_dwCheduleID;
    struMaterial.dwMaterialSeq = m_dwSeq;
    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
    struRecvData.dwSize = sizeof(struRecvData);
    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_REPLACE_MATERIAL, &struSendData, &struRecvData))
    {
        DWORD dwErr = NET_ECMS_GetLastError();
        AfxMessageBox("NET_EHOME_S2C_REPLACE_MATERIAL failed, err[%d]", dwErr);
    }
    else
    {
        AfxMessageBox("NET_EHOME_S2C_REPLACE_MATERIAL success");
    }
}


void CSubDlgXmlCfg::OnBnClickedBtnInsert()
{
    // TODO:  在此添加控件通知处理程序代码
    NET_EHOME_TERMINAL_CONTROL struCtrl = { 0 };
    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
    struSendData.dwSize = sizeof(struSendData);
    struSendData.lpInBuffer = &struCtrl;
    struSendData.dwInBufferSize = sizeof(struCtrl);
    struCtrl.dwControlType = NET_EHOME_CONTROL_INSERT;
    struCtrl.struInsertInfo.dwInsertType = NET_EHOME_INSERT_TYPE_PROGRAM;
    struCtrl.dwConnPort = 7661;
    struCtrl.struInsertInfo.struProgramInfo.dwTextNo = m_dwCheduleID;
    struCtrl.struInsertInfo.struProgramInfo.dwTextSeq = m_dwSeq;
    memcpy(struCtrl.struInsertInfo.struProgramInfo.szPlayMode, "byTime", strlen("byTime"));
    struCtrl.struInsertInfo.struProgramInfo.dwPlayDuration = 120;

    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
    struRecvData.dwSize = sizeof(struRecvData);
    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_PLAYCONTROL, &struSendData, &struRecvData))
    {
        DWORD dwErr = NET_ECMS_GetLastError();
        AfxMessageBox("NET_EHOME_S2C_PLAYCONTROL failed insert, err[%d]", dwErr);
    }
    else
    {
        AfxMessageBox("NET_EHOME_S2C_PLAYCONTROL success insert");
    }
}


void CSubDlgXmlCfg::OnBnClickedBtnUpgrate()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    NET_EHOME_UPGRADE_CONN_PARAM struUpgrate = { 0 };
    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
    struSendData.dwSize = sizeof(struSendData);
    struSendData.lpInBuffer = &struUpgrate;
    struSendData.dwInBufferSize = sizeof(struUpgrate);
    strcpy((char*)struUpgrate.szServerIP, m_strIP);
    struUpgrate.dwServerPort = 7661;
    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
    struRecvData.dwSize = sizeof(struRecvData);
    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_VERSIONUPGRADE, &struSendData, &struRecvData))
    {
        DWORD dwErr = NET_ECMS_GetLastError();
        AfxMessageBox("NET_EHOME_S2C_VERSIONUPGRADE failed, err[%d]", dwErr);
    }
    else
    {
        AfxMessageBox("NET_EHOME_S2C_VERSIONUPGRADE success");
    }
}


void CSubDlgXmlCfg::OnBnClickedBtnInsertStop()
{
    // TODO:  在此添加控件通知处理程序代码
    NET_EHOME_TERMINAL_CONTROL struCtrl = { 0 };
    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
    struSendData.dwSize = sizeof(struSendData);
    struSendData.lpInBuffer = &struCtrl;
    struSendData.dwInBufferSize = sizeof(struCtrl);
    struCtrl.dwControlType = NET_EHOME_CONTROL_STOPINSERT;
    struCtrl.struInsertInfo.dwInsertType = NET_EHOME_INSERT_TYPE_PROGRAM;
    struCtrl.struInsertInfo.struProgramInfo.dwTextNo = 1;
    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
    struRecvData.dwSize = sizeof(struRecvData);
    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_PLAYCONTROL, &struSendData, &struRecvData))
    {
        DWORD dwErr = NET_ECMS_GetLastError();
        char szTemp[128] = { 0 };
        sprintf(szTemp, "NET_EHOME_S2C_TERMINALCONTROL failed stop, err[%d]", dwErr);
        AfxMessageBox(szTemp);
    }
    else
    {
        AfxMessageBox("NET_EHOME_S2C_TERMINALCONTROL success stop");
    }
    //停止播放
    memset(&struCtrl, 0, sizeof(struCtrl));
    memset(&struSendData, 0, sizeof(struSendData));
    struSendData.dwSize = sizeof(struSendData);
    struSendData.lpInBuffer = &struCtrl;
    struSendData.dwInBufferSize = sizeof(struCtrl);
    struCtrl.dwControlType = NET_EHOME_CONTROL_STOPPLAY;
    struCtrl.struInsertInfo.dwInsertType = NET_EHOME_INSERT_TYPE_PROGRAM;
    struCtrl.struInsertInfo.struProgramInfo.dwTextNo = 1;

    memset(&struRecvData, 0, sizeof(struRecvData));
    struRecvData.dwSize = sizeof(struRecvData);
    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_PLAYCONTROL, &struSendData, &struRecvData))
    {
        DWORD dwErr = NET_ECMS_GetLastError();
        char szTemp[128] = { 0 };
        sprintf(szTemp, "NET_EHOME_CONTROL_STOPPLAY failed stop, err[%d]", dwErr);
        AfxMessageBox(szTemp);
    }
    else
    {
        AfxMessageBox("NET_EHOME_CONTROL_STOPPLAY success");
    }



    //取消日程发布

    //NET_EHOME_POST_SCHEDULE struRelease = { 0 };
    //memset(&struSendData, 0, sizeof(struSendData));
    //struSendData.dwSize = sizeof(struSendData);
    //struSendData.lpInBuffer = &struRelease;
    //struSendData.dwInBufferSize = sizeof(struRelease);
    //strcpy((char*)struRelease.byServerIP, m_strIP);
    //struRelease.dwServerPort = 7661;
    //struRelease.dwScheduleID = 1;
    //
    //memset(&struRecvData, 0, sizeof(struRecvData));
    //struRecvData.dwSize = sizeof(struRecvData);
    //if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_CANCEL_SCHEDULE, &struSendData, &struRecvData))
    //{
    //    DWORD dwErr = NET_ECMS_GetLastError();
    //    char szTemp[128] = { 0 };
    //    sprintf(szTemp, "NET_EHOME_S2C_CANCEL_SCHEDULE failed, err[%d]", dwErr);
    //    AfxMessageBox(szTemp);
    //}
    //else
    //{
    //    AfxMessageBox("NET_EHOME_S2C_CANCEL_SCHEDULE success");
    //}
}


//void CSubDlgXmlCfg::OnBnClickedBtnGetTerminalInfo()
//{
//    // TODO:  在此添加控件通知处理程序代码
//    NET_EHOME_TERMINAL_INFO struTerminalInfo = { 0 };
//    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
//    struSendData.dwSize = sizeof(struSendData);
//    struSendData.lpInBuffer = NULL;
//    struSendData.dwInBufferSize = 0;
//    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
//    struRecvData.dwSize = sizeof(struRecvData);
//    struRecvData.lpOutBuffer = &struTerminalInfo;
//    struRecvData.dwOutBufferSize = sizeof(NET_EHOME_TERMINAL_INFO);
//    CheckInitParam();
//    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_GET_TERMINAL_INFO, &struSendData, &struRecvData))
//    {
//        DWORD dwErr = NET_ECMS_GetLastError();
//        AfxMessageBox("NET_EHOME_S2C_GET_TERMINAL_INFO failed, err[%d]", dwErr);
//    }
//    else
//    {
//        AfxMessageBox("NET_EHOME_S2C_GET_TERMINAL_INFO success");
//    }
//}


//void CSubDlgXmlCfg::OnBnClickedBtnInfoDiffusionAddition()
//{
//    // TODO:  在此添加控件通知处理程序代码
//    CDlgInfoDiffusionAddition dlg;
//    CheckInitParam();
//    dlg.m_lLoginID = m_lLoginID;
//    dlg.m_iDeviceIndex = m_iDeviceIndex;
//    dlg.DoModal();
//}


//void CSubDlgXmlCfg::OnBnClickedButtonBinaryConfig()
//{
//    // TODO:  在此添加控件通知处理程序代码
//
//    NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
//    struSendData.dwSize = sizeof(struSendData);
//    char sInput[512] = { 0 };
//    strcpy(sInput, "{\"cmd\":1001,\"data\":{\"ip\":\"10.100.60.26\",\"port\":123}}");
//    struSendData.lpInBuffer = sInput;
//    struSendData.dwInBufferSize = strlen(sInput);
//    NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
//    struRecvData.dwSize = sizeof(struRecvData);
//    char sOutput[512] = { 0 };
//    struRecvData.lpOutBuffer = sOutput;
//    struRecvData.dwOutBufferSize = sizeof(sOutput);
//    if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_TRANS_WITH_RET, &struSendData, &struRecvData))
//    {
//        DWORD dwErr = NET_ECMS_GetLastError();
//    }
//
//    return;
//}


void CSubDlgXmlCfg::OnBnClickedBtnInfopublsih()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgInfoPublish dlg;
	dlg.DoModal();
}
