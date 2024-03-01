/**********************************************************
FileName:   GlobalDefinition.h
Description: demo and struct define for EHomeDemo      
Date:        2014/12/27
Author:    zhaolin
Modification History:      
    <version>            <time>                    <desc>
    <1.0>                    <2014/12/27>            <created>
***********************************************************/
#ifndef _GLOBAL_DEFINITION_H_
#define _GLOBAL_DEFINITION_H_
#include "HCISUPCMS.h"

#define MAX_DEVICES            1024    //max device number
#define MAX_CHAN_NUM_DEMO    256    //max channels

#define DEMO_FULL_WIDTH 1024    //default demo width
#define DEMO_FULL_HEIGHT 768    //default demo height
#define OUTPUT_INTERVAL 4        //interval between each display window
#define LIST_ENLARGE_HIGH 575    //enlarge height of    local log or alarm list

#define TREE_ALL             0    //device list    
#define DEVICE_LOGOUT         1   //device not log in
#define DEVICE_LOGIN         2   //devic3e login
#define DEVICE_FORTIFY         3   //on guard
#define DEVICE_ALARM         4   //alarm on device
#define DEVICE_FORTIFY_ALARM 5    //onguard & alarm on device

#define CHAN_ORIGINAL         6   //no preview, no record
#define CHAN_PLAY             7   //preview
#define CHAN_RECORD           8   //record
#define CHAN_PLAY_RECORD      9   //preview and record

#define CHAN_ALARM              10   //no preview, no record, only alarm
#define CHAN_PLAY_ALARM             11   //review, no record, with alarm info
#define CHAN_PLAY_RECORD_ALARM     12   //preview & record & alarm
#define CHAN_OFF_LINE             13     //channel off-line

//device list config
#define TREE_ALL_T     0        //root node
#define DEVICETYPE      1          //device
#define CHANNELTYPE     2          //channel    

//demo 消息
#define WM_ADD_LOG                        WM_USER + 1    //add log 
#define WM_ADD_DEV                        WM_USER + 2 //add device
#define WM_DEL_DEV                        WM_USER + 3 //delete device
#define WM_CHANGE_CHANNEL_ITEM_IMAGE    WM_USER + 4     //change channel node icon
#define WM_PROC_EXCEPTION                WM_USER + 5    //process exception
#define WM_CHANGE_IP                        WM_USER + 6 //ip address changed
#define WM_HTTP_ALARM                      WM_USER + 7
#define WM_ASYNC_RESP            WM_USER + 8
#define WM_VEDIO_INTERCOME_CMD   WM_USER + 9  //video intercome command
#define WM_DEL_OFFLINEDEV       WM_USER + 10 //for version5, delete offline device
#define WM_SETPREVIEW           WM_USER + 11 //SetPreviewDataCB
#define WM_DEV_SLEEP            WM_USER + 12 //device sleep

//local log type
#define ALARM_INFO_T        0    //alarm
#define OPERATION_SUCC_T    1    //operation succeed
#define OPERATION_FAIL_T    2    //operation fail
#define PLAY_SUCC_T            3    //player succeed
#define PLAY_FAIL_T            4    //player fail

//主界面模式
#define PREVIEW_T        0
#define PLAY_BACK_T        1
#define AUDIO_TALK_T    2
#define COM_CONFIG_T    3
#define LOCAL_CONFIG_T 4

#define MAX_OUTPUTS            512 //预览画面数
#define MAX_LISTEN_NUM      10  //最大监听个数


#define SYSTEM_TIMER            1        //timer for system parameter,cpu
#define CYCLE_PREVIEW_TIMER        2        //switch display timer

enum DEMO_CHANNEL_TYPE
{
    DEMO_CHANNEL_TYPE_INVALID = -1,
    DEMO_CHANNEL_TYPE_ANALOG  = 0,
    DEMO_CHANNEL_TYPE_IP = 1,
    DEMO_CHANNEL_TYPE_ZERO //零通道
};

typedef struct tagLISTEN_INFO
{
    NET_EHOME_IPADDRESS struIP;
    int                iLinkType;  //0-TCP , 1-UDP
    LONG               lHandle;
}LISTEN_INFO, *LPLISTEN_INFO;

typedef struct tagSTRU_CHANNEL_INFO
{ 
    int     iDeviceIndex;        //device index
    int     iChanIndex;            //channel index
    //int     iSessionID;
    DWORD   dwStreamType;       //码流类型，0-主码流，1-子码流，2-码流3，3- 语音监听
    DWORD   dwLinkMode;         //码流连接方式: 0：TCP方式,1：UDP方式,8-NPQ
    //LONG    lPreviewHandle;     //preview handle
    DEMO_CHANNEL_TYPE iChanType;
    int     iChannelNO;
    BOOL    bEnable;            //enable
    DWORD    dwImageType;
    NET_EHOME_IPADDRESS struIP;
    int     iPlayWndIndex;          //播放窗口,通过这个参数，来找到对应的CDlgOutput对象
    BOOL    bPlay;
    DWORD   dwPreviewSessionId;   //是否加密
    DWORD   dwStreamFormat;
    BYTE    byEncrypt;            //enable
    tagSTRU_CHANNEL_INFO()
    {
        iDeviceIndex        = -1;
        iChanIndex          = -1;
        //iSessionID          = -1;
        dwStreamType        = 0;
        dwLinkMode          = 0;
        //lPreviewHandle      = -1;
        iChannelNO          = -1;
        bEnable             = FALSE;
        iChanType           = DEMO_CHANNEL_TYPE_INVALID;
        dwImageType         = CHAN_ORIGINAL;
        memset(struIP.szIP, 0, sizeof(struIP.szIP));
        memcpy(struIP.szIP, "10.10.34.17", strlen("10.10.34.17"));
        struIP.wPort = 8000;
        iPlayWndIndex = -1;
        bPlay = FALSE;

        dwPreviewSessionId = 0;
        dwStreamFormat = 1; //默认标准流
        byEncrypt = FALSE;
    }
}STRU_CHANNEL_INFO, *LPSTRU_CHANNEL_INFO;

typedef struct tagLOCAL_DEVICE_INFO
{
    int     iDeviceIndex;
    LONG    lLoginID;                //ID
    BYTE    byDeviceID[256];
    BYTE    byPassword[32];
    BYTE    byFirmwareVersion[64];
    BYTE    byDevLocalIP[128];
    WORD    wDevLocalPort;
    WORD    wManuFacture;
    DWORD   dwDevType;
    BYTE    bySerialNumber[50];        //SN
    DWORD   dwDeviceChanNum;            //channel numder  (analog + ip)
    DWORD   dwStartChan;                //start channel number    
    DWORD   dwDiskNum;                //HD number    
    DWORD   dwAlarmInNum;            //alarm in number
    DWORD   dwAlarmOutNum;            //alarm out number
    DWORD   dwAudioNum;                //voice talk number
    DWORD   dwAnalogChanNum;            //analog channel number
    DWORD   dwIPChanNum;                //IP channel number
    DWORD   dwZeroChanNum;              //零通道个数
    DWORD   dwZeroChanStart;            //零通道起始个数
    BYTE    sDeviceSerial[NET_EHOME_SERIAL_LEN/*12*/];    //设备序列号，数字序列号
    BYTE    sIdentifyCode[8];    //设备验证码，出厂时，设备固件中写入的随机码。默认为abcdef
    BOOL    bPlayDevice;
    DWORD   dwVersion;
    STRU_CHANNEL_INFO struChanInfo[MAX_CHAN_NUM_DEMO];
    BYTE    byEhomeKey[32];
    BYTE    bySessionKey[16];
    BYTE    byClouldSecretKey[64];
    BYTE    byDeviceFullSerial[MAX_FULL_SERIAL_NUM_LEN];   //设备完整序列号，设备型号+日期+短序号+扩展
    BYTE   byStartDTalkChan;    //起始数字对讲通道号，区别于模拟对讲通道号，0表示无数字对讲通道
public:
    tagLOCAL_DEVICE_INFO()
    {
        iDeviceIndex = -1;
        lLoginID = -1;                
        memset(byDeviceID, 0, sizeof(byDeviceID));
        memset(byPassword, 0, sizeof(byPassword));
        memset(byFirmwareVersion, 0, sizeof(byFirmwareVersion));
        memset(byDevLocalIP, 0, sizeof(byDevLocalIP));
        wDevLocalPort = 0;
        wManuFacture = 0;
        dwDevType = 0;
        memset(bySerialNumber, 0, sizeof(bySerialNumber));
        dwDeviceChanNum = 0;
        dwStartChan = 0;
        dwDiskNum = 0;
        dwAlarmInNum = 0;
        dwAlarmOutNum = 0;
        dwAudioNum = 0;
        dwAnalogChanNum = 0;
        dwIPChanNum = 0;
        bPlayDevice = FALSE;
        byStartDTalkChan = 0;
        memset(sDeviceSerial, 0, sizeof(sDeviceSerial));
        memset(byDeviceFullSerial, 0, sizeof(byDeviceFullSerial));

        memset(sIdentifyCode, 0, sizeof(sIdentifyCode));

        memset(byEhomeKey, 0, sizeof(byEhomeKey));
        memset(bySessionKey, 0, sizeof(bySessionKey));
        memset(byClouldSecretKey, 0, sizeof(byClouldSecretKey));
    }
}LOCAL_DEVICE_INFO, *LPLOCAL_DEVICE_INFO;

typedef struct tagLOCAL_PARAM
{
    BOOL        bSuccLog;                //display log success on log list
    BOOL        bFailLog;                //display filaure operation on log list
    BOOL        bOutputDebugString;
    BOOL        bEnlarged;
    BOOL        bFullScrn;
    tagLOCAL_PARAM()
    {
        bSuccLog        = TRUE;
        bFailLog            = TRUE;
        bOutputDebugString = TRUE;
    }

}LOCAL_PARAM;

typedef struct tagLOCAL_LOG_INFO 
{
    int iLogType;
    char szTime[64];
    char szLogInfo[512];
    char szDevInfo[128];
    char szErrInfo[256];
    tagLOCAL_LOG_INFO ()
    {
        iLogType = 0;
        szTime[0] = '\0';
        szLogInfo[0] = '\0';
        szDevInfo[0] = '\0';
        szErrInfo[0] = '\0';
    }
}LOCAL_LOG_INFO, *LPLOCAL_LOG_INFO;

#endif    //_GLOBAL_DEFINITION_H_