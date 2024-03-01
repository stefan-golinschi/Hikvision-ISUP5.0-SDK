// DlgAudioTalk.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgAudioTalk.h"
#include <assert.h>
#include "Public/Convert/Convert.h"


// CDlgAudioTalk dialog

BOOL g_bStopAudioTalk = TRUE;

CDlgAudioTalk *g_pAudioTalkDlg = NULL;

char g_szRemoteIP[128] = {0};
int  g_nRemotePort = 0;

IMPLEMENT_DYNAMIC(CDlgAudioTalk, CDialog)

#define CSTRING_TO_CHARS(str,sz) memcpy((sz),str.GetBuffer(str.GetLength()),str.GetLength())

CDlgAudioTalk *CDlgAudioTalk::s_pAudioTalkDlg = NULL;

/*********************************************************
  Function:    DataFromSoundIn
  Desc:        get local audio data and send to device
  Input:    buffer, local voice data buffer; dwSize, data length; dwOwner, owner data;
  Output:    none
  Return:    none
**********************************************************/
void CALLBACK DataFromSoundInEx(char* buffer, DWORD dwSize, void* pOwner)
{
    ASSERT(pOwner);
    CDlgAudioTalk* p = (CDlgAudioTalk*)pOwner;
    p->PrepareLocalData(buffer, dwSize, pOwner);
}


CDlgAudioTalk::CDlgAudioTalk(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgAudioTalk::IDD, pParent)
    , m_byEncrypt(FALSE)
    , m_bVoiceBroadcast(FALSE)
    , m_iBroadcastLevel(0)
    , m_iBroadcastVolume(0)
{
    m_iCurDevIndex = -1;
    memset(&m_struVoiceTalkPara, 0, sizeof(m_struVoiceTalkPara));
    m_lVoiceTalkHandle = -1;
    m_hVoiceTransmit = NULL;
    m_bExitThread = FALSE;
    m_bCMSAudioTalk = TRUE;

    m_csIP = g_pMainDlg->m_sLocalIP;
    m_nPort = 7500;

    m_lListenHandle = -1;

    m_bWaveDeal = FALSE;

    m_dwBufNum = 6;

    memset(&m_talkMr, 0, sizeof(m_talkMr));

    m_talkMr.byAudioType = AUDIOTALKTYPE_PCM;

    //init WAVEFORMATEX
    m_struWaveFormat.cbSize             = sizeof(WAVEFORMATEX);
    m_struWaveFormat.nBlockAlign        = CHANNEL * BITS_PER_SAMPLE / 8;
    m_struWaveFormat.nChannels          = CHANNEL;
    m_struWaveFormat.nSamplesPerSec     = SAMPLES_PER_SECOND;
    m_struWaveFormat.wBitsPerSample     = BITS_PER_SAMPLE;
    m_struWaveFormat.nAvgBytesPerSec    = SAMPLES_PER_SECOND * m_struWaveFormat.nBlockAlign;
    m_struWaveFormat.wFormatTag         = WAVE_FORMAT_PCM;

    m_hExitEvent = NULL;

    m_pRenderBuf = NULL;
    m_rIndexRV = 0;
    m_ReceiveIndexRV = 0;
    m_nBufNo = 0;
    m_bOpenPlayThread = FALSE;

    m_pRenderBuf = new BYTE[160*40];

    m_bRecord = FALSE;

    m_nLocalType = 0;
    m_bOpenTalk = FALSE;

    m_hVoiceSendThread = NULL;
    m_bVoiceSendThreadStart = FALSE;
    m_AudioSendDataList.clear();

    m_lAudioTalkHandle = -1;

    g_pAudioTalkDlg = this;
    m_bVersion4 = FALSE;
}

CDlgAudioTalk::~CDlgAudioTalk()
{
    if (m_pRenderBuf != NULL)
    {
        delete[] m_pRenderBuf;
    }

    DeleteCriticalSection(&m_csAudioBuf);
    CloseHandle(m_hListMutex);
    g_pMainDlg->RegisterDevIDMsg(NULL, NULL);
}

void CDlgAudioTalk::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CMB_VOICETALK_CB_TYPE, m_combCBDataType);
    DDX_Control(pDX, IDC_CMB_VOICE_CHAN, m_cmbVoiceChan);
    DDX_Control(pDX, IDC_COM_LOCAL_TYPE, m_cmbLocalType);
    DDX_Text(pDX, IDC_EDT_STREAM_PORT, m_nPort);
    DDX_Text(pDX, IDC_IPADDR_STREAM_SERVER, m_csIP);
    DDX_Control(pDX, IDC_COMBO_AUDIOTYPE, m_cmbAudioType);
    DDX_Check(pDX, IDC_CHECK_ENCRYPT, m_byEncrypt);
    DDX_Check(pDX, IDC_CHECK_VOICE_BROADCAST, m_bVoiceBroadcast);
    DDX_Text(pDX, IDC_EDIT_BROADCAST_LEVEL, m_iBroadcastLevel);
    DDX_Text(pDX, IDC_EDIT_BROADCAST_VOLUME, m_iBroadcastVolume);
    DDX_Control(pDX, IDC_COM_SAMPLING_RATE, m_cmbAudioSamplingRate);
}


BEGIN_MESSAGE_MAP(CDlgAudioTalk, CDialog)
    ON_BN_CLICKED(IDC_BTN_START_VOICETALK, &CDlgAudioTalk::OnBnClickedBtnStartVoicetalk)
    ON_BN_CLICKED(IDC_BTN_STOP_VOICETALK, &CDlgAudioTalk::OnBnClickedBtnStopVoicetalk)
    ON_BN_CLICKED(IDC_BTN_START_TRANSMIT, &CDlgAudioTalk::OnBnClickedBtnStartTransmit)
    ON_BN_CLICKED(IDC_BTN_STOP_TRANSMIT, &CDlgAudioTalk::OnBnClickedBtnStopTransmit)
    ON_BN_CLICKED(IDC_BTN_LISTEN, &CDlgAudioTalk::OnBnClickedBtnStartVoicetalkListen)
    ON_BN_CLICKED(IDC_BTN_REQ_AUDIOTALK, &CDlgAudioTalk::OnBnClickedBtnReqAudioTalk)
    ON_BN_CLICKED(IDC_BTN_START_PUSH, &CDlgAudioTalk::OnBnClickedBtnStartPush)
    ON_BN_CLICKED(IDC_BTN_STOP_AUDIOTALK, &CDlgAudioTalk::OnBnClickedBtnStopPush)
    ON_BN_CLICKED(IDC_BTN_STOP_LISTEN, &CDlgAudioTalk::OnBnClickedBtnStopListen)
    ON_BN_CLICKED(IDC_BTN_AUDIOTALK_TEST, &CDlgAudioTalk::OnBnClickedBtnAudiotalkTest)
    ON_MESSAGE(WM_START_VOICE_TALK_OK, &CDlgAudioTalk::OnStartVoiceTalkOK)
    ON_MESSAGE(WM_START_PUSH_VOICE_TALK_OK, &CDlgAudioTalk::OnStartPushVoiceTalkOK)
    ON_MESSAGE(WM_STOP_VOICE_TALK_OK, &CDlgAudioTalk::OnStopVoiceTalkWithSTMOK)
    ON_BN_CLICKED(IDC_BTN_VEDIO_INTERCOME, &CDlgAudioTalk::OnBnClickedBtnVedioIntercome)
    ON_BN_CLICKED(IDC_BTN_START_VOICETALK2, &CDlgAudioTalk::OnBnClickedBtnStartVoicetalk2)
    ON_BN_CLICKED(IDC_BTN_STOP_VOICETALK3, &CDlgAudioTalk::OnBnClickedBtnStopVoicetalk3)
END_MESSAGE_MAP()



BOOL CDlgAudioTalk::OnInitDialog()
{
    CDialog::OnInitDialog();

    char szLan[128] = {0};
    CString csTemp = _T("");

    // TODO:  Add extra initialization here
    m_combCBDataType.DeleteString(0);
    g_StringLanType(szLan, "编码数据", "decode data");
    csTemp = szLan;
    m_combCBDataType.AddString(csTemp);
    g_StringLanType(szLan, "PCM数据", "PCM data");
    csTemp = szLan;
    m_combCBDataType.AddString(csTemp);

    m_combCBDataType.SetCurSel(0);
    m_cmbAudioSamplingRate.SetCurSel(0);

    m_hExitEvent   = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hHaveRDataRV = CreateEvent(NULL, TRUE, FALSE, NULL);

    //encoder
    //m_pEncoder = NET_DVR_InitG722Encoder();

    //g726
    //m_pG726Enc = NET_DVR_InitG726Encoder(&m_pG726EncM);

    m_cmbLocalType.InsertString(0, _T("none"));
    m_cmbLocalType.InsertString(1, _T("Local Play"));
    m_cmbLocalType.InsertString(2, _T("ECHO"));
    m_cmbLocalType.InsertString(3, _T("ECHO & Local Play"));
    m_cmbLocalType.SetCurSel(0);

    m_cmbAudioType.InsertString(0, _T("OggVorbis"));
    m_cmbAudioType.InsertString(1, _T("G711.MU"));
    m_cmbAudioType.InsertString(2, _T("G711.A"));
    m_cmbAudioType.InsertString(3, _T("G726"));
    m_cmbAudioType.InsertString(4, _T("AAC"));
    m_cmbAudioType.InsertString(5, _T("MP2L2"));
    m_cmbAudioType.InsertString(6, _T("PCM"));
    m_cmbAudioType.InsertString(7, _T("MP3"));
    m_cmbAudioType.InsertString(8, _T("G723"));
    m_cmbAudioType.InsertString(9, _T("MP1L2"));
    m_cmbAudioType.InsertString(10, _T("ADPCM"));
    m_cmbAudioType.SetCurSel(0);

    UpdateData(FALSE);

    s_pAudioTalkDlg = this;
    m_hListMutex = CreateMutex(nullptr, FALSE, nullptr);
    InitializeCriticalSection(&m_csAudioBuf);

    g_pMainDlg->RegisterDevIDMsg(DeviceIDChanged, this);


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

FILE *g_file = NULL;

void CALLBACK g_fVoiceDataCallBack(LONG iVoiceComHandle, char *pRecvDataBuffer, DWORD dwBufSize, DWORD dwEncodeType, BYTE byAudioFlag, void *pUser)
{
    UN_REFERENCED_PARAMETER(pUser);
    UN_REFERENCED_PARAMETER(byAudioFlag);
    UN_REFERENCED_PARAMETER(dwEncodeType);
//     if (!NET_ECMS_SendVoiceTransData(iVoiceComHandle, pRecvDataBuffer, dwBufSize))
//     {
//         printf("NET_ECMS_SendVoiceTransData failed,errorcode:%d\n", NET_ECMS_GetLastError());
//     }
//     else
//     {
//         printf("NET_ECMS_SendVoiceTransData success!\n");
//     }
    /* UN_REFERENCED_PARAMETER(iVoiceComHandle)
     UN_REFERENCED_PARAMETER(pRecvDataBuffer)
     UN_REFERENCED_PARAMETER(dwBufSize)
     UN_REFERENCED_PARAMETER(dwEncodeType)
     UN_REFERENCED_PARAMETER(pUser)
     UN_REFERENCED_PARAMETER(byAudioFlag)*/
    /*
    if (byAudioFlag == 1)
    {
        if (g_file == NULL)
        {
            char szAudioFilePath[MAX_PATH] = {0};
            char szAppPath[MAX_PATH] = {0};
            GetCurrentDirectory(MAX_PATH, szAppPath);                   //获取程序的当前目录
            sprintf(szAudioFilePath, "%s/AudioFiles/%s", szAppPath, "ehome_recv.au");

            g_file = fopen(szAudioFilePath, "wb");
        }
        fwrite(pRecvDataBuffer, dwBufSize, 1, g_file);
    }
    */
}

//设备连接上来时
BOOL CALLBACK CDlgAudioTalk::onDeviceAudioTalkConnectCB(LONG lAudioTalkHandle, NET_EHOME_VOICETALK_NEWLINK_CB_INFO *pNewLinkCBMsg, void *pUserData)
{
    CDlgAudioTalk *pVoiceTalk = (CDlgAudioTalk*)pUserData;
    if (pVoiceTalk->m_iSessionID != pNewLinkCBMsg->lSessionID)
    {
        return FALSE;
    }
    pVoiceTalk->m_lAudioTalkHandle = lAudioTalkHandle;

    if (g_struDeviceInfo[pVoiceTalk->m_iCurDevIndex].dwVersion == 5)
    {
        memcpy(pNewLinkCBMsg->szDeviceID, g_struDeviceInfo[pVoiceTalk->m_iCurDevIndex].byDeviceID, 256);
        memcpy(pNewLinkCBMsg->sDeviceSerial, g_struDeviceInfo[pVoiceTalk->m_iCurDevIndex].sDeviceSerial, 12);
        pNewLinkCBMsg->dwAudioChan = pVoiceTalk->m_cmbVoiceChan.GetCurSel() + 1;
        memcpy(pNewLinkCBMsg->byToken, "ut.d463nmdbdgqrpto754vcyo11bltilzxy-5j0uqbwhyc-1rxlzdx-trnzgryzz", sizeof(pNewLinkCBMsg->byToken));
    }

    g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 2, "onDeviceAudioTalkConnectCB: szDeviceID[%s] iSessionID[%d] dwChannelNo[%d] dwEncodeType[%d]",
        pNewLinkCBMsg->szDeviceID, pNewLinkCBMsg->lSessionID, pNewLinkCBMsg->dwAudioChan, pNewLinkCBMsg->dwEncodeType);
    //音频编码类型设置
    //pVoiceTalk->m_AudioTalkParam.byAudioType = pNewLinkCBMsg->dwEncodeType;
    //pVoiceTalk->m_AudioTalkParam.byAudioType = pVoiceTalk->m_cmbAudioType.GetCurSel();
    //设置回调
    NET_EHOME_VOICETALK_DATA_CB_PARAM struAudioTalkCBParam = { 0 };

    struAudioTalkCBParam.fnVoiceTalkDataCB = CDlgAudioTalk::AudioTalkStreamCallback;
    struAudioTalkCBParam.pUserData = pUserData;

    if (!NET_ESTREAM_SetVoiceTalkDataCB(lAudioTalkHandle, &struAudioTalkCBParam))
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 2, "NET_ESTREAM_SetVoiceTalkDataCB, Error[%d]", NET_ESTREAM_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ESTREAM_SetVoiceTalkDataCB");
    }

    if (pVoiceTalk->m_AudioTalkParam.byAudioType != pNewLinkCBMsg->dwEncodeType && 0 != pNewLinkCBMsg->dwEncodeType)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "信令编码格式与流媒体编码格式不一致");
        return FALSE;
    }

    pVoiceTalk->m_AudioTalkParam.byAudioType = (BYTE)pNewLinkCBMsg->dwEncodeType;
    pVoiceTalk->m_cmbAudioType.SetCurSel(pVoiceTalk->m_AudioTalkParam.byAudioType);

    pVoiceTalk->OpenAudioOut();//播放接收的音频数据

    if (1 == pVoiceTalk->m_nLocalType)//用户选择echo或echo local play模式不采集，选择local play采集
    {
        pVoiceTalk->OpenAudioIn(); //采集本地音频数据
    }

    return TRUE;
}

void CDlgAudioTalk::OnBnClickedBtnStartVoicetalkListen()
{
    UpdateData(TRUE);
    if (m_lListenHandle > -1)
    {
        MessageBox("Already StartListen");
        return;
    }

    NET_EHOME_LISTEN_VOICETALK_CFG struListen = {0};
    struListen.fnNewLinkCB = CDlgAudioTalk::onDeviceAudioTalkConnectCB;
    struListen.pUser = this;

    CSTRING_TO_CHARS(m_csIP, struListen.struIPAdress.szIP);
    struListen.struIPAdress.wPort = (WORD)m_nPort;     //端口
    struListen.byLinkEncrypt = m_byEncrypt;

    m_lListenHandle = NET_ESTREAM_StartListenVoiceTalk(&struListen);
    if (m_lListenHandle > -1)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ESTREAM_StartListenAudioTalk, Error=[%d]", NET_ESTREAM_GetLastError());
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 2, "NET_ESTREAM_StartListenAudioTalk, Error=[%d]", NET_ESTREAM_GetLastError());
    }

    UpdateData(FALSE);
}

void CDlgAudioTalk::OnBnClickedBtnStopListen()
{
     if (NET_ESTREAM_StopListenVoiceTalk(m_lListenHandle))
     {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ESTREAM_StopListenVoiceTalk");
        m_lListenHandle = -1;
     }
     else
     {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 2, "NET_ESTREAM_StopListenVoiceTalk");
     }
     
}

void CDlgAudioTalk::OnBnClickedBtnReqAudioTalk()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);

    m_bVersion4 = TRUE;
    if (m_bOpenTalk)
    {
        MessageBox("Already Start Talk");
        return;
    }

    m_bCMSAudioTalk = FALSE;

    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    m_iCurDevIndex = iDeviceIndex;
    NET_EHOME_VOICE_TALK_IN struVoiceTalkIn = {0};
    struVoiceTalkIn.dwVoiceChan = m_cmbVoiceChan.GetCurSel() + 1;
    struVoiceTalkIn.struStreamSever.wPort = (WORD)m_nPort;
    struVoiceTalkIn.byEncodingType[0] = ENUM_ENCODING_G711_MU;
    struVoiceTalkIn.byLinkEncrypt = m_byEncrypt;
    struVoiceTalkIn.byBroadcast = m_bVoiceBroadcast;
    struVoiceTalkIn.byBroadLevel = m_iBroadcastLevel;
    struVoiceTalkIn.byBroadVolume = m_iBroadcastVolume;
    struVoiceTalkIn.byAudioSamplingRate = m_cmbAudioSamplingRate.GetCurSel();
    //获取一下IP地址和端口号吧
    DWORD dwPortTemp = 0;
    g_pMainDlg->GetAddressByType(2, 0, struVoiceTalkIn.struStreamSever.szIP, sizeof(struVoiceTalkIn.struStreamSever.szIP), (char *)&dwPortTemp, sizeof(dwPortTemp));
    if (0 != dwPortTemp)
    {
        struVoiceTalkIn.struStreamSever.wPort = (WORD)dwPortTemp;
    }
    int iAudioType = -1;
    if (GetAudioType(iAudioType))
    {
        m_talkMr.byAudioType = (BYTE)iAudioType;
    }
    
    NET_EHOME_VOICE_TALK_OUT struVoiceTalkOut = {0};

    if (!NET_ECMS_StartVoiceWithStmServer(g_struDeviceInfo[iDeviceIndex].lLoginID, &struVoiceTalkIn, &struVoiceTalkOut))
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_StartVoiceWithStmServer dwVoiceChan[%d]", struVoiceTalkIn.dwVoiceChan);
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ECMS_StartVoiceWithStmServer");
        m_bExitThread = FALSE;
        if (!g_pMainDlg->m_bAsyncCalllback)
        {
            AfterStartVoiceWithSTM(struVoiceTalkOut.lSessionID);
        }
    }
}

LRESULT CDlgAudioTalk::OnStartVoiceTalkOK(WPARAM wParam, LPARAM lParam)
{
    int lSessionID = (int)lParam;
    AfterStartVoiceWithSTM(lSessionID);
    return 0;
}

void CDlgAudioTalk::AfterStartVoiceWithSTM(int lSessionID)
{
    m_iSessionID = lSessionID;
    int iAudioType = -1;
    if (GetAudioType(iAudioType))
    {
        m_AudioTalkParam.byAudioType = (BYTE)iAudioType;
        m_cmbAudioType.SetCurSel(m_AudioTalkParam.byAudioType);
    }
    OnBnClickedBtnStartPush();
    m_bOpenTalk = TRUE;
}

void CDlgAudioTalk::OnBnClickedBtnStartPush()
{
    m_nLocalType = m_cmbLocalType.GetCurSel();
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    m_iCurDevIndex = iDeviceIndex;
    OpenAudioOut();//播放接收的音频数据

    NET_EHOME_PUSHVOICE_IN struPushVoiceIn = { 0 };
    struPushVoiceIn.dwSize = sizeof(struPushVoiceIn);
    struPushVoiceIn.lSessionID = m_iSessionID;
    if (g_struDeviceInfo[iDeviceIndex].dwVersion == 5)
    {
        memcpy(struPushVoiceIn.byToken, "ut.d463nmdbdgqrpto754vcyo11bltilzxy-5j0uqbwhyc-1rxlzdx-trnzgryzz", sizeof(struPushVoiceIn.byToken));
    }

    NET_EHOME_PUSHVOICE_OUT struPushVoiceOut = { 0 };
    struPushVoiceOut.dwSize = sizeof(struPushVoiceOut);

    if (!NET_ECMS_StartPushVoiceStream(g_struDeviceInfo[iDeviceIndex].lLoginID, &struPushVoiceIn, &struPushVoiceOut))
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_StartPushVoiceStream");
    }
    else
    {
        g_bStopAudioTalk = FALSE;
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ECMS_StartPushVoiceStream");

        AfterStartPushVoiceStream();
    }
}

LRESULT CDlgAudioTalk::OnStartPushVoiceTalkOK(WPARAM wParam, LPARAM lParam)
{
    AfterStartPushVoiceStream();
    return 0;
}

void CDlgAudioTalk::AfterStartPushVoiceStream()
{
    //需要确认Push是否成功
    DWORD dwThreadId;
    m_bVoiceSendThreadStart = TRUE;
    m_hVoiceSendThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(SendDataFromSoundIn), this, 0, &dwThreadId);
    m_bOpenTalk = TRUE;
}



void CDlgAudioTalk::OnBnClickedBtnStopPush()
{
    g_bStopAudioTalk = TRUE;
    m_bVersion4 = FALSE;
    m_bExitThread = TRUE;

    WaitForSingleObject(m_hVoiceTransmit, 3000);

    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    //if(!NET_ECMS_StopVoiceTalkWithStmServer(g_struDeviceInfo[iDeviceIndex].lLoginID, m_iSessionID))
    NET_EHOME_STOPVOICETALK_STM_PARAM struStopParam = { 0 };
    struStopParam.lSessionID = m_iSessionID;
    if (!NET_ECMS_StopVoiceTalkWithStmServerEx(g_struDeviceInfo[iDeviceIndex].lLoginID, &struStopParam))
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_StopVoiceTalkWithStmServer");
    }
    else
    {
        g_bStopAudioTalk = FALSE;
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ECMS_StopVoiceTalkWithStmServer");
    }

    if (!g_pMainDlg->m_bAsyncCalllback && !m_bVersion4)
    {
        //同步
        AfterStopVoiceTalkWithSTM();
    }
}

LRESULT CDlgAudioTalk::OnStopVoiceTalkWithSTMOK(WPARAM wParam, LPARAM lParam)
{
    AfterStopVoiceTalkWithSTM();
    return 0;
}

void CDlgAudioTalk::AfterStopVoiceTalkWithSTM()
{
    if (g_file != NULL)
    {
        fclose(g_file);
        g_file = NULL;
    }

    NET_ESTREAM_StopVoiceTalk(m_lAudioTalkHandle);
    m_bOpenTalk = FALSE;

    m_bVoiceSendThreadStart = FALSE;
    if (WAIT_TIMEOUT == WaitForSingleObject(m_hVoiceSendThread, 3000))
    {
        TerminateThread(m_hVoiceSendThread, 0);
    }
    m_hVoiceSendThread = NULL;

    m_AudioTalkCom.CloseAudioIn();
    m_AudioTalkCom.CloseAudioOut();

    std::list<AUDIO_DATA>::iterator iter = m_AudioSendDataList.begin();
    for (iter; iter != m_AudioSendDataList.end(); iter++)
    {
        if ((*iter).buffer != NULL)
        {
            delete (*iter).buffer;
            (*iter).buffer = NULL;
        }
    }
    m_AudioSendDataList.clear();

    ChangeAudioCtlState(g_pMainDlg->GetCurChanIndex());
}

BOOL CDlgAudioTalk::PrepareLocalData(char* buffer, DWORD dwSize, void* pOwner)
{
    CDlgAudioTalk* p = (CDlgAudioTalk*)pOwner;
    if (!p->m_bVoiceSendThreadStart)
    {
        return FALSE;
    }

    AUDIO_DATA stuAudioData;
    memset(&stuAudioData, 0, sizeof(AUDIO_DATA));
    char *pAudioBuff = new char[2048];
    if (NULL == pAudioBuff)
    {
        return FALSE;
    }
    //AAC格式长度可变,PCM下固定为1920,其余长度<160
    memcpy(pAudioBuff, buffer, min(dwSize, 2048));
    stuAudioData.buffer = pAudioBuff;
    stuAudioData.dwSize = min(dwSize, 2048);
    WaitForSingleObject(p->m_hListMutex, INFINITE);
    p->m_AudioSendDataList.push_back(stuAudioData);
    ReleaseMutex(p->m_hListMutex);

    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ESTREAM_onVoiceTalkData, size=[%d]", size);
    return TRUE;
}

DWORD WINAPI CDlgAudioTalk::SendDataFromSoundIn(LPVOID lpArg)
{
    CDlgAudioTalk* p = (CDlgAudioTalk*)lpArg;

    while (p->m_bVoiceSendThreadStart)
    {
        AUDIO_DATA stuAudioData;
        memset(&stuAudioData, 0, sizeof(AUDIO_DATA));
        WaitForSingleObject(p->m_hListMutex, INFINITE);
        if (p->m_AudioSendDataList.empty())
        {
            ReleaseMutex(p->m_hListMutex);
            Sleep(5);
            continue;
        }
        stuAudioData = p->m_AudioSendDataList.front();
        p->m_AudioSendDataList.pop_front();
        ReleaseMutex(p->m_hListMutex);

        if (stuAudioData.buffer != NULL && stuAudioData.dwSize != 0)
        {
            p->SendDataToDevice(stuAudioData.buffer, stuAudioData.dwSize);
            //             TALK_MR struTalkMr;
            //             struTalkMr.byAudioType = 0;
            //             p->PutIntoBuf(stuAudioData.buffer, stuAudioData.dwSize, &struTalkMr);
            delete stuAudioData.buffer;
            stuAudioData.buffer = NULL;
        }
    }
    return 0;
}

void CALLBACK CDlgAudioTalk::DeviceIDChanged(LONG iDeviceID, void *pUserData)
{
    CDlgAudioTalk *pThis = static_cast<CDlgAudioTalk *>(pUserData);
    if (NULL != pThis)
    {
        pThis->ChangeAudioCtlState(iDeviceID);
    }
}

void CDlgAudioTalk::ChangeAudioCtlState(LONG iDeviceID)
{
    if (m_lAudioTalkHandle >= 0 || m_lVoiceTalkHandle >= 0 || iDeviceID > MAX_DEVICES)
    {
        return; //已经在对讲的情况下不做任何改变
    }
    if (iDeviceID < 0)
    {
        GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_STOP_VOICETALK)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_STOP_TRANSMIT)->EnableWindow(FALSE);

        GetDlgItem(IDC_BTN_REQ_AUDIOTALK)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_START_PUSH)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_STOP_AUDIOTALK)->EnableWindow(FALSE);
    }
    else
    {
        m_iCurDevIndex = iDeviceID;
        //获取设备协议类型，修改控件状态
        DWORD dwVersion = g_struDeviceInfo[m_iCurDevIndex].dwVersion;
        if (dwVersion == 2)
        {
            GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_STOP_VOICETALK)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_STOP_TRANSMIT)->EnableWindow(TRUE);

            GetDlgItem(IDC_BTN_REQ_AUDIOTALK)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_START_PUSH)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_STOP_AUDIOTALK)->EnableWindow(FALSE);
        }
        else
        {
            GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_STOP_VOICETALK)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_STOP_TRANSMIT)->EnableWindow(FALSE);

            GetDlgItem(IDC_BTN_REQ_AUDIOTALK)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_START_PUSH)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_STOP_AUDIOTALK)->EnableWindow(TRUE);
        }
    }
    CheckInitParam();
}

//老的语音对讲
void CDlgAudioTalk::OnBnClickedBtnStartVoicetalk()
{
    // TODO: Add your control notification handler code here
    m_struVoiceTalkPara.bNeedCBNoEncData = m_combCBDataType.GetCurSel();
    m_struVoiceTalkPara.pUser  = this;
    m_struVoiceTalkPara.byVoiceTalk = 0;
    m_struVoiceTalkPara.cbVoiceDataCallBack = g_fVoiceDataCallBack;

    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();

    m_lVoiceTalkHandle = NET_ECMS_StartVoiceTalk(g_struDeviceInfo[iDeviceIndex].lLoginID, m_cmbVoiceChan.GetCurSel() + 1, &m_struVoiceTalkPara);
    if (m_lVoiceTalkHandle < 0)
    {
        int err = NET_ECMS_GetLastError();
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_StartVoiceTalk");
        return;
    }
    GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(FALSE);
}

void CDlgAudioTalk::OnBnClickedBtnStopVoicetalk()
{
    // TODO: Add your control notification handler code here

//     if (NET_ECMS_StopListen(0))
//     {
//         g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ECMS_StopListen() Success");
//     }
//     else
//     {
//         g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_StopListen() Failed");
//     }
// 
//     //DEBUG
//     if (NET_ECMS_Fini())
//     {
//         g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ECMS_Fini() Success");
//     }
//     else
//     {
//         g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_Fini() Failed");
//     }
//     return;

    if (NET_ECMS_StopVoiceTalk(m_lVoiceTalkHandle))
    {
        m_lVoiceTalkHandle = -1;
        GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(TRUE);
    }

    ChangeAudioCtlState(g_pMainDlg->GetCurDeviceIndex());
}


int g_nRecvTotalLen = 0;


BOOL CALLBACK CDlgAudioTalk::AudioTalkStreamCallback(LONG iAudioTalkHandle, NET_EHOME_VOICETALK_DATA_CB_INFO *pVoiceTalkCBMsg, void *pUserData)
{
    UN_REFERENCED_PARAMETER(iAudioTalkHandle)
    //在这里进行语音的解码和播放
    //WARNING:设置回调时，pUser目前被设置成NULL，先不要使用

    CDlgAudioTalk *pVoiceTalk = (CDlgAudioTalk*)pUserData;

    //if (!pVoiceTalk->m_bExitThread)
    {

        g_nRecvTotalLen += pVoiceTalkCBMsg->dwDataLen;

        if (pVoiceTalk->m_nLocalType & 1)
        {
            //本地播放
            pVoiceTalk->PutIntoBuf((char*)pVoiceTalkCBMsg->pData, (int)pVoiceTalkCBMsg->dwDataLen, &pVoiceTalk->m_talkMr);
        }

        if (pVoiceTalk->m_nLocalType & 2)
        {
            //ECHO
            pVoiceTalk->PrepareLocalData((char*)pVoiceTalkCBMsg->pData, pVoiceTalkCBMsg->dwDataLen, pUserData);
        }
    }

    return TRUE;
}

void CDlgAudioTalk::SendAudioData(char* szAudioData, int iAudioLen)
{
    int iDataLen = 0;
    int iDataSendLen = 0;
    int iAllLen = iAudioLen;
    if (m_AudioTalkParam.byAudioType == AUDIOTALKTYPE_G722 || m_AudioTalkParam.byAudioType == AUDIOTALKTYPE_G726)
    {
        iDataLen = AUDDECSIZE;
    }
    else if (m_AudioTalkParam.byAudioType == AUDIOTALKTYPE_G711_A || m_AudioTalkParam.byAudioType == AUDIOTALKTYPE_G711_MU)
    {
        iDataLen = G711_AUDDECSIZE;
    }
    else if (m_AudioTalkParam.byAudioType == AUDIOTALKTYPE_ADPCM)
    {
        iDataLen = ADPCM_AUDDECSIZE;
    }
    else
    {
        iDataLen = iAudioLen;
    }

    do
    {
        if (szAudioData != NULL && iAudioLen != 0)
        {
            SendDataToDevice((char*)szAudioData + iDataSendLen, iDataLen);
            iDataSendLen = iDataSendLen + iDataLen;
            iAllLen = iAllLen - iDataLen;
            if (iAllLen < iDataLen)
            {
                iAllLen = 0;
            }
        }
    } while (iAllLen > 0);

    return;
}

void CDlgAudioTalk::OnBnClickedBtnStartTransmit()
{
    // TODO: Add your control notification handler code here

    m_struVoiceTalkPara.pUser = this;
    m_struVoiceTalkPara.byVoiceTalk = 1;
    m_struVoiceTalkPara.cbVoiceDataCallBack = g_fVoiceDataCallBack;
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();

    m_lVoiceTalkHandle = NET_ECMS_StartVoiceTalk(g_struDeviceInfo[iDeviceIndex].lLoginID, m_cmbVoiceChan.GetCurSel() + 1, &m_struVoiceTalkPara);
    if (m_lVoiceTalkHandle < 0)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_StartVoiceTalk");
        return;
    }

    DWORD dwThreadId;
    m_bExitThread = FALSE;
    m_hVoiceTransmit = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(SendVoiceDataThread),this,0,&dwThreadId);
    GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(FALSE);
}

void CDlgAudioTalk::OnBnClickedBtnStopTransmit()
{
    // TODO: Add your control notification handler code here
    m_bExitThread = TRUE;
    if(WAIT_TIMEOUT == WaitForSingleObject(m_hVoiceTransmit, 3000))
    {
        TerminateThread(m_hVoiceTransmit, 0);
    }
    m_hVoiceTransmit = NULL;

    if (NET_ECMS_StopVoiceTalk(m_lVoiceTalkHandle))
    {
        m_lVoiceTalkHandle = -1;
        GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(TRUE);
    }

    if (g_file != NULL)
    {
        fclose(g_file);
        g_file = NULL;
    }
}



DWORD WINAPI CDlgAudioTalk::SendVoiceDataThread (LPVOID lpArg)
{
    CDlgAudioTalk *pVoiceTalk = (CDlgAudioTalk*)lpArg;

    char szAudioFilePath[MAX_PATH] = {0};
    char szAppPath[MAX_PATH] = {0};
    GetCurrentDirectory(MAX_PATH, szAppPath);                   //获取程序的当前目录
    sprintf(szAudioFilePath, "%s\\AudioFiles\\%s", szAppPath, "ehome_send.au");

    FILE *pFile = fopen(szAudioFilePath, "rb");
    if (pFile == NULL)
    {
        return 0;
    }
    char szBuf[80] = {0};
    int nRemainLen = 0;

    int nCurPos = fseek(pFile,0, SEEK_END);
    nRemainLen = ftell(pFile);
    fseek(pFile, nCurPos, SEEK_SET);

    int nSendTotal = 0;
    while(!pVoiceTalk->m_bExitThread)
    {
        if (nRemainLen >= 80)
        {
            nSendTotal += 80;
            fread(szBuf, 80, 1, pFile);
            //CMS语音对讲               
            if (pVoiceTalk->m_bCMSAudioTalk)
            {
                if (NET_ECMS_SendVoiceTransData(pVoiceTalk->m_lVoiceTalkHandle, szBuf, 80))
                {
                    int error = NET_ECMS_GetLastError();
                    if (error != 0)
                    {
                        error = error;
                    }
                }
            }
            else
            {
                //Stream语音对讲
                NET_EHOME_VOICETALK_DATA struVoicTalkData = {0};
                struVoicTalkData.pSendBuf = (BYTE*)szBuf;
                struVoicTalkData.dwDataLen = 80;
                NET_ESTREAM_SendVoiceTalkData(pVoiceTalk->m_lAudioTalkHandle, &struVoicTalkData);
            }
            nRemainLen -= 80;
        }
        else
        {
            nSendTotal += nRemainLen;
            fread(szBuf, nRemainLen, 1, pFile);
            //CMS语音对讲
            if (pVoiceTalk->m_bCMSAudioTalk)
            {
                NET_ECMS_SendVoiceTransData(pVoiceTalk->m_lVoiceTalkHandle, szBuf, nRemainLen);
            }
            else
            {
                //Stream语音对讲
                NET_EHOME_VOICETALK_DATA struVoicTalkData = {0};
                struVoicTalkData.pSendBuf = (BYTE*)szBuf;
                struVoicTalkData.dwDataLen = nRemainLen;
                NET_ESTREAM_SendVoiceTalkData(pVoiceTalk->m_lAudioTalkHandle, &struVoicTalkData);
            }
            break;
        }
        Sleep(15);
    }

    g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "SendVoiceDataThread, nSendTotal=[%d]", nSendTotal);

    fclose(pFile);
    return 0;
}

void  CDlgAudioTalk::CheckInitParam()
{
    UpdateData(TRUE);
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex < 0)
    {
        char szLan[1024] = {0};
        g_StringLanType(szLan, "请先选择一个设备!", "please select a device first!");
        g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, 0, szLan);
        return;
    }
    m_cmbVoiceChan.ResetContent();
    char szAudioChan[32] = {0};
    m_cmbVoiceChan.Clear();
    for (unsigned int i = 0 ; i < g_struDeviceInfo[iDeviceIndex].dwAudioNum; i++)
    {
        if (i < g_struDeviceInfo[iDeviceIndex].byStartDTalkChan-1)
        {
            sprintf(szAudioChan, "channel %d", i + 1);
            m_cmbVoiceChan.AddString(szAudioChan);
        }
        else
        {
            sprintf(szAudioChan, "IPchannel %d", i + 1);
            m_cmbVoiceChan.AddString(szAudioChan);
        }
    }
    m_cmbVoiceChan.SetCurSel(0);
    UpdateData(FALSE);
}



BOOL CDlgAudioTalk::SendDataToDevice(char *buf, DWORD dwSize)
{
    NET_EHOME_VOICETALK_DATA struVoicTalkData = {0};

    struVoicTalkData.pSendBuf = (BYTE*)buf;
    struVoicTalkData.dwDataLen = dwSize;
    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_ESTREAM_SendVoiceTalkData, dwDataLen=[%d]", dwSize);
    return (NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, &struVoicTalkData/*NULL*/) > 0);
    
    /*
    if (!m_bWaveDeal)
    {
        return FALSE;
    }

    BYTE G711EncBuf[G711_AUDDECSIZE*2]  = {0};
    BYTE G711EncBufA[G711_AUDDECSIZE*2] = {0};
    BYTE G726EncBuf[G726_AUDDECSIZE]    = {0};
    UINT16 wVoiceBuf[G711_AUDDECSIZE*2] = {0};

    memcpy(m_wG711AudioTemp, buf, AUDENCSIZE);
    //AudioBufDownScale((short *)m_wG711AudioTemp, (short *)wVoiceBuf, AUDENCSIZE / 2);
    UINT16 wVoiceBuf1[G711_AUDDECSIZE] = {0};
    UINT16 wVoiceBuf2[G711_AUDDECSIZE] = {0};
    memcpy(wVoiceBuf1, wVoiceBuf, G711_AUDDECSIZE * 2);
    memcpy(wVoiceBuf2, wVoiceBuf + G711_AUDDECSIZE, G711_AUDDECSIZE * 2);

    NET_DVR_EncodeG711Frame(0, (BYTE*)wVoiceBuf1, G711EncBuf);
    NET_DVR_EncodeG711Frame(0, (BYTE*)wVoiceBuf2, G711EncBuf + G711_AUDDECSIZE);
    NET_DVR_EncodeG711Frame(1, (BYTE*)wVoiceBuf1, G711EncBufA);
    NET_DVR_EncodeG711Frame(1, (BYTE*)wVoiceBuf2, G711EncBufA + G711_AUDDECSIZE);

    if (NULL != m_pG726EncM)
    {
        NET_DVR_EncodeG726Frame(m_pG726EncM, (BYTE*)wVoiceBuf, G726EncBuf, m_bReset);
        m_bReset = 0;
    }

    int i = 0, j = 0;

    //encode 
    if (NULL != m_pEncoder)
    {
        NET_DVR_EncodeG722Frame(m_pEncoder, (BYTE*)buf, m_byEncBuf);
    }

    NET_EHOME_VOICETALK_DATA struVoicTalkData = {0};

    if (m_iAudioEncType == AUDIOTALKTYPE_G722)
    {
        struVoicTalkData.pSendBuf = (BYTE*)m_byEncBuf;
        struVoicTalkData.dwDataLen = AUDDECSIZE;

        //if (!NET_DVR_VoiceComSendData(g_struDeviceInfo[i].lVoiceCom[j], (char*)m_byEncBuf, AUDDECSIZE))
        if (NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, &struVoicTalkData) < 0)
        {
            //failed
        }
    }
    else if(m_iAudioEncType == AUDIOTALKTYPE_G711_MU)
    {
        struVoicTalkData.pSendBuf = (BYTE*)G711EncBuf;
        struVoicTalkData.dwDataLen = G711_AUDDECSIZE;

        //if (!NET_DVR_VoiceComSendData(g_struDeviceInfo[i].lVoiceCom[j], (char*)G711EncBuf, G711_AUDDECSIZE))
        if (NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, &struVoicTalkData) < 0)
        {
            //failed
        }

        struVoicTalkData.pSendBuf = (BYTE*)G711EncBuf+G711_AUDDECSIZE;
        struVoicTalkData.dwDataLen = G711_AUDDECSIZE;

        //if (!NET_DVR_VoiceComSendData(g_struDeviceInfo[i].lVoiceCom[j], (char*)G711EncBuf+G711_AUDDECSIZE, G711_AUDDECSIZE))
        if (NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, &struVoicTalkData) < 0)
        {
            //failed
        }
    }
    else if (m_iAudioEncType == AUDIOTALKTYPE_G711_A)
    {
        struVoicTalkData.pSendBuf = (BYTE*)G711EncBufA;
        struVoicTalkData.dwDataLen = G711_AUDDECSIZE;
        //if (!NET_DVR_VoiceComSendData(g_struDeviceInfo[i].lVoiceCom[j], (char*)G711EncBufA, G711_AUDDECSIZE))
        if (NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, &struVoicTalkData) < 0)
        {
            //failed
        }

        struVoicTalkData.pSendBuf = (BYTE*)G711EncBufA+G711_AUDDECSIZE;
        struVoicTalkData.dwDataLen = G711_AUDDECSIZE;
        //if (!NET_DVR_VoiceComSendData(g_struDeviceInfo[i].lVoiceCom[j], (char*)G711EncBufA+G711_AUDDECSIZE, G711_AUDDECSIZE))
        if (NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, &struVoicTalkData) < 0)
        {
            //failed
        }
    }
    else if (m_iAudioEncType == AUDIOTALKTYPE_G726)
    {
        struVoicTalkData.pSendBuf = (BYTE*)G726EncBuf;
        struVoicTalkData.dwDataLen = G726_AUDDECSIZE;

        //if (!NET_DVR_VoiceComSendData(g_struDeviceInfo[i].lVoiceCom[j], (char*)G726EncBuf, G726_AUDDECSIZE))
        if (NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, &struVoicTalkData) < 0)
        {
            //failed
        }
    }

    return TRUE;
    */
}


BOOL CDlgAudioTalk::OpenAudioIn()
{
#if 0
#ifndef WIN64
    //并设置从本机采集音频数据的回调
    m_SoundIn.SetSoundInDataCB(DataFromSoundInEx, (DWORD)this);
    //开启音频输入，
    if (m_SoundIn.Start(&m_struWaveFormat, m_dwBufNum, m_dwBufSize))
    {
        //success
        m_bOpenWavIn = TRUE;
        return TRUE;
    }
    else
    {
        //failed
        return FALSE;
    }
#else
    return FALSE;
#endif
#else
    m_AudioTalkCom.SetSoundInDataCB(DataFromSoundInEx, (void*)this);

    AudioParam stAudioParam = { 1, 16, 16000, 16000, AUDIO_TYPE_PCM };
    ConvertAudioTalkToAudioType(&m_AudioTalkParam, &stAudioParam);
    m_AudioTalkCom.OpenAudioIn(&stAudioParam);

    return TRUE;
#endif
}


BOOL CDlgAudioTalk::OpenAudioOut()
{
#if 0
#ifndef WIN64
    m_ReceiveIndexRV = 0;
    m_rIndexRV = 0;
    m_nBufNo = 0;

    WAVEFORMATEX struWaveFormat = {0};
    DWORD dwBufSize = AUDENCSIZE;
    memcpy(&struWaveFormat, &m_struWaveFormat, sizeof(WAVEFORMATEX));
    //默认格式是按照G722采集播放，如果是其它类型，此处修改采样率等

    //FIXME
    if (m_talkMr.byAudioType != AUDIOTALKTYPE_G722) 
    {
        struWaveFormat.nSamplesPerSec  = SAMPLES_PER_SECOND / 2;
        struWaveFormat.nAvgBytesPerSec = SAMPLES_PER_SECOND * m_struWaveFormat.nBlockAlign/2;
        if (m_talkMr.byAudioType == AUDIOTALKTYPE_G726)
        {
            dwBufSize = G726_AUDENCSIZE; //G726
        }
        else
        {
            dwBufSize = G711_AUDENCSIZE; //G711
        }
    }

    //目前支持的播放为，模拟设备直接采样发送过来的数据（PCM数据）
    if (m_SoundOut.OpenSound(struWaveFormat, m_dwBufNum, dwBufSize, CALLBACK_FUNCTION, 0))
    {
        //m_SoundOut.SetVolume(0x7fff7fff);
        m_SoundOut.SetVolume(0xF000F000);
        if (m_SoundOut.PlaySound())
        {
            m_bOpenWavOut = TRUE;
            return TRUE;
        }
    }
    else
    {
        //failed
    }
    return FALSE;
#else
    return FALSE;
#endif
#else
    AudioParam stAudioParam = { 1, 16, 16000, 16000, AUDIO_TYPE_PCM };
    ConvertAudioTalkToAudioType(&m_AudioTalkParam, &stAudioParam);
    m_AudioTalkCom.OpenAudioOut(&stAudioParam);

    return TRUE;
#endif
}

BOOL CDlgAudioTalk::ConvertAudioTalkToAudioType(AUDIOTALK_PARAM *pAudioTalk, AudioParam *pAudioParam)
{
    if (pAudioTalk == NULL || pAudioParam == NULL)
    {
        return FALSE;
    }
    //
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();

    switch (pAudioTalk->byAudioType)
    {//AUDIOTALKTYPE_G722这个定义好像是以netsdk定义的与EHome的不一样?
    case AUDIOTALKTYPE_G722:
        pAudioParam->nBitRate = BITRATE_ENCODE_16k;
        pAudioParam->nBitWidth = BITS_16;
        pAudioParam->nSampleRate = SAMPLERATE_16K;
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_G722;
        break;
    case AUDIOTALKTYPE_G711_MU:
        //pAudioParam->nBitRate = BITRATE_ENCODE_64k;
        pAudioParam->nBitWidth = BITS_16;
        pAudioParam->nSampleRate = SAMPLERATE_08K;
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_G711U;
        break;
    case AUDIOTALKTYPE_G711_A:
        //pAudioParam->nBitRate = BITRATE_ENCODE_64k;
        pAudioParam->nBitWidth = BITS_16;
        pAudioParam->nSampleRate = SAMPLERATE_08K;
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_G711A;
        break;
    case AUDIOTALKTYPE_MP2L2:
        //pAudioParam->nBitRate = BITRATE_ENCODE_64k;
        pAudioParam->nBitWidth = BITS_16;
        pAudioParam->nSampleRate = SAMPLERATE_16K;
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_MPEG2;
        break;
    case AUDIOTALKTYPE_G726:
        //pAudioParam->nBitRate = BITRATE_ENCODE_64k;
        pAudioParam->nBitWidth = BITS_16;
        pAudioParam->nSampleRate = SAMPLERATE_08K;
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_G726;
        break;
    case AUDIOTALKTYPE_AAC:
        //pAudioParam->nBitRate = BITRATE_ENCODE_64k;
        pAudioParam->nBitWidth = BITS_16;
        pAudioParam->nSampleRate = SAMPLERATE_16K;
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_AAC;
        break;
    case AUDIOTALKTYPE_PCM:
        //pAudioParam->nBitRate = BITRATE_ENCODE_64k;
        pAudioParam->nBitWidth = BITS_16;
        pAudioParam->nSampleRate = SAMPLERATE_16K;
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_PCM;
        break;
    case AUDIOTALKTYPE_MP3:
        if (!GetAudioParam(iDeviceIndex, &pAudioParam->nBitWidth, &pAudioParam->nSampleRate, &pAudioParam->nBitRate))
        {
            pAudioParam->nBitRate = BITRATE_ENCODE_64k;
            pAudioParam->nBitWidth = BITS_16;
            pAudioParam->nSampleRate = SAMPLERATE_16K;
        }
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_MP3;
        break;
    case AUDIOTALKTYPE_ADPCM:
        pAudioParam->nBitWidth = BITS_16;
        pAudioParam->nSampleRate = SAMPLERATE_08K;
        pAudioParam->enAudioEncodeTypeEx = AUDIO_TYPE_ADPCM;
    default:
      //  ERROR("convert audio not support\n");
        break;
    }
    return TRUE;
}

BOOL CDlgAudioTalk::PutIntoBuf(char *lpTemp, int Bytes, LPTALK_MR lpTalkMR)
{

#if 1
    m_AudioTalkCom.InputStreamData(lpTemp, Bytes);
    return TRUE;
#else
    int nTemp = 0;
    int nPacketStart = 0;
    DWORD dwAudDecSize = 0;
    if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G722)
    {
        dwAudDecSize = AUDDECSIZE;
    }
    else if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G726)
    {
        dwAudDecSize = G726_AUDDECSIZE;
    }
    else
    {
        dwAudDecSize = G711_AUDDECSIZE;
    }

    EnterCriticalSection(&m_csAudioBuf);

    /*
    if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G722)
    {
        dwAudDecSize = AUDDECSIZE;
    }
    else if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G726)
    {
        dwAudDecSize = G726_AUDDECSIZE;
    }
    else
    {
        dwAudDecSize = G711_AUDDECSIZE;
    }
    */
    try
    {
        if ((m_ReceiveIndexRV + Bytes) <= AUDIOBUF)
        {
            if (!m_bOpenPlayThread)
            {
                memcpy(m_pRenderBuf+m_ReceiveIndexRV, lpTemp, Bytes);
                m_ReceiveIndexRV += Bytes;
                m_ReceiveIndexRV = m_ReceiveIndexRV % AUDIOBUF;
            }
            else
            {
                if (((m_ReceiveIndexRV + Bytes) >= m_rIndexRV)  
                    && (m_ReceiveIndexRV < m_rIndexRV))
                {   //buffer1 overflow
                    TRACE("buffer1 overflow.");
                    ::SetEvent(m_hHaveRDataRV);
                    nPacketStart = (m_rIndexRV - dwAudDecSize + m_ReceiveIndexRV % dwAudDecSize);
                    if ((nPacketStart + Bytes) <= (DWORD)AUDIOBUF)
                    {
                        memcpy(m_pRenderBuf + nPacketStart, lpTemp, Bytes);
                        m_ReceiveIndexRV = nPacketStart + Bytes;
                    }
                    else
                    {
                        nTemp = AUDIOBUF - nPacketStart;
                        memcpy(m_pRenderBuf + nPacketStart, lpTemp, nTemp);
                        memcpy(m_pRenderBuf, lpTemp + nTemp, Bytes - nTemp);
                        m_ReceiveIndexRV = Bytes - nTemp;
                    }
                }
                else    
                {
                    memcpy(m_pRenderBuf + m_ReceiveIndexRV, lpTemp, Bytes);
                    m_ReceiveIndexRV += Bytes;
                    m_ReceiveIndexRV = m_ReceiveIndexRV % AUDIOBUF;
                }
            }
        }
        else
        {
            if (m_bOpenPlayThread)
            {
                if ((Bytes >= (m_rIndexRV + AUDIOBUF - m_ReceiveIndexRV))
                    || (m_rIndexRV >= m_ReceiveIndexRV))
                {    //buffer2 overflow
                    TRACE("buffer2 overflow");
                    ::SetEvent(m_hHaveRDataRV);
                    if (m_rIndexRV != 0)
                    {
                        nPacketStart = (m_rIndexRV - dwAudDecSize + m_ReceiveIndexRV % dwAudDecSize);
                    }
                    else
                    {
                        nPacketStart = (m_rIndexRV + AUDIOBUF - dwAudDecSize + m_ReceiveIndexRV % dwAudDecSize);
                    }
                    if ((nPacketStart + Bytes) <= (DWORD)AUDIOBUF)
                    {
                        memcpy(m_pRenderBuf + nPacketStart, lpTemp, Bytes);
                        m_ReceiveIndexRV = nPacketStart + Bytes;
                    }
                    else
                    {
                        nTemp = AUDIOBUF - nPacketStart;
                        memcpy(m_pRenderBuf + nPacketStart, lpTemp, nTemp);
                        memcpy(m_pRenderBuf, lpTemp + nTemp, Bytes - nTemp);
                        m_ReceiveIndexRV = Bytes - nTemp;
                    }
                }
                else
                {
                    memcpy(m_pRenderBuf + m_ReceiveIndexRV, lpTemp, nTemp = AUDIOBUF - m_ReceiveIndexRV);
                    memcpy(m_pRenderBuf, lpTemp + nTemp, Bytes - nTemp);
                    m_ReceiveIndexRV = Bytes - nTemp;
                }
            }
        }
    }
    catch(...)
    {
        LeaveCriticalSection(&m_csAudioBuf);
        return FALSE;
    }
    //    TRACE("number:%d, m_nBufNo:%d\n", (m_ReceiveIndexRV[lpTalkMR->byIndex] + AUDIOBUF - m_rIndexRV[lpTalkMR->byIndex]) % (AUDIOBUF), m_nBufNo[lpTalkMR->byIndex]*dwAudDecSize);
    if ((((m_ReceiveIndexRV + AUDIOBUF - m_rIndexRV) % (AUDIOBUF)) >= (m_nBufNo * (int)dwAudDecSize)))
    {
        ::SetEvent(m_hHaveRDataRV);
        m_nBufNo = 1;
        if (!m_bOpenPlayThread)
        {
            m_bOpenPlayThread = TRUE;
            AfxBeginThread(PlayAudioThread, (LPVOID)lpTalkMR);
        }
    }

    LeaveCriticalSection(&m_csAudioBuf);
    return TRUE;
#endif
}

UINT CDlgAudioTalk::PlayAudioThread(LPVOID pParam)
{
    ASSERT(pParam);
    try
    {
        s_pAudioTalkDlg->InputAudioData((LPTALK_MR)pParam);
    }
    catch (...)
    {
        TRACE("Input data exception\n");
        return 1;
    }

    return 0;
}


void CDlgAudioTalk::InputAudioData(LPTALK_MR lpTalkMR)
{
#ifndef WIN64
    HANDLE hWaitEvents[2];

    hWaitEvents[0] = m_hExitEvent;
    hWaitEvents[1] = m_hHaveRDataRV;

    void *pDecoder = NULL;//g722 decoder
    void *pG726Dec = NULL;//g726解码句柄
    void *pG726DecM = NULL;//g726解码模块句柄

    DWORD dwReadLength = 0;
    DWORD dwPlayLength = 0;

    lpTalkMR->byAudioType = AUDIOTALKTYPE_PCM;

//     if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G722)
//     {
//         dwReadLength = AUDDECSIZE;
//         dwPlayLength = AUDENCSIZE;
//         pDecoder = NET_DVR_InitG722Decoder(BIT_RATE_16000);
//     }
//     else if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G726)
//     {
//         dwReadLength = G726_AUDDECSIZE;
//         dwPlayLength = G726_AUDENCSIZE;
//         pG726Dec = NET_DVR_InitG726Decoder(&pG726DecM);
//     }
//     else if (lpTalkMR->byAudioType == AUDIOTALKTYPE_PCM)
//     {
//         dwReadLength = AUDENCSIZE;
//         dwPlayLength = AUDENCSIZE;
//     }
//     else
//     {
//         dwReadLength = G711_AUDDECSIZE;
//         dwPlayLength = G711_AUDENCSIZE;
//     }


    dwReadLength = AUDENCSIZE;
    dwPlayLength = AUDENCSIZE;

    BYTE  *lpTemp = NULL;
    lpTemp = new BYTE[dwReadLength];
    BYTE  *lpPlayBuf = NULL;
    lpPlayBuf = new BYTE[dwPlayLength];
    DWORD dwWaitResult = 0;
    try
    {
        while (1)
        {
            dwWaitResult = WaitForMultipleObjects(2, hWaitEvents, FALSE, INFINITE);
            if (WAIT_OBJECT_0 == dwWaitResult || WAIT_FAILED == dwWaitResult)
            {
                ExitPlayAudio(lpTemp, lpPlayBuf, pDecoder, pG726Dec, pG726DecM, lpTalkMR->byIndex);
                ResetEvent(m_hExitEvent);
                return;
            }

            lpTalkMR->byAudioType = AUDIOTALKTYPE_PCM;

            EnterCriticalSection(&m_csAudioBuf);
            if (CopyAudioData(lpTemp, dwReadLength, lpTalkMR->byIndex))
            {
                /*
                if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G722)
                {
                    if (NULL != pDecoder)
                    {
                        if (!NET_DVR_DecodeG722Frame(pDecoder, (BYTE*)lpTemp, lpPlayBuf))
                        {
                            int i = 0;
                            i++;
                        }
                    }
                }
                else if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G711_MU)
                {
                    NET_DVR_DecodeG711Frame(0,(BYTE*)lpTemp,lpPlayBuf);
                }
                else if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G711_A)
                {
                    NET_DVR_DecodeG711Frame(1,(BYTE*)lpTemp,lpPlayBuf);
                }
                else if (lpTalkMR->byAudioType == AUDIOTALKTYPE_G726)
                {
                    if (NULL != pG726DecM)
                    {
                        NET_DVR_DecodeG726Frame(pG726DecM, (BYTE*)lpTemp, lpPlayBuf, byReset);
                        byReset = 0;
                    }
                }
                else if (lpTalkMR->byAudioType == AUDIOTALKTYPE_PCM)
                {
                    memcpy(lpPlayBuf, lpTemp, dwPlayLength);
                }
                */

                memcpy(lpPlayBuf, lpTemp, dwPlayLength);

                if (!m_SoundOut.InputData((BYTE*)lpPlayBuf, 0))
                {
                    int i = 0;
                    i++;
                }
            }
            else if (m_pRenderBuf == NULL)
            {
                ExitPlayAudio(lpTemp, lpPlayBuf, pDecoder, pG726Dec, pG726DecM, lpTalkMR->byIndex);
                LeaveCriticalSection(&m_csAudioBuf);
                return;
            }

            LeaveCriticalSection(&m_csAudioBuf);
        }
    }
    catch (...)
    {
        TRACE("InputAudioData exception\n");
        ExitPlayAudio(lpTemp, lpPlayBuf, pDecoder, pG726Dec, pG726DecM, lpTalkMR->byIndex);
        return ;
    }

    ExitPlayAudio(lpTemp, lpPlayBuf, pDecoder, pG726Dec, pG726DecM, lpTalkMR->byIndex);
#endif
    return ;
}


void CDlgAudioTalk::ExitPlayAudio(BYTE *lpTemp, BYTE *lpPlayBuf, void *pDecoder, void *pG726Dec, void *pG726DecM, BYTE byIndex)
{
    UN_REFERENCED_PARAMETER(byIndex)
    m_bOpenPlayThread = FALSE;
    if (lpTemp != NULL)
    {
        delete[] lpTemp;
        lpTemp = NULL;
    }
    if (lpPlayBuf != NULL)
    {
        delete[] lpPlayBuf;
        lpPlayBuf = NULL;
    }
    if (pDecoder)
    {
        //FIXME
        //NET_DVR_ReleaseG722Decoder(pDecoder);
        pDecoder = NULL;
    }
    if (pG726Dec)
    {
        //FIXME
        //NET_DVR_ReleaseG726Decoder(pG726Dec);
        pG726Dec = NULL;
        pG726DecM = NULL;
    }
}



BOOL CDlgAudioTalk::CopyAudioData(PBYTE lpTemp, DWORD dwReadLength, int nIndex)
{
    UN_REFERENCED_PARAMETER(nIndex)
    DWORD dwTemp;
    int  dwPacketNumber = 0;
    int itemp = 0;
    __try
    {
        if (m_rIndexRV == m_ReceiveIndexRV)
        {
            dwPacketNumber = 0;
            m_nBufNo = 6;
            ::ResetEvent(m_hHaveRDataRV);
            itemp = 0;
            return FALSE;
        }
        else if (m_rIndexRV < m_ReceiveIndexRV)    //read < write
        {
            if (dwReadLength > (DWORD)(m_ReceiveIndexRV - m_rIndexRV))
            {
                dwPacketNumber = 0;
                m_nBufNo = 6;
                ::ResetEvent(m_hHaveRDataRV);
                itemp = 0;
                return FALSE;
            }
            else
            {
                CopyMemory(lpTemp, m_pRenderBuf + m_rIndexRV, dwReadLength);
                m_rIndexRV += dwReadLength;
                dwPacketNumber = (m_ReceiveIndexRV - m_rIndexRV) / dwReadLength;
                itemp = 1;
            }
        }
        else//read > write
        {
            if (dwReadLength > (DWORD)(AUDIOBUF - m_rIndexRV))
            {
                dwTemp = AUDIOBUF - m_rIndexRV;
                if ((dwReadLength-dwTemp) < (DWORD)(m_ReceiveIndexRV + 1))
                {
                    CopyMemory(lpTemp, m_pRenderBuf + m_rIndexRV, dwTemp);
                    CopyMemory(lpTemp + dwTemp, m_pRenderBuf, dwReadLength - dwTemp);
                    m_rIndexRV = dwReadLength - dwTemp;
                    dwPacketNumber = (m_ReceiveIndexRV - m_rIndexRV) / dwReadLength;
                    itemp = 2;
                }
                else
                {
                    dwPacketNumber = 0;
                    ::ResetEvent(m_hHaveRDataRV);
                    m_nBufNo = 6;
                    itemp = 3;
                    return FALSE;
                }
            }
            else
            {
                itemp = 4;
                CopyMemory(lpTemp, m_pRenderBuf + m_rIndexRV, dwReadLength);
                m_rIndexRV += dwReadLength;
                dwPacketNumber = (AUDIOBUF + m_ReceiveIndexRV - m_rIndexRV) / dwReadLength;
            }
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        return FALSE;
    }
    return TRUE;
}






/////////////////////////////////////////////////////////////////
//                      多路测试专用                       //
/////////////////////////////////////////////////////////////////


BOOL g_bTestExitThread = FALSE;
BOOL  CALLBACK  onTestDeviceAudioTalkConnectCB(LONG lAudioTalkHandle, NET_EHOME_VOICETALK_NEWLINK_CB_INFO *pNewLinkCBMsg, void *pUserData);
DWORD CALLBACK  TestVoiceTransThread(LPVOID lpArg);
BOOL  CALLBACK  onTestVoiceDataCallBack(LONG iAudioTalkHandle, NET_EHOME_VOICETALK_DATA_CB_INFO *pVoiceTalkCBMsg, void *pUserData);

void CDlgAudioTalk::OnBnClickedBtnAudiotalkTest()
{
    // TODO: 在此添加控件通知处理程序代码

   
    NET_EHOME_VOICETALK_DATA struVoicTalkData = {0};

    char szSendBuf[1280];
    struVoicTalkData.pSendBuf = (BYTE*)szSendBuf;
    struVoicTalkData.dwDataLen = 1280;
    //DEBUG
    //return (NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, NULL) > 0);
    /*int nSendRet = */NET_ESTREAM_SendVoiceTalkData(m_lAudioTalkHandle, &struVoicTalkData);

    //int nError = NET_ESTREAM_GetLastError();

    return;
    
    /*
    UpdateData(TRUE);

    CSTRING_TO_CHARS(m_csIP, g_szRemoteIP);
    g_nRemotePort = m_nPort;

    DWORD dwThreadId;

    for (int i = 0; i < MAX_DEVICES; i++)
    {
        LONG lLoginID = g_struDeviceInfo[i].lLoginID;
        if (lLoginID < 0)
        {
            continue;
        }
        CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(TestVoiceTransThread), (void*)lLoginID, 0, &dwThreadId);
    }
    */
}


BOOL CALLBACK onTestDeviceAudioTalkTestConnectCB(LONG lAudioTalkHandle, NET_EHOME_VOICETALK_NEWLINK_CB_INFO *pNewLinkCBMsg, void *pUserData)
{
    UN_REFERENCED_PARAMETER(pNewLinkCBMsg)

    //设置回调
    NET_EHOME_VOICETALK_DATA_CB_PARAM struAudioTalkCBParam = {0};
    struAudioTalkCBParam.fnVoiceTalkDataCB = onTestVoiceDataCallBack;
    struAudioTalkCBParam.pUserData = pUserData;
    if (!NET_ESTREAM_SetVoiceTalkDataCB(lAudioTalkHandle, &struAudioTalkCBParam))
    {
        //设置回调失败
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), 1, OPERATION_FAIL_T, "NET_ESTREAM_SetVoiceTalkDataCB failed!, Handle=[%d]", lAudioTalkHandle);
        return FALSE;
    }
    else
    {
        //设置回调成功
        //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), 1, OPERATION_SUCC_T, "NET_ESTREAM_SetVoiceTalkDataCB success!, Handle=[%d]", lAudioTalkHandle);
        return TRUE;
    }
}

BOOL CALLBACK onTestVoiceDataCallBack(LONG iAudioTalkHandle, NET_EHOME_VOICETALK_DATA_CB_INFO *pVoiceTalkCBMsg, void *pUserData)
{
    //Stream语音对讲回调
    //Echo设备发过来的语音数据
    UN_REFERENCED_PARAMETER(pUserData)
    char* pAudioBuf = new char[pVoiceTalkCBMsg->dwDataLen];
    memcpy(pAudioBuf, pVoiceTalkCBMsg->pData, pVoiceTalkCBMsg->dwDataLen);

    NET_EHOME_VOICETALK_DATA struVoicTalkData = {0};

    struVoicTalkData.pSendBuf = (BYTE*)pAudioBuf;
    struVoicTalkData.dwDataLen = pVoiceTalkCBMsg->dwDataLen;

    if (NET_ESTREAM_SendVoiceTalkData(iAudioTalkHandle, &struVoicTalkData) < 0)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), 1, OPERATION_FAIL_T, "NET_ESTREAM_SendVoiceTalkData Error[%d]!, iAudioTalkHandle=[%d]\n", NET_ESTREAM_GetLastError(), iAudioTalkHandle);
    }

    delete[] pAudioBuf;
    pAudioBuf = NULL;

    return TRUE;
}


DWORD WINAPI TestVoiceTransThread(LPVOID lpArg)
{
    LONG lLoginID = (LONG)lpArg;

    LONG *pHandle = new(::std::nothrow) LONG[1];

    if (NULL == pHandle)
    {
        printf("Alloc MEM failed");
        return NULL;
    }

    *pHandle = -1;

    //先开启监听
    LONG lListenHandle = -1;
    //Stream 开启语音对讲监听
    NET_EHOME_LISTEN_VOICETALK_CFG struListen = {0};
    struListen.fnNewLinkCB = onTestDeviceAudioTalkTestConnectCB;
    struListen.pUser = pHandle;
    strncpy(struListen.struIPAdress.szIP, g_szRemoteIP, 128);
    struListen.struIPAdress.wPort = (WORD)g_nRemotePort + (WORD)lLoginID;     //端口
    lListenHandle = NET_ESTREAM_StartListenVoiceTalk(&struListen);
    if (lListenHandle == -1)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), 1, OPERATION_FAIL_T, "NET_ESTREAM_StartListenVoiceTalk Error!, LoginID[%d]\n", lLoginID);
    }

    else
    {

        //CMS请求 请求对讲
        NET_EHOME_VOICE_TALK_IN struVoiceTalkIn = {0};
        struVoiceTalkIn.dwVoiceChan = 1;
        struVoiceTalkIn.struStreamSever.wPort = (WORD)g_nRemotePort + (WORD)lLoginID;
        memcpy(struVoiceTalkIn.struStreamSever.szIP, g_szRemoteIP, sizeof(struVoiceTalkIn.struStreamSever.szIP));
        NET_EHOME_VOICE_TALK_OUT struVoiceTalkOut = {0};
        if (!NET_ECMS_StartVoiceWithStmServer(lLoginID, &struVoiceTalkIn, &struVoiceTalkOut))
        {
            //请求对讲失败
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), 1, OPERATION_FAIL_T, "NET_ECMS_StartVoiceWithStmServer Error!, LoginID[%d], lSessionID[%d]\n", lLoginID, struVoiceTalkOut.lSessionID);
        }

        //CMS 请求开始推流
        if (!g_pMainDlg->m_bAsyncCalllback)
        {
            NET_EHOME_PUSHVOICE_IN struPushVoiceIn = { 0 };
            struPushVoiceIn.dwSize = sizeof(struPushVoiceIn);
            struPushVoiceIn.lSessionID = struVoiceTalkOut.lSessionID;
            NET_EHOME_PUSHVOICE_OUT struPushVoiceOut = { 0 };
            struPushVoiceOut.dwSize = sizeof(struPushVoiceOut);
            if (!NET_ECMS_StartPushVoiceStream(lLoginID, &struPushVoiceIn, &struPushVoiceOut))
            {
                g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), 1, OPERATION_FAIL_T, "NET_ECMS_StartPushVoiceStream Error!, LoginID[%d], lSessionID[%d]\n", lLoginID, struVoiceTalkOut.lSessionID);
            }
        }
    }

    if (pHandle != NULL)
    {
        delete[] pHandle;
        pHandle = NULL;
    }

    return 0;
}

void CDlgAudioTalk::OnBnClickedBtnVedioIntercome()
{
    // TODO:  在此添加控件通知处理程序代码
    if (g_pMainDlg->m_pDlgVI == NULL)
    {
        return;
    }

    // 设置当前活动对话框
    g_pMainDlg->m_pDlgVI->ShowWindow(SW_SHOW);
    g_pMainDlg->m_pDlgVI->m_byRequestType = 0;
    g_pMainDlg->m_pDlgVI->m_iDevIndex = m_iCurDevIndex;
}


void CDlgAudioTalk::OnBnClickedBtnStartVoicetalk2()
{
    // TODO:  在此添加控件通知处理程序代码
    // TODO: Add your control notification handler code here
    m_struVoiceTalkPara.bNeedCBNoEncData = m_combCBDataType.GetCurSel();
    m_struVoiceTalkPara.pUser = this;
    m_struVoiceTalkPara.byVoiceTalk = 2;
    m_struVoiceTalkPara.cbVoiceDataCallBack = g_fVoiceDataCallBack;

    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();

    m_lVoiceTalkHandle = NET_ECMS_StartVoiceTalk(g_struDeviceInfo[iDeviceIndex].lLoginID, m_cmbVoiceChan.GetCurSel() + 1, &m_struVoiceTalkPara);
    if (m_lVoiceTalkHandle < 0)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 1, "NET_ECMS_StartVoiceTalk");
        return;
    }

    GetDlgItem(IDC_BTN_START_VOICETALK2)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(FALSE);
}


void CDlgAudioTalk::OnBnClickedBtnStopVoicetalk3()
{
    // TODO:  在此添加控件通知处理程序代码
    if (NET_ECMS_StopVoiceTalk(m_lVoiceTalkHandle))
    {
        m_lVoiceTalkHandle = -1;
        GetDlgItem(IDC_BTN_START_VOICETALK2)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_START_VOICETALK)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_START_TRANSMIT)->EnableWindow(TRUE);
    }

    ChangeAudioCtlState(g_pMainDlg->GetCurDeviceIndex());
}

BOOL CDlgAudioTalk::GetAudioType(int& iAudioType)
{
    //获取设备信息
    NET_EHOME_DEVICE_INFO struDevInfo = { 0 };
    struDevInfo.dwSize = sizeof(NET_EHOME_DEVICE_INFO);
    NET_EHOME_CONFIG struCfg = { 0 };
    struCfg.pOutBuf = &struDevInfo;
    struCfg.dwOutSize = sizeof(NET_EHOME_DEVICE_INFO);

    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();

    if (g_struDeviceInfo[iDeviceIndex].dwVersion >= 5)
    {
        iAudioType = -1;
        if (GetAudioParamWithChan(iDeviceIndex, m_cmbVoiceChan.GetCurSel() + 1, iAudioType))
        {
            return TRUE;
        }
        else //不支持ISPAI协议获取通道走老的方式
        {
            if (!NET_ECMS_GetDevConfig(g_struDeviceInfo[iDeviceIndex].lLoginID, NET_EHOME_GET_DEVICE_INFO, &struCfg, sizeof(NET_EHOME_CONFIG)))
            {
                g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, 1, "NET_EHOME_GET_DEVICE_INFO");
            }
            else
            {
                g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_EHOME_GET_DEVICE_INFO, dwAudioEncType %d", struDevInfo.dwAudioEncType);
                iAudioType = struDevInfo.dwAudioEncType;
                return TRUE;
            }
        }
    }
    else
    {
        if (!NET_ECMS_GetDevConfig(g_struDeviceInfo[iDeviceIndex].lLoginID, NET_EHOME_GET_DEVICE_INFO, &struCfg, sizeof(NET_EHOME_CONFIG)))
        {
            g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, 1, "NET_EHOME_GET_DEVICE_INFO");
        }
        else
        {
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 1, "NET_EHOME_GET_DEVICE_INFO, dwAudioEncType %d", struDevInfo.dwAudioEncType);
            iAudioType = struDevInfo.dwAudioEncType;
            return TRUE;
        }
    }
    return FALSE;
}

int CDlgAudioTalk::AudidTyeStringToInt(const string strAudioTye)
{
    //0 - OggVorbis，1 - G711U，2 - G711A，3 - G726，4 - AAC，5 - MP2L2，6 - PCM，7 - MP3，8 - G723，9 - MP1L2，10 - ADPCM
    if (strAudioTye.length() <= 0)
    {
        return -1;
    }

    if (strAudioTye == "G.722.1")
    {
        return AUDIOTALKTYPE_G722;
    }
    else if(strAudioTye == "G.711alaw")
    {
        return AUDIOTALKTYPE_G711_A;
    }
    else if (strAudioTye == "G.711ulaw")
    {
        return AUDIOTALKTYPE_G711_MU;
    }
    else if (strAudioTye == "G.726")
    {
        return AUDIOTALKTYPE_G726;
    }
    else if (strAudioTye == "AAC")
    {
        return AUDIOTALKTYPE_AAC;
    }
    else if (strAudioTye == "MP2L2")
    {
        return AUDIOTALKTYPE_MP2L2;
    }
    else if (strAudioTye == "PCM")
    {
        return AUDIOTALKTYPE_PCM;
    }
    else if (strAudioTye == "MP3")
    {
        return AUDIOTALKTYPE_MP3;
    }
    else if (strAudioTye == "G723")
    {
        return AUDIOTALKTYPE_G723;
    }
    else if (strAudioTye == "MP1L2")
    {
        return AUDIOTALKTYPE_MP1L2;
    }
    else if (strAudioTye == "ADPCM")
    {
        return AUDIOTALKTYPE_ADPCM;
    }
    else
    {
        return -1;
    }
}

BOOL  CDlgAudioTalk::GetAudioParamWithChan(int iDeviceIndex, int iChannel, int& iAudioType)
{
    NET_EHOME_PTXML_PARAM struPTXML = { 0 };
    int iAudioBitRate = 0;
    int iAudioSamplingRate = 0;
    //先通过透传获取一下设备当前的采样赫兹，编码比特率
    int m_lUserID = g_struDeviceInfo[iDeviceIndex].lLoginID;

    string csTemp= "/ISAPI/System/TwoWayAudio/channels/" + to_string(iChannel);
    CString csURL = csTemp.c_str();
    struPTXML.pRequestUrl = csURL.GetBuffer(0);
    struPTXML.dwRequestUrlLen = csURL.GetLength();

    char *sInput = new char[512 * 1024];
    char *sCond = new char[512 * 1024];
    char *sOutput = new char[512 * 1024];

    memset(sInput, 0, 512 * 1024);
    memset(sCond, 0, 512 * 1024);
    memset(sOutput, 0, 512 * 1024);

    struPTXML.pCondBuffer = sCond;
    struPTXML.dwCondSize = strlen(sCond);
    struPTXML.dwInSize = strlen(sInput);

    struPTXML.pOutBuffer = sOutput;
    struPTXML.dwOutSize = 512 * 1024;
    struPTXML.dwRecvTimeOut = 5000;

    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)struPTXML.pInBuffer, (char*)struPTXML.pInBuffer, struPTXML.dwInSize, &struPTXML.dwInSize);
    A2UTF8((char*)struPTXML.pCondBuffer, (char*)struPTXML.pCondBuffer, struPTXML.dwCondSize, &struPTXML.dwCondSize);

    if (NET_ECMS_GetPTXMLConfig(m_lUserID, &struPTXML))
    {
        // 解析设备返回的XML数据
        CXmlBase xmlBase;
        xmlBase.Parse((char *)struPTXML.pOutBuffer);
        if (xmlBase.FindElem("TwoWayAudioChannel") && xmlBase.IntoElem())
        {
            if (xmlBase.FindElem("audioCompressionType"))
            {
                string strType = xmlBase.GetData();
                iAudioType = AudidTyeStringToInt(strType);
                if (sInput != NULL)
                {
                    delete[] sInput;
                }
                if (sCond != NULL)
                {
                    delete[] sCond;
                }
                if (sOutput != NULL)
                {
                    delete[] sOutput;
                }
                return TRUE;
            }
            else
            {
                g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 1, "no audioCompressionType");
            }
        }
        else
        {
            g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 1, "parse TwoWayAudioChannel error!");
        }
    }
    else
    {
        g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_GetPTXMLConfig");
    }

    if (sInput != NULL)
    {
        delete[] sInput;
    }
    if (sCond != NULL)
    {
        delete[] sCond;
    }
    if (sOutput != NULL)
    {
        delete[] sOutput;
    }
    return FALSE;
}


BOOL  CDlgAudioTalk::GetAudioParam(int iDeviceIndex, unsigned short *wBitWidth, unsigned int* iSampleRate, unsigned int *iBitRate)
{
    NET_EHOME_PTXML_PARAM struPTXML = { 0 };
    int iAudioBitRate = 0;
    int iAudioSamplingRate = 0;
    //先通过透传获取一下设备当前的采样赫兹，编码比特率
    int m_lUserID = g_struDeviceInfo[iDeviceIndex].lLoginID;

    CString csURL = "/ISAPI/System/TwoWayAudio/channels/1";
    struPTXML.pRequestUrl = csURL.GetBuffer(0);
    struPTXML.dwRequestUrlLen = csURL.GetLength();

    char *sInput = new char[512 * 1024];
    char *sCond = new char[512 * 1024];
    char *sOutput = new char[512 * 1024];

    memset(sInput, 0, 512 * 1024);
    memset(sCond, 0, 512 * 1024);
    memset(sOutput, 0, 512 * 1024);

    struPTXML.pCondBuffer = sCond;
    struPTXML.dwCondSize = strlen(sCond);
    struPTXML.dwInSize = strlen(sInput);

    struPTXML.pOutBuffer = sOutput;
    struPTXML.dwOutSize = 512 * 1024;
    struPTXML.dwRecvTimeOut = 5000;

    //需要将字符串字段转换成UTF-8
    A2UTF8((char*)struPTXML.pInBuffer, (char*)struPTXML.pInBuffer, struPTXML.dwInSize, &struPTXML.dwInSize);
    A2UTF8((char*)struPTXML.pCondBuffer, (char*)struPTXML.pCondBuffer, struPTXML.dwCondSize, &struPTXML.dwCondSize);

    if (NET_ECMS_GetPTXMLConfig(m_lUserID, &struPTXML))
    {
        // 解析设备返回的XML数据
        CXmlBase xmlBase;
        xmlBase.Parse((char *)struPTXML.pOutBuffer);

        xmlBase.FindElem("TwoWayAudioChannel") && xmlBase.IntoElem();
        ConvertSingleNodeData(&iAudioBitRate, xmlBase, "audioBitRate", NODE_STRING_TO_INT);
        ConvertSingleNodeData(&iAudioSamplingRate, xmlBase, "audioSamplingRate", NODE_STRING_TO_INT);
        *wBitWidth = BITS_16;
        *iSampleRate = iAudioSamplingRate * 1000;
        *iBitRate = iAudioBitRate * 1000;
    }
    else
    {
        g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_GetPTXMLConfig");
        delete[] sInput;
        delete[] sCond;
        delete[] sOutput;
        return FALSE;
    }
    if (sInput != NULL)
    {
        delete[] sInput;
    }
    if (sCond != NULL)
    {
        delete[] sCond;
    }
    if (sOutput != NULL)
    {
        delete[] sOutput;
    }
    return TRUE;
}
