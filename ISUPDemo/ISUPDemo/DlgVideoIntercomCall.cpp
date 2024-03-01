// DlgVideoIntercomCall.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgVideoIntercomCall.h"
#include "Public/Convert/Convert.h"
#include "public/cjson/cJson.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define VIS_TALK_TIMER		            WM_USER+2		//timer for one talk
#define TIMEOUT 5000
#define REQUEST_URL "/ISAPI/VideoIntercom/callSignal?format=json"
#define REQUEST_BODY_PATTERN_CLIENT "{" \
"    \"CallSignal\":{" \
"        \"cmdType\":\"%s\"," \
"        \"target\":{" \
"            \"periodNumber\":%d," \
"            \"buildingNumber\":%d," \
"            \"unitNumber\":%d," \
"            \"floorNumber\":%d," \
"            \"roomNumber\":%d," \
"            \"devIndex\":%d," \
"            \"unitType\":\"%s\"," \
"        }" \
"    }" \
"}"

#define REQUEST_BODY_PATTERN_RESPONSE "{" \
"    \"CallSignal\":{" \
"        \"cmdType\":\"%s\"" \
"    }" \
"}"

static const char* unitTypeString[] = { "", "indoor", "villa", "confirm", "outdoor", "fence", "doorbell", "manage" };
/////////////////////////////////////////////////////////////////////////////
// CDlgVideoIntercomCall dialog


CDlgVideoIntercomCall::CDlgVideoIntercomCall(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVideoIntercomCall::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVideoIntercomCall)
		// NOTE: the ClassWizard will add member initialization here
     m_byRequestType = 0; 
     m_bClientCall = TRUE;
     m_iDevIndex = -1;
     m_iChannelIndex = -1;
     m_lListenHandle = -1;
     m_lPreviewHandle = -1;
     m_lPort = -1;

     m_dwPeriod = 0;
     m_dwBuildingNumber = 0;
     m_dwUnitNumber = 0;
     m_dwFloorNumber = 0;
     m_dwRoomNumber = 0;
     m_dwDevIndex = 0;
     m_bWorking = FALSE;
     m_bPlayVideo = FALSE;
     m_bPlayVoice = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgVideoIntercomCall::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PERIOD, m_dwPeriod);
    DDX_Text(pDX, IDC_EDIT_BUILDING_NUMBER, m_dwBuildingNumber);
    DDX_Text(pDX, IDC_EDIT_UNIT_NUMBER, m_dwUnitNumber);
    DDX_Text(pDX, IDC_EDIT_FLOOR_NUMBER, m_dwFloorNumber);
    DDX_Text(pDX, IDC_EDIT_ROOM_NUMBER, m_dwRoomNumber);
    DDX_Text(pDX, IDC_EDIT_DEVICE_INDEX, m_dwDevIndex);
    DDX_Control(pDX, IDC_CMB_UNIT_TYPE, m_cmbUnitType);
    DDX_Control(pDX, IDC_LIST_VTALK_LOG, m_listVTalkLog);
}


BEGIN_MESSAGE_MAP(CDlgVideoIntercomCall, CDialog)
	//{{AFX_MSG_MAP(CDlgVideoIntercomCall)
	ON_BN_CLICKED(IDC_BTN_INQUEST, OnBtnInquest)
	ON_BN_CLICKED(IDC_BTN_ANSWER, OnBtnAnswer)
	ON_BN_CLICKED(IDC_BTN_HANG_UP, OnBtnHangUp)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_REFUSE, OnBtnRefuse)
    ON_MESSAGE(WM_VEDIO_INTERCOME_CMD, &CDlgVideoIntercomCall::OnVedioIntercomeCmd)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_VIDEO_TEST, &CDlgVideoIntercomCall::OnBnClickedBtnVideoTest)
    ON_BN_CLICKED(IDC_BTN_VOICE_TEST, &CDlgVideoIntercomCall::OnBnClickedBtnVoiceTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVideoIntercomCall message handlers

void CDlgVideoIntercomCall::OnBtnInquest() 
{
	// TODO: Add your control notification handler code here

    m_byRequestType = 0;

    if (m_iDevIndex < 0)
    {
        AfxMessageBox("please select a channel!");
        //return;
    }
    m_iDevIndex = 1;
    SendCallCmd();
}

void CDlgVideoIntercomCall::OnBtnAnswer() 
{
	// TODO: Add your control notification handler code here
 
    if (!m_bClientCall)
    {
        KillTimer(VIS_TALK_TIMER);
    }
    //show start play
    SendAcceptCmd();
}

void CDlgVideoIntercomCall::OnBtnHangUp() 
{
	// TODO: Add your control notification handler code here
    char szLan[128] = {0};
    if (!m_bClientCall)
    {
        KillTimer(VIS_TALK_TIMER);
    }
    SendByeCmd();  
    // show close link
    GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
    GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
    g_StringLanType(szLan,"","");
    GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
}

void CDlgVideoIntercomCall::ChangeBtnStates()
{
    return;
}

void CDlgVideoIntercomCall::SendCancellCmd()
{
    UpdateData(TRUE);
    GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(SW_HIDE);

    ASSERT(m_iDevIndex > 0);

    char szInpt[2048] = { 0 };
    _snprintf(szInpt, 2047, REQUEST_BODY_PATTERN_RESPONSE, g_cmdString[VI_CANCEL]);
    //PUT
    CString csRet = IsapiPassthroughRequest(g_struDeviceInfo[m_iDevIndex].lLoginID, 1, REQUEST_URL, szInpt);

    char szLan[256] = { 0 };
    //成功
    if (csRet.GetLength() != 0)
    {
        DWORD dwErrorCode = 0;
        cJSON* pRoot = cJSON_Parse(csRet);
        if (pRoot != NULL)
        {
            cJSON* pPercent = cJSON_GetObjectItem(pRoot, "errorCode");
            if (pPercent != NULL && pPercent->type == cJSON_Number)
            {
                dwErrorCode = pPercent->valueint;
            }

            cJSON_Delete(pRoot);
        }

        if (dwErrorCode == 0)
        {
            g_StringLanType(szLan, "取消通话交互成功", "cancel call success");
            AddTalkLog(VI_CANCEL, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
        }
        else
        {
            g_StringLanType(szLan, "取消通话交互失败,错误码:", "cancel call failed,error code:");
            _snprintf(szLan, 255, "%s%d", szLan, dwErrorCode);
            AddTalkLog(VI_CANCEL, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
        }
        m_bWorking = FALSE;
        return;
    }

    g_StringLanType(szLan, "取消通话通信失败,错误码:", "cancel call communicate failed,error code:");
    DWORD errorCode = NET_ECMS_GetLastError();
    _snprintf(szLan, 255, "%s%d", szLan, errorCode);
    AddTalkLog(VI_CANCEL, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
    m_bWorking = FALSE;
    return;
}

void CDlgVideoIntercomCall::SendCallCmd()
{
    UpdateData(TRUE);
    ASSERT(m_iDevIndex > 0);

    char szInpt[2048] = { 0 };
    _snprintf(szInpt, 2047, REQUEST_BODY_PATTERN_CLIENT, g_cmdString[VI_REQUEST],
        m_dwPeriod, m_dwBuildingNumber, m_dwUnitNumber, m_dwFloorNumber, 
        m_dwRoomNumber, m_dwDevIndex, unitTypeString[m_cmbUnitType.GetCurSel()]);
    //PUT
    CString csRet = IsapiPassthroughRequest(g_struDeviceInfo[m_iDevIndex].lLoginID, 1, REQUEST_URL, szInpt);
    
    char szLan[256] = { 0 };
    //成功
    if (csRet.GetLength() != 0)
    {
        DWORD dwErrorCode = 0;
        cJSON* pRoot = cJSON_Parse(csRet);
        if (pRoot != NULL)
        {
            cJSON* pPercent = cJSON_GetObjectItem(pRoot, "errorCode");
            if (pPercent != NULL && pPercent->type == cJSON_Number)
            {
                dwErrorCode = pPercent->valueint;
            }
            cJSON_Delete(pRoot);
        }


        if (dwErrorCode == 0)
        {
            g_StringLanType(szLan, "请求通话交互成功", "request call success");
            AddTalkLog(VI_REQUEST, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
            m_bWorking = TRUE;

            GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(FALSE);
            GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(TRUE);
            GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
            GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(TRUE);
            GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        }
        else
        {
            g_StringLanType(szLan, "请求通话交互失败,错误码:", "request call failed,error code:");
            _snprintf(szLan, 255, "%s%d", szLan, dwErrorCode);
            AddTalkLog(VI_REQUEST, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
            m_bWorking = FALSE;
        }
        return;
    }

    g_StringLanType(szLan, "请求通话通信失败,错误码:", "request call communicate failed,error code:");
    DWORD errorCode = NET_ECMS_GetLastError();
    _snprintf(szLan, 255, "%s%d", szLan, errorCode);
    AddTalkLog(VI_REQUEST, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
    m_bWorking = FALSE;
    return;
}

void CDlgVideoIntercomCall::SendAcceptCmd()
{
    UpdateData(TRUE);

    ASSERT(m_iDevIndex > 0);

    char szInpt[2048] = { 0 };
    _snprintf(szInpt, 2047, REQUEST_BODY_PATTERN_RESPONSE, g_cmdString[VI_ANSWER]);
    //PUT
    CString csRet = IsapiPassthroughRequest(g_struDeviceInfo[m_iDevIndex].lLoginID, 1, REQUEST_URL, szInpt);

    char szLan[256] = { 0 };
    //成功
    if (csRet.GetLength() != 0)
    {
        DWORD dwErrorCode = 0;
        cJSON* pRoot = cJSON_Parse(csRet);
        if (pRoot != NULL)
        {
            cJSON* pPercent = cJSON_GetObjectItem(pRoot, "errorCode");
            if (pPercent != NULL && pPercent->type == cJSON_Number)
            {
                dwErrorCode = pPercent->valueint;
            }

            cJSON_Delete(pRoot);
        }

        if (dwErrorCode == 0)
        {
            g_StringLanType(szLan, "接听通话交互成功", "answer call success");
            AddTalkLog(VI_ANSWER, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
            m_bWorking = TRUE;

            GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(FALSE);
            GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
            GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
            GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(TRUE);
            GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        }
        else
        {
            g_StringLanType(szLan, "接听通话交互失败,错误码:", "answer call failed,error code:");
            _snprintf(szLan, 255, "%s%d", szLan, dwErrorCode);
            AddTalkLog(VI_ANSWER, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
            m_bWorking = FALSE;
        }

        return;
    }

    g_StringLanType(szLan, "接听通话通信失败,错误码:", "answer call communicate failed,error code:");
    DWORD errorCode = NET_ECMS_GetLastError();
    _snprintf(szLan, 255, "%s%d", szLan, errorCode);
    AddTalkLog(VI_ANSWER, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
    m_bWorking = FALSE;
    return;
}

void CDlgVideoIntercomCall::SendRefuseCmd()
{
    UpdateData(TRUE);

    ASSERT(m_iDevIndex > 0);

    char szInpt[2048] = { 0 };
    _snprintf(szInpt, 2047, REQUEST_BODY_PATTERN_RESPONSE, g_cmdString[VI_REJECT]);
    //PUT
    CString csRet = IsapiPassthroughRequest(g_struDeviceInfo[m_iDevIndex].lLoginID, 1, REQUEST_URL, szInpt);

    char szLan[256] = { 0 };
    //成功
    if (csRet.GetLength() != 0)
    {
        DWORD dwErrorCode = 0;
        cJSON* pRoot = cJSON_Parse(csRet);
        if (pRoot != NULL)
        {
            cJSON* pPercent = cJSON_GetObjectItem(pRoot, "errorCode");
            if (pPercent != NULL && pPercent->type == cJSON_Number)
            {
                dwErrorCode = pPercent->valueint;
            }

            cJSON_Delete(pRoot);
        }

        if (dwErrorCode == 0)
        {
            g_StringLanType(szLan, "拒接通话交互成功", "reject call success");
            AddTalkLog(VI_REJECT, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);

            GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
            GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
            GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
            GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
            GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        }
        else
        {
            g_StringLanType(szLan, "拒接通话交互失败,错误码:", "reject call failed,error code:");
            _snprintf(szLan, 255, "%s%d", szLan, dwErrorCode);
            AddTalkLog(VI_REJECT, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
        }

        m_bWorking = FALSE;
        return;
    }

    g_StringLanType(szLan, "拒接通话通信失败,错误码:", "reject call communicate failed,error code:");
    DWORD errorCode = NET_ECMS_GetLastError();
    _snprintf(szLan, 255, "%s%d", szLan, errorCode);
    AddTalkLog(VI_REJECT, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
    m_bWorking = FALSE;
    return;
}

void CDlgVideoIntercomCall::SendBellTimeoutCmd()
{
    UpdateData(TRUE);
    GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
    GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);

    ASSERT(m_iDevIndex > 0);

    char szInpt[2048] = { 0 };
    _snprintf(szInpt, 2047, REQUEST_BODY_PATTERN_RESPONSE, g_cmdString[VI_BELL_TIMEOUT]);
    //PUT
    CString csRet = IsapiPassthroughRequest(g_struDeviceInfo[m_iDevIndex].lLoginID, 1, REQUEST_URL, szInpt);

    char szLan[256] = { 0 };
    //成功
    if (csRet.GetLength() != 0)
    {
        DWORD dwErrorCode = 0;
        cJSON* pRoot = cJSON_Parse(csRet);
        if (pRoot != NULL)
        {
            cJSON* pPercent = cJSON_GetObjectItem(pRoot, "errorCode");
            if (pPercent != NULL && pPercent->type == cJSON_Number)
            {
                dwErrorCode = pPercent->valueint;
            }
            cJSON_Delete(pRoot);
        }


        if (dwErrorCode == 0)
        {
            g_StringLanType(szLan, "无人接听交互成功", "bell timeout success");
            AddTalkLog(VI_BELL_TIMEOUT, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
        }
        else
        {
            g_StringLanType(szLan, "无人接听交互失败,错误码:", "bell timeout failed,error code:");
            _snprintf(szLan, 255, "%s%d", szLan, dwErrorCode);
            AddTalkLog(VI_BELL_TIMEOUT, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
        }

        m_bWorking = FALSE;
        return;
    }

    g_StringLanType(szLan, "无人接听通信失败,错误码:", "bell timeout communicate failed,error code:");
    DWORD errorCode = NET_ECMS_GetLastError();
    _snprintf(szLan, 255, "%s%d", szLan, errorCode);
    AddTalkLog(VI_BELL_TIMEOUT, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
    m_bWorking = FALSE;
    return;
}

void CDlgVideoIntercomCall::SendByeCmd()
{
    UpdateData(TRUE);

    GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
    GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);

    ASSERT(m_iDevIndex > 0);

    char szInpt[2048] = { 0 };
    _snprintf(szInpt, 2047, REQUEST_BODY_PATTERN_RESPONSE, g_cmdString[VI_HANGUP]);
    //PUT
    CString csRet = IsapiPassthroughRequest(g_struDeviceInfo[m_iDevIndex].lLoginID, 1, REQUEST_URL, szInpt);

    char szLan[256] = { 0 };
    //成功
    if (csRet.GetLength() != 0)
    {
        DWORD dwErrorCode = 0;
        cJSON* pRoot = cJSON_Parse(csRet);
        if (pRoot != NULL)
        {
            cJSON* pPercent = cJSON_GetObjectItem(pRoot, "errorCode");
            if (pPercent != NULL && pPercent->type == cJSON_Number)
            {
                dwErrorCode = pPercent->valueint;
            }
            cJSON_Delete(pRoot);
        }


        if (dwErrorCode == 0)
        {
            g_StringLanType(szLan, "结束通话交互成功", "hangup call success");
            AddTalkLog(VI_HANGUP, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
        }
        else
        {
            g_StringLanType(szLan, "结束通话交互失败,错误码:", "hangup call failed,error code:");
            _snprintf(szLan, 255, "%s%d", szLan, dwErrorCode);
            AddTalkLog(VI_HANGUP, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
        }

        m_bWorking = FALSE;
        return;
    }

    g_StringLanType(szLan, "结束通话通信失败,错误码:", "hangup call communicate failed,error code:");
    DWORD errorCode = NET_ECMS_GetLastError();
    _snprintf(szLan, 255, "%s%d", szLan, errorCode);
    AddTalkLog(VI_HANGUP, (char *)g_struDeviceInfo[m_iDevIndex].byDeviceID, szLan);
    m_bWorking = FALSE;
    return;
}

BOOL CDlgVideoIntercomCall::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    m_hPlayWnd = GetDlgItem(IDC_STATIC_REALTIME_VIDEO)->GetSafeHwnd();

    m_cmbUnitType.SetCurSel(0);

    //CRect rcMotion(0, 0, 0, 0);
    //GetDlgItem(IDC_PIC_REALPLAY)->GetWindowRect(&rcMotion);
    //ScreenToClient(&rcMotion);
    //rcMotion.right = rcMotion.left - 5 + 16*22;
    //rcMotion.bottom = rcMotion.top - 12 + 16*18;
    //GetDlgItem(IDC_PIC_REALPLAY)->MoveWindow(rcMotion);

    if (m_byRequestType == 0)
    {
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
    }

    char szLan[32] = { 0 };
    memset(szLan, 0, sizeof(szLan));
    CString csTemp = szLan;
    m_listVTalkLog.InsertColumn(0, csTemp, LVCFMT_LEFT, 0, -1);
    g_StringLanType(szLan, "时间", "Time");
    csTemp = szLan;
    m_listVTalkLog.InsertColumn(1, csTemp, LVCFMT_LEFT, 120);
    g_StringLanType(szLan, "状态", "State");
    csTemp = szLan;
    m_listVTalkLog.InsertColumn(2, csTemp, LVCFMT_LEFT, 180);
    g_StringLanType(szLan, "操作", "Operation");
    csTemp = szLan;
    m_listVTalkLog.InsertColumn(3, csTemp, LVCFMT_LEFT, 200);
    g_StringLanType(szLan, "设备信息", "Device Info");
    csTemp = szLan;
    m_listVTalkLog.InsertColumn(4, csTemp, LVCFMT_LEFT, 80);
    m_listVTalkLog.SetExtendedStyle(m_listVTalkLog.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CDlgVideoIntercomCall::IsapiPassthroughRequest(LONG lUserID, int iCmdType, CString sUrl, CString sInput)
{
    NET_EHOME_PTXML_PARAM struPTXML = { 0 };
    struPTXML.pRequestUrl = sUrl.GetBuffer(0);
    struPTXML.dwRequestUrlLen = sUrl.GetLength();

    char szInput[1024 * 100] = { 0 };
    sprintf(szInput, "%s", sInput);

    struPTXML.pCondBuffer = NULL;
    struPTXML.dwCondSize = 0;
    struPTXML.dwInSize = sInput.GetLength();
    if (struPTXML.dwInSize == 0)
    {
        struPTXML.pInBuffer = NULL;
    }
    else
    {
        struPTXML.pInBuffer = szInput;
    }

    char sOutput[1024 * 100] = { 0 };
    struPTXML.pOutBuffer = sOutput;
    struPTXML.dwOutSize = sizeof(sOutput);
    struPTXML.dwRecvTimeOut = TIMEOUT;

    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)struPTXML.pInBuffer, (char*)struPTXML.pInBuffer, struPTXML.dwInSize, &struPTXML.dwInSize);
    A2UTF8((char*)struPTXML.pCondBuffer, (char*)struPTXML.pCondBuffer, struPTXML.dwCondSize, &struPTXML.dwCondSize);
    if (iCmdType == 0) //GET
    {
        if (NET_ECMS_GetPTXMLConfig(lUserID, &struPTXML))
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 1, "NET_ECMS_GetPTXMLConfig");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "NET_ECMS_GetPTXMLConfig");
            return NULL;
        }
    }
    else if (iCmdType == 1) //PUT
    {
        if (NET_ECMS_PutPTXMLConfig(lUserID, &struPTXML))
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 1, "NET_ECMS_PutPTXMLConfig");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "NET_ECMS_PutPTXMLConfig");
            return NULL;
        }
    }
    else if (iCmdType == 2) //POST
    {
        if (NET_ECMS_PostPTXMLConfig(lUserID, &struPTXML))
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 1, "NET_ECMS_PostPTXMLConfig");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "NET_ECMS_PostPTXMLConfig");
            return NULL;
        }
    }
    else if (iCmdType == 3) //DELETE
    {
        if (NET_ECMS_DeletePTXMLConfig(lUserID, &struPTXML))
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 1, "NET_ECMS_DeletePTXMLConfig");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "NET_ECMS_DeletePTXMLConfig");
            return NULL;
        }
    }
    else
    {
        return NULL;
    }

    //需要将字符串字段转换成GB2312
    UTF82A((char*)sOutput, (char*)sOutput, struPTXML.dwOutSize, &struPTXML.dwOutSize);
    return sOutput;
}

void CDlgVideoIntercomCall::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CDlgVideoIntercomCall::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
    SendCancellCmd();

    // show stop
    char szLan[128] = { 0 };
    GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
    GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);

    g_StringLanType(szLan,"","");
    GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
}

#if (_MSC_VER >= 1500)	//vs2008
void CDlgVideoIntercomCall::OnTimer(UINT_PTR nIDEvent)
#else
void CDlgVideoIntercomCall::OnTimer(UINT nIDEvent) 
#endif
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == VIS_TALK_TIMER)
    {
        DWORD dwState = 0;
        char szLan[128] = {0};
        if (!m_bClientCall)
        {
            SendBellTimeoutCmd();
        }
        
        //show close link 
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        if (!m_bClientCall)
        {
            g_StringLanType(szLan,"无人接听","No answer"); 
        }
        else
        {
            g_StringLanType(szLan,"响铃超时","Bell Timeout");
        }
        GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
    }
	CDialog::OnTimer(nIDEvent);
}

void CDlgVideoIntercomCall::OnBtnRefuse() 
{
	// TODO: Add your control notification handler code here
    SendRefuseCmd();  
    char szLan[128] = { 0 };
        
    // show stop

    GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
    GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
    GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
    g_StringLanType(szLan,"拒接","Refuse");
    GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
}

void CDlgVideoIntercomCall::AddTalkLog(int cmd, char* szDeviceID, char * szLogInfo)
{
    CTime  cTime = CTime::GetCurrentTime();
    char szTime[64] = { 0 };
    sprintf(szTime, "%s", cTime.Format("%y-%m-%d %H:%M:%S").GetBuffer(0));

    char szDevInfo[128] = { 0 };
    char szCmdInfo[64] = { 0 };

    if (szDeviceID == NULL)
    {
        int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
        if (iDeviceIndex != -1 && iDeviceIndex < 512)
        {
            sprintf(szDevInfo, "[%s]", g_struDeviceInfo[iDeviceIndex].byDeviceID);
        }
    }
    else
    {
        memcpy(szDevInfo, szDeviceID, strlen(szDeviceID));
    }

    if (m_byRequestType)
    {
        sprintf(szCmdInfo, "dev->demo(%s)", g_cmdString[cmd]);
    }
    else
    {
        sprintf(szCmdInfo, "demo->dev(%s)", g_cmdString[cmd]);
    }

    if (5000 == m_listVTalkLog.GetItemCount())
    {
        m_listVTalkLog.DeleteAllItems();
    }

    m_listVTalkLog.InsertItem(0, "", -1);
    m_listVTalkLog.SetItemText(0, 1, szTime);
    m_listVTalkLog.SetItemText(0, 2, szCmdInfo);
    m_listVTalkLog.SetItemText(0, 3, szLogInfo);
    m_listVTalkLog.SetItemText(0, 4, szDevInfo);

    return;
}

// 对讲交互事件由主对话框转发的消息
afx_msg LRESULT CDlgVideoIntercomCall::OnVedioIntercomeCmd(WPARAM wParam, LPARAM lParam)
{
    VI_CMD enumCmd = (VI_CMD)wParam;

    // 设备deviceID
    char *szDeviceID = (char *)lParam;

    char szLan[256] = { 0 };

    // 目前只支持一路对话，正在工作时，丢掉新的对讲请求
    if (m_bWorking && enumCmd == VI_REQUEST)
    {
        g_StringLanType(szLan, "Demo正忙，已丢弃设备发起的对讲请求", "Busy now! Discarded device initiates talk request");
        AddTalkLog(enumCmd, szDeviceID, szLan);
        delete[]szDeviceID;
        return 0;
    }

    switch (enumCmd)
    {
    case VI_REQUEST: // 设备侧发起呼叫请求
        m_bClientCall = FALSE;
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(TRUE);
        g_StringLanType(szLan, "设备发起对讲请求", "Device initiates Talk request");
        GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
        SetTimer(VIS_TALK_TIMER, 30 * 1000, NULL);
        StartPlayVideo();
        m_bWorking = TRUE;
        break;
    case VI_CANCEL: // 取消本次呼叫
        g_StringLanType(szLan, "设备已取消对讲", "Device Cancel Talk");
        KillTimer(VIS_TALK_TIMER);
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
        StopPlayVideo();
        m_bWorking = FALSE;
        break;
    case VI_ANSWER://接听本次呼叫
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        g_StringLanType(szLan, "通话中...", "Calling...");
        GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
        m_bWorking = TRUE;
        StartPlayVoice();
        break;
    case VI_REJECT: //拒绝来电呼叫
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        g_StringLanType(szLan, "拒绝接听...", "Refuse Answer...");
        GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
        m_bWorking = FALSE;
        StopPlayVideo();
        break;
    case VI_BELL_TIMEOUT: //被叫响铃超时
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        g_StringLanType(szLan, "无人接听", "No answer");
        GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
        m_bWorking = FALSE;
        StopPlayVideo();
        break;
    case VI_HANGUP: // 结束本次通话
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        g_StringLanType(szLan, "已挂断...", "Hung up...");
        GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
        m_bWorking = FALSE;
        StopPlayVideo();
        StopPlayVoice();
        break;
    case VI_DEVICE_ON_CALL://正在通话中
        GetDlgItem(IDC_BTN_INQUEST)->ShowWindow(TRUE);
        GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_ANSWER)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_HANG_UP)->ShowWindow(FALSE);
        GetDlgItem(IDC_BTN_REFUSE)->ShowWindow(FALSE);
        g_StringLanType(szLan, "正在通话中...", "On Calling...");
        GetDlgItem(IDC_STATIC_SHOW_STATUS)->SetWindowText(szLan);
        m_bWorking = FALSE;
    default:
        sprintf(szLan, "Unknown Command Type[%d]", enumCmd);
        m_bWorking = FALSE;
        break;
    }

    AddTalkLog(enumCmd, szDeviceID, szLan);
    delete[]szDeviceID;

    return 0;
}

BOOL CDlgVideoIntercomCall::ProcStreamData(BYTE byDataType, char* pBuffer, int iDataLen)
{
    if (1 == byDataType)
    {
        if (!PlayM4_GetPort(&m_lPort))
        {
            StopPlayVideo();
            return FALSE;
        }
        if (!PlayM4_SetStreamOpenMode(m_lPort, STREAME_REALTIME/*实时流*/))
        {
            StopPlayVideo();
            return FALSE;
        }
        // 先输入头,前40个字节
        if (!PlayM4_OpenStream(m_lPort, (unsigned char *)pBuffer, (DWORD)iDataLen, 2 * 1024 * 1024/*缓冲区*/))
        {
            StopPlayVideo();
            return FALSE;
        }
        //设置解码后缓存区1-实时性好；3-实时性较好；6-实时性中，流畅性中；15-流畅性好
        PlayM4_SetDisplayBuf(m_lPort, 6);

        if (!PlayM4_Play(m_lPort, m_hPlayWnd))
        {
            StopPlayVideo();
            return FALSE;
        }
    }
    else
    {
        int time = 1000;
        while (time > 0)
        {
            BOOL bRet = PlayM4_InputData(m_lPort, (unsigned char *)pBuffer, (DWORD)iDataLen);

            if (!bRet)
            {
                Sleep(5);
                time--;
                continue;
            }
            break;
        }
    }// End of while( TRUE )
    return TRUE;
}

void CALLBACK fnVIDEO_DATA_CB(LONG lPreviewHandle, NET_EHOME_PREVIEW_CB_MSG *pPreviewCBMsg, void *pUserData)
{
    if (NULL == pPreviewCBMsg)
    {
        return;
    }

    CDlgVideoIntercomCall *pThis = (CDlgVideoIntercomCall *)pUserData;
    pThis->m_lPreviewHandle = lPreviewHandle;

    pThis->ProcStreamData(pPreviewCBMsg->byDataType, (char*)pPreviewCBMsg->pRecvdata, pPreviewCBMsg->dwDataLen);

}

BOOL CALLBACK fnVIDEO_NEWLINK_CB(LONG lPreviewHandle, NET_EHOME_NEWLINK_CB_MSG *pNewLinkCBMsg, void *pUserData)
{
    //预览数据回调参数
    NET_EHOME_PREVIEW_DATA_CB_PARAM struDataCB = { 0 };
    struDataCB.fnPreviewDataCB = fnVIDEO_DATA_CB;
    struDataCB.byStreamFormat = 0;//封装格式：0- PS 
    struDataCB.pUserData = pUserData;

    if (!NET_ESTREAM_SetPreviewDataCB(lPreviewHandle, &struDataCB))
    {
        printf("NET_ESTREAM_SetPreviewDataCB failed, error code: %d\n", NET_ESTREAM_GetLastError());
        return FALSE;
    }
    printf("NET_ESTREAM_SetPreviewDataCB!\n");

    return TRUE;
}

NET_EHOME_PREVIEWINFO_OUT g_struPreviewOut = { 0 };

BOOL CDlgVideoIntercomCall::StartPlayVideo()
{
    m_iDevIndex = g_pMainDlg->GetCurDeviceIndex();
    m_iChannelIndex = g_pMainDlg->GetCurChanIndex();

    if (!m_bPlayVideo)
    {
        NET_EHOME_LISTEN_PREVIEW_CFG struListen = { 0 };
        memcpy(&struListen.struIPAdress, &g_struPreviewListen[0].struIP, sizeof(NET_EHOME_IPADDRESS));
        struListen.struIPAdress.wPort += 1;//+1是为了避免和已监听的端口相同
        struListen.fnNewLinkCB = fnVIDEO_NEWLINK_CB; //预览连接请求回调函数
        struListen.pUser = this;
        struListen.byLinkMode = g_struPreviewListen[0].iLinkType;//0- TCP方式，1- UDP方式 
        struListen.byLinkEncrypt = 0;//默认监听不加密

        //启动预览监听
        m_lListenHandle = NET_ESTREAM_StartListenPreview(&struListen);
        if (m_lListenHandle <= -1)
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "CDlgVideoIntercomCall::StartPlayVideo, NET_ESTREAM_StartListenPreview");
            return false;
        }

        //预览请求输入参数
        NET_EHOME_PREVIEWINFO_IN_V11 struPreviewIn = { 0 };
        struPreviewIn.dwLinkMode = g_struDeviceInfo[m_iDevIndex].struChanInfo[m_iChannelIndex].dwLinkMode;
        struPreviewIn.struStreamSever.wPort = g_struDeviceInfo[m_iDevIndex].struChanInfo[m_iChannelIndex].struIP.wPort + 1;
        memcpy(struPreviewIn.struStreamSever.szIP, g_struDeviceInfo[m_iDevIndex].struChanInfo[m_iChannelIndex].struIP.szIP, 128);

        struPreviewIn.dwStreamType = g_struDeviceInfo[m_iDevIndex].struChanInfo[m_iChannelIndex].dwStreamType;
        struPreviewIn.iChannel = 1;

        struPreviewIn.byEncrypt = g_struDeviceInfo[m_iDevIndex].struChanInfo[m_iChannelIndex].byEncrypt;

        //预览请求输出参数
        NET_EHOME_PREVIEWINFO_OUT struPreviewOut = { 0 };

        if (!NET_ECMS_StartGetRealStreamV11(g_struDeviceInfo[m_iDevIndex].lLoginID, &struPreviewIn, &struPreviewOut))
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "CDlgVideoIntercomCall::StartPlayVideo, NET_ECMS_StartGetRealStreamV11");
            return false;
        }

        memcpy(&g_struPreviewOut, &struPreviewOut, sizeof(struPreviewOut));

        //预览请求推流输入参数 
        NET_EHOME_PUSHSTREAM_IN struPushStreamIn = { 0 };
        struPushStreamIn.dwSize = sizeof(struPushStreamIn);
        struPushStreamIn.lSessionID = struPreviewOut.lSessionID; //SessionID，预览请求会话ID

        //预览请求推流输出参数 
        NET_EHOME_PUSHSTREAM_OUT struPushStreamOut = { 0 };
        if (!NET_ECMS_StartPushRealStream(g_struDeviceInfo[m_iDevIndex].lLoginID, &struPushStreamIn, &struPushStreamOut))
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "CDlgVideoIntercomCall::StartPlayVideo, NET_ECMS_StartPushRealStream");
            return false;
        }

        m_bPlayVideo = TRUE;
    }

    return true;
}

BOOL CDlgVideoIntercomCall::StopPlayVideo()
{
    if (m_bPlayVideo)
    {
        if (m_lPreviewHandle >= 0)
        {
            if (!NET_ESTREAM_StopPreview(m_lPreviewHandle))
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "CDlgVideoIntercomCall::StopPlayVideo, NET_ESTREAM_StopPreview");
            }
            m_lPreviewHandle = -1;
        }

        if (!NET_ECMS_StopGetRealStream(g_struDeviceInfo[m_iDevIndex].lLoginID, g_struPreviewOut.lSessionID))
        {
            g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "CDlgVideoIntercomCall::StopPlayVideo, NET_ECMS_StopGetRealStream");
        }

        if (m_lListenHandle >= 0)
        {
            if (!NET_ESTREAM_StopListenPreview(m_lListenHandle))
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 1, "CDlgVideoIntercomCall::StopPlayVideo, NET_ESTREAM_StopListenPreview");
            }
            m_lListenHandle = -1;
        }


        if (m_lPort >= 0)
        {
            if (!PlayM4_Stop(m_lPort))
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 0, "PlayM4_Stop err[%d]", PlayM4_GetLastError(m_lPort));
            }
            else
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 0, "PlayM4_Stop");
            }
            if (!PlayM4_CloseStream(m_lPort))
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 0, "PlayM4_CloseStream err[%d]", PlayM4_GetLastError(m_lPort));
            }
            else
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 0, "PlayM4_CloseStream");
            }
            if (!PlayM4_FreePort(m_lPort))
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 0, "PlayM4_FreePort err[%d]", PlayM4_GetLastError(m_lPort));
            }
            else
            {
                g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 0, "PlayM4_FreePort");
            }

            m_lPort = -1;
        }

        m_bPlayVideo = FALSE;
    }


    //刷新预览控件
    GetDlgItem(IDC_STATIC_REALTIME_VIDEO)->ShowWindow(FALSE);
    GetDlgItem(IDC_STATIC_REALTIME_VIDEO)->ShowWindow(TRUE);

    return true;
}

BOOL CDlgVideoIntercomCall::StartPlayVoice()
{
    if (!m_bPlayVoice)
    {
        //开启对讲监听
        g_pAudioTalkDlg->OnBnClickedBtnStartVoicetalkListen();
        //开启对讲
        g_pAudioTalkDlg->OnBnClickedBtnReqAudioTalk();

        m_bPlayVoice = TRUE;
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 0, "StartPlayVoice");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 0, "StartPlayVoice, Voice Already Start");
    }

    return true;
}

BOOL CDlgVideoIntercomCall::StopPlayVoice()
{
    if (m_bPlayVoice)
    {
        //停止对讲
        g_pAudioTalkDlg->OnBnClickedBtnStopPush();
        //停止对讲监听
        g_pAudioTalkDlg->OnBnClickedBtnStopListen();
        m_bPlayVoice = FALSE;
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, 0, "StopPlayVoice");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, 0, "StopPlayVoice, Voice Already Stop");
    }

    return true;
}

void CDlgVideoIntercomCall::OnBnClickedBtnVideoTest()
{
    // TODO:  在此添加控件通知处理程序代码
    if (m_bPlayVideo)
    {
        StopPlayVideo();
        m_bPlayVideo = FALSE;
        GetDlgItem(IDC_BTN_VIDEO_TEST)->SetWindowTextA("开始视频测试");
    }
    else
    {
        if (StartPlayVideo())
        {
            m_bPlayVideo = TRUE;
            GetDlgItem(IDC_BTN_VIDEO_TEST)->SetWindowTextA("停止视频测试");
        }
    }
}


void CDlgVideoIntercomCall::OnBnClickedBtnVoiceTest()
{
    // TODO:  在此添加控件通知处理程序代码
    if (m_bPlayVoice)
    {
        StopPlayVoice();
        m_bPlayVoice = FALSE;
        GetDlgItem(IDC_BTN_VOICE_TEST)->SetWindowTextA("开始语音测试");
    }
    else
    {
        if (StartPlayVoice())
        {
            m_bPlayVoice = TRUE;
            GetDlgItem(IDC_BTN_VOICE_TEST)->SetWindowTextA("停止语音测试");
        }
    }
}
