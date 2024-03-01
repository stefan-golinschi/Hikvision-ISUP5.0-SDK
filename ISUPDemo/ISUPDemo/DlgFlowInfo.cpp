// DlgFlowInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgFlowInfo.h"


// CDlgFlowInfo 对话框

IMPLEMENT_DYNAMIC(CDlgFlowInfo, CDialog)

CDlgFlowInfo::CDlgFlowInfo(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgFlowInfo::IDD, pParent)
    , m_dateStart(COleDateTime::GetCurrentTime())
    , m_timeStart(COleDateTime::GetCurrentTime())
    , m_dateStop(COleDateTime::GetCurrentTime())
    , m_timeStop(COleDateTime::GetCurrentTime())
    , m_bTimeType(0)
{
    m_bQuit = FALSE;
    m_bSearching = FALSE;
    m_dwFileNum = 0;
    m_iDeviceIndex = -1;
    m_iChanIndex = -1;
    m_lLoginID = -1;
    m_lFileHandle = -1;
    m_hFileThread = NULL;
    memset(&m_struFindCond, 0, sizeof(m_struFindCond));
    m_struFindCond.dwSize = sizeof(m_struFindCond);
}

CDlgFlowInfo::~CDlgFlowInfo()
{
}

void CDlgFlowInfo::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_FLOW_TYPE, m_cmbFlowType);
    DDX_Control(pDX, IDC_LIST_FLOW_INFO, m_listFlowInfo);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_FLOW_DATE_START, m_dateStart);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_FLOW_TIME_START, m_timeStart);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_FLOW_DATE_STOP, m_dateStop);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_FLOW_TIME_STOP, m_timeStop);
    DDX_CBIndex(pDX, IDC_COMBO_TIME_TYPE, m_bTimeType);
}


BEGIN_MESSAGE_MAP(CDlgFlowInfo, CDialog)
    ON_BN_CLICKED(IDC_BTN_SEARCH__FLOW_INFO, &CDlgFlowInfo::OnBnClickedBtnSearch)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CDlgFlowInfo::CheckInitParam()
{
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex == -1)
    {
        AfxMessageBox("please select a device.");
        return FALSE;
    }

    if (m_iDeviceIndex != iDeviceIndex)
    {
        m_iDeviceIndex = iDeviceIndex;
        m_iChanIndex = -1;
    }

    int iChanIndex = g_pMainDlg->GetCurChanIndex();
    if (iChanIndex == -1)
    {
        AfxMessageBox("please select a channel.");
        return FALSE;
    }

    if (m_iChanIndex != iChanIndex)
    {
        m_iChanIndex = iChanIndex;
        m_bSearching = FALSE;
        m_hFileThread = NULL;
        m_dwFileNum = 0;
    }

    m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    if (m_lLoginID < 0)
    {
        AfxMessageBox("please login device first.");
        return FALSE;
    }

    return TRUE;
}


/*********************************************************
Function:    GetFileThread
Desc:        get recorded file list thread
Input:        pParam,pointer to parameters
Output:    
Return:    
**********************************************************/
UINT CDlgFlowInfo::GetFileThread(LPVOID pParam)
{
    CDlgFlowInfo *pThis = static_cast<CDlgFlowInfo*>(pParam);
    LONG lRet = -1;
    NET_EHOME_FLOW_INFO struFlowInfo;
    memset(&struFlowInfo, 0, sizeof(struFlowInfo));
    CString csTmp;
    char szLan[128] = {0};

    while (!pThis->m_bQuit)
    {
        //目前没有要进行字符转换的字段
        lRet = NET_ECMS_FindNextFile_V11(pThis->m_lFileHandle, &struFlowInfo, sizeof(struFlowInfo));
        //目前没有要进行字符转换的字段
        if (lRet == ENUM_GET_NEXT_STATUS_SUCCESS)
        {
            csTmp.Format("%d", pThis->m_dwFileNum);
            pThis->m_listFlowInfo.InsertItem(pThis->m_dwFileNum, csTmp);
            csTmp.Format("%d",struFlowInfo.dwFlowIndex);
            pThis->m_listFlowInfo.SetItemText(pThis->m_dwFileNum, 1, csTmp);
            csTmp.Format("%d", struFlowInfo.dwFlowValue);
            pThis->m_listFlowInfo.SetItemText(pThis->m_dwFileNum, 2, csTmp);

            pThis->m_dwFileNum++;
        }
        else
        {
            if (lRet == ENUM_GET_NETX_STATUS_NEED_WAIT)
            {
                Sleep(100);
                continue;
            }
            else if ((lRet == ENUM_GET_NETX_STATUS_NO_FILE) || (lRet == ENUM_GET_NEXT_STATUS_FINISH))
            {
                g_StringLanType(szLan, "查找", "Search");
                pThis->GetDlgItem(IDC_BTN_SEARCH__FLOW_INFO)->SetWindowText(szLan);
                pThis->m_bSearching = FALSE;
                pThis->GetDlgItem(IDC_STATIC_FINDING_FLOW_INFO)->ShowWindow(SW_HIDE);
                g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_FindNextFile file num[%d]", pThis->m_dwFileNum);
                pThis->m_dwFileNum = 0;
                break;
            }
            else if(lRet == ENUM_GET_NEXT_STATUS_NOT_SUPPORT)
            {
                g_StringLanType(szLan, "查找", "Search");
                pThis->GetDlgItem(IDC_BTN_SEARCH__FLOW_INFO)->SetWindowText(szLan);
                pThis->m_bSearching = FALSE;
                pThis->GetDlgItem(IDC_STATIC_FINDING_FLOW_INFO)->ShowWindow(SW_HIDE);
                g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_FindNextFile");
                g_StringLanType(szLan, "设备不支持该操作", "Device do not support");
                AfxMessageBox(szLan);
                pThis->m_dwFileNum = 0;
                break;
            }
            else
            {
                g_StringLanType(szLan, "查找", "Search");
                pThis->GetDlgItem(IDC_BTN_SEARCH__FLOW_INFO)->SetWindowText(szLan);
                pThis->m_bSearching = FALSE;
                pThis->GetDlgItem(IDC_STATIC_FINDING_FLOW_INFO)->ShowWindow(SW_HIDE);
                g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_FindNextFile");
                g_StringLanType(szLan, "由于服务器忙，或网络故障，获取流量异常终止", "Since the server is busy, or network failure, abnormal termination of access to the file list");
                AfxMessageBox(szLan);
                pThis->m_dwFileNum = 0;
                break;
            }
        }
    }

    CloseHandle(pThis->m_hFileThread);
    pThis->m_hFileThread = NULL;
    NET_ECMS_StopFindFile(pThis->m_lFileHandle);
    pThis->m_lFileHandle = -1;
    pThis->m_bSearching = FALSE;
    return 0;
}


// CDlgFlowInfo 消息处理程序

void CDlgFlowInfo::OnBnClickedBtnSearch()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);

    char szLan[128] = {0};
    if (m_iDeviceIndex < 0 || m_lLoginID < 0)
    {
        return;
    }

    if (!m_bSearching)
    {
        m_bQuit = FALSE;
        /*
        memset(&m_struFindCond, 0, sizeof(m_struFindCond));
        m_struFindCond.dwSize = sizeof(m_struFindCond);
        m_struFindCond.enumSearchType = ENUM_SEARCH_FLOW_INFO;
        m_struFindCond.dwMaxFileCountPer = 10;

        int nSel = m_cmbFlowType.GetCurSel();
        if (nSel == CB_ERR)
        {
            AfxMessageBox("please choose flow type");
            return;
        }
        CString strSearchMode;
        m_cmbFlowType.GetLBText(nSel, strSearchMode);
        if (0 == strSearchMode.Compare("year"))
        {
            m_struFindCond.unionSearchParam.struFlowParam.bySearchMode = 1;
        }
        else if (0 == strSearchMode.Compare("month"))
        {
            m_struFindCond.unionSearchParam.struFlowParam.bySearchMode = 2;
        }
        else if (0 == strSearchMode.Compare("day"))
        {
            m_struFindCond.unionSearchParam.struFlowParam.bySearchMode = 3;
        }

        m_struFindCond.struStartTime.wYear = (WORD)m_dateStart.GetYear();
        m_struFindCond.struStartTime.byMonth = (BYTE)m_dateStart.GetMonth();
        m_struFindCond.struStartTime.byDay = (BYTE)m_dateStart.GetDay();
        m_struFindCond.struStartTime.byHour = (BYTE)m_timeStart.GetHour();
        m_struFindCond.struStartTime.byMinute = (BYTE)m_timeStart.GetMinute();
        m_struFindCond.struStartTime.bySecond = (BYTE)m_timeStart.GetSecond();

        m_struFindCond.struStopTime.wYear = (WORD)m_dateStop.GetYear();
        m_struFindCond.struStopTime.byMonth = (BYTE)m_dateStop.GetMonth();
        m_struFindCond.struStopTime.byDay = (BYTE)m_dateStop.GetDay();
        m_struFindCond.struStopTime.byHour = (BYTE)m_timeStop.GetHour();
        m_struFindCond.struStopTime.byMinute = (BYTE)m_timeStop.GetMinute();
        m_struFindCond.struStopTime.bySecond = (BYTE)m_timeStop.GetSecond();

        m_lFileHandle = NET_ECMS_StartFindFile(m_lLoginID, &m_struFindCond);
        */

        int nSel = m_cmbFlowType.GetCurSel();
        if (nSel == CB_ERR)
        {
            AfxMessageBox("please choose flow type");
            return;
        }

        NET_EHOME_FLOW_COND struFlowCond = { 0 };
        struFlowCond.bySearchMode = (BYTE)(nSel + 1);
        struFlowCond.byLocalOrUTC = (BYTE)m_bTimeType;
        struFlowCond.struStartTime.wYear = (WORD)m_dateStart.GetYear();
        struFlowCond.struStartTime.byMonth = (BYTE)m_dateStart.GetMonth();
        struFlowCond.struStartTime.byDay = (BYTE)m_dateStart.GetDay();
        struFlowCond.struStartTime.byHour = (BYTE)m_timeStart.GetHour();
        struFlowCond.struStartTime.byMinute = (BYTE)m_timeStart.GetMinute();
        struFlowCond.struStartTime.bySecond = (BYTE)m_timeStart.GetSecond();

        struFlowCond.struStopTime.wYear = (WORD)m_dateStop.GetYear();
        struFlowCond.struStopTime.byMonth = (BYTE)m_dateStop.GetMonth();
        struFlowCond.struStopTime.byDay = (BYTE)m_dateStop.GetDay();
        struFlowCond.struStopTime.byHour = (BYTE)m_timeStop.GetHour();
        struFlowCond.struStopTime.byMinute = (BYTE)m_timeStop.GetMinute();
        struFlowCond.struStopTime.bySecond = (BYTE)m_timeStop.GetSecond();
        struFlowCond.dwMaxFileCountPer = 8;
        m_lFileHandle = NET_ECMS_StartFindFile_V11(m_lLoginID, ENUM_SEARCH_FLOW_INFO, &struFlowCond, sizeof(struFlowCond));
        if (m_lFileHandle < 0)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartFindFile");
            g_StringLanType(szLan, "查找文件列表失败!", "Fail to get file list");
            AfxMessageBox(szLan);
            return;
        }
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartFindFile");

        m_listFlowInfo.DeleteAllItems();
        DWORD dwThreadId;
        if (m_hFileThread == NULL)
        {
            m_hFileThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetFileThread), this, 0, &dwThreadId);
            if (m_hFileThread == NULL)
            {
                g_StringLanType(szLan, "打开查找线程失败!", "Fail to open finding thread!");
                AfxMessageBox(szLan);
                return;
            }
        }

        g_StringLanType(szLan, "停止查找", "Stop Searching");
        GetDlgItem(IDC_BTN_SEARCH__FLOW_INFO)->SetWindowText(szLan);
        m_bSearching = TRUE;
        GetDlgItem(IDC_STATIC_FINDING_FLOW_INFO)->ShowWindow(SW_SHOW);
    }
    else
    {
        if (m_hFileThread)
        {
            m_bQuit = TRUE;
        }
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
        NET_ECMS_StopFindFile(m_lFileHandle);
        g_StringLanType(szLan, "查找", "Search");
        GetDlgItem(IDC_BTN_SEARCH__FLOW_INFO)->SetWindowText(szLan);
        m_bSearching = FALSE;
        GetDlgItem(IDC_STATIC_FINDING_FLOW_INFO)->ShowWindow(SW_HIDE);
        m_dwFileNum = 0;
    }
}
BOOL CDlgFlowInfo::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    DWORD dwExStyle = m_listFlowInfo.GetExtendedStyle();
    dwExStyle |=LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
    m_listFlowInfo.SetExtendedStyle(dwExStyle);

    char szLan[128] = {0};
    g_StringLanType(szLan, "序号", "No.");
    m_listFlowInfo.InsertColumn(0, szLan, LVCFMT_LEFT, 40);
    g_StringLanType(szLan, "索引", "Index");
    m_listFlowInfo.InsertColumn(1, szLan, LVCFMT_LEFT, 40);
    g_StringLanType(szLan, "流量值", "Flow Value");
    m_listFlowInfo.InsertColumn(2, szLan, LVCFMT_LEFT, 80);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgFlowInfo::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    m_bQuit = TRUE;
    if (m_hFileThread)
    {
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
    }
    if (m_lFileHandle > -1)
    {
        NET_ECMS_StopFindFile(m_lFileHandle);
        m_lFileHandle = -1;
    }
    m_bSearching = FALSE;
}
