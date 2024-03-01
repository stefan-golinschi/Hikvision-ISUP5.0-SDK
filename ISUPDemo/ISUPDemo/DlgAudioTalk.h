#pragma once
#include "afxwin.h"
#include <string.h>
#include <list>
#include "HCISUPStream.h"
#include "wavesound.h"
#include "SoundIn.h"
#include "AudioTalkCom.h"
#include "DlgVideoIntercomCall.h"
using namespace std;
// netsdk的类型
// #define AUDIOTALKTYPE_G722       0
// #define AUDIOTALKTYPE_G711_MU    1
// #define AUDIOTALKTYPE_G711_A     2
// #define AUDIOTALKTYPE_MP2L2      5
// #define AUDIOTALKTYPE_G726       6
// #define AUDIOTALKTYPE_AAC        7
// #define AUDIOTALKTYPE_PCM        8
// EHome的类型

#define AUDIOTALKTYPE_G722       0
#define AUDIOTALKTYPE_G711_MU    1
#define AUDIOTALKTYPE_G711_A     2
#define AUDIOTALKTYPE_MP2L2      5
#define AUDIOTALKTYPE_G726       3
#define AUDIOTALKTYPE_AAC        4
#define AUDIOTALKTYPE_PCM        6
#define AUDIOTALKTYPE_MP3        7
#define AUDIOTALKTYPE_G723       8
#define AUDIOTALKTYPE_MP1L2      9
#define AUDIOTALKTYPE_ADPCM      10

#define LISTEN_PORT_BASE    7500

#define AUDENCSIZE            1280
#define AUDDECSIZE            80

#define G711_AUDENCSIZE        320
#define G711_AUDDECSIZE        160

#define G726_AUDENCSIZE     640
#define G726_AUDDECSIZE        80 

#define ADPCM_AUDENCSIZE      640     //编码输入长度
#define ADPCM_AUDDECSIZE      168     //解码输入长度

#define BITS_PER_SAMPLE        16
#define CHANNEL                1
#define SAMPLES_PER_SECOND    16000

#define BIT_RATE_16000        16000
#define AUDIOBUF            (160*40L)

#define MAX_SOUND_OUT        20

typedef struct  STRUCT_TALK_MR
{
    BYTE byFlag;
    BYTE byAudioType;
    BYTE byIndex;
    BYTE res;
    STRUCT_TALK_MR()
    {
        byFlag = 0;
        byAudioType = 0;
        byIndex = 0;
        res = 0;
    }
}TALK_MR, *LPTALK_MR;

typedef struct  STRUCT_AUDIOTALK_PARAM
{
    BYTE byChannels;
    BYTE byAudioType;
    int  nSampleRate;
    int  nBitWidth;
    int  nBitRate;
    STRUCT_AUDIOTALK_PARAM()
    {
        byChannels = CHANNEL;
        byAudioType = AUDIOTALKTYPE_PCM;
        nSampleRate = SAMPLERATE_16K;
        nBitWidth = BITS_16;
        nBitRate = BITRATE_ENCODE_16k;
    }
}AUDIOTALK_PARAM, *LPAUDIOTALK_PARAM;

typedef struct  STRUCT_AUDIO_DATA
{
    char* buffer;
    DWORD dwSize;
    STRUCT_AUDIO_DATA()
    {
        buffer = NULL;
        dwSize = 0;
    }
}AUDIO_DATA, *LP_AUDIO_DATA;

// CDlgAudioTalk dialog

#define WM_START_VOICE_TALK_OK       WM_USER + 9
#define WM_START_PUSH_VOICE_TALK_OK  WM_USER + 10
#define WM_STOP_VOICE_TALK_OK        WM_USER + 11

class CDlgAudioTalk : public CDialog
{
    DECLARE_DYNAMIC(CDlgAudioTalk)

public:
    CDlgAudioTalk(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgAudioTalk();

// Dialog Data
    enum { IDD = IDD_DLG_AUDIO_TALK };

    NET_EHOME_VOICETALK_PARA    m_struVoiceTalkPara;
    LONG m_lVoiceTalkHandle;

protected:

    virtual BOOL OnInitDialog();

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
protected:
    static DWORD WINAPI SendVoiceDataThread (LPVOID lpArg);
    HANDLE    m_hVoiceTransmit;
    BOOL    m_bExitThread;
    HANDLE m_hVoiceSendThread;
    BOOL    m_bVoiceSendThreadStart;
    static DWORD WINAPI SendDataFromSoundIn(LPVOID lpArg);

    static void CALLBACK DeviceIDChanged(LONG iDeviceID, void *pUserData);
    void ChangeAudioCtlState(LONG iDeviceID);
public:
    void    CheckInitParam();
public:

    LRESULT OnStartVoiceTalkOK(WPARAM wParam, LPARAM lParam);
    LRESULT OnStartPushVoiceTalkOK(WPARAM wParam, LPARAM lParam);
    LRESULT OnStopVoiceTalkWithSTMOK(WPARAM wParam, LPARAM lParam);

    void AfterStartVoiceWithSTM(int lSessionID);
    void AfterStartPushVoiceStream();
    void AfterStopVoiceTalkWithSTM();

    
    afx_msg void OnBnClickedBtnStartVoicetalkListen();
    afx_msg void OnBnClickedBtnReqAudioTalk();
    afx_msg void OnBnClickedBtnStartPush();
    afx_msg void OnBnClickedBtnStopPush();
    afx_msg void OnBnClickedBtnStopListen();

    afx_msg void OnBnClickedBtnStartVoicetalk();
    afx_msg void OnBnClickedBtnStopVoicetalk();
    CComboBox m_combCBDataType;
    afx_msg void OnBnClickedBtnStartTransmit();
    afx_msg void OnBnClickedBtnStopTransmit();
    CComboBox m_cmbVoiceChan;

    afx_msg void OnBnClickedBtnAudiotalkTest();

    static BOOL CALLBACK onDeviceAudioTalkConnectCB(LONG lAudioTalkHandle, NET_EHOME_VOICETALK_NEWLINK_CB_INFO *pNewLinkCBMsg, void *pUserData);
    static BOOL CALLBACK AudioTalkStreamCallback(LONG iAudioTalkHandle, NET_EHOME_VOICETALK_DATA_CB_INFO *pVoiceTalkCBMsg, void *pUserData);
    BOOL PrepareLocalData(char* buffer, DWORD dwSize, void* pOwner);
    CString m_csIP;
    int m_nPort;

    CComboBox m_cmbLocalType;

    int m_nLocalType;
    std::list<AUDIO_DATA> m_AudioSendDataList;
    HANDLE m_hListMutex;
private:

    int m_iSessionID;
    int m_iCurDevIndex;

    LONG m_lAudioTalkHandle;
    int  m_iAudioEncType;

    BOOL m_bCMSAudioTalk;
    LONG m_lListenHandle;

    BOOL            m_bWaveDeal;

    TALK_MR         m_talkMr;

    WAVEFORMATEX    m_struWaveFormat;
#ifndef WIN64
    CWaveSound      m_SoundOut;
    CSoundIn        m_SoundIn;
#endif
    BOOL            m_bOpenWavOut;
    BOOL            m_bOpenWavIn;

    HMMIO           m_hFile;
    MMCKINFO        m_MMCKInfoData;
    MMCKINFO        m_MMCKInfoParent;
    MMCKINFO        m_MMCKInfoChild;

    LPBYTE  m_pRenderBuf;        //buffer
    int     m_rIndexRV;          //data that has already been read
    int     m_ReceiveIndexRV;    //received data for m_pRenderBuf
    int     m_nBufNo;
    BOOL    m_bOpenPlayThread;

//     void*   m_pEncoder;     //g722 encoder
//     void*   m_pG726Enc;     //g726编码句柄
//     void*   m_pG726EncM;    //g726编码模块句柄
//     BOOL    m_bReset;       //g726首帧重置

    DWORD   m_dwBufSize;
    DWORD   m_dwBufNum;

//     BYTE    m_byG711DecBuf[G711_AUDDECSIZE*2];
//     BYTE    m_byDecBuf[AUDENCSIZE];
//     BYTE    m_byEncBuf[AUDDECSIZE];
//     BYTE    m_byEncBuf711[G711_AUDDECSIZE];
//     UINT16  m_wG711AudioTemp[AUDENCSIZE/2];

    HANDLE  m_hExitEvent;
    HANDLE  m_hHaveRDataRV;


    BOOL m_bRecord;
    BOOL m_bOpenTalk;
    BOOL m_bVersion4;
    AUDIOTALK_PARAM m_AudioTalkParam;
    CAudioTalkCom m_AudioTalkCom;
    BOOL ConvertAudioTalkToAudioType(AUDIOTALK_PARAM *pAudioTalk, AudioParam *pAudioParam);
public:

    BOOL SendDataToDevice(char *buf, DWORD dwSize);
    static CDlgAudioTalk *s_pAudioTalkDlg;
    void InputAudioData(LPTALK_MR lpTalkMR);
    static UINT PlayAudioThread(LPVOID pParam);
    BOOL CopyAudioData(PBYTE lpTemp, DWORD dwReadLength, int nIndex);
    BOOL PutIntoBuf(char *lpTemp, int Bytes, LPTALK_MR lpTalkMR);
    void ExitPlayAudio(BYTE *lpTemp, BYTE *lpPlayBuf, void *pDecoder, void *pG726Dec, void *pG726DecM, BYTE byIndex);

    BOOL OpenAudioIn();
    BOOL OpenAudioOut();
    CComboBox m_cmbAudioType;
    CRITICAL_SECTION m_csAudioBuf;
    afx_msg void OnBnClickedBtnVedioIntercome();
    afx_msg void OnBnClickedBtnStartVoicetalk2();
    afx_msg void OnBnClickedBtnStopVoicetalk3();
    BOOL GetAudioParam(int iDeviceIndex, unsigned short *wBitWidth, unsigned int *iSampleRate, unsigned int *iBitRate);
    BOOL GetAudioParamWithChan(int iDeviceIndex, int iChannel, int& iAudioType);
    BOOL GetAudioType(int& iAudioType);
    int AudidTyeStringToInt(const string strAudioTye);
    void SendAudioData(char* szAudioData, int iAudioLen);
    BOOL m_byEncrypt;
    // 传输链路是否加密，不加密使用TCP，加密使用TLS
    BOOL m_bLinkEncrypt;
    BOOL m_bVoiceBroadcast;
    int m_iBroadcastLevel;
    int m_iBroadcastVolume;
    CComboBox m_cmbAudioSamplingRate;
};
