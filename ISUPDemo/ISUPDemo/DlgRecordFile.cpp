// DlgRecordFile.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgRecordFile.h"
#include "plaympeg4.h"
#include "DlgCyclePlayBack.h"
#include "Public/Convert/Convert.h"
BOOL  g_bSound = FALSE;
BOOL  g_bSoundSwitchTag = FALSE;
;
//数据回调
BOOL CALLBACK  PlayBackDataCB(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_INFO *pDataCBInfo, void* pUserData)
{
//     char szOutput[128] = {0};
//     sprintf(szOutput, "data type[%d], data len[%d]\n", pDataCBInfo->dwType, pDataCBInfo->dwDataLen);
//     OutputDebugString(szOutput);

    UN_REFERENCED_PARAMETER(iPlayBackLinkHandle)

    if (NULL == pDataCBInfo)
    {
        return FALSE;
    }

    BYTE *pBuffer = pDataCBInfo->pData;
    DWORD dwBufSize = pDataCBInfo->dwDataLen;

    CDlgRecordFile *g_pDlgRemoteFile = (CDlgRecordFile*)pUserData;
    return g_pDlgRemoteFile->InputStreamData(pDataCBInfo->dwType, pDataCBInfo->pData, pDataCBInfo->dwDataLen);
}

//new link回调
BOOL CALLBACK PlayBackNewLinkCB(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_NEWLINK_CB_INFO *pNewLinkCBInfo, void* pUserData)
{
    CDlgRecordFile* pDlg = (CDlgRecordFile*)pUserData;
    UN_REFERENCED_PARAMETER(pNewLinkCBInfo)
        //OutputDebugString("NewLink\n");
        //设置数据回调

    pNewLinkCBInfo->pUserData = pUserData;
    pNewLinkCBInfo->fnPlayBackDataCB = PlayBackDataCB;
    pNewLinkCBInfo->byStreamFormat = 0;
    NET_EHOME_PLAYBACK_DATA_CB_PARAM struCBParam = { 0 };
    struCBParam.fnPlayBackDataCB = PlayBackDataCB;

    struCBParam.pUserData = pUserData;
    CDlgRecordFile *pThis = (CDlgRecordFile*)pUserData;
    pDlg->m_struPlayParam.lPlayBackHandle = iPlayBackLinkHandle;

    if (!NET_ESTREAM_SetPlayBackDataCB(iPlayBackLinkHandle, &struCBParam))
    {
        OutputDebugString("NET_ESTREAM_SetPlayBackDataCB Failed!\n");
    }


    pDlg->GetDlgItem(IDC_BUTTON_START_PLAY)->EnableWindow(FALSE);
    pDlg->GetDlgItem(IDC_BUTTON_STOP_PLAY)->EnableWindow(TRUE);
    return TRUE;
}

// CDlgRecordFile 对话框

IMPLEMENT_DYNAMIC(CDlgRecordFile, CDialog)

CDlgRecordFile::CDlgRecordFile(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgRecordFile::IDD, pParent)
    , m_iSessionID(-1)
    , m_bSave(FALSE)
    , m_bStart(FALSE)
    , m_pVideoFile(NULL)
    , m_PlayDateStart(COleDateTime::GetCurrentTime())
    , m_PlayTimeStart(COleDateTime::GetCurrentTime())
    , m_PlayDateStop(COleDateTime::GetCurrentTime())
    , m_PlayTimeStop(COleDateTime::GetCurrentTime())
    , m_byTimeType(0)
    , m_byRecordType(0)
    , m_bTypeTime(0)
	, m_bPause(0)
    , m_byEncrypt(FALSE)
    , m_byStreamType(0)
    , m_bHls(FALSE)
    , m_dwPlaybackPort(8888)
{
    m_lLoginID = -1;
    m_iDeviceIndex = -1;
    m_lPlayBackListenHandle = -1;
    //m_struPlayParam.hPlayWnd = GetDlgItem(IDC_STATIC_PLAY_WND)->m_hWnd;
    m_struPlayParam.lPlayBackHandle = -1;
    m_struPlayParam.lPort = -1;

    m_nFileNum = 0;
    memset(m_szFileName, 0, sizeof(m_szFileName));
}

CDlgRecordFile::~CDlgRecordFile()
{
    //OnBnClickedButtonStopPlay();
    if (m_iSessionID >=0)
    {
        NET_ECMS_StopPlayBack(m_lLoginID, m_iSessionID);
        m_iSessionID = -1;
    }
    //停止该路连接        
    if (m_struPlayParam.lPlayBackHandle != -1)        
    {            
        NET_ESTREAM_StopPlayBack(m_struPlayParam.lPlayBackHandle);            
        m_struPlayParam.lPlayBackHandle = -1;        
    }    

    //停止监听
    if (m_lPlayBackListenHandle != -1)
    {
        NET_ESTREAM_StopListenPlayBack(m_lPlayBackListenHandle);
        m_lPlayBackListenHandle = -1;
    }
    

    if (m_struPlayParam.lPort >= 0)
    {
        PlayM4_FreePort(m_struPlayParam.lPort);
        m_struPlayParam.lPort = -1;
    }

}

void CDlgRecordFile::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_REC_FILE, m_lstRecFile);
    DDX_Control(pDX, IDC_COMBO_REC_FILE_TYPE, m_cmbRecFileType);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_DATE_START, m_dateStart);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_TIME_START, m_timeStart);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_DATE_STOP, m_DateStop);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_TIME_STOP, m_timeStop);
    DDX_Control(pDX, IDC_STATIC_PLAY_WND, m_staticPlayWnd);
    DDX_Control(pDX, IDC_COMBO_SEEK_TYPE, m_cmbSeekType);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_DATE_START_PLAY, m_PlayDateStart);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_TIME_START_PLAY, m_PlayTimeStart);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_DATE_STOP_PLAY, m_PlayDateStop);
    DDX_DateTimeCtrl(pDX, IDC_COMBO_TIME_STOP_PLAY, m_PlayTimeStop);
    DDX_CBIndex(pDX, IDC_COMBO_TIME_TYPE, m_byTimeType);
    DDX_CBIndex(pDX, IDC_COMBO_RECORD_TYPE, m_byRecordType);
    DDX_CBIndex(pDX, IDC_COMBO_TIME_TYPE2, m_bTypeTime);
    DDX_Control(pDX, IDC_COMBO_PROTOCOL_TYPE, m_protocolType);
    DDX_Check(pDX, IDC_CHECK_ENCRYPT, m_byEncrypt);
    DDX_CBIndex(pDX, IDC_COMBO_STREAM_TYPE, m_byStreamType);
    DDX_Check(pDX, IDC_CHECK_HLS, m_bHls);
    DDX_Text(pDX, IDC_EDIT_PLAYBACKPORT, m_dwPlaybackPort);
}


BEGIN_MESSAGE_MAP(CDlgRecordFile, CDialog)
    ON_BN_CLICKED(IDC_BTN_SEARCH_REC_FILE, &CDlgRecordFile::OnBnClickedBtnSearchRecFile)
    ON_BN_CLICKED(IDC_BUTTON_CYCLE, &CDlgRecordFile::OnBnClickedButtonCycle)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_REC_FILE, &CDlgRecordFile::OnNMDblclkListRecFile)
    ON_BN_CLICKED(IDC_BUTTON_START_PLAY, &CDlgRecordFile::OnBnClickedButtonStartPlay)
    ON_BN_CLICKED(IDC_BUTTON_STOP_PLAY, &CDlgRecordFile::OnBnClickedButtonStopPlay)
    ON_STN_CLICKED(IDC_STATIC_PLAY_WND, &CDlgRecordFile::OnStnClickedStaticPlayWnd)
    ON_NOTIFY(NM_CLICK, IDC_LIST_REC_FILE, &CDlgRecordFile::OnNMClickListRecFile)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgRecordFile::OnBnClickedButtonSave)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CDlgRecordFile::OnBnClickedButtonPause)
    ON_BN_CLICKED(IDC_PLAYBACK_SOUND, &CDlgRecordFile::OnBnClickedPlaybackSound)
    ON_BN_CLICKED(IDC_BUTTON_FAST_PLAY, &CDlgRecordFile::OnBnClickedButtonFastPlay)
    ON_BN_CLICKED(IDC_BUTTON_SLOW_PLAY, &CDlgRecordFile::OnBnClickedButtonSlowPlay)
END_MESSAGE_MAP()


BOOL CDlgRecordFile::CheckInitParam()
{
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex == -1)
    {
        UpdateData(FALSE);
        return FALSE;
    }

    if ( m_iDeviceIndex == -1 || m_iDeviceIndex != iDeviceIndex)
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
        m_dwFileNum = 0;

    }

    if (m_lLoginID < 0)
    {
        UpdateData(FALSE);
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
UINT CDlgRecordFile::GetFileThread(LPVOID pParam)
{
    CDlgRecordFile *pThis = static_cast<CDlgRecordFile*>(pParam);
    LONG lRet = -1;
   // NET_EHOME_FINDDATA struFileInfo;
   // memset(&struFileInfo, 0, sizeof(struFileInfo));
    //struFileInfo.dwSize = sizeof(struFileInfo);

    NET_EHOME_REC_FILE struFileInfo;
    memset(&struFileInfo, 0, sizeof(struFileInfo));

    CString csTmp;
    char szLan[128] = {0};
   // struFileInfo.dwSize = sizeof(NET_EHOME_FINDDATA);
    while (!pThis->m_bQuit)
    {
        lRet = lRet = NET_ECMS_FindNextFile_V11(pThis->m_lFileHandle, &struFileInfo, sizeof(struFileInfo));
       // lRet = NET_ECMS_FindNextFile(pThis->m_lFileHandle, &struFileInfo);
        if (lRet == ENUM_GET_NEXT_STATUS_SUCCESS)
        {      
            if (pThis->m_nFileNum < 512)
            {

                memcpy(&pThis->m_struFindData, &struFileInfo, sizeof(struFileInfo));

                strcpy(pThis->m_szFileName[pThis->m_nFileNum], struFileInfo.sFileName);
                pThis->m_nFileNum++;

                pThis->m_lstRecFile.InsertItem(pThis->m_dwFileNum, struFileInfo.sFileName, 0);

                if (struFileInfo.dwFileSize / 1024 == 0)
                {
                    csTmp.Format("%d", struFileInfo.dwFileSize);
                }
                else if (struFileInfo.dwFileSize / 1024 > 0 && struFileInfo.dwFileSize / (1024 * 1024) == 0)
                {
                    csTmp.Format("%dK", struFileInfo.dwFileSize / 1024);
                }
                else// if ()
                {
                    csTmp.Format("%dM", struFileInfo.dwFileSize / 1024 / 1024);//different from hard disk capacity, files need tranformation
                }
                //csTmp.Format("%d",struFileInfo.dwFileSize);
                pThis->m_lstRecFile.SetItemText(pThis->m_dwFileNum, 1, csTmp);
                csTmp.Format("%04d%02d%02d%02d%02d%02d", struFileInfo.struStartTime.wYear, \
                    struFileInfo.struStartTime.byMonth, struFileInfo.struStartTime.byDay, \
                    struFileInfo.struStartTime.byHour, struFileInfo.struStartTime.byMinute, \
                    struFileInfo.struStartTime.bySecond);
                pThis->m_lstRecFile.SetItemText(pThis->m_dwFileNum, 2, csTmp);
                csTmp.Format("%04d%02d%02d%02d%02d%02d", struFileInfo.struStopTime.wYear, struFileInfo.struStopTime.byMonth, \
                    struFileInfo.struStopTime.byDay, struFileInfo.struStopTime.byHour, \
                    struFileInfo.struStopTime.byMinute, struFileInfo.struStopTime.bySecond);
                pThis->m_lstRecFile.SetItemText(pThis->m_dwFileNum, 3, csTmp);


                csTmp.Format("%d", struFileInfo.dwFileMainType);
                pThis->m_lstRecFile.SetItemText(pThis->m_dwFileNum, 4, csTmp);

                csTmp.Format("%d", struFileInfo.dwFileSubType);
                pThis->m_lstRecFile.SetItemText(pThis->m_dwFileNum, 5, csTmp);

                csTmp.Format("%d", struFileInfo.dwFileIndex);
                pThis->m_lstRecFile.SetItemText(pThis->m_dwFileNum, 6, csTmp);

                csTmp.Format("%d", struFileInfo.byTimeDiffH);
                pThis->m_lstRecFile.SetItemText(pThis->m_dwFileNum, 7, csTmp);

                csTmp.Format("%d", struFileInfo.byTimeDiffM);
                pThis->m_lstRecFile.SetItemText(pThis->m_dwFileNum, 8, csTmp);

            }
            
            
            pThis->m_dwFileNum++;
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
                pThis->GetDlgItem(IDC_BTN_SEARCH_REC_FILE)->SetWindowText(szLan);
                 pThis->m_bSearching = FALSE;
                 pThis->GetDlgItem(IDC_STATIC_FINDING_REC_FILE)->ShowWindow(SW_HIDE);
                 if (pThis->m_dwFileNum > 512)
                 {
                     g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, 1, "only show the first [512] data,the total num is [%d]", pThis->m_dwFileNum);
                 }
                 else
                 {
                     g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_FindNextFile file num[%d]", pThis->m_dwFileNum);
                 }
                pThis->m_dwFileNum = 0;
                break;
            }
            else if (lRet == ENUM_GET_NEXT_STATUS_NOT_SUPPORT)
            {
                pThis->GetDlgItem(IDC_BTN_SEARCH_REC_FILE)->SetWindowText("查找");
                pThis->m_bSearching = FALSE;
                pThis->GetDlgItem(IDC_BTN_SEARCH_REC_FILE)->ShowWindow(SW_HIDE);
                g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_FindNextFile");
                g_StringLanType(szLan, "设备不支持该操作", "Device do not support");
                AfxMessageBox(szLan);
                pThis->m_dwFileNum = 0;
                break;
            }
            else
            {
                pThis->GetDlgItem(IDC_BTN_SEARCH_REC_FILE)->SetWindowText("查找");
                pThis->m_bSearching = FALSE;
                pThis->GetDlgItem(IDC_STATIC_FINDING_REC_FILE)->ShowWindow(SW_HIDE);
                g_pMainDlg->AddLog(pThis->m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_FindNextFile");
                g_StringLanType(szLan, "由于服务器忙,或网络故障,获取文件列表异常终止", "Since the server is busy, or network failure, abnormal termination of access to the file list");
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

// CDlgRecordFile 消息处理程序

void CDlgRecordFile::OnBnClickedBtnSearchRecFile()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    memset(m_szFileName, 0, sizeof(m_szFileName));
    m_nFileNum = 0;

    char szLan[128] = {0};
    if (m_iDeviceIndex < 0)
    {
        g_StringLanType(szLan,"请先登录设备!", "Please Login First!");
        AfxMessageBox(szLan);
        return;
    }

    if (!m_bSearching)
    {
        m_bQuit = FALSE;

        /*
        memset(&m_struFindCond, 0, sizeof(m_struFindCond));
        m_struFindCond.dwSize = sizeof(m_struFindCond);
        m_struFindCond.enumSearchType = ENUM_SEARCH_RECORD_FILE;
        m_struFindCond.dwMaxFileCountPer = 100;
        m_struFindCond.iChannel = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChannelNO;
        m_struFindCond.unionSearchParam.struRecordFileParam.dwFileType = m_cmbRecFileType.GetItemData(m_cmbRecFileType.GetCurSel());
        
        m_struFindCond.struStartTime.wYear = (WORD)m_dateStart.GetYear();
        m_struFindCond.struStartTime.byMonth = (BYTE)m_dateStart.GetMonth();
        m_struFindCond.struStartTime.byDay = (BYTE)m_dateStart.GetDay();
        m_struFindCond.struStartTime.byHour = (BYTE)m_timeStart.GetHour();
        m_struFindCond.struStartTime.byMinute = (BYTE)m_timeStart.GetMinute();
        m_struFindCond.struStartTime.bySecond = (BYTE)m_timeStart.GetSecond();

        m_struFindCond.struStopTime.wYear = (WORD)m_DateStop.GetYear();
        m_struFindCond.struStopTime.byMonth = (BYTE)m_DateStop.GetMonth();
        m_struFindCond.struStopTime.byDay = (BYTE)m_DateStop.GetDay();
        m_struFindCond.struStopTime.byHour = (BYTE)m_timeStop.GetHour();
        m_struFindCond.struStopTime.byMinute = (BYTE)m_timeStop.GetMinute();
        m_struFindCond.struStopTime.bySecond = (BYTE)m_timeStop.GetSecond();
        */
        NET_EHOME_REC_FILE_COND struRecFileCond = { 0 };
        struRecFileCond.dwChannel = (DWORD)m_iChanIndex+1;
        struRecFileCond.byLocalOrUTC = (BYTE)m_bTypeTime;
        struRecFileCond.dwRecType = m_cmbRecFileType.GetItemData(m_cmbRecFileType.GetCurSel());
        struRecFileCond.dwMaxFileCountPer = 8;

        struRecFileCond.struStartTime.wYear = (WORD)m_dateStart.GetYear();
        struRecFileCond.struStartTime.byMonth = (BYTE)m_dateStart.GetMonth();
        struRecFileCond.struStartTime.byDay = (BYTE)m_dateStart.GetDay();
        struRecFileCond.struStartTime.byHour = (BYTE)m_timeStart.GetHour();
        struRecFileCond.struStartTime.byMinute = (BYTE)m_timeStart.GetMinute();
        struRecFileCond.struStartTime.bySecond = (BYTE)m_timeStart.GetSecond();

        struRecFileCond.struStopTime.wYear = (WORD)m_DateStop.GetYear();
        struRecFileCond.struStopTime.byMonth = (BYTE)m_DateStop.GetMonth();
        struRecFileCond.struStopTime.byDay = (BYTE)m_DateStop.GetDay();
        struRecFileCond.struStopTime.byHour = (BYTE)m_timeStop.GetHour();
        struRecFileCond.struStopTime.byMinute = (BYTE)m_timeStop.GetMinute();
        struRecFileCond.struStopTime.bySecond = (BYTE)m_timeStop.GetSecond();



        // m_lFileHandle = NET_ECMS_StartFindFile(m_lLoginID, &m_struFindCond);
        m_lFileHandle = NET_ECMS_StartFindFile_V11(m_lLoginID, ENUM_SEARCH_RECORD_FILE, &struRecFileCond, sizeof(struRecFileCond));

        //m_lFileHandle = NET_ECMS_StartFindFile(m_lLoginID, &m_struFindCond);
        if (m_lFileHandle < 0)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartFindFile");
            g_StringLanType(szLan, "查找文件列表失败!", "Fail to get file list");
            AfxMessageBox(szLan);
            return;
        }
        m_lstRecFile.DeleteAllItems();
        DWORD dwThreadId;
        if (m_hFileThread == NULL)
        {
            m_hFileThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetFileThread),this,0,&dwThreadId);        
            if (m_hFileThread == NULL)
            {
                g_StringLanType(szLan, "打开查找线程失败!", "Fail to open finding thread!");
                AfxMessageBox(szLan);
                return;
            }
        }

        g_StringLanType(szLan, "停止查找", "Stop Searching");
        GetDlgItem(IDC_BTN_SEARCH_REC_FILE)->SetWindowText(szLan);
        m_bSearching = TRUE;
        GetDlgItem(IDC_STATIC_FINDING_REC_FILE)->ShowWindow(SW_SHOW);
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
        GetDlgItem(IDC_BTN_SEARCH_REC_FILE)->SetWindowText(szLan);
        m_bSearching = FALSE;
        GetDlgItem(IDC_STATIC_FINDING_REC_FILE)->ShowWindow(SW_HIDE);
        m_dwFileNum = 0;
    }

}

BOOL CDlgRecordFile::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    char szLan[128] = {0};

    g_StringLanType(szLan, "文件名称", "File Name");
    m_lstRecFile.InsertColumn(0, szLan,LVCFMT_LEFT,150,-1);
    g_StringLanType(szLan, "大小", "Size");
    m_lstRecFile.InsertColumn(1, szLan,LVCFMT_LEFT,50,-1);
    g_StringLanType(szLan, "开始时间", "Start time");
    m_lstRecFile.InsertColumn(2, szLan, LVCFMT_LEFT, 120, -1);

    g_StringLanType(szLan, "结束时间", "Stop Time");
    m_lstRecFile.InsertColumn(3, szLan, LVCFMT_LEFT, 120, -1);

    g_StringLanType(szLan,"主类型","Main Type");
    m_lstRecFile.InsertColumn(4, szLan,LVCFMT_LEFT,50,-1);

    g_StringLanType(szLan,"次类型","Sub Type");
    m_lstRecFile.InsertColumn(5, szLan,LVCFMT_LEFT,50,-1);

    g_StringLanType(szLan,"索引","Index");
    m_lstRecFile.InsertColumn(6, szLan,LVCFMT_LEFT,50,-1);

    g_StringLanType(szLan, "时差时", "Time difference H");
    m_lstRecFile.InsertColumn(7, szLan, LVCFMT_LEFT, 50, -1);

    g_StringLanType(szLan, "时差钟", "Time difference M");
    m_lstRecFile.InsertColumn(8, szLan, LVCFMT_LEFT, 50, -1);

    m_cmbSeekType.SetCurSel(0);

    CTime timeCur = CTime::GetCurrentTime();
    CTime timeStart(timeCur.GetYear(),timeCur.GetMonth(),timeCur.GetDay(),0,0,0);
    CTime timeStop(timeCur.GetYear(),timeCur.GetMonth(),timeCur.GetDay(),23,59,59);
    m_dateStart = timeStart;
    m_timeStart = timeStart;
    m_DateStop = timeStop;
    m_timeStop = timeStop;

    COleDateTime timeTemp = COleDateTime::GetCurrentTime();
    COleDateTime timeStartTemp(timeTemp.GetYear(), timeTemp.GetMonth(), timeTemp.GetDay(), 0, 0, 0);
    m_PlayDateStart = timeStartTemp;
    m_PlayTimeStart = timeStartTemp;

    m_bQuit = FALSE;

    int index = 0;
    m_cmbRecFileType.ResetContent();
    g_StringLanType(szLan, "全部", "All");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_ALL);
    index++;

    g_StringLanType(szLan, "定时录像", "Schedule");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_TIMING);
    index++;

    g_StringLanType(szLan, "移动侦测", "Motion Detect");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_MOTION_DETECT);
    index++;

    g_StringLanType(szLan, "报警触发", "Alarm");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_ALARMIN);
    index++;

    g_StringLanType(szLan, "报警|动测", "Alarm|Motion");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_MOTION_OR_ALARMIN);
    index++;

    g_StringLanType(szLan, "报警&动测", "Alarm&Motion");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_MOTION_AND_ALARMIN);
    index++;

    g_StringLanType(szLan, "命令触发", "Command Record");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_COMMAND);
    index++;

    g_StringLanType(szLan, "手动录像", "Manual Record");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_MANUAL);
    index++;

    g_StringLanType(szLan, "震动报警", "Shake Alarm");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_SHAKE_ALARM);
    index++;

    g_StringLanType(szLan, "环境报警", "Environment Alarm");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_ENVIRONMENT_ALARM);
    index++;

    g_StringLanType(szLan, "智能报警", "VCA Alarm");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_VCA_ALARM);
    index++;

    g_StringLanType(szLan, "PIR报警", "PIR Alarm");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_PIR_ALARM);
    index++;

    g_StringLanType(szLan, "无线报警", "WIRELESS Alarm");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_WIRELESS_ALARM);
    index++;

    g_StringLanType(szLan, "呼救报警", "CALLHELP Alarm");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_CALLHELP_ALARM);
    index++;

    g_StringLanType(szLan, "全部报警录像", "Alarm Record");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, FILE_ALL_ALARM);
    index++;

    g_StringLanType(szLan, "车牌识别图片", "License Recognition");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, PIC_LICENSE_RECOGNITION);
    index++;


    g_StringLanType(szLan, "稽查报警图片", "Inspection Alarm");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, PIC_INSPECT_ALARM);
    index++;


    g_StringLanType(szLan, "手动抓拍图片", "Manual Snap");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, PIC_MANUAL_SNAP);
    index++;

    g_StringLanType(szLan, "回放抓拍图片", "PlayBack Snap");
    m_cmbRecFileType.InsertString(index, szLan);
    m_cmbRecFileType.SetItemData(index, PIC_PLAYBACK_SNAP);
    index++;

    m_cmbRecFileType.SetCurSel(0);
    m_struPlayParam.lPort = -1;
    m_struPlayParam.hPlayWnd = GetDlgItem(IDC_STATIC_PLAY_WND)->m_hWnd;
    m_protocolType.SetCurSel(0);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgRecordFile::OnNMDblclkListRecFile(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    UN_REFERENCED_PARAMETER(pNMHDR)
    UpdateData(TRUE);

    CString csFileName;
    int iFileSelPos = 0;
    POSITION  posItem = m_lstRecFile.GetFirstSelectedItemPosition();
    char szLan[128] = {0};
    if (posItem == NULL)
    {
        g_StringLanType(szLan, "请选择要播放的文件!", "Please select the file to play");
        AfxMessageBox(szLan); 
        return;
    }

    iFileSelPos = m_lstRecFile.GetNextSelectedItem(posItem);
    csFileName.Format("%s",m_lstRecFile.GetItemText(iFileSelPos,0));
    if (csFileName.IsEmpty())
    {
        return;
    }

    if (-1 == m_lPlayBackListenHandle)
    {
        NET_EHOME_PLAYBACK_LISTEN_PARAM struListenParam = { 0 };
        if (1 == m_protocolType.GetCurSel())
        {
            struListenParam.byLinkMode = 8;
        }
        else
        {
            struListenParam.byLinkMode = 0;
        }
        struListenParam.byLinkEncrypt = m_byEncrypt;
        struListenParam.fnNewLinkCB = PlayBackNewLinkCB;
        struListenParam.pUserData = this;
        //g_pMainDlg->GetLocalIP(struListenParam.struIPAdress.szIP);
        strncpy(struListenParam.struIPAdress.szIP, g_pMainDlg->m_sLocalIP, 128);
        struListenParam.struIPAdress.wPort = 8888;
        m_lPlayBackListenHandle = NET_ESTREAM_StartListenPlayBack(&struListenParam);
        if (-1 == m_lPlayBackListenHandle)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 2, "NET_ESTREAM_StartListenPlayBack");
            g_StringLanType(szLan, "NET_ESTREAM_StartListenPlayBack 失败!", "NET_ESTREAM_StartListenPlayBack failed");
        }
    }

    //先开启回放监听
    if (-1 == m_struPlayParam.lPlayBackHandle)
    {
        NET_EHOME_PLAYBACK_INFO_IN struPlayBackInfoIn = {0};
        struPlayBackInfoIn.dwSize = sizeof(struPlayBackInfoIn);
        NET_EHOME_PLAYBACK_INFO_OUT struPlayBackInfoOut = {0};
        struPlayBackInfoIn.byPlayBackMode = (BYTE)m_cmbSeekType.GetCurSel();
        struPlayBackInfoIn.dwChannel = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChannelNO;
        //g_pMainDlg->GetLocalIP(struPlayBackInfoIn.struStreamSever.szIP);
        DWORD dwPortTemp = 0;
        g_pMainDlg->GetAddressByType(1, 0, struPlayBackInfoIn.struStreamSever.szIP, sizeof(struPlayBackInfoIn.struStreamSever.szIP), (char *)&dwPortTemp, sizeof(dwPortTemp));
        // strncpy(struListenParam.struIPAdress.szIP, g_pMainDlg->m_sLocalIP, 128);
        struPlayBackInfoIn.struStreamSever.wPort = (WORD)(dwPortTemp == 0 ? 8888 : dwPortTemp);
        //strncpy(struPlayBackInfoIn.struStreamSever.szIP, g_pMainDlg->m_sLocalIP, 128);
        //struPlayBackInfoIn.struStreamSever.wPort = 8888;
        if (struPlayBackInfoIn.byPlayBackMode == 0)
        {

            strncpy(struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName, csFileName, 100);

            //需要将字符串字段转换成UTF-8
            DWORD dwConvertLen = 0;
            A2UTF8((char*)struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName,
                (char*)struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName,
                MAX_FILE_NAME_LEN, &dwConvertLen);
        }
        else
        {
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.wYear = (WORD)m_PlayDateStart.GetYear();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.byMonth = (BYTE)m_PlayDateStart.GetMonth();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.byDay = (BYTE)m_PlayDateStart.GetDay();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.byHour = (BYTE)m_PlayTimeStart.GetHour();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.byMinute = (BYTE)m_PlayTimeStart.GetMinute();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.bySecond = (BYTE)m_PlayTimeStart.GetSecond();

            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.wYear = (WORD)m_PlayDateStop.GetYear();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.byMonth = (BYTE)m_PlayDateStop.GetMonth();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.byDay = (BYTE)m_PlayDateStop.GetDay();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.byHour = (BYTE)m_PlayTimeStop.GetHour();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.byMinute = (BYTE)m_PlayTimeStop.GetMinute();
            struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.bySecond = (BYTE)m_PlayTimeStop.GetSecond();
        }
        if (1 == m_protocolType.GetCurSel())
        {
            struPlayBackInfoIn.byLinkMode = 8;
        }
        else
        {
            struPlayBackInfoIn.byLinkMode = 0;
        }
        if (!NET_ECMS_StartPlayBack(m_lLoginID, &struPlayBackInfoIn, &struPlayBackInfoOut))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartPlayBack");
            g_StringLanType(szLan, "NET_ECMS_StartPlayBack 失败!", "NET_ECMS_StartPlayBack failed");
            AfxMessageBox(szLan); 
            return;
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartPlayBack");

            m_struPlayParam.lPlayBackHandle = struPlayBackInfoOut.lHandle;
        }

        if (!g_pMainDlg->m_bAsyncCalllback)
        {
            //同步
            NET_EHOME_PUSHPLAYBACK_IN struPushBackIn = { 0 };
            struPushBackIn.dwSize = sizeof(NET_EHOME_PUSHPLAYBACK_IN);
            struPushBackIn.lSessionID = struPlayBackInfoOut.lSessionID;
            m_iSessionID = struPlayBackInfoOut.lSessionID;
            if (1 == m_protocolType.GetCurSel())
            {
                struPlayBackInfoIn.byLinkMode = 8;
            }
            else
            {
                struPlayBackInfoIn.byLinkMode = 0;
            }
            NET_EHOME_PUSHPLAYBACK_OUT struPushBackOut = { 0 };
            struPushBackOut.dwSize = sizeof(struPushBackOut);
            if (!NET_ECMS_StartPushPlayBack(m_lLoginID, &struPushBackIn, &struPushBackOut))
            {
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartPushPlayBack");
            }
            else
            {
                m_struPlayParam.lPlayBackHandle = struPlayBackInfoOut.lHandle;
                m_struPlayParam.lSessionID = struPlayBackInfoOut.lSessionID;
                g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartPushPlayBack");
            }
        }
    }
    else
    {
        //停止该路连接        
        if (m_struPlayParam.lPlayBackHandle != -1)        
        {            
            NET_ESTREAM_StopPlayBack(m_struPlayParam.lPlayBackHandle);            
            m_struPlayParam.lPlayBackHandle = -1;        
        }

        NET_EHOME_STOPPLAYBACK_PARAM struStopParam = { 0 };
        struStopParam.lSessionID = m_struPlayParam.lSessionID;

        //if (!NET_ECMS_StopPlayBack(m_lLoginID, m_struPlayParam.lSessionID))
        if (!NET_ECMS_StopPlayBackEx(g_struDeviceInfo[m_iDeviceIndex].lLoginID, &struStopParam))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StopPlayBackEx");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 2, "NET_ECMS_StopPlayBackEx");
            if (g_pMainDlg->m_bAsyncCalllback)
            {
                LONG lSession = -1;
                while (!g_pMainDlg->PopSessionByHandle(struStopParam.lHandle, &lSession))
                {
                    Sleep(100);
                }
            }
            m_iSessionID = -1;
        }

        if (m_struPlayParam.lPort >= 0)
        {
            PlayM4_FreePort(m_struPlayParam.lPort);
            m_struPlayParam.lPort = -1;
        }

        GetDlgItem(IDC_BUTTON_START_PLAY)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_STOP_PLAY)->EnableWindow(TRUE);

        UpdateData(FALSE);
        Invalidate(TRUE);
    }
    *pResult = 0;
}


void CDlgRecordFile::OnBnClickedButtonCycle()
{
    // TODO: 在此添加控件通知处理程序代码
    CDlgCyclePlayBack dlg;
    dlg.m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();

    
    if (m_nFileNum <= 0)
    {
        MessageBox("没有录像文件，请先查找文件!");
        return;
    }
    int index = 0;
    for (int i = 0; i < MAX_CYCLE_NUM; i++)
    {
        if (i >= m_nFileNum)
        {
            index = 0;
        }
        dlg.AddPlayBackFile(i, m_szFileName[index]);
        index++;
    }
   
    dlg.DoModal();

}

void CDlgRecordFile::OnBnClickedButtonStartPlay()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    char szLan[128] = {0};
    if (m_lLoginID == -1 || m_iDeviceIndex == -1)
    {
        g_StringLanType(szLan, "m_lLoginID m_iDeviceIndex is -1!", "m_lLoginID m_iDeviceIndex is -1");
        AfxMessageBox(szLan); 
        return;
    }

    if (-1 == m_lPlayBackListenHandle)
    {
        NET_EHOME_PLAYBACK_LISTEN_PARAM struListenParam = { 0 };
        if (1 == m_protocolType.GetCurSel())
        {
            struListenParam.byLinkMode = 8;
        }
        else
        {
            struListenParam.byLinkMode = 0;
        }
        struListenParam.fnNewLinkCB = PlayBackNewLinkCB;
        struListenParam.pUserData = this;
        g_pMainDlg->GetLocalIP(struListenParam.struIPAdress.szIP);
        strncpy(struListenParam.struIPAdress.szIP, g_pMainDlg->m_sLocalIP, 128);
        struListenParam.struIPAdress.wPort = m_dwPlaybackPort;
        struListenParam.byLinkEncrypt = m_byEncrypt;
        m_lPlayBackListenHandle = NET_ESTREAM_StartListenPlayBack(&struListenParam);
        if (-1 == m_lPlayBackListenHandle)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 2, "NET_ESTREAM_StartListenPlayBack");
            g_StringLanType(szLan, "NET_ESTREAM_StartListenPlayBack 失败!", "NET_ESTREAM_StartListenPlayBack failed");
        }
    }

    NET_EHOME_PLAYBACK_INFO_IN struPlayBackInfoIn = {0};
    NET_EHOME_PLAYBACK_INFO_OUT struPlayBackInfoOut = {0};
    struPlayBackInfoIn.dwSize = sizeof(NET_EHOME_PLAYBACK_INFO_IN);

    struPlayBackInfoIn.byPlayBackMode = (BYTE)m_cmbSeekType.GetCurSel();
    struPlayBackInfoIn.dwChannel = (DWORD)m_iChanIndex + 1;
    //strncpy(struPlayBackInfoIn.struStreamSever.szIP, g_pMainDlg->m_sLocalIP, 128);
    //g_pMainDlg->GetLocalIP(struPlayBackInfoIn.struStreamSever.szIP);
    //struPlayBackInfoIn.struStreamSever.wPort = 8888;
    DWORD dwPortTemp = 0;
    g_pMainDlg->GetAddressByType(5, 0, struPlayBackInfoIn.struStreamSever.szIP, sizeof(struPlayBackInfoIn.struStreamSever.szIP), (char *)&dwPortTemp, sizeof(dwPortTemp));
    // strncpy(struListenParam.struIPAdress.szIP, g_pMainDlg->m_sLocalIP, 128);
    struPlayBackInfoIn.struStreamSever.wPort = (WORD)(dwPortTemp == 0 ? m_dwPlaybackPort : dwPortTemp);
    if (struPlayBackInfoIn.byPlayBackMode == 0)
    {
        //按文件名（分为按文件名+字节数偏移，按文件时间+时间偏移）
        strncpy(struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName, m_struPlayParam.strFileName, 100);
        //strncpy(struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName, "Playback.mp4", strlen("Playback.mp4"));

        //需要将字符串字段转换成UTF-8
        DWORD dwConvertLen = 0;
        A2UTF8((char*)struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName,
            (char*)struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName,
            MAX_FILE_NAME_LEN, &dwConvertLen);
//         struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.dwSeekType = 1;
//         struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.dwFileOffset = 100;
//         struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.dwFileSpan = 3000;
    }
    else
    {
        //按时间
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.wYear = (WORD)m_PlayDateStart.GetYear();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.byMonth = (BYTE)m_PlayDateStart.GetMonth();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.byDay = (BYTE)m_PlayDateStart.GetDay();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.byHour = (BYTE)m_PlayTimeStart.GetHour();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.byMinute = (BYTE)m_PlayTimeStart.GetMinute();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStartTime.bySecond = (BYTE)m_PlayTimeStart.GetSecond();

        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.wYear = (WORD)m_PlayDateStop.GetYear();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.byMonth = (BYTE)m_PlayDateStop.GetMonth();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.byDay = (BYTE)m_PlayDateStop.GetDay();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.byHour = (BYTE)m_PlayTimeStop.GetHour();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.byMinute = (BYTE)m_PlayTimeStop.GetMinute();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.struStopTime.bySecond = (BYTE)m_PlayTimeStop.GetSecond();
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.byLocalOrUTC = (BYTE)m_byTimeType;
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.byDuplicateSegment = (BYTE)m_byRecordType;
        //增加码流类型选择
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.byStreamType = (BYTE)m_byStreamType;
        struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyTime.byHls = m_bHls;
    }
    if (1 == m_protocolType.GetCurSel())
    {
        struPlayBackInfoIn.byLinkMode = 8;
    }
    else
    {
        struPlayBackInfoIn.byLinkMode = 0;
    }
    struPlayBackInfoIn.byLinkEncrypt = m_byEncrypt;
    if (!NET_ECMS_StartPlayBack(m_lLoginID, &struPlayBackInfoIn, &struPlayBackInfoOut))
    {
        g_StringLanType(szLan, "NET_ECMS_StartPlayBack 失败!", "NET_ECMS_StartPlayBack failed");
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, szLan);
        AfxMessageBox(szLan); 
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartPlayBack1");
//         if (g_pMainDlg->m_bAsyncCalllback)
//         {
//             LONG lSession = -1;
//             while (!g_pMainDlg->PopSessionByHandle(struPlayBackInfoOut.lHandle, &lSession))
//             {
//                 Sleep(100);
//             }
//             m_iSessionID = lSession;
//         }
//         else
//         {
//             m_iSessionID = struPlayBackInfoOut.lSessionID;
//         }

    }

    if (!g_pMainDlg->m_bAsyncCalllback)
    {
        NET_EHOME_PUSHPLAYBACK_IN struPushBackIn = { 0 };
        struPushBackIn.dwSize = sizeof(NET_EHOME_PUSHPLAYBACK_IN);
        struPushBackIn.lSessionID = struPlayBackInfoOut.lSessionID;
        m_iSessionID = struPlayBackInfoOut.lSessionID;

        NET_EHOME_PUSHPLAYBACK_OUT struPushBackOut = { 0 };
        struPushBackOut.dwSize = sizeof(struPushBackOut);
        if (!NET_ECMS_StartPushPlayBack(m_lLoginID, &struPushBackIn, &struPushBackOut))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartPushPlayBack");
        }
        else
        {
            m_struPlayParam.lSessionID = struPlayBackInfoOut.lSessionID;
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartPushPlayBack");
//             if (g_pMainDlg->m_bAsyncCalllback)
//             {
//                 LONG lSession = -1;
//                 while (!g_pMainDlg->PopSessionByHandle(struPushBackOut.lHandle, &lSession))
//                 {
//                     Sleep(100);
//                 }
//             }
        }
    }

    
}

void CDlgRecordFile::OnBnClickedButtonStopPlay()
{
    // TODO: 在此添加控件通知处理程序代码
    char szLan[128] = {0};

    if (m_bPause == TRUE)
    {
        //恢复回放，调用此接口是为了发送一个ECMS_CB_TYPE_RESTART_PLAYBACK消息，避免暂停时候停止，再开启就无法回放了
        NET_EHOME_PLAYBACK_PAUSE_RESTART_PARAM struPlaybackReStartParam = { 0 };
        struPlaybackReStartParam.lSessionID = m_iSessionID;
        NET_ECMS_PlayBackOperate(m_lLoginID, PLAYBACK_OPERATE_RESTART, &struPlaybackReStartParam);
        m_bPause = FALSE;
        g_StringLanType(szLan, "暂停", "Pause");

        CString  csTemp = szLan;
        GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText(csTemp);
    }
    if (m_iSessionID >=0)
    {

        NET_EHOME_STOPPLAYBACK_PARAM struStopParam = { 0 };
        struStopParam.lSessionID = m_iSessionID;

        if (!NET_ECMS_StopPlayBackEx(m_lLoginID, &struStopParam))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StopPlayBackEx[%d]", m_iSessionID);
            g_StringLanType(szLan, "NET_ECMS_StopPlayBackEx 失败!", "NET_ECMS_StopPlayBack failed");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StopPlayBackEx[%d]", m_iSessionID);
            g_StringLanType(szLan, "NET_ECMS_StopPlayBackEx 成功!", "NET_ECMS_StopPlayBackEx Succ");
            if (g_pMainDlg->m_bAsyncCalllback)
            {
                LONG lSession = -1;
                while (!g_pMainDlg->PopSessionByHandle(struStopParam.lHandle, &lSession))
                {
                    Sleep(100);
                }
            }
            m_iSessionID = -1;
        }
    }

    //停止该路连接        
    if (m_struPlayParam.lPlayBackHandle != -1)        
    {            
        NET_ESTREAM_StopPlayBack(m_struPlayParam.lPlayBackHandle);            
        m_struPlayParam.lPlayBackHandle = -1;      
    }    
    GetDlgItem(IDC_BUTTON_START_PLAY)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_STOP_PLAY)->EnableWindow(TRUE);

    if (m_struPlayParam.lPort >= 0)
    {
        PlayM4_FreePort(m_struPlayParam.lPort);
        m_struPlayParam.lPort = -1;
    }
    //停止监听
    if (m_lPlayBackListenHandle != -1)
    {
        NET_ESTREAM_StopListenPlayBack(m_lPlayBackListenHandle);
        m_lPlayBackListenHandle = -1;
    }

    UpdateData(FALSE);
    Invalidate(TRUE);
    if (m_bStart == TRUE)
    {
        m_bSave = FALSE;
        fclose(m_pVideoFile);
        m_pVideoFile = NULL;
        char szLan[128] = { 0 };
        CString csTemp = _T("");
        g_StringLanType(szLan, "保存", "Save");
        csTemp = szLan;
        GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(csTemp);
        m_bStart = FALSE;
    }

}

void CDlgRecordFile::OnStnClickedStaticPlayWnd()
{
    // TODO: 在此添加控件通知处理程序代码
}

void CDlgRecordFile::OnNMClickListRecFile(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    int iItem = pNMItemActivate->iItem;
    int iSubItem = pNMItemActivate->iSubItem;

    if (iItem == -1)
    {
        return;
    }

    CString strFileName = m_lstRecFile.GetItemText(iItem, iSubItem);
    sprintf(m_struPlayParam.strFileName, "%s", strFileName);

    *pResult = 0;
}


void CDlgRecordFile::OnBnClickedButtonSave()
{  
    // TODO:  在此添加控件通知处理程序代码

    // TODO:  Add extra initialization here
    char szLan[128] = { 0 };

    char szFile[256] = { 0 };
    memcpy(szFile, "C:\\EhomeRecord", strlen("C:\\EhomeRecord"));
    if (!PathFileExists(szFile))
    {
        if (!CreateDirectory(szFile, NULL))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "Create file failed!");
            return;
        }
    }

    CString csTemp = _T("");
    //if (m_struPlayParam.lPlayBackHandle >= 0)
    {
        if (m_bStart == FALSE)
        {
            m_bSave = TRUE;
            g_StringLanType(szLan, "停止保存", "Stop Save");
            csTemp = szLan;
            GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(csTemp);
            m_bStart = TRUE;
            char szLan[256] = { 0 };

            CTime  cTime = CTime::GetCurrentTime();
            memset(m_szTime, 0, sizeof(m_szTime));
            sprintf(m_szTime, "%s", cTime.Format("%y-%m-%d_%H_%M_%S").GetBuffer(0));

            sprintf(szLan, "Save File To  C:\\EhomeRecord\\[PlayBack]%s_%s_%d.mp4", g_struDeviceInfo[m_iDeviceIndex].byDeviceID, \
                m_szTime, \
                m_iChanIndex + 1);

            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 2, szLan);
        }
        else
        {
            m_bSave = FALSE;
            if (m_pVideoFile != NULL)
            {
                fclose(m_pVideoFile);
                m_pVideoFile = NULL;
            }

            g_StringLanType(szLan, "保存", "Save");
            csTemp = szLan;
            GetDlgItem(IDC_BUTTON_SAVE)->SetWindowText(csTemp);
            m_bStart = FALSE;
        }
    }
    
}



void CDlgRecordFile::OnBnClickedButtonPause()
{
    // TODO:  在此添加控件通知处理程序代码
    char szLan[128] = { 0 };
    if (m_bPause == FALSE)
    {
        //暂停回放
        NET_EHOME_PLAYBACK_PAUSE_RESTART_PARAM struPlaybackPauseParam = { 0 };
        struPlaybackPauseParam.lSessionID = m_iSessionID;

        if (5 == m_dwVersion)
        {
            if (!NET_ECMS_PlayBackOperate(m_lLoginID, PLAYBACK_OPERATE_PAUSE, &struPlaybackPauseParam))
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_ECMS_PlayBackOperate Failed");
                return;
            }
            else
            {
                if (m_struPlayParam.lPort >= 0)
                {
                    Sleep(1000); //延时一秒，等待socket缓冲区数据接收完成
                    m_bPause = TRUE;
                    Sleep(200);//延时200毫秒,等待播放库数据接收完成
                    PlayM4_Pause(m_struPlayParam.lPort, m_bPause);
                }
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_ECMS_PlayBackOperate SUCC");
            }
            m_bPause = TRUE;
            g_StringLanType(szLan, "恢复", "Resume");
            CString  csTemp = szLan;
            GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText(csTemp);
        }
        else
        {
            m_bPause = TRUE;
            Sleep(200);//延时200毫秒,等待播放库数据接收完成
            PlayM4_Pause(m_struPlayParam.lPort, m_bPause);
        }
        g_StringLanType(szLan, "恢复", "Resume");
        CString  csTemp = szLan;
        GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText(csTemp);
    }
    else
    {
        m_bPause = FALSE;
        g_StringLanType(szLan, "暂停", "Pause");

        CString  csTemp = szLan;
        GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText(csTemp);

        if (m_struPlayParam.lPort >= 0)
        {
            PlayM4_Pause(m_struPlayParam.lPort, m_bPause);
        }

        //恢复回放
        NET_EHOME_PLAYBACK_PAUSE_RESTART_PARAM struPlaybackReStartParam = { 0 };
        struPlaybackReStartParam.lSessionID = m_iSessionID;

        if (5 == m_dwVersion)
        {
            if (!NET_ECMS_PlayBackOperate(m_lLoginID, PLAYBACK_OPERATE_RESTART, &struPlaybackReStartParam))
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_ECMS_PlayBackOperate Failed");
                return;
            }
            else
            {
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_ECMS_PlayBackOperate SUCC");
            }
        }
    }
    return;
}

void CDlgRecordFile::OnBnClickedPlaybackSound()
{
    // TODO:  在此添加控件通知处理程序代码
    char szLan[128] = { 0 };
    if (g_bSoundSwitchTag)
    {
        g_bSoundSwitchTag = FALSE;
        g_StringLanType(szLan, "打开声音", "Open Audio");
    }
    else
    {
        g_bSoundSwitchTag = TRUE;
        g_StringLanType(szLan, "关闭声音", "Close Audio");
    }
    GetDlgItem(IDC_PLAYBACK_SOUND)->SetWindowText(szLan);
}


void CDlgRecordFile::OnBnClickedButtonFastPlay()
{
    // TODO:  在此添加控件通知处理程序代码
    char szLan[128] = { 0 };
    //如果处于暂停状态，则先进行恢复播放
    if (m_bPause)
    {
        m_bPause = FALSE;

        if (m_struPlayParam.lPort >= 0)
        {
            PlayM4_Pause(m_struPlayParam.lPort, m_bPause);
        }

        //恢复回放
        NET_EHOME_PLAYBACK_PAUSE_RESTART_PARAM struPlaybackReStartParam = { 0 };
        struPlaybackReStartParam.lSessionID = m_iSessionID;

        if (5 == m_dwVersion)
        {
            if (!NET_ECMS_PlayBackOperate(m_lLoginID, PLAYBACK_OPERATE_RESTART, &struPlaybackReStartParam))
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_ECMS_PlayBackOperate Failed");
                return;
            }
            else
            {
                g_StringLanType(szLan, "暂停", "Pause");
                CString  csTemp = szLan;
                GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText(csTemp);
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_ECMS_PlayBackOperate SUCC");
            }
        }
    }
    if (m_struPlayParam.lPort >= 0)
    {
        //NPQ回放走UDP链接，快放和慢放是通过和设备进行命令交互实现的
        if (1 == m_protocolType.GetCurSel())
        {
            NET_EHOME_PLAYBACK_PAUSE_RESTART_PARAM struPlaybackReStartParam = { 0 };
            struPlaybackReStartParam.lSessionID = m_iSessionID;
            if (!NET_ECMS_PlayBackOperate(m_lLoginID, PLAYBACK_OPERATE_FAST, &struPlaybackReStartParam))
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_ECMS_PlayBackOperate Failed");
                return;
            }
        }
        if (PlayM4_Fast(m_struPlayParam.lPort))
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "PlayM4_Fast SUCC");
        }
        else
        {
            g_pMainDlg->AddLog(0, PLAY_FAIL_T, 3, "PlayM4_Fast err[%d]", PlayM4_GetLastError(m_struPlayParam.lPort));
        }
    }
    
}


void CDlgRecordFile::OnBnClickedButtonSlowPlay()
{
    // TODO:  在此添加控件通知处理程序代码
    char szLan[128] = { 0 };
    //如果处于暂停状态，则先进行恢复播放
    if (m_bPause)
    {
        m_bPause = FALSE;
        if (m_struPlayParam.lPort >= 0)
        {
            PlayM4_Pause(m_struPlayParam.lPort, m_bPause);
        }

        //恢复回放
        NET_EHOME_PLAYBACK_PAUSE_RESTART_PARAM struPlaybackReStartParam = { 0 };
        struPlaybackReStartParam.lSessionID = m_iSessionID;

        if (5 == m_dwVersion)
        {
            if (!NET_ECMS_PlayBackOperate(m_lLoginID, PLAYBACK_OPERATE_RESTART, &struPlaybackReStartParam))
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_ECMS_PlayBackOperate Failed");
                return;
            }
            else
            {
                g_StringLanType(szLan, "暂停", "Pause");
                CString  csTemp = szLan;
                GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowText(csTemp);
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_ECMS_PlayBackOperate SUCC");
            }
        }
    }
    if (m_struPlayParam.lPort >= 0)
    {
        //NPQ回放走UDP链接，快放和慢放是通过和设备进行命令交互实现的
        if (1 == m_protocolType.GetCurSel())
        {
            NET_EHOME_PLAYBACK_PAUSE_RESTART_PARAM struPlaybackReStartParam = { 0 };
            struPlaybackReStartParam.lSessionID = m_iSessionID;
            if (!NET_ECMS_PlayBackOperate(m_lLoginID, PLAYBACK_OPERATE_SLOW, &struPlaybackReStartParam))
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_ECMS_PlayBackOperate Failed");
                return;
            }
        }
        if (PlayM4_Slow(m_struPlayParam.lPort))
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "PlayM4_Slow SUCC");
        }
        else
        {
            g_pMainDlg->AddLog(0, PLAY_FAIL_T, 3, "PlayM4_Slow err[%d]", PlayM4_GetLastError(m_struPlayParam.lPort));
        }
    }
}

BOOL  CDlgRecordFile::InputStreamData(BYTE byDataType, BYTE* pBuffer, int iDataLen)
{
    if (m_bSave)
    {
        if (m_pVideoFile == NULL)
        {
            char szFile[256] = { 0 };
            sprintf(szFile, "C:\\EhomeRecord\\[PlayBack]%s_%s_%d.mp4", g_struDeviceInfo[m_iDeviceIndex].byDeviceID, \
                m_szTime, \
                m_iChanIndex + 1);
            m_pVideoFile = fopen(szFile, "wb");
        }

        if (m_pVideoFile != NULL)
        {
            fwrite((char*)pBuffer, iDataLen, 1, m_pVideoFile);
        }
    }

    while (m_bPause)
    {
        Sleep(10);
    }
    ////暂停时堵在这里，不处理数据,2.0，4.0暂停是通过demo堵住回调接口，5.0是设备停止发流
    //       
    //   if (5 != g_pDlgRemoteFile->m_dwVersion)
    //   {
    //       while (g_pDlgRemoteFile->m_bPause)
    //       {
    //           Sleep(10);
    //       }
    //   }
    //   else{
    //       //5.0是设备停止发流，但是因为当设备接收到暂停命令后缓冲区中的数据还没发完，设备会继续发送码流，这里对于暂停后设备发送的码流数据不作处理
    //       if (g_pDlgRemoteFile->m_bPause){
    //           //PlayM4_ResetSourceBuffer(g_pDlgRemoteFile->m_struPlayParam.lPort);//注释清空播放库缓冲区操作，恢复回放数据产生跳秒现象
    //           //return TRUE;
    //       }
    //   }

    BOOL bRet = FALSE;
    int i = 0;
    LONG lIndex = m_struPlayParam.lPort;
    switch (byDataType)
    {
    case NET_EHOME_SYSHEAD:
        if (!PlayM4_GetPort(&lIndex))
        {
            m_struPlayParam.lPort = -1;
            break;
        }
        m_struPlayParam.lPort = lIndex;


        bRet = PlayM4_SetOverlayMode(lIndex, FALSE, COLORREF(0));//not play on overplay
        bRet = PlayM4_SetStreamOpenMode(lIndex, STREAME_FILE);

        if (iDataLen > 0)
        {
            bRet = PlayM4_OpenStream(lIndex, (unsigned char *)pBuffer, iDataLen, 2 * 1024 * 1024);//SOURCE_BUF_MIN50*1000
            if (bRet)
            {
                bRet = PlayM4_Play(lIndex, m_struPlayParam.hPlayWnd);
                if (!bRet)
                {
                    g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lIndex));
                }
                else
                {
                    g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_SUCC_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lIndex));
                }
            }
            else
            {
                g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_OpenStream err[%d]!", PlayM4_GetLastError(lIndex));
            }
        }
        break;
    case NET_EHOME_STREAMDATA:
        if (iDataLen > 0 && lIndex >= 0)
        {
            for (i = 0; i < 1000; i++)
            {
                //回放中播放声音，
                if (g_bSoundSwitchTag)
                {
                    if (!g_bSound)
                    {
                        if (!PlayM4_PlaySound(lIndex))
                        {
                            int err = PlayM4_GetLastError(lIndex);
                        }
                        else
                        {
                            g_bSound = TRUE;
                        }

                    }
                }
                else
                {
                    if (g_bSound)
                    {
                        PlayM4_StopSound();
                        g_bSound = FALSE;
                    }
                }

                bRet = PlayM4_InputData(lIndex, (unsigned char *)pBuffer, iDataLen);
                if (!bRet)
                {
                    if (i >= 999)
                    {
                        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_InputData err[%d]!", PlayM4_GetLastError(lIndex));
                    }

                    Sleep(1);
                }
                else
                {
                    break;
                }
            }
        }
        break;
    case NET_EHOME_HLS:
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 0, "PlayBackDataCB recv HLS datalen[%d]", iDataLen);
        break;
    }
    }

    return TRUE;
}
