// DlgCaptureCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgCaptureCfg.h"
#include "afxdialogex.h"
#include "DlgAlarmChanList.h"


// CDlgCaptureCfg 对话框

IMPLEMENT_DYNAMIC(CDlgCaptureCfg, CDialogEx)

CDlgCaptureCfg::CDlgCaptureCfg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCaptureCfg::IDD, pParent)
    , m_iAlarmType(0)
    , m_iAlarmChan(1)
    , m_iSendTo(0)
    , m_bCap(FALSE)
    , m_iResolution(0)
    , m_iQuality(0)
    , m_iTime(0)
    , m_iAlo(0)
    , m_iDig(0)
{
    memset(m_arrDigChan, 0, sizeof(m_arrDigChan));
    memset(m_arrAloChan, 0, sizeof(m_arrAloChan));
    m_iDigNum = 0;
    m_iAloNum = 0;

}

CDlgCaptureCfg::~CDlgCaptureCfg()
{
}

void CDlgCaptureCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO_ALARM_TYPE, m_iAlarmType);
    DDX_Text(pDX, IDC_EDIT_ALARM_CHAN, m_iAlarmChan);
    DDX_CBIndex(pDX, IDC_COMBO_ALARM_TYPE2, m_iSendTo);
    DDX_Check(pDX, IDC_CHECK_CAP, m_bCap);
    DDX_CBIndex(pDX, IDC_COMBO_ALARM_TYPE3, m_iResolution);
    DDX_CBIndex(pDX, IDC_COMBO_ALARM_TYPE4, m_iQuality);
    DDX_CBIndex(pDX, IDC_COMBO_ALARM_TYPE5, m_iTime);
    DDX_Text(pDX, IDC_EDIT_ANALOGCHAN_NUM, m_iAlo);
    DDX_Text(pDX, IDC_EDIT_DIGITALCHAN_NUM, m_iDig);
    DDX_Control(pDX, IDC_COMBO_ALARM_TYPE2, m_cmbSendTo);
}


BEGIN_MESSAGE_MAP(CDlgCaptureCfg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_ANALOGCHAN, &CDlgCaptureCfg::OnBnClickedButtonAnalogchan)
    ON_BN_CLICKED(IDC_BUTTON_DIGITALCHAN, &CDlgCaptureCfg::OnBnClickedButtonDigitalchan)
    ON_BN_CLICKED(IDC_BTN_GET_CAP, &CDlgCaptureCfg::OnBnClickedBtnGetCap)
    ON_BN_CLICKED(IDC_BTN_CAP_SET, &CDlgCaptureCfg::OnBnClickedBtnCapSet)
END_MESSAGE_MAP()


// CDlgCaptureCfg 消息处理程序


BOOL CDlgCaptureCfg::CheckInitParam()
{
    // TODO:  在此添加额外的初始化
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
    m_iChanIndex = g_pMainDlg->GetCurChanIndex();
    if (m_iChanIndex < 0)
    {
        //if (m_dwChannelNo < 0)
        {
            g_StringLanType(szLan, "请选择一个通道!", "please select a channel first!");
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, szLan);
            return FALSE;
        }
    }
    else
    {
        m_dwChannelNo = m_iChanIndex + 1;
    }

    return TRUE;
}

void CDlgCaptureCfg::OnBnClickedButtonAnalogchan()
{
    // TODO:  在此添加控件通知处理程序代码
    CDlgAlarmChanList dlgList;
    char szDlgTitle[1024] = { 0 };
    g_StringLanType(szDlgTitle, _T("模拟通道"), _T("Analog Channel"));
    dlgList.m_strDlgTitle = szDlgTitle;
    dlgList.m_byDataType = 1;
    dlgList.m_byNum = m_iAloNum;
    memcpy(dlgList.m_byAnalogAlarmChan, m_arrAloChan, m_iAloNum);
    if (dlgList.DoModal() != IDOK)
    {
        return;
    }
    memcpy(m_arrAloChan, dlgList.m_byAnalogAlarmChan, m_iAloNum);
}


void CDlgCaptureCfg::OnBnClickedButtonDigitalchan()
{
    // TODO:  在此添加控件通知处理程序代码

    CDlgAlarmChanList dlgList;
    dlgList.m_dwDigitalAlarmChan = m_iDigNum;
    char szDlgTitle[1024] = { 0 };
    g_StringLanType(szDlgTitle, _T("数字通道"), _T("Digital Channel"));
    dlgList.m_strDlgTitle = szDlgTitle;
    dlgList.m_byDataType = 2;
    dlgList.m_byNum = (BYTE)m_iDigNum;
    memcpy(dlgList.m_byDigitalAlarmChan, m_arrDigChan, m_iDigNum);
    if (dlgList.DoModal() != IDOK)
    {
        return;
    }

    memcpy(m_arrDigChan, dlgList.m_byDigitalAlarmChan, m_iDigNum);
}


void CDlgCaptureCfg::OnBnClickedBtnGetCap()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    m_iAloNum = 0;
    m_iDigNum = 0;
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>GetAlarmTriggerCapture</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigParam2>%d</ConfigParam2>\r\n"\
        "</Params>\r\n", m_iAlarmType,m_iAlarmChan);
    char szCmd[32] = "GETDEVICECONFIG";
    char szOutBuffer[1024 * 100] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024 * 100] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024 * 100;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");

    char *pTmp = szOutBuffer + strlen(szOutBuffer) - 2;
    if (strcmp(pTmp, "\r\n") != 0)
    {
        memcpy(szOutBuffer + strlen(szOutBuffer), "\r\n", strlen("\r\n"));
    }
    CXmlBase    xmlBase;
    if (xmlBase.Parse(szOutBuffer) != NULL)
    {

        if (xmlBase.FindElem("Params") && xmlBase.IntoElem())
        {
            if (xmlBase.FindElem("ConfigXML") && xmlBase.IntoElem())
            {
                if (xmlBase.FindElem("CAPTURECONFIG") && xmlBase.IntoElem())
                {
                    ConvertSingleNodeData(&m_bCap, xmlBase, "UseCapture", NODE_STRING_TO_INT);
                    ConvertSingleNodeData(&m_iSendTo, xmlBase, "SendTo", NODE_STRING_TO_INT);
                    ConvertSingleNodeData(&m_iResolution, xmlBase, "Resolution", NODE_STRING_TO_INT);
                    ConvertSingleNodeData(&m_iQuality, xmlBase, "Quality", NODE_STRING_TO_INT);
                    int iSendTo = 0;
                    ConvertSingleNodeData(&m_iTime, xmlBase, "Frequency", NODE_STRING_TO_INT);
                    
                    m_cmbSendTo.SetCurSel(m_iSendTo - 1);
                    if (xmlBase.FindElem("CAPTURECHAN") && xmlBase.IntoElem())
                    {
                        char szNodeName[32] = { 0 };
                        for (int i = 0; i < 1024; i++)
                        {
                            sprintf(szNodeName, "AnalogChan%d", (i + 1));
                            if (ConvertSingleNodeData(&m_arrAloChan[i], xmlBase, szNodeName, NODE_STRING_TO_BYTE))
                            {
                                m_iAloNum++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        memset(szNodeName, 0, 32);
                        for (int i = 0; i < 1024; i++)
                        {
                            sprintf(szNodeName, "DigitChan%d", (i + 1));
                            if (ConvertSingleNodeData(&m_arrDigChan[i], xmlBase, szNodeName, NODE_STRING_TO_BYTE))
                            {
                                m_iDigNum++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        xmlBase.OutOfElem();
                    }
                    m_iSendTo -= 1;
                    if (m_iQuality == 40)
                    {
                        m_iQuality = 0;
                    }
                    else if (m_iQuality == 60)
                    {
                        m_iQuality = 1;
                    }
                    else if (m_iQuality == 80)
                    {
                        m_iQuality = 2;
                    }
                    m_iAlo = m_iAloNum;
                    m_iDig = m_iDigNum;
                    m_iTime -= 1;
                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}


void CDlgCaptureCfg::OnBnClickedBtnCapSet()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[5*1024] = { 0 };
    CString str;
    for (int i = 0; i < m_iAloNum; i++)
    {
        CString strTemp;
        strTemp.Format("<AnalogChan%d>%d</AnalogChan%d>\r\n", i + 1, m_arrAloChan[i], i + 1);
        str += strTemp;
    }
    for (int i = 0; i < m_iDigNum; i++)
    {
        CString strTemp;
        strTemp.Format("<DigitChan%d>%d</DigitChan%d>\r\n", i + 1, m_arrDigChan[i], i + 1);
        str += strTemp;
    }
    if (m_iQuality == 0)
    {
        m_iQuality = 40;
    }
    else if (m_iQuality == 1)
    {
        m_iQuality = 60;
    }
    else if (m_iQuality == 2)
    {
        m_iQuality = 80;
    }

    int iSendTo = m_cmbSendTo.GetCurSel() + 1;
    char szChan[1024 * 100] = { 0 };
    memcpy(szChan, str.GetBuffer(0), str.GetLength());
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetAlarmTriggerCapture</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigParam2>%d</ConfigParam2>\r\n"\
        "<ConfigXML>\r\n"
        "<CAPTURECONFIG>\r\n"
        "<UseCapture>%d</UseCapture>\r\n"
        "<SendTo>%d</SendTo>\r\n"
        "<Interval>10</Interval>\r\n"
        "<Resolution>%d</Resolution>\r\n"
        "<Quality>%d</Quality>\r\n"
        "<Frequency>%d</Frequency>\r\n"
        "<CAPTURECHAN>\r\n"
        "%s"
        "</CAPTURECHAN>\r\n"
        "</CAPTURECONFIG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_iAlarmType, m_iAlarmChan, m_bCap, iSendTo, m_iResolution, m_iQuality, m_iTime + 1, szChan);
    char szCmd[32] = "SETDEVICECONFIG";
    char szOutBuffer[1024 * 100] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024 * 100] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024 * 100;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
}


BOOL CDlgCaptureCfg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    OnBnClickedBtnGetCap();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
