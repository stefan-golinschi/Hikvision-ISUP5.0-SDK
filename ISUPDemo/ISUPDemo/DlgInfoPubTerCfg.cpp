// DlgInfoPubTerCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgInfoPubTerCfg.h"
#include "afxdialogex.h"


// CDlgInfoPubTerCfg 对话框

IMPLEMENT_DYNAMIC(CDlgInfoPubTerCfg, CDialogEx)

CDlgInfoPubTerCfg::CDlgInfoPubTerCfg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInfoPubTerCfg::IDD, pParent)
{
	CheckInitParam();
}

CDlgInfoPubTerCfg::~CDlgInfoPubTerCfg()
{
}

void CDlgInfoPubTerCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInfoPubTerCfg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETPLAYERPARAM, &CDlgInfoPubTerCfg::OnBnClickedBtnSetplayerparam)
	ON_BN_CLICKED(IDC_BTN_GETPLAYERPARAM, &CDlgInfoPubTerCfg::OnBnClickedBtnGetplayerparam)
END_MESSAGE_MAP()


// CDlgInfoPubTerCfg 消息处理程序
BOOL CDlgInfoPubTerCfg::CheckInitParam()
{
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
	return TRUE;
}

void CDlgInfoPubTerCfg::OnBnClickedBtnSetplayerparam()
{
	// TODO:  在此添加控件通知处理程序代码
	NET_EHOME_PLAYER_PARAM struPlayerParam = { 0 };
	memcpy(struPlayerParam.szCfgType, "light", strlen("light"));
	struPlayerParam.struDisplayParam.bAutoLightEnable = 0;
	struPlayerParam.struDisplayParam.dwBackLightLevel = 50;

	NET_EHOME_BINARY_SEND_DATA struSendData = { 0 };
	struSendData.dwSize = sizeof(struSendData);
	struSendData.lpInBuffer = &struPlayerParam;
	struSendData.dwInBufferSize = sizeof(struPlayerParam);

	NET_EHOME_BINARY_RECV_DATA struRecvData = { 0 };
	struRecvData.dwSize = sizeof(struRecvData);
	struRecvData.lpOutBuffer = NULL;
	struRecvData.dwOutBufferSize = NULL;
	if (!NET_ECMS_STDBinaryCfg(m_lLoginID, NET_EHOME_S2C_SETPLAYERPARAM, &struSendData, &struRecvData))
	{
		DWORD dwErr = NET_ECMS_GetLastError();
		AfxMessageBox("NET_EHOME_S2C_SETPLAYERPARAM failed insert, err=%d", dwErr);
	}
	else
	{
		AfxMessageBox("NET_EHOME_S2C_SETPLAYERPARAM success insert");
	}
}


void CDlgInfoPubTerCfg::OnBnClickedBtnGetplayerparam()
{
	// TODO:  在此添加控件通知处理程序代码
	
}
