// DlgPassthroughProxy.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgPassthroughProxy.h"


// CDlgPassthroughProxy 对话框
//void CALLBACK fnPassthroughDataCb(DWORD dwProxyType, LONG lLisetenHandle, void* pDeviceID, DWORD dwDevIDLen, void* pDataBuffer, DWORD dwDataLen, void* pUser)
//{
//    UN_REFERENCED_PARAMETER(dwProxyType)
//        UN_REFERENCED_PARAMETER(lLisetenHandle)
//        UN_REFERENCED_PARAMETER(dwDevIDLen)
//        UN_REFERENCED_PARAMETER(pDataBuffer)
//        UN_REFERENCED_PARAMETER(dwDataLen)
//        UN_REFERENCED_PARAMETER(pUser)
//        char sz[1024] = { 0 };
//    sprintf(sz, "deviceID[%s]", pDeviceID);
//    return;
//}

IMPLEMENT_DYNAMIC(CDlgPassthroughProxy, CDialog)

CDlgPassthroughProxy::CDlgPassthroughProxy(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPassthroughProxy::IDD, pParent)
    , m_sSDKListenIP(_T(""))
    , m_dwSDKListenPort(0)
    , m_sHTTPListenIP(_T(""))
    , m_dwHTTPListenPort(0)
    , m_lListenSDK(-1)
    , m_lListenHTTP(-1)
    , m_csHttpIn(_T(""))
    , m_csHttpOut(_T(""))
{
   
}

CDlgPassthroughProxy::~CDlgPassthroughProxy()
{
}

void CDlgPassthroughProxy::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_IP_SDK, m_sSDKListenIP);
    DDX_Text(pDX, IDC_EDIT_PORT_SDK, m_dwSDKListenPort);
    DDX_Text(pDX, IDC_EDIT_IP_HTTP, m_sHTTPListenIP);
    DDX_Text(pDX, IDC_EDIT_PORT_HTTP, m_dwHTTPListenPort);
    DDX_Text(pDX, IDC_EDIT_HTTP_IN, m_csHttpIn);
    DDX_Text(pDX, IDC_EDIT_HTTP_OUT, m_csHttpOut);
}


BEGIN_MESSAGE_MAP(CDlgPassthroughProxy, CDialog)
    ON_BN_CLICKED(IDC_BTN_START_PT_LISTEN, &CDlgPassthroughProxy::OnBnClickedBtnStartPtListen)
    ON_BN_CLICKED(IDC_BTN_STOP_PT_LISTEN, &CDlgPassthroughProxy::OnBnClickedBtnStopPtListen)
    ON_BN_CLICKED(IDC_BTN_HTTP, &CDlgPassthroughProxy::OnBnClickedBtnHttp)
END_MESSAGE_MAP()


// CDlgPassthroughProxy 消息处理程序

void CDlgPassthroughProxy::OnBnClickedBtnStartPtListen()
{
    // TODO: 在此添加控件通知处理程序代码
    char szLan[512] = {0};
    if (m_lListenSDK > -1 || m_lListenHTTP > -1)
    {
        g_StringLanType(szLan, "透传代理已开启，请先关闭", "Please stop the passthrough proxy first.");
        AfxMessageBox(szLan);
        return;
    }

    UpdateData(TRUE);
   
    NET_EHOME_PT_PARAM struPTPara = {0};
    if (m_sSDKListenIP.GetLength() != 0)
    {
        strcpy(struPTPara.struIP.szIP, m_sSDKListenIP);
        struPTPara.struIP.wPort = (WORD)m_dwSDKListenPort;
        struPTPara.byProxyType = ENUM_PROXY_TYPE_NETSDK;
        m_lListenSDK = NET_ECMS_StartListenProxy(&struPTPara); //NetSDK代理
        if (m_lListenSDK < 0)
        {
            g_StringLanType(szLan, "开启NETSDK透传代理失败", "Start NetSDK PT proxy failed.");
            AfxMessageBox(szLan);
            return;
        }
    }

    if (m_sHTTPListenIP.GetLength() != 0)
    {
        strcpy(struPTPara.struIP.szIP, m_sHTTPListenIP);
        struPTPara.struIP.wPort = (WORD)m_dwHTTPListenPort;
        struPTPara.byProxyType = ENUM_PROXY_TYPE_HTTP;
        m_lListenHTTP = NET_ECMS_StartListenProxy(&struPTPara); //HTTP代理
        if (m_lListenHTTP < 0)
        {
            g_StringLanType(szLan, "开启HTTP透传代理失败", "Start HTTP PT proxy failed.");
            AfxMessageBox(szLan);
            return;
        }
    }
}

void CDlgPassthroughProxy::OnBnClickedBtnStopPtListen()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_lListenSDK > -1)
    {
        NET_ECMS_StopListenProxy(m_lListenSDK, ENUM_PROXY_TYPE_NETSDK);
        m_lListenSDK = -1;
    }
    if (m_lListenHTTP > -1)
    {
        NET_ECMS_StopListenProxy(m_lListenHTTP, ENUM_PROXY_TYPE_HTTP);
        m_lListenHTTP = -1;
    }
}


void CDlgPassthroughProxy::OnBnClickedBtnHttp()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex == -1)
    {
        AfxMessageBox("please select a device.");
    }

    if (m_iDeviceIndex != iDeviceIndex)
    {
        m_iDeviceIndex = iDeviceIndex;
        m_iChanIndex = -1;
    }

    m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    if (m_lLoginID < 0)
    {
        AfxMessageBox("please login device first.");
    }

    NET_EHOME_HTTP_PARAM struParam = { 0 };
    char szTemp[10240] = { 0 };
    char szOut[500 * 1024] = { 0 };
    sprintf(szTemp, "%s",m_csHttpIn.GetString());
    struParam.pInBuffer = szTemp;
    struParam.dwInSize = strlen(szTemp);
    struParam.pOutBuffer = szOut;
    struParam.dwOutSize = 500 * 1024;
    struParam.dwRecvTimeOut = 5000;

    char szLan[1024] = { 0 };
    if (!NET_ECMS_HTTPConfig(m_lLoginID, &struParam))
    {
        sprintf_s(szLan, "NET_ECMS_HTTPConfig error:%d", NET_ECMS_GetLastError());
        g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, 0, szLan);
    }
    else
    {
        m_csHttpOut = szOut;
        sprintf_s(szLan, "NET_ECMS_HTTPConfig success");
        g_pMainDlg->AddLog(-1, OPERATION_SUCC_T, 0, szLan);
    }
    UpdateData(FALSE);
}
