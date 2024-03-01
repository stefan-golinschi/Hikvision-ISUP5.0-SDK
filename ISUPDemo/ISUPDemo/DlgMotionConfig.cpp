// DlgMotionConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgMotionConfig.h"
#include "afxdialogex.h"
#include "Public/TinyXML/XmlBase.h"


// CDlgMotionConfig 对话框

IMPLEMENT_DYNAMIC(CDlgMotionConfig, CDialogEx)

CDlgMotionConfig::CDlgMotionConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMotionConfig::IDD, pParent)
    , m_bUseMonition(FALSE)
    , m_iSensitive(0)
    , m_iHideSensive(0)
    , m_bVideoLost(FALSE)
    , m_bPrivate(FALSE)
{

}

CDlgMotionConfig::~CDlgMotionConfig()
{
}

void CDlgMotionConfig::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_SENSIVE, m_bUseMonition);
    DDX_CBIndex(pDX, IDC_COMBO_SENSIVE, m_iSensitive);
    DDX_CBIndex(pDX, IDC_COMBO_SENSIVE_HIDE, m_iHideSensive);
    DDX_Check(pDX, IDC_CHECK_VIDEO_LOST, m_bVideoLost);
    DDX_Check(pDX, IDC_CHECK_PRIVATE, m_bPrivate);
}


BEGIN_MESSAGE_MAP(CDlgMotionConfig, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_SET, &CDlgMotionConfig::OnBnClickedBtnSet)
    ON_BN_CLICKED(IDC_BTN_SET_HID, &CDlgMotionConfig::OnBnClickedBtnSetHid)
    ON_BN_CLICKED(IDC_BTN_GET, &CDlgMotionConfig::OnBnClickedBtnGet)
    ON_BN_CLICKED(IDC_BTN_GET_HID, &CDlgMotionConfig::OnBnClickedBtnGetHid)
    ON_BN_CLICKED(IDC_BTN_SET_VIDEO, &CDlgMotionConfig::OnBnClickedBtnSetVideo)
    ON_BN_CLICKED(IDC_BTN_GET_VIDEO, &CDlgMotionConfig::OnBnClickedBtnGetVideo)
    ON_BN_CLICKED(IDC_BTN_SET_PRIVATE, &CDlgMotionConfig::OnBnClickedBtnSetPrivate)
    ON_BN_CLICKED(IDC_BTN_GET_PRIVATE, &CDlgMotionConfig::OnBnClickedBtnGetPrivate)
END_MESSAGE_MAP()


// CDlgMotionConfig 消息处理程序


void CDlgMotionConfig::OnBnClickedBtnSet()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    int iSensitive = m_iSensitive - 1;
    if (iSensitive == -1)
    {
        iSensitive = 255;
    }
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetMotionPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigXML>\r\n"
        "<MOTIONCFG>\r\n"
        "<IsUseMotion>%d</IsUseMotion>\r\n"
        "<Sensitive>%d</Sensitive>\r\n"
        "</MOTIONCFG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>",m_dwChannelNo,m_bUseMonition,iSensitive);
    char szCmd[32] = "SETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
}


BOOL CDlgMotionConfig::CheckInitParam()
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


void CDlgMotionConfig::OnBnClickedBtnSetHid()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetHideAlarmPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigXML>\r\n"
        "<HIDEALARMCFG>\r\n"
        "<Sensitive>%d</Sensitive>\r\n"
        "</HIDEALARMCFG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_dwChannelNo,m_iHideSensive);
    char szCmd[32] = "SETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
}


void CDlgMotionConfig::OnBnClickedBtnGet()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>GetMotionPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "</Params>", m_dwChannelNo);
    char szCmd[32] = "GETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

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
        if (xmlBase.FindElem("Params"))
        {
            xmlBase.IntoElem();
            if (xmlBase.FindElem("ConfigXML") && xmlBase.IntoElem())
            {
                if (xmlBase.FindElem("MOTIONCFG") && xmlBase.IntoElem())
                {
                    ConvertSingleNodeData(&m_bUseMonition, xmlBase, "IsUseMotion", NODE_STRING_TO_INT);
                    ConvertSingleNodeData(&m_iSensitive, xmlBase, "Sensitive", NODE_STRING_TO_INT);
                    if (m_iSensitive == 255)
                    {
                        m_iSensitive = 0;
                    }
                    else
                    {
                        m_iSensitive +=1;
                    }
                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}


void CDlgMotionConfig::OnBnClickedBtnGetHid()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>GetHideAlarmPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "</Params>", m_dwChannelNo);
    char szCmd[32] = "GETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

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
                if (xmlBase.FindElem("HIDEALARMCFG") && xmlBase.IntoElem())
                {
                    ConvertSingleNodeData(&m_iHideSensive, xmlBase, "Sensitive", NODE_STRING_TO_INT);
                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}


void CDlgMotionConfig::OnBnClickedBtnSetVideo()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetVILostPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigXML>\r\n"
        "<VILOSTCFG>\r\n"
        "<IsUseVILost>%d</IsUseVILost>\r\n"
        "</VILOSTCFG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_dwChannelNo, m_bVideoLost);
    char szCmd[32] = "SETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
}


void CDlgMotionConfig::OnBnClickedBtnGetVideo()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>GetVILostPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "</Params>", m_dwChannelNo);
    char szCmd[32] = "GETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

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
                if (xmlBase.FindElem("VILOSTCFG") && xmlBase.IntoElem())
                {
                    ConvertSingleNodeData(&m_bVideoLost, xmlBase, "IsUseVILost", NODE_STRING_TO_INT);
                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}


void CDlgMotionConfig::OnBnClickedBtnSetPrivate()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"\
        "<ConfigCmd>SetHidePara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigXML>\r\n"
        "<HIDECFG>\r\n"
        "<IsUseHide>%d</IsUseHide>\r\n"
        "</HIDECFG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_dwChannelNo, m_bPrivate);
    char szCmd[32] = "SETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
}


void CDlgMotionConfig::OnBnClickedBtnGetPrivate()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>GetHidePara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "</Params>", m_dwChannelNo);
    char szCmd[32] = "GETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

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
                if (xmlBase.FindElem("HIDECFG") && xmlBase.IntoElem())
                {
                    ConvertSingleNodeData(&m_bPrivate, xmlBase, "IsUseHide", NODE_STRING_TO_INT);
                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}
