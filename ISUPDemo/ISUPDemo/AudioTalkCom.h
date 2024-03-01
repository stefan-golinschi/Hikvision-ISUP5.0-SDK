#ifndef _AUDIO_INTERFACECOM_PUBLIC_H_
#define _AUDIO_INTERFACECOM_PUBLIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AudioIntercom.h"


typedef void (CALLBACK *SoundInCB)(char*, DWORD, void*);

class CAudioTalkCom
{
public:
    CAudioTalkCom();
    virtual ~CAudioTalkCom();

    int OpenAudioIn(AudioParam* pstAudioParam);
    int CloseAudioIn();
    int OpenAudioOut(AudioParam* pstAudioParam);
    int CloseAudioOut();
    int InputStreamData(char *lpTemp, int Bytes);
    int SetSoundInDataCB(SoundInCB pfDataCb, void* pUser);

protected:
    
    int GetSoundCardInfo(UINT32* pdwDeviceNum, char *pCapDevName, int length);
    int GetPlayCardInfo(UINT32* pdwDeviceNum, char *pCapDevName, int length);

    int OpenStreamEx(int iPlayPort, AudioParam* pstAudioParam);

    int RegisterPcmOutputDataCallBack(int iCapturePort, OutputDataCallBackEx pfnOutputDataCallBack, void* pUser);
    int RegisterOutputDataCallBackEx(int iCapturePort, AudioParam* pstAudioParam, OutputDataCallBackEx pfnOutputDataCallBackEx, void* pUser);
    int RegisterDecodeDataCallBack(int iPlayPort, OutputDataCallBack pfnOutputDataCallBack, void* pUser);

    static void CALLBACK AudioOutputDataCallBack(OutputDataInfoEx* pstDataInfo, void* pUser);
    static void CALLBACK AudioPcmDataCallBack(OutputDataInfoEx* pstDataInfo, void* pUser);

    BOOL CreateCaptureHandle(int* pCapturePort, const char* pDeviceName);
    BOOL ReleaseCaptureHandle(int iCapturePort);
    BOOL CreatePlayHandle(int* pPlayPort, const char* pDeviceName);
    BOOL ReleasePlayHandle(int iPlayPort);

    BOOL StartCapture(int iCapturePort);
    BOOL StopCapture(int iCapturePort);
    BOOL StartPlay(int iPlayPort);
    BOOL StopPlay(int iPlayPort);

protected:
    int m_iCapturePort;
    int m_iPlayPort;
    AudioParam m_stuAudioInParam;
    AudioParam m_stuAudioOutParam;
    SoundInCB m_soundInCB;
    void* m_pUser;
};

#endif
