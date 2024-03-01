// DlgOutput.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgOutput.h"

// CDlgOutput dialog

IMPLEMENT_DYNAMIC(CDlgOutput, CDialog)


CDlgOutput::CDlgOutput(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgOutput::IDD, pParent)
    , m_hWnd(NULL)
    , m_bPlay(FALSE)
    , m_lPort(-1)
    , m_iDeviceIndex(-1)
    , m_iChanIndex(-1)
    , m_iWndIndex(-1)
    , m_lPlayHandle(-1)
    , m_iSessionID(-1)
    , m_bRecord(FALSE)
    , m_fVideoFile(NULL)
    , m_pFatherDlg(NULL)
{
    m_bSound = FALSE;
    m_bSoundEnabled = FALSE;
    memset(m_szHead, 0, sizeof(m_szHead));
}

CDlgOutput::~CDlgOutput()
{
    CloseHandle(m_hPlayEvent);
    //StopPlay();
}

void CDlgOutput::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOutput, CDialog)
//    ON_WM_NCLBUTTONDOWN()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CDlgOutput message handlers


/*************************************************
Function:     DrawOutputBorder    
Description: draw preview window output border
Input:         none
Output:      none
Return:         none
*************************************************/
void CDlgOutput::DrawOutputBorder(void)
{
    if (!IsWindowVisible())
    {
        return;
    }

    CPen *pOldPen = NULL;
    CPen pPen;
    CRect rc(0,0,0,0);
    GetWindowRect(&rc);
    g_pMainDlg->GetDlgItem(IDC_STATIC_PREVIEWBG)->ScreenToClient(&rc);   
    if (g_pMainDlg->m_iCurWndIndex == m_iWndIndex)
    {
        pPen.CreatePen(PS_SOLID, 2, RGB(255,0,0));
    }
    else
    {
        pPen.CreatePen(PS_SOLID, 2, RGB(125, 125, 116));  
    }
    //     rc.top-=1;
    //     rc.left-=1;
    rc.right += OUTPUT_INTERVAL/2;
    rc.bottom += OUTPUT_INTERVAL/2;

    CDC *pDC = g_pMainDlg->GetDlgItem(IDC_STATIC_PREVIEWBG)->GetDC();   
    ASSERT(pDC);

    pDC->SelectStockObject(NULL_BRUSH);
    pOldPen = pDC->SelectObject(&pPen);
    pDC->Rectangle(&rc);
    
    if (pOldPen)
    {
        pDC->SelectObject(pOldPen);
    }

    ReleaseDC(pDC);
}
BOOL CDlgOutput::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_hWnd = GetSafeHwnd();

    m_hPlayEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgOutput::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (g_pMainDlg->m_iCurWndIndex != m_iWndIndex)
    {
        char szLan[128] = { 0 };
        CString csTemp = _T("");
        g_pMainDlg->AddLog(m_iWndIndex, OPERATION_SUCC_T, 0, "win change");
        m_pFatherDlg->m_iCurWndIndex = m_iWndIndex;
        if (g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_lPlayHandle >= 0)
        {
            if (g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_bRecord == TRUE)
            {
               // g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_bRecord = TRUE;
               // m_pFatherDlg->m_bRecord = TRUE;
                g_StringLanType(szLan, "停止录像", "Stop Record");
                csTemp = szLan;
                m_pFatherDlg->GetDlgItem(IDC_BTN_SALVE)->SetWindowText(csTemp);
            }
            else
            {
               // g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_bRecord = FALSE;
               // m_pFatherDlg->m_bRecord = FALSE;
               // fclose(g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_fVideoFile);
               // g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_fVideoFile = NULL;
                g_StringLanType(szLan, "开始录像", "Start Record");
                csTemp = szLan;
                m_pFatherDlg->GetDlgItem(IDC_BTN_SALVE)->SetWindowText(csTemp);
            }
        }
        else
        {
            g_StringLanType(szLan, "开始录像", "Start Record");
            csTemp = szLan;
            m_pFatherDlg->GetDlgItem(IDC_BTN_SALVE)->SetWindowText(csTemp);
        }
    }
    g_pMainDlg->m_iCurWndIndex = m_iWndIndex;

    //update current window video parameters
    //g_pMainDlg->m_dlgOutputCtrl->UpdateVideoEff();
    //
    //line around output window
    for (int i = 0;i < MAX_WIN_NUM;i++)
    {
        g_pMainDlg->m_pDlgPreview[i].DrawOutputBorder();
    }

    CDialog::OnLButtonDown(nFlags, point);
}

BOOL  CDlgOutput::SetPlayParam(LONG lPlayHandle, int iCurWndIndex, int iSession)
{
    UN_REFERENCED_PARAMETER(iSession)
    UN_REFERENCED_PARAMETER(iCurWndIndex)
    if (m_bPlay)
    {
        return FALSE;
    }
    if ((m_lPlayHandle == lPlayHandle) /*|| (m_iWndIndex == iCurWndIndex) || (m_iSessionID == iSession)*/)
    {
        //崩了吧，问题好查点
        char *p = NULL;
        *p = 1;
    }
    m_lPlayHandle = lPlayHandle;
    //m_iWndIndex = iCurWndIndex;
    //m_iSessionID = iSession;

    return TRUE;
}

BOOL  CDlgOutput::InputStreamData(BYTE byDataType, char* pBuffer, int iDataLen)
{
    if (m_bRecord)
    {
        if (m_fVideoFile == NULL)
        {
            char szFile[256] = { 0 };
            sprintf(szFile, "C:\\EhomeRecord\\%s_%4d-%02d-%02d_%02d_%02d_%02d_%d.mp4", g_struDeviceInfo[m_iDeviceIndex].byDeviceID,\
                m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(), m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond(), \
                m_iChanIndex+1);
            m_fVideoFile = fopen(szFile, "wb");

            if (NULL != m_fVideoFile && strlen(m_szHead) > 0) //写40字节海康头
            {
                fwrite(m_szHead, 40, 1, m_fVideoFile);
            }

        }
        if (NULL != m_fVideoFile)
        {
            if (g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].dwStreamFormat == 1) //RTP
            {
                fwrite(&iDataLen, 4, 1, m_fVideoFile);
            }
            fwrite(pBuffer, iDataLen, 1, m_fVideoFile);
        }
    }

   
    if(1 == byDataType)
    {
        if ( !PlayM4_GetPort(&m_lPort))
        {
            StopPlay();
            return FALSE;
        }
        if (!PlayM4_SetStreamOpenMode(m_lPort, STREAME_REALTIME/*实时流*/ ))
        {
            StopPlay();
            return FALSE;
        }
        // 先输入头,前40个字节
        if(!PlayM4_OpenStream(m_lPort, (unsigned char *)pBuffer, (DWORD)iDataLen, 2*1024*1024/*缓冲区*/ ))
        {
            StopPlay();
            return FALSE; 
        }
        //设置解码后缓存区1-实时性好；3-实时性较好；6-实时性中，流畅性中；15-流畅性好
        PlayM4_SetDisplayBuf(m_lPort, 6);

        //设置是否纯RTP音频数据设置音频显示线程模式 true：音频线程分离模式； false： 音视频显示在同一线程模式
        PlayM4_SetAudioThreadModel(m_lPort, g_pMainDlg->m_bRtpAudio);

        if(!PlayM4_Play(m_lPort, m_hWnd ))
        {
            StopPlay();
            return FALSE;
        }
        if (iDataLen == 40)
        {
            memcpy(m_szHead, pBuffer, iDataLen);
        }
        //fileTest.Open("d:/streamtest.mp4", CFile::modeCreate | CFile::modeWrite);
        //char szData[4] = { 0 }; //规定使用两4个字节表示长度
        //DWORD iLen = iDataLen;
        //DWORD i = 0;

        //while (i < 4 && iLen != 0)
        //{
        //    szData[i] = iLen % 256;
        //    iLen = iLen / 256;
        //    i++;
        //}

        //fileTest.Write(&szData, 4);
        //fileTest.Write(pBuffer, iDataLen);
    }
    else
    {
        if (m_bSoundEnabled)
        {
            if (!m_bSound)
            {
                if (!PlayM4_PlaySound(m_lPort))
                {
                   int iErrNO = PlayM4_GetLastError(m_lPort);
                   g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, "PlayM4_PlaySound err[%d] m_hWnd[%d]m_lPort[%d]", iErrNO, m_hWnd, m_lPort);
                   //开失败也置为true，否则会一直开，由于该接口是全局的，一直开会影响到其他通道。
                   m_bSound = TRUE;
                }
                else
                {
                    m_bSound = TRUE;
                }

            }
        }
        else
        {
            if (m_bSound)
            {
                PlayM4_StopSound();
                m_bSound = FALSE;
            }
        }
        int time = 1000;
        while( time > 0 )
        {
            //char szData[4] = { 0 };
            //DWORD iLen = iDataLen;
            //DWORD i = 0;

            //while (i < 4 && iLen != 0)
            //{
            //    szData[i] = iLen % 256;
            //    iLen = iLen / 256;
            //    i++;
            //}

            //fileTest.Write(&szData, 4);
            //fileTest.Write(pBuffer, iDataLen);
           
            BOOL bRet = PlayM4_InputData(m_lPort, (unsigned char *)pBuffer, (DWORD)iDataLen);

            if(!bRet)
            {
               // g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "PlayM4_InputData failed");
                Sleep(5);
                time --;
                continue;
            }
            break;
        }
//         if(!PlayM4_InputData(m_lPort, (unsigned char *)pBuffer, (DWORD)iDataLen))
//         {
//             g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "PlayM4_InputData failed");
//         }
    }// End of while( TRUE )
    return TRUE;
}

BOOL  CDlgOutput::CleanPlayParam()
{
//    g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iPlayWndIndex = -1;
//     m_iDeviceIndex = -1;
//     m_iChanIndex = -1;

    m_lPlayHandle = -1;
    m_iSessionID = -1;
    //m_iWndIndex = -1;
    return TRUE;
}


BOOL CDlgOutput::StartPlay(LPSTRU_CHANNEL_INFO pChanInfo, HTREEITEM hChanItem)
{
    memset(m_szHead, 0, sizeof(m_szHead));
    pChanInfo->dwPreviewSessionId = 0;
    // TODO: Add your message handler code here and/or call default
    if (pChanInfo->iDeviceIndex<0 || pChanInfo->iChanIndex<0)
    {
        return FALSE;
    }
    m_iDeviceIndex = pChanInfo->iDeviceIndex;
    m_iChanIndex   = pChanInfo->iChanIndex;
    LONG lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;

/*
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char sCmd[32] = "GETDEVICECONFIG";
    char sInBuf[512] = "<Params>\r\n<ConfigCmd>GetDevAbility</ConfigCmd>\r\n<ConfigParam1>1 </ConfigParam1>\r\n\
                                                      <ConfigParam2></ConfigParam2>\r\n<ConfigParam3></ConfigParam3>\r\n<ConfigParam4></ConfigParam4>\r\n</Params>";
    char sOutBuf[1024] = { 0 };
    char sStatusBuf[1024] = { 0 };
    struXmlCfg.dwCmdLen = strlen(sCmd);
    struXmlCfg.pCmdBuf = sCmd;
    struXmlCfg.pInBuf = sInBuf;
    struXmlCfg.pOutBuf = sOutBuf;
    struXmlCfg.pStatusBuf = sStatusBuf;
    struXmlCfg.dwInSize = 512;
    struXmlCfg.dwOutSize = 1024;
    struXmlCfg.dwStatusSize = 1024;

    if (!NET_ECMS_XMLConfig(lUserID, &struXmlCfg, sizeof(NET_EHOME_XML_CFG)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "GetDevAbility failed");
    }*/

    if(m_bPlay)
    {
        if(!StopPlay())
        {
            //AfxMessageBox("停止预览失败");
            return FALSE;
        }
    }

    m_hChanItem = hChanItem;

    NET_EHOME_PREVIEWINFO_OUT struParamOut = { 0 };

    //如果是零通道
    if (g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChanType == DEMO_CHANNEL_TYPE_ZERO)
    {
        NET_EHOME_PREVIEWINFO_IN_V11 struParamIn = { 0 };

        struParamIn.byDelayPreview = 1;//是否延时取流

        struParamIn.dwLinkMode = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].dwLinkMode;
        struParamIn.struStreamSever.wPort = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].struIP.wPort;
        memcpy(struParamIn.struStreamSever.szIP, g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].struIP.szIP, 128);

        struParamIn.dwStreamType = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].dwStreamType;
        struParamIn.iChannel = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChannelNO; //零通道通道号
        struParamIn.byEncrypt = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].byEncrypt;

        if (!NET_ECMS_StartGetRealStreamV11(lUserID, &struParamIn, &struParamOut))
        {
            m_hChanItem = NULL;
            //memset(&m_struChanInfo, 0, sizeof(STRU_CHANNEL_INFO));
            CleanPlayParam();
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartGetRealStreamV11 failed");
            return FALSE;
            //MessageBox("NET_ECMS_StartGetRealStream failed");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartGetRealStreamV11 SUCC");
        }

        if (g_pMainDlg->m_bAsyncCalllback)
        {
            LONG lSessionID = -1;
            while (!g_pMainDlg->PopSessionByHandle(struParamOut.lHandle, &lSessionID))
            {
                Sleep(100);
            }
            pChanInfo->dwPreviewSessionId = lSessionID;
        }
        else
        {
            pChanInfo->dwPreviewSessionId = struParamOut.lSessionID;
        }
    }
    else
    {
        NET_EHOME_PREVIEWINFO_IN_V11 struParamIn = { 0 };

        struParamIn.dwLinkMode = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].dwLinkMode;
        struParamIn.struStreamSever.wPort = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].struIP.wPort;
        memcpy(struParamIn.struStreamSever.szIP, g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].struIP.szIP, 128);

        struParamIn.dwStreamType = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].dwStreamType;
        struParamIn.iChannel = m_iChanIndex + 1;

        struParamIn.byEncrypt = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].byEncrypt;
        if (!NET_ECMS_StartGetRealStreamV11(lUserID, &struParamIn, &struParamOut))
        {
            m_hChanItem = NULL;
            //memset(&m_struChanInfo, 0, sizeof(STRU_CHANNEL_INFO));
            CleanPlayParam();
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartGetRealStream failed");
            return FALSE;
            //MessageBox("NET_ECMS_StartGetRealStream failed");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartGetRealStream SUCC");
        }

//         if (g_pMainDlg->m_bAsyncCalllback)
//         {
//             LONG lSessionID = -1;
//             while (!g_pMainDlg->PopSessionByHandle(struParamOut.lHandle, &lSessionID))
//             {
//                 Sleep(100);
//             }
//             pChanInfo->dwPreviewSessionId = lSessionID;
//         }
//         else
        {
            pChanInfo->dwPreviewSessionId = struParamOut.lSessionID;
        }
        
    }

    if (!g_pMainDlg->m_bAsyncCalllback)
    {
        Sleep(1000);

        NET_EHOME_PUSHSTREAM_IN struPushIn = { 0 };
        NET_EHOME_PUSHSTREAM_OUT struPushOut = { 0 };
        struPushIn.dwSize = sizeof(struPushIn);
        struPushIn.lSessionID = pChanInfo->dwPreviewSessionId;
        struPushOut.dwSize = sizeof(struPushOut);
        if (!NET_ECMS_StartPushRealStream(lUserID, &struPushIn, &struPushOut))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartPushStream failed");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartPushStream SUCC");
        }


        if (WAIT_OBJECT_0 != WaitForSingleObject(m_hPlayEvent, 5000))
        {
            //没有等到预览请求
            //memset(&m_struChanInfo, 0, sizeof(STRU_CHANNEL_INFO));
            CleanPlayParam();
            return FALSE;
        }
        ResetEvent(m_hPlayEvent);

//         if (g_pMainDlg->m_bAsyncCalllback)
//         {
//             while (!g_pMainDlg->PopSessionByHandle(struPushOut.lHandle))
//             {
//                 Sleep(100);
//             }
//         }


        m_iSessionID = struParamOut.lSessionID;
        m_bPlay = TRUE;

        ::PostMessage(g_pMainDlg->m_hWnd, WM_CHANGE_CHANNEL_ITEM_IMAGE, WPARAM(m_iDeviceIndex), LPARAM(m_iChanIndex));
        return TRUE;
    }

    return TRUE;

}

BOOL CDlgOutput::StopPlay()
{
    // TODO: Add your message handler code here and/or call default
    BOOL bRet = true;
    LONG lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;

//     if (!m_bPlay)
//     {
//         return TRUE;
//     }


    //if(!NET_ECMS_StopGetRealStream(lUserID, m_iSessionID))
    if (!NET_ESTREAM_StopPreview(m_lPlayHandle))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 2, "silujie:NET_ESTREAM_StopPreview[%d]", m_lPlayHandle);
        bRet = false;
        //return FALSE;
    }

    NET_EHOME_STOPSTREAM_PARAM struStopParam = { 0 };
    struStopParam.lSessionID = m_iSessionID;
    if (!NET_ECMS_StopGetRealStreamEx(lUserID, &struStopParam))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StopGetRealStreamEx[%d]", m_iSessionID);
        bRet = false;
        //return FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StopGetRealStreamEx, Session[%d], Handle[%d]", m_iSessionID, struStopParam.lHandle);
    
        if (g_pMainDlg->m_bAsyncCalllback)
        {
            LONG lSessionID = -1;
            while (!g_pMainDlg->PopSessionByHandle(struStopParam.lHandle, &lSessionID))
            {
                Sleep(100);
            }
        }
    }

    ::PostMessage(g_pMainDlg->m_hWnd, WM_CHANGE_CHANNEL_ITEM_IMAGE, WPARAM(m_iDeviceIndex), LPARAM(m_iChanIndex));

    CleanPlayParam();

    if (m_lPort >= 0)
    {
        
        //fileTest.Close();
        if (!PlayM4_Stop(m_lPort))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, "PlayM4_Stop err[%d]", PlayM4_GetLastError(m_lPort));
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 0, "PlayM4_Stop");
        }
        if (!PlayM4_CloseStream(m_lPort))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, "PlayM4_CloseStream err[%d]", PlayM4_GetLastError(m_lPort));
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 0, "PlayM4_CloseStream");
        }
        if (!PlayM4_FreePort(m_lPort))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, "PlayM4_FreePort err[%d]", PlayM4_GetLastError(m_lPort));
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 0, "PlayM4_FreePort");
        }

        m_lPort = -1;
    }

    m_bPlay = FALSE;
    g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].bPlay = false;
    DrawOutputBorder();

    Invalidate(TRUE);

    if (g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_bRecord == TRUE)
    {
        g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_bRecord = FALSE;
        m_pFatherDlg->m_bRecord = FALSE;
        fclose(g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_fVideoFile);
        g_pMainDlg->m_pDlgPreview[m_iWndIndex].m_fVideoFile = NULL;
        char szLan[128] = { 0 };
        CString csTemp = _T("");
        g_StringLanType(szLan, "开始录像", "Start Record");
        csTemp = szLan;
        m_pFatherDlg->GetDlgItem(IDC_BTN_SALVE)->SetWindowText(csTemp);
    }
    return bRet;
}

void CDlgOutput::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    //can be enlarged when play
    UN_REFERENCED_PARAMETER(nFlags);
    UN_REFERENCED_PARAMETER(point);
    if (m_lPlayHandle < 0 && !g_struLocalParam.bEnlarged)
    {
        return;
    }
    if (g_pMainDlg->m_iCurWndIndex != m_iWndIndex)
    {
        g_pMainDlg->m_iCurWndIndex = m_iWndIndex;
    }
    //g_pMainDlg->ArrangeOutputs(g_pMainDlg->m_iCurWndNum);
    if (g_struLocalParam.bFullScrn)
    {
        g_struLocalParam.bEnlarged = FALSE;
        g_struLocalParam.bFullScrn = FALSE;
        g_pMainDlg->GetDlgItem(IDC_COMBO_WNDNUM)->EnableWindow(TRUE);
        g_pMainDlg->FullScreen(g_struLocalParam.bFullScrn);
        if (g_struLocalParam.bFullScrn)
        {
            g_pMainDlg->ArrangeOutputs(g_pMainDlg->m_iCurWndNum);
        }
        else//muti-screen
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 0, "multi screen display");
            g_pMainDlg->ArrangeOutputs(g_pMainDlg->m_iCurWndNum);
            g_pMainDlg->GetDlgItem(IDC_STATIC_PREVIEWBG)->Invalidate(TRUE);
        }
        return;
    }
    g_pMainDlg->GetDlgItem(IDC_COMBO_WNDNUM)->EnableWindow(FALSE);

    if (g_struLocalParam.bEnlarged || 1 == g_pMainDlg->m_iCurWndNum)
    {
        g_struLocalParam.bFullScrn = TRUE;

        //brush chosen line
        g_pMainDlg->GetDlgItem(IDC_STATIC_PREVIEWBG)->Invalidate(TRUE);
        g_pMainDlg->FullScreen(g_struLocalParam.bFullScrn);
    }
    else
        //single screen display
    {    
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 0, "single screen display ");
        g_struLocalParam.bEnlarged = TRUE;
        g_pMainDlg->ArrangeOutputs(1);
    }
}
