// DlgPrivateArea.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgPrivateArea.h"
#include "afxdialogex.h"
#include "DlgMotionArea.h"


// CDlgPrivateArea 对话框

IMPLEMENT_DYNAMIC(CDlgPrivateArea, CDialogEx)

CDlgPrivateArea::CDlgPrivateArea(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPrivateArea::IDD, pParent)
{
    m_iAreaNum = 0;
}

CDlgPrivateArea::~CDlgPrivateArea()
{
}

void CDlgPrivateArea::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPrivateArea, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_GET, &CDlgPrivateArea::OnBnClickedBtnGet)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgPrivateArea::OnBnClickedBtnClear)
    ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgPrivateArea::OnBnClickedButtonSet)
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgPrivateArea 消息处理程序


void CDlgPrivateArea::OnBnClickedBtnGet()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    HideArea hideArea[4] = { 0 };
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>GetHideArea</ConfigCmd>\r\n"\
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
    m_iAreaNum = 0;

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
                if (xmlBase.FindElem("HIDEAREACFG") && xmlBase.IntoElem())
                {
                    do
                    {
                        if (xmlBase.FindElem("AREARECT") && xmlBase.IntoElem())
                        {

                            ConvertSingleNodeData(&hideArea[m_iAreaNum].iLeft, xmlBase, "AREALEFT", NODE_STRING_TO_INT);
                            ConvertSingleNodeData(&hideArea[m_iAreaNum].iTop, xmlBase, "AREATOP", NODE_STRING_TO_INT);
                            ConvertSingleNodeData(&hideArea[m_iAreaNum].iRight, xmlBase, "AREARIGHT", NODE_STRING_TO_INT);
                            ConvertSingleNodeData(&hideArea[m_iAreaNum].iBottom, xmlBase, "AREABOTTOM", NODE_STRING_TO_INT);
                            m_iAreaNum++;
                        }
                        xmlBase.OutOfElem();
                    } while (xmlBase.NextSibElem());
                    
                    CRect rect;
                    CClientDC dc(this);
                    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
                    ScreenToClient(&rect);
                    int iDifx = rect.left;
                    int iDify = rect.top;
                    m_iAreaNum ;

                    for (int i = 0; i < m_iAreaNum; i++)
                    {
                        m_startPoint[i].x = hideArea[i].iLeft * (rect.Width()) / 704 + iDifx;
                        m_endPoint[i].x = hideArea[i].iRight * (rect.Width()) / 704 + iDifx;
                        m_startPoint[i].y = hideArea[i].iTop * (rect.Height()) / 576 + iDify;
                        m_endPoint[i].y = hideArea[i].iBottom * (rect.Height()) / 576 + iDify;
                        dc.Rectangle(CRect(m_startPoint[i], m_endPoint[i]));
                    }
                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}


BOOL CDlgPrivateArea::CheckInitParam()
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

void CDlgPrivateArea::OnBnClickedBtnClear()
{
    // TODO:  在此添加控件通知处理程序代码

    int i = 0;

    Invalidate();
    for (i = 0; i < HIDE_AREA_NUM; i++)
    {
        m_startPoint[i] = 0;
        m_endPoint[i] = 0;
        m_bStartValid[i] = FALSE;
    }
    m_iAreaNum = 0;
    UpdateWindow();
}


void CDlgPrivateArea::OnBnClickedButtonSet()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    HideArea hideArea[4] = { 0 };
    CRect rect;
    int iDifx = 0;
    int iDify = 0;
    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    iDifx = rect.left;
    iDify = rect.top;

    GetDlgItem(IDC_STATIC_AREA)->GetClientRect(&rect);

    CString str;
    for (int i = 0; i < HIDE_AREA_NUM; i++)
    {
        hideArea[i].iLeft = (MEGA_MIN(m_startPoint[i].x, m_endPoint[i].x) - iDifx) * 704 / (rect.Width());
        hideArea[i].iRight = (MEGA_MAX(m_startPoint[i].x, m_endPoint[i].x) - iDifx) * 704 / (rect.Width());
        hideArea[i].iTop = (MEGA_MIN(m_startPoint[i].y, m_endPoint[i].y) - iDify) * 576 / (rect.Height());
        hideArea[i].iBottom = (MEGA_MAX(m_startPoint[i].y, m_endPoint[i].y) - iDify) * 576 / (rect.Height());
        CString strTmp;
        if (i>= m_iAreaNum)
        {
            memset(&hideArea[i], 0, sizeof(hideArea[i]));
        }
        strTmp.Format("<AREARECT>\r\n"
            "<AREALEFT>%d</AREALEFT>\r\n"
            "<AREATOP>%d</AREATOP>\r\n"
            "<AREARIGHT>%d</AREARIGHT>\r\n"
            "<AREABOTTOM>%d</AREABOTTOM>\r\n"
            "</AREARECT>\r\n", hideArea[i].iLeft, hideArea[i].iTop, hideArea[i].iRight, hideArea[i].iBottom);
        str += strTmp;
    }

    char szArea[1024] = { 0 };
    memcpy(szArea, str.GetBuffer(0), str.GetLength());
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetHideArea</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigXML>\r\n"
        "<HIDEAREACFG>\r\n"
        "%s"
        "</HIDEAREACFG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_dwChannelNo, szArea);
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

void CDlgPrivateArea::RefreshAreaNum()
{
    if (m_iAreaNum == HIDE_AREA_NUM)
    {
        OnBnClickedBtnClear();
    }

    m_iAreaNum++;

    return;
}





void CDlgPrivateArea::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    CRect rect;
    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (rect.PtInRect(point))
    {
        RefreshAreaNum();
        if (m_iAreaNum)
        {
            m_endPoint[m_iAreaNum - 1] = 0;
            m_startPoint[m_iAreaNum - 1] = point;
            m_bStartValid[m_iAreaNum - 1] = TRUE;
        }
    }
    else
    {
        if (m_iAreaNum)
        {
            m_bStartValid[m_iAreaNum - 1] = FALSE;
        }
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgPrivateArea::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    if (m_iAreaNum && m_bStartValid[m_iAreaNum - 1])
    {
        CRect rect;
        GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
        ScreenToClient(&rect);
        CClientDC dc(this);

        if (rect.PtInRect(point) && rect.PtInRect(m_startPoint[m_iAreaNum - 1]))
        {
            m_endPoint[m_iAreaNum - 1] = point;
            dc.Rectangle(CRect(m_startPoint[m_iAreaNum - 1], m_endPoint[m_iAreaNum - 1]));
        }
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}
