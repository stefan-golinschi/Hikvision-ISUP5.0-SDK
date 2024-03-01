// DlgCommonCfg.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgCommonCfg.h"


// CDlgCommonCfg dialog

IMPLEMENT_DYNAMIC(CDlgCommonCfg, CDialog)

CDlgCommonCfg::CDlgCommonCfg(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgCommonCfg::IDD, pParent)
    , m_iConfigType(SUB_DEVICE_CFG_T)
{

}

CDlgCommonCfg::~CDlgCommonCfg()
{
}

void CDlgCommonCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_COMMON_CFG, m_tabCommonCfg);
}


BEGIN_MESSAGE_MAP(CDlgCommonCfg, CDialog)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_COMMON_CFG, &CDlgCommonCfg::OnTcnSelchangeTabCommonCfg)
    ON_BN_CLICKED(IDC_BTN_REFRESH, &CDlgCommonCfg::OnBnClickedBtnRefresh)
    ON_BN_CLICKED(IDC_BTN_SAVE_CFG, &CDlgCommonCfg::OnBnClickedBtnSaveCfg)
END_MESSAGE_MAP()


// CDlgCommonCfg message handlers

BOOL CDlgCommonCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    char szLan[128] = {0};
    g_StringLanType(szLan, "设备参数配置", "Device Setup");
    m_tabCommonCfg.InsertItem(0, szLan);
    g_StringLanType(szLan, "网络参数配置", "Network Setup");
    m_tabCommonCfg.InsertItem(1, szLan);
    g_StringLanType(szLan, "通道参数配置", "Channel Setup");
    m_tabCommonCfg.InsertItem(2, szLan);
    g_StringLanType(szLan, "报警参数配置", "Alarm Setup");
    m_tabCommonCfg.InsertItem(3, szLan);    
    g_StringLanType(szLan, "透传参数配置", "Xml Setup");
    m_tabCommonCfg.InsertItem(4, szLan);
    g_StringLanType(szLan, "图片参数配置", "Picture Setup");
    m_tabCommonCfg.InsertItem(5, szLan);


    m_subDlgDeviceCfg.Create(IDD_SUB_DLG_DEVICE_CFG, &m_tabCommonCfg);
    m_subDlgNetworkCfg.Create(IDD_SUB_DLG_NETWORK_CFG, &m_tabCommonCfg);
    m_subDlgChanCfg.Create(IDD_SUB_DLG_CHANNEL_CFG, &m_tabCommonCfg);
    m_subDlgAlarmCfg.Create(IDD_SUB_DLG_ALARM_CFG, &m_tabCommonCfg);
    m_subDlgXmlCfg.Create(IDD_SUB_DLG_XML_CFG, &m_tabCommonCfg);
    m_subDlgPicCfg.Create(IDD_SUB_DLG_PIC_CFG, &m_tabCommonCfg);

    m_subDlgDeviceCfg.ShowWindow(SW_SHOW);
    m_subDlgNetworkCfg.ShowWindow(SW_HIDE);
    m_subDlgChanCfg.ShowWindow(SW_HIDE);
    m_subDlgAlarmCfg.ShowWindow(SW_HIDE);
    m_subDlgXmlCfg.ShowWindow(SW_HIDE);
    m_subDlgPicCfg.ShowWindow(SW_HIDE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCommonCfg::OnTcnSelchangeTabCommonCfg(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    UN_REFERENCED_PARAMETER(pNMHDR)
    m_iConfigType = m_tabCommonCfg.GetCurSel();

    if (m_iConfigType == SUB_DEVICE_CFG_T)
    {
        m_subDlgDeviceCfg.ShowWindow(SW_SHOW);
    }
    else
    {
        m_subDlgDeviceCfg.ShowWindow(SW_HIDE);
    }

    if (m_iConfigType == SUB_NETWORK_CFG_T)
    {
        m_subDlgNetworkCfg.ShowWindow(SW_SHOW);
    }
    else
    {
        m_subDlgNetworkCfg.ShowWindow(SW_HIDE);
    }

    if (m_iConfigType == SUB_CHAN_CFG_T)
    {
        m_subDlgChanCfg.ShowWindow(SW_SHOW);
    }
    else
    {   
        m_subDlgChanCfg.ShowWindow(SW_HIDE);
    }

    if (m_iConfigType == SUB_ALARM_CFG_T)
    {
        m_subDlgAlarmCfg.ShowWindow(SW_SHOW);
    }
    else
    {
        m_subDlgAlarmCfg.ShowWindow(SW_HIDE);
    }

    if (m_iConfigType == SUB_XML_XFG_T)
    {
        m_subDlgXmlCfg.ShowWindow(SW_SHOW);
    }
    else
    {
        m_subDlgXmlCfg.ShowWindow(SW_HIDE);
    }

    if (m_iConfigType == SUB_PIC_CFG_T)
    {
        m_subDlgPicCfg.ShowWindow(SW_SHOW);
    }
    else
    {
        m_subDlgPicCfg.ShowWindow(SW_HIDE);
    }

    CurCfgUpdate();
    if (m_iConfigType == SUB_XML_XFG_T)
    {
        GetDlgItem(IDC_BTN_SAVE_CFG)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_BTN_REFRESH)->ShowWindow(SW_HIDE);
    }
    else
    {
        GetDlgItem(IDC_BTN_SAVE_CFG)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_BTN_REFRESH)->ShowWindow(SW_SHOW);
    }
    *pResult = 0;
}

void    CDlgCommonCfg::CurCfgUpdate()
{
    if (m_iConfigType == SUB_DEVICE_CFG_T)
    {
        m_subDlgDeviceCfg.CurCfgUpdate();
    }
    else if (m_iConfigType == SUB_NETWORK_CFG_T)
    {
        m_subDlgNetworkCfg.CurCfgUpdate();
    }
    else if (m_iConfigType == SUB_CHAN_CFG_T)
    {
        m_subDlgChanCfg.CurCfgUpdate();
    }
    else if (m_iConfigType == SUB_ALARM_CFG_T)
    {
        m_subDlgAlarmCfg.CurCfgUpdate();
    }
    else if (m_iConfigType == SUB_XML_XFG_T)
    {
        m_subDlgXmlCfg.CurCfgUpdate();
    }
    else if (m_iConfigType == SUB_PIC_CFG_T)
    {
        m_subDlgPicCfg.CurCfgUpdate();
    }
}
void CDlgCommonCfg::OnBnClickedBtnRefresh()
{
    // TODO: Add your control notification handler code here
    CurCfgUpdate();
}

void CDlgCommonCfg::OnBnClickedBtnSaveCfg()
{
    // TODO: Add your control notification handler code here
    if (m_iConfigType == SUB_DEVICE_CFG_T)
    {
        m_subDlgDeviceCfg.CurCfgSetup();
    }
    else if (m_iConfigType == SUB_NETWORK_CFG_T)
    {
        m_subDlgNetworkCfg.CurCfgSetup();
    }
    else if (m_iConfigType == SUB_CHAN_CFG_T)
    {
        m_subDlgChanCfg.CurCfgSetup();
    }
    else if (m_iConfigType == SUB_ALARM_CFG_T)
    {
        m_subDlgAlarmCfg.CurCfgSetup();
    }
    else if (m_iConfigType == SUB_XML_XFG_T)
    {
        m_subDlgXmlCfg.CurCfgSetup();
    }
    else if (m_iConfigType == SUB_PIC_CFG_T)
    {
        m_subDlgPicCfg.CurCfgSetup();
    }
}
