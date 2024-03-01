c// SubDlgNetworkCfg.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "SubDlgNetworkCfg.h"
#include "Public/Convert/Convert.h"


// CSubDlgNetworkCfg dialog

IMPLEMENT_DYNAMIC(CSubDlgNetworkCfg, CDialog)

CSubDlgNetworkCfg::CSubDlgNetworkCfg(CWnd* pParent /*=NULL*/)
    : CDialog(CSubDlgNetworkCfg::IDD, pParent)
    , m_strDevAddr(_T(""))
    , m_iSDKPort(0)
    , m_iHttpPort(0)
    , m_iDeviceIndex(-1)
    , m_lLoginID(-1)
    , m_strMaskAddr(_T(""))
    , m_iMTUValue(0)
    , m_strMACAddr(_T(""))
    , m_bPPPoEEnable(FALSE)
    , m_strPPPoEUser(_T(""))
    , m_strPPPoEAddr(_T(""))
    , m_strPPPoEPWD(_T(""))
    , m_strGatewayAddr(_T(""))
    , m_strMulticastAddr(_T(""))
    , m_strDDNS1Addr(_T(""))
    , m_strIPResolverAddr(_T(""))
    , m_iIPResolverPort(0)
    , m_strDDNS2Addr(_T(""))
    , m_strAlarmHostAddr(_T(""))
    , m_iAlarmHostPort(0)
{
    memset(&m_struNetworkCfg, 0 ,sizeof(m_struNetworkCfg));
    m_struNetworkCfg.dwSize = sizeof(m_struNetworkCfg);
}

CSubDlgNetworkCfg::~CSubDlgNetworkCfg()
{
}

void CSubDlgNetworkCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_DEV_ADDR, m_strDevAddr);
    DDX_Text(pDX, IDC_EDIT_SDK_PORT, m_iSDKPort);
    DDX_Text(pDX, IDC_EDIT_HTTP_PORT, m_iHttpPort);
    DDX_Text(pDX, IDC_EDIT_MASK_ADDR, m_strMaskAddr);
    DDX_Control(pDX, IDC_COMBO_NET_TYPE, m_cmbNetType);
    DDX_Text(pDX, IDC_EDIT_MTU_VALUE, m_iMTUValue);
    DDX_Text(pDX, IDC_EDIT_MAC_ADDR, m_strMACAddr);
    DDX_Check(pDX, IDC_CHECK_PPPOE_ENABLE, m_bPPPoEEnable);
    DDX_Text(pDX, IDC_EDIT_PPPOE_USER, m_strPPPoEUser);
    DDX_Text(pDX, IDC_EDIT_PPPOE_ADDR, m_strPPPoEAddr);
    DDX_Text(pDX, IDC_EDIT_PPPOE_PWD, m_strPPPoEPWD);
    DDX_Text(pDX, IDC_EDIT_GATEWAY_ADDR, m_strGatewayAddr);
    DDX_Text(pDX, IDC_EDIT_MULTI_ADDR, m_strMulticastAddr);
    DDX_Text(pDX, IDC_EDIT_DDNS1_ADDR, m_strDDNS1Addr);
    DDX_Text(pDX, IDC_EDIT_IPRESOLVER_ADDR, m_strIPResolverAddr);
    DDX_Text(pDX, IDC_EDIT_IPRESOLVER_PORT, m_iIPResolverPort);
    DDX_Text(pDX, IDC_EDIT_DDNS2_ADDR, m_strDDNS2Addr);
    DDX_Text(pDX, IDC_EDIT_ALARMHOST_ADDR, m_strAlarmHostAddr);
    DDX_Text(pDX, IDC_EDIT_ALARMHOST_PORT, m_iAlarmHostPort);
}


BEGIN_MESSAGE_MAP(CSubDlgNetworkCfg, CDialog)
    ON_BN_CLICKED(IDC_CHECK_PPPOE_ENABLE, &CSubDlgNetworkCfg::OnBnClickedCheckPppoeEnable)
END_MESSAGE_MAP()


// CSubDlgNetworkCfg message handlers

BOOL CSubDlgNetworkCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CRect rc(0, 0, 0, 0);
    GetParent()->GetClientRect(&rc);
    ((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
    MoveWindow(&rc);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSubDlgNetworkCfg::CheckInitParam()
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

void CSubDlgNetworkCfg::CurCfgUpdate()
{
    if(!CheckInitParam())
    {
        return;
    }
    NET_EHOME_CONFIG    struCfg = {0};
    memset(&m_struNetworkCfg, 0 , sizeof(m_struNetworkCfg));
    m_struNetworkCfg.dwSize = sizeof(m_struNetworkCfg);
    //获取网络参数
    struCfg.pOutBuf = &m_struNetworkCfg;
    struCfg.dwOutSize = sizeof(m_struNetworkCfg);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_NETWORK_CFG, &struCfg, sizeof(struCfg)))
    {
        //需要将字符串字段转换成GB2312
        DWORD dwConvertLen = 0;
        UTF82A((char*)m_struNetworkCfg.struPPPoE.sPPPoEUser, (char*)m_struNetworkCfg.struPPPoE.sPPPoEUser, NAME_LEN, &dwConvertLen);
        UTF82A((char*)m_struNetworkCfg.struPPPoE.sPPPoEPassword, (char*)m_struNetworkCfg.struPPPoE.sPPPoEPassword, PASSWD_LEN, &dwConvertLen);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_NETWORK_CFG");
        m_strDevAddr = m_struNetworkCfg.struEtherNet.struDevIP.sIpV4;
        m_strMaskAddr = m_struNetworkCfg.struEtherNet.struDevIPMask.sIpV4;
        m_cmbNetType.SetCurSel(m_struNetworkCfg.struEtherNet.dwNetInterface - 1);
        m_iSDKPort = m_struNetworkCfg.struEtherNet.wDevPort;
        m_iMTUValue = m_struNetworkCfg.struEtherNet.wMTU;
        m_strMACAddr.Format("%02x-%02x-%02x-%02x-%02x-%02x", 
            m_struNetworkCfg.struEtherNet.byMACAddr[0],
            m_struNetworkCfg.struEtherNet.byMACAddr[1],
            m_struNetworkCfg.struEtherNet.byMACAddr[2],
            m_struNetworkCfg.struEtherNet.byMACAddr[3],
            m_struNetworkCfg.struEtherNet.byMACAddr[4],
            m_struNetworkCfg.struEtherNet.byMACAddr[5]);
        m_strMACAddr.MakeUpper();
        m_bPPPoEEnable = m_struNetworkCfg.struPPPoE.dwPPPoE;
        if (m_bPPPoEEnable)
        {
            GetDlgItem(IDC_EDIT_PPPOE_USER)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDIT_PPPOE_ADDR)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDIT_PPPOE_PWD)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_EDIT_PPPOE_USER)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_PPPOE_ADDR)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_PPPOE_PWD)->EnableWindow(FALSE);
        }
        m_strPPPoEUser = m_struNetworkCfg.struPPPoE.sPPPoEUser;
        m_strPPPoEAddr = m_struNetworkCfg.struPPPoE.struPPPoEIP.sIpV4;
        m_strPPPoEPWD = m_struNetworkCfg.struPPPoE.sPPPoEPassword;
        m_strGatewayAddr = m_struNetworkCfg.struGateWayIP.sIpV4;
        m_strMulticastAddr = m_struNetworkCfg.struMultiCastIP.sIpV4;
        m_strDDNS1Addr = m_struNetworkCfg.struDDNSServer1IP.sIpV4;
        m_strDDNS2Addr = m_struNetworkCfg.struDDNSServer2IP.sIpV4;
        m_strIPResolverAddr = m_struNetworkCfg.struIPResolver.sIpV4;
        m_iIPResolverPort = m_struNetworkCfg.wIPResolverPort;
        m_strAlarmHostAddr = m_struNetworkCfg.struAlarmHostIP.sIpV4;
        m_iAlarmHostPort = m_struNetworkCfg.wAlarmHostPort;
        m_iHttpPort = m_struNetworkCfg.wHTTPPort;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_NETWORK_CFG");
    }
    UpdateData(FALSE);
}

void CSubDlgNetworkCfg::CurCfgSetup()
{
    if(!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    memset(&m_struNetworkCfg.struEtherNet.struDevIP, 0, sizeof(m_struNetworkCfg.struEtherNet.struDevIP));
    memcpy(m_struNetworkCfg.struEtherNet.struDevIP.sIpV4, m_strDevAddr.GetBuffer(0), m_strDevAddr.GetLength());
    memset(&m_struNetworkCfg.struEtherNet.struDevIPMask, 0, sizeof(m_struNetworkCfg.struEtherNet.struDevIPMask));
    memcpy(m_struNetworkCfg.struEtherNet.struDevIPMask.sIpV4, m_strMaskAddr.GetBuffer(0), m_strMaskAddr.GetLength());
    m_struNetworkCfg.struEtherNet.dwNetInterface = m_cmbNetType.GetCurSel() + 1;
    m_struNetworkCfg.struEtherNet.wDevPort = (WORD)m_iSDKPort;
    sscanf(m_strMACAddr.GetBuffer(0), _T("%x-%x-%x-%x-%x-%x"), 
        &m_struNetworkCfg.struEtherNet.byMACAddr[0],
        &m_struNetworkCfg.struEtherNet.byMACAddr[1],
        &m_struNetworkCfg.struEtherNet.byMACAddr[2],
        &m_struNetworkCfg.struEtherNet.byMACAddr[3],
        &m_struNetworkCfg.struEtherNet.byMACAddr[4],
        &m_struNetworkCfg.struEtherNet.byMACAddr[5]);
    m_struNetworkCfg.struPPPoE.dwPPPoE = m_bPPPoEEnable;
    memset(&m_struNetworkCfg.struPPPoE.struPPPoEIP, 0, sizeof(m_struNetworkCfg.struPPPoE.struPPPoEIP));
    if (m_bPPPoEEnable)
    {
        memcpy(m_struNetworkCfg.struPPPoE.struPPPoEIP.sIpV4, m_strPPPoEAddr.GetBuffer(0), m_strPPPoEAddr.GetLength());
        memcpy(m_struNetworkCfg.struPPPoE.sPPPoEUser, m_strPPPoEUser, NAME_LEN);
        memcpy(m_struNetworkCfg.struPPPoE.sPPPoEPassword, m_strPPPoEPWD, PASSWD_LEN);
    }
    else
    {
        memset(m_struNetworkCfg.struPPPoE.sPPPoEUser, 0, NAME_LEN);
        memset(m_struNetworkCfg.struPPPoE.sPPPoEPassword, 0, PASSWD_LEN);
    }
    memset(&m_struNetworkCfg.struGateWayIP, 0, sizeof(m_struNetworkCfg.struGateWayIP));
    memcpy(m_struNetworkCfg.struGateWayIP.sIpV4, m_strGatewayAddr.GetBuffer(0), m_strGatewayAddr.GetLength());
    memset(&m_struNetworkCfg.struMultiCastIP, 0, sizeof(m_struNetworkCfg.struMultiCastIP));
    memcpy(m_struNetworkCfg.struMultiCastIP.sIpV4, m_strMulticastAddr.GetBuffer(0), m_strMulticastAddr.GetLength());
    memset(&m_struNetworkCfg.struDDNSServer1IP, 0, sizeof(m_struNetworkCfg.struDDNSServer1IP));
    memcpy(m_struNetworkCfg.struDDNSServer1IP.sIpV4, m_strDDNS1Addr.GetBuffer(0), m_strDDNS1Addr.GetLength());
    memset(&m_struNetworkCfg.struDDNSServer2IP, 0, sizeof(m_struNetworkCfg.struDDNSServer2IP));
    memcpy(m_struNetworkCfg.struDDNSServer2IP.sIpV4, m_strDDNS2Addr.GetBuffer(0), m_strDDNS2Addr.GetLength());
    memset(&m_struNetworkCfg.struIPResolver, 0, sizeof(m_struNetworkCfg.struIPResolver));
    memcpy(m_struNetworkCfg.struIPResolver.sIpV4, m_strIPResolverAddr.GetBuffer(0), m_strIPResolverAddr.GetLength());
    m_struNetworkCfg.wIPResolverPort = (WORD)m_iIPResolverPort;
    memset(&m_struNetworkCfg.struAlarmHostIP, 0, sizeof(m_struNetworkCfg.struAlarmHostIP));
    memcpy(m_struNetworkCfg.struAlarmHostIP.sIpV4, m_strAlarmHostAddr.GetBuffer(0), m_strAlarmHostAddr.GetLength());
    m_struNetworkCfg.wAlarmHostPort = (WORD)m_iAlarmHostPort;
    m_struNetworkCfg.wHTTPPort = (WORD)m_iHttpPort;
    m_struNetworkCfg.struEtherNet.wMTU = (WORD)m_iMTUValue;

    //需要将字符串字段转换成UTF-8
    DWORD dwConvertLen = 0;
    A2UTF8((char*)m_struNetworkCfg.struPPPoE.sPPPoEUser, (char*)m_struNetworkCfg.struPPPoE.sPPPoEUser, NAME_LEN, &dwConvertLen);
    A2UTF8((char*)m_struNetworkCfg.struPPPoE.sPPPoEPassword, (char*)m_struNetworkCfg.struPPPoE.sPPPoEPassword, PASSWD_LEN, &dwConvertLen);

    NET_EHOME_CONFIG    struCfg = {0};
    struCfg.pInBuf = &m_struNetworkCfg;
    struCfg.dwInSize = sizeof(m_struNetworkCfg);

    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_NETWORK_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_NETWORK_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_NETWORK_CFG");
    }
}
void CSubDlgNetworkCfg::OnBnClickedCheckPppoeEnable()
{
    // TODO: 在此添加控件通知处理程序代码
    m_bPPPoEEnable = !m_bPPPoEEnable;
    if (m_bPPPoEEnable)
    {
        GetDlgItem(IDC_EDIT_PPPOE_USER)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_PPPOE_ADDR)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_PPPOE_PWD)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_PPPOE_USER)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_PPPOE_ADDR)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_PPPOE_PWD)->EnableWindow(FALSE);
    }
}

