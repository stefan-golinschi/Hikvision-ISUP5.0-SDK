// DlgHidArea.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgHidArea.h"
#include "afxdialogex.h"
#include "DlgMotionArea.h"


// CDlgHidArea 对话框

IMPLEMENT_DYNAMIC(CDlgHidArea, CDialog)

CDlgHidArea::CDlgHidArea(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHidArea::IDD, pParent)
    , m_bStartValid(FALSE)
    , m_bEndValid(FALSE)
    , m_bClear(TRUE)
{

}

CDlgHidArea::~CDlgHidArea()
{
}

void CDlgHidArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHidArea, CDialog)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgHidArea::OnBnClickedBtnClear)
    ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgHidArea::OnBnClickedButtonSet)
    ON_BN_CLICKED(IDC_BTN_GET, &CDlgHidArea::OnBnClickedBtnGet)
END_MESSAGE_MAP()


// CDlgHidArea 消息处理程序





BOOL CDlgHidArea::CheckInitParam()
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

void CDlgHidArea::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值

    CRect rect;
    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (rect.PtInRect(point))
    {
        m_endPoint = 0;
        m_startPoint = point;
        m_bStartValid = TRUE;
        Invalidate();
    }
    else
    {
        m_bStartValid = FALSE;
    }

    CDialog::OnLButtonDown(nFlags, point);
}

void CDlgHidArea::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值


    if (m_bStartValid)
    {
        CRect rect;
        GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
        ScreenToClient(&rect);
        CClientDC dc(this);

        if (rect.PtInRect(point) && rect.PtInRect(m_startPoint))
        {
            m_endPoint = point;
            dc.Rectangle(CRect(m_startPoint, m_endPoint));
            m_bEndValid = TRUE;
            m_bClear = FALSE;
        }
        else
        {
            m_bEndValid = FALSE;
        }
    }
    CDialog::OnLButtonUp(nFlags, point);
}


void CDlgHidArea::OnBnClickedBtnClear()
{
    // TODO:  在此添加控件通知处理程序代码

    Invalidate();
    m_startPoint = 0;
    m_endPoint = 0;
    m_bClear = TRUE;
    m_bEndValid = FALSE;
}


void CDlgHidArea::OnBnClickedButtonSet()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    CRect rect;
    int iDifx = 0;
    int iDify = 0;
    int iLeft, iTop, iRight, iBottom;
    if (m_bEndValid)
    {
        GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
        ScreenToClient(&rect);
        iDifx = rect.left;
        iDify = rect.top;

        GetDlgItem(IDC_STATIC_AREA)->GetClientRect(&rect);
        iLeft = (MEGA_MIN(m_startPoint.x, m_endPoint.x) - iDifx) * 704 / (rect.Width());
        iRight = (MEGA_MAX(m_startPoint.x, m_endPoint.x) - iDifx) * 704 / (rect.Width());
        iTop = (MEGA_MIN(m_startPoint.y, m_endPoint.y) - iDify) * 576 / (rect.Height());
        iBottom = (MEGA_MAX(m_startPoint.y, m_endPoint.y) - iDify) * 576 / (rect.Height());
    }
    else
    {
        iLeft = 0;
        iRight = 0;
        iTop = 0;
        iBottom = 0;
    }

    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetHideAlarmArea</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigXML>\r\n"
        "<HIDEALARMAREACFG>\r\n"
        "<AREARECT>\r\n"
        "<AREALEFT>%d</AREALEFT>\r\n"
        "<AREATOP>%d</AREATOP>\r\n"
        "<AREARIGHT>%d</AREARIGHT>\r\n"
        "<AREABOTTOM>%d</AREABOTTOM>\r\n"
        "</AREARECT>\r\n"
        "</HIDEALARMAREACFG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_dwChannelNo, iLeft,iTop,iRight,iBottom);
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


void CDlgHidArea::OnBnClickedBtnGet()
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
        "<ConfigCmd>GetHideAlarmArea</ConfigCmd>\r\n"\
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
                if (xmlBase.FindElem("HIDEALARMAREACFG") && xmlBase.IntoElem())
                {
                    int iLeft, iTop, iRight, iBottom;
                    if (xmlBase.FindElem("AREARECT") && xmlBase.IntoElem())
                    {
                        
                        ConvertSingleNodeData(&iLeft, xmlBase, "AREALEFT", NODE_STRING_TO_INT);
                        ConvertSingleNodeData(&iTop, xmlBase, "AREATOP", NODE_STRING_TO_INT);
                        ConvertSingleNodeData(&iRight, xmlBase, "AREARIGHT", NODE_STRING_TO_INT);
                        ConvertSingleNodeData(&iBottom, xmlBase, "AREABOTTOM", NODE_STRING_TO_INT);
                    }
                    CRect rect;
                    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
                    ScreenToClient(&rect);
                    int iDifx = rect.left;
                    int iDify = rect.top;

                    m_startPoint.x = iLeft * (rect.Width()) / 704 + iDifx;
                    m_endPoint.x = iRight * (rect.Width()) / 704 + iDifx;
                    m_startPoint.y = iTop * (rect.Height()) / 576 + iDify;
                    m_endPoint.y = iBottom * (rect.Height()) / 576 + iDify;
                    CClientDC dc(this);
                    dc.Rectangle(CRect(m_startPoint, m_endPoint));
                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}
