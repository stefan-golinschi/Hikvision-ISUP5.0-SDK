// DlgMotionArea.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgMotionArea.h"
#include "afxdialogex.h"



// CDlgMotionArea 对话框

IMPLEMENT_DYNAMIC(CDlgMotionArea, CDialog)

CDlgMotionArea::CDlgMotionArea(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMotionArea::IDD, pParent)
{

}

CDlgMotionArea::~CDlgMotionArea()
{
}

void CDlgMotionArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMotionArea, CDialog)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_BN_CLICKED(IDC_BUTTON_SET, &CDlgMotionArea::OnBnClickedButtonSet)
    ON_BN_CLICKED(IDC_BTN_GET, &CDlgMotionArea::OnBnClickedBtnGet)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgMotionArea::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CDlgMotionArea 消息处理程序



BOOL CDlgMotionArea::CheckInitParam()
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

void CDlgMotionArea::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值

    CRect rect;
    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (rect.PtInRect(point))
    {
        if (m_iAreaNum >= MOTION_AREA_NUM)
        {
            OnBnClickedBtnClear();
        }

        m_endPoint = 0;
        m_startPoint = point;
        m_bStartValid = TRUE;
    }
    else
    {
        m_bStartValid = FALSE;
    }
   
    CDialog::OnLButtonDown(nFlags, point);
}


void CDlgMotionArea::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    do
    {
        if (m_bStartValid)
        {
            CRect rect;
            GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
            ScreenToClient(&rect);
            if (rect.PtInRect(point) && rect.PtInRect(m_startPoint))
            {
                if (point.x == m_startPoint.x
                    || point.y == m_startPoint.y)
                {
                    m_endPoint = 0;
                    m_bStartValid = FALSE;
                    break;
                }
                m_endPoint = point;
                DrawPoint(m_startPoint, m_endPoint);
                m_iAreaNum++;
            }
        }

    } while (0);

    CDialog::OnLButtonUp(nFlags, point);
}


void CDlgMotionArea::OnBnClickedButtonSet()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    CString str;
    for (int i = 0; i < ROW; i++)
    {
        CString strTmp;
        strTmp.Format("<Mask>%d</Mask>\r\n", m_aiArea[i]);
        str += strTmp;
    }
    char szMask[1024] = { 0 };
    memcpy(szMask, str.GetBuffer(0), str.GetLength());
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetMotionArea</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigXML>\r\n"
        "<MOTIONAREACFG>\r\n"
        "<Row>18</Row>\r\n"
        "<BlockPerRow>22</BlockPerRow>\r\n"
        "<PictureWidth>0</PictureWidth>\r\n"
        "<PictureHeight>0</PictureHeight>\r\n"
        "<AREAS>\r\n"
        "%s"
        "</AREAS>\r\n"
        "</MOTIONAREACFG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_dwChannelNo, szMask);
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


BOOL CDlgMotionArea::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CRect rect;
    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    rect.bottom = rect.top + ROW * 16;
    rect.right = rect.left + BlockPerRow * 16;

    GetDlgItem(IDC_STATIC_AREA)->MoveWindow(&rect, TRUE);

    memset(m_aiArea, 0, sizeof(unsigned int)*ROW);
    m_startPoint = 0;
    m_endPoint = 0;
    m_bStartValid = FALSE;
    m_iAreaNum = 0;

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}

void CDlgMotionArea::DrawPoint(CPoint &A, CPoint &B)
{
    CRect rect;
    int iDifx = 0;
    int iDify = 0;
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;
    CClientDC dc(this);
    int i = 0;
    int j = 0;
    CPoint start;
    CPoint stop;

    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
    ScreenToClient(&rect);
    iDifx = rect.left;
    iDify = rect.top;

    GetDlgItem(IDC_STATIC_AREA)->GetClientRect(&rect);

    left = (MEGA_MIN(A.x, B.x) - iDifx)*(BlockPerRow * 10) / (rect.Width());
    right = (MEGA_MAX(A.x, B.x) - iDifx)*(BlockPerRow * 10) / (rect.Width());
    top = (MEGA_MIN(A.y, B.y) - iDify)*(ROW * 10) / (rect.Height());
    bottom = (MEGA_MAX(A.y, B.y) - iDify)*(ROW * 10) / (rect.Height());

    left = left / 10 * 10;
    right = (right + 9) / 10 * 10;
    top = top / 10 * 10;
    bottom = (bottom + 9) / 10 * 10;

    start.x = left * (rect.Width()) / (BlockPerRow * 10) + iDifx;
    start.y = top * (rect.Height()) / (ROW * 10) + iDify;
    stop.x = right * (rect.Width()) / (BlockPerRow * 10) + iDifx;
    stop.y = bottom * (rect.Height()) / (ROW * 10) + iDify;
    dc.Rectangle(CRect(start, stop));

    for (i = top / 10; i < bottom / 10; i++)
    {
        for (j = left / 10; j < right / 10; j++)
        {
            m_aiArea[i] = m_aiArea[i] | (1 << j);
        }
    }

    return;
}


void CDlgMotionArea::OnBnClickedBtnGet()
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
        "<ConfigCmd>GetMotionArea</ConfigCmd>\r\n"\
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
                if (xmlBase.FindElem("MOTIONAREACFG") && xmlBase.IntoElem())
                {
                    if (xmlBase.FindElem("AREAS") && xmlBase.IntoElem())
                    {
                        int i = 0;
                        do
                        {
                            ConvertSingleNodeData(&m_aiArea[i], xmlBase, "Mask", NODE_STRING_TO_INT);
                            i++;
                        } while (xmlBase.NextSibElem());
                       
                    }
                    CRect rect;
                    CClientDC dc(this);
                    GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rect);
                    ScreenToClient(&rect);
                    int iDifx = rect.left;
                    int iDify = rect.top;
                    for (int i = 0; i < ROW; i++)
                    {
                        for (int j = 0; j < BlockPerRow; j++)
                        {
                            if (m_aiArea[i] & (1 << j))
                            {
                                m_startPoint.x = j * 10 * rect.Width() / (BlockPerRow * 10) + iDifx;
                                m_startPoint.y = i * 10 * rect.Height() / (ROW * 10) + iDify;
                                m_endPoint.x = (j + 1) * 10 * rect.Width() / (BlockPerRow * 10) + iDifx;
                                m_endPoint.y = (i + 1) * 10 * rect.Height() / (ROW * 10) + iDify;
                                dc.Rectangle(CRect(m_startPoint, m_endPoint));
                            }
                        }
                    }
                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}


void CDlgMotionArea::OnBnClickedBtnClear()
{
    // TODO:  在此添加控件通知处理程序代码

    Invalidate();
    m_startPoint = 0;
    m_endPoint = 0;
    m_bStartValid = FALSE;
    m_iAreaNum = 0;
    memset(m_aiArea, 0, sizeof(unsigned int)*ROW);
    UpdateWindow();
}
