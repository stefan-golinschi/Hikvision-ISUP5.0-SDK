// DlgGpsInfo.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgGpsInfo.h"

//#define GPS_CONFIG_XML "%s<ConfigXML>\r\n<GPSINFOPARA>\r\n<GPSTransInterval>%d</GPSTransInterval>\r\n<MaxSpeed>%d</MaxSpeed>\r\n<MinSpeed>%d</MinSpeed>\r\n</GPSINFOPARA>\r\n</ConfigXML>"
#define GPS_CONFIG_XML "<GPSINFOPARA>\r\n<GPSTransInterval>%d</GPSTransInterval>\r\n<Enable>%d</Enable>\r\n</GPSINFOPARA>\r\n"

IMPLEMENT_DYNAMIC(CDlgGpsInfo, CDialog)

CDlgGpsInfo::CDlgGpsInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGpsInfo::IDD, pParent)
    , m_iInteral(0)
    , m_iMaxSpeed(0)
    , m_iMinSpeed(0)
{

}

CDlgGpsInfo::~CDlgGpsInfo()
{
}

void CDlgGpsInfo::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_INTERAL, m_iInteral);
    DDX_Text(pDX, IDC_EDIT_MAXSPEED, m_iMaxSpeed);
    DDX_Text(pDX, IDC_EDIT_MINSPEED, m_iMinSpeed);
    DDX_Check(pDX, IDC_CHECK_ENABLE, m_bEnable);
}


BEGIN_MESSAGE_MAP(CDlgGpsInfo, CDialog)
    ON_BN_CLICKED(IDC_BTN_GET, &CDlgGpsInfo::OnBnClickedBtnGet)
    ON_BN_CLICKED(IDC_BTN_SET, &CDlgGpsInfo::OnBnClickedBtnSet)
END_MESSAGE_MAP()


// CDlgGpsInfo 消息处理程序


void CDlgGpsInfo::OnBnClickedBtnGet()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    memset(m_szCommand, 0, 32);
    memset(m_szInputXml, 0, 1500);
    memset(m_szOutputXml, 0, 1500);

    memcpy(m_szCommand, "GETDEVICECONFIG", strlen("GETDEVICECONFIG"));
    sprintf_s(m_szInputXml, CONFIG_GET_PARAM_XML, "GetGPSInfoPara", 0, 0, 0, 0);

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
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "Get GPS Info");
    }

    CXmlBase xmlGPSInfo;
    xmlGPSInfo.Parse((char*)m_struCfg.pOutBuf);
    if (xmlGPSInfo.FindElem("Params") && xmlGPSInfo.IntoElem())
    {
        if (xmlGPSInfo.FindElem("ConfigXML") && xmlGPSInfo.IntoElem())
        {
            if (xmlGPSInfo.FindElem("GPSINFOPARA") && xmlGPSInfo.IntoElem())
            {
                ConvertSingleNodeData(&m_struGPSInfo.iInteralTime, xmlGPSInfo, "GPSTransInterval", NODE_STRING_TO_INT);
                ConvertSingleNodeData(&m_struGPSInfo.bEnable, xmlGPSInfo, "Enable", NODE_STRING_TO_BYTE);
                //ConvertSingleNodeData(&m_struGPSInfo.iMaxSpeed, xmlGPSInfo, "MaxSpeed", NODE_STRING_TO_INT);
                //ConvertSingleNodeData(&m_struGPSInfo.iMinSpeed, xmlGPSInfo, "MinSpeed", NODE_STRING_TO_INT);
            }
        }
    }

    m_iInteral = m_struGPSInfo.iInteralTime;
    m_iMaxSpeed = m_struGPSInfo.iMaxSpeed;
    m_iMinSpeed = m_struGPSInfo.iMinSpeed;
    m_bEnable = m_struGPSInfo.bEnable - 1;
    UpdateData(FALSE);
}


void CDlgGpsInfo::OnBnClickedBtnSet()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    m_struGPSInfo.iInteralTime = m_iInteral;
    m_struGPSInfo.iMaxSpeed = m_iMaxSpeed;
    m_struGPSInfo.iMinSpeed = m_iMinSpeed;
    m_struGPSInfo.bEnable = m_bEnable;
    memset(m_szCommand, 0, 32);
    memset(m_szInputXml, 0, 1500);
    memset(m_szOutputXml, 0, 1500);

    memcpy(m_szCommand, "SETDEVICECONFIG", strlen("SETDEVICECONFIG"));
    char sInput[1500] = { 0 };
    sprintf_s(sInput, GPS_CONFIG_XML, m_struGPSInfo.iInteralTime,m_struGPSInfo.bEnable+1);
    sprintf_s(m_szInputXml, CONFIG_SET_PARAM_XML, "SetGPSInfoPara", 0, 0, 0, 0, sInput);
    //sprintf_s(m_szInputXml, GPS_CONFIG_XML, CONFIG_PARAM_XML, m_struGPSInfo.iInteralTime, m_struGPSInfo.iMaxSpeed, m_struGPSInfo.iMinSpeed);



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
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "Set GPS Info");
    }
}

BOOL CDlgGpsInfo::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    m_lUserID = g_struDeviceInfo[iDeviceIndex].lLoginID;

    memset(&m_struCfg, 0, sizeof(m_struCfg));

    m_struCfg.pCmdBuf = m_szCommand;
    m_struCfg.pInBuf = m_szInputXml;
    m_struCfg.pOutBuf = m_szOutputXml;

    OnBnClickedBtnGet();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
