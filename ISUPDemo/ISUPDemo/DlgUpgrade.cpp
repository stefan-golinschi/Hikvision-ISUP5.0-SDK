// DlgUpgrade.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgUpgrade.h"
#include "afxdialogex.h"
#include "Public/Convert/Convert.h"

// CDlgUpgrade 对话框

IMPLEMENT_DYNAMIC(CDlgUpgrade, CDialog)

CDlgUpgrade::CDlgUpgrade(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpgrade::IDD, pParent)
    , m_csPassword(_T(""))
    , m_dwPort(0)
    , m_csUserName(_T(""))
    , m_csFileName(_T(""))
    , m_iChannel(1)
    , m_lLoginID(0)
    , m_iDevIndex(0)
    , m_csSubDevIndex(_T(""))
    , m_strDevType(_T(""))
{

}

CDlgUpgrade::~CDlgUpgrade()
{
}

void CDlgUpgrade::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_csPassword);
    DDX_Text(pDX, IDC_EDIT_PORT, m_dwPort);
    DDX_Text(pDX, IDC_EDIT_USERNAME, m_csUserName);
    DDX_Text(pDX, IDC_EDIT_UPGRADE_FILENAME, m_csFileName);
    DDX_Text(pDX, IDC_EDIT_CHANNEL, m_iChannel);
    DDX_Control(pDX, IDC_UPGRADE_SERVER_IP, m_IPServer);
    DDX_Control(pDX, IDC_COMBO_DEV_TYPE, m_cmbDevType);
    DDX_Text(pDX, IDC_EDIT_DEV_INDEX, m_iDevIndex);
    DDX_Text(pDX, IDC_EDIT_SUBDEV_INDEX, m_csSubDevIndex);
}


BEGIN_MESSAGE_MAP(CDlgUpgrade, CDialog)
    ON_BN_CLICKED(IDOK, &CDlgUpgrade::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgUpgrade 消息处理程序


void CDlgUpgrade::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    DWORD dwIP = 0;
    char sServerIP[16] = { 0 };
    m_IPServer.GetAddress(dwIP);
    memcpy(sServerIP, IPToStr(dwIP), 16);

    SetDevTypeValue();

    CString m_strInputXml;
    m_strInputXml.Format("<?xml version = \"1.0\" encoding = \"UTF-8\"?>\r\n"\
        "<PPVSPMessage>\r\n"\
        "<Version>4.0</Version>\r\n"\
        "<Sequence>1</Sequence>\r\n"\
        "<CommandType>REQUEST</CommandType>\r\n"\
        "<Method>CONTROL</Method>\r\n"\
        "<Command>UPDATE</Command>\r\n"\
        "<Params>\r\n"\
        "<FTPServerIP>%s</FTPServerIP>\r\n"\
        "<FTPServerPort>%d</FTPServerPort>\r\n"\
        "<Account>%s</Account>\r\n"\
        "<Password>%s</Password>\r\n"\
        "<File>%s</File>\r\n"\
        "<Channel>%d</Channel>\r\n"\
        "<Type>%s</Type>\r\n"\
        "<IndexID>%d</IndexID>\r\n"\
        "<DevIndex>%s</DevIndex>\r\n"\
        "</Params>\r\n"\
        "</PPVSPMessage>\r\n", sServerIP, m_dwPort, m_csUserName, m_csPassword, m_csFileName, m_iChannel, m_strDevType, m_iDevIndex, m_csSubDevIndex);

    NET_EHOME_XML_REMOTE_CTRL_PARAM struXMLCfg = { 0 };
    memset(&struXMLCfg, 0, sizeof(struXMLCfg));
    char szCmd[32] = { 0 };
    DWORD dwCmdLen = 0;
    strcpy(szCmd, "UPDATE");
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
        MessageBox("Start upgrade successful");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLRemoteControl");
        MessageBox("Start upgrade false");
    }
}

void CDlgUpgrade::SetDevTypeValue()
{
    UpdateData(TRUE);
    switch (m_cmbDevType.GetCurSel())
    {
    case 0:
        m_strDevType = _T("");
        break;
    case 1:
        m_strDevType = _T("cardReader");//485读卡器
        break;
    case 2:
        m_strDevType = _T("netReader");//网络读卡器
        break;
    default:
        break;
    }
}

BOOL CDlgUpgrade::CheckInitParam()
{
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
    return TRUE;
}


BOOL CDlgUpgrade::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CheckInitParam();
    m_cmbDevType.SetCurSel(0);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
