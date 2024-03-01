// DlgDevLog.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgAlarmHOstLog.h"
#include "afxdialogex.h"


// CDlgDevLog 对话框

IMPLEMENT_DYNAMIC(CDlgAlarmHostLog, CDialog)

CDlgAlarmHostLog::CDlgAlarmHostLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmHostLog::IDD, pParent)
{

}

CDlgAlarmHostLog::~CDlgAlarmHostLog()
{
}

void CDlgAlarmHostLog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_LOG_MAJOR_TYPE, m_comboMajorType);
    DDX_Control(pDX, IDC_COMBO_LOG_MINOR_TYPE, m_comboMinorType);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_LOG_DATE_START, m_ctDateStart);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_LOG_DATE_STOP, m_ctDateStop);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_LOG_TIME_START, m_ctTimeStart);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_LOG_TIME_STOP, m_ctTimeStop);
    DDX_Control(pDX, IDC_LIST_DEV_LOG, m_listDeviceLog);
}


/** @fn UINT CDlgDevLog::GetFileThread(LPVOID pParam)
 *  @brief  
 *  @param (in)	LPVOID pParam    
 *  @return UINT
 */
UINT CDlgAlarmHostLog::GetFileThread(LPVOID pParam)
{
    CDlgAlarmHostLog *pLogSearch = static_cast<CDlgAlarmHostLog*>(pParam);
    LONG lRet = -1;
    //NET_EHOME_FINDDATA struFileInfo;
    // NET_EHOME_DEV_LOG struFileInfo;
    NET_EHOME_DEV_LOG struDevLog = { 0 };

    //memset(&m_DevLog, 0, sizeof(m_DevLog));
    CString csTmp;
    char szLan[128] = { 0 };


    while (!pLogSearch->m_bQuit)
    {
        lRet = lRet = NET_ECMS_FindNextFile_V11(pLogSearch->m_lFileHandle, &struDevLog, sizeof(struDevLog));
        if (lRet == ENUM_GET_NEXT_STATUS_SUCCESS)
        {
            csTmp.Format("%d", pLogSearch->m_dwLogNum);
            pLogSearch->m_listDeviceLog.InsertItem(pLogSearch->m_dwLogNum, csTmp, 0);
            csTmp.Format("%04d-%02d-%02d %02d:%02d:%02d", struDevLog.struLogTime.wYear, struDevLog.struLogTime.byMonth, struDevLog.struLogTime.byDay, \
                struDevLog.struLogTime.byHour, struDevLog.struLogTime.byMinute, struDevLog.struLogTime.bySecond);
            pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_dwLogNum, 1, csTmp);
            switch (struDevLog.dwMajorType)
            {
            case 1:
                g_StringLanType(szLan, "报警", "Alarm");
                csTmp.Format("%s", szLan);
                break;
            case 2:
                g_StringLanType(szLan, "异常", "Abnormal");
                csTmp.Format("%s", szLan);
                break;
            case 3:
                g_StringLanType(szLan, "操作", "Operation");
                csTmp.Format("%s", szLan);
                break;
            default:
                g_StringLanType(szLan, "未知类型", "Unknown");
                csTmp.Format("%s", szLan);
                break;
            }
            
            pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_dwLogNum, 2, csTmp);
            //次类型
            csTmp.Format("0x%x", struDevLog.dwMinorType);
            pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_dwLogNum, 3, csTmp);

            pLogSearch->m_dwLogNum++;

      
        }
        else
        {
            if (lRet == ENUM_GET_NETX_STATUS_NEED_WAIT)
            {
                Sleep(5);
                continue;
            }

            if ((lRet == ENUM_GET_NETX_STATUS_NO_FILE) || (lRet == ENUM_GET_NEXT_STATUS_FINISH))
            {
                g_StringLanType(szLan, "查找", "Search");
                pLogSearch->GetDlgItem(IDC_BTN_SEARCH_DEV_LOG)->SetWindowText(szLan);
                pLogSearch->m_bSearching = FALSE;
              //  pLogSearch->GetDlgItem(IDC_STATIC_FINDING_PIC_FILE)->ShowWindow(SW_HIDE);
                g_pMainDlg->AddLog(pLogSearch->m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_FindNextFile file num[%d]", pLogSearch->m_dwLogNum);
                pLogSearch->m_dwLogNum = 0;
                break;
            }
            else if (lRet == ENUM_GET_NEXT_STATUS_NOT_SUPPORT)
            {
                g_StringLanType(szLan, "查找", "Search");
                pLogSearch->GetDlgItem(IDC_BTN_SEARCH_DEV_LOG)->SetWindowText(szLan);
                pLogSearch->m_bSearching = FALSE;
              //  pLogSearch->GetDlgItem(IDC_STATIC_FINDING_PIC_FILE)->ShowWindow(SW_HIDE);
                g_pMainDlg->AddLog(pLogSearch->m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_FindNextFile");
                g_StringLanType(szLan, "设备不支持该操作", "Device do not support");
                AfxMessageBox(szLan);
                pLogSearch->m_dwLogNum = 0;
                break;
            }
            else
            {
                g_StringLanType(szLan, "查找", "Search");
                pLogSearch->GetDlgItem(IDC_BTN_SEARCH_DEV_LOG)->SetWindowText(szLan);
                pLogSearch->m_bSearching = FALSE;
               // pLogSearch->GetDlgItem(IDC_STATIC_FINDING_PIC_FILE)->ShowWindow(SW_HIDE);
                g_pMainDlg->AddLog(pLogSearch->m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_FindNextFile");
                g_StringLanType(szLan, "由于服务器忙,或网络故障,获取文件列表异常终止", "Since the server is busy, or network failure, abnormal termination of access to the file list");
                AfxMessageBox(szLan);
                pLogSearch->m_dwLogNum = 0;
                break;
            }
        }
    }
    CloseHandle(pLogSearch->m_hFileThread);
    pLogSearch->m_hFileThread = NULL;
    NET_ECMS_StopFindFile(pLogSearch->m_lFileHandle);
    pLogSearch->m_lFileHandle = -1;
    pLogSearch->m_bSearching = FALSE;
    return 0;
}

BOOL CDlgAlarmHostLog::CheckInitParam()
{
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex == -1)
    {
        UpdateData(FALSE);
        return FALSE;
    }

    if (m_iDeviceIndex == -1 || m_iDeviceIndex != iDeviceIndex)
    {
        m_iChanIndex = -1;
    }

    int iChanIndex = g_pMainDlg->GetCurChanIndex();
    if (iChanIndex == -1)
    {
        //g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "set the channel index = 0");
        //iChanIndex = 0;
        AfxMessageBox("please select a channel!");
        UpdateData(FALSE);
        return FALSE;
    }

    UpdateData(TRUE);

    m_iDeviceIndex = iDeviceIndex;
    m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;

    //initialize parameters again while switching channel
    if (m_iChanIndex != iChanIndex)
    {
        m_iChanIndex = iChanIndex;


        m_bSearching = FALSE;

        //m_hPareDlgWnd = NULL;
        //m_hPareTabWnd = NULL;

        m_hFileThread = NULL;
        m_dwLogNum = 0;

    }

    if (m_lLoginID < 0)
    {
        UpdateData(FALSE);
        return FALSE;
    }

    return TRUE;
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostLog, CDialog)
    ON_BN_CLICKED(IDC_BTN_SEARCH_DEV_LOG, &CDlgAlarmHostLog::OnBnClickedBtnSearchDevLog)
END_MESSAGE_MAP()


// CDlgDevLog 消息处理程序


/** @fn void CDlgDevLog::OnBnClickedBtnSearchDevLog()
 *  @brief  search log
 *  @return void
 */
void CDlgAlarmHostLog::OnBnClickedBtnSearchDevLog()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    char szLan[128] = { 0 };
    if (m_iDeviceIndex < 0)
    {
        g_StringLanType(szLan, "请先登录设备!", "Please Login First!");
        AfxMessageBox(szLan);
        return;
    }

    if (!m_bSearching)
    {
        m_bQuit = FALSE;

        memset(&m_struAlarmHostLogCond, 0, sizeof(m_struAlarmHostLogCond));
 
        
        m_struAlarmHostLogCond.dwMajorType = m_comboMajorType.GetItemData(m_comboMajorType.GetCurSel());
        m_struAlarmHostLogCond.dwMinorType = m_comboMinorType.GetItemData(m_comboMinorType.GetCurSel());

        if (m_struAlarmHostLogCond.dwMajorType == 0)
        {
            m_struAlarmHostLogCond.dwMajorType = 0xffff;
        }

        m_struAlarmHostLogCond.dwStartIndex = 0;
        m_struAlarmHostLogCond.dwMaxFileCountPer = 8;

        m_struAlarmHostLogCond.struStartTime.wYear = (WORD)m_ctDateStart.GetYear();
        m_struAlarmHostLogCond.struStartTime.byMonth = (BYTE)m_ctDateStart.GetMonth();
        m_struAlarmHostLogCond.struStartTime.byDay = (BYTE)m_ctDateStart.GetDay();
        m_struAlarmHostLogCond.struStartTime.byHour = (BYTE)m_ctTimeStart.GetHour();
        m_struAlarmHostLogCond.struStartTime.byMinute = (BYTE)m_ctTimeStart.GetMinute();
        m_struAlarmHostLogCond.struStartTime.bySecond = (BYTE)m_ctTimeStart.GetSecond();

        m_struAlarmHostLogCond.struStopTime.wYear = (WORD)m_ctDateStop.GetYear();
        m_struAlarmHostLogCond.struStopTime.byMonth = (BYTE)m_ctDateStop.GetMonth();
        m_struAlarmHostLogCond.struStopTime.byDay = (BYTE)m_ctDateStop.GetDay();
        m_struAlarmHostLogCond.struStopTime.byHour = (BYTE)m_ctTimeStop.GetHour();
        m_struAlarmHostLogCond.struStopTime.byMinute = (BYTE)m_ctTimeStop.GetMinute();
        m_struAlarmHostLogCond.struStopTime.bySecond = (BYTE)m_ctTimeStop.GetSecond();

        // m_lFileHandle = NET_ECMS_StartFindFile(m_lLoginID, &m_struFindCond);
        m_lFileHandle = NET_ECMS_StartFindFile_V11(m_lLoginID, ENUM_SEARCH_ALARM_HOST_LOG, &m_struAlarmHostLogCond, sizeof(m_struAlarmHostLogCond));
        if (m_lFileHandle < 0)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartFindFile");
            g_StringLanType(szLan, "查找设备日志列表失败!", "Fail to get Dev log list");
            AfxMessageBox(szLan);
            return;
        }
        m_listDeviceLog.DeleteAllItems();
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
        GetDlgItem(IDC_BTN_SEARCH_DEV_LOG)->SetWindowText(szLan);
        m_bSearching = TRUE;

        //GetDlgItem(IDC_STATIC_FINDING_PIC_FILE)->ShowWindow(SW_SHOW);
    }
    else
    {
        if (m_hFileThread)
        {
            m_bQuit = TRUE;
            //TerminateThread(m_hFileThread, 0);
        }
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
        NET_ECMS_StopFindFile(m_lFileHandle);
        g_StringLanType(szLan, "查找", "Search");
        GetDlgItem(IDC_BTN_SEARCH_PIC_FILE)->SetWindowText(szLan);
        m_bSearching = FALSE;
        GetDlgItem(IDC_STATIC_FINDING_PIC_FILE)->ShowWindow(SW_HIDE);
        m_dwLogNum = 0;
    }
}


BOOL CDlgAlarmHostLog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CenterWindow();
    m_bSearch = FALSE;
    m_hFileThread = NULL;


    char szLan[128] = { 0 };

    CString tmp;
    tmp.Format("全部日志");
 
    tmp.Format("78K");
 


    m_listDeviceLog.SetExtendedStyle(m_listDeviceLog.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLan, "序号", "No.");
    m_listDeviceLog.InsertColumn(0, szLan, LVCFMT_RIGHT, 60, -1);
    g_StringLanType(szLan, "日志时间", "Log Time");
    m_listDeviceLog.InsertColumn(1, szLan, LVCFMT_LEFT, 140, -1);
    g_StringLanType(szLan, "主类型", "Major Type");
    m_listDeviceLog.InsertColumn(2, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "次类型", "Minor Type");
    m_listDeviceLog.InsertColumn(3, szLan, LVCFMT_LEFT, 150, -1);
    m_dwLogNum = 0;
    m_lLogHandle = -1;
    CTime ctCurTime = CTime::GetCurrentTime();
    CTime time(ctCurTime.GetYear(), ctCurTime.GetMonth(), ctCurTime.GetDay(), 0, 0, 0);
    CTime time0(ctCurTime.GetYear(), ctCurTime.GetMonth(), ctCurTime.GetDay(), 23, 59, 59);
    m_ctDateStart = time;
    m_ctTimeStart = time;
    m_ctDateStop = ctCurTime;
    m_ctTimeStop = time0;
    m_iMode = 2;
    m_iMajType = 0;
    m_iMinType = 0;

    int index = 0;
    m_comboMinorType.ResetContent();
    g_StringLanType(szLan, "全部", "All");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_ALL);
    index++;

    g_StringLanType(szLan, "定时录像", "Schedule");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_TIMING);
    index++;

    g_StringLanType(szLan, "移动侦测", "Motion Detect");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_MOTION_DETECT);
    index++;

    g_StringLanType(szLan, "报警触发", "Alarm");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_ALARMIN);
    index++;

    g_StringLanType(szLan, "报警|动测", "Alarm|Motion");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_MOTION_OR_ALARMIN);
    index++;

    g_StringLanType(szLan, "报警&动测", "Alarm&Motion");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_MOTION_AND_ALARMIN);
    index++;

    g_StringLanType(szLan, "命令触发", "Command Record");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_COMMAND);
    index++;

    g_StringLanType(szLan, "手动录像", "Manual Record");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_MANUAL);
    index++;

    g_StringLanType(szLan, "智能报警", "VCA Alarm");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, 7);
    index++;

    g_StringLanType(szLan, "PIR报警", "PIR Alarm");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_PIR_ALARM);
    index++;

    g_StringLanType(szLan, "无线报警", "WIRELESS Alarm");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_WIRELESS_ALARM);
    index++;

    g_StringLanType(szLan, "呼救报警", "CALLHELP Alarm");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_CALLHELP_ALARM);
    index++;

    g_StringLanType(szLan, "报警录像", "Alarm Record");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_PIR_WIRELESS_CALLHELP);
    index++;

    g_StringLanType(szLan, "智能交通事件报警", "Intelligent Transportation");
    m_comboMinorType.InsertString(index, szLan);
    m_comboMinorType.SetItemData(index, FILE_INTELLIGENT_TRANSPORT);
    index++;

    m_comboMajorType.SetCurSel(m_iMajType);
    m_comboMinorType.SetCurSel(m_iMinType);
    CheckInitParam();
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}


