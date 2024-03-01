// DlgSubPicCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgSubPicCfg.h"
#include "Public/Convert/Convert.h"


// CDlgSubPicCfg 对话框

IMPLEMENT_DYNAMIC(CDlgSubPicCfg, CDialog)

CDlgSubPicCfg::CDlgSubPicCfg(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSubPicCfg::IDD, pParent)
    , m_dwChannelNo(0)
    , m_bIsShowChanName(FALSE)
    , m_sChanName(_T(""))
    , m_dwPosChanX(0)
    , m_dwPosChanY(0)
    , m_bIsShowOSD(FALSE)
    , m_dwPosOSDX(0)
    , m_dwPosOSDY(0)
    , m_bIsShowWeek(FALSE)
{
   memset(m_bShow, 0, sizeof(m_bShow));
  // memset(m_csString, 0, sizeof(m_csString));
   memset(m_iPosX, 0, sizeof(m_iPosX));
   memset(m_iPosY, 0, sizeof(m_iPosY));
    m_iDeviceIndex = -1;
    m_lLoginID = -1;
    m_iChanIndex = -1;
    memset(&m_struPicCfg, 0, sizeof(m_struPicCfg));
    m_struPicCfg.dwSize = sizeof(m_struPicCfg);
}

CDlgSubPicCfg::~CDlgSubPicCfg()
{
}

void CDlgSubPicCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //DDX_Text(pDX, IDC_EDIT_CHANNEL_NO, m_dwChannelNo);
    DDX_Check(pDX, IDC_CHK_SHOW_CHANNEL_NAME, m_bIsShowChanName);
    DDX_Text(pDX, IDC_EDIT_CHANNEL_NAME, m_sChanName);
    DDX_Text(pDX, IDC_EDIT_POS_CHAN_X, m_dwPosChanX);
    DDX_Text(pDX, IDC_EDIT_POS_CHAN_Y, m_dwPosChanY);
    DDX_Check(pDX, IDC_CHK_SHOW_OSD, m_bIsShowOSD);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_X, m_dwPosOSDX);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_Y, m_dwPosOSDY);
    DDX_Control(pDX, IDC_COMBO_OSD_TYPE, m_cmbOSDType);
    DDX_Control(pDX, IDC_COMBO_OSD_ATTRIB, m_cmbOSDAttrib);
    DDX_Check(pDX, IDC_CHK_SHOW_WEEK, m_bIsShowWeek);
    DDX_Check(pDX, IDC_CHK_SHOW_OSD2, m_bShow[0]);
    DDX_Check(pDX, IDC_CHK_SHOW_OSD3, m_bShow[1]);
    DDX_Check(pDX, IDC_CHK_SHOW_OSD4, m_bShow[2]);
    DDX_Check(pDX, IDC_CHK_SHOW_OSD5, m_bShow[3]);
    DDX_Text(pDX, IDC_EDIT_STRING1, m_csString[0]);
    DDX_Text(pDX, IDC_EDIT_STRING2, m_csString[1]);
    DDX_Text(pDX, IDC_EDIT_STRING3, m_csString[2]);
    DDX_Text(pDX, IDC_EDIT_STRING4, m_csString[3]);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_X2, m_iPosX[0]);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_X3, m_iPosX[1]);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_X4, m_iPosX[2]);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_X5, m_iPosX[3]);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_Y2, m_iPosY[0]);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_Y3, m_iPosY[1]);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_Y4, m_iPosY[2]);
    DDX_Text(pDX, IDC_EDIT_POS_OSD_Y5, m_iPosY[3]);
}


BEGIN_MESSAGE_MAP(CDlgSubPicCfg, CDialog)
    ON_BN_CLICKED(IDC_BTN_SET_OSD, &CDlgSubPicCfg::OnBnClickedBtnSetOsd)
    ON_BN_CLICKED(IDC_BTN_GET_OSD, &CDlgSubPicCfg::OnBnClickedBtnGetOsd)
END_MESSAGE_MAP()

BOOL CDlgSubPicCfg::CheckInitParam()
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
        m_dwChannelNo = m_iChanIndex+1;
    }


    return TRUE;
}

void CDlgSubPicCfg::CurCfgUpdate()
{
    if(!CheckInitParam())
    {
        return;
    }

    UpdateData(TRUE);

    //获取图片参数
    memset(&m_struPicCfg, 0, sizeof(m_struPicCfg));
    m_struPicCfg.dwSize = sizeof(m_struPicCfg);
    NET_EHOME_CONFIG struCfg = {0};
    struCfg.pCondBuf = &m_dwChannelNo;
    struCfg.dwCondSize = sizeof(m_dwChannelNo);
    struCfg.pOutBuf = &m_struPicCfg;
    struCfg.dwOutSize = sizeof(m_struPicCfg);

    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_PIC_CFG, &struCfg, sizeof(struCfg)))
    {
        //需要将byChannelName转换成GB2312
        DWORD dwConvertLen = 0;
        UTF82A((char*)m_struPicCfg.byChannelName, (char*)m_struPicCfg.byChannelName, NAME_LEN, &dwConvertLen);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_PIC_CFG");
        m_sChanName = m_struPicCfg.byChannelName;
        m_bIsShowChanName = m_struPicCfg.bIsShowChanName;
        m_dwPosChanX = m_struPicCfg.wChanNameXPos;
        m_dwPosChanY = m_struPicCfg.wChanNameYPos;
        m_bIsShowOSD = m_struPicCfg.bIsShowOSD;
        m_dwPosOSDX = m_struPicCfg.wOSDXPos;
        m_dwPosOSDY = m_struPicCfg.wOSDYPos;
        m_cmbOSDType.SetCurSel(m_struPicCfg.byOSDType);
        m_cmbOSDAttrib.SetCurSel(m_struPicCfg.byOSDAtrib);
        m_bIsShowWeek = m_struPicCfg.bIsShowWeek;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_PIC_CFG");
    }

    UpdateData(FALSE);
}

void CDlgSubPicCfg::CurCfgSetup()
{
    if(!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);

    //设置压缩参数
    memset(&m_struPicCfg, 0, sizeof(m_struPicCfg));
    m_struPicCfg.dwSize = sizeof(m_struPicCfg);
    if (strlen(m_sChanName) >(NAME_LEN - 1)/*算上结束符，不能超过32字节*/)
    {
        AfxMessageBox("Channel name too long!");
        return;
    }
    strcpy((char*)m_struPicCfg.byChannelName, m_sChanName);
    m_struPicCfg.bIsShowChanName = m_bIsShowChanName;
    m_struPicCfg.wChanNameXPos = (WORD)m_dwPosChanX;
    m_struPicCfg.wChanNameYPos = (WORD)m_dwPosChanY;
    m_struPicCfg.bIsShowOSD = m_bIsShowOSD;
    m_struPicCfg.wOSDXPos = (WORD)m_dwPosOSDX;
    m_struPicCfg.wOSDYPos = (WORD)m_dwPosOSDY;

    m_struPicCfg.bIsShowWeek = m_bIsShowWeek;

    int nSel = m_cmbOSDType.GetCurSel();
    if (nSel == CB_ERR)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "Please Select OSD Type!");
        return;
    }

    m_struPicCfg.byOSDType = (BYTE)nSel;

    nSel = m_cmbOSDAttrib.GetCurSel();
    if (nSel == CB_ERR)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "Please Select OSD Attrib!");
        return;
    }
    m_struPicCfg.byOSDAtrib = (BYTE)nSel;

    //需要将字符串字段转换成UTF-8
    DWORD dwConvertLen = 0;
    A2UTF8((char*)m_struPicCfg.byChannelName, (char*)m_struPicCfg.byChannelName, NAME_LEN, &dwConvertLen);

    NET_EHOME_CONFIG struCfg = {0};
    struCfg.pCondBuf = &m_dwChannelNo;
    struCfg.dwCondSize = sizeof(m_dwChannelNo);
    struCfg.pInBuf = &m_struPicCfg;
    struCfg.dwInSize = sizeof(m_struPicCfg);

    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_PIC_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_PIC_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_PIC_CFG");
    }
}


// CDlgSubPicCfg 消息处理程序

BOOL CDlgSubPicCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CRect rc(0, 0, 0, 0);
    GetParent()->GetClientRect(&rc);
    ((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
    MoveWindow(&rc);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}



void CDlgSubPicCfg::OnBnClickedBtnSetOsd()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);

    CString str;
    for (int i = 0; i < 4; i++)
    {
        CString strTemp;
        strTemp.Format("<STRINGINFO>\r\n"
            "<String>%s</String>\r\n"
            "<IsShowString>%d</IsShowString>\r\n"
            "<XPos>%d</XPos>\r\n"
            "<YPos>%d</YPos>\r\n"
            "</STRINGINFO>\r\n"
            , m_csString[i], m_bShow[i], m_iPosX[i], m_iPosY[i]);
        str += strTemp;
    }
    char szShowString[1024] = { 0 };
    memcpy(szShowString, str.GetBuffer(0), str.GetLength());
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024 * 2] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetShowStringPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigXML>\r\n"
        "<SHOWSTRINGINFO>\r\n"
        "%s"
        "</SHOWSTRINGINFO>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_dwChannelNo, szShowString);
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
    A2UTF8((char*)szInBuf, (char*)szInBuf, 1500, &struXmlCfg.dwInSize);
    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
}


void CDlgSubPicCfg::OnBnClickedBtnGetOsd()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);

    memset(m_bShow, 0, sizeof(m_bShow));
   
    memset(m_iPosX, 0, sizeof(m_iPosX));
    memset(m_iPosY, 0, sizeof(m_iPosY));
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>GetShowStringPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "</Params>", m_dwChannelNo);
    char szCmd[32] = "GETDEVICECONFIG";
    char szOutBuffer[1024 * 10] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024 * 2] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024 * 2;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    UTF82A((char*)szOutBuffer, (char*)szOutBuffer, struXmlCfg.dwOutSize, &struXmlCfg.dwOutSize);
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
                if (xmlBase.FindElem("SHOWSTRINGINFO") && xmlBase.IntoElem())
                {  
                    int i = 0;
                        do
                        {
                            if (xmlBase.FindElem("STRINGINFO") && xmlBase.IntoElem())
                            {
                                m_csString[i] = "";
                                char szShowString[64] = { 0 };
                                 ConvertSingleNodeData(szShowString, xmlBase, "String", NODE_STRING_TO_ARRAY,64);
                                m_csString[i] = szShowString;
                                ConvertSingleNodeData(&m_bShow[i], xmlBase, "IsShowString", NODE_STRING_TO_INT);
                                ConvertSingleNodeData(&m_iPosX[i], xmlBase, "XPos", NODE_STRING_TO_INT);
                                ConvertSingleNodeData(&m_iPosY[i], xmlBase, "YPos", NODE_STRING_TO_INT);
                                xmlBase.OutOfElem();
                                i++;
                            }
                        } while (xmlBase.NextSibElem());
                 

                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}
