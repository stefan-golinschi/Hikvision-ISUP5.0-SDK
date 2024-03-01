#ifndef _HC_EHOME_STREAM_H_
#define _HC_EHOME_STREAM_H_

#include "HCISUPPublic.h"


typedef struct tagNET_EHOME_PREVIEW_CB_MSG
{
    BYTE    byDataType;    //NET_DVR_SYSHEAD(1)-码流头，NET_DVR_STREAMDATA(2)-码流数据
    BYTE    byRes1[3];
    void*   pRecvdata;     //码流头或者数据
    DWORD   dwDataLen;     //数据长度
    BYTE    byRes2[128];
}NET_EHOME_PREVIEW_CB_MSG, *LPNET_EHOME_PREVIEW_CB_MSG;

typedef void(CALLBACK *PREVIEW_DATA_CB)(LONG iPreviewHandle, NET_EHOME_PREVIEW_CB_MSG* pPreviewCBMsg, void* pUserData);

typedef struct tagNET_EHOME_NEWLINK_CB_MSG
{
    BYTE    szDeviceID[MAX_DEVICE_ID_LEN];      //设备标示符(出参)
    LONG    iSessionID;                         //设备分配给该取流会话的ID(出参)
    DWORD   dwChannelNo;                        //设备通道号(出参)
    BYTE    byStreamType;                       //0-主码流，1-子码流(出参)
    BYTE    byRes1[2];
    BYTE    byStreamFormat;                     //封装格式：0- PS,1-标准流(入参), 2-原始码流(即设备给什么流获取到的就是什么流)
    char   sDeviceSerial[NET_EHOME_SERIAL_LEN]; //设备序列号，数字序列号(出参)
#if (defined(OS_WINDOWS64) || defined(OS_POSIX64))//win64及linux64下指针为8字节
    PREVIEW_DATA_CB    fnPreviewDataCB;    //数据回调函数(入参)
    void               *pUserData;         //用户参数, 在fnPreviewDataCB回调出来(入参)
#else
    PREVIEW_DATA_CB    fnPreviewDataCB;    //数据回调函数(入参)
    BYTE               byRes2[4];
    void               *pUserData;         //用户参数, 在fnPreviewDataCB回调出来(入参)
    BYTE               byRes3[4];
#endif 
    BYTE               byRes[96];
}NET_EHOME_NEWLINK_CB_MSG, *LPNET_EHOME_NEWLINK_CB_MSG;

typedef BOOL (CALLBACK *PREVIEW_NEWLINK_CB)(LONG lLinkHandle, NET_EHOME_NEWLINK_CB_MSG* pNewLinkCBMsg, void* pUserData);

typedef struct tagNET_EHOME_LISTEN_PREVIEW_CFG
{
    NET_EHOME_IPADDRESS struIPAdress;   //本地监听信息，IP为0.0.0.0的情况下，默认为本地地址，多个网卡的情况下，默认为从操作系统获取到的第一个
    PREVIEW_NEWLINK_CB  fnNewLinkCB;    //预览请求回调函数，当收到预览连接请求后，SDK会回调该回调函数。
    void*               pUser;          // 用户参数，在fnNewLinkCB中返回出来
    BYTE                byLinkMode;     //0：TCP，1：UDP 2: HRUDP方式 8：NPQ方式
	BYTE                byLinkEncrypt;  //是否启用链路加密,TCP通过TLS传输，UDP(包括NPQ)使用DTLS传输，0-不启用，1-启用
    BYTE                byRes[126];
}NET_EHOME_LISTEN_PREVIEW_CFG, *LPNET_EHOME_LISTEN_PREVIEW_CFG;

typedef struct tagNET_EHOME_PREVIEW_DATA_CB_PARAM
{
    PREVIEW_DATA_CB fnPreviewDataCB;    //数据回调函数
    void*           pUserData;          //用户参数, 在fnPreviewDataCB回调出来
    BYTE            byStreamFormat;     //封装格式：0- PS
    BYTE            byRes[127];         //保留
}NET_EHOME_PREVIEW_DATA_CB_PARAM, *LPNET_EHOME_PREVIEW_DATA_CB_PARAM;

typedef enum tagNET_EHOME_ESTREAM_INIT_CFG_TYPE
{
    NET_EHOME_ESTREAM_INIT_CFG_LIBEAY_PATH = 0, //设置OpenSSL的libeay32.dll/libcrypto.so所在路径
    NET_EHOME_ESTREAM_INIT_CFG_SSLEAY_PATH = 1,  //设置OpenSSL的ssleay32.dll/libssl.so所在路径
	NET_EHOME_ESTREAM_INIT_CFG_USERCERTIFICATE_PATH = 2, //设置TLS监听和DTLS监听的服务器证书路径，TLS和DTLS复用同一个证书
	NET_EHOME_ESTREAM_INIT_CFG_USERPRIVATEKEY_PATH = 3 //设置TLS监听和DTLS监听的私钥证书路径，TLS和DTLS复用同一个证书
}NET_EHOME_ESTREAM_INIT_CFG_TYPE;

typedef struct tagNET_EHOME_LOCAL_PLAYBACK_PARAM
{
    DWORD dwSize;
    BYTE  byPlayBackSync;    //是否开启回放同步接收，0-表示使用异步方式，1-表示使用同步方式;
    BYTE  byRes[131];
}NET_EHOME_LOCAL_PLAYBACK_PARAM, *LPNET_EHOME_LOCAL_PLAYBACK_PARAM;

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_Init();

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_Fini();

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_SetSDKInitCfg(NET_EHOME_ESTREAM_INIT_CFG_TYPE enumType, void* const lpInBuff);

NET_DVR_API DWORD CALLBACK NET_ESTREAM_GetLastError();

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_SetExceptionCallBack(DWORD dwMessage, HANDLE hWnd, void (CALLBACK* fExceptionCallBack)(DWORD dwType, LONG iUserID, LONG iHandle, void* pUser), void* pUser);

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_SetLogToFile(LONG iLogLevel, char *strLogDir, BOOL bAutoDel);

NET_DVR_API LONG CALLBACK NET_ESTREAM_SendRealStreamData(LONG lUserID, BYTE byDataType, char *pSendBuf, DWORD dwDataLen);
//获取版本号
NET_DVR_API DWORD CALLBACK NET_ESTREAM_GetBuildVersion();

NET_DVR_API LONG  CALLBACK NET_ESTREAM_StartListenPreview(LPNET_EHOME_LISTEN_PREVIEW_CFG pListenParam);

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_StopListenPreview(LONG iListenHandle);

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_StopPreview(LONG iPreviewHandle);

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_SetPreviewDataCB(LONG iHandle, LPNET_EHOME_PREVIEW_DATA_CB_PARAM pStruCBParam);

NET_DVR_API BOOL  CALLBACK NET_ESTREAM_SetStandardPreviewDataCB(LONG iHandle, LPNET_EHOME_PREVIEW_DATA_CB_PARAM pStruCBParam);

#define NET_EHOME_DEVICEID_LEN        256 //设备ID长度

typedef struct tagNET_EHOME_PLAYBACK_DATA_CB_INFO
{
    DWORD   dwType;     //类型 0-头信息 1-码流数据 15-HLS索引回放 16-延时摄影回放
    BYTE*   pData;      //数据指针
    DWORD   dwDataLen;  //数据长度
    BYTE    byRes[128]; //保留
}NET_EHOME_PLAYBACK_DATA_CB_INFO, *LPNET_EHOME_PLAYBACK_DATA_CB_INFO;

typedef BOOL(CALLBACK *PLAYBACK_DATA_CB)(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_INFO* pDataCBInfo, void* pUserData);

typedef struct tagNET_EHOME_PLAYBACK_NEWLINK_CB_INFO
{
    char         szDeviceID[NET_EHOME_DEVICEID_LEN];   //设备标示符(出参)
    LONG         lSessionID;     //设备分配给该回放会话的ID，0表示无效(出参)
    DWORD        dwChannelNo;    //设备通道号，0表示无效(出参)
    char         sDeviceSerial[NET_EHOME_SERIAL_LEN/*12*/]; //设备序列号，数字序列号(出参)
    BYTE         byStreamFormat;         //码流封装格式：0-PS 1-RTP(入参) 
    BYTE         byRes1[3];
#if (defined(OS_WINDOWS64) || defined(OS_POSIX64))//win64及linux64下指针为8字节
    PLAYBACK_DATA_CB   fnPlayBackDataCB;   //数据回调函数(入参)
    void*              pUserData;         //用户参数, 在fnPlayBackDataCB回调出来(入参)
#else
    PLAYBACK_DATA_CB   fnPlayBackDataCB;   //数据回调函数(入参)
    BYTE               byRes2[4];
    void*              pUserData;         //用户参数, 在fnPlayBackDataCB回调出来(入参)
    BYTE               byRes3[4];
#endif
    BYTE               byRes[88];
}NET_EHOME_PLAYBACK_NEWLINK_CB_INFO, *LPNET_EHOME_PLAYBACK_NEWLINK_CB_INFO;

typedef BOOL (CALLBACK *PLAYBACK_NEWLINK_CB)(LONG lPlayBackLinkHandle, NET_EHOME_PLAYBACK_NEWLINK_CB_INFO* pNewLinkCBInfo, void* pUserData);

typedef struct tagNET_EHOME_PLAYBACK_LISTEN_PARAM
{
    NET_EHOME_IPADDRESS struIPAdress;   //本地监听信息，IP为0.0.0.0的情况下，默认为本地地址，
    //多个网卡的情况下，默认为从操作系统获取到的第一个
    PLAYBACK_NEWLINK_CB fnNewLinkCB;    //回放新连接回调函数
    void*               pUserData;      //用户参数，在fnNewLinkCB中返回出来
    BYTE                byLinkMode;     //0：TCP，1：UDP (UDP保留)
	BYTE                byLinkEncrypt;  //是否启用链路加密,TCP通过TLS传输，UDP(包括NPQ)使用DTLS传输，0-不启用，1-启用
    BYTE                byRes[126];
}NET_EHOME_PLAYBACK_LISTEN_PARAM, *LPNET_EHOME_PLAYBACK_LISTEN_PARAM;


typedef struct tagNET_EHOME_PLAYBACK_DATA_CB_PARAM
{
    PLAYBACK_DATA_CB    fnPlayBackDataCB;   //数据回调函数
    void*               pUserData;          //用户参数, 在fnPlayBackDataCB回调出来
    BYTE                byStreamFormat;     //码流封装格式：0-PS 1-RTP 
    BYTE                byRes[127];         //保留
}NET_EHOME_PLAYBACK_DATA_CB_PARAM, *LPNET_EHOME_PLAYBACK_DATA_CB_PARAM;

#define EHOME_PREVIEW_EXCEPTION     0x102    //预览取流异常
#define EHOME_PLAYBACK_EXCEPTION    0x103    //回放取流异常
#define EHOME_AUDIOTALK_EXCEPTION   0x104    //语音对讲取流异常

#define NET_EHOME_SYSHEAD           1    //系统头数据
#define NET_EHOME_STREAMDATA        2    //视频流数据
#define NET_EHOME_STREAMEND         3    //视频流结束标记
#define NET_EHOME_HLS               15   //HLS回放索引
#define NET_EHOME_TIMELAPSE         16   //延时摄影回放

NET_DVR_API LONG CALLBACK NET_ESTREAM_StartListenPlayBack(LPNET_EHOME_PLAYBACK_LISTEN_PARAM pListenParam);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetPlayBackDataCB(LONG iPlayBackLinkHandle, NET_EHOME_PLAYBACK_DATA_CB_PARAM* pDataCBParam);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopPlayBack(LONG iPlayBackLinkHandle);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopListenPlayBack(LONG iPlaybackListenHandle);

//--------------------------------------------------------------------------------------------------------------
#define NET_EHOME_DEVICEID_LEN      256 //设备ID长度
#define NET_EHOME_SERIAL_LEN        12

typedef struct tagNET_EHOME_VOICETALK_DATA_CB_INFO
{
    BYTE*   pData;          //数据指针
    DWORD   dwDataLen;      //数据长度
    BYTE    byRes[128];     //保留
}NET_EHOME_VOICETALK_DATA_CB_INFO, *LPNET_EHOME_VOICETALK_DATA_CB_INFO;

typedef BOOL(CALLBACK *VOICETALK_DATA_CB)(LONG lHandle, NET_EHOME_VOICETALK_DATA_CB_INFO* pDataCBInfo, void* pUserData);

typedef struct tagNET_EHOME_VOICETALK_NEWLINK_CB_INFO
{
    BYTE    szDeviceID[NET_EHOME_DEVICEID_LEN/*256*/];   //设备标示符(出参)
    DWORD   dwEncodeType; // //SDK赋值,当前对讲设备的语音编码类型,0- G722_1，1-G711U，2-G711A，3-G726，4-AAC，5-MP2L2，6-PCM, 7-MP3, 8-G723, 9-MP1L2, 10-ADPCM, 99-RAW(未识别类型)(出参)
    char    sDeviceSerial[NET_EHOME_SERIAL_LEN/*12*/];    //设备序列号，数字序列号(出参)
    DWORD   dwAudioChan; //对讲通道(出参)
    LONG    lSessionID;  //设备分配给该回放会话的ID，0表示无效(出参)
    BYTE    byToken[64];
#if (defined(OS_WINDOWS64) || defined(OS_POSIX64))//win64及linux64下指针为8字节
    VOICETALK_DATA_CB  fnVoiceTalkDataCB;   //数据回调函数(入参)
    void               *pUserData;         //用户参数, 在fnVoiceTalkDataCB回调出来(入参)
#else
    VOICETALK_DATA_CB  fnVoiceTalkDataCB;   //数据回调函数(入参)
    BYTE               byRes1[4];
    void               *pUserData;         //用户参数, 在fnVoiceTalkDataCB回调出来(入参)
    BYTE               byRes2[4];
#endif 
    BYTE               byRes[48];
} NET_EHOME_VOICETALK_NEWLINK_CB_INFO, *LPNET_EHOME_VOICETALK_NEWLINK_CB_INFO;

typedef BOOL (CALLBACK *VOICETALK_NEWLINK_CB)(LONG lHandle, NET_EHOME_VOICETALK_NEWLINK_CB_INFO* pNewLinkCBInfo, void* pUserData);

typedef struct tagNET_EHOME_LISTEN_VOICETALK_CFG
{
    NET_EHOME_IPADDRESS struIPAdress;       //本地监听信息，IP为0.0.0.0的情况下，默认为本地地址，
    //多个网卡的情况下，默认为从操作系统获取到的第一个
    VOICETALK_NEWLINK_CB     fnNewLinkCB;   //新连接回调函数
    void*          pUser;                   //用户参数，在fnNewLinkCB中返回出来
	BYTE           byLinkMode;     //0：TCP，1：UDP (UDP保留)
	BYTE           byLinkEncrypt;  //是否启用链路加密,TCP通过TLS传输，UDP(包括NPQ)使用DTLS传输，0-不启用，1-启用
    BYTE           byRes[126];
}NET_EHOME_LISTEN_VOICETALK_CFG, *LPNET_EHOME_LISTEN_VOICETALK_CFG;

typedef struct tagNET_EHOME_VOICETALK_DATA_CB_PARAM
{
    VOICETALK_DATA_CB   fnVoiceTalkDataCB;  //数据回调函数
    void*   pUserData;  //用户参数, 在fnVoiceTalkDataCB回调出来
    BYTE    byRes[128]; //保留
}NET_EHOME_VOICETALK_DATA_CB_PARAM, *LPNET_EHOME_VOICETALK_DATA_CB_PARAM;

typedef struct tagNET_EHOME_VOICETALK_DATA
{
    BYTE*   pSendBuf;       //音频数据缓冲区
    DWORD   dwDataLen;      //音频数据长度
    DWORD   dwTimeout;            //数据发送超时时间，单位：毫秒；0-默认（5000ms）
    BYTE    byRes[124];           //保留
}NET_EHOME_VOICETALK_DATA, *LPNET_EHOME_VOICETALK_DATA;

NET_DVR_API LONG CALLBACK NET_ESTREAM_StartListenVoiceTalk(LPNET_EHOME_LISTEN_VOICETALK_CFG pListenParam);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopListenVoiceTalk(LONG lListenHandle);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetVoiceTalkDataCB(LONG lHandle, LPNET_EHOME_VOICETALK_DATA_CB_PARAM pStruCBParam);
NET_DVR_API LONG CALLBACK NET_ESTREAM_SendVoiceTalkData (LONG lHandle, LPNET_EHOME_VOICETALK_DATA pVoicTalkData);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_StopVoiceTalk(LONG lHandle);

NET_DVR_API BOOL CALLBACK NET_ESTREAM_SetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE enumType, void* const lpInBuff);
NET_DVR_API BOOL CALLBACK NET_ESTREAM_GetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE enumType, void* lpOutBuff);


#endif //_HC_EHOME_STREAM_H_
