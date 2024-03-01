// DlgISAPIConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgEhome50Auth.h"
#include "afxdialogex.h"

// CDlgISAPIConfig 对话框
extern CEHomeDemoDlg *g_pMainDlg;
extern BOOL g_bSS_Enable;
IMPLEMENT_DYNAMIC(CDlgEhome50Auth, CDialogEx)

CDlgEhome50Auth::CDlgEhome50Auth(int iDevID, CWnd* pParent /*=NULL*/)
: CDialogEx(CDlgEhome50Auth::IDD, pParent)
, m_cmEhomeKey(_T(""))
, m_iDevID(iDevID)
{

}

CDlgEhome50Auth::~CDlgEhome50Auth()
{
}

void CDlgEhome50Auth::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_EHOME50_DEVICEID, m_cmDeviceID);
    DDX_Text(pDX, IDC_EDT_EHOME50_EHOMEKEY, m_cmEhomeKey);
}


BEGIN_MESSAGE_MAP(CDlgEhome50Auth, CDialogEx)
    ON_BN_CLICKED(IDOK_EHOME50_AUTH, &CDlgEhome50Auth::OnBnClickedBtnCommand)
END_MESSAGE_MAP()


// CDlgISAPIConfig 消息处理程序


void CDlgEhome50Auth::OnBnClickedBtnCommand()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    if (m_iDevID >= 0)
    {
        memset(g_struDeviceInfo[m_iDevID].byEhomeKey, 0, 32);
        strncpy((char*)g_struDeviceInfo[m_iDevID].byEhomeKey, m_cmEhomeKey.GetBuffer(), m_cmEhomeKey.GetLength());
        if (g_bSS_Enable)
        {
            char szClouldSecretKey[256] = { 0 };
            if (NET_ESS_HAMSHA256((char*)g_struDeviceInfo[m_iDevID].byDeviceID
                , (char*)g_struDeviceInfo[m_iDevID].byEhomeKey, szClouldSecretKey, 255))
            {
                memcpy((char*)g_struDeviceInfo[m_iDevID].byClouldSecretKey, szClouldSecretKey, 64);
            }
        }
    }
    else
    {
        LOCAL_DEVICE_INFO *lpTemp = new LOCAL_DEVICE_INFO();

        strncpy((char*)lpTemp->byDeviceID, m_cmDeviceID.GetBuffer(), m_cmDeviceID.GetLength());
        strncpy((char*)lpTemp->byEhomeKey, m_cmEhomeKey.GetBuffer(), m_cmEhomeKey.GetLength());
        if (g_bSS_Enable)
        {
            char szClouldSecretKey[256] = { 0 };
            if (NET_ESS_HAMSHA256((char*)lpTemp->byDeviceID, (char*)lpTemp->byEhomeKey, szClouldSecretKey, 255))
            {
                memcpy((char*)lpTemp->byClouldSecretKey, szClouldSecretKey, 64);
            }
        }
        lpTemp->dwVersion = 5;

        if (g_pMainDlg != NULL)
        {
            //允许注册
            ::PostMessage(g_pMainDlg->m_hWnd, WM_ADD_DEV, 1, (LPARAM)lpTemp);
        }
    }
    return CDialogEx::OnOK();
}


BOOL CDlgEhome50Auth::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (m_iDevID >= 0)
    {
        CString strDevID(g_struDeviceInfo[m_iDevID].byDeviceID);
        GetDlgItem(IDC_EDT_EHOME50_DEVICEID)->SetWindowText(strDevID);
        GetDlgItem(IDC_EDT_EHOME50_DEVICEID)->EnableWindow(FALSE);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
