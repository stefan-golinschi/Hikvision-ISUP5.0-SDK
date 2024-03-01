// DlgDevLog.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgDevLog.h"
#include "afxdialogex.h"
//#include "HCNetSDK.h"
#include "LogType.h"


// CDlgDevLog 对话框

IMPLEMENT_DYNAMIC(CDlgDevLog, CDialog)

CDlgDevLog::CDlgDevLog(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDevLog::IDD, pParent)
{

}

CDlgDevLog::~CDlgDevLog()
{
}

void CDlgDevLog::DoDataExchange(CDataExchange* pDX)
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
UINT CDlgDevLog::GetFileThread(LPVOID pParam)
{
    CDlgDevLog *pLogSearch = static_cast<CDlgDevLog*>(pParam);
    LONG lRet = -1;
    //NET_EHOME_FINDDATA struFileInfo;
    // NET_EHOME_DEV_LOG struFileInfo;
    NET_EHOME_DEV_LOG struDevLog = { 0 };

    //memset(&m_DevLog, 0, sizeof(m_DevLog));
    CString csTmp;
    char szLan[128] = { 0 };


    while (!pLogSearch->m_bQuit)
    {
        lRet = NET_ECMS_FindNextFile_V11(pLogSearch->m_lFileHandle, &struDevLog, sizeof(struDevLog));
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
            case 4:
                g_StringLanType(szLan, "信息", "Information");
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

BOOL CDlgDevLog::CheckInitParam()
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


BEGIN_MESSAGE_MAP(CDlgDevLog, CDialog)
    ON_BN_CLICKED(IDC_BTN_SEARCH_DEV_LOG, &CDlgDevLog::OnBnClickedBtnSearchDevLog)
END_MESSAGE_MAP()


// CDlgDevLog 消息处理程序


/** @fn void CDlgDevLog::OnBnClickedBtnSearchDevLog()
 *  @brief  search log
 *  @return void
 */
void CDlgDevLog::OnBnClickedBtnSearchDevLog()
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

        memset(&m_DevLogCond, 0, sizeof(m_DevLogCond));
 
        
        m_DevLogCond.dwMajorType = m_comboMajorType.GetCurSel();
        m_DevLogCond.dwMinorType = m_comboMinorType.GetItemData(m_comboMinorType.GetCurSel());
        if (m_DevLogCond.dwMajorType == 0)
        {
            m_DevLogCond.dwMajorType = 0xffff;
        }
        if (m_DevLogCond.dwMinorType == 0)
        {
            m_DevLogCond.dwMinorType = 0xffff;
        }
       

        m_DevLogCond.dwStartIndex = 0;
        m_DevLogCond.dwMaxFileCountPer = 8;

        m_DevLogCond.struStartTime.wYear = (WORD)m_ctDateStart.GetYear();
        m_DevLogCond.struStartTime.byMonth = (BYTE)m_ctDateStart.GetMonth();
        m_DevLogCond.struStartTime.byDay = (BYTE)m_ctDateStart.GetDay();
        m_DevLogCond.struStartTime.byHour = (BYTE)m_ctTimeStart.GetHour();
        m_DevLogCond.struStartTime.byMinute = (BYTE)m_ctTimeStart.GetMinute();
        m_DevLogCond.struStartTime.bySecond = (BYTE)m_ctTimeStart.GetSecond();

        m_DevLogCond.struStopTime.wYear = (WORD)m_ctDateStop.GetYear();
        m_DevLogCond.struStopTime.byMonth = (BYTE)m_ctDateStop.GetMonth();
        m_DevLogCond.struStopTime.byDay = (BYTE)m_ctDateStop.GetDay();
        m_DevLogCond.struStopTime.byHour = (BYTE)m_ctTimeStop.GetHour();
        m_DevLogCond.struStopTime.byMinute = (BYTE)m_ctTimeStop.GetMinute();
        m_DevLogCond.struStopTime.bySecond = (BYTE)m_ctTimeStop.GetSecond();

        // m_lFileHandle = NET_ECMS_StartFindFile(m_lLoginID, &m_struFindCond);
        m_lFileHandle = NET_ECMS_StartFindFile_V11(m_lLoginID, ENUM_SEARCH_DEV_LOG, &m_DevLogCond, sizeof(m_DevLogCond));
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
        GetDlgItem(IDC_BTN_SEARCH_DEV_LOG)->SetWindowText(szLan);
        m_bSearching = FALSE;
       // GetDlgItem(IDC_STATIC_FINDING_PIC_FILE)->ShowWindow(SW_HIDE);
        m_dwLogNum = 0;
    }
}


BOOL CDlgDevLog::OnInitDialog()
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
    m_comboMajorType.SetCurSel(0);

    AddMinorString();

    CheckInitParam();
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}

/*********************************************************
Function:	AddMinorString
Desc:		add the minor type to combo
Input:	none
Output:	none
Return:	none
**********************************************************/
void CDlgDevLog::AddMinorString()
{
    CString szTemp;
    int iSel = 0;
    m_comboMinorType.ResetContent();
    m_comboMinorType.AddString("All");
    m_comboMinorType.SetItemData(iSel, 0);
    m_comboMinorType.SetCurSel(0);

    szTemp.Format("ALARM_IN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ALARM_IN);

    szTemp.Format("ALARM_OUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ALARM_OUT);

    szTemp.Format("MOTDET_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_MOTDET_START);

    szTemp.Format("MOTDET_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_MOTDET_STOP);

    szTemp.Format("HIDE_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HIDE_ALARM_START);

    szTemp.Format("HIDE_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HIDE_ALARM_STOP);

    szTemp.Format("VCA_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_START);

    szTemp.Format("VCA_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_STOP);

    //场景变更侦测日志查询2013-07-17
    szTemp.Format("VCA_SECNECHANGE_DETECTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_SECNECHANGE_DETECTION);

    szTemp.Format("WIRELESS_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_WIRELESS_ALARM_START);

    szTemp.Format("WIRELESS_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_WIRELESS_ALARM_STOP);

    szTemp.Format("PIR_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PIR_ALARM_START);

    szTemp.Format("PIR_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PIR_ALARM_STOP);

    szTemp.Format("CALLHELP_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CALLHELP_ALARM_START);

    szTemp.Format("CALLHELP_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CALLHELP_ALARM_STOP);

    szTemp.Format("DETECTFACE_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DETECTFACE_ALARM_START);

    szTemp.Format("DETECTFACE_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DETECTFACE_ALARM_STOP);

    szTemp.Format("DETECTFACE_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DETECTFACE_ALARM_STOP);

#ifdef NET_DVR_SET_TRAVERSE_PLANE_DETECTION
    szTemp.Format("TRAVERSE_PLANE_DETECTION start");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_LINE_DETECTION_BEGIN);

    szTemp.Format("TRAVERSE_PLANE_DETECTION end");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_LINE_DETECTION_END);
#endif //NET_DVR_SET_TRAVERSE_PLANE_DETECTION

    szTemp.Format("field start");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_INTRUDE_BEGIN);

    szTemp.Format("field end");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_INTRUDE_END);

    szTemp.Format("Audio Exception");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_AUDIOINPUT);

    szTemp.Format("Voice ABNORMAL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_AUDIOABNORMAL);

    szTemp.Format("defous detection start");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_DEFOCUS_DETECTION_BEGIN);

    szTemp.Format("defous detection end");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_DEFOCUS_DETECTION_END);

    szTemp.Format("external alarm");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_EXT_ALARM);

    szTemp.Format("ALARM_VCA_LECTURE_DETECTION_BEGIN");//2014-03-21
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_LECTURE_DETECTION_BEGIN);

    szTemp.Format("ALARM_VCA_LECTURE_DETECTION_END");//2014-03-21
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_LECTURE_DETECTION_END);

    szTemp.Format("ALARM_AUDIOSTEEPDROP");//2014-03-21
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_AUDIOSTEEPDROP);

    szTemp.Format("ALARM_VCA_ANSWER_DETECTION_BEGIN");//2014-03-21
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ANSWER_DETECTION_BEGIN);

    szTemp.Format("ALARM_VCA_ANSWER_DETECTION_END");//2014-03-21
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ANSWER_DETECTION_END);

    szTemp.Format("SMART_REGION_ENTRANCE_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_REGION_ENTRANCE_BEGIN);

    szTemp.Format("SMART_REGION_ENTRANCE_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_REGION_ENTRANCE_END);

    szTemp.Format("SMART_PEOPLE_GATHERING_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_PEOPLE_GATHERING_BEGIN);

    szTemp.Format("SMART_PEOPLE_GATHERING_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_PEOPLE_GATHERING_END);

    szTemp.Format("SMART_FAST_MOVING_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_FAST_MOVING_BEGIN);

    szTemp.Format("SMART_FAST_MOVING_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_FAST_MOVING_END);

    szTemp.Format("SMART_REGION_EXITING_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_REGION_EXITING_BEGIN);

    szTemp.Format("SMART_REGION_EXITING_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_REGION_EXITING_END);

    szTemp.Format("SMART_LOITERING_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_LOITERING_BEGIN);

    szTemp.Format("SMART_LOITERING_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_LOITERING_END);

    szTemp.Format("SMART_PARKING_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_PARKING_BEGIN);

    szTemp.Format("SMART_PARKING_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_PARKING_END);

    szTemp.Format("SMART_UNATTENDED_BAGGAGE_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_UNATTENDED_BAGGAGE_BEGIN);

    szTemp.Format("SMART_UNATTENDED_BAGGAGE_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_UNATTENDED_BAGGAGE_END);


    szTemp.Format("SMART_OBJECT_REMOVAL_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_OBJECT_REMOVAL_BEGIN);

    szTemp.Format("SMART_OBJECT_REMOVAL_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_OBJECT_REMOVAL_END);

    szTemp.Format("THERMAL_FIREDETECTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_FIREDETECTION);

    szTemp.Format("THERMAL_FIREDETECTION_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_FIREDETECTION_END);

    szTemp.Format("MINOR_SMART_VANDALPROOF_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_VANDALPROOF_BEGIN);

    szTemp.Format("MINOR_SMART_VANDALPROOF_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_VANDALPROOF_END);

    szTemp.Format("MINOR_FACESNAP_MATCH_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_FACESNAP_MATCH_ALARM_START);

    szTemp.Format("MINOR_FACESNAP_MATCH_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_FACESNAP_MATCH_ALARM_STOP);

    szTemp.Format("MINOR_WHITELIST_FACESNAP_MATCH_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_WHITELIST_FACESNAP_MATCH_ALARM_START);

    szTemp.Format("MINOR_WHITELIST_FACESNAP_MATCH_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_WHITELIST_FACESNAP_MATCH_ALARM_STOP);

    szTemp.Format("%s", "MINOR_POS_START_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_POS_START_ALARM);

    szTemp.Format("%s", "MINOR_POS_END_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_POS_END_ALARM);

    szTemp.Format("%s", "MINOR_SAFETY_HELMET_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SAFETY_HELMET_ALARM_START);

    szTemp.Format("%s", "MINOR_SAFETY_HELMET_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SAFETY_HELMET_ALARM_STOP);

    szTemp.Format("%s", "MINOR_HFPD_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HFPD_ALARM_START);

    szTemp.Format("%s", "MINOR_HFPD_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HFPD_ALARM_STOP);

    szTemp.Format("%s", "MINOR_MIXED_TARGET_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_MIXED_TARGET_ALARM_START);

    szTemp.Format("%s", "MINOR_MIXED_TARGET_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_MIXED_TARGET_ALARM_STOP);

    szTemp.Format("%s", "MINOR_VCA_GET_UP_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_GET_UP_ALARM_BEGIN);

    szTemp.Format("%s", "MINOR_VCA_GET_UP_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_GET_UP_ALARM_END);

    szTemp.Format("%s", "MINOR_VCA_ADV_REACH_HEIGHT_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ADV_REACH_HEIGHT_ALARM_BEGIN);

    szTemp.Format("%s", "MINOR_VCA_ADV_REACH_HEIGHT_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ADV_REACH_HEIGHT_ALARM_END);

    szTemp.Format("%s", "MINOR_VCA_TOILET_TARRY_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_TOILET_TARRY_ALARM_BEGIN);

    szTemp.Format("%s", "MINOR_VCA_TOILET_TARRY_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_TOILET_TARRY_ALARM_END);

    szTemp.Format("%s", "MINOR_HUMAN_RECOGNITION_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HUMAN_RECOGNITION_ALARM_BEGIN);

    szTemp.Format("%s", "MINOR_HUMAN_RECOGNITION_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HUMAN_RECOGNITION_ALARM_END);

    szTemp.Format("%s", "MINOR_STUDENTS_STOODUP_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STUDENTS_STOODUP_ALARM_BEGIN);

    szTemp.Format("%s", "MINOR_STUDENTS_STOODUP_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STUDENTS_STOODUP_ALARM_END);

    szTemp.Format("%s", "MINOR_FRAMES_PEOPLE_COUNTING_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_FRAMES_PEOPLE_COUNTING_ALARM);

    szTemp.Format("%s", "MINOR_FACE_SNAP_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_FACE_SNAP_ALARM_BEGIN);

    szTemp.Format("%s", "MINOR_FACE_SNAP_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_FACE_SNAP_ALARM_END);

    szTemp.Format("%s", "MINOR_FACE_THERMOMETRY_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_FACE_THERMOMETRY_ALARM);

    szTemp.Format("MINOR_HEART_RATE_ABNORMAL_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HEART_RATE_ABNORMAL_BEGIN);

    szTemp.Format("MINOR_HEART_RATE_ABNORMAL_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HEART_RATE_ABNORMAL_END);

    szTemp.Format("MINOR_BLOOD_OXYGEN_ABNORMAL_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_BLOOD_OXYGEN_ABNORMAL_BEGIN);

    szTemp.Format("MINOR_BLOOD_OXYGEN_ABNORMAL_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_BLOOD_OXYGEN_ABNORMAL_END);

    szTemp.Format("MINOR_SYSTOLIC_BLOOD_PRESSURE_ABNORMAL_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SYSTOLIC_BLOOD_PRESSURE_ABNORMAL_BEGIN);

    szTemp.Format("MINOR_SYSTOLIC_BLOOD_PRESSURE_ABNORMAL_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SYSTOLIC_BLOOD_PRESSURE_ABNORMAL_END);

    szTemp.Format("MINOR_DIASTOLIC_BLOOD_PRESSURE_ABNORMAL_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DIASTOLIC_BLOOD_PRESSURE_ABNORMAL_BEGIN);

    szTemp.Format("MINOR_DIASTOLIC_BLOOD_PRESSURE_ABNORMAL_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DIASTOLIC_BLOOD_PRESSURE_ABNORMAL_END);

    szTemp.Format("MINOR_TEACHER_BEHAVIOR_DETECT_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_TEACHER_BEHAVIOR_DETECT_ALARM_BEGIN);

    szTemp.Format("MINOR_TEACHER_BEHAVIOR_DETECT_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_TEACHER_BEHAVIOR_DETECT_ALARM_END);

    szTemp.Format("MINOR_PERIMETER_CAPTURE_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PERIMETER_CAPTURE_ALARM_BEGIN);

    szTemp.Format("MINOR_PERIMETER_CAPTURE_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PERIMETER_CAPTURE_ALARM_END);

    szTemp.Format("MINOR_UNREGISTERED_STREET_VENDOR_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_UNREGISTERED_STREET_VENDOR_ALARM);

    szTemp.Format("MINOR_VCA_LEAVE_POSITION_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_LEAVE_POSITION_START);

    szTemp.Format("MINOR_VCA_LEAVE_POSITION_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_LEAVE_POSITION_STOP);

    szTemp.Format("MINOR_VCA_STOOODUP_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_STOOODUP_START);

    szTemp.Format("MINOR_VCA_STOOODUP_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_STOOODUP_STOP);

    szTemp.Format("MINOR_VCA_PEOPLENUM_CHANGE_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_PEOPLENUM_CHANGE_START);

    szTemp.Format("MINOR_VCA_PEOPLENUM_CHANGE_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_PEOPLENUM_CHANGE_STOP);

    szTemp.Format("MINOR_VCA_RUNNING_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_RUNNING_START);

    szTemp.Format("MINOR_VCA_RUNNING_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_RUNNING_STOP);

    szTemp.Format("MINOR_VCA_VIOLENT_MOTION_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_VIOLENT_MOTION_START);

    szTemp.Format("MINOR_VCA_VIOLENT_MOTION_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_VIOLENT_MOTION_STOP);

    szTemp.Format("MINOR_VCA_FAIL_DOWN_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_FAIL_DOWN_START);

    szTemp.Format("MINOR_VCA_FAIL_DOWN_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_FAIL_DOWN_STOP);

    szTemp.Format("MINOR_VCA_RETENTION_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_RETENTION_START);

    szTemp.Format("MINOR_VCA_RETENTION_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_RETENTION_STOP);

    szTemp.Format("MINOR_VCA_SITUATION_ANALYSIS_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_SITUATION_ANALYSIS_START);

    szTemp.Format("MINOR_VCA_SITUATION_ANALYSIS_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_SITUATION_ANALYSIS_STOP);

    szTemp.Format("MINOR_UNREGISTERED_STREET_VENDOR_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_UNREGISTERED_STREET_VENDOR_ALARM);

    szTemp.Format("MINOR_PERSON_QUEUE_TIME_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PERSON_QUEUE_TIME_ALARM_BEGIN);

    szTemp.Format("MINOR_PERSON_QUEUE_TIME_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PERSON_QUEUE_TIME_ALARM_END);

    szTemp.Format("MINOR_PERSON_QUEUE_COUNTING_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PERSON_QUEUE_COUNTING_ALARM_BEGIN);

    szTemp.Format("MINOR_PERSON_QUEUE_COUNTING_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PERSON_QUEUE_COUNTING_ALARM_END);

    szTemp.Format("MINOR_ACCESS_CONTROLLER_EVENT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ACCESS_CONTROLLER_EVENT);

    szTemp.Format("MINOR_VIDEO_INTERCOM_EVENT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VIDEO_INTERCOM_EVENT);

    szTemp.Format("MINOR_GJD_EVENT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_GJD_EVENT);

    szTemp.Format("MINOR_LUMINITE_EVENT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LUMINITE_EVENT);

    szTemp.Format("MINOR_OPTEX_EVENT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_OPTEX_EVENT);

    szTemp.Format("MINOR_CAMERA_DETECTOR_EVENT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CAMERA_DETECTOR_EVENT);

    szTemp.Format("MINOR_SECURITY_CONTROL_PANEL_EVENT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SECURITY_CONTROL_PANEL_EVENT);

    szTemp.Format("MINOR_VCA_SPACE_CHANGE_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_SPACE_CHANGE_START);

    szTemp.Format("MINOR_VCA_SPACE_CHANGE_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_SPACE_CHANGE_STOP);

    szTemp.Format("MINOR_MANUAL_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_MANUAL_ALARM);

    szTemp.Format("MINOR_DETECTOR_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DETECTOR_ALARM);

    szTemp.Format("MINOR_LINKAGE_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LINKAGE_ALARM);

    szTemp.Format("MINOR_REMOTE_CREATE_STORAGE_POOL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CREATE_STORAGE_POOL);

    szTemp.Format("MINOR_REMOTE_DEL_STORAGE_POOL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DEL_STORAGE_POOL);

    szTemp.Format("MINOR_REMOTE_CLOUD_ENABLE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CLOUD_ENABLE);

    szTemp.Format("MINOR_REMOTE_CLOUD_DISABLE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CLOUD_DISABLE);

    szTemp.Format("MINOR_REMOTE_CLOUD_MODIFY_PARAM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CLOUD_MODIFY_PARAM);

    szTemp.Format("MINOR_REMOTE_CLOUD_MODIFY_VOLUME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CLOUD_MODIFY_VOLUME);


    //exception
    szTemp.Format("---Excp----------");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, 0);

    szTemp.Format("RAID_ERROR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RAID_ERROR);

    szTemp.Format("VI_LOST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VI_LOST);

    szTemp.Format("ILLEGAL_ACCESS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ILLEGAL_ACCESS);

    szTemp.Format("HD_FULL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HD_FULL);

    szTemp.Format("HD_ERROR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HD_ERROR);

    szTemp.Format("REC_ERROR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REC_ERROR);

    szTemp.Format("DCD_LOST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DCD_LOST);

    szTemp.Format("IPC_NO_LINK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_NO_LINK);

    szTemp.Format("IP_CONFLICT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IP_CONFLICT);

    szTemp.Format("NET_BROKEN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_NET_BROKEN);

    szTemp.Format("VI_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VI_EXCEPTION);

    szTemp.Format("IPC_IP_CONFLICT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_IP_CONFLICT);

    szTemp.Format("SENCE_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SENCE_EXCEPTION);

    szTemp.Format("RESOLUTION_MISMATCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RESOLUTION_MISMATCH);

    szTemp.Format("RECORD_OVERFLOW");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RECORD_OVERFLOW);

    szTemp.Format("DIAL_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DIAL_EXCEPTION);

    szTemp.Format("POE_POWER_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_POE_POWER_EXCEPTION);

    szTemp.Format("MINOR_UPLOAD_DATA_CS_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_UPLOAD_DATA_CS_EXCEPTION);

    szTemp.Format("AI_LOST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_AI_LOST);

    szTemp.Format("MINOR_ACCESSORIES_PLATE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ACCESSORIES_PLATE);

    szTemp.Format("MINOR_KMS_EXPAMSION_DISK_LOST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_KMS_EXPAMSION_DISK_LOST);

    szTemp.Format("MINOR_ABNORMAL_PORT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ABNORMAL_PORT);

    szTemp.Format("MINOR_CAMERA_ANGLE_ANOMALY");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CAMERA_ANGLE_ANOMALY);

    szTemp.Format("MINOR_DATA_DISK_ERROE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DATA_DISK_ERROE);

    szTemp.Format("MINOR_INTELLIGENT_SYSTEM_RUNNING_ERROR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_INTELLIGENT_SYSTEM_RUNNING_ERROR);

    szTemp.Format("MINOR_FACESNAP_RESOLUTION_OVERFLOW");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_FACESNAP_RESOLUTION_OVERFLOW);

    szTemp.Format("MINOR_SMD_RESOLUTION_OVERFLOW");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMD_RESOLUTION_OVERFLOW);

    szTemp.Format("MINOR_AUDIO_LOSS_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_AUDIO_LOSS_EXCEPTION);

    szTemp.Format("MINOR_SAFETY_HELMET_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SAFETY_HELMET_EXCEPTION);

    szTemp.Format("MINOR_VCA_PIC_LENGTH_OVERFLOW");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_PIC_LENGTH_OVERFLOW);

    szTemp.Format("MINOR_FACE_MODEL_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_FACE_MODEL_EXCEPTION);

    szTemp.Format("MINOR_CLUSTER_DEVICE_OFFLINE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_DEVICE_OFFLINE);

    szTemp.Format("MINOR_CLUSTER_CONFIG_FAILED");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_CONFIG_FAILED);

    szTemp.Format("MINOR_CLUSTER_DISASTER_TOLERANCE_EXCEPT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_DISASTER_TOLERANCE_EXCEPT);

    szTemp.Format("MINOR_CLUSTER_STORFULL_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_STORFULL_EXCEPTION);

    szTemp.Format("MINOR_CLUSTER_VERSION_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_VERSION_EXCEPTION);

    szTemp.Format("MINOR_CLUSTER_OFFLINENODE_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_OFFLINENODE_EXCEPTION);

    szTemp.Format("MINOR_CLUSTER_RECORDCYCLE_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_RECORDCYCLE_EXCEPTION);

    szTemp.Format("MINOR_CLUSTER_IPCTRANSFER_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_IPCTRANSFER_EXCEPTION);

    szTemp.Format("MINOR_CLUSTER_IPCONFLICT_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CLUSTER_IPCONFLICT_EXCEPTION);

    szTemp.Format("MINOR_GET_SUB_STREAM_FAILURE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_GET_SUB_STREAM_FAILURE);

    szTemp.Format("MINOR_HDD_SHM_DETECT_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HDD_SHM_DETECT_EXCEPTION);

    szTemp.Format("MINOR_DEVICE_FORTIFY_FAILURE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DEVICE_FORTIFY_FAILURE);

    szTemp.Format("MINOR_EVENT_UPLOAD_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_EVENT_UPLOAD_EXCEPTION);

    szTemp.Format("MINOR_MAIN_POWER_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_MAIN_POWER_FAULT);

    szTemp.Format("MINOR_BACK_UP_POWER_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_BACK_UP_POWER_FAULT);

    szTemp.Format("MINOR_TAMPER_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_TAMPER_FAULT);

    szTemp.Format("MINOR_RS232_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RS232_FAULT);

    szTemp.Format("MINOR_RS485_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RS485_FAULT);

    szTemp.Format("MINOR_LAN_STATUS_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LAN_STATUS_FAULT);

    szTemp.Format("MINOR_LAN_LINK1_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LAN_LINK1_FAULT);

    szTemp.Format("MINOR_LAN_LINK2_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LAN_LINK2_FAULT);

    szTemp.Format("MINOR_SIM_CARD_STATUS_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SIM_CARD_STATUS_FAULT);

    szTemp.Format("MINOR_4G_LINK1_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_4G_LINK1_FAULT);

    szTemp.Format("MINOR_4G_LINK2_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_4G_LINK2_FAULT);

    szTemp.Format("MINOR_OTHER_FAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_OTHER_FAULT);

    //operation
    szTemp.Format("%s", "---Oper----------");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, 0);
    szTemp.Format("START_DVR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_START_DVR);

    szTemp.Format("STOP_DVR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STOP_DVR);

    szTemp.Format("STOP_ABNORMAL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STOP_ABNORMAL);

    szTemp.Format("REBOOT_DVR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REBOOT_DVR);

    szTemp.Format("LOCAL_LOGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_LOGIN);

    szTemp.Format("%s", "LOCAL_LOGOUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_LOGOUT);

    szTemp.Format("LOCAL_CFG_PARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFG_PARM);

    szTemp.Format("%s", "LOCAL_PLAYBYFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PLAYBYFILE);

    szTemp.Format("%s", "LOCAL_PLAYBYTIME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PLAYBYTIME);

    szTemp.Format("%s", "LOCAL_START_REC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_START_REC);

    szTemp.Format("%s", "LOCAL_STOP_REC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_STOP_REC);

    szTemp.Format("%s", "LOCAL_PTZCTRL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PTZCTRL);

    szTemp.Format("%s", "LOCAL_PREVIEW");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PREVIEW);

    szTemp.Format("%s", "LOCAL_MODIFY_TIME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MODIFY_TIME);

    szTemp.Format("%s", "LOCAL_UPGRADE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_UPGRADE);

    szTemp.Format("%s", "LOCAL_COPYFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COPYFILE);


    szTemp.Format("%s", "LOCAL_LOCKFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_LOCKFILE);

    szTemp.Format("%s", "LOCAL_UNLOCKFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_UNLOCKFILE);

    szTemp.Format("%s", "LOCAL_FORMAT_HDD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FORMAT_HDD);

    szTemp.Format("%s", "LOCAL_CFGFILE_OUTPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFGFILE_OUTPUT);

    szTemp.Format("%s", "LOCAL_CFGFILE_INPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFGFILE_INPUT);

    szTemp.Format("%s", "LOCAL_DVR_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DVR_ALARM);
    szTemp.Format("%s", "IPC_ADD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_ADD);

    szTemp.Format("%s", "IPC_DEL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_DEL);

    szTemp.Format("%s", "IPC_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_SET);

    m_comboMinorType.AddString("LOCAL_START_BACKUP");
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_START_BACKUP);
    m_comboMinorType.AddString("LOCAL_STOP_BACKUP");
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_STOP_BACKUP);
    m_comboMinorType.AddString("LOCAL_COPYFILE_START_TIME");
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COPYFILE_START_TIME);
    m_comboMinorType.AddString("LOCAL_COPYFILE_END_TIME");
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COPYFILE_END_TIME);
    //m_comboMinorType.AddString("LOCAL_COPYFILE_END_TIME");

    szTemp.Format("%s", "LOCAL_OPERATE_LOCK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_OPERATE_LOCK);

    szTemp.Format("%s", "LOCAL_OPERATE_UNLOCK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_OPERATE_UNLOCK);

    szTemp.Format("%s", "LOCAL_TAG_OPT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_TAG_OPT);

    szTemp.Format("%s", "LOCAL_VOUT_SWITCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_VOUT_SWITCH);

    szTemp.Format("%s", "STREAM_CABAC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STREAM_CABAC);

    szTemp.Format("%s", "LOCAL_LOAD_HDISK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_LOAD_HDISK);

    szTemp.Format("%s", "LOCAL_DELETE_HDISK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DELETE_HDISK);

    szTemp.Format("%s", "LOCAL_MAIN_AUXILIARY_PORT_SWITCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MAIN_AUXILIARY_PORT_SWITCH);

    szTemp.Format("%s", "LOCAL_HARD_DISK_CHECK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_HARD_DISK_CHECK);

    szTemp.Format("%s", "LOCAL_PIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PIN);

    szTemp.Format("%s", "LOCAL_DIAL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DIAL);

    //ITS的操作日志 bg
    szTemp.Format("%s", "LOCAL_ADD_CAR_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_CAR_INFO);

    szTemp.Format("%s", "LOCAL_MOD_CAR_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_CAR_INFO);

    szTemp.Format("%s", "LOCAL_DEL_CAR_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_CAR_INFO);

    szTemp.Format("%s", "LOCAL_FIND_CAR_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_CAR_INFO);

    szTemp.Format("%s", "LOCAL_ADD_MONITOR_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_MONITOR_INFO);

    szTemp.Format("%s", "LOCAL_MOD_MONITOR_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_MONITOR_INFO);

    szTemp.Format("%s", "LOCAL_DEL_MONITOR_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_MONITOR_INFO);

    szTemp.Format("%s", "LOCAL_FIND_MONITOR_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_MONITOR_INFO);

    szTemp.Format("%s", "LOCAL_FIND_NORMAL_PASS_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_NORMAL_PASS_INFO);

    szTemp.Format("%s", "LOCAL_FIND_ABNORMAL_PASS_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_ABNORMAL_PASS_INFO);

    szTemp.Format("%s", "LOCAL_FIND_PEDESTRIAN_PASS_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_PEDESTRIAN_PASS_INFO);

    szTemp.Format("%s", "LOCAL_PIC_PREVIEW");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PIC_PREVIEW);

    szTemp.Format("%s", "LOCAL_SET_GATE_PARM_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_GATE_PARM_CFG);

    szTemp.Format("%s", "LOCAL_GET_GATE_PARM_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_GET_GATE_PARM_CFG);

    szTemp.Format("%s", "LOCAL_SET_DATAUPLOAD_PARM_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_DATAUPLOAD_PARM_CFG);

    szTemp.Format("%s", "LOCAL_GET_DATAUPLOAD_PARM_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_GET_DATAUPLOAD_PARM_CFG);

    //2013-11-19
    szTemp.Format("%s", "LOCAL_DEVICE_CONTROL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEVICE_CONTROL);

    szTemp.Format("%s", "LOCAL_ADD_EXTERNAL_DEVICE_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_EXTERNAL_DEVICE_INFO);

    szTemp.Format("%s", "LOCAL_MOD_EXTERNAL_DEVICE_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_EXTERNAL_DEVICE_INFO);

    szTemp.Format("%s", "LOCAL_DEL_EXTERNAL_DEVICE_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_EXTERNAL_DEVICE_INFO);

    szTemp.Format("%s", "LOCAL_FIND_EXTERNAL_DEVICE_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_EXTERNAL_DEVICE_INFO);

    szTemp.Format("%s", "LOCAL_ADD_CHARGE_RULE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_CHARGE_RULE);

    szTemp.Format("%s", "LOCAL_MOD_CHARGE_RULE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_CHARGE_RULE);

    szTemp.Format("%s", "LOCAL_DEL_CHARGE_RULE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_CHARGE_RULE);

    szTemp.Format("%s", "LOCAL_FIND_CHARGE_RULE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_CHARGE_RULE);

    szTemp.Format("%s", "LOCAL_COUNT_NORMAL_CURRENTINFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COUNT_NORMAL_CURRENTINFO);

    szTemp.Format("%s", "LOCAL_EXPORT_NORMAL_CURRENTINFO_REPORT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_EXPORT_NORMAL_CURRENTINFO_REPORT);

    szTemp.Format("%s", "LOCAL_COUNT_ABNORMAL_CURRENTINFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COUNT_ABNORMAL_CURRENTINFO);

    szTemp.Format("%s", "LOCAL_EXPORT_ABNORMAL_CURRENTINFO_REPORT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_EXPORT_ABNORMAL_CURRENTINFO_REPORT);

    szTemp.Format("%s", "LOCAL_COUNT_PEDESTRIAN_CURRENTINFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COUNT_PEDESTRIAN_CURRENTINFO);

    szTemp.Format("%s", "LOCAL_EXPORT_PEDESTRIAN_CURRENTINFO_REPORT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_EXPORT_PEDESTRIAN_CURRENTINFO_REPORT);

    szTemp.Format("%s", "LOCAL_FIND_CAR_CHARGEINFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_CAR_CHARGEINFO);

    szTemp.Format("%s", "LOCAL_COUNT_CAR_CHARGEINFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COUNT_CAR_CHARGEINFO);

    szTemp.Format("%s", "LOCAL_EXPORT_CAR_CHARGEINFO_REPORT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_EXPORT_CAR_CHARGEINFO_REPORT);

    szTemp.Format("%s", "LOCAL_FIND_SHIFTINFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_SHIFTINFO);

    szTemp.Format("%s", "LOCAL_FIND_CARDINFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_CARDINFO);

    szTemp.Format("%s", "LOCAL_ADD_RELIEF_RULE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_RELIEF_RULE);

    szTemp.Format("%s", "LOCAL_MOD_RELIEF_RULE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_RELIEF_RULE);

    szTemp.Format("%s", "LOCAL_DEL_RELIEF_RULE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_RELIEF_RULE);

    szTemp.Format("%s", "LOCAL_FIND_RELIEF_RULE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_RELIEF_RULE);

    szTemp.Format("%s", "LOCAL_GET_ENDETCFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_GET_ENDETCFG);

    szTemp.Format("%s", "LOCAL_SET_ENDETCFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_ENDETCFG);

    szTemp.Format("%s", "LOCAL_SET_ENDEV_ISSUEDDATA");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_ENDEV_ISSUEDDATA);

    szTemp.Format("%s", "LOCAL_DEL_ENDEV_ISSUEDDATA");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_ENDEV_ISSUEDDATA);
    //ITS的操作日志 ed
    //wireless Operation start
    szTemp.Format("%s", "LOCAL_SMS_SEND");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SMS_SEND);

    szTemp.Format("%s", "ALARM_SMS_SEND");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ALARM_SMS_SEND);

    szTemp.Format("%s", "SMS_RECV");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMS_RECV);

    szTemp.Format("%s", "LOCAL_SMS_SEARCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SMS_SEARCH);

    szTemp.Format("%s", "LOCAL_SMS_READ");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SMS_READ);

    szTemp.Format("%s", "LOCAL_WHITELIST_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_WHITELIST_SET);

    szTemp.Format("%s", "LOCAL_DIAL_PARA_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DIAL_PARA_SET);

    szTemp.Format("%s", "LOCAL_DIAL_SCHEDULE_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DIAL_SCHEDULE_SET);

    szTemp.Format("%s", "PLAT_OPER");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PLAT_OPER);

    szTemp.Format("%s", "LOCAL_SET_DEVICE_ACTIVE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_DEVICE_ACTIVE);

    szTemp.Format("%s", "LOCAL_PARA_FACTORY_DEFAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PARA_FACTORY_DEFAULT);

    szTemp.Format("%s", "LOCAL_RESTORE_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_RESTORE_CFG);
    //wireless Operation end
    ////remote 
    szTemp.Format("%s", "---Remote----------");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, 0);

    szTemp.Format("%s", "REMOTE_LOGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_LOGIN);

    szTemp.Format("%s", "REMOTE_LOGOUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_LOGOUT);

    szTemp.Format("%s", "REMOTE_START_REC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_START_REC);

    szTemp.Format("%s", "REMOTE_STOP_REC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_STOP_REC);

    szTemp.Format("%s", "START_TRANS_CHAN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_START_TRANS_CHAN);

    szTemp.Format("%s", "STOP_TRANS_CHAN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STOP_TRANS_CHAN);

    szTemp.Format("REMOTE_GET_PARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_PARM);

    szTemp.Format("REMOTE_CFG_PARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFG_PARM);

    szTemp.Format("%s", "REMOTE_GET_STATUS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_STATUS);

    szTemp.Format("%s", "REMOTE_ARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ARM);

    szTemp.Format("%s", "REMOTE_DISARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DISARM);

    szTemp.Format("%s", "REMOTE_REBOOT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_REBOOT);

    szTemp.Format("%s", "START_VT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_START_VT);

    szTemp.Format("%s", "STOP_VT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STOP_VT);

    szTemp.Format("%s", "REMOTE_UPGRADE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_UPGRADE);

    szTemp.Format("%s", "REMOTE_PLAYBYFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PLAYBYFILE);

    szTemp.Format("%s", "REMOTE_PLAYBYTIME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PLAYBYTIME);

    szTemp.Format("%s", "REMOTE_PTZCTRL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PTZCTRL);

    szTemp.Format("%s", "REMOTE_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_STOP);


    szTemp.Format("%s", "REMOTE_LOCKFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_LOCKFILE);

    szTemp.Format("%s", "REMOTE_CFGFILE_OUTPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFGFILE_OUTPUT);


    szTemp.Format("%s", "REMOTE_CFGFILE_INTPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFGFILE_INTPUT);

    szTemp.Format("%s", "REMOTE_FORMAT_HDD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FORMAT_HDD);
    szTemp.Format("%s", "REMOTE_DVR_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DVR_ALARM);
    szTemp.Format("%s", "REMOTE_IPC_ADD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IPC_ADD);

    szTemp.Format("%s", "REMOTE_IPC_DEL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IPC_DEL);

    szTemp.Format("%s", "REMOTE_IPC_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IPC_SET);

    szTemp.Format("%s", "REBOOT_VCA_LIB");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REBOOT_VCA_LIB);

    szTemp.Format("%s", "REMOTE_INQUEST_DEL_FILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_INQUEST_DEL_FILE);

    szTemp.Format("%s", "LOCAL_UNLOAD_HDISK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_UNLOAD_HDISK);

    szTemp.Format("%s", "LOCAL_AUDIO_MIX");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_AUDIO_MIX);

    szTemp.Format("%s", "REMOTE_AUDIO_MIX");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_AUDIO_MIX);

    szTemp.Format("%s", "LOCAL_TRIAL_PAUSE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_TRIAL_PAUSE);

    szTemp.Format("%s", "LOCAL_TRIAL_RESUME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_TRIAL_RESUME);

    szTemp.Format("%s", "REMOTE_TRIAL_PAUSE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_TRIAL_PAUSE);

    szTemp.Format("%s", "REMOTE_TRIAL_RESUME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_TRIAL_RESUME);

    szTemp.Format("%s", "MINOR_REMOTE_MODIFY_VERIFICATION_CODE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_MODIFY_VERIFICATION_CODE);

    szTemp.Format("%s", "MINOR_LOCAL_MAKECALL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MAKECALL);

    szTemp.Format("%s", "MINOR_LOCAL_REJECTCALL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_REJECTCALL);

    szTemp.Format("%s", "MINOR_LOCAL_ANSWERCALL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ANSWERCALL);

    szTemp.Format("%s", "MINOR_LOCAL_HANGUPCALL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_HANGUPCALL);

    szTemp.Format("%s", "MINOR_REMOTE_MAKECALL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_MAKECALL);

    szTemp.Format("%s", "MINOR_REMOTE_REJECTCALL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_REJECTCALL);

    szTemp.Format("%s", "MINOR_REMOTE_ANSWERCALL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ANSWERCALL);

    szTemp.Format("%s", "MINOR_REMOTE_HANGUPCALL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_HANGUPCALL);

    szTemp.Format("%s", "LOCAL_DELETE_HDISK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DELETE_HDISK);

    szTemp.Format("%s", "LOCAL_MAIN_AUXILIARY_PORT_SWITCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MAIN_AUXILIARY_PORT_SWITCH);

    szTemp.Format("%s", "LOCAL_HARD_DISK_CHECK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_HARD_DISK_CHECK);

    szTemp.Format("%s", "REMOTE_DELETE_HDISK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DELETE_HDISK);

    szTemp.Format("%s", "REMOTE_LOAD_HDISK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_LOAD_HDISK);

    szTemp.Format("%s", "REMOTE_UNLOAD_HDISK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_UNLOAD_HDISK);

    szTemp.Format("%s", "SMS_CONTROL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMS_CONTROL);

    szTemp.Format("%s", "CALL_ONLINE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CALL_ONLINE);

    szTemp.Format("%s", "REMOTE_PIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PIN);

    //ITS操作日志类型 Remote bg
    szTemp.Format("%s", "REMOTE_DEVICE_CONTROL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DEVICE_CONTROL);

    szTemp.Format("%s", "REMOTE_SET_GATE_PARM_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_GATE_PARM_CFG);

    szTemp.Format("%s", "REMOTE_GET_GATE_PARM_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_GATE_PARM_CFG);

    szTemp.Format("%s", "REMOTE_SET_DATAUPLOAD_PARM_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_DATAUPLOAD_PARM_CFG);

    szTemp.Format("%s", "REMOTE_GET_DATAUPLOAD_PARM_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_DATAUPLOAD_PARM_CFG);

    szTemp.Format("%s", "REMOTE_GET_BASE_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_BASE_INFO);

    szTemp.Format("%s", "REMOTE_GET_OVERLAP_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_OVERLAP_CFG);

    szTemp.Format("%s", "REMOTE_SET_OVERLAP_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_OVERLAP_CFG);

    szTemp.Format("%s", "REMOTE_GET_ROAD_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_ROAD_INFO);

    szTemp.Format("%s", "REMOTE_START_TRANSCHAN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_START_TRANSCHAN);

    szTemp.Format("%s", "REMOTE_GET_ECTWORKSTATE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_ECTWORKSTATE);

    szTemp.Format("%s", "REMOTE_GET_ECTCHANINFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_ECTCHANINFO);

    //2013-11-19
    szTemp.Format("%s", "REMOTE_ADD_EXTERNAL_DEVICE_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ADD_EXTERNAL_DEVICE_INFO);

    szTemp.Format("%s", "REMOTE_MOD_EXTERNAL_DEVICE_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_MOD_EXTERNAL_DEVICE_INFO);

    szTemp.Format("%s", "REMOTE_GET_ENDETCFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_ENDETCFG);

    szTemp.Format("%s", "REMOTE_SET_ENDETCFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_ENDETCFG);

    szTemp.Format("%s", "REMOTE_ENDEV_ISSUEDDATA");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ENDEV_ISSUEDDATA);

    szTemp.Format("%s", "REMOTE_DEL_ENDEV_ISSUEDDATA");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DEL_ENDEV_ISSUEDDATA);

    //日志类型（新增）2013-06-09
    szTemp.Format("%s", "REMOTE_ON_CTRL_LAMP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ON_CTRL_LAMP);

    szTemp.Format("%s", "REMOTE_OFF_CTRL_LAMP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_OFF_CTRL_LAMP);
    //ITS操作日志类型 Remote ed
    //remote wireless Operation start
    szTemp.Format("%s", "REMOTE_SMS_SEND");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SMS_SEND);

    szTemp.Format("%s", "REMOTE_SMS_SEARCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SMS_SEARCH);

    szTemp.Format("%s", "REMOTE_SMS_READ");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SMS_READ);

    szTemp.Format("%s", "REMOTE_DIAL_DISCONN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DIAL_DISCONN);

    szTemp.Format("%s", "REMOTE_DIAL_CONNECT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DIAL_CONNECT);

    szTemp.Format("%s", "REMOTE_WHITELIST_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_WHITELIST_SET);

    szTemp.Format("%s", "REMOTE_DIAL_PARA_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DIAL_PARA_SET);

    szTemp.Format("%s", "REMOTE_DIAL_SCHEDULE_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DIAL_SCHEDULE_SET);

    szTemp.Format("%s", "REMOTE_SET_DEVICE_ACTIVE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_DEVICE_ACTIVE);

    szTemp.Format("%s", "MINOR_REMOTE_CFG_POE_WORK_MODE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFG_POE_WORK_MODE);

    szTemp.Format("%s", "MINOR_LOCAL_CFG_POE_WORK_MODE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFG_POE_WORK_MODE);

    szTemp.Format("%s", "MINOR_REMOTE_CFG_FACE_CONTRAST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFG_FACE_CONTRAST);

    szTemp.Format("%s", "MINOR_LOCAL_CFG_FACE_CONTRAST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFG_FACE_CONTRAST);

    szTemp.Format("%s", "MINOR_REMOTE_CFG_WHITELIST_FACE_CONTRAST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFG_WHITELIST_FACE_CONTRAST);

    szTemp.Format("%s", "MINOR_LOCAL_CFG_WHITELIST_FACE_CONTRAST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFG_WHITELIST_FACE_CONTRAST);

    szTemp.Format("%s", "REMOTE_PARA_FACTORY_DEFAULT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PARA_FACTORY_DEFAULT);

    szTemp.Format("%s", "REMOTE_RESTORE_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_RESTORE_CFG);

    szTemp.Format("%s", "MINOR_LOCAL_CHECK_TIME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CHECK_TIME);

    szTemp.Format("%s", "MINOR_VCA_ONEKEY_EXPORT_PICTURE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ONEKEY_EXPORT_PICTURE);

    szTemp.Format("%s", "MINOR_VCA_ONEKEY_DELETE_PICTURE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ONEKEY_DELETE_PICTURE);

    szTemp.Format("%s", "MINOR_VCA_ONEKEY_EXPORT_VIDEO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ONEKEY_EXPORT_VIDEO);

    szTemp.Format("%s", "MINOR_VCA_ONEKEY_DELETE_VIDEO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ONEKEY_DELETE_VIDEO);

    //remote wireless Operation end

    //information
    szTemp.Format("%s", "--INFOR-------------");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, 0);

    szTemp.Format("%s", "HDD_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HDD_INFO);

    szTemp.Format("%s", "SMART_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_INFO);

    szTemp.Format("%s", "REC_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REC_START);

    szTemp.Format("%s", "REC_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REC_STOP);

    szTemp.Format("%s", "REC_OVERDUE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REC_OVERDUE);

    szTemp.Format("%s", "LINK_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LINK_START);

    szTemp.Format("%s", "LINK_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LINK_STOP);

    szTemp.Format("%s", "NET_DISK_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_NET_DISK_INFO);

    szTemp.Format("%s", "RAID_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RAID_INFO);

    szTemp.Format("%s", "RUN_STATUS_INFO ");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RUN_STATUS_INFO);

    szTemp.Format("%s", "PLAT_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PLAT_INFO);

    szTemp.Format("%s", "DIAL_STAT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DIAL_STAT);

    szTemp.Format("%s", "UNLOCK_RECORD ");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_UNLOCK_RECORD);

    szTemp.Format("%s", "ZONE_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VIS_ALARM);

    szTemp.Format("%s", "MINOR_TALK_RECORD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_TALK_RECORD);

    szTemp.Format("%s", "MINOR_ACCESSORIES_MESSAGE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ACCESSORIES_MESSAGE);

    szTemp.Format("%s", "MINOR_THERMAL_SHIPSDETECTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_SHIPSDETECTION);

    szTemp.Format("%s", "MINOR_KMS_EXPAMSION_DISK_INSERT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_KMS_EXPAMSION_DISK_INSERT);

    szTemp.Format("%s", "MINOR_IPC_CONNECT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_CONNECT);

    szTemp.Format("%s", "MINOR_INTELLIGENT_BOARD_STATUS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_INTELLIGENT_BOARD_STATUS);

    szTemp.Format("%s", "MINOR_IPC_CONNECT_STATUS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_CONNECT_STATUS);

    szTemp.Format("%s", "MINOR_AUTO_TIMING");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_AUTO_TIMING);

    szTemp.Format("%s", "MINOR_EZVIZ_OPERATION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_EZVIZ_OPERATION);

    szTemp.Format("%s", "MINOR_VOICE_START_DETECTED");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VOICE_START_DETECTED);

    szTemp.Format("%s", "MINOR_VOICE_END_DETECTED");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VOICE_END_DETECTED);

    szTemp.Format("%s", "MINOR_DOUBLE_VERIFICATION_PASS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DOUBLE_VERIFICATION_PASS);

    szTemp.Format("%s", "MINOR_THERMAL_THERMOMETRY_EARLYWARNING_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_THERMOMETRY_EARLYWARNING_BEGIN);

    szTemp.Format("%s", "MINOR_THERMAL_THERMOMETRY_EARLYWARNING_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_THERMOMETRY_EARLYWARNING_END);

    szTemp.Format("%s", "MINOR_THERMAL_THERMOMETRY_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_THERMOMETRY_ALARM_BEGIN);

    szTemp.Format("%s", "MINOR_THERMAL_THERMOMETRY_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_THERMOMETRY_ALARM_END);

    szTemp.Format("%s", "MINOR_THERMAL_THERMOMETRY_DIFF_ALARM_BEGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_THERMOMETRY_DIFF_ALARM_BEGIN);

    szTemp.Format("%s", "MINOR_THERMAL_THERMOMETRY_DIFF_ALARM_END");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_THERMAL_THERMOMETRY_DIFF_ALARM_END);

    szTemp.Format("%s", "MINOR_LOCAL_RESET_LOGIN_PASSWORD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_RESET_LOGIN_PASSWORD);

    szTemp.Format("%s", "MINOR_REMOTE_RESET_LOGIN_PASSWORD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_RESET_LOGIN_PASSWORD);

    szTemp.Format("%s", "MINOR_REMOTE_FACE_BASE_CREATE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FACE_BASE_CREATE);

    szTemp.Format("%s", "MINOR_REMOTE_FACE_BASE_MODIFY");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FACE_BASE_MODIFY);

    szTemp.Format("%s", "MINOR_REMOTE_FACE_BASE_DELETE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FACE_BASE_DELETE);

    szTemp.Format("%s", "MINOR_REMOTE_FACE_DATA_APPEND");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FACE_DATA_APPEND);

    szTemp.Format("%s", "MINOR_REMOTE_FACE_DATA_SEARCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FACE_DATA_SEARCH);

    szTemp.Format("%s", "MINOR_REMOTE_FACE_DATA_ANALYSIS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FACE_DATA_ANALYSIS);

    szTemp.Format("%s", "MINOR_REMOTE_FACE_DATA_EDIT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FACE_DATA_EDIT);

    szTemp.Format("%s", "MINOR_REMOTE_FACE_DATA_DELET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FACE_DATA_DELET);

    szTemp.Format("%s", "MINOR_REMOTE_VCA_ANALYSIS_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_VCA_ANALYSIS_CFG);

    szTemp.Format("%s", "MINOR_LOCAL_FACE_BASE_IMPORT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_BASE_IMPORT);

    szTemp.Format("%s", "MINOR_LOCAL_FACE_BASE_EXPORT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_BASE_EXPORT);

    szTemp.Format("%s", "MINOR_LOCAL_FACE_BASE_CREATE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_BASE_CREATE);

    szTemp.Format("%s", "MINOR_LOCAL_FACE_BASE_MODIFY");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_BASE_MODIFY);


    szTemp.Format("%s", "MINOR_LOCAL_FACE_BASE_DELETE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_BASE_DELETE);


    szTemp.Format("%s", "MINOR_LOCAL_FACE_DATA_APPEND");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_DATA_APPEND);


    szTemp.Format("%s", "MINOR_LOCAL_FACE_DATA_SEARCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_DATA_SEARCH);

    szTemp.Format("%s", "MINOR_LOCAL_FACE_DATA_ANALYSIS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_DATA_ANALYSIS);

    szTemp.Format("%s", "MINOR_LOCAL_FACE_DATA_EDIT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_DATA_EDIT);

    szTemp.Format("%s", "MINOR_LOCAL_FACE_DATA_DELETE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FACE_DATA_DELETE);

    szTemp.Format("%s", "MINOR_LOCAL_VCA_ANALYSIS_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_VCA_ANALYSIS_CFG);

    szTemp.Format("%s", "MINOR_REMOTE_HFPD_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_HFPD_CFG);

    szTemp.Format("%s", "MINOR_REMOTE_IOTCFGFILE_INPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IOTCFGFILE_INPUT);

    szTemp.Format("%s", "MINOR_REMOTE_IOTCFGFILE_OUTPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IOTCFGFILE_OUTPUT);

    szTemp.Format("%s", "MINOR_LOCAL_IOT_ADD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_IOT_ADD);

    szTemp.Format("%s", "MINOR_REMOTE_IOT_ADD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IOT_ADD);

    szTemp.Format("%s", "MINOR_LOCAL_IOT_DEL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_IOT_DEL);

    szTemp.Format("%s", "MINOR_REMOTE_IOT_DEL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IOT_DEL);

    szTemp.Format("%s", "MINOR_LOCAL_IOT_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_IOT_SET);

    szTemp.Format("%s", "MINOR_REMOTE_IOT_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IOT_SET);

    szTemp.Format("%s", "MINOR_LOCAL_IOTCFGFILE_INPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_IOTCFGFILE_INPUT);

    szTemp.Format("%s", "MINOR_LOCAL_IOTCFGFILE_OUTPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_IOTCFGFILE_OUTPUT);

    szTemp.Format("%s", "MINOR_LOCAL_VAD_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_VAD_CFG);

    szTemp.Format("%s", "MINOR_REMOTE_VAD_CFG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_VAD_CFG);

    szTemp.Format("%s", "MINOR_LOCAL_ADDRESS_FILTER_CONFIG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADDRESS_FILTER_CONFIG);

    szTemp.Format("%s", "MINOR_REMOTE_ADDRESS_FILTER_CONFIG");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ADDRESS_FILTER_CONFIG);
}

