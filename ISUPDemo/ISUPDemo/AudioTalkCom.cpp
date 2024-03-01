#include "stdafx.h"
#include "AudioTalkCom.h"

#pragma warning(disable:4353)

CAudioTalkCom::CAudioTalkCom() :m_iCapturePort(-1)
, m_iPlayPort(-1)
{
    memset(&m_stuAudioInParam, 0, sizeof(m_stuAudioInParam));
    memset(&m_stuAudioOutParam, 0, sizeof(m_stuAudioOutParam));
    
    m_soundInCB = NULL;
    m_pUser = NULL;
}

CAudioTalkCom::~CAudioTalkCom()
{

}

int CAudioTalkCom::OpenAudioIn(AudioParam* pstAudioParam)
{
    if (pstAudioParam == NULL)
    {
        return -1;
    }
    memcpy(&m_stuAudioInParam, pstAudioParam, sizeof(AudioParam));

    UINT32 dwDeviceNum = 0;
    char cCapDevName[128];
    if (GetSoundCardInfo(&dwDeviceNum, cCapDevName, 128) < 0)
    {
        return -1;
    }
    if (!CreateCaptureHandle(&m_iCapturePort, cCapDevName))
    {
        return -1;
    }
    if (RegisterOutputDataCallBackEx(m_iCapturePort, &m_stuAudioInParam, AudioOutputDataCallBack, this) < 0)
    {
        if (m_iCapturePort >= 0)
        {
            ReleaseCaptureHandle(m_iCapturePort);
        }
        return -1;
    }
    //if (RegisterPcmOutputDataCallBack(m_iCapturePort, AudioPcmDataCallBack, this) < 0)
    //{
        //return -1;
    //}
    if (!StartCapture(m_iCapturePort))
    {
        if (m_iCapturePort >= 0)
        {
            ReleaseCaptureHandle(m_iCapturePort);
        }
        return -1;
    }
    TRACE("OpenAudioIn success: %d,%s\n\n", m_iCapturePort, cCapDevName);
    return 0;
}

int CAudioTalkCom::CloseAudioIn()
{
    if (m_iCapturePort >= 0)
    {
        StopCapture(m_iCapturePort);
        ReleaseCaptureHandle(m_iCapturePort);
    }

    return 0;
}

int CAudioTalkCom::OpenAudioOut(AudioParam* pstAudioParam)
{
    if (pstAudioParam == NULL)
    {
        return -1;
    }
    memcpy(&m_stuAudioOutParam, pstAudioParam, sizeof(AudioParam));

    UINT32 dwDeviceNum = 0;
    char cCapDevName[128];
    if (GetPlayCardInfo(&dwDeviceNum, cCapDevName, 128) < 0)
    {
        return -1;
    }
    if (!CreatePlayHandle(&m_iPlayPort, cCapDevName))
    {
        return -1;
    }    
    if (OpenStreamEx(m_iPlayPort, pstAudioParam) < 0)
    {
        if (m_iPlayPort >= 0)
        {
            ReleasePlayHandle(m_iPlayPort);
        }
        return -1;
    }
    if (!StartPlay(m_iPlayPort))
    {
        if (m_iPlayPort >= 0)
        {
            ReleasePlayHandle(m_iPlayPort);
        }
        return -1;
    }
    TRACE("OpenAudioOut success: %d,%s\n\n", m_iPlayPort, cCapDevName);
    return 0;
}

int CAudioTalkCom::CloseAudioOut()
{
    if (m_iPlayPort >= 0)
    {
        StopPlay(m_iPlayPort);
        ReleasePlayHandle(m_iPlayPort);
    }
    return 0;
}

int CAudioTalkCom::InputStreamData(char *lpTemp, int Bytes)
{
    if (AUDIOCOM_InputStreamData(m_iPlayPort, (unsigned char*)lpTemp, (unsigned int)Bytes) == 0)
    {
        ERROR("AUDIOCOM_InputStreamData failed:%d\n", AUDIOCOM_GetLastError(m_iPlayPort));
        return -1;
    }
    return 0;
}

int CAudioTalkCom::SetSoundInDataCB(SoundInCB pfDataCb, void* pUser)
{
    m_soundInCB = pfDataCb;
    m_pUser = pUser;
    return 0;
}

int CAudioTalkCom::GetSoundCardInfo(UINT32* pdwDeviceNum, char *pCapDevName, int length)
{
    if (pdwDeviceNum == NULL || pCapDevName == NULL)
    {
        return -1;
    }

    if (AUDIOCOM_GetCaptureDeviceNum(pdwDeviceNum) == 0)
    {
        ERROR("AUDIOCOM_GetCaptureDeviceNum failed\n");
        return -1;
    }

    TRACE("ALL CaptureDevice DEVICE NUMBER: %d\n\n", *pdwDeviceNum);
    TRACE("ALL CaptureDevice DEVICE NAMEs:\n");
    for (unsigned int i = 0; i < *pdwDeviceNum; i++)
    {
        memset(pCapDevName, 0, length);
        if (AUDIOCOM_GetOneCaptureDeviceName(i, pCapDevName) > 0)
        {
            TRACE("%d-CapDevName:%s\n\n", i, pCapDevName);
            break;
        }
       
    }
    return 0;
}

int CAudioTalkCom::GetPlayCardInfo(UINT32* pdwDeviceNum, char *pCapDevName, int length)
{
    if (pdwDeviceNum == NULL || pCapDevName == NULL)
    {
        return -1;
    }

    if (AUDIOCOM_GetPlayDeviceNum(pdwDeviceNum) == 0)
    {
        ERROR("AUDIOCOM_GetPlayDeviceNum failed\n");
        return -1;
    }
    TRACE("ALL GetPlayCardInfo DEVICE NUMBER: %d\n\n", *pdwDeviceNum);

    TRACE("ALL GetPlayCardInfo DEVICE NAMEs:\n");
    for (unsigned int i = 0; i < *pdwDeviceNum; i++)
    {
        memset(pCapDevName, 0, length);
        if (AUDIOCOM_GetOnePlayDeviceName(i, pCapDevName) > 0)
        {
            TRACE("%d-PlayDevName:%s\n\n", i, pCapDevName);
            break;
        }
    }
    return 0;
}

int CAudioTalkCom::OpenStreamEx(int iPlayPort, AudioParam* pstAudioParam)
{
    int iRet = AUDIOCOM_OpenStreamEx(iPlayPort, pstAudioParam);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_OpenStreamEx failed:%d\n", AUDIOCOM_GetLastError(iPlayPort));
        return -1;
    }
    return 0;
}

int CAudioTalkCom::RegisterPcmOutputDataCallBack(int iCapturePort, OutputDataCallBackEx pfnOutputDataCallBack, void* pUser)
{
    int iRet = AUDIOCOM_RegisterCaptureDataCallBackEx(iCapturePort, pfnOutputDataCallBack, this);
    if (iRet != 1)
    {
        ERROR("RegisterPcmOutputDataCallBack failed:%d\n", AUDIOCOM_GetLastError(iCapturePort));
        return -1;
    }
    return 0;
}

int CAudioTalkCom::RegisterOutputDataCallBackEx(int iCapturePort, AudioParam* pstAudioParam, OutputDataCallBackEx pfnOutputDataCallBackEx, void* pUser)
{
    TRACE("AudioOutputDataCallBack: type %d,nBitRate %d,nBitWidth %d,nSampleRate %d\n\n", pstAudioParam->enAudioEncodeTypeEx, pstAudioParam->nBitRate, pstAudioParam->nBitWidth, pstAudioParam->nSampleRate);
    int iRet = AUDIOCOM_RegisterOutputDataCallBackEx(iCapturePort, pstAudioParam, pfnOutputDataCallBackEx, this);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_RegisterOutputDataCallBackEx failed:%d\n", AUDIOCOM_GetLastError(iCapturePort));
        return -1;
    }
    return 0;
}

void CAudioTalkCom::AudioOutputDataCallBack(OutputDataInfoEx* pstDataInfo, void* pUser)
{
    CAudioTalkCom *pAudioTalk = static_cast<CAudioTalkCom*>(pUser);

//     TRACE("AudioOutputDataCallBack: %d\n\n", pstDataInfo->dwDataLen);
//     FILE *pfile = fopen("OutputAudio.wav", "ab+");
//     if (pfile != NULL)
//     {
//         fwrite(pstDataInfo->pData, 1, pstDataInfo->dwDataLen, pfile);
//         fflush(pfile);
//         fclose(pfile);
//     }
    if (pAudioTalk->m_soundInCB != NULL)
    {
        pAudioTalk->m_soundInCB((char*)pstDataInfo->pData, pstDataInfo->dwDataLen, pAudioTalk->m_pUser);
    }
}

void CAudioTalkCom::AudioPcmDataCallBack(OutputDataInfoEx* pstDataInfo, void* pUser)
{
    CAudioTalkCom *pAudioTalk = static_cast<CAudioTalkCom*>(pUser);
//     TRACE("AudioPcmDataCallBack: %d\n\n", pstDataInfo->dwDataLen);
//     FILE *pfile = fopen("OutputAudio1.pcm", "ab+");
//     if (pfile != NULL)
//     {
//         fwrite(pstDataInfo->pData, 1, pstDataInfo->dwDataLen, pfile);
//         fflush(pfile);
//         fclose(pfile);
//     }
    if (pAudioTalk->m_soundInCB != NULL)
    {
        pAudioTalk->m_soundInCB((char*)pstDataInfo->pData, pstDataInfo->dwDataLen, pAudioTalk->m_pUser);
    }
}

int CAudioTalkCom::RegisterDecodeDataCallBack(int iPlayPort, OutputDataCallBack pfnOutputDataCallBack, void* pUser)
{
    return 0;
}

BOOL CAudioTalkCom::CreateCaptureHandle(int* pCapturePort, const char* pDeviceName)
{
    int iRet = AUDIOCOM_CreateCaptureHandleEx(pCapturePort, pDeviceName);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_CreateCaptureHandleEx failed:%d\n", AUDIOCOM_GetLastError(*pCapturePort));
        return FALSE;
    }
    TRACE("Capture port = %d, name %s\n", *pCapturePort, pDeviceName);
    return TRUE;
}

BOOL CAudioTalkCom::ReleaseCaptureHandle(int iCapturePort)
{
    TRACE("ReleaseCaptureHandle\n");
    int iRet = AUDIOCOM_ReleaseCaptureHandle(iCapturePort);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_ReleaseCaptureHandle failed:%d\n", AUDIOCOM_GetLastError(iCapturePort));
        return FALSE;
    }
    return TRUE;
}

BOOL CAudioTalkCom::CreatePlayHandle(int* pPlayPort, const char* pDeviceName)
{
    TRACE("CreatePlayHandle\n");
    int iRet = AUDIOCOM_CreatePlayHandle(pPlayPort, pDeviceName);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_CreatePlayHandle failed:%d\n", AUDIOCOM_GetLastError(*pPlayPort));
        return FALSE;
    }
    return TRUE;
}

BOOL CAudioTalkCom::ReleasePlayHandle(int iPlayPort)
{
    TRACE("ReleasePlayHandle\n");
    int iRet = AUDIOCOM_ReleasePlayHandle(iPlayPort);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_ReleasePlayHandle failed:%d\n", AUDIOCOM_GetLastError(iPlayPort));
        return FALSE;
    }
    return TRUE;
}

BOOL CAudioTalkCom::StartCapture(int iCapturePort)
{
    TRACE("start StartCapture\n");
    int iRet = AUDIOCOM_StartCapture(iCapturePort);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_StartCapture failed:%d\n", AUDIOCOM_GetLastError(iCapturePort));
        return FALSE;
    }
    return TRUE;
}

BOOL CAudioTalkCom::StopCapture(int iCapturePort)
{
    TRACE("stop capture\n");
    int iRet = AUDIOCOM_StopCapture(iCapturePort);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_StopCapture failed:%d\n", AUDIOCOM_GetLastError(iCapturePort));
        return FALSE;
    }    
    return TRUE;
}

BOOL CAudioTalkCom::StartPlay(int iPlayPort)
{
    TRACE("start StartPlay\n");
    int iRet = AUDIOCOM_StartPlay(iPlayPort);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_StartPlay failed:%d\n", AUDIOCOM_GetLastError(iPlayPort));
        return FALSE;
    }
    return TRUE;
}

BOOL CAudioTalkCom::StopPlay(int iPlayPort)
{
    TRACE("stop StopPlay\n");
    int iRet = AUDIOCOM_StopPlay(iPlayPort);
    if (iRet != 1)
    {
        ERROR("AUDIOCOM_StopPlay failed:%d\n", AUDIOCOM_GetLastError(iPlayPort));
        return FALSE;
    }
    return TRUE;
}




