// DlgWirelessInfoPara.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgWirelessInfoPara.h"
#include "afxdialogex.h"

#define WIRELESS_INFO_CONFIG_XML "<WIRELESSINFOPARA>\r\n<Enable>%d</Enable>\r\n<InfoTransInterval>%d</InfoTransInterval>\r\n</WIRELESSINFOPARA>\r\n"
// DlgWirelessInfoPara 对话框

IMPLEMENT_DYNAMIC(CDlgWirelessInfoPara, CDialog)

CDlgWirelessInfoPara::CDlgWirelessInfoPara(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWirelessInfoPara::IDD, pParent)
    , m_iInternal(0)
{
    memset(&m_struWirelessInfo, 0, sizeof(m_struWirelessInfo));
}

CDlgWirelessInfoPara::~CDlgWirelessInfoPara()
{
}

void CDlgWirelessInfoPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_INTERAL_WIRELESSINFO_UPLOAD, m_iInternal);
    DDX_Check(pDX, IDC_CHECK_ENABLE1, m_bEnable);
}


BEGIN_MESSAGE_MAP(CDlgWirelessInfoPara, CDialog)
    ON_BN_CLICKED(IDC_BTN_GET_WIRELESSINFO, &CDlgWirelessInfoPara::OnBnClickedBtnGetWirelessinfo)
    ON_BN_CLICKED(IDC_BTN_SET_WIRELESSINFO, &CDlgWirelessInfoPara::OnBnClickedBtnSetWirelessinfo)
END_MESSAGE_MAP()


// DlgWirelessInfoPara 消息处理程序
BOOL CDlgWirelessInfoPara::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    m_lUserID = g_struDeviceInfo[iDeviceIndex].lLoginID;

    memset(&m_struCfg, 0, sizeof(m_struCfg));

    m_struCfg.pCmdBuf = m_szCommand;
    m_struCfg.pInBuf = m_szInputXml;
    m_struCfg.pOutBuf = m_szOutputXml;

    OnBnClickedBtnGetWirelessinfo();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}

void CDlgWirelessInfoPara::OnBnClickedBtnGetWirelessinfo()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    memset(m_szCommand, 0, 32);
    memset(m_szInputXml, 0, 1500);
    memset(m_szOutputXml, 0, 1500);

    memcpy(m_szCommand, "GETDEVICECONFIG", strlen("GETDEVICECONFIG"));
    sprintf_s(m_szInputXml, CONFIG_GET_PARAM_XML, "GetWirelessInfoPara", 0, 0, 0, 0);

    m_struCfg.dwCmdLen = strlen(m_szCommand);
    m_struCfg.dwInSize = strlen(m_szInputXml);
    m_struCfg.dwOutSize = MAX_EHOME_PROTOCOL_LEN;

    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)m_struCfg.pCmdBuf, (char*)m_struCfg.pCmdBuf, sizeof(m_szCommand), &m_struCfg.dwCmdLen);
    A2UTF8((char*)m_struCfg.pInBuf, (char*)m_struCfg.pInBuf, MAX_EHOME_PROTOCOL_LEN, &m_struCfg.dwInSize);

    if (!NET_ECMS_XMLConfig(m_lUserID, &m_struCfg, sizeof(m_struCfg)))
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "Get Wireless Info");
    }

    CXmlBase xmlWirelessInfo;
    xmlWirelessInfo.Parse((char*)m_struCfg.pOutBuf);
    if (xmlWirelessInfo.FindElem("Params") && xmlWirelessInfo.IntoElem())
    {
        if (xmlWirelessInfo.FindElem("ConfigXML") && xmlWirelessInfo.IntoElem())
        {
            if (xmlWirelessInfo.FindElem("WIRELESSINFOPARA") && xmlWirelessInfo.IntoElem())
            {
                ConvertSingleNodeData(&m_struWirelessInfo.byEnable, xmlWirelessInfo, "Enable", NODE_STRING_TO_BYTE);
                ConvertSingleNodeData(&m_struWirelessInfo.dwInfoTransInterval, xmlWirelessInfo, "InfoTransInterval", NODE_STRING_TO_INT);
  
            }
        }
    }

    m_iInternal = m_struWirelessInfo.dwInfoTransInterval;
    m_bEnable = m_struWirelessInfo.byEnable;
    UpdateData(FALSE);
}


void CDlgWirelessInfoPara::OnBnClickedBtnSetWirelessinfo()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    m_struWirelessInfo.dwInfoTransInterval = m_iInternal;
    m_struWirelessInfo.byEnable = m_bEnable;

    memset(m_szCommand, 0, 32);
    memset(m_szInputXml, 0, 1500);
    memset(m_szOutputXml, 0, 1500);

    memcpy(m_szCommand, "SETDEVICECONFIG", strlen("SETDEVICECONFIG"));
    char sInput[1500] = { 0 };
    sprintf_s(sInput, WIRELESS_INFO_CONFIG_XML, m_struWirelessInfo.byEnable,m_struWirelessInfo.dwInfoTransInterval);
    sprintf_s(m_szInputXml, CONFIG_SET_PARAM_XML, "SetWirelessInfoPara", 0, 0, 0, 0, sInput);
   
    m_struCfg.dwCmdLen = strlen(m_szCommand);
    m_struCfg.dwInSize = strlen(m_szInputXml);
    m_struCfg.dwOutSize = MAX_EHOME_PROTOCOL_LEN;

    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)m_struCfg.pCmdBuf, (char*)m_struCfg.pCmdBuf, sizeof(m_szCommand), &m_struCfg.dwCmdLen);
    A2UTF8((char*)m_struCfg.pInBuf, (char*)m_struCfg.pInBuf, MAX_EHOME_PROTOCOL_LEN, &m_struCfg.dwInSize);

    if (!NET_ECMS_XMLConfig(m_lUserID, &m_struCfg, sizeof(m_struCfg)))
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "Set Wireless Info");
    }
}


