// DlgIFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgIFrame.h"
#include "afxdialogex.h"


// CDlgIFrame 对话框

IMPLEMENT_DYNAMIC(CDlgIFrame, CDialog)

CDlgIFrame::CDlgIFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIFrame::IDD, pParent)
    , m_lUserID(-1)
    , m_iDeviceIndex(-1)
    , m_iChanIndex(-1)
    , m_iChannelNo(-1)
{

}

CDlgIFrame::~CDlgIFrame()
{
}

void CDlgIFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_STREAM_TYPE, m_cmbStreamType);
}


BEGIN_MESSAGE_MAP(CDlgIFrame, CDialog)
    ON_BN_CLICKED(IDOK, &CDlgIFrame::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgIFrame 消息处理程序
BOOL CDlgIFrame::OnInitDialog()
{
  
    CDialog::OnInitDialog();
    m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    m_iChanIndex = g_pMainDlg->GetCurChanIndex();
    m_iChannelNo=g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChannelNO;
    return TRUE;
}

void CDlgIFrame::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码
    NET_EHOME_MAKE_I_FRAME struIFrame = { 0 };
    struIFrame.iChannel = m_iChannelNo;
    struIFrame.dwStreamType = m_cmbStreamType.GetCurSel() + 1;
    if (!NET_ECMS_MakeIFrame(m_lUserID, &struIFrame))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_MakeIFrame failed");
        return ;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_MakeIFrame success");
        return;
    }
}
