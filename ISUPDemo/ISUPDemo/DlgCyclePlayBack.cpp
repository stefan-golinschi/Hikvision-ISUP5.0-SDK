// CyclePlayBack.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgCyclePlayBack.h"
#include "Public/Convert/Convert.h"

//char g_szFileName[MAX_CYCLE_NUM][100] = {"ch01_08000000013000106", "ch01_08000000013000206", "ch01_08000000013000306", "ch01_08000000013000406"};
//char g_szFileName[MAX_CYCLE_NUM][100] = {"ch01_08000000011000106"};
// CCyclePlayBack 对话框

CDlgCyclePlayBack *g_CyclePlayBack = NULL;

//数据回调
BOOL CALLBACK  CyclePlayBackDataCB_0(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_INFO *pDataCBInfo, void* pUserData)
{
//     char szOutput[128] = {0};
//     sprintf(szOutput, "handle[%d], data type[%d], data len[%d]\n",iPlayBackLinkHandle, pDataCBInfo->dwType, pDataCBInfo->dwDataLen);
//     OutputDebugString(szOutput);

    UN_REFERENCED_PARAMETER(iPlayBackLinkHandle)
    if (NULL == pDataCBInfo)
    {
        return FALSE;
    }

    BYTE *pBuffer = pDataCBInfo->pData;
    DWORD dwBufSize = pDataCBInfo->dwDataLen;

    CDlgCyclePlayBack *pThis = (CDlgCyclePlayBack*)pUserData;
    

    BOOL bRet = FALSE;
    int i = 0;
    LONG lPort = -1;

    //return bRet;

    switch (pDataCBInfo->dwType)
    {
    case NET_EHOME_SYSHEAD:   
        if (!PlayM4_GetPort(&lPort))
        {
            return FALSE;
        }

        pThis->m_struPlayParam[0].lPort = lPort;
        pThis->m_struPlayParam[0].hWnd = pThis->m_PlayWnd[0];

//         EnterCriticalSection(&pThis->m_cs);
//         pThis->SetPlayPort(iPlayBackLinkHandle, lPort);
//         lIndex = pThis->FindIndex(iPlayBackLinkHandle);
//         LeaveCriticalSection(&pThis->m_cs);
//         if (lIndex < 0)
//         {
//             //ASSERT(0);
//             return FALSE;
//         }


        bRet = PlayM4_SetOverlayMode(lPort, FALSE, COLORREF(0));//not play on overplay
        bRet = PlayM4_SetStreamOpenMode(lPort, STREAME_FILE);

        if (dwBufSize > 0)
        {
            bRet = PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 600*1000);//SOURCE_BUF_MIN50*1000
            if (bRet)
            {
                //EnterCriticalSection(&pThis->m_cs);
                bRet = PlayM4_Play(lPort, pThis->m_struPlayParam[0].hWnd);
                //LeaveCriticalSection(&pThis->m_cs);
                if (!bRet)
                {
                    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lPort));
                }
                else
                {
                    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_SUCC_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lPort));
                }
            }
            else
            {
                //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_OpenStream err[%d]!", PlayM4_GetLastError(lPort));
            }
        }
        break;
    case NET_EHOME_STREAMDATA:
        //EnterCriticalSection(&pThis->m_cs);
//         lIndex = pThis->FindIndex(iPlayBackLinkHandle);
//         if (lIndex < 0)
//         {
//             //ASSERT(0);
//             return FALSE;
//         }

        
        if (dwBufSize > 0 && pThis->m_struPlayParam[0].lPort >=0)
        {
            for (i=0; i<1000; i++)
            {
                bRet = PlayM4_InputData(pThis->m_struPlayParam[0].lPort, pBuffer, dwBufSize);
                if (!bRet)
                {
//                     if ( i >=999)
//                     {
//                         g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_InputData err[%d]!", PlayM4_GetLastError(lPort));
//                     }

                    Sleep(1);
                }
                else
                {
                    break;
                }
            }
        }
        //LeaveCriticalSection(&pThis->m_cs);
        break;
    }

    return TRUE;
}

BOOL CALLBACK  CyclePlayBackDataCB_1(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_INFO *pDataCBInfo, void* pUserData)
{
    //     char szOutput[128] = {0};
    //     sprintf(szOutput, "handle[%d], data type[%d], data len[%d]\n",iPlayBackLinkHandle, pDataCBInfo->dwType, pDataCBInfo->dwDataLen);
    //     OutputDebugString(szOutput);
    UN_REFERENCED_PARAMETER(iPlayBackLinkHandle)
    if (NULL == pDataCBInfo)
    {
        return FALSE;
    }

    BYTE *pBuffer = pDataCBInfo->pData;
    DWORD dwBufSize = pDataCBInfo->dwDataLen;

    CDlgCyclePlayBack *pThis = (CDlgCyclePlayBack*)pUserData;


    BOOL bRet = FALSE;
    int i = 0;
    LONG lPort = -1;

    //return bRet;

    switch (pDataCBInfo->dwType)
    {
    case NET_EHOME_SYSHEAD:   
        if (!PlayM4_GetPort(&lPort))
        {
            return FALSE;
        }

        pThis->m_struPlayParam[1].lPort = lPort;
        pThis->m_struPlayParam[1].hWnd = pThis->m_PlayWnd[1];


        bRet = PlayM4_SetOverlayMode(lPort, FALSE, COLORREF(0));//not play on overplay
        bRet = PlayM4_SetStreamOpenMode(lPort, STREAME_FILE);

        if (dwBufSize > 0)
        {
            bRet = PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 600*1000);//SOURCE_BUF_MIN50*1000
            if (bRet)
            {

                bRet = PlayM4_Play(lPort, pThis->m_struPlayParam[1].hWnd);

                if (!bRet)
                {
                    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lPort));
                }
                else
                {
                    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_SUCC_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lPort));
                }
            }
            else
            {
                //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_OpenStream err[%d]!", PlayM4_GetLastError(lPort));
            }
        }
        break;
    case NET_EHOME_STREAMDATA:
        if (dwBufSize > 0 && pThis->m_struPlayParam[1].lPort >=0)
        {
            for (i=0; i<1000; i++)
            {
                bRet = PlayM4_InputData(pThis->m_struPlayParam[1].lPort, pBuffer, dwBufSize);
                if (!bRet)
                {
//                     if ( i >=999)
//                     {
//                         g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_InputData err[%d]!", PlayM4_GetLastError(lPort));
//                     }

                    Sleep(1);
                }
                else
                {
                    break;
                }
            }
        }
        break;
    }

    return TRUE;
}

BOOL CALLBACK  CyclePlayBackDataCB_2(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_INFO *pDataCBInfo, void* pUserData)
{
    //     char szOutput[128] = {0};
    //     sprintf(szOutput, "handle[%d], data type[%d], data len[%d]\n",iPlayBackLinkHandle, pDataCBInfo->dwType, pDataCBInfo->dwDataLen);
    //     OutputDebugString(szOutput);
    UN_REFERENCED_PARAMETER(iPlayBackLinkHandle)
    if (NULL == pDataCBInfo)
    {
        return FALSE;
    }

    BYTE *pBuffer = pDataCBInfo->pData;
    DWORD dwBufSize = pDataCBInfo->dwDataLen;

    CDlgCyclePlayBack *pThis = (CDlgCyclePlayBack*)pUserData;


    BOOL bRet = FALSE;
    int i = 0;
    LONG lPort = -1;

    //return bRet;

    switch (pDataCBInfo->dwType)
    {
    case NET_EHOME_SYSHEAD:   
        if (!PlayM4_GetPort(&lPort))
        {
            return FALSE;
        }

        pThis->m_struPlayParam[2].lPort = lPort;
        pThis->m_struPlayParam[2].hWnd = pThis->m_PlayWnd[2];


        bRet = PlayM4_SetOverlayMode(lPort, FALSE, COLORREF(0));//not play on overplay
        bRet = PlayM4_SetStreamOpenMode(lPort, STREAME_FILE);

        if (dwBufSize > 0)
        {
            bRet = PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 600*1000);//SOURCE_BUF_MIN50*1000
            if (bRet)
            {

                bRet = PlayM4_Play(lPort, pThis->m_struPlayParam[2].hWnd);

                if (!bRet)
                {
                    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lPort));
                }
                else
                {
                    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_SUCC_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lPort));
                }
            }
            else
            {
                //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_OpenStream err[%d]!", PlayM4_GetLastError(lPort));
            }
        }
        break;
    case NET_EHOME_STREAMDATA:


        if (dwBufSize > 0 && pThis->m_struPlayParam[2].lPort >=0)
        {
            for (i=0; i<1000; i++)
            {
                bRet = PlayM4_InputData(pThis->m_struPlayParam[2].lPort, pBuffer, dwBufSize);
                if (!bRet)
                {

                    Sleep(1);
                }
                else
                {
                    break;
                }
            }
        }

        break;
    }

    return TRUE;
}

BOOL CALLBACK  CyclePlayBackDataCB_3(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_INFO *pDataCBInfo, void* pUserData)
{
    //     char szOutput[128] = {0};
    //     sprintf(szOutput, "handle[%d], data type[%d], data len[%d]\n",iPlayBackLinkHandle, pDataCBInfo->dwType, pDataCBInfo->dwDataLen);
    //     OutputDebugString(szOutput);
    UN_REFERENCED_PARAMETER(iPlayBackLinkHandle)
    if (NULL == pDataCBInfo)
    {
        return FALSE;
    }

    BYTE *pBuffer = pDataCBInfo->pData;
    DWORD dwBufSize = pDataCBInfo->dwDataLen;

    CDlgCyclePlayBack *pThis = (CDlgCyclePlayBack*)pUserData;


    BOOL bRet = FALSE;
    int i = 0;
    LONG lPort = -1;

    //return bRet;

    switch (pDataCBInfo->dwType)
    {
    case NET_EHOME_SYSHEAD:   
        if (!PlayM4_GetPort(&lPort))
        {
            return FALSE;
        }

        pThis->m_struPlayParam[3].lPort = lPort;
        pThis->m_struPlayParam[3].hWnd = pThis->m_PlayWnd[3];


        bRet = PlayM4_SetOverlayMode(lPort, FALSE, COLORREF(0));//not play on overplay
        bRet = PlayM4_SetStreamOpenMode(lPort, STREAME_FILE);

        if (dwBufSize > 0)
        {
            bRet = PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 600*1000);//SOURCE_BUF_MIN50*1000
            if (bRet)
            {
                bRet = PlayM4_Play(lPort, pThis->m_struPlayParam[3].hWnd);
                if (!bRet)
                {
                    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lPort));
                }
                else
                {
                    //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_SUCC_T, 0, "PlayM4_Play err [%d]!", PlayM4_GetLastError(lPort));
                }
            }
            else
            {
                //g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), PLAY_FAIL_T, 0, "PlayM4_OpenStream err[%d]!", PlayM4_GetLastError(lPort));
            }
        }
        break;
    case NET_EHOME_STREAMDATA:

        if (dwBufSize > 0 && pThis->m_struPlayParam[3].lPort >=0)
        {
            for (i=0; i<1000; i++)
            {
                bRet = PlayM4_InputData(pThis->m_struPlayParam[3].lPort, pBuffer, dwBufSize);
                if (!bRet)
                {
                    Sleep(1);
                }
                else
                {
                    break;
                }
            }
        }

        break;
    }

    return TRUE;
}

//new link回调
BOOL CALLBACK CyclePlayBackNewLinkCB_0(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_NEWLINK_CB_INFO *pNewLinkCBInfo, void* pUserData)
{
    UN_REFERENCED_PARAMETER(pNewLinkCBInfo)
    //设置数据回调
    NET_EHOME_PLAYBACK_DATA_CB_PARAM struCBParam = {0};
    struCBParam.fnPlayBackDataCB = CyclePlayBackDataCB_0;
    struCBParam.pUserData = pUserData;
    CDlgCyclePlayBack *pThis = (CDlgCyclePlayBack*)pUserData;
//     for (int i=0; i<MAX_CYCLE_NUM; i++)
//     {
//         if (-1 == pThis->m_lPlayBackSession[i])
//         {
//             pThis->m_lPlayBackSession[i] = iPlayBackLinkHandle;
//             break;
//         }
//     }

//     EnterCriticalSection(&pThis->m_cs);
//     BOOL bRet = pThis->SetPlaySession(iPlayBackLinkHandle);
//     LeaveCriticalSection(&pThis->m_cs);
//     if (!bRet)
//     {
//         sprintf(szLan, "SetPlaySession failed handle[%d], close this handle \n", iPlayBackLinkHandle);
//         OutputDebugString(szLan);
//         NET_ESTREAM_ClosePlayBackLink(iPlayBackLinkHandle);
//         return FALSE;
//     }
//     else
//     {
//         sprintf(szLan, "New Link handle[%d]\n", iPlayBackLinkHandle);
//         OutputDebugString(szLan);
//     }

    pThis->m_struPlayParam[0].lStreamSession = iPlayBackLinkHandle;

    NET_ESTREAM_SetPlayBackDataCB(iPlayBackLinkHandle, &struCBParam);
    return TRUE;
}

BOOL CALLBACK CyclePlayBackNewLinkCB_1(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_NEWLINK_CB_INFO *pNewLinkCBInfo, void* pUserData)
{
    UN_REFERENCED_PARAMETER(pNewLinkCBInfo)
    //设置数据回调
    NET_EHOME_PLAYBACK_DATA_CB_PARAM struCBParam = {0};
    struCBParam.fnPlayBackDataCB = CyclePlayBackDataCB_1;
    struCBParam.pUserData = pUserData;
    CDlgCyclePlayBack *pThis = (CDlgCyclePlayBack*)pUserData;

    pThis->m_struPlayParam[1].lStreamSession = iPlayBackLinkHandle;
    NET_ESTREAM_SetPlayBackDataCB(iPlayBackLinkHandle, &struCBParam);
    return TRUE;
}

BOOL CALLBACK CyclePlayBackNewLinkCB_2(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_NEWLINK_CB_INFO *pNewLinkCBInfo, void* pUserData)
{
    UN_REFERENCED_PARAMETER(pNewLinkCBInfo)
    //设置数据回调
    NET_EHOME_PLAYBACK_DATA_CB_PARAM struCBParam = {0};
    struCBParam.fnPlayBackDataCB = CyclePlayBackDataCB_2;
    struCBParam.pUserData = pUserData;
    CDlgCyclePlayBack *pThis = (CDlgCyclePlayBack*)pUserData;
 
    pThis->m_struPlayParam[2].lStreamSession = iPlayBackLinkHandle;


    NET_ESTREAM_SetPlayBackDataCB(iPlayBackLinkHandle, &struCBParam);
    return TRUE;
}

BOOL CALLBACK CyclePlayBackNewLinkCB_3(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_NEWLINK_CB_INFO *pNewLinkCBInfo, void* pUserData)
{
    UN_REFERENCED_PARAMETER(pNewLinkCBInfo)
    //设置数据回调
    NET_EHOME_PLAYBACK_DATA_CB_PARAM struCBParam = {0};
    struCBParam.fnPlayBackDataCB = CyclePlayBackDataCB_3;
    struCBParam.pUserData = pUserData;
    CDlgCyclePlayBack *pThis = (CDlgCyclePlayBack*)pUserData;
    pThis->m_struPlayParam[3].lStreamSession = iPlayBackLinkHandle;

    NET_ESTREAM_SetPlayBackDataCB(iPlayBackLinkHandle, &struCBParam);
    return TRUE;
}


IMPLEMENT_DYNAMIC(CDlgCyclePlayBack, CDialog)

CDlgCyclePlayBack::CDlgCyclePlayBack(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgCyclePlayBack::IDD, pParent)
{
    for (int i=0; i<MAX_CYCLE_NUM; i++)
    {
        m_struPlayParam[i].lStreamSession = -1;
        m_struPlayParam[i].lPort = -1;
        m_struPlayParam[i].hWnd = NULL;
        m_struPlayParam[i].lCmsSession = -1;
        m_lCycleListenHandle[i] = -1;
        m_hCycleThread[i] = NULL;
    }

    m_iDeviceIndex = 0;
    m_bCycleStart = FALSE;
    m_bExitCycle = FALSE;

}

CDlgCyclePlayBack::~CDlgCyclePlayBack()
{
    //DeleteCriticalSection(&m_cs);
}

void CDlgCyclePlayBack::AddPlayBackFile(int index, char* pFile)
{
    if (index >= 0 && index < MAX_CYCLE_NUM)
    {
        strcpy(m_szFileName[index], pFile);
    }
}

void CDlgCyclePlayBack::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCyclePlayBack, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_START_CYCLE, &CDlgCyclePlayBack::OnBnClickedButtonStartCycle)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_STOP_CYCLE, &CDlgCyclePlayBack::OnBnClickedButtonStopCycle)
END_MESSAGE_MAP()


BOOL CDlgCyclePlayBack::IsFull()
{
    int i = 0;
    for (i=0; i<MAX_CYCLE_NUM; i++)
    {
        if (m_struPlayParam[i].lStreamSession == -1)
        {
            break;
        }
    } 

    if (i == MAX_CYCLE_NUM)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

BOOL CDlgCyclePlayBack::SetPlaySession(LONG lSession)
{
    int i = 0;
    for (i=0; i<MAX_CYCLE_NUM; i++)
    {
        if (m_struPlayParam[i].lStreamSession == -1)
        {
            m_struPlayParam[i].lStreamSession = lSession;
            m_struPlayParam[i].hWnd = m_PlayWnd[i];
            break;
        }
    }

    if (i == MAX_CYCLE_NUM)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BOOL CDlgCyclePlayBack::SetPlayPort(LONG lSession, LONG lPort)
{
    int i = 0;
    for (i=0; i<MAX_CYCLE_NUM; i++)
    {
        if (m_struPlayParam[i].lStreamSession == lSession)
        {
            m_struPlayParam[i].lPort = lPort;
            break;
        }
    }

    if (i == MAX_CYCLE_NUM)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

int CDlgCyclePlayBack::FindIndex(LONG lSession)
{
    
    for (int i=0; i<MAX_CYCLE_NUM; i++)
    {
        if (m_struPlayParam[i].lStreamSession == lSession)
        {
            return i;
        }
    }

    return -1;

}

// CCyclePlayBack 消息处理程序

void CDlgCyclePlayBack::StartPlayBack(int iIndex)
{
    if (iIndex < 0 || iIndex >= MAX_CYCLE_NUM)
    {
        ASSERT(0);
    }

    char szLan[256] = {0};

    NET_EHOME_PLAYBACK_LISTEN_PARAM struListenParam = {0};
    struListenParam.byLinkMode = 0;
    

    switch (iIndex)
    {
    case 0:
        struListenParam.fnNewLinkCB = CyclePlayBackNewLinkCB_0;
        break;
    case 1:
        struListenParam.fnNewLinkCB = CyclePlayBackNewLinkCB_1;
        break;
    case 2:
        struListenParam.fnNewLinkCB = CyclePlayBackNewLinkCB_2;
        break;
    case 3:
        struListenParam.fnNewLinkCB = CyclePlayBackNewLinkCB_3;
        break;
    default:
        struListenParam.fnNewLinkCB = CyclePlayBackNewLinkCB_0;
        break;
    }

    struListenParam.pUserData = this;
   // g_pMainDlg->GetLocalIP(struListenParam.struIPAdress.szIP);
    strncpy(struListenParam.struIPAdress.szIP, g_pMainDlg->m_sLocalIP, 128);
    struListenParam.struIPAdress.wPort = m_wPort[iIndex];
    
    m_lCycleListenHandle[iIndex] = NET_ESTREAM_StartListenPlayBack(&struListenParam);

    if(-1 == m_lCycleListenHandle[iIndex])
    {
        //g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 2, "NET_ESTREAM_StartListenPlayBack");
        g_StringLanType(szLan, "NET_ESTREAM_StartListenPlayBack 失败 \n!", "NET_ESTREAM_StartListenPlayBack failed \n");
        OutputDebugString(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ESTREAM_StartListenPlayBack [%d]",  NET_ESTREAM_GetLastError());
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ESTREAM_StartListenPlayBack Succ Port[%d]", m_wPort[iIndex]);
        sprintf(szLan, "NET_ESTREAM_StartListenPlayBack handle[%d], index[%d]\n", m_lCycleListenHandle[iIndex], iIndex);
        OutputDebugString(szLan);
    }

    NET_EHOME_PLAYBACK_INFO_IN struPlayBackInfoIn = {0};
    struPlayBackInfoIn.byPlayBackMode = 0;
    struPlayBackInfoIn.dwChannel = 1;
    //g_pMainDlg->GetLocalIP(struPlayBackInfoIn.struStreamSever.szIP);

    //strncpy(struPlayBackInfoIn.struStreamSever.szIP, g_pMainDlg->m_sLocalIP, 128);
    //struPlayBackInfoIn.struStreamSever.wPort = m_wPort[iIndex];

    DWORD dwPortTemp = 0;
    g_pMainDlg->GetAddressByType(5, 0, struPlayBackInfoIn.struStreamSever.szIP, sizeof(struPlayBackInfoIn.struStreamSever.szIP), (char *)&dwPortTemp, sizeof(dwPortTemp));
    // strncpy(struListenParam.struIPAdress.szIP, g_pMainDlg->m_sLocalIP, 128);
    struPlayBackInfoIn.struStreamSever.wPort = (WORD)(dwPortTemp == 0 ? m_wPort[iIndex] : dwPortTemp);

    strncpy(struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName, m_szFileName[iIndex], 100);

    //需要将字符串字段转换成UTF-8
    DWORD dwConvertLen = 0;
    A2UTF8((char*)struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName, 
           (char*)struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName, 
           MAX_FILE_NAME_LEN, &dwConvertLen);

    NET_EHOME_PLAYBACK_INFO_OUT struPlayBackInfoOut = {0};
    if (!NET_ECMS_StartPlayBack(g_struDeviceInfo[m_iDeviceIndex].lLoginID, &struPlayBackInfoIn, &struPlayBackInfoOut))
    {
        //g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartPlayBack");
        g_StringLanType(szLan, "NET_ECMS_StartPlayBack 失败! \n", "NET_ECMS_StartPlayBack failed \n");
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartPlayBack, [Error:%d][FileName:%s]",  NET_ECMS_GetLastError(), struPlayBackInfoIn.unionPlayBackMode.struPlayBackbyName.szFileName);
        OutputDebugString(szLan); 
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartPlayBack Succ");
    }

//     if (g_pMainDlg->m_bAsyncCalllback)
//     {
//         LONG lSession = -1;
//         while (!g_pMainDlg->PopSessionByHandle(struPlayBackInfoOut.lHandle, &lSession))
//         {
//             Sleep(100);
//         }
//         m_struPlayParam[iIndex].lCmsSession = lSession;
//     }
//     else
//     {
//         m_struPlayParam[iIndex].lCmsSession = struPlayBackInfoOut.lSessionID;
//     }

    if (!g_pMainDlg->m_bAsyncCalllback)
    {
        NET_EHOME_PUSHPLAYBACK_IN struPushBackIn = { 0 };
        struPushBackIn.dwSize = sizeof(NET_EHOME_PUSHPLAYBACK_IN);
        struPushBackIn.lSessionID = m_struPlayParam[iIndex].lCmsSession;

        NET_EHOME_PUSHPLAYBACK_OUT struPushBackOut = { 0 };
        struPushBackOut.dwSize = sizeof(struPushBackOut); 
        if (!NET_ECMS_StartPushPlayBack(g_struDeviceInfo[m_iDeviceIndex].lLoginID, &struPushBackIn, &struPushBackOut))
        {
            g_StringLanType(szLan, "NET_ECMS_StartPushPlayBack! \n", "NET_ECMS_StartPushPlayBack failed \n");
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_StartPushPlayBack [%d]", NET_ECMS_GetLastError());
            OutputDebugString(szLan);
            return;
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_StartPushPlayBack Succ");
        }
    }

//     if (g_pMainDlg->m_bAsyncCalllback)
//     {
//         LONG lSession = -1;
//         while (!g_pMainDlg->PopSessionByHandle(struPushBackOut.lHandle))
//         {
//             Sleep(100);
//         }
//     }

}

/** @fn    void CDlgCyclePlayBack::StopPlayBack(int iIndex)
 *  @brief
 *  @param (in)    int iIndex    
 *  @return    void
 */
void CDlgCyclePlayBack::StopPlayBack(int iIndex)
{

    char szLan[256] = {0};

    //停止该路连接        
    if (m_struPlayParam[iIndex].lStreamSession != -1)        
    {
        NET_ESTREAM_StopPlayBack(m_struPlayParam[iIndex].lStreamSession);
        sprintf(szLan, "NET_ESTREAM_StopPlayBack handle[%d], index[%d] \n", m_struPlayParam[iIndex].lStreamSession, iIndex);
        OutputDebugString(szLan);
        m_struPlayParam[iIndex].lStreamSession = -1;

    }    

    //停止监听
    if (m_lCycleListenHandle[iIndex] != -1)
    {
        NET_ESTREAM_StopListenPlayBack(m_lCycleListenHandle[iIndex]);
        sprintf(szLan, "NET_ESTREAM_StopListenPlayBack listen handle[%d], index[%d] \n", m_lCycleListenHandle[iIndex], iIndex);
        OutputDebugString(szLan);
        m_lCycleListenHandle[iIndex] = -1;
    }

//     if(!NET_ESTREAM_StopListenPlayBack(m_lCycleListenHandle[iIndex]))
//     {
//         //g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 2, "NET_ESTREAM_StopListenPlayBack");
//         g_StringLanType(szLan, "NET_ESTREAM_StopListenPlayBack 失败!", "NET_ESTREAM_StopListenPlayBack failed");
//         OutputDebugString(szLan); 
//         return;
//     }

    if (m_struPlayParam[iIndex].lPort >= 0)
    {
        PlayM4_FreePort(m_struPlayParam[iIndex].lPort);
        sprintf(szLan, "PlayM4_FreePort lPort[%d], index[%d] \n", m_struPlayParam[iIndex].lPort, iIndex);
        OutputDebugString(szLan);
        m_struPlayParam[iIndex].lPort = -1;
    }

    //     UpdateData(FALSE);
    Invalidate(TRUE);
}



BOOL CDlgCyclePlayBack::OnInitDialog()
{
    CDialog::OnInitDialog();

    g_CyclePlayBack = this;

    // TODO:  在此添加额外的初始化
    m_PlayWnd[0] = GetDlgItem(IDC_STATIC_WND1)->m_hWnd;
    m_PlayWnd[1] = GetDlgItem(IDC_STATIC_WND2)->m_hWnd;
    m_PlayWnd[2] = GetDlgItem(IDC_STATIC_WND3)->m_hWnd;
    m_PlayWnd[3] = GetDlgItem(IDC_STATIC_WND4)->m_hWnd;

    m_wPort[0] = 8888;
    m_wPort[1] = 8889;
    m_wPort[2] = 8890;
    m_wPort[3] = 8891;

    //InitializeCriticalSection(&m_cs);


    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgCyclePlayBack::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    switch (nIDEvent)
    {
    case CYCLE_PLAYBACK_TIMER:
        if (!m_bCycleStart)
        {
            CycleStartPlayBack();
        }
        else
        {
            CycleStopPlayBack();
        }
        break;
    default:
        break;
    }

    CDialog::OnTimer(nIDEvent);
}

UINT CDlgCyclePlayBack::CycleThreadProc(LPVOID pParam)
{
    int i = (int)pParam;
    while (!g_CyclePlayBack->m_bExitCycle)
    {
        //OutputDebugString("StartPlayBack\n");
        g_CyclePlayBack->StartPlayBack(i);

        Sleep(8000);

        g_CyclePlayBack->StopPlayBack(i);

        Sleep(2000);
        //OutputDebugString("StopPlayBack\n");

    }

    OutputDebugString("exit\n");

    return 0;
}

void CDlgCyclePlayBack::OnBnClickedButtonStartCycle()
{
    // TODO: 在此添加控件通知处理程序代码
//     SetTimer(CYCLE_PLAYBACK_TIMER, 2000, NULL);
//     CycleStartPlayBack();

    m_bExitCycle = FALSE;

    for (int i=0; i<MAX_CYCLE_NUM; i++)
    {
        if (NULL == m_hCycleThread[i])
        {
            m_hCycleThread[i] = CreateThread(NULL, 0 ,LPTHREAD_START_ROUTINE(CycleThreadProc), (LPVOID)i, 0, NULL);
        }

        if (NULL == m_hCycleThread[i])
        {
            ASSERT(0);
            return;
        }
    }


}

void CDlgCyclePlayBack::OnBnClickedButtonStopCycle()
{
    // TODO: 在此添加控件通知处理程序代码

    m_bExitCycle = TRUE;

    for (int i=0; i<MAX_CYCLE_NUM; i++)
    {
        WaitForSingleObject(m_hCycleThread[i], INFINITE);
        CloseHandle(m_hCycleThread[i]);
        m_hCycleThread[i] = NULL;
    }
    

//     KillTimer(CYCLE_PLAYBACK_TIMER);
//     CycleStopPlayBack();

}

void CDlgCyclePlayBack::CycleStartPlayBack()
{
    m_bCycleStart = TRUE;
    for (int i=0; i<MAX_CYCLE_NUM; i++)
    {
        StartPlayBack(i);
    }
}

void CDlgCyclePlayBack::CycleStopPlayBack()
{
    m_bCycleStart = FALSE;
    for (int i=0; i<MAX_CYCLE_NUM; i++)
    {
        StopPlayBack(i);
    }


    NET_EHOME_STOPPLAYBACK_PARAM struStopParam = { 0 };
    struStopParam.lSessionID = 0;

    //if (!NET_ECMS_StopPlayBack(g_struDeviceInfo[m_iDeviceIndex].lLoginID, 0))
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
    }


    
}
