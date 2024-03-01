// DlgListen.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgListen.h"


// CDlgListen 对话框

IMPLEMENT_DYNAMIC(CDlgListen, CDialog)

CDlgListen::CDlgListen(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgListen::IDD, pParent)
    , m_wPort(0)
    , m_pFatherDlg(NULL)
    , m_rdLinkType(0)
    , m_byEncrypt(FALSE)
{
}

CDlgListen::~CDlgListen()
{
    for (int i = 0; i < MAX_LISTEN_NUM; ++i)
    {
        if (g_struPreviewListen[i].lHandle < 0)
        {
            SetDefautVar(TRUE, i);
        }
    }
}

void CDlgListen::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_LISTEN_TYPE, m_cmListenType);
    DDX_Control(pDX, IDC_COMBO_PORT_NO, m_cmPortNo);
    DDX_Control(pDX, IDC_IPADDRESS1, m_ipAddress);
    DDX_Text(pDX, IDC_EDIT_PORT, m_wPort);
    DDX_Control(pDX, IDC_COMBO_LINK_TYPE, m_cmLinkType);
    DDX_Radio(pDX, IDC_RADIO1, m_rdLinkType);
    DDX_Check(pDX, IDC_CHECK_ENCRYPT, m_byEncrypt);
}


BEGIN_MESSAGE_MAP(CDlgListen, CDialog)
    ON_CBN_SELCHANGE(IDC_COMBO_PORT_NO, &CDlgListen::OnCbnSelchangeComboPortNo)
    ON_BN_CLICKED(IDC_BTN_START_LISTEN, &CDlgListen::OnBnClickedBtnStartListen)
    ON_BN_CLICKED(IDC_BTN_STOP_LISTEN, &CDlgListen::OnBnClickedBtnStopListen)
    ON_BN_CLICKED(IDC_RADIO1, &CDlgListen::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, &CDlgListen::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO3, &CDlgListen::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO4, &CDlgListen::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CDlgListen 消息处理程序

BOOL CDlgListen::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    char szLan[128] = {0};
    CString csTemp = _T("");
    m_cmListenType.ResetContent();
    g_StringLanType(szLan, "预览监听", "preview listen");
    csTemp = szLan;
    m_cmListenType.AddString(csTemp);
    m_cmPortNo.ResetContent();
    int i = 0;
    char sTemp[16] = {0};
    for (i=0; i<MAX_LISTEN_NUM; i++)
    {
        sprintf(sTemp, "%d", i);
        m_cmPortNo.AddString(sTemp);
    }
    m_cmListenType.SetCurSel(0);
    m_cmPortNo.SetCurSel(0);
    UpdateData(FALSE);

    OnCbnSelchangeComboPortNo();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgListen::OnCbnSelchangeComboPortNo()
{
    // TODO: 在此添加控件通知处理程序代码
    // UpdateData(TRUE);
    int iIndex = m_cmPortNo.GetCurSel();

    DWORD dwIP = ntohl(inet_addr(g_struPreviewListen[iIndex].struIP.szIP));
    m_ipAddress.SetAddress(dwIP);

    if (0 != g_struPreviewListen[iIndex].struIP.wPort)
    {
        m_wPort = g_struPreviewListen[iIndex].struIP.wPort;
    }
    else
    {
        m_wPort = 8000;   //默认8000
    }

    if (8 == g_struPreviewListen[iIndex].iLinkType)
    {
        m_rdLinkType = 3;
    }
    else
    {
        m_rdLinkType = g_struPreviewListen[iIndex].iLinkType;
    }

    if (g_struPreviewListen[iIndex].lHandle > -1)
    {
        GetDlgItem(IDC_BTN_START_LISTEN)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_STOP_LISTEN)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BTN_START_LISTEN)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_STOP_LISTEN)->EnableWindow(FALSE);
    }

    m_byEncrypt = g_bArrayEncrypt[iIndex];
    UpdateData(FALSE);
}

void CDlgListen::OnBnClickedBtnStartListen()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    int iIndex = m_cmPortNo.GetCurSel();

    DWORD dwIP = 0;
    m_ipAddress.GetAddress(dwIP);
    CString csIP = IPToStr(dwIP);
    memcpy(g_struPreviewListen[iIndex].struIP.szIP, csIP, csIP.GetLength());
    g_struPreviewListen[iIndex].struIP.wPort = (WORD)m_wPort;
    if (3 == m_rdLinkType)
    {
        g_struPreviewListen[iIndex].iLinkType = 8;
    }
    else
    {
        g_struPreviewListen[iIndex].iLinkType = m_rdLinkType;
    }
    g_bArrayEncrypt[iIndex] = m_byEncrypt;

    if (g_struPreviewListen[iIndex].lHandle > -1)
    {
        if(NET_ESTREAM_StopListenPreview(g_struPreviewListen[iIndex].lHandle))
        {
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 2, "NET_ESTREAM_StopListenPreview");
            g_struPreviewListen[iIndex].lHandle = -1;
        }
        else
        {
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 2, "NET_ESTREAM_StopListenPreview");
        }
    }

    NET_EHOME_LISTEN_PREVIEW_CFG struListen = {0};
    memcpy(&struListen.struIPAdress, &g_struPreviewListen[iIndex].struIP, sizeof(NET_EHOME_IPADDRESS));
    struListen.fnNewLinkCB = CEHomeDemoDlg::fnPREVIEW_NEWLINK_CB;
    struListen.pUser = g_pMainDlg;
    struListen.byLinkMode = (BYTE)g_struPreviewListen[iIndex].iLinkType;
    struListen.byLinkEncrypt = g_bArrayEncrypt[iIndex];
    LONG lHandle = NET_ESTREAM_StartListenPreview(&struListen);
    if (lHandle > -1)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 2, "NET_ESTREAM_StartListenPreview");
        g_struPreviewListen[iIndex].lHandle = lHandle;
        GetDlgItem(IDC_BTN_START_LISTEN)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_STOP_LISTEN)->EnableWindow(TRUE);
    }
    else
    {
        SetDefautVar(TRUE, iIndex);
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 2, "NET_ESTREAM_StartListenPreview");
    }


    //UpdateData(FALSE);
}

void CDlgListen::OnBnClickedBtnStopListen()
{
    // TODO: 在此添加控件通知处理程序代码
    //UpdateData(TRUE);
    int iIndex = m_cmPortNo.GetCurSel();
    if(NET_ESTREAM_StopListenPreview(g_struPreviewListen[iIndex].lHandle))
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 2, "NET_ESTREAM_StopListenPreview");
        g_struPreviewListen[iIndex].lHandle = -1;
        SetDefautVar(TRUE, iIndex);
        GetDlgItem(IDC_BTN_START_LISTEN)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_STOP_LISTEN)->EnableWindow(FALSE);
        if (g_pMainDlg->m_pDlgPreview[m_pFatherDlg->m_iCurWndIndex].m_bRecord)
        {
            g_pMainDlg->m_pDlgPreview[m_pFatherDlg->m_iCurWndIndex].m_bRecord = FALSE;
            m_pFatherDlg->m_bRecord = FALSE;
            fclose(g_pMainDlg->m_pDlgPreview[m_pFatherDlg->m_iCurWndIndex].m_fVideoFile);
            g_pMainDlg->m_pDlgPreview[m_pFatherDlg->m_iCurWndIndex].m_fVideoFile = NULL;
            char szLan[128] = { 0 };
            CString csTemp = _T("");
            g_StringLanType(szLan, "开始录像", "Start Record");
            csTemp = szLan;
            m_pFatherDlg->GetDlgItem(IDC_BTN_SALVE)->SetWindowText(csTemp);
        }
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 2, "NET_ESTREAM_StopListenPreview");
    }
    UpdateData(FALSE);
}


void CDlgListen::OnBnClickedRadio1()
{
    UpdateData(TRUE);
    int iIndex = m_cmPortNo.GetCurSel();
    if (m_pFatherDlg->m_iCurWndIndex >= 0 && g_struPreviewListen[iIndex].lHandle < 0)
    {
        if (3 == m_rdLinkType)
        {
            g_struPreviewListen[iIndex].iLinkType = 8;
        }
        else
        {
            g_struPreviewListen[iIndex].iLinkType = m_rdLinkType;;
        }
    }
    else
    {
        if (8 == g_struPreviewListen[iIndex].iLinkType)
        {
            m_rdLinkType = 3;
        }
        else
        {
            m_rdLinkType = g_struPreviewListen[iIndex].iLinkType;
        }
        char szLan[32];
        sprintf(szLan, "请先停止监听");
        AfxMessageBox(szLan);
    }
    UpdateData(FALSE);
}

void CDlgListen::SetDefautVar(BOOL bSetGlobalVar, int index)
{
    if (bSetGlobalVar)
    {
        memcpy(g_struPreviewListen[index].struIP.szIP, "0.0.0.0", sizeof("0.0.0.0"));
        g_struPreviewListen[index].struIP.wPort = 8000;
        g_struPreviewListen[index].iLinkType = 0;
        g_bArrayEncrypt[index] = FALSE;
    }
    //else
    //{
    //	m_ipAddress.SetAddress(0);
    //	m_wPort = 8000;
    //	m_rdLinkType = 0;
    //	m_byEncrypt = 0;
    //}
}

