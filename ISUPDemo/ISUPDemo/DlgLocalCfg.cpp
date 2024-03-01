// DlgLocalCfg.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgLocalCfg.h"
#include "HCISUPAlarm.h"
#include "DlgCmsAlarm.h"
#include "EHomeDemoDlg.h"

//#include "HCISUPAlarm.h"


// CDlgLocalCfg dialog

IMPLEMENT_DYNAMIC(CDlgLocalCfg, CDialog)

CDlgLocalCfg::CDlgLocalCfg(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgLocalCfg::IDD, pParent)
    , m_strLogPath(_T(""))
    , m_iLogLevel(0)
    , m_bAutoDel(FALSE)
    , m_dwKeepLive(0)
    , m_dwCount(0)
    , m_csPicServerIP(_T(""))
    , m_csAlarmServerIP(_T(""))
    , m_iAlarmUdpPort(0)
    , m_iAlarmTcpPort(0)
    , m_iPicServerPort(0)
    , m_iAlarmType(0)
    , m_iPicType(0)
    , m_iAlarmMqttPort(0)
    , m_bHttps(FALSE)
    , m_strDasConfigFile(_T(""))
    , m_iErrorCode(0)
    , m_bSmod(FALSE)
    , m_strPublicKey(_T(""))
    , m_strPrivateKey(_T(""))
    , m_strSK(_T(""))
    , m_strAK(_T(""))
    , m_iPoolID(0)
{

}

CDlgLocalCfg::~CDlgLocalCfg()
{
}

void CDlgLocalCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_LOG_PATH, m_strLogPath);
    DDV_MaxChars(pDX, m_strLogPath, 255);
    DDX_Text(pDX, IDC_EDIT_LOG_PATH4, m_iLogLevel);
    DDX_Check(pDX, IDC_CHK_LOG_AUTO_DEL, m_bAutoDel);
    DDX_Control(pDX, IDC_COM_CMS_ACCESS_TYPE, m_comCmsAccess);
    DDX_Control(pDX, IDC_COM_ALARM_ACCESS_TYPE, m_comAlarmAccess);
    DDX_Control(pDX, IDC_COM_STREAM_ACCESS_TYPE, m_comStreamAccess);
    DDX_Text(pDX, IDC_EDT_KEEPALIVE_INTERVAL, m_dwKeepLive);
    DDX_Text(pDX, IDC_EDT_TIME_COUNT, m_dwCount);
    DDX_Text(pDX, IDC_EDT_ALARM_SERVER_IP, m_csAlarmServerIP);
    DDX_Text(pDX, IDC_EDT_ALARM_SERVER_UDP_PORT, m_iAlarmUdpPort);
    DDX_Text(pDX, IDC_EDT_ALARM_SERVER_TCP_PORT, m_iAlarmTcpPort);
    DDX_Text(pDX, IDC_EDT_PIC_SERVER_IP, m_csPicServerIP);
    DDX_Text(pDX, IDC_EDT_PIC_SERVER_PORT, m_iPicServerPort);
    DDX_CBIndex(pDX, IDC_COM_ALARM_TYPE, m_iAlarmType);
    DDX_CBIndex(pDX, IDC_COM_PIC_TYPE, m_iPicType);
    DDX_Text(pDX, IDC_EDT_ALARM_SERVER_MQTT_PORT, m_iAlarmMqttPort);
    DDX_Check(pDX, IDC_CHECK_HTTPS, m_bHttps);
    DDX_Text(pDX, IDC_EDIT_DAS_CONFIG_FILE, m_strDasConfigFile);
    DDX_Control(pDX, IDC_COM_SERVER_TYPE, m_comboServerType);
    DDX_Control(pDX, IDC_CMB_CB_TYPE, m_callBackType);
    DDX_Control(pDX, IDC_CMB_ERROR_ENABLED, m_bErrorCodeTest);
    DDX_Text(pDX, IDC_EDT_ERROR_CODE, m_iErrorCode);
    DDX_Check(pDX, IDC_CHECK_SMOD, m_bSmod);
    DDX_Text(pDX, IDC_EDIT3, m_strPublicKey);
    DDX_Text(pDX, IDC_EDIT4, m_strPrivateKey);
    DDX_Text(pDX, IDC_EDIT_SERSK, m_strSK);
    DDX_Text(pDX, IDC_EDIT_SERAK, m_strAK);
    DDX_Text(pDX, IDC_EDIT_POOLID, m_iPoolID);
}


BEGIN_MESSAGE_MAP(CDlgLocalCfg, CDialog)
    ON_BN_CLICKED(IDC_BTN_OK, &CDlgLocalCfg::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDC_BTN_GET_SECURE_ACCESS, &CDlgLocalCfg::OnBtnGetSecureAccessClicked)
    ON_BN_CLICKED(IDC_BTN_SET_SECURE_ACCESS, &CDlgLocalCfg::OnBtnSetSecureAccessClicked)
    ON_BN_CLICKED(IDC_BTN_CMS_ALARM, &CDlgLocalCfg::OnBnClickedBtnCmsAlarm)
    ON_BN_CLICKED(IDC_BTN_REG_CFG, &CDlgLocalCfg::OnBnClickedBtnRegCfg)
    ON_EN_CHANGE(IDC_EDIT_LOG_PATH4, &CDlgLocalCfg::OnEnChangeEditLogPath4)
    ON_EN_CHANGE(IDC_EDT_TIME_COUNT, &CDlgLocalCfg::OnEnChangeEdtTimeCount)
    ON_BN_CLICKED(IDC_BTN_GET_COMPATH, &CDlgLocalCfg::OnBnClickedBtnGetCompath)
    ON_BN_CLICKED(IDC_BTN_REG_CFG_CUR, &CDlgLocalCfg::OnBnClickedBtnRegCfgCur)
    ON_BN_CLICKED(IDC_BTN_DAS_OK, &CDlgLocalCfg::OnBnClickedBtnDasOk)
    ON_BN_CLICKED(IDC_BTN_ENABLED_ERROR, &CDlgLocalCfg::OnBnClickedBtnEnabledError)
    ON_CBN_SELCHANGE(IDC_COM_PIC_TYPE, &CDlgLocalCfg::OnCbnSelchangeComPicType)
    ON_BN_CLICKED(IDC_BTN_ALARM_OK, &CDlgLocalCfg::OnBnClickedBtnAlarmOk)
END_MESSAGE_MAP()


// CDlgLocalCfg message handlers

void CDlgLocalCfg::OnBnClickedBtnOk()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    g_pMainDlg->m_struLogInfo.dwLogLevel = m_iLogLevel;
    memcpy(g_pMainDlg->m_struLogInfo.byLogPath, m_strLogPath.GetBuffer(0), strlen(m_strLogPath.GetBuffer(0)));
    g_pMainDlg->m_struLogInfo.bAutoDel = m_bAutoDel;

    NET_ECMS_SetLogToFile(g_pMainDlg->m_struLogInfo.dwLogLevel, (char *)g_pMainDlg->m_struLogInfo.byLogPath, g_pMainDlg->m_struLogInfo.bAutoDel);
    NET_EALARM_SetLogToFile(g_pMainDlg->m_struLogInfo.dwLogLevel, (const char *)g_pMainDlg->m_struLogInfo.byLogPath, g_pMainDlg->m_struLogInfo.bAutoDel);
    NET_ESTREAM_SetLogToFile(g_pMainDlg->m_struLogInfo.dwLogLevel, (char *)g_pMainDlg->m_struLogInfo.byLogPath, g_pMainDlg->m_struLogInfo.bAutoDel);
    NET_ESS_SetLogToFile(g_pMainDlg->m_struLogInfo.dwLogLevel, (const char *)g_pMainDlg->m_struLogInfo.byLogPath, g_pMainDlg->m_struLogInfo.bAutoDel);

    ModifyLogInfoToFile();



    CString str;
    GetDlgItem(IDC_EDT_COMPATH)->GetWindowText(str);

    char szPath[MAX_PATH] = { 0 };
    strcpy(szPath, str.GetBuffer());

    BOOL bRet = NET_ECMS_SetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE::COM_PATH, szPath);
    if (!bRet)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetSDKLocalCfg COM_PATH, Path[%s] Failed", szPath);
    }
    bRet = NET_EALARM_SetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE::COM_PATH, szPath);
    if (!bRet)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_EALARM_SetSDKLocalCfg COM_PATH, Path[%s] Failed", szPath);
    }
    bRet = NET_ESTREAM_SetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE::COM_PATH, szPath);
    if (!bRet)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ESTREAM_SetSDKLocalCfg COM_PATH, Path[%s] Failed", szPath);
    }

}

VOID CDlgLocalCfg::ModifyLogInfoToFile()
{
    CXmlBase   struXMLBase;
    struXMLBase.LoadFile(".\\EHomeDemo.xml");

    if (struXMLBase.FindElem("LocalCfg") && struXMLBase.IntoElem())
    {
        if (struXMLBase.FindElem("Param") && struXMLBase.IntoElem())
        {
            char szLogLevel[32] = { 0 };
            char szLogAutoDel[32] = { 0 };
            sprintf(szLogLevel, "%d", m_iLogLevel);

            if (m_bAutoDel)
            {
                sprintf(szLogAutoDel, "%s", "true");
            }
            else
            {
                sprintf(szLogAutoDel, "%s", "false");
            }
            

            struXMLBase.ModifyData("LogPath", (const char *)m_strLogPath.GetBuffer(0));
            struXMLBase.ModifyData("LogLevel", (const char *)szLogLevel);
            struXMLBase.ModifyData("LogAutoDel", (const char *)szLogAutoDel);

            struXMLBase.WriteToFile(".\\EHomeDemo.xml");
        }
    }
}

BOOL CDlgLocalCfg::OnInitDialog()
{   
    CDialog::OnInitDialog();

    m_iLogLevel = g_pMainDlg->m_struLogInfo.dwLogLevel;
    m_strLogPath.SetString((const char *)g_pMainDlg->m_struLogInfo.byLogPath);
    m_bAutoDel = g_pMainDlg->m_struLogInfo.bAutoDel;

    m_comCmsAccess.SetCurSel(g_pMainDlg->m_byCmsSecureAccessType);
    m_comAlarmAccess.SetCurSel(g_pMainDlg->m_byAlarmSecureAccessType);
    m_comStreamAccess.SetCurSel(g_pMainDlg->m_byStreamSecureAccessType);

    m_dwKeepLive = g_pMainDlg->m_struServInfo.dwKeepAliveSec;
    m_dwCount = g_pMainDlg->m_struServInfo.dwTimeOutCount;

    m_iAlarmType = g_pMainDlg->m_struServInfo.dwAlarmServerType;
    m_iPicType = g_pMainDlg->m_struServInfo.dwPicServerType;
    m_iAlarmUdpPort = g_pMainDlg->m_struServInfo.struUDPAlarmSever.wPort;
    m_iAlarmTcpPort = g_pMainDlg->m_struServInfo.struTCPAlarmSever.wPort;
    m_iAlarmMqttPort = g_pMainDlg->m_wAlarmServerMqttPort;
    m_iPicServerPort = g_pMainDlg->m_struServInfo.struPictureSever.wPort;
    m_csPicServerIP = g_pMainDlg->m_struServInfo.struPictureSever.szIP;
    m_csAlarmServerIP = g_pMainDlg->m_struServInfo.struTCPAlarmSever.szIP;

    m_comboServerType.SetCurSel(g_iServerType);
    m_strDasConfigFile = g_pMainDlg->m_strDasConfigFile;
    m_callBackType.SetCurSel(0);
    UpdateData(FALSE);
    OnCbnSelchangeComPicType();
    return TRUE;
}

void CDlgLocalCfg::OnBtnGetSecureAccessClicked()
{
    NET_EHOME_LOCAL_ACCESS_SECURITY struAccessSecure = {0};
    struAccessSecure.dwSize = sizeof(struAccessSecure);

    if (!NET_ECMS_GetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ECMS_GetSDKLocalCfg, Error=[%d]", NET_ECMS_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_ECMS_GetSDKLocalCfg");
        m_comCmsAccess.SetCurSel(struAccessSecure.byAccessSecurity);
    }

    memset(&struAccessSecure, 0, sizeof(struAccessSecure));
    struAccessSecure.dwSize = sizeof(struAccessSecure);
    if (!NET_EALARM_GetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_EALARM_GetSDKLocalCfg, Error=[%d]", NET_EALARM_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_GetSDKLocalCfg");
        m_comAlarmAccess.SetCurSel(struAccessSecure.byAccessSecurity);
    }

    memset(&struAccessSecure, 0, sizeof(struAccessSecure));
    struAccessSecure.dwSize = sizeof(struAccessSecure);
    if (!NET_ESTREAM_GetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ESTREAM_GetSDKLocalCfg, Error=[%d]", NET_ESTREAM_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 2, "NET_EALARM_GetSDKLocalCfg");
        m_comStreamAccess.SetCurSel(struAccessSecure.byAccessSecurity);
    }
}

void CDlgLocalCfg::OnBtnSetSecureAccessClicked()
{
    NET_EHOME_LOCAL_ACCESS_SECURITY struAccessSecure = {0};
    struAccessSecure.dwSize = sizeof(struAccessSecure);

    struAccessSecure.byAccessSecurity = (BYTE)m_comCmsAccess.GetCurSel();

    if (!NET_ECMS_SetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ECMS_SetSDKLocalCfg, Error=[%d]", NET_ECMS_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_ECMS_SetSDKLocalCfg");
        g_pMainDlg->m_byCmsSecureAccessType = (BYTE)m_comCmsAccess.GetCurSel();
    }

    struAccessSecure.byAccessSecurity = (BYTE)m_comAlarmAccess.GetCurSel();

    if (!NET_EALARM_SetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_EALARM_SetSDKLocalCfg, Error=[%d]", NET_EALARM_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_SetSDKLocalCfg");
        g_pMainDlg->m_byAlarmSecureAccessType = (BYTE)m_comAlarmAccess.GetCurSel();
    }

    struAccessSecure.byAccessSecurity = (BYTE)m_comStreamAccess.GetCurSel();
    if (!NET_ESTREAM_SetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ESTREAM_SetSDKLocalCfg, Error=[%d]", NET_ESTREAM_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 2, "NET_ESTREAM_SetSDKLocalCfg");
        g_pMainDlg->m_byStreamSecureAccessType = (BYTE)m_comStreamAccess.GetCurSel();
    }

    ModifySecureInfoToFile();

}

void CDlgLocalCfg::ModifySecureInfoToFile()
{
    CXmlBase   struXMLBase;
    struXMLBase.LoadFile(".\\EHomeDemo.xml");

    if (struXMLBase.FindElem("LocalCfg") && struXMLBase.IntoElem())
    {
        if (struXMLBase.FindElem("Param") && struXMLBase.IntoElem())
        {
            char szCMSSecurity[32] = { 0 };
            char szAMSSecurity[32] = { 0 };
            char szSMSSecurity[32] = { 0 };

            sprintf(szCMSSecurity, "%d", g_pMainDlg->m_byCmsSecureAccessType);
            sprintf(szAMSSecurity, "%d", g_pMainDlg->m_byAlarmSecureAccessType);
            sprintf(szSMSSecurity, "%d", g_pMainDlg->m_byStreamSecureAccessType);

            struXMLBase.ModifyData("CmsAccessSecurity", (const char *)szCMSSecurity);
            struXMLBase.ModifyData("AlarmAccessSecurity", (const char *)szAMSSecurity);
            struXMLBase.ModifyData("StreamAccessSecurity", (const char *)szSMSSecurity);

            struXMLBase.WriteToFile(".\\EHomeDemo.xml");
        }
    }
}

void CDlgLocalCfg::OnBnClickedBtnCmsAlarm()
{
    // TODO:  在此添加控件通知处理程序代码
    CDlgCmsAlarm dlg;
    dlg.DoModal();
}

void CDlgLocalCfg::OnBnClickedBtnRegCfg()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);


    memcpy(g_pMainDlg->m_struServInfo.struPictureSever.szIP, m_csPicServerIP.GetBuffer(0), m_csPicServerIP.GetLength());
    g_pMainDlg->m_struServInfo.struPictureSever.wPort = m_iPicServerPort;
    g_pMainDlg->m_struServInfo.dwPicServerType = m_iPicType;

    NET_ESS_StopListen(g_pMainDlg->m_lSSHandle);

    NET_EHOME_SS_LISTEN_PARAM struSSListenParam = { 0 };
    if (!m_bSmod)
    {
        struSSListenParam.bySecurityMode = 1;
    }
    memcpy(struSSListenParam.struAddress.szIP, m_csPicServerIP.GetBuffer(0), m_csPicServerIP.GetLength());
    struSSListenParam.struAddress.wPort = m_iPicServerPort;
    memcpy(struSSListenParam.szKMS_UserName, "test", strlen("test"));
    memcpy(struSSListenParam.szKMS_Password, "12345", strlen("12345"));
    memcpy(struSSListenParam.szAccessKey, "test", strlen("test"));
    memcpy(struSSListenParam.szSecretKey, "12345", strlen("12345"));
    struSSListenParam.fnSSMsgCb = SS_Message_Callback;
    if (m_callBackType.GetCurSel() == 0)
    {
        struSSListenParam.fnSStorageCb = SS_Storage_Callback;
    }
    else if (m_callBackType.GetCurSel() == 1)
    {
        struSSListenParam.fnSSRWCb = SS_RW_Callback;
    }
    else if (m_callBackType.GetCurSel() == 2)
    {
        struSSListenParam.fnSSRWCbEx = SS_RW_CallbackEx;
    }

    struSSListenParam.pUserData = this;
    struSSListenParam.byHttps = m_bHttps;
    if (struSSListenParam.byHttps)
    {
        NET_EHOME_SS_LISTEN_HTTPS_PARAM struHttpsParam = { 0 };
        struHttpsParam.byHttps = 1;
        struHttpsParam.byCertificateFileType = 0;
        struHttpsParam.byPrivateKeyFileType = 0;
        struHttpsParam.dwSSLVersion = 3;
        strncpy(struHttpsParam.szUserCertificateFile
            , m_strPublicKey.GetBuffer()
            , MAX_PATH - 1);
        strncpy(struHttpsParam.szUserPrivateKeyFile
            , m_strPrivateKey.GetBuffer()
            , MAX_PATH - 1);
        struHttpsParam.dwSSLVersion = 0;
        NET_ESS_SetListenHttpsParam(&struHttpsParam);
    }
    else
    {
        NET_EHOME_SS_LISTEN_HTTPS_PARAM struHttpsParam = { 0 };
        struHttpsParam.byCertificateFileType = 0;
        struHttpsParam.byPrivateKeyFileType = 0;
        strncpy(struHttpsParam.szUserCertificateFile
            , m_strPublicKey.GetBuffer()
            , MAX_PATH - 1);
        strncpy(struHttpsParam.szUserPrivateKeyFile
            , m_strPrivateKey.GetBuffer()
            , MAX_PATH - 1);
        struHttpsParam.dwSSLVersion = 0;
        NET_ESS_SetListenHttpsParam(&struHttpsParam);
    }

    ModifyServerInfoToFile();
    // 端口映射时，设置映射地址信息
    NET_EHOME_IPADDRESS struPublicAddress = { 0 };
    g_pMainDlg->GetAddressByType(4, 0, struPublicAddress.szIP, sizeof(struPublicAddress.szIP), (char *)&(struPublicAddress.wPort), sizeof(struPublicAddress.wPort));
    if (struPublicAddress.wPort == 0)
    {
        struPublicAddress.wPort = struSSListenParam.struAddress.wPort;
    }
    //memcpy(struPublicAddress.szIP, m_struServInfo.struPictureSever.szIP, sizeof(m_struServInfo.struPictureSever.szIP));
    NET_ESS_SetSDKInitCfg(NET_EHOME_SS_INIT_CFG_PUBLIC_IP_PORT, &struPublicAddress);
    //NET_ESS_SetSDKInitCfg(NET_EHOME_SS_INIT_CFG_PUBLIC_IP_PORT, &struSSListenParam.struAddress);

    g_pMainDlg->m_lSSHandle = NET_ESS_StartListen(&struSSListenParam);
    if (-1 == g_pMainDlg->m_lSSHandle)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_ESS_StartListen Failed, wPort[%d], error[%d]",
            struSSListenParam.struAddress.wPort, NET_ESS_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_SS_StartListen Succ wPort[%d]", struSSListenParam.struAddress.wPort);
    }
}


void CDlgLocalCfg::ModifyServerInfoToFile()
{
    CXmlBase   struXMLBase;
    struXMLBase.LoadFile(".\\EHomeDemo.xml");

    if (struXMLBase.FindElem("LocalCfg") && struXMLBase.IntoElem())
    {
        if (struXMLBase.FindElem("Param") && struXMLBase.IntoElem())
        {
            char szKeepAliveSeconds[32] = { 0 };
            char szKeepAliveCount[32] = { 0 };
            char szAlarmServerUdpPort[32] = { 0 };
            char szAlarmServerType[32] = { 0 };
            char szAlarmServerTcpPort[32] = { 0 };
            char szAlarmServerMqttPort[32] = { 0 };
            char szPictureServerType[32] = { 0 };
            char szPictureServerPort[32] = { 0 };

            sprintf(szKeepAliveSeconds, "%d", m_dwKeepLive);
            sprintf(szKeepAliveCount, "%d", m_dwCount);
            sprintf(szAlarmServerType, "%d", m_iAlarmType);

            sprintf(szAlarmServerUdpPort, "%d", m_iAlarmUdpPort);
            
            sprintf(szAlarmServerTcpPort, "%d", m_iAlarmTcpPort);
            sprintf(szAlarmServerMqttPort, "%d", m_iAlarmMqttPort);

            sprintf(szPictureServerType, "%d", m_iPicType);
            sprintf(szPictureServerPort, "%d", m_iPicServerPort);

            struXMLBase.ModifyData("KeepAliveSeconds", (const char *)szKeepAliveSeconds);
            struXMLBase.ModifyData("KeepAliveCount", (const char *)szKeepAliveCount);
            struXMLBase.ModifyData("AlarmServerType", (const char *)szAlarmServerType);
            struXMLBase.ModifyData("AlarmServerIP", (const char *)m_csAlarmServerIP.GetBuffer(0));
            struXMLBase.ModifyData("AlarmServerUdpPort", (const char *)szAlarmServerUdpPort);
            struXMLBase.ModifyData("AlarmServerTcpPort", (const char *)szAlarmServerTcpPort);
            struXMLBase.ModifyData("AlarmServerMqttPort", (const char *)szAlarmServerMqttPort);
            struXMLBase.ModifyData("PictureServerType", (const char *)szPictureServerType);
            struXMLBase.ModifyData("PictureServerIP", (const char *)m_csPicServerIP.GetBuffer(0));
            struXMLBase.ModifyData("PictureServerPort", (const char *)szPictureServerPort);

            struXMLBase.WriteToFile(".\\EHomeDemo.xml");
        }
    }
}

void CDlgLocalCfg::OnEnChangeEditLogPath4()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialog::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}


void CDlgLocalCfg::OnEnChangeEdtTimeCount()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialog::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}


void CDlgLocalCfg::OnBnClickedBtnGetCompath()
{
    // TODO:  在此添加控件通知处理程序代码
    char szComPath[MAX_PATH] = { 0 };
    NET_ECMS_GetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE::COM_PATH, szComPath);
    CString str(szComPath);
    GetDlgItem(IDC_EDT_COMPATH)->SetWindowText(str);

    UpdateData(FALSE);
    
}


void CDlgLocalCfg::OnBnClickedBtnRegCfgCur()
{
    // TODO:  在此添加控件通知处理程序代码
    // 设置指定用户的超时时间
    UpdateData(TRUE);
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();

    int lUserID = g_struDeviceInfo[iDeviceIndex].lLoginID;
    if (lUserID < 0)
    {
        return;
    }

    NET_ECMS_SetAliveTimeout(lUserID, m_dwKeepLive, m_dwCount);
}


void CDlgLocalCfg::OnBnClickedBtnDasOk()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CXmlBase   struXMLBase;
    struXMLBase.LoadFile(".\\EHomeDemo.xml");

    g_iServerType = m_comboServerType.GetCurSel();
    g_pMainDlg->m_strDasConfigFile = m_strDasConfigFile;

    if (struXMLBase.FindElem("LocalCfg") && struXMLBase.IntoElem())
    {
        if (struXMLBase.FindElem("DasInfo") && struXMLBase.IntoElem())
        {
            char szServerType[32] = { 0 };

            sprintf(szServerType, "%d", m_comboServerType.GetCurSel());

            struXMLBase.ModifyData("ServerType", (const char *)szServerType);
            struXMLBase.ModifyData("DasInfoPath", (const char *)m_strDasConfigFile.GetBuffer());

            struXMLBase.WriteToFile(".\\EHomeDemo.xml");
        }
    }
}


void CDlgLocalCfg::OnBnClickedBtnEnabledError()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bErrorCodeTest.GetCurSel() == 1)
    {
        g_bSS_EnableError = TRUE;
    }
    else
    {
        g_bSS_EnableError = FALSE;
    }
    g_iSSErrorCode = m_iErrorCode;
    UpdateData(FALSE);
}


void CDlgLocalCfg::OnCbnSelchangeComPicType()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_iPicType == 2)//云存储协议
    {
        GetDlgItem(IDC_EDIT_SERAK)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_SERSK)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_POOLID)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_SERAK)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_SERSK)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_POOLID)->EnableWindow(FALSE);
    }
}

void CDlgLocalCfg::OnBnClickedBtnAlarmOk()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    memset(&g_pMainDlg->m_struServInfo, 0, sizeof(g_pMainDlg->m_struServInfo));
    g_pMainDlg->m_struServInfo.dwKeepAliveSec = m_dwKeepLive;
    g_pMainDlg->m_struServInfo.dwTimeOutCount = m_dwCount;
    g_pMainDlg->m_struServInfo.dwAlarmKeepAliveSec = m_dwKeepLive;
    g_pMainDlg->m_struServInfo.dwAlarmTimeOutCount = m_dwCount;
    if (m_iAlarmUdpPort == m_iAlarmTcpPort || m_iAlarmUdpPort == m_iAlarmMqttPort || m_iAlarmMqttPort == m_iAlarmTcpPort)
    {
        AfxMessageBox(_T("Port Conflict!!! Please Set Different Port Value."));
        return;
    }

    if (g_pMainDlg->m_lUdpAlarmHandle >= 0)
    {
        if (!NET_EALARM_StopListen(g_pMainDlg->m_lUdpAlarmHandle))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_EALARM_StopListen UDP Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_StopListen UDP succ");
        }
    }
    if (g_pMainDlg->m_lTcpAlarmHandle >= 0)
    {
        if (!NET_EALARM_StopListen(g_pMainDlg->m_lTcpAlarmHandle))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_EALARM_StopListen TCP Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_StopListen TCP succ");
        }
    }
    if (g_pMainDlg->m_lEhome50AlarmHandle >= 0)
    {
        if (!NET_EALARM_StopListen(g_pMainDlg->m_lEhome50AlarmHandle))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_EALARM_StopListen mqtt Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_StopListen mqtt succ");
        }
    }

    if (m_iAlarmType == 0)
    {
        memcpy(g_pMainDlg->m_struServInfo.struUDPAlarmSever.szIP, m_csAlarmServerIP.GetBuffer(0), m_csAlarmServerIP.GetLength());
        g_pMainDlg->m_struServInfo.struUDPAlarmSever.wPort = m_iAlarmUdpPort;
        g_pMainDlg->m_struServInfo.dwAlarmServerType = 0;

        NET_EHOME_ALARM_LISTEN_PARAM struAlarmListenParam = { 0 };
        memcpy(struAlarmListenParam.struAddress.szIP, m_csAlarmServerIP.GetBuffer(0), m_csAlarmServerIP.GetLength());

        struAlarmListenParam.struAddress.wPort = m_iAlarmUdpPort;
        struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
        struAlarmListenParam.pUserData = g_pMainDlg;
        struAlarmListenParam.byProtocolType = 1;
        struAlarmListenParam.byUseThreadPool = 0;
        struAlarmListenParam.dwKeepAliveSec = g_pMainDlg->m_struServInfo.dwAlarmKeepAliveSec;
        struAlarmListenParam.dwTimeOutCount = g_pMainDlg->m_struServInfo.dwAlarmTimeOutCount;
        g_pMainDlg->m_lUdpAlarmHandle = NET_EALARM_StartListen(&struAlarmListenParam);
        if (-1 == g_pMainDlg->m_lUdpAlarmHandle)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_EALARM_StartListen udp Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_StartListen udp succ");
        }
    }
    else if (m_iAlarmType == 1)
    {
        memcpy(g_pMainDlg->m_struServInfo.struUDPAlarmSever.szIP, m_csAlarmServerIP.GetBuffer(0), m_csAlarmServerIP.GetLength());
        g_pMainDlg->m_struServInfo.struUDPAlarmSever.wPort = m_iAlarmUdpPort;
        memcpy(g_pMainDlg->m_struServInfo.struTCPAlarmSever.szIP, m_csAlarmServerIP.GetBuffer(0), m_csAlarmServerIP.GetLength());
        g_pMainDlg->m_struServInfo.struTCPAlarmSever.wPort = m_iAlarmTcpPort;
        g_pMainDlg->m_struServInfo.dwAlarmServerType = 1;


        NET_EHOME_ALARM_LISTEN_PARAM struAlarmListenParam = { 0 };
        memcpy(struAlarmListenParam.struAddress.szIP, m_csAlarmServerIP.GetBuffer(0), m_csAlarmServerIP.GetLength());

        struAlarmListenParam.struAddress.wPort = m_iAlarmTcpPort;
        struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
        struAlarmListenParam.pUserData = g_pMainDlg;
        struAlarmListenParam.byProtocolType = 0;
        struAlarmListenParam.byUseThreadPool = 0;
        struAlarmListenParam.dwKeepAliveSec = g_pMainDlg->m_struServInfo.dwKeepAliveSec;
        struAlarmListenParam.dwTimeOutCount = g_pMainDlg->m_struServInfo.dwTimeOutCount;

        g_pMainDlg->m_lTcpAlarmHandle = NET_EALARM_StartListen(&struAlarmListenParam);
        if (-1 == g_pMainDlg->m_lTcpAlarmHandle)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_EALARM_StartListen tcp Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_StartListen tcp succ");
        }

        struAlarmListenParam.struAddress.wPort = m_iAlarmUdpPort;
        struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
        struAlarmListenParam.pUserData = g_pMainDlg;
        struAlarmListenParam.byProtocolType = 1;
        struAlarmListenParam.byUseThreadPool = 0;

        g_pMainDlg->m_lUdpAlarmHandle = NET_EALARM_StartListen(&struAlarmListenParam);
        if (-1 == g_pMainDlg->m_lUdpAlarmHandle)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_EALARM_StartListen udp Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_StartListen udp succ");
        }

    }
    else if (m_iAlarmType == 2)
    {
        memcpy(g_pMainDlg->m_struServInfo.struTCPAlarmSever.szIP, m_csAlarmServerIP.GetBuffer(0), m_csAlarmServerIP.GetLength());
        g_pMainDlg->m_struServInfo.struTCPAlarmSever.wPort = m_iAlarmMqttPort;
        g_pMainDlg->m_struServInfo.dwAlarmServerType = 2;

        NET_EHOME_ALARM_LISTEN_PARAM struAlarmListenParam = { 0 };
        memcpy(struAlarmListenParam.struAddress.szIP, m_csAlarmServerIP.GetBuffer(0), m_csAlarmServerIP.GetLength());

        struAlarmListenParam.struAddress.wPort = m_iAlarmMqttPort;
        struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
        struAlarmListenParam.pUserData = g_pMainDlg;
        struAlarmListenParam.byProtocolType = 2;
        struAlarmListenParam.byUseThreadPool = 0;
        struAlarmListenParam.dwKeepAliveSec = g_pMainDlg->m_struServInfo.dwKeepAliveSec;
        struAlarmListenParam.dwTimeOutCount = g_pMainDlg->m_struServInfo.dwTimeOutCount;

        g_pMainDlg->m_lEhome50AlarmHandle = NET_EALARM_StartListen(&struAlarmListenParam);
        if (-1 == g_pMainDlg->m_lEhome50AlarmHandle)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_EALARM_StartListen mqtt Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_StartListen mqtt succ");
        }
    }
    ModifyServerInfoToFile();
}
