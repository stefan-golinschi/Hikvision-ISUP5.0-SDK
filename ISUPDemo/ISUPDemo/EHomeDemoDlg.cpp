
// EHomeDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "EHomeDemoDlg.h"
#include "math.h"

#include "HCISUPCMS.h"
#include "HCISUPAlarm.h"
#include "HCISUPSS.h"
#include "DlgListen.h"
#include "DlgChanInfo.h"
#include "DlgPassthroughProxy.h"
#include "DlgGpsInfo.h"
#include "DlgWirelessInfoPara.h"
#include "Public/tinyXML/XmlBase.h"
#include "Public/Convert/Convert.h"
#include "public/cjson/cJson.h"


#include "DlgIFrame.h"
#include "DlgUpgradeHttp.h"
#include "DlgISAPIConfig.h"
#include "DlgISAPIPassthrough.h"
#include "DlgUpgrade.h"
#include "DlgMotionConfig.h"
#include "DlgRecordCfg.h"
#include "DlgCaptureCfg.h"
#include "DlgMotionArea.h"
#include "DlgPrivateArea.h"
#include "DlgHidArea.h"
#include "DlgEhome50Auth.h"
#include "DlgStreamEncrypt.h"
#include "DlgCheckStreamKey.h"
#include "DlgHttpAlarm.h"
#include "DlgStorageServer.h"
#include "Language.h"
#include "DlgWebConfig.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GET_SUB_FOUR_VERSION(dwBuildVersion) (0xff000000 & (dwBuildVersion))>>24, (0x00ff0000 & (dwBuildVersion))>>16,\
        (0x0000ff00 & (dwBuildVersion))>>8, (0x000000ff & (dwBuildVersion))

#define ISUP_DEMO_CONFIG_XML ".\\EHomeDemo.xml"

#pragma warning (disable: 4996)
void CALLBACK fnPassthroughDataCb(DWORD dwProxyType, LONG lLisetenHandle, void* pDeviceID, DWORD dwDevIDLen, void* pDataBuffer, DWORD dwDataLen, void* pUser)
{
    UN_REFERENCED_PARAMETER(dwProxyType)
    UN_REFERENCED_PARAMETER(lLisetenHandle)
    UN_REFERENCED_PARAMETER(dwDevIDLen)
    UN_REFERENCED_PARAMETER(pDataBuffer)
    UN_REFERENCED_PARAMETER(dwDataLen)
    UN_REFERENCED_PARAMETER(pUser)
	char sz[1024] = {0};
	sprintf(sz, "deviceID[%s]", pDeviceID);
	return;
}


//CDlgOutput* m_pDlgPreview;
LOCAL_DEVICE_INFO g_struDeviceInfo[MAX_DEVICES];
CEHomeDemoDlg *g_pMainDlg = NULL;
BOOL g_bExitDemo = FALSE;//control post message when exit, so can release the buffer.
LOCAL_PARAM g_struLocalParam;        //demo local configure
LISTEN_INFO g_struPreviewListen[MAX_LISTEN_NUM];
BOOL g_bArrayEncrypt[MAX_LISTEN_NUM]; //预览监听加密状态
int g_pCycleTimer;
BOOL g_bTCPLink = TRUE;
LONG g_lCmsAlarm = -1;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
int g_iCurScreenWidth = 0;        //current screen width
int g_iCurScreenHeight = 0;       //current screen height
void* g_pHttpAlarm;
int g_dwHttpAlarmLen;

void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn);

int g_iServerType = 0;//服务器类型：0-LBS&DAS 1-LBS 2-DAS

BOOL g_bSS_Enable = TRUE;//是否启用图片服务器
BOOL g_bSS_EnableError = FALSE; //是否启用图片服务器错误码测试
int g_iSSErrorCode = 0;
const char* g_cmdString[] = { "", "request", "cancel", "answer", "reject", "bellTimeout", "hangUp", "deviceOnCall" };
/*********************************************************
  Function:    g_StringLanType
  Desc:        get the current operation language string type
  Input:    szLanCn, Chinese string; szLanEn, English string;
  Output:    szDstLan, current string
  Return:    none
**********************************************************/
void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn)
{
        UN_REFERENCED_PARAMETER(szLanEn)
#ifdef DEMO_LAN_CN
        sprintf(szDstLan, "%s", szLanCn);    
#else    
        sprintf(szDstLan, "%s", szLanEn);
#endif    
}

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// 对话框数据
    enum { IDD = IDD_ABOUTBOX };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEHomeDemoDlg 对话框
#define SS_PATH "C:\\StorageServer"
#define SS_STORAGE_PATH "C:\\StorageServer\\Storage"
#define SS_MESSAGE_PATH "C:\\StorageServer\\Message"
#define SS_RW_PATH "C:\\StorageServer\\Rw"
#define SS_RW_PATH_EX "C:\\StorageServer\\RwEx"
#define PIC_URI_LEN 128


BOOL _stdcall SS_Message_Callback(LONG iHandle, NET_EHOME_SS_MSG_TYPE enumType
    , void *pOutBuffer, DWORD dwOutLen, void *pInBuffer, DWORD dwInLen, void *pUser)
{
    UN_REFERENCED_PARAMETER(pUser);
    UN_REFERENCED_PARAMETER(dwOutLen);
    UN_REFERENCED_PARAMETER(iHandle);
    if (enumType == NET_EHOME_SS_MSG_TOMCAT)
    {
        LPNET_EHOME_SS_TOMCAT_MSG pTomcatMsg = (LPNET_EHOME_SS_TOMCAT_MSG)pOutBuffer;
        char szPicUri[PIC_URI_LEN * 5 + 1] = { 0 };
        for (int i = 0; i < (int)pTomcatMsg->dwPicNum; i++)
        {
            sprintf_s(szPicUri + i * PIC_URI_LEN, PIC_URI_LEN, "%s"
                , pTomcatMsg->pPicURLs + (i * MAX_URL_LEN_SS));
        }
        char szUrlHead[PIC_URI_LEN + 1] = { 0 };
        memcpy(szUrlHead, pTomcatMsg->szDevUri, 128);
        int picNum = pTomcatMsg->dwPicNum;
        char szMsg[5 * PIC_URI_LEN + 1] = { 0 };
        sprintf_s(szMsg, 5 * PIC_URI_LEN, "tomcat:uri[%s],picNum:[%d] ,picInfo:[%s]"
            , szUrlHead, picNum, szPicUri);
        OutputDebugString(szMsg);

        if (!PathIsDirectory(SS_PATH))
        {
            if (!CreateDirectory(SS_PATH, NULL))
            {
                return FALSE;
            }
        }
        if (!PathIsDirectory(SS_MESSAGE_PATH))
        {
            if (!CreateDirectory(SS_MESSAGE_PATH, NULL))
            {
                return FALSE;
            }
        }
        char strFilePath[MAX_PATH] = { 0 };
        sprintf_s(strFilePath, MAX_PATH - 1, "%s\\tomcatOutput.txt", SS_MESSAGE_PATH);
        FILE* pFile = fopen(strFilePath, "w+");

        if (pFile != NULL)
        {
            for (int i = 0; i < (int)pTomcatMsg->dwPicNum; i++)
            {
                fwrite(szPicUri + i * PIC_URI_LEN, 1, strlen(szPicUri + i * PIC_URI_LEN), pFile);
                fwrite("\n", 1, 1, pFile);
            }
            fclose(pFile);
        }
        //AfxMessageBox(szMsg);
    }
    else if (enumType == NET_EHOME_SS_MSG_KMS_USER_PWD)
    {
        //*(BOOL*)pInBuffer = 1;
    }
    else if (enumType == NET_EHOME_SS_MSG_CLOUD_AK)
    {
        //strncpy((char*)pInBuffer, "5e998dbbafb44ca783099afcdead40fa7A3Vf7Fh", dwInLen);
        //return TRUE;
        char temp[128] = { 0 };
        memset(temp, 0, 128);
        if (dwOutLen > 128)
        {
            dwOutLen = 128;
        }
        memcpy(temp, pOutBuffer, dwOutLen);
        CEHomeDemoDlg* pDlg = static_cast<CEHomeDemoDlg*>(pUser);
        for (int i = 0; i < MAX_DEVICES; i++)
        {
            if (g_struDeviceInfo[i].iDeviceIndex != -1)
            {
                if (strncmp((char*)g_struDeviceInfo[i].byDeviceID, temp, 64) == 0)
                {
                    strncpy((char*)pInBuffer, (char*)g_struDeviceInfo[i].byClouldSecretKey
                        , strlen((char*)g_struDeviceInfo[i].byClouldSecretKey));
                    break;
                }
            }
        }
    }
    return TRUE;
}

BOOL _stdcall SS_Storage_Callback(LONG iHandle, const char* pFileName, void *pFileBuf, DWORD dwFileLen, char *pFilePath, void *pUser)
{
    UN_REFERENCED_PARAMETER(iHandle);
    UN_REFERENCED_PARAMETER(pUser);
    if (pFileName == NULL || pFileBuf == NULL || dwFileLen == 0)
    {
        return FALSE;
    }
    if (!PathIsDirectory(SS_PATH))
    {
        if (!CreateDirectory(SS_PATH, NULL))
        {
            return FALSE;
        }
    }
    if (!PathIsDirectory(SS_STORAGE_PATH))
    {
        if (!CreateDirectory(SS_STORAGE_PATH, NULL))
        {
            return FALSE;
        }
    }
    char strFilePath[MAX_PATH] = { 0 };
    CString strFilePathTmp(pFileName);
    strFilePathTmp.Replace(':', '_');
    sprintf_s(strFilePath, MAX_PATH - 1, "%s\\%s", SS_STORAGE_PATH, strFilePathTmp.GetBuffer());

    FILE* pFile = fopen(strFilePath, "wb+");
    if (pFile == NULL)
    {

        return FALSE;
    }

    DWORD dwWriteLen = fwrite(pFileBuf, 1, dwFileLen, pFile);

    fclose(pFile);
    if (dwWriteLen != dwFileLen)
    {
        return FALSE;
    }

    strncpy(pFilePath, strFilePath, MAX_PATH - 1);
    return TRUE;
}

BOOL _stdcall SS_RW_Callback(LONG iHandle, BYTE byAct, const char* pFileName, void *pFileBuf, LONG* dwFileLen, const char* pFileUrl, void *pUser)
{
    //Todo
    UN_REFERENCED_PARAMETER(iHandle);
    UN_REFERENCED_PARAMETER(pUser);
    if (pFileUrl == NULL || pUser == NULL)
    {
        return FALSE;
    }
    if (!PathIsDirectory(SS_RW_PATH))
    {
        if (!CreateDirectory(SS_RW_PATH, NULL))
        {
            return FALSE;
        }
    }
    if (!PathIsDirectory(SS_RW_PATH_EX))
    {
        if (!CreateDirectory(SS_RW_PATH_EX, NULL))
        {
            return FALSE;
        }
    }
    if (0 == byAct)
    {

        char strFilePath[MAX_PATH] = { 0 };
        //CString strFilePathTmp(pFileName);
        //strFilePathTmp.Replace(':', '_');
        sprintf_s(strFilePath, MAX_PATH - 1, "%s\\cgy_test.jpg", SS_RW_PATH);


        FILE* pFile = fopen(strFilePath, "wb+");
        if (pFile == NULL)
        {

            return FALSE;
        }

        DWORD dwWriteLen = fwrite(pFileBuf, 1, (unsigned int)*dwFileLen, pFile);

        fclose(pFile);
        /*if (dwWriteLen != (DWORD)dwFileLen)
        {
        return FALSE;
        }*/

        //strncpy(pFilePath, strFilePath, MAX_PATH - 1);
        return TRUE;
    }
    else if (1 == byAct)
    {
        if (pFileBuf == NULL) //第一次回调
        {

            char strFilePath[MAX_PATH] = { 0 };
            sprintf_s(strFilePath, MAX_PATH - 1, "%s\\cgy_test.jpg", SS_RW_PATH);

            FILE* pFile = fopen(strFilePath, "rb+");
            if (pFile == NULL)
            {
                return FALSE;
            }

            fseek(pFile, 0, SEEK_END);
            int iFileLen = ftell(pFile);
            *dwFileLen = iFileLen;  //测试文件大小
            fclose(pFile);
        }
        else if (pFileBuf != NULL) //第二次回调
        {
            char strFilePath[MAX_PATH] = { 0 };
            sprintf_s(strFilePath, MAX_PATH - 1, "%s\\cgy_test.jpg", SS_RW_PATH);

            FILE* pFile = fopen(strFilePath, "rb+");
            if (pFile == NULL)
            {
                return FALSE;
            }

            DWORD dwWriteLen = fread(pFileBuf, 1, (unsigned int)*dwFileLen, pFile);

            if (dwWriteLen != (DWORD)*dwFileLen)
            {
                fclose(pFile);
                return FALSE;
            }

            fclose(pFile);
        }
        return TRUE;
    }
    else if (2 == byAct)
    {
        char strFilePath[MAX_PATH] = { 0 };
        sprintf_s(strFilePath, MAX_PATH - 1, "%s\\cgy_test.jpg", SS_RW_PATH);
        //删除文件
        if (remove(strFilePath) != 0)
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL _stdcall SS_RW_CallbackEx(LONG iHandle, NET_EHOME_SS_RW_PARAM* pStruRwParam, NET_EHOME_SS_EX_PARAM* pExStruct)
{
    UN_REFERENCED_PARAMETER(iHandle);
    UN_REFERENCED_PARAMETER(pStruRwParam->pUser);
    
    if (!PathIsDirectory(SS_PATH))
    {
        if (!CreateDirectory(SS_PATH, NULL))
        {
            return FALSE;
        }
    }
    if (!PathIsDirectory(SS_RW_PATH_EX))
    {
        if (!CreateDirectory(SS_RW_PATH_EX, NULL))
        {
            return FALSE;
        }
    }
    if (g_bSS_EnableError)
    {
        pExStruct->unionStoreInfo.struCloud.dwErrorCode = g_iSSErrorCode;
        return FALSE;
    }

    if (0 == pStruRwParam->byAct) //客户端上传文件
    {
        if (pStruRwParam->pFileName == NULL || pStruRwParam->pFileBuf == NULL || pStruRwParam->dwFileLen == 0)
        {
            return FALSE;
        }
        NET_EHOME_SS_EX_PARAM* pExstru = (NET_EHOME_SS_EX_PARAM*)pExStruct;
        if (pExstru->byProtoType == 1)//表示联合体中使用的结构体是云存储结构体
        {
            char strPoolId[33] = { 0 };
            memcpy(strPoolId, pExstru->unionStoreInfo.struCloud.pPoolId, pExstru->unionStoreInfo.struCloud.byPoolIdLength);
        }

        char strFilePath[MAX_PATH] = { 0 };
        CString strFilePathTmp(pStruRwParam->pFileName);
        strFilePathTmp.Replace(':', '_');
        sprintf_s(strFilePath, MAX_PATH - 1, "%s\\%s", SS_RW_PATH_EX, strFilePathTmp.GetBuffer());

        FILE* pFile = fopen(strFilePath, "wb+");
        if (pFile == NULL)
        {

            return FALSE;
        }

        DWORD dwWriteLen = fwrite(pStruRwParam->pFileBuf, 1, (unsigned int)*pStruRwParam->dwFileLen, pFile);

        fclose(pFile);
        if (dwWriteLen != (DWORD)*pStruRwParam->dwFileLen)
        {
            return FALSE;
        }

        //test for url index
        char strRetIndex[256] = { 0 };
        sprintf_s(strRetIndex, 255, "hubinglun_test_for_Return_Index_%s", pStruRwParam->pFileName);
        pStruRwParam->byUseRetIndex = 1;
        strncpy(pStruRwParam->pRetIndex, strRetIndex, 256);

        return TRUE;
    }
    else if (1 == pStruRwParam->byAct) //客户端下载文件
    {
        if (pStruRwParam->pFileName == NULL)
        {
            return FALSE;
        }
        if (pStruRwParam->pFileBuf == NULL) //第一次回调
        {
            char strFilePath[MAX_PATH] = { 0 };

            CString strFilePathTmp = CString(pStruRwParam->pFileUrl); //测试文件
            int iPos = strFilePathTmp.Find("?");
            if (iPos != -1)
            {
                strFilePathTmp = strFilePathTmp.Left(iPos);
            }

            iPos = strFilePathTmp.Find("hubinglun_test_for_Return_Index_");
            if (iPos != -1)
            {
                strFilePathTmp = strFilePathTmp.Right(strFilePathTmp.GetLength() - iPos - strlen("hubinglun_test_for_Return_Index_"));
            }
            //strFilePathTmp.Replace(':', '_');
            sprintf_s(strFilePath, MAX_PATH - 1, "%s\\%s", SS_RW_PATH_EX, strFilePathTmp.GetBuffer());

            FILE* pFile = fopen(strFilePath, "rb+");
            if (pFile == NULL)
            {
                return FALSE;
            }

            fseek(pFile, 0, SEEK_END);
            int iFileLen = ftell(pFile);
            *pStruRwParam->dwFileLen = iFileLen;  //测试文件大小
            fclose(pFile);
        }
        else if (pStruRwParam->pFileBuf != NULL) //第二次回调
        {
            char strFilePath[MAX_PATH] = { 0 };
            CString strFilePathTmp = CString(pStruRwParam->pFileUrl); //测试文件
            int iPos = strFilePathTmp.Find("?");
            if (iPos != -1)
            {
                strFilePathTmp = strFilePathTmp.Left(iPos);
            }

            iPos = strFilePathTmp.Find("hubinglun_test_for_Return_Index_");
            if (iPos != -1)
            {
                strFilePathTmp = strFilePathTmp.Right(strFilePathTmp.GetLength() - iPos - strlen("hubinglun_test_for_Return_Index_"));
            }
            sprintf_s(strFilePath, MAX_PATH - 1, "%s\\%s", SS_RW_PATH_EX, strFilePathTmp.GetBuffer());

            FILE* pFile = fopen(strFilePath, "rb+");
            if (pFile == NULL)
            {
                return FALSE;
            }

            DWORD dwWriteLen = fread(pStruRwParam->pFileBuf, 1, (unsigned int)*pStruRwParam->dwFileLen, pFile);

            if (dwWriteLen != (DWORD)*pStruRwParam->dwFileLen)
            {
                fclose(pFile);
                return FALSE;
            }

            fclose(pFile);
        }
        return TRUE;
    }
    else if (2 == pStruRwParam->byAct)
    {
        char strFilePath[MAX_PATH] = { 0 };
        char pFileData[MAX_PATH] = { 0 };
        CString strFilePathTmp = CString(pStruRwParam->pFileUrl); //测试文件
        int iPos = strFilePathTmp.Find("?");
        if (iPos != -1)
        {
            strFilePathTmp = strFilePathTmp.Left(iPos);
        }

        iPos = strFilePathTmp.Find("hubinglun_test_for_Return_Index_");
        if (iPos != -1)
        {
            strFilePathTmp = strFilePathTmp.Right(strFilePathTmp.GetLength() - iPos - strlen("hubinglun_test_for_Return_Index_"));
        }
        sprintf_s(strFilePath, MAX_PATH - 1, "%s\\%s", SS_RW_PATH_EX, strFilePathTmp.GetBuffer());
        //删除文件
        if (remove(strFilePath) != 0)
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

BOOL _stdcall EHOME_REGISTER(LONG iUserID, DWORD dwDataType, void *pOutBuffer, DWORD dwOutLen, void *pInBuffer, DWORD dwInLen, void *pUser)
{
    //return FALSE;
    UN_REFERENCED_PARAMETER(dwInLen)
    UN_REFERENCED_PARAMETER(dwOutLen)
    CEHomeDemoDlg *pDlg = static_cast<CEHomeDemoDlg *>(pUser);

    if (NULL == pDlg || g_bExitDemo == TRUE)
    {
        return FALSE;
    }

    if (ENUM_DEV_ON == dwDataType)
    {
        if (pInBuffer == NULL)
        {
            return FALSE;
        }
        LOCAL_DEVICE_INFO *lpTemp = new LOCAL_DEVICE_INFO();
        NET_EHOME_DEV_REG_INFO_V12 *pDevInfo = (NET_EHOME_DEV_REG_INFO_V12 *)pOutBuffer;

        //UTF-8转GBK
        DWORD dwOutLenTemp = 0;
        UTF82A((char*)pDevInfo->struRegInfo.byDeviceID, (char*)pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN, &dwOutLenTemp);
        UTF82A((char*)pDevInfo->struRegInfo.sDeviceSerial, (char*)pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN, &dwOutLenTemp);

        if (pDevInfo != NULL)
        {
            memcpy(lpTemp->byDeviceID, pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN);
            lpTemp->lLoginID = iUserID;
            memcpy(lpTemp->sDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN);
            //memcpy(lpTemp->sIdentifyCode, pDevInfo->struRegInfo.sIdentifyCode, CODE_LEN);
        }
        lpTemp->lLoginID = iUserID;

        //先拷贝到一个大的缓冲区，在获取长度
        char szDeviceSerial[NET_EHOME_SERIAL_LEN+1] = {0};
        memcpy(szDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN);
        char szSessionKey[MAX_MASTER_KEY_LEN + 1] = { 0 };
        memcpy(szSessionKey, pDevInfo->struRegInfo.bySessionKey, MAX_MASTER_KEY_LEN);
        if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "2", 1) == 0)
        {
            lpTemp->dwVersion = 2;
        }
        else if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "4", 1) == 0)
        {
            lpTemp->dwVersion = 4;
        }
        else
        {
            lpTemp->dwVersion = 5;
        }

        NET_EHOME_SERVER_INFO_V50 *pServInfo = (NET_EHOME_SERVER_INFO_V50 *)pInBuffer;
        memcpy(pServInfo, &pDlg->m_struServInfo, sizeof(pDlg->m_struServInfo));
        pDlg->GetAddressByType(0, lpTemp->dwVersion, (char *)pServInfo, sizeof(NET_EHOME_SERVER_INFO_V50), NULL, 0);

        if (g_bSS_Enable)//启用EHome5.0存储服务器
        {
            for (int i = 0; i < MAX_DEVICES; i++)
            {
                //if (g_struDeviceInfo[i].iDeviceIndex != -1) //重注册上线时，DeviceIndex = -1
                {
                    if (memcmp(g_struDeviceInfo[i].byDeviceID, pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN) == 0)
                    {
                        pServInfo->byClouldHttps = pDlg->m_dlgLocalCfg->m_bHttps;
                        if (pDlg->m_dlgLocalCfg->m_strAK.GetLength() > 0 && pDlg->m_dlgLocalCfg->m_strSK.GetLength() > 0)
                        {
                            memcpy(pServInfo->byClouldAccessKey, pDlg->m_dlgLocalCfg->m_strAK.GetBuffer(0), pDlg->m_dlgLocalCfg->m_strAK.GetLength());
                            memcpy(pServInfo->byClouldSecretKey, pDlg->m_dlgLocalCfg->m_strSK.GetBuffer(0), pDlg->m_dlgLocalCfg->m_strSK.GetLength());
                        }
                        else
                        {
                            memcpy(pServInfo->byClouldAccessKey, pDevInfo->struRegInfo.byDeviceID, 64);
                            memcpy(pServInfo->byClouldSecretKey, g_struDeviceInfo[i].byClouldSecretKey, 64);
                        }

                        if (pDlg->m_dlgLocalCfg->m_iPoolID != 0)
                        {
                            pServInfo->dwClouldPoolId = pDlg->m_dlgLocalCfg->m_iPoolID;
                        }
                        else
                        {
                            pServInfo->dwClouldPoolId = 1;
                        }
                        break;
                    }
                }
            }
        }
        
        //报警端口在映射环境下会被修改为监听端口
        //if (lpTemp->dwVersion == 5)//Ehome5.0协议报警端口复用TCP端口
        //{
        //    pServInfo->struTCPAlarmSever.wPort = pDlg->m_wAlarmServerMqttPort;
        //}

        pDlg->m_dlgPlayBack->m_dlgRecordFile.m_dwVersion = lpTemp->dwVersion;

        //在这里进行设备的验证，只有序列号在m_stAccessDeviceList中的设备才会允许注册

        if (pDlg->m_bUseAccessList)
        {
            for (int i = 0; i < 64; i++)
            {
                if (lpTemp->dwVersion >= 4)
                {
                    if (strcmp((char*)pDevInfo->struRegInfo.sDeviceSerial, pDlg->m_stAccessDeviceList[i].sSerialNumber) == 0)
                    {
                        //允许注册
                        ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
                        return TRUE;
                    }
                }
                else
                {
                    if (strcmp((char*)pDevInfo->struRegInfo.byDeviceID, pDlg->m_stAccessDeviceList[i].sSerialNumber) == 0)
                    {
                        //允许注册
                        ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
                        return TRUE;
                    }
                }
            }
        }
        else
        {
            //允许注册
            ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
            return TRUE;
        }

        //不允许注册
        return FALSE;
    }
    else if (ENUM_DEV_OFF == dwDataType || ENUM_DEV_SESSIONKEY_ERROR == dwDataType)
    {
        ::PostMessage(pDlg->m_hWnd, WM_DEL_DEV, NULL, (LPARAM)iUserID); 
    }
    else if (ENUM_DEV_AUTH == dwDataType)
    {
        for (int i = 0; i < MAX_DEVICES; i++)
        {
            if (g_struDeviceInfo[i].iDeviceIndex == -1)
            {
                char byDeviceID[256] = { 0 };
                DWORD dwOutLenTemp = 0;
                NET_EHOME_DEV_REG_INFO_V12 *pDevInfo = (NET_EHOME_DEV_REG_INFO_V12 *)pOutBuffer;
                LOCAL_DEVICE_INFO *lpTemp = new LOCAL_DEVICE_INFO();
                UTF82A((char*)pDevInfo->struRegInfo.byDeviceID, byDeviceID, MAX_DEVICE_ID_LEN, &dwOutLenTemp);
                strncpy((char*)lpTemp->byDeviceID, byDeviceID, MAX_DEVICE_ID_LEN);
                strncpy((char*)lpTemp->sDeviceSerial, (char*)pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN);
                lpTemp->dwVersion = 5;
                strncpy((char*)lpTemp->byDeviceFullSerial, (char*)pDevInfo->byDeviceFullSerial, MAX_FULL_SERIAL_NUM_LEN);

                if (g_pMainDlg != NULL)
                {
                    //允许注册
                    ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
                }

                break;
            }
            else
            {
                char byDeviceIDTemp[256] = { 0 };
                char sDeviceSerialTemp[NET_EHOME_SERIAL_LEN] = { 0 };
                DWORD dwOutLenTemp = 0;
                char szDeviceName[256] = { 0 };
                NET_EHOME_DEV_REG_INFO_V12 *pDevInfo = (NET_EHOME_DEV_REG_INFO_V12 *)pOutBuffer;
                UTF82A((char*)pDevInfo->struRegInfo.byDeviceID, byDeviceIDTemp, MAX_DEVICE_ID_LEN, &dwOutLenTemp);
                UTF82A((char*)pDevInfo->sDevName, szDeviceName, 256, &dwOutLenTemp);
               if (memcmp(g_struDeviceInfo[i].byDeviceID, byDeviceIDTemp, MAX_DEVICE_ID_LEN) == 0 && memcmp(g_struDeviceInfo[i].sDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN) == 0)
                {
                    if (strlen((char*)g_struDeviceInfo[i].byEhomeKey) == 0)
                    {
                        string szkeyTemp = pDlg->GetDevAuthKey((char *)g_struDeviceInfo[i].byDeviceID);
                        if (!szkeyTemp.empty())
                        {
                            memcpy(g_struDeviceInfo[i].byEhomeKey, szkeyTemp.c_str(), 32);
                        }
                    }
                   g_struDeviceInfo[i].dwVersion = 5;
                   memcpy(pInBuffer, g_struDeviceInfo[i].byEhomeKey, 32);
                   break;
                }
                //如果序列号相同但是，deviceID不同，这时是改了名字了，就把之前的设备下线

                if (memcmp(g_struDeviceInfo[i].sDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN) == 0)
                {
                    if (!memcmp(g_struDeviceInfo[i].byDeviceID, byDeviceIDTemp, MAX_DEVICE_ID_LEN) == 0)
                    {
                        strncpy(sDeviceSerialTemp, (char*)g_struDeviceInfo[i].sDeviceSerial, NET_EHOME_SERIAL_LEN);
                        ::PostMessage(pDlg->m_hWnd, WM_DEL_OFFLINEDEV, NULL, (LPARAM)sDeviceSerialTemp);
                    }
                    break;
                }
                //如果序列号和DeviceID相同，但是协议版本不一致，即修改了协议版本，把之前的设备下线
                else if (strlen((char*)pDevInfo->struRegInfo.byDevProtocolVersion) != 0)
                {
                    DWORD dwVer = 0;
                    if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "2", 1) == 0)
                    {
                        dwVer = 2;
                    }
                    else if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "4", 1) == 0)
                    {
                        dwVer = 4;
                    }
                    else
                    {
                        dwVer = 5;
                    }
                    if (g_struDeviceInfo[i].dwVersion != dwVer)
                    {
                        g_struDeviceInfo[i].dwVersion = dwVer;
                        strncpy(sDeviceSerialTemp, (char*)g_struDeviceInfo[i].sDeviceSerial, NET_EHOME_SERIAL_LEN);
                        ::PostMessage(pDlg->m_hWnd, WM_DEL_OFFLINEDEV, NULL, (LPARAM)sDeviceSerialTemp);
                    }
                }
            }
        }
    }
    else if (ENUM_DEV_SESSIONKEY == dwDataType)
    {
        NET_EHOME_DEV_REG_INFO_V12 *pDevInfo = (NET_EHOME_DEV_REG_INFO_V12 *)pOutBuffer;
        NET_EHOME_DEV_SESSIONKEY devSessionkey = { 0 };
        memcpy(devSessionkey.sDeviceID, pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN);
        memcpy(devSessionkey.sSessionKey, pDevInfo->struRegInfo.bySessionKey, MAX_MASTER_KEY_LEN);

        DWORD idx = 0;
        for (; idx < MAX_DEVICES; idx++)
        {
            if (!memcmp(g_struDeviceInfo[idx].byDeviceID, pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN))
            {
                break;
            }
        }

        g_pMainDlg->RecordDevAuthInfo((char*)g_struDeviceInfo[idx].byDeviceID, (char*)g_struDeviceInfo[idx].byEhomeKey);

        char szSessionKey[17] = { 0 };
        memcpy(szSessionKey, devSessionkey.sSessionKey, MAX_MASTER_KEY_LEN);
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "Get sSessionKey [%s]", szSessionKey);
        if (0 == g_iServerType)
        {
            NET_ECMS_SetDeviceSessionKey(&devSessionkey);

            NET_EALARM_SetDeviceSessionKey(&devSessionkey);
        }
        else if (1 == g_iServerType)
        {

            CXmlBase   struXMLBase;
            if (_T("") == pDlg->m_strDasConfigFile)
            {
                pDlg->m_strDasConfigFile = _T(".\\DasInfo.xml");
            }
            struXMLBase.LoadFile(pDlg->m_strDasConfigFile);

            if (!struXMLBase.FindElem("DasInfo"))
            {
                struXMLBase.CreateRoot("DasInfo");
            }
            else
            {
                struXMLBase.IntoElem();
            }
                
            while (struXMLBase.FindElem("Daslist"))
            {
                struXMLBase.RemoveNode();
                struXMLBase.IntoElem();
            }

            if (struXMLBase.AddNode("Daslist"))
            {
                if (struXMLBase.AddNode("Das"))
                {
                    ConvertSingleNodeData(pDevInfo->struRegInfo.byDeviceID, struXMLBase, "DeviceID", NODE_ARRAY_TO_STRING, MAX_DEVICE_ID_LEN);
                    ConvertSingleNodeData(pDevInfo->struRegInfo.bySessionKey, struXMLBase, "SessionKey", NODE_ARRAY_TO_STRING, MAX_MASTER_KEY_LEN);

                    struXMLBase.OutOfElem();
                }
                struXMLBase.OutOfElem();
            }
            struXMLBase.OutOfElem();

            struXMLBase.WriteToFile(pDlg->m_strDasConfigFile);
        }
    }
    else if (ENUM_DEV_DAS_REQ == dwDataType)
    {
        char szLocalIP[128] = { 0 };
        DWORD dwPort = 0;
        pDlg->GetDasAddressByType(3, 0, szLocalIP, sizeof(szLocalIP), (char *)&dwPort, sizeof(dwPort));
        if (dwPort == 0)
        {
            dwPort = pDlg->m_nPort;
        }
        //if (memcmp(pDlg->m_sLocalIP, "0.0.0.0", strlen("0.0.0.0")) == 0)
        //{
        //    pDlg->GetLocalIP(szLocalIP);
        //}
        //else
        //{
        //    strncpy(szLocalIP, pDlg->m_sLocalIP, 128 - 1);
        //}

        sprintf((char*)pInBuffer, "{\"Type\":\"DAS\",\"DasInfo\":{\"Address\":\"%s\","
            "\"Domain\":\"test.ys7.com\",\"ServerID\":\"das_%s_%d\",\"Port\":%d,\"UdpPort\":%d}}"
            , szLocalIP, szLocalIP, dwPort, dwPort, dwPort);
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "ENUM_DEV_DAS_REQ das:[%s] port[%d]", szLocalIP, dwPort);
    }
    else if (ENUM_DEV_SESSIONKEY_REQ == dwDataType)
    {
        CXmlBase   struXMLBaseDas;
        if (_T("") == pDlg->m_strDasConfigFile)
        {
            pDlg->m_strDasConfigFile = _T(".\\DasInfo.xml");
        }
        struXMLBaseDas.LoadFile(pDlg->m_strDasConfigFile);

        if (struXMLBaseDas.FindElem("DasInfo") && struXMLBaseDas.IntoElem())
        {
            if (struXMLBaseDas.FindElem("Daslist") && struXMLBaseDas.IntoElem())
            {
                do
                {
                    if (struXMLBaseDas.FindElem("Das") && struXMLBaseDas.IntoElem())
                    {
                        NET_EHOME_DEV_SESSIONKEY devSessionkey = { 0 };

                        if (struXMLBaseDas.FindElem("DeviceID"))
                        {
                            memcpy(devSessionkey.sDeviceID, struXMLBaseDas.GetData().c_str(), min(strlen(struXMLBaseDas.GetData().c_str()), MAX_MASTER_KEY_LEN));
                        }
                        if (struXMLBaseDas.FindElem("SessionKey"))
                        {
                            memcpy(devSessionkey.sSessionKey, struXMLBaseDas.GetData().c_str(), min(strlen(struXMLBaseDas.GetData().c_str()), MAX_MASTER_KEY_LEN));
                        }
                        NET_ECMS_SetDeviceSessionKey(&devSessionkey);
                        NET_EALARM_SetDeviceSessionKey(&devSessionkey);

                        char szDevSessionKey[17] = { 0 };
                        memcpy(szDevSessionKey, devSessionkey.sSessionKey, MAX_MASTER_KEY_LEN);
                        //char szLan[1024] = { 0 };
                        //sprintf(szLan, "sSessionKey: %s\r\n", szSessionKey);
                        //OutputDebugStringA(szLan);
                        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "Set sSessionKey [%s]", szDevSessionKey);
                        struXMLBaseDas.OutOfElem();
                    }
                } while (struXMLBaseDas.NextSibElem());
                struXMLBaseDas.OutOfElem();
            }
            struXMLBaseDas.OutOfElem();
        }
    }
    else if (ENUM_DEV_ADDRESS_CHANGED == dwDataType)
    {
        BYTE byEhomeKey[32] = { 0 };
        //获取设备EHomeKey
        for (int i = 0; i < MAX_DEVICES; i++)
        {
            if (g_struDeviceInfo[i].iDeviceIndex != -1)
            {
                if (g_struDeviceInfo[i].lLoginID == iUserID)
                {
                    memcpy(byEhomeKey, g_struDeviceInfo[i].byEhomeKey, 32);
                    break;
                }
            }
        }
        //先把原来的设备删了吧
        ::PostMessage(pDlg->m_hWnd, WM_CHANGE_IP, NULL, (LPARAM)iUserID);

        g_pMainDlg->AddLog(iUserID, OPERATION_SUCC_T, 0, "设备IP发生变化后，重新注册!");

        LOCAL_DEVICE_INFO *lpTemp = new LOCAL_DEVICE_INFO();
        memcpy(lpTemp->byEhomeKey, byEhomeKey, 32);
        NET_EHOME_DEV_REG_INFO_V12 *pDevInfo = (NET_EHOME_DEV_REG_INFO_V12 *)pOutBuffer;

        //UTF-8转GBK
        DWORD dwOutLen = 0;
        UTF82A((char*)pDevInfo->struRegInfo.byDeviceID, (char*)pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN, &dwOutLen);
        UTF82A((char*)pDevInfo->struRegInfo.sDeviceSerial, (char*)pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN, &dwOutLen);

        if (pDevInfo != NULL)
        {
            memcpy(lpTemp->byDeviceID, pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN);
            lpTemp->lLoginID = iUserID;
            memcpy(lpTemp->sDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN);
            //memcpy(lpTemp->sIdentifyCode, pDevInfo->struRegInfo.sIdentifyCode, CODE_LEN);
        }

        memcpy(lpTemp->byDeviceID, pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN);
        memcpy(lpTemp->sDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN);
        lpTemp->lLoginID = iUserID;

        //先拷贝到一个大的缓冲区，在获取长度
        char szDeviceSerial[NET_EHOME_SERIAL_LEN+1] = {0};
        memcpy(szDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN);
       // lpTemp->dwVersion = pDevInfo->struRegInfo.byDevProtocolVersion[0];
        //if (strlen(szDeviceSerial) < 0)
        //{
        //    lpTemp->dwVersion = 2;
        //}
        //else
        //{
        //    lpTemp->dwVersion = 4;
        //}
        if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "2", 1) == 0)
        {
            lpTemp->dwVersion = 2;
        }
        else if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "4", 1) == 0)
        {
            lpTemp->dwVersion = 4;
        }
        else
        {
            lpTemp->dwVersion = 5;
        }


        //在这里进行设备的验证，只有序列号在m_stAccessDeviceList中的设备才会允许注册

        if (pDlg->m_bUseAccessList)
        {
            for (int i = 0; i < 64; i++)
            {
                if (lpTemp->dwVersion >= 4)
                {
                    if (strcmp((char*)pDevInfo->struRegInfo.sDeviceSerial, pDlg->m_stAccessDeviceList[i].sSerialNumber) == 0)
                    {
                        //允许注册
                        ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
                        return TRUE;
                    }
                }
                else
                {
                    if (strcmp((char*)pDevInfo->struRegInfo.byDeviceID, pDlg->m_stAccessDeviceList[i].sSerialNumber) == 0)
                    {
                        //允许注册
                        ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
                        return TRUE;
                    }
                }
            }
        }
        else
        {
            //允许注册
            ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
            return TRUE;
        }

        //不允许注册
        return FALSE;
    }
	else if (ENUM_DEV_DAS_REREGISTER == dwDataType)
	{
        if (pInBuffer == NULL)
        {
            return FALSE;
        }

        BYTE byEhomeKey[32] = { 0 };
        BYTE byClouldSecretKey[64] = { 0 };
        //获取设备EHomeKey
        for (int i = 0; i < MAX_DEVICES; i++)
        {
            if (g_struDeviceInfo[i].iDeviceIndex != -1)
            {
                if (g_struDeviceInfo[i].lLoginID == iUserID)
                {
                    memcpy(byEhomeKey, g_struDeviceInfo[i].byEhomeKey, 32);
                    memcpy(byClouldSecretKey, g_struDeviceInfo[i].byClouldSecretKey, 64);
                    break;
                }
            }
        }

        
        NET_EHOME_DEV_REG_INFO_V12 *pDevInfo = (NET_EHOME_DEV_REG_INFO_V12 *)pOutBuffer;
        if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "2", 1) == 0)
        {
            return TRUE;//对于Ehome2.0的设备来说是没有序列号的只有deviceID，所以把根据下面的先把序列号删除会造成通道重复
        }

        //先把原来的删除
        ::PostMessage(pDlg->m_hWnd, WM_DEL_OFFLINEDEV, NULL, (LPARAM)pDevInfo->struRegInfo.sDeviceSerial);
        //UTF-8转GBK
        LOCAL_DEVICE_INFO *lpTemp = new LOCAL_DEVICE_INFO();
        memcpy(lpTemp->byEhomeKey, byEhomeKey, 32);
        memcpy(lpTemp->byClouldSecretKey, byClouldSecretKey, 64);
        DWORD dwOutLenTemp = 0;
        UTF82A((char*)pDevInfo->struRegInfo.byDeviceID, (char*)pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN, &dwOutLenTemp);
        UTF82A((char*)pDevInfo->struRegInfo.sDeviceSerial, (char*)pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN, &dwOutLenTemp);

        if (pDevInfo != NULL)
        {
            memcpy(lpTemp->byDeviceID, pDevInfo->struRegInfo.byDeviceID, MAX_DEVICE_ID_LEN);
            lpTemp->lLoginID = iUserID;
            memcpy(lpTemp->sDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN);
        }
        lpTemp->lLoginID = iUserID;

        //先拷贝到一个大的缓冲区，在获取长度
        char szDeviceSerial[NET_EHOME_SERIAL_LEN + 1] = { 0 };
        memcpy(szDeviceSerial, pDevInfo->struRegInfo.sDeviceSerial, NET_EHOME_SERIAL_LEN);
        char szSessionKey[MAX_MASTER_KEY_LEN + 1] = { 0 };
        memcpy(szSessionKey, pDevInfo->struRegInfo.bySessionKey, MAX_MASTER_KEY_LEN);

        if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "2", 1) == 0)
        {
            lpTemp->dwVersion = 2;
        }
        else if (memcmp(pDevInfo->struRegInfo.byDevProtocolVersion, "4", 1) == 0)
        {
            lpTemp->dwVersion = 4;
        }
        else
        {
            lpTemp->dwVersion = 5;
        }

        //在这里进行设备的验证，只有序列号在m_stAccessDeviceList中的设备才会允许注册
        if (pDlg->m_bUseAccessList)
        {
            for (int i = 0; i < 64; i++)
            {
                if (lpTemp->dwVersion >= 4)
                {
                    if (strcmp((char*)pDevInfo->struRegInfo.sDeviceSerial, pDlg->m_stAccessDeviceList[i].sSerialNumber) == 0)
                    {
                        //允许注册
                        ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
                        return TRUE;
                    }
                }
                else
                {
                    if (strcmp((char*)pDevInfo->struRegInfo.byDeviceID, pDlg->m_stAccessDeviceList[i].sSerialNumber) == 0)
                    {
                        //允许注册
                        ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
                        return TRUE;
                    }
                }
            }
        }
        else
        {
            //允许注册
            ::PostMessage(pDlg->m_hWnd, WM_ADD_DEV, NULL, (LPARAM)lpTemp);
            return TRUE;
        }
        //不允许注册
        return FALSE;
	}
    else if (ENUM_DEV_DAS_PINGREO == dwDataType)
    {
        NET_EHOME_DEV_REG_INFO_V12 *pDevInfo = (NET_EHOME_DEV_REG_INFO_V12 *)pOutBuffer;
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "ENUM_DEV_DAS_PINGREO [%s]", pDevInfo->struRegInfo.byDeviceID);
        return TRUE;
    }
    else if (ENUM_DEV_SLEEP == dwDataType)
    {
        ::PostMessage(pDlg->m_hWnd, WM_DEV_SLEEP, NULL, (LPARAM)iUserID);

        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "Device sleep [%d]", iUserID);
        return TRUE;
    }
    else if (ENUM_DEV_DAS_EHOMEKEY_ERROR == dwDataType)
    {
        NET_EHOME_DEV_REG_INFO_V12 *pDevInfo = (NET_EHOME_DEV_REG_INFO_V12 *)pOutBuffer;
        int a = 0;
    }
    return TRUE;
}


BOOL CEHomeDemoDlg::IsInAccessDeviceList(char* pSerialNumber)
{
    if (pSerialNumber == NULL)
    {
        return FALSE;
    }
    for (int i = 0; i < 64; i++)
    {
        if (strcmp((char*)pSerialNumber,m_stAccessDeviceList[i].sSerialNumber) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CALLBACK AlarmMsgCallBack(LONG lHandle, NET_EHOME_ALARM_MSG *pAlarmMsg, void *pUser)
{
    UN_REFERENCED_PARAMETER(lHandle)
    CEHomeDemoDlg *pDlg = static_cast<CEHomeDemoDlg *>(pUser);

    if (NULL == pDlg)
    {
        return FALSE;
    }

    //在这里根据设备的序列号，进行Token认证
    if (pDlg->m_bUseAccessList && !pDlg->IsInAccessDeviceList(pAlarmMsg->sSerialNumber))
    {
        return FALSE;
    }



    pDlg->ProcessAlarmData(pAlarmMsg->dwAlarmType, pAlarmMsg->pAlarmInfo, pAlarmMsg->dwAlarmInfoLen, pAlarmMsg->pXmlBuf, pAlarmMsg->dwXmlBufLen,pAlarmMsg->pHttpUrl,pAlarmMsg->dwHttpUrlLen);

    return TRUE;
}


CEHomeDemoDlg::CEHomeDemoDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CEHomeDemoDlg::IDD, pParent)
, m_iSelListType(0)
, m_bListLogEnlarge(FALSE)
, m_bListAlarmEnlarge(FALSE)
, m_iMainType(PREVIEW_T)
, m_iCurWndNum(4)
, m_iCurWndIndex(0)
, m_lTcpAlarmHandle(-1)
, m_lUdpAlarmHandle(-1)
, m_lEhome50AlarmHandle(-1)
, m_lSSHandle(-1)
, m_iCurDeviceIndex(-1)
, m_iCurChanIndex(-1)
, m_bCyclePreview(FALSE)
, m_fnDevCB(NULL)
, m_pDevUserData(NULL)
, m_iPreviewHandle(0)
, m_bRecord(FALSE)
, m_bSound(FALSE)
, m_byUseCmsPort(0)
, m_bAsyncCalllback(FALSE)
, m_lCMSLFListenHandle(-1)
, m_bRtpAudio(FALSE)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    //清空可接入设备列表
    memset(&m_stAccessDeviceList, 0, sizeof(m_stAccessDeviceList));
    memset(&struServInfo,0,sizeof(struServInfo));

    m_bUseAccessList = FALSE; //不启用可接入设备列表

    //初始化可接入设备列表（不在该列表中的设备不允许注册）
    //目前只允许设备序列号为ESimulator0000的设备接入
    //memcpy(m_stAccessDeviceList[0].sSerialNumber, "哈-00001", 12);
    //memcpy(m_stAccessDeviceList[1].sSerialNumber, "哈-00001", 12);

    memset(&m_struServInfo, 0, sizeof(m_struServInfo));
    m_struServInfo.struUDPAlarmSever.wPort = 7661;
    memset(m_sLocalIP, 0, sizeof(m_sLocalIP));
    memset(&m_struLogInfo, 0, sizeof(m_struLogInfo));
    memcpy(m_struLogInfo.byLogPath, "C:/EHomeSdkLog/", sizeof(m_struLogInfo.byLogPath));
    m_struLogInfo.dwLogLevel = 3;
    m_struLogInfo.bAutoDel = true;
    m_byCmsSecureAccessType = 0;
    m_byAlarmSecureAccessType = 0;
    m_byStreamSecureAccessType = 0;
    m_wAlarmServerMqttPort = 0;
    m_strDasConfigFile = _T("C:/EHomeSdkLog/DasInfo.xml");
    memset(m_szDBKey, 0, sizeof(m_szDBKey));
}

CEHomeDemoDlg::~CEHomeDemoDlg()
{
    //停止预览监听后，才能把播放窗口释放
    StopPreviewListen();
    if(m_pDlgPreview != NULL)
    {
        delete[] m_pDlgPreview;
        m_pDlgPreview = NULL;
    }
    if (m_dlgPlayBack != NULL)
    {
        delete m_dlgPlayBack;
        m_dlgPlayBack = NULL;
    }

    if (m_dlgAudioTalk != NULL)
    {
        delete m_dlgAudioTalk;
        m_dlgAudioTalk = NULL;
    }

    if (m_dlgCommonCfg != NULL)
    {
        delete m_dlgCommonCfg;
        m_dlgCommonCfg = NULL;
    }

    if (m_dlgLocalCfg != NULL)
    {
        delete m_dlgLocalCfg;
        m_dlgLocalCfg = NULL;
    }

    if (m_dlgOutputCtrl != NULL)
    {
        delete m_dlgOutputCtrl;
        m_dlgOutputCtrl = NULL;
    }

    if (m_dlgHttpAlarm != NULL)
    {
        delete m_dlgHttpAlarm;
        m_dlgHttpAlarm = NULL;
    }

    if (m_pDlgVI != NULL)
    {
        delete m_pDlgVI;
        m_pDlgVI = NULL;
    }

    NET_EHOME_AMS_ADDRESS struAmsAddr = {0};
    struAmsAddr.dwSize = sizeof(struAmsAddr);
    struAmsAddr.byEnable = 2;
    //CMS停止接收
    NET_ECMS_SetSDKLocalCfg(AMS_ADDRESS, &struAmsAddr);
    NET_EALARM_StopListen(m_lUdpAlarmHandle);
    NET_EALARM_StopListen(m_lTcpAlarmHandle);
    NET_EALARM_StopListen(m_lEhome50AlarmHandle);
    NET_EALARM_StopListen(m_lCmsAlarm);
    if (g_bSS_Enable)
    {
        NET_ESS_StopListen(m_lSSHandle);
        NET_ESS_Fini();
    }
    NET_EALARM_Fini();
    NET_ECMS_StopListen(0);
    NET_ECMS_Fini();
    NET_ESTREAM_Fini();
}
void CEHomeDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_WNDNUM, m_comboWinNum);
    DDX_Control(pDX, IDC_TREE_DEV, m_treeDeviceList);
    DDX_Control(pDX, IDC_LIST_ALL_LOG, m_listAllLog);
    DDX_Control(pDX, IDC_LIST_ALARM_INFO, m_listAlarmInfo);
    DDX_Text(pDX, IDC_EDIT_PREVIEW_HANDLE, m_iPreviewHandle);
    DDX_Check(pDX, IDC_CHECK_RTP_AUDIO, m_bRtpAudio);
}

BEGIN_MESSAGE_MAP(CEHomeDemoDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RADIO_ALARM_INFO, &CEHomeDemoDlg::OnBnClickedRadioAlarmInfo)
    ON_BN_CLICKED(IDC_RADIO_LOCAL_LOG, &CEHomeDemoDlg::OnBnClickedRadioLocalLog)
    ON_CBN_SELCHANGE(IDC_COMBO_WNDNUM, &CEHomeDemoDlg::OnCbnSelchangeComboWndnum)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ALL_LOG, &CEHomeDemoDlg::OnLvnColumnclickListAllLog)
    ON_BN_CLICKED(IDC_BTN_PLAYBACK, &CEHomeDemoDlg::OnBnClickedBtnPlayback)
    ON_BN_CLICKED(IDC_BTN_PREVIEW, &CEHomeDemoDlg::OnBnClickedBtnPreview)
    ON_BN_CLICKED(IDC_BTN_AUDIO_TALK, &CEHomeDemoDlg::OnBnClickedBtnAudioTalk)
    ON_BN_CLICKED(IDC_BTN_CFG, &CEHomeDemoDlg::OnBnClickedBtnCfg)
    ON_BN_CLICKED(IDC_BTN_LOCAL_CFG, &CEHomeDemoDlg::OnBnClickedBtnLocalCfg)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CEHomeDemoDlg::OnBnClickedBtnExit)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ALARM_INFO, &CEHomeDemoDlg::OnLvnColumnclickListAlarmInfo)
    ON_MESSAGE(WM_ADD_LOG, OnWMAddLog)
    ON_MESSAGE(WM_ADD_DEV, OnWMAddDev)
    ON_MESSAGE(WM_DEL_DEV, OnWMDelDev)
    ON_MESSAGE(WM_DEL_OFFLINEDEV, OnWMDelOfflineDev)
    ON_MESSAGE(WM_PROC_EXCEPTION, OnWMProcException)
    ON_MESSAGE(WM_CHANGE_CHANNEL_ITEM_IMAGE,ChangeChannelItemImage)
    ON_MESSAGE(WM_CHANGE_IP, OnWMChangeIPAddr)
    ON_MESSAGE(WM_SETPREVIEW, OnWMSetPreview)
    ON_MESSAGE(WM_DEV_SLEEP, OnWMDevSleep)

    //    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DEV, &CEHomeDemoDlg::OnTvnSelchangedTreeDev)
    ON_NOTIFY(NM_CLICK, IDC_TREE_DEV, &CEHomeDemoDlg::OnNMClickTreeDev)
    ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEV, &CEHomeDemoDlg::OnNMDblclkTreeDev)

    ON_BN_CLICKED(IDC_BTN_OTHER, &CEHomeDemoDlg::OnBnClickedBtnOther)
    ON_BN_CLICKED(IDC_BTN_PREVIEW_LISTEN, &CEHomeDemoDlg::OnBnClickedBtnPreviewListen)
    ON_NOTIFY(NM_RCLICK, IDC_TREE_DEV, &CEHomeDemoDlg::OnNMRClickTreeDev)
    ON_BN_CLICKED(IDC_BTN_CIRCLE_PREVIEW, &CEHomeDemoDlg::OnBnClickedBtnCirclePreview)
    ON_WM_TIMER()
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_BTN_CLOSE_PREVIEW, &CEHomeDemoDlg::OnBnClickedBtnClosePreview)
    ON_COMMAND(ID_CHANNEL_INFO, &CEHomeDemoDlg::OnMenuChannelInfo)
    ON_COMMAND(ID_MENU_PROXY, &CEHomeDemoDlg::OnMenuProxy)
    ON_COMMAND(ID_MENU_GPS_INFO, &CEHomeDemoDlg::OnGetGpsInfo)
    ON_COMMAND(ID_MENU_WIRELESS_INFO, &CEHomeDemoDlg::OnBtnWirelessInfo)
    ON_COMMAND(ID_MENU_ISAPI_PT, &CEHomeDemoDlg::OnMenuIsapiPt)
    ON_BN_CLICKED(IDC_BTN_SALVE, &CEHomeDemoDlg::OnBnClickedBtnSalve)
    ON_COMMAND(ID_MENU_UPGRADE, &CEHomeDemoDlg::OnMenuUpgrade)
    ON_BN_CLICKED(IDC_BTN_SOUND, &CEHomeDemoDlg::OnBnClickedBtnSound)
    ON_COMMAND(ID_ENABLE_CFG, &CEHomeDemoDlg::OnEnableCfg)
    ON_COMMAND(ID_RECODR_CFG, &CEHomeDemoDlg::OnRecodrCfg)
    ON_COMMAND(ID_CAPTURE_PIC, &CEHomeDemoDlg::OnCapturePic)
    ON_COMMAND(ID_MONTION_AREA, &CEHomeDemoDlg::OnMontionArea)
    ON_COMMAND(ID_PRIVATE_AREA, &CEHomeDemoDlg::OnPrivateArea)
    ON_COMMAND(ID_HIDE_ALARM, &CEHomeDemoDlg::OnHideAlarm)

    ON_COMMAND(IDR_MENU_EHOME50_AUTH, &CEHomeDemoDlg::OnMenuAddEhome50DevAuth)
    ON_COMMAND(IDR_MENU_EHOME50_AUTH_DEV_BTN, &CEHomeDemoDlg::OnMenuSetEhome50DevAuth)
    ON_COMMAND(IDR_MENU_LOGOUT_BTN, &CEHomeDemoDlg::OnMenuLogout)
#ifndef DEMO_LAN_EN
    ON_BN_CLICKED(IDC_BTN_IFRAME, &CEHomeDemoDlg::OnBnClickedBtnIframe)
    ON_BN_CLICKED(IDC_BTN_STREAM_ENCRYPT, &CEHomeDemoDlg::OnBnClickedBtnStreamEncrypt)
    ON_COMMAND(ID_MENU_HTTP_UPGRADE, &CEHomeDemoDlg::OnMenuHttpUpgrade)
    ON_COMMAND(ID_MENU_ISAPI_CFG, &CEHomeDemoDlg::OnMenuIsapiCfg)
    ON_COMMAND(ID_MENU_HTTP_ALARM, &CEHomeDemoDlg::OnMenuHttpAlarm)
#endif
    ON_COMMAND(ID_MENU_STREAM_CHECK, &CEHomeDemoDlg::OnMenuStreamCheck)

    ON_COMMAND(ID_MENU_STORAGESERVER, &CEHomeDemoDlg::OnMenuStorageserver)
    ON_BN_CLICKED(IDC_CHK_ASYNC, &CEHomeDemoDlg::OnBnClickedChkAsync)

    ON_COMMAND(ID_MENU_WAKEUP_DEVICE, &CEHomeDemoDlg::OnMenuWakeupDevice)
    ON_COMMAND(ID_MENU_SLEEP_DEVICE, &CEHomeDemoDlg::OnMenuSleepDevice)

    ON_COMMAND(ID_MENU_WEB_CONFIG, &CEHomeDemoDlg::OnMenuWebConfig)
    ON_BN_CLICKED(IDC_CHECK_RTP_AUDIO, &CEHomeDemoDlg::OnBnClickedCheckRtpAudio)
END_MESSAGE_MAP()


// CEHomeDemoDlg 消息处理程序

BOOL CEHomeDemoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);            // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    //ShowWindow(SW_MINIMIZE);

    // TODO: 在此添加额外的初始化代码
    g_pMainDlg = this;

    //memset(m_sDasIP, 0, 128);
    //memcpy(m_sDasIP, "0.0.0.0", 7);
    //m_nDasPort = 0;

    //GetLocalIP((char*)m_sLocalIP);
    InitParamFromXML();//读取XML配置文件放在界面初始化前面
    InitChildWindow();
    InitMainWin();    
    LoadTreeImage();
    CreateTree();

    if (((CButton*)GetDlgItem(IDC_CHK_ASYNC))->GetCheck() == 1)
    {
        m_bAsyncCalllback = TRUE;
    }
    else
    {
        m_bAsyncCalllback = FALSE;
    }


    InitLib();

    if (1 != g_iServerType)
    {
        InitPreviewListenParam();//LBS服务器默认不开启预览监听
    }
    SetStreamExceptionCallBack(); //设置码流异常回调函数，支持预览，回放，语音对讲异常回调

    CRect rtDesk;
    CRect rtDlg;

    ::GetWindowRect(::GetDesktopWindow(), &rtDesk);
    GetWindowRect(&rtDlg);

    int iXpos = rtDesk.Width() / 2 - rtDlg.Width() / 2;
    int iYpos = rtDesk.Height() / 2 - rtDlg.Height() / 2;

    SetWindowPos(NULL, iXpos, iYpos, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
    //get current system resolution
    g_iCurScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    g_iCurScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEHomeDemoDlg::InitLanguage()
{
    ((CButton*)GetDlgItem(IDC_BTN_PREVIEW))->SetWindowTextA(IDC_BTN_PREVIEW_TEXT);
    ((CButton*)GetDlgItem(IDC_BTN_PLAYBACK))->SetWindowTextA(IDC_BTN_PLAYBACK_TEXT);
}

void CEHomeDemoDlg::SetAddr(CString csIP, int nPort)
{
    strncpy(m_sLocalIP, csIP.GetBuffer(), csIP.GetLength());
    m_nPort = nPort;
}

void CEHomeDemoDlg::InitParamFromXML()
{
    CXmlBase   struXMLBase;
    struXMLBase.LoadFile(ISUP_DEMO_CONFIG_XML);

    if (struXMLBase.FindElem("LocalCfg") && struXMLBase.IntoElem())
    {
        if (struXMLBase.FindElem("Param") && struXMLBase.IntoElem())
        {
            ConvertSingleNodeData(m_struLogInfo.byLogPath, struXMLBase, "LogPath", NODE_STRING_TO_ARRAY, 128);
            ConvertSingleNodeData(&m_struLogInfo.dwLogLevel, struXMLBase, "LogLevel", NODE_STRING_TO_INT);
            ConvertSingleNodeData(&m_struLogInfo.bAutoDel, struXMLBase, "LogAutoDel", NODE_STRING_TO_BOOL);

            ConvertSingleNodeData(&m_struServInfo.dwKeepAliveSec, struXMLBase, "KeepAliveSeconds", NODE_STRING_TO_INT);
            ConvertSingleNodeData(&m_struServInfo.dwTimeOutCount, struXMLBase, "KeepAliveCount", NODE_STRING_TO_INT);

            // 报警暂时保持和注册服务器一致
            m_struServInfo.dwAlarmKeepAliveSec = m_struServInfo.dwKeepAliveSec;
            m_struServInfo.dwAlarmTimeOutCount = m_struServInfo.dwTimeOutCount;

            ConvertSingleNodeData(&m_struServInfo.dwAlarmServerType, struXMLBase, "AlarmServerType", NODE_STRING_TO_INT);

            ConvertSingleNodeData(m_struServInfo.struTCPAlarmSever.szIP, struXMLBase, "AlarmServerIP", NODE_STRING_TO_ARRAY, 128);
            memcpy(m_struServInfo.struUDPAlarmSever.szIP, m_struServInfo.struTCPAlarmSever.szIP, 128);

            ConvertSingleNodeData(&m_struServInfo.struUDPAlarmSever.wPort, struXMLBase, "AlarmServerUdpPort", NODE_STRING_TO_WORD);

            ConvertSingleNodeData(&m_struServInfo.struTCPAlarmSever.wPort, struXMLBase, "AlarmServerTcpPort", NODE_STRING_TO_WORD);
            ConvertSingleNodeData(&m_wAlarmServerMqttPort, struXMLBase, "AlarmServerMqttPort", NODE_STRING_TO_WORD);

            ConvertSingleNodeData(&m_byUseCmsPort, struXMLBase, "AlarmServerPortUseCms", NODE_STRING_TO_INT);

            ConvertSingleNodeData(m_struServInfo.struNTPSever.szIP, struXMLBase, "NTPServerIP", NODE_STRING_TO_ARRAY, 128);
            ConvertSingleNodeData(&m_struServInfo.struNTPSever.wPort, struXMLBase, "NTPServerPort", NODE_STRING_TO_WORD);
            ConvertSingleNodeData(&m_struServInfo.dwNTPInterval, struXMLBase, "NTPInterval", NODE_STRING_TO_INT);

            ConvertSingleNodeData(&m_struServInfo.dwPicServerType, struXMLBase, "PictureServerType", NODE_STRING_TO_INT);
            ConvertSingleNodeData(m_struServInfo.struPictureSever.szIP, struXMLBase, "PictureServerIP", NODE_STRING_TO_ARRAY, 128);
            ConvertSingleNodeData(&m_struServInfo.struPictureSever.wPort, struXMLBase, "PictureServerPort", NODE_STRING_TO_WORD);
            
            ConvertSingleNodeData(&m_byCmsSecureAccessType, struXMLBase, "CmsAccessSecurity", NODE_STRING_TO_INT);
            ConvertSingleNodeData(&m_byAlarmSecureAccessType, struXMLBase, "AlarmAccessSecurity", NODE_STRING_TO_INT);
            ConvertSingleNodeData(&m_byStreamSecureAccessType, struXMLBase, "StreamAccessSecurity", NODE_STRING_TO_INT);
            

            ConvertSingleNodeData(&m_byCmsSecureAccessType, struXMLBase, "CmsAccessSecurity", NODE_STRING_TO_BYTE);
            ConvertSingleNodeData(&m_byAlarmSecureAccessType, struXMLBase, "AlarmAccessSecurity", NODE_STRING_TO_BYTE);
            ConvertSingleNodeData(&m_byStreamSecureAccessType, struXMLBase, "StreamAccessSecurity", NODE_STRING_TO_BYTE);
            ConvertSingleNodeData(&m_byAlarmPictureSeparate, struXMLBase, "AlarmPictureSeparate", NODE_STRING_TO_INT);
            ConvertSingleNodeData(m_szDBKey, struXMLBase, "SSDataBaseKey", NODE_STRING_TO_ARRAY, 32);

            //ConvertSingleNodeData(m_sDasIP, struXMLBase, "DasServerIP", NODE_STRING_TO_ARRAY, 128);
            //ConvertSingleNodeData(&m_nDasPort, struXMLBase, "DasServerPort", NODE_STRING_TO_INT);
            struXMLBase.OutOfElem();
        }

        struXMLBase.OutOfElem();
    }

    //读取认证信息
    ReadDevAuthInfoFromXML();
}

void CEHomeDemoDlg::InitPreviewListenParam()
{
    int i = 0;
    //char szLocalIP[128] = {0};
    //GetLocalIP(szLocalIP);
    for (i=0; i<MAX_LISTEN_NUM; i++)
    {
        g_struPreviewListen[i].lHandle = -1;
        memcpy(g_struPreviewListen[i].struIP.szIP, m_sLocalIP, strlen(m_sLocalIP));
        g_struPreviewListen[i].struIP.wPort = 8000;
        g_struPreviewListen[i].iLinkType = 0;
    }
    //开启预览监听
    memset(g_bArrayEncrypt, FALSE, sizeof(g_bArrayEncrypt));

    NET_EHOME_LISTEN_PREVIEW_CFG struListen = { 0 };
    memcpy(&struListen.struIPAdress, &g_struPreviewListen[0].struIP, sizeof(NET_EHOME_IPADDRESS));
    struListen.fnNewLinkCB = fnPREVIEW_NEWLINK_CB;
    struListen.pUser = g_pMainDlg;
    struListen.byLinkMode = g_struPreviewListen[0].iLinkType;
    struListen.byLinkEncrypt = 0;//默认监听不加密

    LONG lHandle = NET_ESTREAM_StartListenPreview(&struListen);
    if (lHandle > -1)
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 2, "NET_ESTREAM_StartListenPreview");
        g_struPreviewListen[0].lHandle = lHandle;
        // GetDlgItem(IDC_BTN_START_LISTEN)->EnableWindow(FALSE);
        // GetDlgItem(IDC_BTN_STOP_LISTEN)->EnableWindow(TRUE);
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 2, "NET_ESTREAM_StartListenPreview");
    }
}

void CEHomeDemoDlg::StopPreviewListen()
{
    int i = 0;
    for (i=0; i<MAX_LISTEN_NUM; i++)
    {
        if(g_struPreviewListen[i].lHandle != -1)
        {
            NET_ESTREAM_StopListenPreview(g_struPreviewListen[i].lHandle);
            g_struPreviewListen[i].lHandle = -1;
        }
    }
}

void CEHomeDemoDlg::SetDeviceIndex(LONG iDeviceIndex)
{
    m_iCurDeviceIndex = iDeviceIndex;
    DeviceIDChanged();
}

void CEHomeDemoDlg::GetAddressByType(LONG iType, DWORD dwVersion, char *pOutValue, DWORD dwOutLen, char *pSubValue, DWORD dwSubLen) const
{
    CXmlBase   struXMLBase;
    struXMLBase.LoadFile(".\\EHomeDemo.xml");

    LONG iMapType = -1;
    if (struXMLBase.FindElem("LocalCfg") && struXMLBase.IntoElem())
    {
        iMapType = 0;
        if (struXMLBase.FindElem("AddressMap") && struXMLBase.IntoElem())
        {
            ConvertSingleNodeData(&iMapType, struXMLBase, "Enable", NODE_STRING_TO_INT, sizeof(iMapType));
            if (iMapType != 1 && iMapType != 0 && iMapType != -1)
            {
                iMapType = -1;
            }
        }
        else if (struXMLBase.FindElem("Param") && struXMLBase.IntoElem())
        {
            //暂时什么事也不用干
        }
        else
        {
            iMapType = -1;
        }
    }

    //优先使用映射地址
    if (DevOnServerAdd == iType)
    {
        FormatRegAddress(pOutValue, dwOutLen, dwVersion, iMapType, struXMLBase);
    }
    else if (StreamAdd == iType)
    {
        GetStreamAddress(pOutValue, dwOutLen, pSubValue, dwSubLen, iMapType, struXMLBase);
    }
    else if (AudioAdd == iType)
    {
        GetAudioAddress(pOutValue, dwOutLen, pSubValue, dwSubLen, iMapType, struXMLBase);
    }
    else if (PSSAdd == iType)
    {
        GetPicServerAddress(pOutValue, dwOutLen, pSubValue, dwSubLen, iMapType, struXMLBase);
    }
    else if (PlayBackAdd == iType)
    {
        GetPlayBackServerAddress(pOutValue, dwOutLen, pSubValue, dwSubLen, iMapType, struXMLBase);
    }
}

void CEHomeDemoDlg::FormatRegAddress(char *pOutValue, DWORD dwOutLen, DWORD dwVersion, LONG iMapType, CXmlBase &xmlBase) const
{

    NET_EHOME_SERVER_INFO_V50 *pServInfo = (NET_EHOME_SERVER_INFO_V50 *)pOutValue;
    //那么不管什么IP地址，都取demo开启时候的IP地址即可
    memcpy(pServInfo, &m_struServInfo, sizeof(m_struServInfo));
    if (dwVersion == 5)//Ehome5.0协议报警端口复用TCP端口
    {
        pServInfo->struTCPAlarmSever.wPort = m_wAlarmServerMqttPort;
    }

    //好了，根据映射类型和希望获取到的服务器类型，获取IP地址
    if (1 == iMapType)
    {
        //报警服务器
        ConvertSingleNodeData(pServInfo->struTCPAlarmSever.szIP, xmlBase, "AlarmServerIP", NODE_STRING_TO_ARRAY, sizeof(pServInfo->struTCPAlarmSever.szIP));
        ConvertSingleNodeData(pServInfo->struUDPAlarmSever.szIP, xmlBase, "AlarmServerIP", NODE_STRING_TO_ARRAY, sizeof(pServInfo->struUDPAlarmSever.szIP));

        ConvertSingleNodeData(&(pServInfo->struUDPAlarmSever.wPort), xmlBase, "AlarmServerUdpPort", NODE_STRING_TO_INT, sizeof(pServInfo->struUDPAlarmSever.wPort));
        ConvertSingleNodeData(&(pServInfo->struTCPAlarmSever.wPort), xmlBase, "AlarmServerTcpPort", NODE_STRING_TO_INT, sizeof(pServInfo->struTCPAlarmSever.wPort));
        if (dwVersion == 5)//Ehome5.0协议报警端口复用TCP端口
        {
            ConvertSingleNodeData(&(pServInfo->struTCPAlarmSever.wPort), xmlBase, "AlarmServerMqttPort", NODE_STRING_TO_INT, sizeof(pServInfo->struTCPAlarmSever.wPort));
        }

        // 图片服务器
        ConvertSingleNodeData(pServInfo->struPictureSever.szIP, xmlBase, "PictureServerIP", NODE_STRING_TO_ARRAY, 128);
        ConvertSingleNodeData(&(pServInfo->struPictureSever.wPort), xmlBase, "PictureServerPort", NODE_STRING_TO_WORD);
    }

    if (IsNULLAddress(pServInfo->struTCPAlarmSever.szIP))
    {
        GetLocalIP(pServInfo->struTCPAlarmSever.szIP);
    }
    if (IsNULLAddress(pServInfo->struUDPAlarmSever.szIP))
    {
        GetLocalIP(pServInfo->struTCPAlarmSever.szIP);
    }
    
}

void CEHomeDemoDlg::GetStreamAddress(char *pOutValue, DWORD dwOutLen, char *pSubValue, DWORD dwSubLen, LONG iMapType, CXmlBase &xmlBase) const
{
    if (-1 == iMapType || 0 == iMapType)
    {
        //获取本地监听地址
        strncpy(pOutValue, g_pMainDlg->m_sLocalIP, min(dwOutLen, sizeof(g_pMainDlg->m_sLocalIP)));
    }
    else
    {
        ConvertSingleNodeData(pOutValue, xmlBase, "StreamServerIP", NODE_STRING_TO_ARRAY, dwOutLen);
        ConvertSingleNodeData(pSubValue, xmlBase, "StreamServerPort", NODE_STRING_TO_INT, dwSubLen);
    }

    if (IsNULLAddress(pOutValue))
    {
        GetLocalIP(pOutValue);
    }
}

void CEHomeDemoDlg::GetPlayBackServerAddress(char *pOutValue, DWORD dwOutLen, char *pSubValue, DWORD dwSubLen, LONG iMapType, CXmlBase &xmlBase) const
{
    if (-1 == iMapType || 0 == iMapType)
    {
        //获取本地监听地址
        strncpy(pOutValue, g_pMainDlg->m_sLocalIP, min(dwOutLen, sizeof(g_pMainDlg->m_sLocalIP)));
    }
    else
    {
        ConvertSingleNodeData(pOutValue, xmlBase, "PlayBackServerIP", NODE_STRING_TO_ARRAY, dwOutLen);
        ConvertSingleNodeData(pSubValue, xmlBase, "PlayBackServerPort", NODE_STRING_TO_INT, dwSubLen);
    }

    if (IsNULLAddress(pOutValue))
    {
        GetLocalIP(pOutValue);
    }
}

void CEHomeDemoDlg::GetAudioAddress(char *pOutValue, DWORD dwOutLen, char *pSubValue, DWORD dwSubLen, LONG iMapType, CXmlBase &xmlBase) const
{
    if (-1 == iMapType || 0 == iMapType)
    {
        //获取本地监听地址
        strncpy(pOutValue, g_pMainDlg->m_sLocalIP, min(dwOutLen, sizeof(g_pMainDlg->m_sLocalIP)));
    }
    else
    {
        ConvertSingleNodeData(pOutValue, xmlBase, "AudioServerIP", NODE_STRING_TO_ARRAY, dwOutLen);
        ConvertSingleNodeData(pSubValue, xmlBase, "AudioServerPort", NODE_STRING_TO_INT, dwSubLen);
    }

    if (IsNULLAddress(pOutValue))
    {
        GetLocalIP(pOutValue);
    }
}

void CEHomeDemoDlg::GetDasAddress(char *pOutValue, DWORD dwOutLen, char *pSubValue, DWORD dwSubLen, LONG iMapType, CXmlBase &xmlBase) const
{
    if (-1 == iMapType || 0 == iMapType)
    {
        //获取本地监听地址
        strncpy(pOutValue, g_pMainDlg->m_sLocalIP, min(dwOutLen, sizeof(g_pMainDlg->m_sLocalIP)));
    }
    else
    {
        ConvertSingleNodeData(pOutValue, xmlBase, "DASIP", NODE_STRING_TO_ARRAY, dwOutLen);
        ConvertSingleNodeData(pSubValue, xmlBase, "DASPort", NODE_STRING_TO_INT, dwSubLen);
    }

    if (IsNULLAddress(pOutValue))
    {
        GetLocalIP(pOutValue);
    }
}

void CEHomeDemoDlg::GetPicServerAddress(char *pOutValue, DWORD dwOutLen, char *pSubValue, DWORD dwSubLen, LONG iMapType, CXmlBase &xmlBase) const
{
    if (-1 == iMapType || 0 == iMapType)
    {
        //获取本地监听地址
        strncpy(pOutValue, g_pMainDlg->m_sLocalIP, min(dwOutLen, sizeof(g_pMainDlg->m_sLocalIP)));
    }
    else
    {
        ConvertSingleNodeData(pOutValue, xmlBase, "PictureServerIP", NODE_STRING_TO_ARRAY, dwOutLen);
        ConvertSingleNodeData(pSubValue, xmlBase, "PictureServerPort", NODE_STRING_TO_INT, dwSubLen);
    }

    if (IsNULLAddress(pOutValue))
    {
        GetLocalIP(pOutValue);
    }
}

BOOL CEHomeDemoDlg::IsNULLAddress(char *pAddress) const
{
    if (strlen(pAddress) == 0 || memcmp(pAddress, "0.0.0.0", strlen("0.0.0.0")) == 0)
    {
        return TRUE;
    }

    return FALSE;
}

void CALLBACK CEHomeDemoDlg::fnPREVIEW_DATA_CB(LONG  iPreviewHandle, NET_EHOME_PREVIEW_CB_MSG *pPreviewCBMsg, void *pUserData)
{
    UN_REFERENCED_PARAMETER(iPreviewHandle)
    //int iWinIndex = *(int*)pUserData;
    int iWinIndex = (int)pUserData;
    if (NULL == pPreviewCBMsg)
    {
        return ;
    }
    g_pMainDlg->m_pDlgPreview[iWinIndex].InputStreamData(pPreviewCBMsg->byDataType, (char*)pPreviewCBMsg->pRecvdata, pPreviewCBMsg->dwDataLen);

    //测试新增接口，将回调中收到的码流数据转发给设备
    /*
    if (NET_ESTREAM_SendRealStreamData(iPreviewHandle, pPreviewCBMsg->byDataType, (char*)pPreviewCBMsg->pRecvdata, pPreviewCBMsg->dwDataLen) < 0)
    {
        g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_FAIL_T, 2, "NET_ESTREAM_SendRealStreamData failed. lPreviewHandle[%d], m_iCurWndIndex[%d] ", iPreviewHandle, g_pMainDlg->m_iCurWndIndex);
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_SUCC_T, 2, "NET_ESTREAM_SendRealStreamData succ. lPreviewHandle[%d], m_iCurWndIndex[%d] ", iPreviewHandle, g_pMainDlg->m_iCurWndIndex);
    }
    */
}

BOOL CALLBACK CEHomeDemoDlg::fnPREVIEW_NEWLINK_CB(LONG lPreviewHandle,NET_EHOME_NEWLINK_CB_MSG *pNewLinkCBMsg, void *pUserData)
{
    //需要将字符串字段转换成GB2312
    DWORD dwConvertLen = 0;
    UTF82A((char*)pNewLinkCBMsg->szDeviceID, (char*)pNewLinkCBMsg->szDeviceID, MAX_DEVICE_ID_LEN, &dwConvertLen);


    CEHomeDemoDlg *pThis = (CEHomeDemoDlg*)pUserData;
   int iIndex = -1;
    for (int i = 0; i < MAX_DEVICES; i++)
    {
        if (strcmp((char*)pNewLinkCBMsg->szDeviceID, (char*)g_struDeviceInfo[i].byDeviceID) == 0)
        {
            iIndex = i;
            break;
        }
        for (int j = 0; j < (int)(g_struDeviceInfo[i].dwAnalogChanNum + g_struDeviceInfo[i].dwIPChanNum); j++)
        {
            if (g_struDeviceInfo[i].struChanInfo[j].dwPreviewSessionId == (DWORD)pNewLinkCBMsg->iSessionID)
            {
                iIndex = i;
                break;
            }
        }
        if (iIndex >= 0)
        {
            break;
        }
    }
    if (iIndex<0)
    {
        AfxMessageBox("The device has not registered to DAS, please config preview channel info.");
        return FALSE;
    }
    
   // struDataCB.byEncrypt = g_struDeviceInfo[iIndex].struChanInfo[pNewLinkCBMsg->dwChannelNo].bEnable;
    //memcpy(struDataCB.byEhomeKey, g_struDeviceInfo[iIndex].byEhomeKey, 32);

    if(!g_pMainDlg->m_pDlgPreview[g_pMainDlg->m_iCurWndIndex].SetPlayParam(lPreviewHandle, g_pMainDlg->m_iCurWndIndex, pNewLinkCBMsg->iSessionID))
    {
        g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_FAIL_T, 2, "SetPlayParam failed. lPreviewHandle[%d],\
                                                                              m_iCurWndIndex[%d] ", lPreviewHandle, g_pMainDlg->m_iCurWndIndex);
        return FALSE;
    }

    //if (g_pMainDlg->m_bRtpAudio) //当仅仅使用RTP音频数据时
    {
        pNewLinkCBMsg->byStreamFormat = g_struDeviceInfo[iIndex].struChanInfo[pNewLinkCBMsg->dwChannelNo].dwStreamFormat;
    }
    pNewLinkCBMsg->fnPreviewDataCB = fnPREVIEW_DATA_CB;
    pNewLinkCBMsg->pUserData = (void*)g_pMainDlg->m_iCurWndIndex;

    //::PostMessage(pThis->m_hWnd, WM_SETPREVIEW, NULL, (LPARAM)lPreviewHandle);
    g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_SUCC_T, 2, "Device ID = [%s]", pNewLinkCBMsg->szDeviceID);
    SetEvent(g_pMainDlg->m_pDlgPreview[g_pMainDlg->m_iCurWndIndex].m_hPlayEvent);
    if (pThis->m_bCyclePreview)
    {
        g_pMainDlg->m_iCurWndIndex++;
    }
    return TRUE;
}

void CALLBACK CEHomeDemoDlg::fnPREVIEW_EXCEPTION_CB(DWORD dwType, LONG iUserID, LONG iHandle, void* pUser)
{
     UN_REFERENCED_PARAMETER(pUser)

     DWORD dwError = NET_ESTREAM_GetLastError();
     g_pMainDlg->m_dwType = dwType;
    if(EHOME_PREVIEW_EXCEPTION == dwType)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "Preview exception, handle=%d, Error:%d", iHandle, dwError);
        ::PostMessage(g_pMainDlg->m_hWnd, WM_PROC_EXCEPTION, dwType, (LPARAM)iHandle);
    }
    else if (EHOME_PLAYBACK_EXCEPTION == dwType)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "Playback exception, handle=%d, Error:%d", iHandle, dwError);
        ::PostMessage(g_pMainDlg->m_hWnd, WM_PROC_EXCEPTION, dwType, (LPARAM)iHandle);
    }
    else if (EHOME_AUDIOTALK_EXCEPTION == dwType)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "AudioVoice exception, handle=%d, Error:%d", iHandle, dwError);
        ::PostMessage(g_pMainDlg->m_hWnd, WM_PROC_EXCEPTION, dwType, (LPARAM)iHandle);
    }
}

LRESULT CEHomeDemoDlg::OnWMProcException(WPARAM wParam, LPARAM lParam)
{
    UN_REFERENCED_PARAMETER(wParam)
    int iHandle = lParam;
    int iType = wParam;
    if (EHOME_PREVIEW_EXCEPTION == iType)
    {
        if (!NET_ESTREAM_StopPreview(iHandle))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "OnWMProcException NET_ESTREAM_StopPreview failed");
        }
    }
    else if (EHOME_PLAYBACK_EXCEPTION == iType)
    {
        if (!NET_ESTREAM_StopPlayBack(iHandle))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "OnWMProcException NET_ESTREAM_StopPlayBack failed");
        }
    }
    else if (EHOME_AUDIOTALK_EXCEPTION == iType)
    {
        if (!NET_ESTREAM_StopVoiceTalk(iHandle))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "OnWMProcException NET_ESTREAM_StopVoiceTalk failed");
        }
    }
    
    return TRUE;
}

void CEHomeDemoDlg::SetStreamExceptionCallBack()
{
    NET_ESTREAM_SetExceptionCallBack(0, 0, fnPREVIEW_EXCEPTION_CB, this);
}

void CEHomeDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEHomeDemoDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEHomeDemoDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CEHomeDemoDlg::InitChildWindow()
{
    m_pDlgPreview = new CDlgOutput[MAX_WIN_NUM];
    if (NULL == m_pDlgPreview)
    {
        return;
    }
    for (int i = 0; i < MAX_WIN_NUM; i++)
    {
        m_pDlgPreview[i].m_iWndIndex = i;
        m_pDlgPreview[i].Create(IDD_DLG_OUTPUT,this);
        m_pDlgPreview[i].m_pFatherDlg = this;
    }

    m_dlgPlayBack = new CDlgPlayBack;
    m_dlgPlayBack->Create(IDD_DLG_PLAY_BACK,this);
    m_dlgPlayBack->ShowWindow(SW_HIDE);

    m_dlgAudioTalk = new CDlgAudioTalk;
    m_dlgAudioTalk->Create(IDD_DLG_AUDIO_TALK,this);
    m_dlgAudioTalk->ShowWindow(SW_HIDE);

    m_dlgCommonCfg = new CDlgCommonCfg;
    m_dlgCommonCfg->Create(IDD_DLG_COMMON_CFG,this);
    m_dlgCommonCfg->ShowWindow(SW_HIDE);

    m_dlgLocalCfg = new CDlgLocalCfg;
    m_dlgLocalCfg->Create(IDD_DLG_LOCAL_CFG,this);
    m_dlgLocalCfg->ShowWindow(SW_HIDE);

    m_dlgOutputCtrl = new CDlgOutputCtrl;
    m_dlgOutputCtrl->Create(IDD_DLG_OUTPUT_CTRL,this);
    m_dlgOutputCtrl->ShowWindow(SW_HIDE);

    m_dlgHttpAlarm = new CDlgHttpAlarm;
    m_dlgHttpAlarm->Create(IDD_DLG_HTTPALARM, this);
    m_dlgHttpAlarm->ShowWindow(SW_HIDE);

    //m_pDlgVI = NULL;
    m_pDlgVI = new CDlgVideoIntercomCall;
    m_pDlgVI->Create(IDD_DLG_VIDEO_INTERCOM_CALL, this);
    m_pDlgVI->ShowWindow(SW_HIDE);
}

void CEHomeDemoDlg::InitMainWin()
{
    m_iTreeWidth = 165;        //device tree width
    m_iRightWidth = 170;    //PTZ pic box width
    m_iFunBtnHeight = 70;     //column height
    m_iListLogHeight = 110 ;//list log height
    char szLan[128] = {0};
    CString csTemp = _T("");

    m_listAlarmInfo.InsertColumn(0,csTemp,LVCFMT_LEFT,0,-1); 
    g_StringLanType(szLan, "时间", "Time");
    csTemp = szLan;
    m_listAlarmInfo.InsertColumn(1, csTemp,LVCFMT_LEFT,150);
    g_StringLanType(szLan, "报警信息", "Alarm Info");
    csTemp = szLan;
    m_listAlarmInfo.InsertColumn(2, csTemp,LVCFMT_LEFT,350);
    g_StringLanType(szLan, "设备信息", "Device Info");
    csTemp = szLan;
    m_listAllLog.InsertColumn(4, csTemp, LVCFMT_LEFT, 140);
    g_StringLanType(szLan, "错误信息", "Error Info");
    csTemp = szLan;
    m_listAllLog.InsertColumn(5, csTemp, LVCFMT_LEFT, 80);
    m_listAllLog.SetExtendedStyle(m_listAllLog.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);


    OnBnClickedRadioLocalLog();

    memset(szLan, 0, sizeof(szLan));
    csTemp = szLan;
    m_listAllLog.InsertColumn(0, csTemp, LVCFMT_LEFT, 0, -1); 
    g_StringLanType(szLan, "时间", "Time");
    csTemp = szLan;
    m_listAllLog.InsertColumn(1, csTemp, LVCFMT_LEFT, 120);
    g_StringLanType(szLan, "状态", "State");
    csTemp = szLan;
    m_listAllLog.InsertColumn(2, csTemp, LVCFMT_LEFT, 40);
    g_StringLanType(szLan, "操作", "Operation");
    csTemp = szLan;
    m_listAllLog.InsertColumn(3, csTemp, LVCFMT_LEFT, 300);
    g_StringLanType(szLan, "设备信息", "Device Info");
    csTemp = szLan;
    m_listAllLog.InsertColumn(4,csTemp,LVCFMT_LEFT,140);
    g_StringLanType(szLan, "错误信息", "Error Info");
    csTemp = szLan;
    m_listAllLog.InsertColumn(5,csTemp,LVCFMT_LEFT,80);
    m_listAllLog.SetExtendedStyle(m_listAllLog.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);

    m_comboWinNum.ResetContent();
    csTemp = _T("1");
    m_comboWinNum.AddString(csTemp);
    csTemp = _T("4");
    m_comboWinNum.AddString(csTemp);
    csTemp = _T("9");
    m_comboWinNum.AddString(csTemp);
    csTemp = _T("16");
    m_comboWinNum.AddString(csTemp);
    csTemp = _T("25");
    m_comboWinNum.AddString(csTemp);
    csTemp = _T("36");
    m_comboWinNum.AddString(csTemp);
    csTemp = _T("49");
    m_comboWinNum.AddString(csTemp);
    csTemp = _T("64");
    m_comboWinNum.AddString(csTemp);

    m_comboWinNum.SetCurSel(1);

    m_iSelListType = 0;//local log
    ((CButton *)GetDlgItem(IDC_RADIO_LOCAL_LOG))->SetCheck(TRUE);
    ((CButton *)GetDlgItem(IDC_RADIO_ALARM_INFO))->SetCheck(FALSE);
    //OnSelchangeComboListType();
    MoveWindow(0, 0, DEMO_FULL_WIDTH, DEMO_FULL_HEIGHT, TRUE);

    InitPreviewRect();//preview window, part of main window
}

void CEHomeDemoDlg::CreateTree()
{
    m_treeDeviceList.DeleteAllItems();
    char szLan[128] = {0};
    g_StringLanType(szLan, "设备树", "Device Tree");
    HTREEITEM hRoot = m_treeDeviceList.InsertItem(szLan, TREE_ALL, TREE_ALL);

    m_treeDeviceList.Expand(hRoot,TVE_EXPAND);
    m_treeDeviceList.Expand(m_treeDeviceList.GetRootItem(),TVE_EXPAND);
}

void CEHomeDemoDlg::InitPreviewRect()
{
    m_rectPreviewBG.top    = m_iFunBtnHeight;
    m_rectPreviewBG.left   = m_iTreeWidth;
    m_rectPreviewBG.right  = DEMO_FULL_WIDTH - m_iRightWidth;
    m_rectPreviewBG.bottom = m_rectPreviewBG.Width()*3/4 + m_iFunBtnHeight+4;//+4

    m_rectRightArea.top    = m_iFunBtnHeight - 3;
    m_rectRightArea.left   = DEMO_FULL_WIDTH - m_iRightWidth + 5;//
    m_rectRightArea.right  = DEMO_FULL_WIDTH;//
    m_rectRightArea.bottom = m_rectPreviewBG.Width()*3/4 + m_iFunBtnHeight + 4; //+55button height

    GetDlgItem(IDC_STATIC_PREVIEWBG)->MoveWindow(&m_rectPreviewBG,TRUE);

    m_dlgOutputCtrl->MoveWindow(&m_rectRightArea,TRUE);
    m_dlgOutputCtrl->ShowWindow(SW_SHOW);

    //output box layout
    ArrangeOutputs(m_iCurWndNum);//
}

void CEHomeDemoDlg::ArrangeOutputs(int iNumber)
{
    if (iNumber == 0)
    {
        //AddLog(m_iCurDeviceIndex, OPERATION_FAIL_T, "ArrangeOutputs number=0!");
        return;
    }
    int i = 0;
    CRect crect;

    int iSqrtNum = 0;//sqrt value of window number
    int iWidth = 0;//window width
    int iHeight = 0;//window height

    iSqrtNum = (int)sqrt((double)iNumber);

    for (i = 0;i < MAX_WIN_NUM;i++)
    {
        m_pDlgPreview[i].ShowWindow(SW_HIDE);    
    }

    
        iWidth = (m_rectPreviewBG.Width()-OUTPUT_INTERVAL*(iSqrtNum-1))/iSqrtNum;//a single pic width in partition
        iHeight = (m_rectPreviewBG.Height()-OUTPUT_INTERVAL*(iSqrtNum-1))/iSqrtNum;//a single pic height in partition
        int iPlayIndex = 0;
        for (i = 0; i < iNumber; i++)
        {
            
            if (g_struLocalParam.bEnlarged)
            {//double click to zoom some pic, iNumber = 1
                iPlayIndex = m_iCurWndIndex;
            }
            else
            {
                iPlayIndex = i;
            }
            m_pDlgPreview[iPlayIndex].MoveWindow(m_iTreeWidth + (i%iSqrtNum)*(iWidth+OUTPUT_INTERVAL),\
                m_iFunBtnHeight+(i/iSqrtNum)*(iHeight+OUTPUT_INTERVAL),iWidth,iHeight,TRUE);            
            m_pDlgPreview[iPlayIndex].ShowWindow(SW_SHOW);
            m_pDlgPreview[iPlayIndex].DrawOutputBorder();
        }    
}


/*********************************************************
Function:    LoadTreeImage
Desc:        load tree iamge
Input:    
Output:    
Return:    
**********************************************************/
#define MAX_BMPS 14
void CEHomeDemoDlg::LoadTreeImage(void)
{
    CBitmap cBmp[MAX_BMPS];

    m_imageTreeList.Create(16,16,ILC_COLOR32 | ILC_MASK,1,1);

    cBmp[TREE_ALL].LoadBitmap(IDB_BITMAP_TREE);
    m_imageTreeList.Add(&cBmp[TREE_ALL],RGB(1,1,1));

    cBmp[DEVICE_LOGOUT].LoadBitmap(IDB_BITMAP_LOGOUT);
    m_imageTreeList.Add(&cBmp[DEVICE_LOGOUT],RGB(1,1,1));
    cBmp[DEVICE_LOGIN].LoadBitmap(IDB_BITMAP_LOGIN);
    m_imageTreeList.Add(&cBmp[DEVICE_LOGIN],RGB(1,1,1));
    cBmp[DEVICE_FORTIFY].LoadBitmap(IDB_BITMAP_FORTIFY);
    m_imageTreeList.Add(&cBmp[DEVICE_FORTIFY],RGB(1,1,1));
    cBmp[DEVICE_ALARM].LoadBitmap(IDB_BITMAP_DEV_ALARM);
    m_imageTreeList.Add(&cBmp[DEVICE_ALARM],RGB(1,1,1));

    cBmp[DEVICE_FORTIFY_ALARM].LoadBitmap(IDB_BITMAP_FORTIFY_ALARM);
    m_imageTreeList.Add(&cBmp[DEVICE_FORTIFY_ALARM],RGB(1,1,1));

    cBmp[CHAN_ORIGINAL].LoadBitmap(IDB_BITMAP_CAMERA);
    m_imageTreeList.Add(&cBmp[CHAN_ORIGINAL],RGB(1,1,1));
    cBmp[CHAN_PLAY].LoadBitmap(IDB_BITMAP_PLAY);
    m_imageTreeList.Add(&cBmp[CHAN_PLAY],RGB(1,1,1));
    cBmp[CHAN_RECORD].LoadBitmap(IDB_BITMAP_REC);
    m_imageTreeList.Add(&cBmp[CHAN_RECORD],RGB(1,1,1));
    cBmp[CHAN_PLAY_RECORD].LoadBitmap(IDB_BITMAP_PLAYANDREC);
    m_imageTreeList.Add(&cBmp[CHAN_PLAY_RECORD],RGB(1,1,1));

    cBmp[CHAN_ALARM].LoadBitmap(IDB_BITMAP_ALARM);
    m_imageTreeList.Add(&cBmp[CHAN_ALARM],RGB(1,1,1));
    cBmp[CHAN_PLAY_ALARM].LoadBitmap(IDB_BITMAP_PLAY_ALARM);
    m_imageTreeList.Add(&cBmp[CHAN_PLAY_ALARM],RGB(1,1,1));

    cBmp[CHAN_PLAY_RECORD_ALARM].LoadBitmap(IDB_BITMAP_P_R_A);
    m_imageTreeList.Add(&cBmp[CHAN_PLAY_RECORD_ALARM],RGB(1,1,1));

    cBmp[CHAN_OFF_LINE].LoadBitmap(IDB_BITMAP_CHAN_OFF);
    m_imageTreeList.Add(&cBmp[CHAN_OFF_LINE],RGB(1,1,1));

    m_treeDeviceList.SetImageList(&m_imageTreeList, LVSIL_NORMAL);
}
void CEHomeDemoDlg::OnBnClickedRadioAlarmInfo()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    m_iSelListType = 1;
    OnSelchangeComboListType();
    UpdateData(FALSE);    
}

void CEHomeDemoDlg::OnBnClickedRadioLocalLog()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    m_iSelListType = 0;
    OnSelchangeComboListType();
    UpdateData(FALSE);
}

void CEHomeDemoDlg::OnSelchangeComboListType()
{
    UpdateData(TRUE);
    ListRestore();
//     if (m_iMainType == CONFIG_ALL_T)
//     {
//         m_dlgConfigAll->ConfigWndUpdate();            
//     }
//     else if (m_iMainType == PLAY_BACK_T)
//     {
//         m_dlgPlayBack->ShowWindow(SW_SHOW);
//         m_dlgPlayBack->PlayBackWinUpdate();
//     }
//     else if (m_iMainType == PREVIEW_T)
//     {
//         PreviewReferShow(TRUE);
//     }
    switch (m_iSelListType)
    {
    case 0:
        m_listAlarmInfo.ShowWindow(SW_HIDE);
        m_listAllLog.ShowWindow(SW_SHOW);    
        break;
    case 1:        
        m_listAllLog.ShowWindow(SW_HIDE);
        m_listAlarmInfo.ShowWindow(SW_SHOW);
        break;
    default:
        break;
    }

}

void CEHomeDemoDlg::ListRestore()
{
    CRect rc(0,0,0,0);
    if (m_bListLogEnlarge)
    {
        m_listAllLog.GetWindowRect(&rc);
        ScreenToClient(&rc);
        rc.top+=LIST_ENLARGE_HIGH;//move top down, compress
        m_listAllLog.MoveWindow(&rc);
        m_bListLogEnlarge = FALSE;
    }

    if (m_bListAlarmEnlarge)
    {
        m_listAlarmInfo.GetWindowRect(&rc);
        ScreenToClient(&rc);
        rc.top+=LIST_ENLARGE_HIGH;//move top down, compress
        m_listAlarmInfo.MoveWindow(&rc);
        m_bListAlarmEnlarge = FALSE;
    }
}
void CEHomeDemoDlg::OnCbnSelchangeComboWndnum()
{
    // TODO: Add your control notification handler code here
    int iIndex = m_comboWinNum.GetCurSel();
    m_iCurWndNum = (int)pow(double(iIndex+1), 2);//current window number
    if (!IsPlaying())
    {
        m_iCurWndIndex = 0;//initialize current window index while switch window
        ArrangeOutputs(m_iCurWndNum);
        GetDlgItem(IDC_STATIC_PREVIEWBG)->Invalidate(TRUE);
    }
    else
    {
        char szLan[128] = {0};
        g_StringLanType(szLan, "请先停止播放", "Please stop previewing");
        AfxMessageBox(szLan); 
    }
}

/*********************************************************
Function:    IsPlaying
Desc:        whether any window is previewing
Input:    
Output:    
Return:    
**********************************************************/
BOOL CEHomeDemoDlg::IsPlaying(void)
{
    if (m_iMainType == PREVIEW_T)
    {
        for (int i = 0; i < MAX_WIN_NUM; i ++)
        {
            if (m_pDlgPreview[i].m_bPlay)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void CEHomeDemoDlg::OnLvnColumnclickListAllLog(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    UN_REFERENCED_PARAMETER(pNMHDR)
    EnlargeList(m_listAllLog, m_bListLogEnlarge);

    *pResult = 0;
}

void CEHomeDemoDlg::OnLvnColumnclickListAlarmInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    UN_REFERENCED_PARAMETER(pNMHDR)
    EnlargeList(m_listAlarmInfo, m_bListAlarmEnlarge);
    *pResult = 0;
}

/*********************************************************
Function:    EnlargeList
Desc:        enlarge list when click the column
Input:    list, log or alarm information tree; bEnargeList, TRUE/FALSE;
Output:    none
Return:    none
**********************************************************/
void CEHomeDemoDlg::EnlargeList(CListCtrl &list, BOOL &bEnargeList)
{
    CRect rc(0,0,0,0);
    list.GetWindowRect(&rc);
    ScreenToClient(&rc);

    if (!bEnargeList)
    {
        rc.top-=LIST_ENLARGE_HIGH;//move top upper, stretch
        list.MoveWindow(&rc);
        bEnargeList = TRUE;
        if (m_iMainType == PLAY_BACK_T)
        {
            m_dlgPlayBack->ShowWindow(SW_HIDE);
        }
        else if (m_iMainType == AUDIO_TALK_T)
        {
            m_dlgAudioTalk->ShowWindow(SW_HIDE);    
        }
        else if(m_iMainType == COM_CONFIG_T)
        {
            m_dlgCommonCfg->ShowWindow(SW_HIDE);
        }
        else if (m_iMainType == LOCAL_CONFIG_T)
        {
            m_dlgLocalCfg->ShowWindow(SW_HIDE);
        }
        else
        {
            PreviewReferShow(FALSE);
        }
    }
    else
    {
        rc.top+=LIST_ENLARGE_HIGH;//move top down, compress
        list.MoveWindow(&rc);
        bEnargeList = FALSE;
        if (m_iMainType == PLAY_BACK_T)
        {
            m_dlgPlayBack->ShowWindow(SW_SHOW);
        }
        else if (m_iMainType == AUDIO_TALK_T)
        {
            m_dlgAudioTalk->ShowWindow(SW_SHOW);    
        }
        else if(m_iMainType == COM_CONFIG_T)
        {
            m_dlgCommonCfg->ShowWindow(SW_SHOW);
        }
        else if (m_iMainType == LOCAL_CONFIG_T)
        {
            m_dlgLocalCfg->ShowWindow(SW_SHOW);
        }
        else
        {
            PreviewReferShow(TRUE);            
        }
    }
}

void CEHomeDemoDlg::PreviewReferShow(BOOL bShow)
{
    int iShowStat = bShow?SW_SHOW:SW_HIDE;

    if (bShow)
    {
        ArrangeOutputs(m_iCurWndNum);
    }
    else
    {            
        for (int i=0; i<MAX_WIN_NUM; i++)
        {
            m_pDlgPreview[i].ShowWindow(SW_HIDE);
        }
    }
    GetDlgItem(IDC_STATIC_PREVIEWBG)->ShowWindow(iShowStat);
    GetDlgItem(IDC_STATIC_WINNUM)->ShowWindow(iShowStat);        
    GetDlgItem(IDC_COMBO_WNDNUM)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_PREVIEW_LISTEN)->ShowWindow(iShowStat);
    //GetDlgItem(IDC_BTN_CIRCLE_PREVIEW)->ShowWindow(iShowStat);
    GetDlgItem(IDC_EDIT_PREVIEW_HANDLE)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_CLOSE_PREVIEW)->ShowWindow(iShowStat);
    m_dlgOutputCtrl->ShowWindow(iShowStat);
#ifndef DEMO_LAN_EN
    GetDlgItem(IDC_BTN_IFRAME)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_STREAM_ENCRYPT)->ShowWindow(iShowStat);
#endif
    GetDlgItem(IDC_BTN_SALVE)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_SOUND)->ShowWindow(iShowStat);
}

void CEHomeDemoDlg::MoveChildWin(DWORD dwWinType)
{
    CRect rect(0,0,0,0);    

    rect.top    = m_iFunBtnHeight-2;
    rect.left   = m_iTreeWidth-2;//-2 cover preview box
    rect.right  = DEMO_FULL_WIDTH - m_iRightWidth ;
    rect.bottom = DEMO_FULL_HEIGHT - m_iListLogHeight - 25;//+4

    switch (dwWinType)
    {
    case PLAY_BACK_T:
        m_dlgPlayBack->MoveWindow(&rect,TRUE);
        break;
    case AUDIO_TALK_T:
        m_dlgAudioTalk->MoveWindow(&rect,TRUE);
        break;
    case COM_CONFIG_T:
        m_dlgCommonCfg->MoveWindow(&rect,TRUE);
        break;
    case LOCAL_CONFIG_T:
        m_dlgLocalCfg->MoveWindow(&rect,TRUE);
        break;
    default:
        break;
    }
}

void CEHomeDemoDlg::OnBnClickedBtnPlayback()
{
    // TODO: Add your control notification handler code here
    if (m_iMainType != PLAY_BACK_T)
    {
        m_iMainType = PLAY_BACK_T;
        m_dlgPlayBack->ShowWindow(SW_SHOW);
        m_dlgAudioTalk->ShowWindow(SW_HIDE);
        m_dlgCommonCfg->ShowWindow(SW_HIDE);
        m_dlgLocalCfg->ShowWindow(SW_HIDE);
        PreviewReferShow(FALSE);

        MoveChildWin(m_iMainType);    
    }
}

void CEHomeDemoDlg::OnBnClickedBtnPreview()
{
    // TODO: Add your control notification handler code here
    if (m_iMainType != PREVIEW_T)
    {
        //modify preview flag
        m_iMainType = PREVIEW_T;
        m_dlgPlayBack->ShowWindow(SW_HIDE);
        m_dlgAudioTalk->ShowWindow(SW_HIDE);
        m_dlgCommonCfg->ShowWindow(SW_HIDE);
        m_dlgLocalCfg->ShowWindow(SW_HIDE);
        PreviewReferShow(TRUE);
    }
}

void CEHomeDemoDlg::OnBnClickedBtnAudioTalk()
{
    // TODO: Add your control notification handler code here
    if (m_iMainType != AUDIO_TALK_T)
    {
        m_iMainType = AUDIO_TALK_T;
        m_dlgPlayBack->ShowWindow(SW_HIDE);
        m_dlgAudioTalk->ShowWindow(SW_SHOW);
        m_dlgCommonCfg->ShowWindow(SW_HIDE);
        m_dlgLocalCfg->ShowWindow(SW_HIDE);
        PreviewReferShow(FALSE);

        MoveChildWin(m_iMainType);        

        m_dlgAudioTalk->CheckInitParam();
    }
}

void CEHomeDemoDlg::OnBnClickedBtnCfg()
{
    // TODO: Add your control notification handler code here
    if (m_iMainType != COM_CONFIG_T)
    {
        m_iMainType = COM_CONFIG_T;
        m_dlgPlayBack->ShowWindow(SW_HIDE);
        m_dlgAudioTalk->ShowWindow(SW_HIDE);
        m_dlgCommonCfg->ShowWindow(SW_SHOW);
        m_dlgLocalCfg->ShowWindow(SW_HIDE);
        PreviewReferShow(FALSE);

        MoveChildWin(m_iMainType);        

        m_dlgCommonCfg->OnBnClickedBtnRefresh();
    }

    CMenu pMenu;
    CRect rectBtnElse(0, 0, 0, 0);
    GetDlgItem(IDC_BTN_CFG)->GetWindowRect(&rectBtnElse);

    if (!pMenu.LoadMenu(IDR_MENU_CONFIG))
    {
        return;
    }

    pMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, rectBtnElse.left, rectBtnElse.bottom, this);
}

void CEHomeDemoDlg::OnBnClickedBtnLocalCfg()
{
    // TODO: Add your control notification handler code here
    if (m_iMainType != LOCAL_CONFIG_T)
    {
        m_iMainType = LOCAL_CONFIG_T;
        m_dlgPlayBack->ShowWindow(SW_HIDE);
        m_dlgAudioTalk->ShowWindow(SW_HIDE);
        m_dlgCommonCfg->ShowWindow(SW_HIDE);
        m_dlgLocalCfg->ShowWindow(SW_SHOW);
        PreviewReferShow(FALSE);

        MoveChildWin(m_iMainType);        
    }
}

void CEHomeDemoDlg::OnBnClickedBtnExit()
{
    // TODO: Add your control notification handler code here
    char szLan[128] = {0};
    g_StringLanType(szLan, "确定要退出吗?", "Sure to exit?");
    if (IDOK != MessageBox(szLan,"Warning",IDOK))
    {
        return;
    }
    g_bExitDemo = TRUE;//make not post message
    StopPlayAll();
    CDialog::OnCancel();
}


void CEHomeDemoDlg::OnCancel()
{
    // TODO: Add your specialized code here and/or call the base class
    StopPlayAll();
    OnBnClickedBtnExit();    
}



/*********************************************************
Function:    AddLog
Desc:        add local log
Input:    iLogType, log type, 0-alam, 1-operate log, 2-debug info; csLogInfo log info
Output:    
Return:    
**********************************************************/
/** @fn void CEHomeDemoDlg::AddLog(int iDeviceIndex, int iLogType, int iComType, const char* format, ...)
 *  @brief
 *  @param (in)    int iDeviceIndex    
 *  @param (in)    int iLogType    
 *  @param (in)    int iComType 模块类型，0-模块无关，1-CMS模块，2-STREAM模块，3-ALARM模块
 *  @param (in)    const char * format    
 *  @param (in)    ...    
 *  @return void
 */
void CEHomeDemoDlg::AddLog(int iDeviceIndex, int iLogType, int iComType, const char* format, ...)
{    
    if (g_bExitDemo)
    {
        return;
    }
    CTime  cTime = CTime::GetCurrentTime();
    char szTime[64] = {0};
    char szLogType[32] = "FAIL";
    char szLogInfo[1024] = {0};
    char szDevInfo[256] = {0};
    char szErrInfo[256] = {0};
    char szLog[1024] = {0};
    va_list arglist;
    va_start(arglist,format);
    vsprintf(szLogInfo,format,arglist); 
    va_end(arglist);

    sprintf(szTime, "%s", cTime.Format("%y-%m-%d %H:%M:%S").GetBuffer(0));

    if (iDeviceIndex != -1 && iDeviceIndex < 512)
    {
        sprintf(szDevInfo, "[%s]", g_struDeviceInfo[iDeviceIndex].byDeviceID);
    }

    switch (iLogType)
    {
    case OPERATION_SUCC_T:
    case PLAY_SUCC_T:
        sprintf(szErrInfo, "");
        sprintf(szLogType, "SUCC");
        break;
    case PLAY_FAIL_T:
        sprintf(szErrInfo, "PLAY_M4 Eorror!!!");    
        break;
    case OPERATION_FAIL_T:
    default:
        switch (iComType)
        {
        case 1:
            sprintf(szErrInfo, "CMS_ERR[%d]", NET_ECMS_GetLastError());
            break;
        case 2:
            sprintf(szErrInfo, "PREVIEW_ERR[%d]", NET_ESTREAM_GetLastError());
            break;
        case 3:
            sprintf(szErrInfo, "ALARM_ERR[%d]", NET_EALARM_GetLastError());
            break;
        case 4:
            sprintf(szErrInfo, "SS_ERR[%d]", NET_ESS_GetLastError());
            break;
        default:
            break;
        }
        break;
    }
    //sprintf(szErrInfo, "err[%d:%s]", NET_DVR_GetLastError(), NET_DVR_GetErrorMsg());
    

    if (g_struLocalParam.bOutputDebugString)
    {
        OutputDebugString(szLog);
    }
    if (!g_struLocalParam.bSuccLog && !g_struLocalParam.bFailLog && (iLogType != ALARM_INFO_T ))
    {
        return;
    }
    LPLOCAL_LOG_INFO pLogInfo = NULL;
    try
    {  

        pLogInfo = new LOCAL_LOG_INFO;
        memset(pLogInfo, 0, sizeof(LOCAL_LOG_INFO));
        if (pLogInfo == NULL)
        {
            return;
        }
        pLogInfo->iLogType = iLogType;
        memcpy(pLogInfo->szTime, szTime, 64);
        memcpy(pLogInfo->szLogInfo, szLogInfo, 512);
        memcpy(pLogInfo->szDevInfo, szDevInfo, 128);
        memcpy(pLogInfo->szErrInfo, szErrInfo, 256);
        ::PostMessage(g_pMainDlg->m_hWnd, WM_ADD_LOG, iDeviceIndex, (LPARAM)pLogInfo);
    }
    catch (...)
    {
        if (pLogInfo != NULL)
        {
            return;
        }
        OutputDebugString("New Log Exception!!\n");
    }
}

/*********************************************************
Function:    OnWMAddLog
Desc:        responding to the message WM_ADD_LOG
Input:    wParam, parameter 1;lParam, parameter 2;
Output:    none
Return:    result code
**********************************************************/
LRESULT CEHomeDemoDlg::OnWMAddLog(WPARAM wParam, LPARAM lParam)
{
    UN_REFERENCED_PARAMETER(wParam)
    LPLOCAL_LOG_INFO pLogInfo = LPLOCAL_LOG_INFO(lParam);
    if (NULL == pLogInfo)
    {
        return 0;
    }
    char szLogType[32] = "FAIL";

    char szTime[64] = {0};
    char szLogInfo[512] = {0};
    char szDevInfo[128] = {0};
    char szErrInfo[256] = {0};
    memcpy(szTime, pLogInfo->szTime, 64);
    memcpy(szLogInfo, pLogInfo->szLogInfo, 512);
    memcpy(szDevInfo, pLogInfo->szDevInfo, 128);
    memcpy(szErrInfo, pLogInfo->szErrInfo, 256);
    if ( 5000 == m_listAllLog.GetItemCount())
    {
        m_listAllLog.DeleteAllItems();
    }

    switch (pLogInfo->iLogType)
    {
    case ALARM_INFO_T:
        sprintf(szLogType, "Alarm");
        m_listAlarmInfo.InsertItem(0, "", -1);
        m_listAlarmInfo.SetItemText(0, 1, szTime);
        m_listAlarmInfo.SetItemText(0, 2, szLogInfo);
        m_listAlarmInfo.SetItemText(0, 3, szDevInfo);
        break;
    case OPERATION_SUCC_T:
        sprintf(szLogType, "SUCC");    
        if (g_struLocalParam.bSuccLog)
        {
            m_listAllLog.InsertItem(0, "", -1);
            m_listAllLog.SetItemText(0, 1, szTime);
            m_listAllLog.SetItemText(0, 2, szLogType);
            m_listAllLog.SetItemText(0, 3, szLogInfo);
            m_listAllLog.SetItemText(0, 4, szDevInfo);
            m_listAllLog.SetItemText(0, 5, szErrInfo);
        }
        break;
    case OPERATION_FAIL_T:
        sprintf(szLogType, "FAIL");        
        if (g_struLocalParam.bFailLog)
        {
            m_listAllLog.InsertItem(0, "", -1);
            m_listAllLog.SetItemText(0, 1, szTime);
            m_listAllLog.SetItemText(0, 2, szLogType);
            m_listAllLog.SetItemText(0, 3, szLogInfo);
            m_listAllLog.SetItemText(0, 4, szDevInfo);
            m_listAllLog.SetItemText(0, 5, szErrInfo);
        }
        break;
    case PLAY_SUCC_T:
        sprintf(szLogType, "SUCC");
        if (g_struLocalParam.bSuccLog)
        {
            m_listAllLog.InsertItem(0, "", -1);
            m_listAllLog.SetItemText(0, 1, szTime);
            m_listAllLog.SetItemText(0, 2, szLogType);
            m_listAllLog.SetItemText(0, 3, szLogInfo);
            m_listAllLog.SetItemText(0, 4, szDevInfo);
            m_listAllLog.SetItemText(0, 5, szErrInfo);
        }
        break;
    case PLAY_FAIL_T:
        sprintf(szLogType, "FAIL");    
        if (g_struLocalParam.bFailLog)
        {
            m_listAllLog.InsertItem(0, "", -1);
            m_listAllLog.SetItemText(0, 1, szTime);
            m_listAllLog.SetItemText(0, 2, szLogType);
            m_listAllLog.SetItemText(0, 3, szLogInfo);
            m_listAllLog.SetItemText(0, 4, szDevInfo);
            m_listAllLog.SetItemText(0, 5, szErrInfo);
        }
        break;
    default:
        sprintf(szLogType, "FAIL");    
        if (g_struLocalParam.bFailLog)
        {
            m_listAllLog.InsertItem(0, "", -1);
            m_listAllLog.SetItemText(0, 1, szTime);
            m_listAllLog.SetItemText(0, 2, szLogType);
            m_listAllLog.SetItemText(0, 3, szLogInfo);
            m_listAllLog.SetItemText(0, 4, szDevInfo);
            m_listAllLog.SetItemText(0, 5, szErrInfo);
        }
        break;
    }
    if (pLogInfo != NULL)
    {
        delete pLogInfo;
        pLogInfo = NULL;
    }

    return 0;
}

/*********************************************************
Function:    GetAfterItem
Desc:        get device insert point, make sure new device insert afterword last node
Input:    hRoot:handle of item tree root
Output:    
Return:    
**********************************************************/
HTREEITEM CEHomeDemoDlg::GetAfterItem(HTREEITEM hRoot)
{
    HTREEITEM hReturn = TVI_FIRST;
    HTREEITEM hChild = m_treeDeviceList.GetChildItem(hRoot);

    while (hChild)
    {
        if (TREE_ALL_T ==  m_treeDeviceList.GetItemData(hChild) / 1000)   //break if it is device tree node
        {
            break;
        }

        hReturn = hChild;
        hChild = m_treeDeviceList.GetNextSiblingItem(hChild);
    }

    return hReturn;
}

LRESULT CEHomeDemoDlg::OnWMAddDev(WPARAM wParam, LPARAM lParam)
{
    UN_REFERENCED_PARAMETER(wParam)
    LPLOCAL_DEVICE_INFO pDevInfo = LPLOCAL_DEVICE_INFO(lParam);
    if (NULL == pDevInfo)
    {
        return 0;
    }

    int i = 0;
    unsigned int j = 0;
    char szLan[64] = {0};

    HTREEITEM hRoot = m_treeDeviceList.GetRootItem();

    // 先遍历一边，设备是否存在
    for (; i < MAX_DEVICES; i++)
    {
        if ((g_struDeviceInfo[i].iDeviceIndex == -1 && 
            strlen((char*)g_struDeviceInfo[i].byDeviceID) == 0 && 
            strlen((char*)g_struDeviceInfo[i].sDeviceSerial) == 0) || // 新的设备
            (memcmp(pDevInfo->byDeviceID, g_struDeviceInfo[i].byDeviceID, MAX_DEVICE_ID_LEN) == 0 && 
             memcmp(pDevInfo->sDeviceSerial, g_struDeviceInfo[i].sDeviceSerial, NET_EHOME_SERIAL_LEN) == 0))    //已有设备
        {
            break;
        }
    }
    if (i == MAX_DEVICES)   //设备列表已满，从头开始
    {
        i = 0;
    }

    /*for (; i < MAX_DEVICES; i++)
    {
        if (g_struDeviceInfo[i].iDeviceIndex == -1
            || memcmp(pDevInfo->byDeviceID, g_struDeviceInfo[i].byDeviceID, 256) == 0)
        {*/
            //m_iCurDeviceIndex = i;
            int iTmp = g_struDeviceInfo[i].iDeviceIndex;
            if (wParam == NULL)
            {
                if (strlen((char*)g_struDeviceInfo[i].byEhomeKey) > 0)
                {
                    memcpy(pDevInfo->byEhomeKey, g_struDeviceInfo[i].byEhomeKey, 32);
                }
                if (strlen((char*)g_struDeviceInfo[i].bySessionKey) > 0)
                {
                    memcpy(pDevInfo->bySessionKey, g_struDeviceInfo[i].bySessionKey, 16);
                }
                if (g_bSS_Enable)
                {
                    char szClouldSecretKey[256] = { 0 };
                    if (NET_ESS_HAMSHA256((char*)pDevInfo->byDeviceID, (char*)pDevInfo->byEhomeKey, szClouldSecretKey, 255))
                    {
                        memcpy(pDevInfo->byClouldSecretKey, szClouldSecretKey, 64);
                    }
                }
            }
            memcpy(&g_struDeviceInfo[i], pDevInfo, sizeof(LOCAL_DEVICE_INFO));
            g_struDeviceInfo[i].iDeviceIndex = iTmp;

            HTREEITEM hDevice = NULL;

            if (pDevInfo->dwVersion >= 2)
            {
                if (g_struDeviceInfo[i].iDeviceIndex < 0)
                {
                    if (strlen((char*)g_struDeviceInfo[i].byDeviceID) > 0 && pDevInfo->lLoginID >= 0)
                    {
                        hDevice = m_treeDeviceList.InsertItem((LPCTSTR)g_struDeviceInfo[i].byDeviceID, DEVICE_LOGIN, DEVICE_LOGIN, hRoot, GetAfterItem(hRoot));
                    }
                    else if (strlen((char*)g_struDeviceInfo[i].sDeviceSerial) > 0 &&  pDevInfo->lLoginID >= 0)
                    {
                        hDevice = m_treeDeviceList.InsertItem((LPCTSTR)g_struDeviceInfo[i].sDeviceSerial, DEVICE_LOGIN, DEVICE_LOGIN, hRoot, GetAfterItem(hRoot));
                    }
                    else
                    {
                        if (pDevInfo->dwVersion == 5 && pDevInfo->lLoginID < 0)
                        {
                            CString strName;
                            char szLan[16] = { 0 };
                            g_StringLanType(szLan, "未上线", "offline");
                            strName.Format(_T("%s(%s)"), g_struDeviceInfo[i].byDeviceID, szLan);
                            hDevice = m_treeDeviceList.InsertItem(strName, DEVICE_LOGIN, DEVICE_LOGIN, hRoot, GetAfterItem(hRoot));
                        }
                        else
                        {
                            AddLog(-1, OPERATION_FAIL_T, 1, "version is larger than four, serial is null，show DeviceID!");
                            hDevice = m_treeDeviceList.InsertItem((LPCTSTR)g_struDeviceInfo[i].byDeviceID, DEVICE_LOGIN, DEVICE_LOGIN, hRoot, GetAfterItem(hRoot));
                        }
                    }
                }
                else
                {
                    HTREEITEM hDevItem = m_treeDeviceList.GetChildItem(hRoot);
                    while (hDevItem != NULL)
                    {
                        if (DEVICETYPE == (int)m_treeDeviceList.GetItemData(hDevItem) / 1000)
                        {
                            if (i == (int)m_treeDeviceList.GetItemData(hDevItem) % 1000)
                            {
                                hDevice = hDevItem;
                                break;
                            }
                        }
                        hDevItem = m_treeDeviceList.GetNextVisibleItem(hDevItem);
                    }
                    if (hDevice == NULL)
                    {
                        AddLog(-1, OPERATION_FAIL_T, 1, "GET TREE ITEM FAIL");
                        return -1;
                    }
                    if (pDevInfo->dwVersion == 5 && pDevInfo->lLoginID < 0)
                    {
                        return -1;
                    }
                    if (strlen((char*)g_struDeviceInfo[i].byDeviceID) > 0)
                    {
                        m_treeDeviceList.SetItemText(hDevice, (char*)g_struDeviceInfo[i].byDeviceID);
                    }
                    else
                    {
                        m_treeDeviceList.SetItemText(hDevice, (char*)g_struDeviceInfo[i].sDeviceSerial);
                    }
                }
            }
            else
            {

                hDevice = m_treeDeviceList.InsertItem((LPCTSTR)g_struDeviceInfo[i].byDeviceID, DEVICE_LOGIN, DEVICE_LOGIN, hRoot,GetAfterItem(hRoot));
            }
            
            g_struDeviceInfo[i].iDeviceIndex = i;
            m_treeDeviceList.SetItemData(hDevice, DEVICETYPE * 1000 + i);//
            if (pDevInfo->lLoginID >= 0)//DAS服务器设置时，一旦sessionKey不对，就会导致获取参数返回错误码10，此时在主线程如果继续获取参数，而设备不断注册时就会导致不断卡死
            {
                HTREEITEM hChannel = NULL;

                //获取设备信息
                NET_EHOME_DEVICE_INFO struDevInfo = { 0 };
                struDevInfo.dwSize = sizeof(NET_EHOME_DEVICE_INFO);
                NET_EHOME_CONFIG struCfg = { 0 };
                struCfg.pOutBuf = &struDevInfo;
                struCfg.dwOutSize = sizeof(NET_EHOME_DEVICE_INFO);
                DWORD dwChannelNum = 0;
                if (pDevInfo->dwVersion == 5)
                {
                    if (NET_ECMS_GetDevConfig(g_struDeviceInfo[i].lLoginID, NET_EHOME_GET_DEVICE_INFO, &struCfg, sizeof(NET_EHOME_CONFIG)))//针对门禁设备只实现了ISAPI协议
                    {
                        AddLog(g_struDeviceInfo[i].iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_DEVICE_INFO");
                    }
                    else if (GetISAPIChannels(g_struDeviceInfo[i].lLoginID, &dwChannelNum)) //针对门禁设备只实现了ISAPI协议
                    {
                        AddLog(g_struDeviceInfo[i].iDeviceIndex, OPERATION_SUCC_T, 1, "GetISAPIChannels");
                    }
                    else
                    {
                        AddLog(g_struDeviceInfo[i].iDeviceIndex, OPERATION_FAIL_T, 1, "GetISAPIChannels");
                    }
                }
                else if (!NET_ECMS_GetDevConfig(g_struDeviceInfo[i].lLoginID, NET_EHOME_GET_DEVICE_INFO, &struCfg, sizeof(NET_EHOME_CONFIG)))
                {
                    AddLog(g_struDeviceInfo[i].iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_DEVICE_INFO");
                  
                    //  return -1;
                }
                else
                {
                    //需要将字符串字段转换成GB2312
                    DWORD dwConvertLen = 0;
                    UTF82A((char*)struDevInfo.sSerialNumber, (char*)struDevInfo.sSerialNumber, MAX_SERIALNO_LEN, &dwConvertLen);
                    UTF82A((char*)struDevInfo.sSIMCardSN, (char*)struDevInfo.sSIMCardSN, MAX_SERIALNO_LEN, &dwConvertLen);
                    UTF82A((char*)struDevInfo.sSIMCardPhoneNum, (char*)struDevInfo.sSIMCardPhoneNum, MAX_PHOMENUM_LEN, &dwConvertLen);

                    AddLog(g_struDeviceInfo[i].iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_DEVICE_INFO sSerialNumber=[%s]", struDevInfo.sSerialNumber);
                    AddLog(g_struDeviceInfo[i].iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_DEVICE_INFO sSIMCardSN=[%s]", struDevInfo.sSIMCardSN);
                    AddLog(g_struDeviceInfo[i].iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_DEVICE_INFO sSIMCardPhoneNum=[%s]", struDevInfo.sSIMCardPhoneNum);

                    HTREEITEM hSubDevItem = m_treeDeviceList.GetChildItem(hDevice);
                    while (hSubDevItem != NULL)
                    {
                        m_treeDeviceList.DeleteItem(hSubDevItem);
                        hSubDevItem = m_treeDeviceList.GetChildItem(hDevice);
                    }
                }
                
                if (dwChannelNum > 0)
                {
                    struDevInfo.dwChannelNumber = dwChannelNum;
                }

                g_struDeviceInfo[i].dwAlarmInNum = struDevInfo.dwAlarmInPortNum;
                g_struDeviceInfo[i].dwAlarmOutNum = struDevInfo.dwAlarmOutPortNum;
                g_struDeviceInfo[i].dwAnalogChanNum = struDevInfo.dwChannelNumber;
                g_struDeviceInfo[i].dwDeviceChanNum = struDevInfo.dwChannelAmount;
                g_struDeviceInfo[i].dwIPChanNum = struDevInfo.dwChannelAmount - struDevInfo.dwChannelNumber;
                g_struDeviceInfo[i].dwAudioNum = struDevInfo.dwAudioChanNum;
                g_struDeviceInfo[i].byStartDTalkChan = struDevInfo.byStartDTalkChan;
                //analog channel
                for (j = 0; j < struDevInfo.dwChannelNumber; j++)
                {
                    //由于目前的通道无法判断是否可用，因此都当做可用
                    g_struDeviceInfo[i].struChanInfo[j].bEnable = TRUE;
                    //这是模拟通道
                    g_struDeviceInfo[i].struChanInfo[j].iChannelNO = j + 1;
                    g_struDeviceInfo[i].struChanInfo[j].iChanType = DEMO_CHANNEL_TYPE_ANALOG;
                    sprintf(szLan, "Camera%d", g_struDeviceInfo[i].struChanInfo[j].iChannelNO);
                    hChannel = m_treeDeviceList.InsertItem(szLan, CHAN_ORIGINAL, CHAN_ORIGINAL, hDevice);

                    g_struDeviceInfo[i].struChanInfo[j].iDeviceIndex = i;
                    g_struDeviceInfo[i].struChanInfo[j].iChanIndex = j;

                    m_treeDeviceList.SetItemData(hChannel, CHANNELTYPE * 1000 + g_struDeviceInfo[i].struChanInfo[j].iChanIndex);

                }

                //然后添加IP通道
                for (j = struDevInfo.dwChannelNumber; j < struDevInfo.dwChannelAmount; j++)
                {
                    //由于目前的通道无法判断是否可用，因此都当做可用
                    g_struDeviceInfo[i].struChanInfo[j].bEnable = TRUE;
                    //这是IP通道
                    g_struDeviceInfo[i].struChanInfo[j].iChannelNO = struDevInfo.dwStartChannel + j;
                    g_struDeviceInfo[i].struChanInfo[j].iChanType = DEMO_CHANNEL_TYPE_IP;
                    sprintf(szLan, "IPCamera%d", g_struDeviceInfo[i].struChanInfo[j].iChannelNO);
                    hChannel = m_treeDeviceList.InsertItem(szLan, CHAN_ORIGINAL, CHAN_ORIGINAL, hDevice);

                    g_struDeviceInfo[i].struChanInfo[j].iDeviceIndex = i;
                    g_struDeviceInfo[i].struChanInfo[j].iChanIndex = j;

                    m_treeDeviceList.SetItemData(hChannel, CHANNELTYPE * 1000 + g_struDeviceInfo[i].struChanInfo[j].iChanIndex);

                }

                //零通道处理逻辑
                g_struDeviceInfo[i].dwZeroChanNum = struDevInfo.dwSupportZeroChan;// SupportZeroChan:支持零通道的个数：0-不支持，1-支持1路，2-支持2路，以此类推
                if (0 == struDevInfo.dwStartZeroChan)// 零通道起始编号，默认10000
                {
                    g_struDeviceInfo[i].dwZeroChanStart = 10000;
                }
                else
                {
                    g_struDeviceInfo[i].dwZeroChanStart = struDevInfo.dwStartZeroChan;
                }
                for (int k = 0; k < (int)struDevInfo.dwSupportZeroChan; k++)
                {
                    int nZeroChannelIndex = j + k;
                    g_struDeviceInfo[i].struChanInfo[nZeroChannelIndex].bEnable = TRUE;

                    //这是零通道
                    g_struDeviceInfo[i].struChanInfo[nZeroChannelIndex].iChannelNO = g_struDeviceInfo[i].dwZeroChanStart + k;
                    g_struDeviceInfo[i].struChanInfo[nZeroChannelIndex].iChanType = DEMO_CHANNEL_TYPE_ZERO;
                    sprintf(szLan, "ZeroChannel%d", g_struDeviceInfo[i].struChanInfo[nZeroChannelIndex].iChannelNO);
                    hChannel = m_treeDeviceList.InsertItem(szLan, CHAN_ORIGINAL, CHAN_ORIGINAL, hDevice);

                    g_struDeviceInfo[i].struChanInfo[nZeroChannelIndex].iDeviceIndex = i;
                    g_struDeviceInfo[i].struChanInfo[nZeroChannelIndex].iChanIndex = nZeroChannelIndex;

                    m_treeDeviceList.SetItemData(hChannel, CHANNELTYPE * 1000 + g_struDeviceInfo[i].struChanInfo[nZeroChannelIndex].iChanIndex);
                }

                for (int j = 0; j < 256; j++)
                {
                    char szLocalIP[128] = { 0 };
                    //g_pMainDlg->GetLocalIP(szLocalIP);
                    DWORD dwPortTemp = 0;
                    GetAddressByType(StreamAdd, 0, g_struDeviceInfo[i].struChanInfo[j].struIP.szIP, 
                        sizeof(g_struDeviceInfo[i].struChanInfo[j].struIP.szIP), 
                        (char *)(&dwPortTemp), sizeof(dwPortTemp)); //预览不关心版本

                    //DWORD dwIP = ntohl(inet_addr(szLocalIP));
                    //memcpy(g_struDeviceInfo[i].struChanInfo[j].struIP.szIP, IPToStr(dwIP), 16);
                    if (dwPortTemp != 0)
                    {
                        g_struDeviceInfo[i].struChanInfo[j].struIP.wPort = (WORD)dwPortTemp;
                    }
                }
            }
            ////analog channel & IP channel
            //for (j= 0; j < MAX_CHAN_NUM_DEMO; j++)
            //{
            //    if (g_struDeviceInfo[i].struChanInfo[j].iChannelNO != -1 )
            //    {
            //        if (g_struDeviceInfo[i].struChanInfo[j].bEnable)
            //        {
            //            if (g_struDeviceInfo[i].struChanInfo[j].iChanType == DEMO_CHANNEL_TYPE_ANALOG)
            //            {
            //                sprintf(szLan, "Camera%d", g_struDeviceInfo[i].struChanInfo[j].iChannelNO);
            //                hChannel = m_treeDeviceList.InsertItem(szLan,CHAN_ORIGINAL,CHAN_ORIGINAL,hDevice);
            //            }
            //            else if (g_struDeviceInfo[i].struChanInfo[j].iChanType == DEMO_CHANNEL_TYPE_IP)
            //            {
            //                sprintf(szLan, "IPCamera%d", g_struDeviceInfo[i].struChanInfo[j].iChannelNO);
            //                hChannel = m_treeDeviceList.InsertItem(szLan,CHAN_ORIGINAL,CHAN_ORIGINAL,hDevice);
            //            }                            
            //        }

            //        g_struDeviceInfo[i].struChanInfo[j].iDeviceIndex = i;
            //        g_struDeviceInfo[i].struChanInfo[j].iChanIndex = j;

            //        m_treeDeviceList.SetItemData(hChannel, CHANNELTYPE * 1000 + g_struDeviceInfo[i].struChanInfo[j].iChanIndex);
            //    }  
            //}

            m_treeDeviceList.Expand(hRoot,TVE_EXPAND);
            m_treeDeviceList.Expand(hDevice,TVE_EXPAND);
            /*break;
        }
    }*/

    delete pDevInfo;

    return 0;
}

//DWORD  LogoutThread(LPVOID pParam)
//{
//    int lLoginID = (int)pParam;
//
//    NET_ECMS_ForceLogout(lLoginID);
//
//
//    //if (g_struDeviceInfo[iDevindex].iDeviceIndex == -1)
//    //{
//    //    return 0;
//    //}
//
//    g_pMainDlg->AddLog(-1, OPERATION_SUCC_T, 1, "NET_ECMS_ForceLogout [%d]", lLoginID);
//
//    g_pMainDlg->DelDev(lLoginID);
//
//    return 0;
//}

LRESULT CEHomeDemoDlg::OnWMDelDev(WPARAM wParam, LPARAM lParam)
{
    UN_REFERENCED_PARAMETER(wParam)
        int lLoginID = (int)lParam;

    //DWORD dwThreadId = 0;
    //HANDLE hLogoutThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(LogoutThread), (LPVOID)lLoginID, 0, &dwThreadId);

    //if (hLogoutThread == NULL)
    //{
    //    char szLan[256] = { 0 };
    //    AddLog(-1, OPERATION_SUCC_T, 1, "打开注销线程失败");
    //    return 0;
    //}

    NET_ECMS_ForceLogout(lLoginID);


    //if (g_struDeviceInfo[iDevindex].iDeviceIndex == -1)
    //{
    //    return 0;
    //}

    AddLog(-1, OPERATION_SUCC_T, 1, "NET_ECMS_ForceLogout [%d]", lLoginID);

    DelDev(lLoginID);

    return 0;
}

LRESULT CEHomeDemoDlg::OnWMDelOfflineDev(WPARAM wParam, LPARAM lParam)
{
    UN_REFERENCED_PARAMETER(wParam)
    char* sDeviceSerial = (char*)(lParam);
    DelOfflineDev(sDeviceSerial);

    return 0;
}

LRESULT CEHomeDemoDlg::OnWMChangeIPAddr(WPARAM wParam, LPARAM lParam)
{
    UN_REFERENCED_PARAMETER(wParam)
    int lLoginID = (int)lParam;
    DelDev(lLoginID);
    return 0;
}


LRESULT CEHomeDemoDlg::OnWMSetPreview(WPARAM wParam, LPARAM lParam)
{
  //  Sleep(5000);
    LONG lPreviewHandle = (LONG)lParam;
    NET_EHOME_PREVIEW_DATA_CB_PARAM struDataCB = { 0 };
    struDataCB.fnPreviewDataCB = fnPREVIEW_DATA_CB;
    struDataCB.pUserData = (void*)g_pMainDlg->m_iCurWndIndex;

    if (!NET_ESTREAM_SetPreviewDataCB(lPreviewHandle, &struDataCB))
    {
        g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_FAIL_T, 2, "stream handle = %d", lPreviewHandle);
        //出问题先崩了吧，不然问题不好查
        char *p = NULL;
        *p = 1;
        return FALSE;
    }
    g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_SUCC_T, 2, "stream handle = %d", lPreviewHandle);
    return 0;
}

LRESULT CEHomeDemoDlg::OnWMDevSleep(WPARAM wParam, LPARAM lParam)
{
    UN_REFERENCED_PARAMETER(wParam)
        int  iLoginID = lParam;
    if (iLoginID < 0)
    {
        return 0;
    }
    int i = 0;
    HTREEITEM hRoot = m_treeDeviceList.GetRootItem();

    for (i = 0; i < MAX_DEVICES; i++)
    {
        if (iLoginID == g_struDeviceInfo[i].lLoginID)
        {
            //m_iCurDeviceIndex = i;
            int iTmp = g_struDeviceInfo[i].iDeviceIndex;

            HTREEITEM hDevice = NULL;

            CString strName;
            char szLan[16] = { 0 };
            g_StringLanType(szLan, "设备休眠", "deviceSleep");
            strName.Format(_T("%s(%s)"), g_struDeviceInfo[i].byDeviceID, szLan);

            HTREEITEM hDevItem = m_treeDeviceList.GetChildItem(hRoot);
            while (hDevItem != NULL)
            {
                if (DEVICETYPE == (int)m_treeDeviceList.GetItemData(hDevItem) / 1000)
                {
                    if (i == (int)m_treeDeviceList.GetItemData(hDevItem) % 1000)
                    {
                        hDevice = hDevItem;
                        break;
                    }
                }
                hDevItem = m_treeDeviceList.GetNextVisibleItem(hDevItem);
            }
            if (hDevice == NULL)
            {
                AddLog(-1, OPERATION_FAIL_T, 1, "GET TREE ITEM FAIL");
                return -1;
            }

            m_treeDeviceList.SetItemText(hDevice, strName);

            m_treeDeviceList.Expand(hRoot, TVE_EXPAND);
            m_treeDeviceList.Expand(hDevice, TVE_EXPAND);
            break;
        }
    }

    return 0;
}

void CEHomeDemoDlg::DelDev(LONG lLoginID)
{
    int iDevindex = -1;
    int i = 0;
    int j = 0;
    HTREEITEM hRoot = m_treeDeviceList.GetRootItem();

    HTREEITEM hDev = m_treeDeviceList.GetChildItem(hRoot);

    if (NULL == hDev)
    {
        return;
    }

    for (i = 0; i < MAX_DEVICES; i++)
    {
        iDevindex = m_treeDeviceList.GetItemData(hDev) % 1000;
        if (g_struDeviceInfo[iDevindex].iDeviceIndex != -1)
        {
            //说明有添加设备，那么就判断一下
            if (g_struDeviceInfo[iDevindex].lLoginID == lLoginID)
            {
                for (int k = 0; k<(int)g_struDeviceInfo[iDevindex].dwDeviceChanNum; k++)
                {
                    //如果设备要下线，先把这个设备的预览通道都关了
                    int iPlayWndIndex = g_struDeviceInfo[iDevindex].struChanInfo[k].iPlayWndIndex;
                    if (iPlayWndIndex >= 0)
                    {
                        if (!m_pDlgPreview[iPlayWndIndex].StopPlay())
                        {
                            g_pMainDlg->AddLog(iDevindex, OPERATION_FAIL_T, 0, "Stop previewing Failed in OnWMDelDev");
                        }
                    }
                }

                //找到设备
                m_treeDeviceList.DeleteItem(hDev);
                memset(&g_struDeviceInfo[iDevindex], 0, sizeof(g_struDeviceInfo[iDevindex]));
                g_struDeviceInfo[iDevindex].iDeviceIndex = -1;
                for (j = 0; j < MAX_CHAN_NUM_DEMO; j++)
                {
                    g_struDeviceInfo[iDevindex].struChanInfo[j].iDeviceIndex = -1;
                    g_struDeviceInfo[iDevindex].struChanInfo[j].iChanIndex = -1;
                    g_struDeviceInfo[iDevindex].struChanInfo[j].iChannelNO = -1;
                }
                break;
            }
        }

        //能到这里说明设备还没找到，那么进行到下一个节点
        hDev = m_treeDeviceList.GetNextSiblingItem(hDev);
        if (hDev == NULL)
        {
            break;
        }
    }
}

void CEHomeDemoDlg::DelOfflineDev(char* sDeviceSerial)
{
    int iDevindex = -1;
    int i = 0;
    int j = 0;
    HTREEITEM hRoot = m_treeDeviceList.GetRootItem();

    HTREEITEM hDev = m_treeDeviceList.GetChildItem(hRoot);

    if (NULL == hDev)
    {
        return;
    }

    for (i = 0; i < MAX_DEVICES; i++)
    {
        iDevindex = m_treeDeviceList.GetItemData(hDev) % 1000;
        if (g_struDeviceInfo[iDevindex].iDeviceIndex != -1)
        {
            if (memcmp(g_struDeviceInfo[iDevindex].sDeviceSerial, sDeviceSerial, NET_EHOME_SERIAL_LEN) == 0)
            {
                //找到设备
                m_treeDeviceList.DeleteItem(hDev);
                memset(&g_struDeviceInfo[iDevindex], 0, sizeof(g_struDeviceInfo[iDevindex]));
                g_struDeviceInfo[iDevindex].iDeviceIndex = -1;
                for (j = 0; j < MAX_CHAN_NUM_DEMO; j++)
                {
                    g_struDeviceInfo[iDevindex].struChanInfo[j].iDeviceIndex = -1;
                    g_struDeviceInfo[iDevindex].struChanInfo[j].iChanIndex = -1;
                    g_struDeviceInfo[iDevindex].struChanInfo[j].iChannelNO = -1;
                }
                break;
            }
        }

        //能到这里说明设备还没找到，那么进行到下一个节点
        hDev = m_treeDeviceList.GetNextSiblingItem(hDev);
        if (hDev == NULL)
        {
            break;
        }
    }
}

void CEHomeDemoDlg::ProcessAlarmData(DWORD dwAlarmType, void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen, void *pUrl, DWORD dwUrlLen)
{
 //   char *szBuf = new char[409600];
	//memset(szBuf, 0, 409600);
 //   memcpy(szBuf, pXml, dwXmlLen);
    //AddLog(-1, ALARM_INFO_T, 0, "%s--%d", (char*)pXml, dwXmlLen);
    if (pUrl != NULL)
    {
        dwAlarmType = EHOME_ISAPI_ALARM;
    }
    switch (dwAlarmType)
    {
    case EHOME_INFO_RELEASE_PRIVATE:
    {
        LPNET_EHOME_TERMINAL_REPORT_INFO pStruRelease = (LPNET_EHOME_TERMINAL_REPORT_INFO)pStru;
        break;
    }
    case EHOME_ALARM:
        ProcessEhomeAlarm(pStru, dwStruLen, pXml, dwXmlLen);
        break;
    case EHOME_ALARM_HEATMAP_REPORT:
        ProcessEhomeHeatMapReport(pStru, dwStruLen, pXml, dwXmlLen);
        break;
    case EHOME_ALARM_FACESNAP_REPORT:
        ProcessEhomeFaceSnapReport(pStru, dwStruLen, pXml, dwXmlLen);
        break;;
    case EHOME_ALARM_GPS:
        ProcessEhomeGps(pStru, dwStruLen, pXml, dwXmlLen);
        break;
    case EHOME_ALARM_CID_REPORT:
        ProcessEhomeCid(pStru, dwStruLen, pXml, dwXmlLen);
        break;
//     case EHOME_ALARM_GPS_DATA:
        //ProcessEhomeGpsData(pStru, dwStruLen, pXml, dwXmlLen);
//         break;
    case EHOME_ALARM_NOTICE_PICURL:
        ProcessEhomeNoticePicUrl(pStru, dwStruLen, pXml, dwXmlLen);
        break;
    case  EHOME_ALARM_NOTIFY_FAIL:
        ProcessEhomeNotifyFail(pStru, dwStruLen, pXml, dwXmlLen);
        break;
    case  EHOME_ALARM_ACS:
        ProcessEhomeAlarmAcs(pXml, dwXmlLen);
        break;
    case EHOME_ALARM_WIRELESS_INFO:
        ProcessEhomeWirelessInfo(pStru, dwStruLen, pXml, dwXmlLen);
        break;
    case EHOME_ISAPI_ALARM:
        ProcessHttpAlarmInfo(pStru, dwStruLen, pUrl, dwUrlLen);
        break;
    case EHOME_ALARM_MPDCDATA:
        ProcessEhomeMPDCData(pStru, dwStruLen, pUrl, dwUrlLen);
        break;
    case EHOME_ALARM_QRCODE:
        ProcessEhomeAlarmQrCode(pXml, dwXmlLen);
        break;
    case EHOME_ALARM_FACETEMP:
        ProcessEhomeAlarmFaceTemp(pXml, dwXmlLen);
        break;
    default:        
        AddLog(-1, ALARM_INFO_T, 0, "[Unknown_Alarm]");
        break;
    }
}

int CEHomeDemoDlg::GetCurDeviceIndex()
{
    if (m_iCurDeviceIndex < MAX_DEVICES && m_iCurDeviceIndex >= 0)
    {
        return m_iCurDeviceIndex;
    }
    //AddLog(-1, OPERATION_SUCC_T, 0, "please select a device at first!");
    return -1;
}

int CEHomeDemoDlg::GetCurChanIndex()
{
    if (m_iCurChanIndex >= 0 && m_iCurChanIndex < 512)
    {
        return m_iCurChanIndex;
    }

    return -1;
} 

void CEHomeDemoDlg::ProcessEhomeAlarm(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen)
    UN_REFERENCED_PARAMETER(pXml)
    UN_REFERENCED_PARAMETER(dwStruLen)
    if (pStru == NULL)
    {
        return;
    }
    NET_EHOME_ALARM_INFO *pStruAlarmInfo = static_cast<NET_EHOME_ALARM_INFO *>(pStru);
    for (int i = 0; i < 256; i++)
    {
        if (pStruAlarmInfo->szDeviceID[i] == 0xcd)
        {
            pXml = pXml;
        }
    }

    AddLog(-1, ALARM_INFO_T, 0, "[ALARM]DeviceID:%s,Time:%s,Type:%d,Action:%d,Channel:%d,AlarmIn:%d,DiskNo:%d", pStruAlarmInfo->szDeviceID, \
        pStruAlarmInfo->szAlarmTime, pStruAlarmInfo->dwAlarmType, pStruAlarmInfo->dwAlarmAction, pStruAlarmInfo->dwVideoChannel, \
        pStruAlarmInfo->dwAlarmInChannel, pStruAlarmInfo->dwDiskNumber);
    switch (pStruAlarmInfo->dwAlarmType)
    {
    case ALARM_TYPE_DEV_CHANGED_STATUS:
        AddLog(-1, ALARM_INFO_T, 0, "[ALARM_TYPE_DEV_CHANGED_STATUS]byDeviceStatus:%d", pStruAlarmInfo->uStatusUnion.struDevStatusChanged.byDeviceStatus);
        break;
    case ALARM_TYPE_CHAN_CHANGED_STATUS:
        AddLog(-1, ALARM_INFO_T, 0, "[ALARM_TYPE_CHAN_CHANGED_STATUS]byChanStatus:%d,wChanNO:%d", pStruAlarmInfo->uStatusUnion.struChanStatusChanged.byChanStatus,\
            pStruAlarmInfo->uStatusUnion.struChanStatusChanged.wChanNO);
        break;
    case ALARM_TYPE_HD_CHANGED_STATUS:
        AddLog(-1, ALARM_INFO_T, 0, "[ALARM_TYPE_HD_CHANGED_STATUS]byHDStatus:%d,wHDNo:%d,dwVolume:%d", pStruAlarmInfo->uStatusUnion.struHdStatusChanged.byHDStatus,\
            pStruAlarmInfo->uStatusUnion.struHdStatusChanged.wHDNo, pStruAlarmInfo->uStatusUnion.struHdStatusChanged.dwVolume);
        break;
    case ALARM_TYPE_DEV_TIMING_STATUS:
        AddLog(-1, ALARM_INFO_T, 0, "[ALARM_TYPE_DEV_TIMING_STATUS]byCPUUsage:%d,byMainFrameTemp:%d,byBackPanelTemp:%d,dwMemoryTotal:%d,dwMemoryUsage:%d",\
            pStruAlarmInfo->uStatusUnion.struDevTimeStatus.byCPUUsage, pStruAlarmInfo->uStatusUnion.struDevTimeStatus.byMainFrameTemp,\
            pStruAlarmInfo->uStatusUnion.struDevTimeStatus.byBackPanelTemp, pStruAlarmInfo->uStatusUnion.struDevTimeStatus.dwMemoryTotal,\
            pStruAlarmInfo->uStatusUnion.struDevTimeStatus.dwMemoryUsage);
        break;
    case ALARM_TYPE_CHAN_TIMING_STATUS:
        AddLog(-1, ALARM_INFO_T, 0, "[ALARM_TYPE_CHAN_TIMING_STATUS]byLinkNum:%d,wChanNO:%d,dwBitRate:%d", pStruAlarmInfo->uStatusUnion.struChanTimeStatus.byLinkNum,\
            pStruAlarmInfo->uStatusUnion.struChanTimeStatus.wChanNO, pStruAlarmInfo->uStatusUnion.struChanTimeStatus.dwBitRate);
        break;
    case ALARM_TYPE_HD_TIMING_STATUS:
        AddLog(-1, ALARM_INFO_T, 0, "[ALARM_TYPE_HD_TIMING_STATUS]wHDNo:%d,dwHDFreeSpace:%d", pStruAlarmInfo->uStatusUnion.struHdTimeStatus.wHDNo,\
            pStruAlarmInfo->uStatusUnion.struHdTimeStatus.dwHDFreeSpace);
        break;
    default:
        break;
    }
    
}

void CEHomeDemoDlg::ProcessEhomeHeatMapReport(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen)
    UN_REFERENCED_PARAMETER(pXml)
    UN_REFERENCED_PARAMETER(dwStruLen)
    if (pStru == NULL)
    {
        return;
    }
    NET_EHOME_HEATMAP_REPORT *pStruHeatMapReport = static_cast<NET_EHOME_HEATMAP_REPORT *>(pStru);
    AddLog(-1, ALARM_INFO_T, 0, "[HEATMAPREPORT]DeviceID:%s,Channel:%d,Start:%s,Stop:%s,HeatMapValue:%d %d %d,Size:%d %d",\
        pStruHeatMapReport->byDeviceID, pStruHeatMapReport->dwVideoChannel, pStruHeatMapReport->byStartTime, pStruHeatMapReport->byStopTime,\
        pStruHeatMapReport->struHeatmapValue.dwMaxHeatMapValue, pStruHeatMapReport->struHeatmapValue.dwMinHeatMapValue, \
        pStruHeatMapReport->struHeatmapValue.dwTimeHeatMapValue, pStruHeatMapReport->struPixelArraySize.dwLineValue, \
        pStruHeatMapReport->struPixelArraySize.dwColumnValue);
}

void CEHomeDemoDlg::ProcessEhomeFaceSnapReport(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen)
    UN_REFERENCED_PARAMETER(pXml)
    UN_REFERENCED_PARAMETER(dwStruLen)
    if (pStru == NULL)
    {
        return;
    }
    NET_EHOME_FACESNAP_REPORT *pStruFaceSnapReport = static_cast<NET_EHOME_FACESNAP_REPORT *>(pStru);
    AddLog(-1, ALARM_INFO_T, 0, "[FACESNAPREPORT]DeviceID:%s,Channel:%d,Time:%s,PicID:%d,Score:%d,TargetID:%d,Target Zone[%d %d %d %d],"
        "FacePicZone[%d %d %d %d],HumanFeature:[%d %d %d %d],Duration:%d,FacePicLen:%d,BackGroundPicLen:%d", \
        pStruFaceSnapReport->byDeviceID, pStruFaceSnapReport->dwVideoChannel, pStruFaceSnapReport->byAlarmTime, pStruFaceSnapReport->dwFacePicID, \
        pStruFaceSnapReport->dwFaceScore, pStruFaceSnapReport->dwTargetID, pStruFaceSnapReport->struTarketZone.dwX, pStruFaceSnapReport->struTarketZone.dwY, \
        pStruFaceSnapReport->struTarketZone.dwWidth, pStruFaceSnapReport->struTarketZone.dwHeight, pStruFaceSnapReport->struFacePicZone.dwX, pStruFaceSnapReport->struFacePicZone.dwY, \
        pStruFaceSnapReport->struFacePicZone.dwWidth, pStruFaceSnapReport->struFacePicZone.dwHeight, pStruFaceSnapReport->struHumanFeature.byAgeGroup, \
        pStruFaceSnapReport->struHumanFeature.bySex, pStruFaceSnapReport->struHumanFeature.byEyeGlass, pStruFaceSnapReport->struHumanFeature.byMask, pStruFaceSnapReport->dwStayDuration, \
        pStruFaceSnapReport->dwFacePicLen, pStruFaceSnapReport->dwBackgroudPicLen);
}


void CEHomeDemoDlg::ProcessEhomeGps(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen)
    UN_REFERENCED_PARAMETER(pXml)
    UN_REFERENCED_PARAMETER(dwStruLen)
    if (pStru == NULL)
    {
        return;
    }
    NET_EHOME_GPS_INFO *pStruGps = static_cast<NET_EHOME_GPS_INFO *>(pStru);
    AddLog(-1, ALARM_INFO_T, 0, "[GPS]DeviceID:%s,SampleTime:%s,Division:[%c %c],Satelites:%d,Precision:%d,Longitude:%d,Latitude:%d,Direction:%d,Speed:%d,Height:%d",\
        pStruGps->byDeviceID, pStruGps->bySampleTime, pStruGps->byDivision[0], pStruGps->byDivision[1], pStruGps->bySatelites, pStruGps->byPrecision, pStruGps->dwLongitude,\
        pStruGps->dwLatitude, pStruGps->dwDirection, pStruGps->dwSpeed, pStruGps->dwHeight);
}

void CEHomeDemoDlg::ProcessEhomeCid(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen)
    UN_REFERENCED_PARAMETER(pXml)
    UN_REFERENCED_PARAMETER(dwStruLen)
    if (pStru == NULL)
    {
        return;
    }
    if (NULL != pStru)
    {
        NET_EHOME_CID_INFO *pStruCidInfo = static_cast<NET_EHOME_CID_INFO *>(pStru);
        NET_EHOME_CID_INFO_INTERNAL_EX *pStruCidInfoEx = (NET_EHOME_CID_INFO_INTERNAL_EX *)(pStruCidInfo->pCidInfoEx);
        NET_EHOME_CID_INFO_PICTUREINFO_EX* pStruPicInfoEx = (NET_EHOME_CID_INFO_PICTUREINFO_EX*)(pStruCidInfo->pPicInfoEx);
        NET_EHOME_CID_INFO_INTERNAL_VIDEOINFO* pStruVideoInfo = (NET_EHOME_CID_INFO_INTERNAL_VIDEOINFO*)(pStruCidInfoEx->pVideoInfo);
        for (int i = 0; i < 256; i++)
        {
            if (pStruCidInfo->byDeviceID[i] == 0xcd)
            {
                pXml = pXml;
            }
        }
        char cDescribe[256] = { 0 };
        char cUUID[64] = { 0 };
        char szLinkedSubSystem[256] = { 0 };
        strncpy(cDescribe, pStruCidInfo->byCIDDescribe, strlen(pStruCidInfo->byCIDDescribe));
        if (pStruCidInfo->byExtend == 1)
        {
            memset(cDescribe, 0, 256);
            strncpy(cDescribe, pStruCidInfoEx->byCIDDescribeEx, strlen(pStruCidInfoEx->byCIDDescribeEx));
            strncpy(cUUID, pStruCidInfoEx->byUUID, strlen(pStruCidInfoEx->byUUID));
            AddLog(-1, ALARM_INFO_T, 0, "[CID_EX]uuid[%s],recheck[%d],Recheck URL[%s],videoType[%s]", pStruCidInfoEx->byUUID, pStruCidInfoEx->byRecheck, pStruCidInfoEx->byVideoURL, pStruCidInfoEx->byVideoType);
            for (int i = 0; i < MAX_PICTURE_NUM; ++i)
            {
                if ((pStruCidInfoEx->byRecheck == 1) && ((pStruPicInfoEx->byPictureURL[i][0]) != '\0'))
                {
                    AddLog(-1, ALARM_INFO_T, 0, "[CID_EX]uuid[%s],recheck[%d],PicURL[%s]", pStruCidInfoEx->byUUID, pStruCidInfoEx->byRecheck, pStruPicInfoEx->byPictureURL[i]);
                }
            }

            for (int j = 0; j < MAX_SUBSYSTEM_LEN; j++)
            {
                if (pStruCidInfoEx->byLinkageSubSystem[j] > 0)//关联子系统最小值为1
                {
                    sprintf(szLinkedSubSystem, "%s[%d] ", szLinkedSubSystem, pStruCidInfoEx->byLinkageSubSystem[j]);
                }
            }
            AddLog(-1, ALARM_INFO_T, 0, "[CID_EX] All Linked SubSystem: %s", szLinkedSubSystem);

            if (pStruCidInfoEx->byMultiVideoUrl == 1)
            {
                for (int k = 0; k < MAX_VIDEO_NUM; k++)
                {
                    if ((pStruCidInfoEx->byRecheck == 1) && (pStruVideoInfo->byVideoURL[k][0] != '\0'))
                    {
                        AddLog(-1, ALARM_INFO_T, 0, "[CID_EX]uuid[%s],recheck[%d],VideoURL[%s]", pStruCidInfoEx->byUUID, pStruCidInfoEx->byRecheck, pStruVideoInfo->byVideoURL[k]);
                    }
                }
            }
        }

        //UTF-8转GBK
        DWORD dwOutLen = 0;
        UTF82A(pStruCidInfo->struCIDParam.byUserName, pStruCidInfo->struCIDParam.byUserName, NAME_LEN, &dwOutLen);

        AddLog(-1, ALARM_INFO_T, 0, "[CID]uuid[%s],DeviceID:%s,CID code:%d,CID type:%d,Subsys No:%d,Describe:%s,TriggerTime:%s,UploadTime:%s,CID param[%d %d %d %d %d %d %d %s]", \
            cUUID, pStruCidInfo->byDeviceID, pStruCidInfo->dwCIDCode, pStruCidInfo->dwCIDType, pStruCidInfo->dwSubSysNo, cDescribe, \
            pStruCidInfo->byTriggerTime, pStruCidInfo->byUploadTime, pStruCidInfo->struCIDParam.dwUserType, pStruCidInfo->struCIDParam.lUserNo, \
            pStruCidInfo->struCIDParam.lZoneNo, pStruCidInfo->struCIDParam.lKeyboardNo, pStruCidInfo->struCIDParam.lVideoChanNo, \
            pStruCidInfo->struCIDParam.lDiskNo, pStruCidInfo->struCIDParam.lModuleAddr, pStruCidInfo->struCIDParam.byUserName);
    }
}

void CEHomeDemoDlg::ProcessEhomeNoticePicUrl(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen)
    UN_REFERENCED_PARAMETER(pXml)
    UN_REFERENCED_PARAMETER(dwStruLen)
    if (pStru == NULL)
    {
        return;
    }
    NET_EHOME_NOTICE_PICURL *pStruNoticePicUrl = static_cast<NET_EHOME_NOTICE_PICURL *>(pStru);
    for (int i = 0; i < 256; i++)
    {
        if (pStruNoticePicUrl->byDeviceID[i] == 0xcd)
        {
            pXml = pXml;
        }
    }
    AddLog(-1, ALARM_INFO_T, 0, "[NOTICEPICURL]DeviceID:%s,PicType:%d,AlarmType:%d,AlarmChan:%d,AlarmTime:%s,CaptureChan:%d,PicTime:%s,URL:%s,ManualSeq:%d", \
        pStruNoticePicUrl->byDeviceID, pStruNoticePicUrl->wPicType, pStruNoticePicUrl->wAlarmType, pStruNoticePicUrl->dwAlarmChan, pStruNoticePicUrl->byAlarmTime, pStruNoticePicUrl->dwCaptureChan,\
        pStruNoticePicUrl->byPicTime, pStruNoticePicUrl->byPicUrl, pStruNoticePicUrl->dwManualSnapSeq);
}

void CEHomeDemoDlg::ProcessEhomeNotifyFail(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen)
    UN_REFERENCED_PARAMETER(pXml)
    UN_REFERENCED_PARAMETER(dwStruLen)
    if (pStru == NULL)
    {
        return;
    }
    NET_EHOME_NOTIFY_FAIL_INFO *pStruNotifyFail= static_cast<NET_EHOME_NOTIFY_FAIL_INFO *>(pStru);
    for (int i = 0; i < 256; i++)
    {
        if (pStruNotifyFail->byDeviceID[i] == 0xcd)
        {
            pXml = pXml;
        }
    }
    AddLog(-1, ALARM_INFO_T, 0, "[NOTIFYFAIL]DeviceID:%s,FailedCommand:%d,PicType:%d,ManualSeq:%d", \
        pStruNotifyFail->byDeviceID, pStruNotifyFail->wFailedCommand, pStruNotifyFail->wPicType, pStruNotifyFail->dwManualSnapSeq);
}

void CEHomeDemoDlg::ProcessEhomeAlarmQrCode(void *pXml, DWORD dwXmlLen)
{
    if ((pXml == NULL) || (dwXmlLen == 0))
    {
        return;
    }

    char szInfoBuf[4 * 1024] = { 0 };
    char byOutput[2 * 1024] = { 0 };
    char byJSON[2 * 1024] = { 0 };

    if (XmlPrase((char*)pXml, "<Command>", "</Command>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "[%s] ", byOutput);
    }

    memset(byOutput, 0, 2 * 1024);
    memset(byJSON, 0, 2 * 1024);
    char byStr[32] = { 0 };
    if (XmlPrase((char*)pXml, "<Params>", "</Params>", byOutput))
    {
        strncpy(byJSON, byOutput + 9, strlen(byOutput) - 9 - 3);
        cJSON* pRoot = cJSON_Parse(byJSON);
        if (pRoot != NULL)
        {
            cJSON* pNode = cJSON_GetObjectItem(pRoot, "ipAddress");
            if (pNode != NULL && pNode->type == cJSON_String)
            {
                strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
                sprintf(szInfoBuf + strlen(szInfoBuf), "ipAddress[%s] ", byStr);
            }

            memset(byStr, 0, 32);
            pNode = cJSON_GetObjectItem(pRoot, "dateTime");
            if (pNode != NULL && pNode->type == cJSON_String)
            {
                strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
                sprintf(szInfoBuf + strlen(szInfoBuf), "dateTime[%s] ", byStr);
            }

            pNode = cJSON_GetObjectItem(pRoot, "activePostCount");
            if (pNode != NULL && pNode->type == cJSON_Number)
            {
                sprintf(szInfoBuf + strlen(szInfoBuf), "activePostCount[%d] ", pNode->valueint);
            }
            
            memset(byStr, 0, 32);
            pNode = cJSON_GetObjectItem(pRoot, "eventType");
            if (pNode != NULL && pNode->type == cJSON_String)
            {
                strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
                sprintf(szInfoBuf + strlen(szInfoBuf), "eventType[%s] ", byStr);
            }

            memset(byStr, 0, 32);
            pNode = cJSON_GetObjectItem(pRoot, "eventState");
            if (pNode != NULL && pNode->type == cJSON_String)
            {
                strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
                sprintf(szInfoBuf + strlen(szInfoBuf), "eventState[%s] ", byStr);
            }

            pNode = cJSON_GetObjectItem(pRoot, "QRCodeEvent");
            if (pNode != NULL && pNode->type == cJSON_Object)
            {
                memset(byStr, 0, 32);
                cJSON* pQRCodeInfo = cJSON_GetObjectItem(pRoot, "QRCodeInfo");
                if (pQRCodeInfo != NULL && pQRCodeInfo->type == cJSON_String)
                {
                    strncpy(byStr, pQRCodeInfo->valuestring, strlen(pQRCodeInfo->valuestring));
                    sprintf(szInfoBuf + strlen(szInfoBuf), "QRCodeInfo[%s] ", byStr);
                }
            }
            
            cJSON_Delete(pRoot);
        }
    }


    AddLog(-1, ALARM_INFO_T, 0, "%s", szInfoBuf);
}

void CEHomeDemoDlg::ProcessEhomeAlarmFaceTemp(void *pXml, DWORD dwXmlLen)
{
    if ((pXml == NULL) || (dwXmlLen == 0))
    {
        return;
    }

    char szInfoBuf[4 * 1024] = { 0 };
    char byOutput[2 * 1024] = { 0 };
    char byJSON[2 * 1024] = { 0 };

    if (XmlPrase((char*)pXml, "<Command>", "</Command>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "[%s] ", byOutput);
    }

    memset(byOutput, 0, 2 * 1024);
    memset(byJSON, 0, 2 * 1024);
    char byStr[32] = { 0 };
    if (XmlPrase((char*)pXml, "<Params>", "</Params>", byOutput))
    {
        strncpy(byJSON, byOutput + 9, strlen(byOutput) - 9 - 3);
        cJSON* pRoot = cJSON_Parse(byJSON);
        if (pRoot != NULL)
        {
            cJSON* pNode = cJSON_GetObjectItem(pRoot, "ipAddress");
            if (pNode != NULL && pNode->type == cJSON_String)
            {
                strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
                sprintf(szInfoBuf + strlen(szInfoBuf), "ipAddress[%s] ", byStr);
            }

            memset(byStr, 0, 32);
            pNode = cJSON_GetObjectItem(pRoot, "dateTime");
            if (pNode != NULL && pNode->type == cJSON_String)
            {
                strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
                sprintf(szInfoBuf + strlen(szInfoBuf), "dateTime[%s] ", byStr);
            }

            pNode = cJSON_GetObjectItem(pRoot, "activePostCount");
            if (pNode != NULL && pNode->type == cJSON_Number)
            {
                sprintf(szInfoBuf + strlen(szInfoBuf), "activePostCount[%d] ", pNode->valueint);
            }

            memset(byStr, 0, 32);
            pNode = cJSON_GetObjectItem(pRoot, "eventType");
            if (pNode != NULL && pNode->type == cJSON_String)
            {
                strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
                sprintf(szInfoBuf + strlen(szInfoBuf), "eventType[%s] ", byStr);
            }

            memset(byStr, 0, 32);
            pNode = cJSON_GetObjectItem(pRoot, "eventState");
            if (pNode != NULL && pNode->type == cJSON_String)
            {
                strncpy(byStr, pNode->valuestring, strlen(pNode->valuestring));
                sprintf(szInfoBuf + strlen(szInfoBuf), "eventState[%s] ", byStr);
            }

            pNode = cJSON_GetObjectItem(pRoot, "FaceTemperatureMeasurementEvent");
            if (pNode != NULL && pNode->type == cJSON_Object)
            {
                memset(byStr, 0, 32);
                cJSON* pChildNode = cJSON_GetObjectItem(pRoot, "thermometryUnit");
                if (pChildNode != NULL && pChildNode->type == cJSON_String)
                {
                    strncpy(byStr, pChildNode->valuestring, strlen(pChildNode->valuestring));
                    sprintf(szInfoBuf + strlen(szInfoBuf), "thermometryUnit[%s] ", byStr);
                }

                pChildNode = cJSON_GetObjectItem(pRoot, "currTemperature");
                if (pChildNode != NULL && pChildNode->type == cJSON_Number)
                {
                    sprintf(szInfoBuf + strlen(szInfoBuf), "thermometryUnit[%f] ", pChildNode->valuedouble);
                }
            }

            cJSON_Delete(pRoot);
        }
    }


    AddLog(-1, ALARM_INFO_T, 0, "%s", szInfoBuf);
}

void CEHomeDemoDlg::ProcessEhomeAlarmAcs(void *pXml, DWORD dwXmlLen)
{
    if ((pXml == NULL) || (dwXmlLen == 0))
    {
        return;
    }

    char szInfoBuf[4*1024] = { 0 };
    char byOutput[1024] = { 0 };

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<Command>", "</Command>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "[%s] ", byOutput);
    }

    if (strcmp("ACS", byOutput) || strcmp("IDCARDINFO", byOutput))
    {
        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<currentEvent>", "</currentEvent>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "currentEvent[%s] ", byOutput);
        }

        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<QRCodeInfo>", "</QRCodeInfo>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "QRCodeInfo[%s] ", byOutput);
        }

        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<thermometryUnit>", "</thermometryUnit>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "thermometryUnit[%s] ", byOutput);
        }

        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<currTemperature>", "</currTemperature>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "currTemperature[%s] ", byOutput);
        }

        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<isAbnomalTemperature>", "</isAbnomalTemperature>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "isAbnomalTemperature[%s] ", byOutput);
        }

        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<remoteCheck>", "</remoteCheck>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "remoteCheck[%s] ", byOutput);
        }

        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<mask>", "</mask>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "mask[%s] ", byOutput);
        }

        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<visibleLightURL>", "</visibleLightURL>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "visibleLightURL[%s] ", byOutput);
        }

        memset(byOutput, 0, 32);
        if (XmlPrase((char*)pXml, "<thermalURL>", "</thermalURL>", byOutput))
        {
            sprintf(szInfoBuf + strlen(szInfoBuf), "thermalURL[%s] ", byOutput);
        }
    }
    

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<DeviceID>", "</DeviceID>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "DeviceID:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<MajorType>", "</MajorType>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "Major:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<MinorType>", "</MinorType>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "Minor:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<IDNum>", "</IDNum>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "IDNum:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<passportNo>", "</passportNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "passportNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<byOCR>", "</byOCR>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "OCR:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<name>", "</name>", byOutput))
    {
        DWORD dwConvertLen = 0;
        UTF82A((char*)byOutput, (char*)byOutput, 32, &dwConvertLen);
        sprintf(szInfoBuf + strlen(szInfoBuf), "name:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<countryIssue>", "</countryIssue>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "countryIssue:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<birth>", "</birth>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "birth:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<expireData>", "</expireData>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "expireData:%s ", byOutput);
    }


    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<Time>", "</Time>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "Time:%s ", byOutput);
    }

    memset(byOutput, 0, 99);
    if (XmlPrase((char*)pXml, "<localName>", "</localName>", byOutput))
    {
        DWORD dwConvertLen = 0;
        UTF82A((char*)byOutput, (char*)byOutput, 99, &dwConvertLen);
        sprintf(szInfoBuf + strlen(szInfoBuf), "localName:%s ", byOutput);
    }
    memset(byOutput, 0, 99);
    if (XmlPrase((char*)pXml, "<placeOfBirth>", "</placeOfBirth>", byOutput))
    {
        DWORD dwConvertLen = 0;
        UTF82A((char*)byOutput, (char*)byOutput, 99, &dwConvertLen);
        sprintf(szInfoBuf + strlen(szInfoBuf), "placeOfBirth:%s ", byOutput);
    }
    memset(byOutput, 0, 99);
    if (XmlPrase((char*)pXml, "<addr>", "</addr>", byOutput))
    {
        DWORD dwConvertLen = 0;
        UTF82A((char*)byOutput, (char*)byOutput, 99, &dwConvertLen);
        sprintf(szInfoBuf + strlen(szInfoBuf), "addr:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<phone>", "</phone>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "phone:%s ", byOutput);
    }

    memset(byOutput, 0, 99);
    if (XmlPrase((char*)pXml, "<job>", "</job>", byOutput))
    {
        DWORD dwConvertLen = 0;
        UTF82A((char*)byOutput, (char*)byOutput, 99, &dwConvertLen);
        sprintf(szInfoBuf + strlen(szInfoBuf), "job:%s ", byOutput);
    }
    memset(byOutput, 0, 99);
    if (XmlPrase((char*)pXml, "<title>", "</title>", byOutput))
    {
        DWORD dwConvertLen = 0;
        UTF82A((char*)byOutput, (char*)byOutput, 99, &dwConvertLen);
        sprintf(szInfoBuf + strlen(szInfoBuf), "title:%s ", byOutput);
    }
    memset(byOutput, 0, 99);
    if (XmlPrase((char*)pXml, "<resume>", "</resume>", byOutput))
    {
        DWORD dwConvertLen = 0;
        UTF82A((char*)byOutput, (char*)byOutput, 99, &dwConvertLen);
        sprintf(szInfoBuf + strlen(szInfoBuf), "resume:%s ", byOutput);
    }
    memset(byOutput, 0, 999);
    if (XmlPrase((char*)pXml, "<monitor>", "</monitor>", byOutput))
    {
        DWORD dwConvertLen = 0;
        UTF82A((char*)byOutput, (char*)byOutput, 999, &dwConvertLen);
        sprintf(szInfoBuf + strlen(szInfoBuf), "monitor:%s ", byOutput);
    }

    /*memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<NetUserName>", "</NetUserName>", byOutput))
    {
    sprintf(szInfoBuf + strlen(szInfoBuf), "Name:%s ", byOutput);
    }*/

    /*memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<NetUserIp>", "</NetUserIp>", byOutput))
    {
    sprintf(szInfoBuf + strlen(szInfoBuf), "Ip:%s ", byOutput);
    }*/

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<CardNo>", "</CardNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "CardNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<CardType>", "</CardType>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "CardType:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<CardReaderNo>", "</CardReaderNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "CardReaderNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<DoorNo>", "</DoorNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "DoorNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<MultiCardVerifyNo>", "</MultiCardVerifyNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "MultiCardVerifyNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<MultiCardGroupNo>", "</MultiCardGroupNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "MultiCardGroupNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<AlarmInNo>", "</AlarmInNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "AlarmInNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<AlarmOutNo>", "</AlarmOutNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "AlarmOutNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<CaseSensorNo>", "</CaseSensorNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "CaseSensorNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<Rs485No>", "</Rs485No>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "Rs485No:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<RetransFlag>", "</RetransFlag>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "RetransFlag:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<WhiteListNo>", "</WhiteListNo>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "WhiteListNo:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<ReportChannel>", "</ReportChannel>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "ReportChannel:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<MacAddr>", "</MacAddr>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "MacAddr:%s ", byOutput);
    }

    memset(byOutput, 0, 32);
    if (XmlPrase((char*)pXml, "<SwipeCardType>", "</SwipeCardType>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "SwipeCardType:%s ", byOutput);
    }

    AddLog(-1, ALARM_INFO_T, 0, "%s", szInfoBuf);
}

BOOL CEHomeDemoDlg::XmlPrase(char* pXml, char* pInputBefore, char* pInputAfter, char* pOutput)
{
    if ((pXml == NULL) || (pInputBefore == NULL) || (pInputAfter == NULL) || (pOutput == NULL))
    {
        return false;
    }
    if ((strlen(pInputBefore) == 0) || (strlen(pInputAfter) == 0))
    {
        return false;
    }

    char* pBefore = strstr(pXml, pInputBefore);
    char* pAfter = strstr(pXml, pInputAfter);
    if ((pBefore == NULL) || (pAfter == NULL) || ((pBefore + strlen(pInputBefore)) == pAfter))
    {
        return false;
    }

    int iLength = (pAfter - pBefore - strlen(pInputBefore)) / sizeof(char);
    memcpy(pOutput, (pBefore + strlen(pInputBefore)), iLength);

    return true;
}

void CEHomeDemoDlg::ProcessEhomeWirelessInfo(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen)
        UN_REFERENCED_PARAMETER(pXml)
        UN_REFERENCED_PARAMETER(dwStruLen)
        NET_EHOME_ALARMWIRELESSINFO *pStruWirelessInfo = static_cast<NET_EHOME_ALARMWIRELESSINFO *>(pStru);
    AddLog(-1, ALARM_INFO_T, 0, "[Wireless]DeviceID:%s,DataTraffic:%f,SignalIntensity:%d", pStruWirelessInfo->byDeviceID, ((float)pStruWirelessInfo->dwDataTraffic) / 100, pStruWirelessInfo->bySignalIntensity);
}

/** @fn void CEHomeDemoDlg::ProcessHttpAlarmInfo(void *pXml, DWORD dwXmlLen, void *pUrl, DWORD dwUrlLen)
 *  @brief 解析HTTP报警
 *  @param (in)	void * pXml    
 *  @param (in)	DWORD dwXmlLen    
 *  @param (in)	void * pUrl    
 *  @param (in)	DWORD dwUrlLen    
 *  @return void
 */
void CEHomeDemoDlg::ProcessHttpAlarmInfo(void *pXml, DWORD dwXmlLen, void *pUrl, DWORD dwUrlLen)
{
	char szLan[128] = { 0 };

	if (pUrl != NULL && dwUrlLen > 0)
    {
        //ISUP4.0事件上报，默认报警与图片数据不分离，此时返回的url不为空，这种情况增加下demo打印
		//分离后的数据，url字段不为空
		g_StringLanType(szLan, "ISAPI报警上传", "ISAPI Alarm");
		AddLog(-1, ALARM_INFO_T, 0, szLan);
		return;
    }

    if (pXml == NULL)
    {
        return;
    }
    LPNET_EHOME_ALARM_ISAPI_INFO pISAPIAlarm = (NET_EHOME_ALARM_ISAPI_INFO *)(pXml);

    NET_EHOME_ALARM_ISAPI_INFO struISAPIAlarm = { 0 };
    memcpy(&struISAPIAlarm, pISAPIAlarm, sizeof(NET_EHOME_ALARM_ISAPI_INFO));

    SYSTEMTIME t;
    GetLocalTime(&t);
    char chTime[128];
    sprintf(chTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);

    char cEventType[32] = { 0 };
    char cCmdType[64] = { 0 };
    char cDeviceID[256] = { 0 };
    if (struISAPIAlarm.pAlarmData != NULL)
    {
        char cFilename[256] = { 0 };
        char cExt[12] = { 0 };
        char cSubFilename[256] = { 0 };
        DWORD dwWrittenBytes = 0;
        sprintf(cFilename, "%s\\%s[%4.4d%2.2d%2.2d]", "C:\\Picture", "ISAPIAlarmData",t.wYear, t.wMonth, t.wDay);
        if (GetFileAttributes(cFilename) != FILE_ATTRIBUTE_DIRECTORY)
        {
            CreateDirectory(cFilename, NULL);
        }

        memcpy(cSubFilename, cFilename, 256);
        //判断报警数据格式
        if (struISAPIAlarm.byDataType == 2)
        {
            sprintf(cExt, "%s", "json");
            cJSON* pRoot = cJSON_Parse(struISAPIAlarm.pAlarmData);
            if (pRoot != NULL)
            {
                cJSON* pPercent = cJSON_GetObjectItem(pRoot, "eventType");
                if (pPercent != NULL && pPercent->type == cJSON_String)
                {
                    strncpy(cEventType, pPercent->valuestring, strlen(pPercent->valuestring));
                }

                cJSON* pNode = cJSON_GetObjectItem(pRoot, "VoiceTalkEvent");
                if (pNode != NULL && pNode->type == cJSON_Object)
                {
                    cJSON* pCmdType = cJSON_GetObjectItem(pNode, "cmdType");
                    if (pCmdType != NULL && pCmdType->type == cJSON_String)
                    {
                        strncpy(cCmdType, pCmdType->valuestring, strlen(pCmdType->valuestring));
                    }
                }

                pNode = cJSON_GetObjectItem(pRoot, "deviceID");
                if (pNode != NULL && pNode->type == cJSON_String)
                {
                    strncpy(cDeviceID, pNode->valuestring, strlen(pNode->valuestring));
                }
                cJSON_Delete(pRoot);
            }
        }
        else if (struISAPIAlarm.byDataType == 1)
        {
            XmlPrase((char*)struISAPIAlarm.pAlarmData, "<eventType>", "</eventType>", cEventType);
            sprintf(cExt, "%s", "xml");
        }
        else
        {
            sprintf(cExt, "%s", "txt");
        }

        g_StringLanType(szLan, "ISAPI报警上传", "ISAPI Alarm");
        sprintf(szLan, "%s:EventType[%s]", szLan, cEventType);
        AddLog(-1, ALARM_INFO_T, 0, szLan);

        char cFilename2[256] = { 0 };
        sprintf(cFilename2, "%s\\[%s]COMM_ISAPI_ALARM_SPILT[%s]_%d.%s", cFilename, cEventType, chTime, rand() % GetTickCount(), cExt);

        HANDLE hFile2 = CreateFile(cFilename2, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile2 == INVALID_HANDLE_VALUE)
        {
            return;
        }
        DWORD dwRet = WriteFile(hFile2, struISAPIAlarm.pAlarmData, struISAPIAlarm.dwAlarmDataLen, &dwWrittenBytes, NULL);
        if (dwRet == 0 || dwWrittenBytes < struISAPIAlarm.dwAlarmDataLen)
        {
            DWORD dwError = GetLastError();
            g_pMainDlg->AddLog(-1, OPERATION_FAIL_T,0, "Frames People Counting Info Wtite to File Fail.System Error:%d, dwJsonDataLen:%d, WrittenBytes:%d ", dwError, struISAPIAlarm.dwAlarmDataLen, dwWrittenBytes);
        }
        CloseHandle(hFile2);

        sprintf(cFilename, "%s\\[%s]%s.%s", cFilename, cEventType, "COMM_ISAPI_ALARM", cExt);

        CFile file;
        if (!file.Open(cFilename, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeReadWrite))
        {
            return;
        }
        file.SeekToEnd();
        file.Write(struISAPIAlarm.pAlarmData, struISAPIAlarm.dwAlarmDataLen);
        char szEnd[3] = { 0 };
        szEnd[0] = '\r';
        szEnd[1] = '\n';
        file.Write(szEnd, strlen(szEnd));
        file.Close();

        if (struISAPIAlarm.pPicPackData != NULL)
        {
            int i = 0;
            while (i <= struISAPIAlarm.byPicturesNumber - 1)
            {
                DWORD dwWrittenBytes = 0;
                {
                    sprintf(cFilename, "%s\\[%s]PicData[%s]_%s_%d.jpg", cSubFilename, cEventType, chTime, ((NET_EHOME_ALARM_ISAPI_PICDATA *)struISAPIAlarm.pPicPackData)[i].szFilename, rand() % GetTickCount());
                }

                HANDLE hFile = CreateFile(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile == INVALID_HANDLE_VALUE)
                {
                    return;
                }
                DWORD dwRet = WriteFile(hFile, ((NET_EHOME_ALARM_ISAPI_PICDATA *)struISAPIAlarm.pPicPackData)[i].pPicData, ((NET_EHOME_ALARM_ISAPI_PICDATA *)struISAPIAlarm.pPicPackData)[i].dwPicLen, &dwWrittenBytes, NULL);
                if (dwRet == 0 || dwWrittenBytes < ((NET_EHOME_ALARM_ISAPI_PICDATA *)struISAPIAlarm.pPicPackData)[i].dwPicLen)
                {
                    DWORD dwError = GetLastError();
                    g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, 0, "Frames People Counting Info Wtite to File Fail.System Error:%d, dwJsonDataLen:%d, WrittenBytes:%d ", dwError, ((NET_EHOME_ALARM_ISAPI_PICDATA *)struISAPIAlarm.pPicPackData)[i].dwPicLen, dwWrittenBytes);
                }
                CloseHandle(hFile);;
                i++;
            }
        }
    }

    // 处理设备上报的对讲信令交互事件
    if (!strcmp(cEventType, "voiceTalkEvent"))
    {
        ManageVideoIntercomeCmd(cCmdType, cDeviceID, struISAPIAlarm.pAlarmData);
    }

    return;
}


void CEHomeDemoDlg::ProcessEhomeMPDCData(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
{
    UN_REFERENCED_PARAMETER(dwXmlLen);
    UN_REFERENCED_PARAMETER(pXml);
    UN_REFERENCED_PARAMETER(dwStruLen);
    NET_EHOME_ALARM_MPDCDATA *pStruMPDCData = static_cast<NET_EHOME_ALARM_MPDCDATA *>(pStru);
    AddLog(-1, ALARM_INFO_T, 0, "[MPDCData]DeviceID:%s,SampleTime:%s,RetranseFlag:%d,Count:%d",
        pStruMPDCData->byDeviceID,
        pStruMPDCData->bySampleTime,
        pStruMPDCData->byRetranseFlag,
        pStruMPDCData->struMPData.dwCount);
}

static VI_CMD GetVedioIntercomeCmd(const char*cCmdType)
{
    assert((cCmdType != NULL) && (strlen(cCmdType) > 0));

    for (int i = 0; i < sizeof(g_cmdString) / sizeof(g_cmdString[0]); i++)
    {
        if (!strcmp(g_cmdString[i], cCmdType))
        {
            return (VI_CMD)i;
        }
    }

    return VI_UNKNOWN;
}

void CEHomeDemoDlg::ManageVideoIntercomeCmd(const char*cCmdType, const char*cDeviceID, const char *szEvent)
{
    if (cCmdType == NULL || strlen(cCmdType) == 0 || cDeviceID == NULL || strlen(cDeviceID) == 0)
    {
        AddLog(-1, 0, OPERATION_FAIL_T, "ManageVideoIntercomeCmd::parameter error!");
        return;
    }

    if (m_pDlgVI == NULL)
    {
        return;
    }
    
    m_pDlgVI->ShowWindow(SW_SHOW);

    char * pDeviceID = new char[256];
    memset(pDeviceID, 0, 256);
    ASSERT(strlen(cDeviceID) < 256);
    strncpy(pDeviceID, cDeviceID, strlen(cDeviceID));

    m_pDlgVI->m_iDevIndex = -1;
    for (int i = 0; i < 1024; i++)
    {
        if (!strcmp((char *)g_struDeviceInfo[i].byDeviceID, pDeviceID))
        {
            m_pDlgVI->m_iDevIndex = i;
            break;
        }
    }

    if (m_pDlgVI->m_iDevIndex == -1)
    {
        AddLog(-1, 0, OPERATION_FAIL_T, "ManageVideoIntercomeCmd::this DeviceID didn't registered");
        return;
    }

    VI_CMD viCmd = GetVedioIntercomeCmd(cCmdType);
    ::PostMessage(m_pDlgVI->m_hWnd, WM_VEDIO_INTERCOME_CMD, viCmd, (LPARAM)pDeviceID);
    return;
}

// void CEHomeDemoDlg::ProcessEhomeGpsData(void *pStru, DWORD dwStruLen, void *pXml, DWORD dwXmlLen)
// {
//     UN_REFERENCED_PARAMETER(dwXmlLen)
//     UN_REFERENCED_PARAMETER(pXml)
//     UN_REFERENCED_PARAMETER(dwStruLen)
//     NET_EHOME_GPS_INFO *pStruGpsData = static_cast<NET_EHOME_GPS_INFO *>(pStru);
//     AddLog(-1, ALARM_INFO_T, 0, "[GPSDATA]DeviceID:%s,Time:%s,bySvsNum:%d,byLocateMode:%d,wPrecision:%d,dwHeight:%d,iLatitude:%d,iLongitude:%d,dwVehicleSpeed:%d,dwVehicleDirection:%d,dwMileage:%d",
//         pStruGpsData->byDeviceID, pStruGpsData->bySampleTime, pStruGpsData->bySatelites, pStruGpsData->byLocateMode, pStruGpsData->byPrecision,
//         pStruGpsData->dwHeight, pStruGpsData->dwLatitude,pStruGpsData->dwLongitude, pStruGpsData->dwSpeed, pStruGpsData->dwDirection, pStruGpsData->dwMileage);
// }

//void CEHomeDemoDlg::OnTvnSelchangedTreeDev(NMHDR *pNMHDR, LRESULT *pResult)
//{
//    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
//    // TODO: Add your control notification handler code here
//    *pResult = 0;
//}

void CEHomeDemoDlg::OnNMClickTreeDev(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    UN_REFERENCED_PARAMETER(pNMHDR)
    CPoint pt(0,0);
    GetCursorPos(&pt);

    CRect rect(0,0,0,0);
    ScreenToClient(&pt);
    GetDlgItem(IDC_TREE_DEV)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    pt.x = pt.x-rect.left;
    pt.y = pt.y-rect.top;

    UINT uFlags = 0;
    HTREEITEM hSelect = m_treeDeviceList.HitTest(pt,&uFlags);
    if (hSelect == NULL)
    {
        AddLog(m_iCurDeviceIndex, OPERATION_FAIL_T, 0, "please click the right node!");
        return;
    }

    DWORD dwNoteData = m_treeDeviceList.GetItemData(hSelect);
    int iType = dwNoteData/1000;
    int iIndex = dwNoteData%1000;
    if (DEVICETYPE == iType)
    {
        SetDeviceIndex(iIndex);
        //m_iCurDeviceIndex = iIndex;
        m_hCurDeviceItem = hSelect;
        m_iCurChanIndex = 0;    //如果仅选中设备树，默认第1个通道
        m_hCurChanItem = NULL;
    }
    else if (CHANNELTYPE == iType)
    {
        m_iCurChanIndex = iIndex;
        m_hCurChanItem = hSelect;
        HTREEITEM hParent = m_treeDeviceList.GetParentItem(hSelect);
        if (hParent != NULL)
        {
            dwNoteData = m_treeDeviceList.GetItemData(hParent);
            iType = dwNoteData/1000;
            iIndex = dwNoteData%1000;
            if (DEVICETYPE == iType)
            {
                //m_iCurDeviceIndex = iIndex;
                SetDeviceIndex(iIndex);
                m_hCurDeviceItem = hParent;
            }
        }
    }
    else
    {
        //m_iCurDeviceIndex = -1;
        SetDeviceIndex(-1);
        m_hCurDeviceItem = NULL;
        m_iCurChanIndex = -1;
        m_hCurChanItem = NULL;
    }

    //DeviceIDChanged();


    if (m_iMainType == PLAY_BACK_T)
    {
        m_dlgPlayBack->PlayBackWinUpdate();
    }

    *pResult = 0;
}

void CEHomeDemoDlg::OnNMDblclkTreeDev(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    UN_REFERENCED_PARAMETER(pNMHDR)
    HTREEITEM hSelect = m_treeDeviceList.GetSelectedItem();
    if (NULL == hSelect || m_iMainType != PREVIEW_T)//it works to double click device tree only while preview
    {
        return;
    }

//     if (g_bPlayAllDevice)//disable to single designated play while all play
//     {
//         char szLan[128] = {0};
//         g_StringLanType(szLan, "请先停止全部播放", "Please stop all play first");
//         AfxMessageBox(szLan);
//         return;
//     }

    DWORD dwNodeData = (DWORD)m_treeDeviceList.GetItemData(hSelect);
    HTREEITEM hParent = NULL;
    int iType = dwNodeData/1000;
    int iIndex = dwNodeData%1000;
    int iPlayWndIndex = -1;
    switch (iType)
    {
    case TREE_ALL_T:
        //DblAllTree();
        break;
    case DEVICETYPE:
       // m_iCurDeviceIndex = iIndex;
        SetDeviceIndex(iIndex);
        m_hCurDeviceItem = hSelect;
        m_iCurChanIndex = -1;
        m_hCurChanItem = NULL;
        if (GetCurDeviceIndex() != -1)
        {
            PlayDevice(m_iCurDeviceIndex, m_iCurWndIndex);
        }        
        m_treeDeviceList.Expand(hSelect, TVE_COLLAPSE);//expend reverse operation
        //    m_treeDeviceList.Expand(hSelect,TVE_EXPAND);
        break;
    case CHANNELTYPE:  //double click channel
        m_iCurChanIndex = iIndex;
        m_hCurChanItem = hSelect;
        hParent = m_treeDeviceList.GetParentItem(hSelect);
        if (hParent != NULL)
        {
            if (DEVICETYPE == m_treeDeviceList.GetItemData(hParent)/1000)
            {
               // m_iCurDeviceIndex = m_treeDeviceList.GetItemData(hParent)%1000;
                SetDeviceIndex(m_treeDeviceList.GetItemData(hParent) % 1000);
                m_hCurDeviceItem = hParent;
            }
        }
        /*如果当前通道在窗口上进行预览，切换预览窗口时应该先把之前的预览窗口关闭*/
        /*获取当前通道正在进行预览的窗口*/
        iPlayWndIndex = g_struDeviceInfo[g_pMainDlg->m_iCurDeviceIndex].struChanInfo[g_pMainDlg->m_iCurChanIndex].iPlayWndIndex;
        if (iPlayWndIndex >= 0 && m_pDlgPreview[iPlayWndIndex].m_bPlay)
        {
            /*判断当前通道是否有预览窗口在进行预览*/
            char szLan[128] = { 0 };
            if (!m_pDlgPreview[iPlayWndIndex].StopPlay())
            {
                /*停止预览*/
                g_StringLanType(szLan, "停止预览失败！", "Stop previewing Failed");
                g_pMainDlg->AddLog(m_iCurDeviceIndex, OPERATION_FAIL_T, 0, szLan);
                //AfxMessageBox(szLan);
            }
            g_struDeviceInfo[m_iCurDeviceIndex].struChanInfo[iIndex].bPlay = false;
            g_struDeviceInfo[m_iCurDeviceIndex].struChanInfo[iIndex].iPlayWndIndex = -1;
        }
        else
        {
            g_struDeviceInfo[g_pMainDlg->m_iCurDeviceIndex].struChanInfo[g_pMainDlg->m_iCurChanIndex].iPlayWndIndex = m_iCurWndIndex;
            PlayChan(m_iCurDeviceIndex, iIndex, hSelect);
        }
        break;
    default:
        {
            m_iCurChanIndex = -1;
            m_hCurChanItem = NULL;
           // m_iCurDeviceIndex = -1;
            SetDeviceIndex(-1);
            m_hCurDeviceItem = NULL;
        }
        break;
    }
    DeviceIDChanged();
    *pResult = 0;
}

void CEHomeDemoDlg::PlayDevice(int iDeviceIndex, int iStartOutputIndex)
{
    UN_REFERENCED_PARAMETER(iStartOutputIndex)
    HTREEITEM hSelect = m_treeDeviceList.GetSelectedItem();
    m_bCyclePreview = true;

    int i=0;
    int iChanIndex = 0;

    HTREEITEM hChannel = m_treeDeviceList.GetChildItem(hSelect);

    if (hChannel ==  NULL)
    {
        g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 0, "there is no channels!!");
        return;
    }
    
    if (g_struDeviceInfo[iDeviceIndex].bPlayDevice)
    {
        g_struDeviceInfo[iDeviceIndex].bPlayDevice = FALSE;
        //StopPlayAll();

        for (i=0; i< m_iCurWndNum; i++)
        {
            if (m_pDlgPreview[i].m_iDeviceIndex == iDeviceIndex)
            {
                m_pDlgPreview[i].StopPlay();
            }    
        }
        m_iCurWndIndex = 0;
    }
    else
    {
        for (i=0; i<m_iCurWndNum; i++)
        {
            //shut down all play channel
            if (m_pDlgPreview[i].m_iDeviceIndex == iDeviceIndex)
            {
                m_pDlgPreview[i].StopPlay();
            }    

            if (m_pDlgPreview[i].m_lPlayHandle < 0)
            {//play the window that is not previewed
                iChanIndex = m_treeDeviceList.GetItemData(hChannel)%1000;
                m_pDlgPreview[i].StartPlay(&g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex], hChannel);
                hChannel = m_treeDeviceList.GetNextSiblingItem(hChannel);
                if (hChannel == NULL)
                {
                    break;
                }
            }

        }
        g_struDeviceInfo[iDeviceIndex].bPlayDevice = TRUE;
     }

    //ChangePlayBtnState();
}
void CEHomeDemoDlg::PlayChan(int iDeviceIndex, int iChanIndex,HTREEITEM hChanItem)
{
    //设备下线时候，会对deviceindex进行赋值，多线程操作，所以需要判断下
    if (iDeviceIndex < 0)
    {
        g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 0, "CEHomeDemoDlg::PlayChan iDeviceIndex < 0");
        return;
    }
    m_bCyclePreview = false;
    char szLan[128] = {0};
    int iPlayWndIndex = g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex].iPlayWndIndex;
    //iPlayWndIndex小于0，说明没有为该通道指定过播放窗口
    //if (/*m_pDlgPreview[iPlayWndIndex].m_bPlay*/iPlayWndIndex >= 0)
    if(g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex].bPlay)
    {
        if (iPlayWndIndex < 0)
        {
            g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex].bPlay = false;
            g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 0, "CEHomeDemoDlg::PlayChan iPlayWndIndex < 0");
            return;
        }

        if (!m_pDlgPreview[iPlayWndIndex].StopPlay())
        {
            g_StringLanType(szLan, "停止预览失败！", "Stop previewing Failed");
            g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 0, szLan);
            //AfxMessageBox(szLan);
        }
        g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex].bPlay = false;
        g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex].iPlayWndIndex = -1;
    }
    else
    {
        //STRU_CHANNEL_INFO struChanInfo;
        //memcpy(&struChanInfo, &g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex], sizeof(STRU_CHANNEL_INFO));
        //ISUP4.0预览第二通道首次预览失败，因为预览sessionID还没有赋值给g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex].dwPreviewSessionId,码流已回调，比对出错
        if (!m_pDlgPreview[m_iCurWndIndex].StartPlay(&g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex], hChanItem)) 
        {
            g_StringLanType(szLan, "预览失败！", "Preview Failed");
            g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 0, szLan);
            //AfxMessageBox(szLan);
        }
        g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex].bPlay = true;

        //g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex].dwPreviewSessionId = struChanInfo.dwPreviewSessionId;
        return;
    }

    //if the channel is already previewed, stop it
//     if (!m_pDlgPreview[m_iCurWndIndex].StopPlay())
//     {
//         g_StringLanType(szLan, "停止预览失败！", "Stop previewing Failed");
//         //AfxMessageBox(szLan);
//     }
}

void CEHomeDemoDlg::ChangePlayBtnState(void)
{
    int i = 0;
    char szLan[32] = {0};
    if (m_iMainType == PREVIEW_T)
    {
        for (i = 0; i < MAX_WIN_NUM; i++)
        {
            if (!m_pDlgPreview[i].m_bPlay)
            {
                continue;
            }

            m_comboWinNum.EnableWindow(FALSE);
            g_StringLanType(szLan, "停 止", "Stop");
            GetDlgItem(IDC_BTN_CIRCLE_PREVIEW)->SetWindowText(szLan);
            return;
        }
    }

    m_comboWinNum.EnableWindow(TRUE);
    g_StringLanType(szLan, "播 放", "Play");
    GetDlgItem(IDC_BTN_CIRCLE_PREVIEW)->SetWindowText(szLan);
}

LRESULT CEHomeDemoDlg::ChangeChannelItemImage(WPARAM wParam, LPARAM lParam)
{
    int iDeviceIndex = int(wParam);
    int iChanIndex = int(lParam);
    if (iDeviceIndex < 0 || iDeviceIndex >MAX_DEVICES\
        || iChanIndex > 512 || iChanIndex < 0)
    {
        OutputDebugString("dev index and chan num err!\n");
        return NULL;
    }

    HTREEITEM hChanItem = GetChanItem(iDeviceIndex, iChanIndex);
    if (NULL == hChanItem)
    {
        OutputDebugString("chan item NULL!\n");
        return NULL;
    }
    int iImage = CHAN_ORIGINAL;
    STRU_CHANNEL_INFO struChanInfo;
    BOOL bPlaying = FALSE;
    memcpy(&struChanInfo, &g_struDeviceInfo[iDeviceIndex].struChanInfo[iChanIndex], sizeof(struChanInfo));


    //bPlaying = struChanInfo.iPlayWndIndex >= 0? TRUE:FALSE;
    bPlaying = struChanInfo.bPlay;
    if (bPlaying)
    {
        iImage = CHAN_PLAY;    
    }
    else
    {
        iImage = CHAN_ORIGINAL;    
    }

    m_treeDeviceList.SetItemImage(hChanItem,iImage,iImage);    

    g_struDeviceInfo[struChanInfo.iDeviceIndex].struChanInfo[struChanInfo.iChanIndex].dwImageType = iImage;

    return 0;
}

HTREEITEM CEHomeDemoDlg::GetChanItem(int iDeviceIndex, int iChanIndex)
{
    HTREEITEM hRoot = m_treeDeviceList.GetRootItem();
    if (hRoot == NULL)
    {
        return NULL;
    }
    int iChanData = 0;
    HTREEITEM hChannel = NULL;

    HTREEITEM hDevItem = m_treeDeviceList.GetChildItem(hRoot);
    while (hDevItem != NULL)
    {
        if (DEVICETYPE == (int)m_treeDeviceList.GetItemData(hDevItem)/1000)
        {
            if (iDeviceIndex ==  (int)m_treeDeviceList.GetItemData(hDevItem)%1000)
            {
                hChannel = m_treeDeviceList.GetChildItem(hDevItem);    
                if (hChannel == NULL)
                {
                    return NULL;
                }
                iChanData = m_treeDeviceList.GetItemData(hChannel);
                while (iChanData%1000 != iChanIndex)
                {
                    hChannel = m_treeDeviceList.GetNextSiblingItem(hChannel);
                    if (hChannel == NULL)
                    {
                        AddLog(iDeviceIndex, OPERATION_FAIL_T, 0, "don't find chanindex[%d] node!!!", iChanIndex);
                        return NULL;
                    }
                    else
                    {
                        iChanData = m_treeDeviceList.GetItemData(hChannel);
                    }

                }
                return hChannel;
            }
        }
        hDevItem = m_treeDeviceList.GetNextVisibleItem(hDevItem);
    }//end while

    return NULL;
}

void CEHomeDemoDlg::OnBnClickedBtnOther()
{
    // TODO: Add your control notification handler code here
    CMenu pMenu;
    CRect rectBtnElse(0,0,0,0);
    GetDlgItem(IDC_BTN_OTHER)->GetWindowRect(&rectBtnElse);

    if (!pMenu.LoadMenu(IDR_MENU_ELSE))
    {
        return;
    }

    pMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, rectBtnElse.left,rectBtnElse.bottom,this);
}

void CEHomeDemoDlg::OnBnClickedBtnPreviewListen()
{
    // TODO: 在此添加控件通知处理程序代码
    CDlgListen dlg;
    dlg.m_pFatherDlg = this;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnNMRClickTreeDev(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    UN_REFERENCED_PARAMETER(pNMHDR)
    CMenu pMenu;
    HTREEITEM hParent=NULL;
    CPoint pt(0,0);
    GetCursorPos(&pt);

    CRect rect(0,0,0,0);
    ScreenToClient(&pt);
    GetDlgItem(IDC_TREE_DEV)->GetWindowRect(&rect);
    ScreenToClient(&rect);

    pt.x = pt.x-rect.left;
    pt.y = pt.y-rect.top;
    //while select first node or blank
    //HTREEITEM hSelect = m_treeDeviceList.GetSelectedItem();
    UINT uFlags = 0;
    HTREEITEM hSelect = m_treeDeviceList.HitTest(pt,&uFlags);
    if (hSelect == NULL )//|| m_iMainType != PREVIEW_T
    {
        return;
    }

    m_treeDeviceList.SelectItem(hSelect);

    int iType = int(m_treeDeviceList.GetItemData(hSelect) / 1000);

    switch (iType)
    {
    case CHANNELTYPE:
        m_iCurChanIndex = int(m_treeDeviceList.GetItemData(hSelect) % 1000);
        m_hCurChanItem = hSelect;

        hParent = m_treeDeviceList.GetParentItem(hSelect);
        if (hParent != NULL)
        {
            if (DEVICETYPE == m_treeDeviceList.GetItemData(hParent)/1000)
            {
               // m_iCurDeviceIndex = m_treeDeviceList.GetItemData(hParent)%1000;
                SetDeviceIndex(m_treeDeviceList.GetItemData(hParent) % 1000);
                m_hCurDeviceItem = hParent;
            }
        }
        if (!pMenu.LoadMenu(IDR_MENU_CHANNEL))
        {
            return;
        }

        break;
    case TREE_ALL_T:
        if (!pMenu.LoadMenu(IDR_MENU_EHOME50_AUTH))
        {
            return;
        }
        break;
    case DEVICETYPE:
        {
            int iDevId = m_treeDeviceList.GetItemData(hSelect) % 1000;
            if (iDevId < 0 || iDevId >= MAX_DEVICES)
            {
                return;
            }
            if (g_struDeviceInfo[iDevId].dwVersion != 5)
            {
                return;
            }
            if (!pMenu.LoadMenu(IDR_MENU_EHOME50_AUTH_DEV))
            {
                return;
            }
            //m_iCurDeviceIndex = iDevId;
            SetDeviceIndex(iDevId);
        }
        break;
    default:
        return;
    }

    //display menu
    GetCursorPos(&pt);
    pMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,pt.x ,pt.y,this);    
    *pResult = 0;
}

void CEHomeDemoDlg::CyclePreview()
{
    if (IsPlaying())
    {
        StopPlayAll();
    }
    else
    {
        int i = 0;
        int j = 0;
        m_iCurWndIndex = 0;
        for (i=0; i<m_iCurWndNum; i++)
        {
            while (j < MAX_DEVICES)
            {
                if (g_struDeviceInfo[j].iDeviceIndex != -1 )
                {
                  //  m_iCurDeviceIndex = j;
                  //  m_iCurChanIndex = 0;
                    g_struDeviceInfo[j/*g_pMainDlg->m_iCurDeviceIndex*/].struChanInfo[0/*g_pMainDlg->m_iCurChanIndex*/].iPlayWndIndex = m_iCurWndIndex;
                    PlayChan(g_struDeviceInfo[j].iDeviceIndex, 0, 0);
                    j++;
                    break;
                }
                j++;
            }
        }
    }
}

// void CEHomeDemoDlg::OnTimer(UINT_PTR nIDEvent)
// {
//     // TODO: Add your message handler code here and/or call default
//     char szLan[128] = {0};
//     switch (nIDEvent)
//     {
//     case CYCLE_PREVIEW_TIMER:
//         CyclePreview();
//         break;
//     default:
//         break;
//     }
// 
//     CDialog::OnTimer(nIDEvent);
// }

void CEHomeDemoDlg::OnBnClickedBtnCirclePreview()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_bCyclePreview)
    {
        KillTimer(CYCLE_PREVIEW_TIMER);
        StopPlayAll();
        m_bCyclePreview = FALSE;
    }
    else
    {
        m_bCyclePreview = TRUE;
        CyclePreview();
        g_pCycleTimer = SetTimer(CYCLE_PREVIEW_TIMER, 5*1000,NULL);
    }
    ChangePlayBtnState();
}

void CEHomeDemoDlg::StopPlayAll(void)
{
    int i = 0;
    for (i = 0; i < MAX_WIN_NUM; i ++)
    {
        if (!m_pDlgPreview[i].m_bPlay)
        {
            continue;
        }
        m_pDlgPreview[i].StopPlay();
    }
}

void CEHomeDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch (nIDEvent)
    {
    case CYCLE_PREVIEW_TIMER:
        CyclePreview();
        break;
    default:
        break;
    }
    CDialog::OnTimer(nIDEvent);
}

void CEHomeDemoDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEHomeDemoDlg::OnBnClickedBtnClosePreview()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
//     NET_ESTREAM_StopListenPreview(m_lPreviewListen1);
//     NET_ESTREAM_StopListenPreview(m_lPreviewListen2);
// 
    if (!NET_ESTREAM_StopPreview(m_iPreviewHandle))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ESTREAM_StopPreview failed");
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 2, "NET_ESTREAM_StopPreview successful");
    }
}

BOOL CEHomeDemoDlg::PopSessionByHandle(LONG handle, LONG *pSession)
{
    HandleSessiuonMapIter iter = m_mapHandleSession.find(handle);
    if (iter != m_mapHandleSession.end())
    {
        if (pSession != NULL)
        {
            *pSession = iter->second;
        }
        
        m_mapHandleSession.erase(iter);
        return TRUE;
    }
    return FALSE;
}

BOOL CEHomeDemoDlg::RegisterDevIDMsg(DEVIDCHANGECB fnCB, void *pUserData)
{
    m_fnDevCB = fnCB;
    m_pDevUserData = pUserData;

    return TRUE;
}

void CEHomeDemoDlg::DeviceIDChanged()
{
    if (NULL != m_fnDevCB)
    {
        m_fnDevCB(m_iCurDeviceIndex, m_pDevUserData);
    }
}

void CALLBACK CEHomeDemoDlg::EcmsCallback(NET_EHOME_CMSCB_DATA *pData, void* pUser)
{
    if (pData == NULL)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "EcmsCallback, pData == NULL");
        return;
    }

    if (pData->dwType == ECMS_CB_TYPE_START_GET_REALSTREAM)
    {
        CDlgOutput* pPreview = (CDlgOutput *)pUser;

        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_START_GET_REALSTREAM, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));

            pPreview->m_iSessionID = -1;

            return;
        }
        else
        {
            NET_EHOME_PREVIEWINFO_OUT *pPreviewOut = (NET_EHOME_PREVIEWINFO_OUT *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_START_GET_REALSTREAM, SUCC, AsyncHandle[%d], Error[%d], lSessionID[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pPreviewOut->lSessionID, pPreviewOut->lHandle);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, pPreviewOut->lSessionID));

            DWORD dwVersion = g_struDeviceInfo[g_pMainDlg->m_iCurDeviceIndex].dwVersion;

            pPreview->m_iSessionID = pPreviewOut->lSessionID;

            if (dwVersion >= 4)
            {
                NET_EHOME_PUSHSTREAM_IN struPushIn = { 0 };
                NET_EHOME_PUSHSTREAM_OUT struPushOut = { 0 };
                struPushOut.dwSize = sizeof(struPushOut);
                struPushIn.dwSize = sizeof(struPushIn);
                struPushIn.lSessionID = pPreviewOut->lSessionID;
                if (!NET_ECMS_StartPushRealStream(pData->lUserID, &struPushIn, &struPushOut))
                {
                    g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "Async call NET_ECMS_StartPushRealStream Failed Error[%d]",
                        NET_ECMS_GetLastError());
                }
                else
                {
                    g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "Async call NET_ECMS_StartPushRealStream SUCC");
                }
            }

            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_START_PUSH_REALSTREAM)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_START_PUSH_REALSTREAM, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            //g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));
            return;
        }
        else
        {
            NET_EHOME_PUSHSTREAM_OUT *pPushStreamOut = (NET_EHOME_PUSHSTREAM_OUT *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_START_PUSH_REALSTREAM, SUCC, AsyncHandle[%d], Error[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pPushStreamOut->lHandle);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, 0));
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_STOP_GET_REALSTREAM_EX)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_STOP_GET_REALSTREAM_EX, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));
            return;
        }
        else
        {
            NET_EHOME_STOPSTREAM_PARAM *pStopStreamParam = (NET_EHOME_STOPSTREAM_PARAM *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_STOP_GET_REALSTREAM_EX, SUCC, AsyncHandle[%d], Error[%d], lSessionID[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pStopStreamParam->lSessionID, pStopStreamParam->lHandle);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, pStopStreamParam->lSessionID));
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_START_PLAYBACK)
    {
        CDlgPlayBack* pPlayback = (CDlgPlayBack *)pUser;
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_START_PLAYBACK, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            //g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));
            return;
        }
        else
        {
            NET_EHOME_PLAYBACK_INFO_OUT *pPlaybackInfoOut = (NET_EHOME_PLAYBACK_INFO_OUT *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_START_PLAYBACK, SUCC, AsyncHandle[%d], Error[%d], lSessionID[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pPlaybackInfoOut->lSessionID, pPlaybackInfoOut->lHandle);
            //g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, pPlaybackInfoOut->lSessionID));
            NET_EHOME_PUSHPLAYBACK_IN struPushBackIn = { 0 };
            struPushBackIn.dwSize = sizeof(NET_EHOME_PUSHPLAYBACK_IN);
            struPushBackIn.lSessionID = pPlaybackInfoOut->lSessionID;
            pPlayback->m_dlgRecordFile.m_iSessionID = pPlaybackInfoOut->lSessionID;

            NET_EHOME_PUSHPLAYBACK_OUT struPushBackOut = { 0 };
            struPushBackOut.dwSize = sizeof(struPushBackOut);
            if (!NET_ECMS_StartPushPlayBack(pData->lUserID, &struPushBackIn, &struPushBackOut))
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "Async NET_ECMS_StartPushPlayBack, Failed, Error[%d]",
                    NET_ECMS_GetLastError());
            }
            else
            {
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "Async NET_ECMS_StartPushPlayBack, SUCC");
            }
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_START_PUSH_PLAYBACK)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_START_PUSH_PLAYBACK, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));
            return;
        }
        else
        {
            NET_EHOME_PUSHPLAYBACK_OUT *pPushPlaybackOut = (NET_EHOME_PUSHPLAYBACK_OUT *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_START_PUSH_PLAYBACK, SUCC, AsyncHandle[%d], Error[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pPushPlaybackOut->lHandle);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_STOP_PLAYBACK_EX)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_STOP_PLAYBACK_EX, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));
            return;
        }
        else
        {
            NET_EHOME_STOPPLAYBACK_PARAM *pStopPlaybackParam = (NET_EHOME_STOPPLAYBACK_PARAM *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_STOP_PLAYBACK_EX, SUCC, AsyncHandle[%d], Error[%d], lSessionID[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pStopPlaybackParam->lSessionID, pStopPlaybackParam->lHandle);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, pStopPlaybackParam->lSessionID));
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_START_VOICETALK_STM)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_START_VOICETALK_STM, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            return;
        }
        else
        {
            NET_EHOME_VOICE_TALK_OUT *pVoiceTalkOut = (NET_EHOME_VOICE_TALK_OUT *)pData->pOutBuffer;

            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_START_VOICETALK_STM, SUCC, AsyncHandle[%d], Error[%d], lSessionID[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pVoiceTalkOut->lSessionID, pVoiceTalkOut->lHandle);

            CDlgAudioTalk* pAudioTalk = (CDlgAudioTalk *)pUser;
            ::PostMessage(pAudioTalk->m_hWnd, WM_START_VOICE_TALK_OK, NULL, (LPARAM)pVoiceTalkOut->lSessionID);

            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_START_PUSH_VOICE_STREAM)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_START_PUSH_VOICE_STREAM, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            return;
        }
        else
        {
            NET_EHOME_PUSHVOICE_OUT *pPushVoiceOut = (NET_EHOME_PUSHVOICE_OUT *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_START_PUSH_VOICE_STREAM, SUCC, AsyncHandle[%d], Error[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pPushVoiceOut->lHandle);
            CDlgAudioTalk* pAudioTalk = (CDlgAudioTalk *)pUser;
            ::PostMessage(pAudioTalk->m_hWnd, WM_START_PUSH_VOICE_TALK_OK, NULL, NULL);
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_STOP_VOICE_TALK_STM_EX)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_STOP_VOICE_TALK_STM_EX, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            return;
        }
        else
        {
            NET_EHOME_STOPVOICETALK_STM_PARAM *pStopVoiceTalkStmParam = (NET_EHOME_STOPVOICETALK_STM_PARAM *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_STOP_VOICE_TALK_STM_EX, SUCC, AsyncHandle[%d], Error[%d], lSessionID[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pStopVoiceTalkStmParam->lSessionID, pStopVoiceTalkStmParam->lHandle);
            CDlgAudioTalk* pAudioTalk = (CDlgAudioTalk *)pUser;
            ::PostMessage(pAudioTalk->m_hWnd, WM_STOP_VOICE_TALK_OK, NULL, NULL);
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_PAUSE_PLAYBACK)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_PAUSE_PLAYBACK, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));
            return;
        }
        else
        {
            NET_EHOME_STOPPLAYBACK_PARAM *pStopPlaybackParam = (NET_EHOME_STOPPLAYBACK_PARAM *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_PAUSE_PLAYBACK, SUCC, AsyncHandle[%d], Error[%d], lSessionID[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pStopPlaybackParam->lSessionID, pStopPlaybackParam->lHandle);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, pStopPlaybackParam->lSessionID));
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_RESTART_PLAYBACK)
    {
        if (!pData->bSucc)
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async ECMS_CB_TYPE_RESTART_PLAYBACK, Failed, AsyncHandle[%d], Error[%d]",
                pData->dwHandle, pData->dwErrorNo);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, -1));
            return;
        }
        else
        {
            NET_EHOME_STOPPLAYBACK_PARAM *pStopPlaybackParam = (NET_EHOME_STOPPLAYBACK_PARAM *)pData->pOutBuffer;
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "Async ECMS_CB_TYPE_RESTART_PLAYBACK, SUCC, AsyncHandle[%d], Error[%d], lSessionID[%d], lHandle[%d]",
                pData->dwHandle, pData->dwErrorNo, pStopPlaybackParam->lSessionID, pStopPlaybackParam->lHandle);
            g_pMainDlg->m_mapHandleSession.insert(std::make_pair(pData->dwHandle, pStopPlaybackParam->lSessionID));
            return;
        }
    }
    else if (pData->dwType == ECMS_CB_TYPE_ISAPI_PASSTHROUGH)
    {
        NET_EHOME_PTXML_PARAM *pParam = (NET_EHOME_PTXML_PARAM *)pData->pOutBuffer;
        return;
    }
    else if (pData->dwType == ECMS_CB_TYPE_START_V2_VOICETALK)
    {
        NET_EHOME_VOICETALK_PARA *pParam = (NET_EHOME_VOICETALK_PARA *)pData->pOutBuffer;
        return;
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "Async Invalid Type, Failed, dwType[%d]", pData->dwType);
    }

    return;
}

void CEHomeDemoDlg::InitLib()
{
    DWORD dwError = 0;
    if (g_bSS_Enable)
    {
        if (strlen(m_szDBKey) > 0)
        {
            NET_EHOME_SS_INIT_PARAM struSSInitParam = { 0 };
            strcpy(struSSInitParam.szKey, m_szDBKey);
            if (!NET_ESS_Init_V11(&struSSInitParam))
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ESS_Init_V11 Failed,, Error=[%d]", NET_ESS_GetLastError());
            }
        }
        else
        {
            if (!NET_ESS_Init())
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ESS_Init Failed,, Error=[%d]", NET_ESS_GetLastError());
            }
        }

        NET_ESS_SetLogToFile(3, "C:/EHomeSdkLog/", TRUE);
        ASSERT(m_dlgLocalCfg != NULL);
        BOOL bHttps = m_dlgLocalCfg->m_bHttps;
        if (bHttps)
        {
            NET_EHOME_SS_LISTEN_HTTPS_PARAM struHttpsParam = { 0 };
            struHttpsParam.byHttps = 1;
            struHttpsParam.byCertificateFileType = 0;
            struHttpsParam.byPrivateKeyFileType = 0;
            strncpy(struHttpsParam.szUserCertificateFile
                , "C:\\Certificate\\server.pem"
                , MAX_PATH - 1);
            strncpy(struHttpsParam.szUserPrivateKeyFile
                , "C:\\Certificate\\serverkey.pem"
                , MAX_PATH - 1);
            struHttpsParam.dwSSLVersion = 0;
            NET_ESS_SetListenHttpsParam(&struHttpsParam);
        }

        // 端口映射时，设置映射地址信息
        NET_EHOME_IPADDRESS struPublicAddress = { 0 };
        GetAddressByType(PSSAdd, 0, struPublicAddress.szIP, sizeof(struPublicAddress.szIP), (char *)&(struPublicAddress.wPort), sizeof(struPublicAddress.wPort));
        if (struPublicAddress.wPort == 0)
        {
            struPublicAddress.wPort = m_struServInfo.struPictureSever.wPort;
        }
        //memcpy(struPublicAddress.szIP, m_struServInfo.struPictureSever.szIP, sizeof(m_struServInfo.struPictureSever.szIP));
        //NET_ESS_SetSDKInitCfg(NET_EHOME_SS_INIT_CFG_PUBLIC_IP_PORT, &struPublicAddress);

        //注释掉图片服务器在demo刚运行即开启的代码，使用mfc界面配置图片服务器开启监听
        /*NET_EHOME_SS_LISTEN_PARAM struSSListenParam = { 0 };
        memcpy(struSSListenParam.struAddress.szIP, m_sLocalIP, sizeof(m_sLocalIP));
        struSSListenParam.struAddress.wPort = m_struServInfo.struPictureSever.wPort;
        memcpy(struSSListenParam.szKMS_UserName, "test", strlen("test"));
        memcpy(struSSListenParam.szKMS_Password, "12345", strlen("12345"));
        memcpy(struSSListenParam.szAccessKey, "test", strlen("test"));
        memcpy(struSSListenParam.szSecretKey, "12345", strlen("12345"));
        struSSListenParam.fnSSMsgCb = SS_Message_Callback;
        struSSListenParam.fnSStorageCb = SS_Storage_Callback;
        struSSListenParam.pUserData = this;
        m_lSSHandle = NET_ESS_StartListen(&struSSListenParam);
        if (-1 == m_lSSHandle)
        {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_ESS_StartListen Failed, port：%d", struSSListenParam.struAddress.wPort);
        }
        else
        {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_ESS_StartListen succ port：%d", struSSListenParam.struAddress.wPort);
        }*/

        //NET_EHOME_SS_STORAGE_URI strutmp = { NET_EHOME_SS_TYPE_CLOUD, 0, 0, 0 };
        //memcpy(strutmp.szFilename, "test+", strlen("test+"));
        //NET_ESS_GenerateStorageUri(&strutmp);
        //g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_ESS_GenerateStorageUri geneturi：%s", strutmp.szUri);


        //图片上传到图片服务器示例代码
        NET_EHOME_SS_CLIENT_PARAM struClientParam;
        memset(&struClientParam, 0, sizeof(struClientParam));
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_TOMCAT;
        memcpy(struClientParam.struAddress.szIP, m_sLocalIP, sizeof(m_sLocalIP));
        struClientParam.struAddress.wPort = 8080;
        struClientParam.byHttps = bHttps;
        LONG lSSClientHandle = -1;
        char szUrl[MAX_URL_LEN_SS] = { 0 };

        //Tomcat图片上传示例代码
        lSSClientHandle = NET_ESS_CreateClient(&struClientParam);
        if (lSSClientHandle >= 0)
        {
            NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);
            NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_FILE_PATH_PARAM_NAME, "C:\\Users\\Public\\Pictures\\Sample Pictures\\菊花.jpg");

            char szUrl[MAX_URL_LEN_SS] = { 0 };
            NET_ESS_ClientDoUpload(lSSClientHandle, szUrl, MAX_URL_LEN_SS - 1);

            NET_ESS_DestroyClient(lSSClientHandle);
        }

        ////VRB图片上传示例代码
        //struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_VRB;
        //lSSClientHandle = NET_ESS_CreateClient(&struClientParam);
        //if (lSSClientHandle >= 0)
        //{
        //    NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);

        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_FILE_PATH_PARAM_NAME, "C:\\Picture\\1.jpg");
        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_VRB_FILENAME_CODE, "filename=1.jpg&code=");

        //    NET_ESS_ClientDoUpload(lSSClientHandle, szUrl, MAX_URL_LEN_SS - 1);

        //    NET_ESS_DestroyClient(lSSClientHandle);
        //}

        ////Tomcat VRB图片下载示例代码
        //struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_VRB;
        //lSSClientHandle = NET_ESS_CreateClient(&struClientParam);
        //if (lSSClientHandle >= 0)
        //{
        //    NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);

        //    byte* bFileContent = NULL;
        //    DWORD dwFileLen = 0;
        //    if (NET_ESS_ClientDoDownload(lSSClientHandle, szUrl, (void**)&bFileContent, dwFileLen))
        //    {
        //        FILE* pFile = fopen("C://Picture//vrb.jpg", "wb+");
        //        if (pFile != NULL)
        //        {
        //            fwrite(bFileContent, 1, dwFileLen, pFile);
        //            fclose(pFile);
        //        }
        //    }

        //    NET_ESS_DestroyClient(lSSClientHandle);
        //}

        ////KMS图片上传示例代码
        //struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_KMS;
        //lSSClientHandle = NET_ESS_CreateClient(&struClientParam);
        //if (lSSClientHandle >= 0)
        //{
        //    NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);

        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_FILE_PATH_PARAM_NAME, "C:\\Picture\\1.jpg");
        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_KMS_USER_NAME, "test");
        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_KMS_PASSWIRD, "12345");

        //    NET_ESS_ClientDoUpload(lSSClientHandle, szUrl, MAX_URL_LEN_SS - 1);

        //    DWORD dwErr = NET_ESS_GetLastError();

        //    NET_ESS_DestroyClient(lSSClientHandle);
        //}

        ////KMS图片下载示例代码
        //lSSClientHandle = NET_ESS_CreateClient(&struClientParam);
        //if (lSSClientHandle >= 0)
        //{
        //    NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);

        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_KMS_USER_NAME, "test");
        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_KMS_PASSWIRD, "12345");

        //    byte* bFileContent = NULL;
        //    DWORD dwFileLen = 0;
        //    if (NET_ESS_ClientDoDownload(lSSClientHandle, szUrl, (void**)&bFileContent, dwFileLen))
        //    {
        //        FILE* pFile = fopen("C://Picture//kms.jpg", "wb+");
        //        if (pFile != NULL)
        //        {
        //            fwrite(bFileContent, 1, dwFileLen, pFile);
        //            fclose(pFile);
        //        }
        //    }

        //    DWORD dwErr = NET_ESS_GetLastError();

        //    NET_ESS_DestroyClient(lSSClientHandle);
        //}

        ////武汉云存储协议图片上传示例代码
        //struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_CLOUD;
        //lSSClientHandle = NET_ESS_CreateClient(&struClientParam);
        //if (lSSClientHandle >= 0)
        //{
        //NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);

        //NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_FILE_PATH_PARAM_NAME, "C:\\Picture\\1.jpg");
        //NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CLOUD_AK_NAME, "test");
        //NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CLOUD_SK_NAME, "12345");

        //NET_ESS_ClientDoUpload(lSSClientHandle, szUrl, MAX_URL_LEN_SS - 1);

        //DWORD dwErr = NET_ESS_GetLastError();

        //byte* bFileContent = NULL;

        //NET_ESS_DestroyClient(lSSClientHandle);
        //}

        ////EHome5.0存储协议图片下载示例代码
        //struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_CLOUD;
        //lSSClientHandle = NET_ESS_CreateClient(&struClientParam);
        //if (lSSClientHandle >= 0)
        //{
        //    NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);

        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CLOUD_AK_NAME, "test");
        //    NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CLOUD_SK_NAME, "12345");
    }

    NET_EHOME_LOCAL_ACCESS_SECURITY struAccessSecure = { 0 };
    BOOL bRet = false;

    {
        bRet = NET_EALARM_Init();
        bRet = NET_EALARM_SetLogToFile(m_struLogInfo.dwLogLevel, (const char *)m_struLogInfo.byLogPath, m_struLogInfo.bAutoDel);

        struAccessSecure.dwSize = sizeof(struAccessSecure);
        struAccessSecure.byAccessSecurity = (BYTE)m_byAlarmSecureAccessType;
        //struAccessSecure.byAccessSecurity = (BYTE)0;
        if (!NET_EALARM_SetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_EALARM_SetSDKLocalCfg ACTIVE_ACCESS_SECURITY Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_EALARM_SetSDKLocalCfg ACTIVE_ACCESS_SECURITY Success");
        }
        //本地通用配置
        NET_EHOME_LOCAL_GENERAL_CFG struGeneralCfg = { 0 };

        switch (m_byAlarmPictureSeparate)
        {
        case 0:
        case 1:
            struGeneralCfg.byAlarmPictureSeparate = m_byAlarmPictureSeparate;
        default:
            break;
        }
        if (!NET_EALARM_SetSDKLocalCfg(LOCAL_CFG_TYPE_GENERAL, &struGeneralCfg))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_EALARM_SetSDKLocalCfg struGeneralCfg Failed");
        }
        else
        {
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_EALARM_SetSDKLocalCfg struGeneralCfg Success");
        }
//         NET_EHOME_ALARM_LISTEN_PARAM struAlarmListenParam = { 0 };
//         memcpy(struAlarmListenParam.struAddress.szIP, m_sLocalIP, sizeof(m_sLocalIP));

        /*struAlarmListenParam.struAddress.wPort = 7332;
        struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
        struAlarmListenParam.pUserData = this;
        struAlarmListenParam.byProtocolType = 1;
        struAlarmListenParam.byUseThreadPool = 0;

        m_lUdpAlarmHandle = NET_EALARM_StartListen(&struAlarmListenParam);*/

        //struAlarmListenParam.struAddress.wPort = 7331;
        //struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
        //struAlarmListenParam.pUserData = this;
        //struAlarmListenParam.byProtocolType = 0;
        //struAlarmListenParam.byUseCmsPort = 0; //是否复用CMS端口：0 - 不复用，非0 - 复用
        //struAlarmListenParam.byUseThreadPool = 0;

        //struAlarmListenParam.byProtocolType = 1;
        //struAlarmListenParam.struAddress.wPort = m_struServInfo.struUDPAlarmSever.wPort;

        //struAlarmListenParam.dwKeepAliveSec = m_struServInfo.dwAlarmKeepAliveSec;
        //struAlarmListenParam.dwTimeOutCount = m_struServInfo.dwAlarmTimeOutCount;


        //注册监听 中dwAlarmServerType 0- 只支持UDP协议上报，1- 支持UDP、TCP两种协议上报 ，2 - 采用MQTT协议方式上报
        if (m_struServInfo.dwAlarmServerType == 0)
        {
            //只开UDP
            NET_EHOME_ALARM_LISTEN_PARAM struUdpAlarmListenParam = { 0 };
            memcpy(struUdpAlarmListenParam.struAddress.szIP, m_sLocalIP, sizeof(m_sLocalIP));
            struUdpAlarmListenParam.byProtocolType = 1;
            struUdpAlarmListenParam.struAddress.wPort = m_struServInfo.struUDPAlarmSever.wPort;
            struUdpAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
            struUdpAlarmListenParam.pUserData = this;
            struUdpAlarmListenParam.dwKeepAliveSec = m_struServInfo.dwAlarmKeepAliveSec;
            struUdpAlarmListenParam.dwTimeOutCount = m_struServInfo.dwAlarmTimeOutCount;
            struUdpAlarmListenParam.byUseCmsPort = 0; //是否复用CMS端口：0 - 不复用，非0 - 复用
            struUdpAlarmListenParam.byUseThreadPool = 0;
            m_lUdpAlarmHandle = NET_EALARM_StartListen(&struUdpAlarmListenParam);
            if (m_lUdpAlarmHandle >= 0)
            {
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 0, "NET_EALARM_StartListen init udp Success");
            }
            else
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 0, "NET_EALARM_StartListen init udp failed");
            }
        }
        else if (m_struServInfo.dwAlarmServerType == 1)
        {
            NET_EHOME_ALARM_LISTEN_PARAM struTcpAlarmListenParam = { 0 };
            memcpy(struTcpAlarmListenParam.struAddress.szIP, m_sLocalIP, sizeof(m_sLocalIP));
            struTcpAlarmListenParam.byProtocolType = 0;
            struTcpAlarmListenParam.struAddress.wPort = m_struServInfo.struTCPAlarmSever.wPort;
            struTcpAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
            struTcpAlarmListenParam.pUserData = this;
            struTcpAlarmListenParam.dwKeepAliveSec = m_struServInfo.dwAlarmKeepAliveSec;
            struTcpAlarmListenParam.dwTimeOutCount = m_struServInfo.dwAlarmTimeOutCount;
            struTcpAlarmListenParam.byUseCmsPort = 0; //是否复用CMS端口：0 - 不复用，非0 - 复用
            struTcpAlarmListenParam.byUseThreadPool = 0;
            NET_EHOME_ALARM_LISTEN_PARAM struUdpAlarmListenParam = { 0 };
            memcpy(struUdpAlarmListenParam.struAddress.szIP, m_sLocalIP, sizeof(m_sLocalIP));
            struUdpAlarmListenParam.byProtocolType = 1;
            struUdpAlarmListenParam.struAddress.wPort = m_struServInfo.struUDPAlarmSever.wPort;
            struUdpAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
            struUdpAlarmListenParam.pUserData = this;
            struUdpAlarmListenParam.dwKeepAliveSec = m_struServInfo.dwAlarmKeepAliveSec;
            struUdpAlarmListenParam.dwTimeOutCount = m_struServInfo.dwAlarmTimeOutCount;
            struUdpAlarmListenParam.byUseCmsPort = 0; //是否复用CMS端口：0 - 不复用，非0 - 复用
            struUdpAlarmListenParam.byUseThreadPool = 0;


            m_lTcpAlarmHandle = NET_EALARM_StartListen(&struTcpAlarmListenParam);
            if (m_lTcpAlarmHandle >= 0)
            {
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 0, "NET_EALARM_StartListen init tcp Success");
            }
            else
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 0, "NET_EALARM_StartListen init tcp failed");
            }
            m_lUdpAlarmHandle = NET_EALARM_StartListen(&struUdpAlarmListenParam);
            if (m_lUdpAlarmHandle >= 0)
            {
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 0, "NET_EALARM_StartListen init udp Success");
            }
            else
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 0, "NET_EALARM_StartListen init udp failed");
            }

        }
        else if (m_struServInfo.dwAlarmServerType == 2)         //开启EHome5.0报警监听
        {
            NET_EHOME_ALARM_LISTEN_PARAM struAlarmListenParam = { 0 };
            //m_lUdpAlarmHandle = NET_EALARM_StartListen(&struAlarmListenParam);    //UDP报警监听
            memcpy(struAlarmListenParam.struAddress.szIP, m_sLocalIP, sizeof(m_sLocalIP));

            //struAlarmListenParam.byUseCmsPort = m_byUseCmsPort;
            struAlarmListenParam.byUseCmsPort = 0; //是否复用CMS端口：0 - 不复用，非0 - 复用
            struAlarmListenParam.byUseThreadPool = 0;
            struAlarmListenParam.byProtocolType = 2;
            struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
            struAlarmListenParam.pUserData = this;
            struAlarmListenParam.struAddress.wPort = m_wAlarmServerMqttPort;
            struAlarmListenParam.dwKeepAliveSec = m_struServInfo.dwAlarmKeepAliveSec;
            struAlarmListenParam.dwTimeOutCount = m_struServInfo.dwAlarmTimeOutCount;

            if (struAlarmListenParam.struAddress.wPort == m_struServInfo.struTCPAlarmSever.wPort)
            {
                char szLan[1024] = { 0 };
                g_StringLanType(szLan, "2.0/4.0 TCP报警端口与5.0报警端口冲突!", "Conflict between 2.0/4.0 TCP alarm port and 5.0 alarm port !");
                AddLog(-1, OPERATION_FAIL_T, 0, szLan);
                AfxMessageBox("2.0/4.0 TCP报警端口与5.0报警端口冲突!");
                return;
            }
            m_lEhome50AlarmHandle = NET_EALARM_StartListen(&struAlarmListenParam);
            if (m_lEhome50AlarmHandle >= 0)
            {
                g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 0, "NET_EALARM_StartListen mqtt init Success");
            }
            else
            {
                g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 0, "NET_EALARM_StartListen mqtt init failed");
            }

        }
        //struAlarmListenParam.byUseCmsPort = 0;
        //struAlarmListenParam.byProtocolType = 2;
        //struAlarmListenParam.struAddress.wPort = m_struServInfo.struTCPAlarmSever.wPort;
        //m_lEhome50AlarmHandle = NET_EALARM_StartListen(&struAlarmListenParam);

        //     memset(struAlarmListenParam.struAddress.szIP, 0, sizeof(struAlarmListenParam.struAddress.szIP));
        //     memcpy(struAlarmListenParam.struAddress.szIP, "127.0.0.1", strlen("127.0.0.1"));

        /*  struAlarmListenParam.struAddress.wPort = (WORD)7333;
        struAlarmListenParam.fnMsgCb = AlarmMsgCallBack;
        struAlarmListenParam.pUserData = this;
        struAlarmListenParam.byUseCmsPort = 1;
        struAlarmListenParam.byUseThreadPool = 0;
        LONG lCmsAlarm = NET_EALARM_StartListen(&struAlarmListenParam);*/

        //if (-1 == m_lUdpAlarmHandle)
        //{
        //    g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 3, "NET_EALARM_StartListen Failed, port：%d", struAlarmListenParam.struAddress.wPort);
        //}
        //else
        //{
        //    // m_lCmsAlarm = lCmsAlarm;
        //    g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 3, "NET_EALARM_StartListen succ port：%d", struAlarmListenParam.struAddress.wPort);
        //}

        NET_EALARM_SetLogToFile(m_struLogInfo.dwLogLevel, (const char *)m_struLogInfo.byLogPath, m_struLogInfo.bAutoDel);
        NET_EALARM_GetBuildVersion();
    }

//     if (!NET_ECMS_SetSDKInitCfg(NET_EHOME_CMS_INIT_CFG_LIBEAY_PATH, "E:\\Task2019\\EHome基线项目\\工程\\OpenSSLTest\\libeay32.dll"))
//     {
//         g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetSDKInitCfg, NET_EHOME_CMS_INIT_CFG_LIBEAY_PATH, Failed, Error[%d]", NET_ECMS_GetLastError());
//     }

//     if (!NET_ECMS_SetSDKInitCfg(NET_EHOME_CMS_INIT_CFG_SSLEAY_PATH, "E:\\Task2019\\EHome基线项目\\工程\\OpenSSLTest\\ssleay32.dll"))
//     {
//         g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetSDKInitCfg, NET_EHOME_CMS_INIT_CFG_SSLEAY_PATH, Failed, Error[%d]", NET_ECMS_GetLastError());
//     }

    //初始化库
    LONG lRet = -1;
    bRet = NET_ECMS_Init();
    CString csTemp = _T("");
    csTemp.Format(_T("Demo Version: 1.0.0.0"));
    GetDlgItem(IDC_STATIC_DEMO_VERSION)->SetWindowText(csTemp);
    DWORD version = NET_ECMS_GetBuildVersion();
    csTemp.Format(_T("ISUP Version: %d.%d.%d.%d"), GET_SUB_FOUR_VERSION(version));
    GetDlgItem(IDC_STATIC_SDKVER)->SetWindowText(csTemp);
    //     bRet = NET_ECMS_SetSDKLocalCfg(NET_EHOME_LOCAL_CFG_TYPE::COM_PATH, "E:\\Task2018\\EHomeSDK2.0.1\\common\\HCEHomeCMS\\win\\lib32");
    //     if (!bRet)
    //     {
    //         g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetSDKLocalCfg COM_PATH Failed");
    //     }

//     NET_EHOME_LOCAL_DEV_PINGREO struDevPingReo = { 0 };
//     struDevPingReo.dwSize = sizeof(struDevPingReo);
//     struDevPingReo.byEnablePingReoCallback = 1;
//     NET_ECMS_SetSDKLocalCfg(DEV_DAS_PINGREO_CALLBACK, &struDevPingReo);

    //使用负载均衡方式必须设置
    BOOL bSessionKeyReqMod = TRUE;
    NET_ECMS_SetSDKLocalCfg(SESSIONKEY_REQ_MOD, &bSessionKeyReqMod);

    if (m_bAsyncCalllback)
    {
        if (!NET_ECMS_SetCallback(NET_EHOME_REALSTREAM_CB, EcmsCallback, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_REALSTREAM_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_PLAYBACK_CB, EcmsCallback, g_pPlayBackDlg))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_PLAYBACK_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_VOICETALK_CB, EcmsCallback, g_pAudioTalkDlg))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_VOICETALK_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_PASSTHROUGH_CB, EcmsCallback, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_PASSTHROUGH_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_V2_VOICETALK_CB, EcmsCallback, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_V2_VOICETALK_CB, Failed");
        }
    }


    bRet = NET_ECMS_SetLogToFile(m_struLogInfo.dwLogLevel, (char *)m_struLogInfo.byLogPath, m_struLogInfo.bAutoDel);
    struAccessSecure.byAccessSecurity = (BYTE)m_byCmsSecureAccessType;
    if (!NET_ECMS_SetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetSDKLocalCfg ACTIVE_ACCESS_SECURITY Failed");
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_ECMS_SetSDKLocalCfg ACTIVE_ACCESS_SECURITY Success");
    }

    GetServerType(g_iServerType, m_strDasConfigFile.GetBuffer(), 128);

    NET_EHOME_AMS_ADDRESS struAmsAddr = { 0 };
    struAmsAddr.dwSize = sizeof(struAmsAddr);
    memcpy(struAmsAddr.struAddress.szIP, m_sLocalIP, sizeof(m_sLocalIP));
    struAmsAddr.struAddress.wPort = m_struServInfo.struUDPAlarmSever.wPort;
    struAmsAddr.byEnable = m_byUseCmsPort;
    //CMS开始接收
    if (!NET_ECMS_SetSDKLocalCfg(AMS_ADDRESS, &struAmsAddr))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "open cms-alarm Failed %d", m_nPort);
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "open cms-alarm  Succ %d", m_nPort);
    }

    NET_EHOME_SET_REREGISTER_MODE struSetReRegisterMode = { 0 };
    struSetReRegisterMode.dwSize = sizeof(struSetReRegisterMode);
    struSetReRegisterMode.dwReRegisterMode = 0;

    if (!NET_ECMS_SetSDKLocalCfg(SET_REREGISTER_MODE, &struSetReRegisterMode))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "Set Reregister Failed");
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "Set Reregister SUCC");
    }

    NET_EHOME_SEND_PARAM struSendParam = { 0 };
    struSendParam.dwSize = sizeof(struSendParam);
    struSendParam.bySendTimes = 3;
    //CMS开始接收
    if (!NET_ECMS_SetSDKLocalCfg(SEND_PARAM, &struSendParam))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetSDKLocalCfg SEND_PARAM failed");
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_ECMS_SetSDKLocalCfg SEND_PARAM succ");
    }

    NET_EHOME_REGISTER_LISTEN_MODE struParam = { 0 };
    struParam.dwSize = sizeof(struParam);
    struParam.dwRegisterListenMode = REGISTER_LISTEN_MODE_ALL;

    //CMS开始接收
    if (!NET_ECMS_SetSDKLocalCfg(REGISTER_LISTEN_MODE, &struParam))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetSDKLocalCfg register listen mode failed");
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_ECMS_SetSDKLocalCfg register listen mode succ");
    }
    
    //开启监听
    NET_EHOME_CMS_LISTEN_PARAM struCMSListenPara = { 0 };
    memcpy(struCMSListenPara.struAddress.szIP, m_sLocalIP, strlen(m_sLocalIP));
    struCMSListenPara.struAddress.wPort = (WORD)m_nPort;
    struCMSListenPara.fnCB = EHOME_REGISTER;
    struCMSListenPara.pUserData = this;
    struCMSListenPara.dwKeepAliveSec = m_struServInfo.dwKeepAliveSec;
    struCMSListenPara.dwTimeOutCount = m_struServInfo.dwTimeOutCount;
    lRet = NET_ECMS_StartListen(&struCMSListenPara);

    if (-1 == lRet)
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_StartListen Failed");
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_ECMS_StartListen Succ");
    }
    DWORD dwVersion = NET_ECMS_GetBuildVersion();
    DWORD dwV1 = (dwVersion >> 24);
    DWORD dwV2 = ((dwVersion & 0x00FF0000) >> 16);
    DWORD dwV3 = ((dwVersion & 0x0000FF00) >> 8);
    DWORD dwV4 = (dwVersion & 0x000000FF);

    g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ECMS_GetBuildVersion, %d.%d.%d.%d", dwV1, dwV2, dwV3, dwV4);
    
    NET_ESTREAM_Init();
    NET_ESTREAM_SetLogToFile(m_struLogInfo.dwLogLevel, (char *)m_struLogInfo.byLogPath, FALSE/*m_struLogInfo.bAutoDel*/);

    struAccessSecure.byAccessSecurity = (BYTE)m_byStreamSecureAccessType;
    if (!NET_ESTREAM_SetSDKLocalCfg(ACTIVE_ACCESS_SECURITY, &struAccessSecure))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 2, "NET_ESTREAM_SetSDKLocalCfg ACTIVE_ACCESS_SECURITY Failed");
    }
    else
    {
        g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 1, "NET_ESTREAM_SetSDKLocalCfg ACTIVE_ACCESS_SECURITY Success");
    }
    NET_ESTREAM_GetBuildVersion();

    ////SSL证书路径设置
    char  szUserCertificateFile[MAX_PATH] = { 0 };
    //strncpy(szUserCertificateFile, "C:\\Certificate\\server.pem", MAX_PATH - 1);
    char  szUserPrivateKeyFile[MAX_PATH] = { 0 };
    //strncpy(szUserPrivateKeyFile, "C:\\Certificate\\serverkey.pem", MAX_PATH - 1);

    char *buf = getcwd(NULL, 0);
    char pBufA[MAX_PATH] = { 0 };
    sprintf(pBufA, ".\\Cert\\server.pem", buf); /*%s\\Cert\\server.pem*/
    strncpy(szUserCertificateFile, pBufA, MAX_PATH - 1);

    char pBufB[MAX_PATH] = { 0 };
    sprintf(pBufB, ".\\Cert\\serverkey.pem", buf);/*%s\\Cert\\serverkey.pem*/
    strncpy(szUserPrivateKeyFile, pBufB, MAX_PATH - 1);

    if (!NET_ESTREAM_SetSDKInitCfg(NET_EHOME_ESTREAM_INIT_CFG_USERCERTIFICATE_PATH, szUserCertificateFile))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ESTREAM_SetSDKInitCfg USERCERTIFICATE_PATH Failed");
    }
    if (!NET_ESTREAM_SetSDKInitCfg(NET_EHOME_ESTREAM_INIT_CFG_USERPRIVATEKEY_PATH, szUserPrivateKeyFile))
    {
        g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ESTREAM_SetSDKInitCfg USERCERTIFICATE_PATH Failed");
    }

    if (buf != NULL)
    {
        delete[] buf;
    }
}


void CEHomeDemoDlg::GetLocalIP(char* pIPAddr) const
{
    if(NULL == pIPAddr)
    {
        return;
    }
    WSADATA wsaData;
    char name[255];//定义用于存放获得的主机名的变量
    char *ip;//定义IP地址变量
    PHOSTENT hostinfo;

    //调用MAKEWORD（）获得Winsock版本的正确值，用于加载Winsock库

    if (WSAStartup(MAKEWORD(2,0), &wsaData) == 0) {
        //现在是加载Winsock库，如果WSAStartup（）函数返回值为0，说明加载成功，程序可以继续
        if (gethostname(name, sizeof(name)) == 0) {
            //如果成功地将本地主机名存放入由name参数指定的缓冲区中
            if ((hostinfo = gethostbyname(name)) != NULL) {
                //这是获取主机名，如果获得主机名成功的话，将返回一个指针，指向hostinfo，hostinfo
                //为PHOSTENT型的变量，下面即将用到这个结构体
                ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
                //调用inet_ntoa（）函数，将hostinfo结构变量中的h_addr_list转化为标准的点分表示的IP
                //地址（如192.168.0.1）
                if ((strlen(ip)+1) > 128)
                {
                    WSACleanup(); 
                    return;
                }
                memcpy(pIPAddr, ip, strlen(ip)+1);                //printf("%s\n",ip);//输出IP地址
            }
        }
        WSACleanup(); //卸载Winsock库，并释放所有资源
    }
}




void CEHomeDemoDlg::OnMenuChannelInfo()
{
    // TODO: 在此添加命令处理程序代码
    int iDeviceIndex  = GetCurDeviceIndex();
    if (iDeviceIndex == -1)
    {
        return;
    }
    int iChanIndex = GetCurChanIndex();
    if (iChanIndex == -1)
    {
        char szLan[128] = {0};
        g_StringLanType(szLan, "通道号错误！", "Channel Error");
        AfxMessageBox(szLan);
        return;
    }
    CDlgChanInfo dlgChannelInfo;
    dlgChannelInfo.m_iDeviceIndex = iDeviceIndex;
    dlgChannelInfo.m_iChannelIndex = iChanIndex;
    dlgChannelInfo.DoModal();
}


BOOL ConvertSingleNodeData(void *pOutVale, CXmlBase &struXml, const char* pNodeName, BYTE byDataType, int iArrayLen /*= 0*/ )
{
    if (byDataType < NODE_TYPE_REVERSE)
    {
        if (struXml.FindElem(pNodeName))
        {
            //string to
            if (byDataType == NODE_STRING_TO_BOOL)    //bool类型
            {
                if (struXml.GetData().compare("true") == 0)
                {
                    *(BOOL*)pOutVale = TRUE;
                    return TRUE;
                }
                else if(struXml.GetData().compare("false") == 0)
                {
                    *(BOOL*)pOutVale = FALSE;
                    return TRUE;
                }            
            }
            else if (byDataType == NODE_STRING_TO_INT)   //int类型
            {
                if (struXml.GetData() != "")
                {
                    *(DWORD*)pOutVale = (DWORD)atoi(struXml.GetData().c_str());
                    return TRUE;
                }
            }
            else if (byDataType == NODE_STRING_TO_ARRAY)
            {
                string strTmp = struXml.GetData().c_str();
                if (strTmp != "")
                {
                    int nLen = (int)strTmp.length();
                    if (nLen > iArrayLen)
                    {
                        nLen = iArrayLen;
                    }
                    memcpy((char*)pOutVale, strTmp.c_str(), (DWORD)nLen);
                    return TRUE;
                }
            }
            else if (byDataType == NODE_STRING_TO_BYTE)
            {
                if (struXml.GetData() != "")
                {
                    *(BYTE*)pOutVale = (BYTE)atoi(struXml.GetData().c_str());
                    return TRUE;
                }
            }
            else if (byDataType == NODE_STRING_TO_WORD)
            {
                if (struXml.GetData() != "")
                {
                    *(WORD*)pOutVale = (WORD)atoi(struXml.GetData().c_str());
                    return TRUE;
                }
            }
            else if(byDataType == NODE_STRING_TO_FLOAT)
            {
                if (struXml.GetData() != "")
                {
                    *(float*)pOutVale = (float)atof(struXml.GetData().c_str());
                    return TRUE;
                }
            }
        }
        else
        {
            return FALSE;
        }

    }
    else if(byDataType > NODE_TYPE_REVERSE)
    {
        if (byDataType == NODE_BOOL_TO_STRING)
        {
            if (*(BOOL*)pOutVale)
            {
                if (struXml.AddNode(pNodeName, "true"))
                {
                    struXml.OutOfElem();
                    return TRUE;
                }
            }
            else if (!(*(BOOL*)pOutVale))
            {
                if(struXml.AddNode(pNodeName, "false"))
                {
                    struXml.OutOfElem();
                    return TRUE;
                }                
            }
        }
        else if (byDataType == NODE_INT_TO_STRING)
        {
            char szBuf[16] = {0};
            itoa(*(int*)pOutVale, szBuf, 10);
            if (struXml.AddNode(pNodeName, szBuf))
            {
                struXml.OutOfElem();
                return TRUE;
            }
        }
        else if (byDataType == NODE_ARRAY_TO_STRING)
        {
            string strValue = "";
            if (iArrayLen == 0)
            {
                strValue = (char*)pOutVale;
            }
            else 
            {
                int nValueLen = (int)strlen((char*)pOutVale);
                if (nValueLen < iArrayLen) //数组没有被填满,有结束符
                {
                    strValue = (char*)pOutVale;
                }
                else    
                {
                    char *lpTmp = NULL;
                    lpTmp = new char[iArrayLen + 1];
                    if (lpTmp == NULL)
                    {
                        return FALSE;
                    }

                    memset(lpTmp, 0, iArrayLen + 1);
                    memcpy(lpTmp, (char*)pOutVale, (DWORD)iArrayLen);//增加一个结束符
                    strValue = lpTmp;
                    delete[] lpTmp;    
                }
            }
            if (struXml.AddNode(pNodeName, strValue.c_str()))
            {
                struXml.OutOfElem();
                return TRUE;
            }
        }
        else if (byDataType == NODE_BYTE_TO_STRING)
        {
            char szBuf[16] = {0};
            int nTmp = (int)*(BYTE*)pOutVale; 
            itoa(nTmp, szBuf, 10);
            if (struXml.AddNode(pNodeName, szBuf))
            {
                struXml.OutOfElem();
                return TRUE;
            }
        }
        else if (byDataType == NODE_WORD_TO_STRING)
        {
            char szBuf[16] = {0};
            int nTmp = (int)*(WORD*)pOutVale; 
            itoa(nTmp, szBuf, 10);
            if (struXml.AddNode(pNodeName, szBuf))
            {
                struXml.OutOfElem();
                return TRUE;
            }
        }
    }

    return FALSE;
}

void CEHomeDemoDlg::OnMenuProxy()
{
    // TODO: 在此添加命令处理程序代码
    m_PassthroughProxyDlg.DoModal();
}


void CEHomeDemoDlg::OnGetGpsInfo()
{
    // TODO:  在此添加命令处理程序代码
    CDlgGpsInfo dlg;
    dlg.DoModal();
}

void CEHomeDemoDlg::OnBtnWirelessInfo()
{
    // TODO:  在此添加命令处理程序代码
    CDlgWirelessInfoPara dlg;
    dlg.DoModal();
}

void CEHomeDemoDlg::OnBnClickedBtnSalve()
{
    // TODO:  在此添加控件通知处理程序代码
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
    
    char szLan[128] = { 0 };
    CString csTemp = _T("");

    if (g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_lPlayHandle >= 0)
    {
        if (g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_bRecord == FALSE)
        {
            g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_bRecord = TRUE;
           // m_bRecord = TRUE;
            g_StringLanType(szLan, "停止录像", "Stop Record");
            csTemp = szLan;
            GetDlgItem(IDC_BTN_SALVE)->SetWindowText(csTemp);
            char szLan[256] = { 0 };
            CTime time = CTime::GetCurrentTime();
            g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_time = time;
            sprintf(szLan, "Save Record File To C:\\EhomeRecord\\%s_%4d-%02d-%02d_%02d_%02d_%02d_%d.ps", g_struDeviceInfo[g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_iDeviceIndex].byDeviceID, \
                time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), \
                m_iCurChanIndex + 1);
            g_pMainDlg->AddLog(0, OPERATION_SUCC_T, 2, szLan);
        }
        else
        {
            g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_bRecord = FALSE;
            //m_bRecord = FALSE;
            if (g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_fVideoFile != NULL)
            {
                fclose(g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_fVideoFile);
                g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_fVideoFile = NULL;
            }
            g_StringLanType(szLan, "开始录像", "Start Record");
            csTemp = szLan;
            GetDlgItem(IDC_BTN_SALVE)->SetWindowText(csTemp);
        }
    }
}


void CEHomeDemoDlg::OnMenuIsapiPt()
{
    // TODO:  在此添加命令处理程序代码
    CDlgISAPIPassthrough dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnMenuUpgrade()
{
    // TODO:  在此添加命令处理程序代码
    CDlgUpgrade dlg;
    dlg.DoModal();
}

void CEHomeDemoDlg::FullScreen(BOOL bFullScreen)
{
    int iShowStat = bFullScreen ? SW_HIDE : SW_SHOW;

    m_treeDeviceList.ShowWindow(iShowStat);

    GetDlgItem(IDC_STATIC_VERSION)->ShowWindow(iShowStat);
    GetDlgItem(IDC_LIST_ALL_LOG)->ShowWindow(iShowStat);
    GetDlgItem(IDC_RADIO_ALARM_INFO)->ShowWindow(iShowStat);
    GetDlgItem(IDC_STATIC)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_EXIT)->ShowWindow(iShowStat);
    GetDlgItem(IDC_TREE_DEV)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_EXIT)->ShowWindow(iShowStat);
    GetDlgItem(IDC_RADIO_LOCAL_LOG)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_PREVIEW)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_AUDIO_TALK)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_CFG)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_PLAYBACK)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_LOCAL_CFG)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_OTHER)->ShowWindow(iShowStat);
    GetDlgItem(IDC_STATIC_DEMO_VERSION)->ShowWindow(iShowStat);
//    GetDlgItem(IDC_STATIC_DEMO_BUILD)->ShowWindow(iShowStat);
    GetDlgItem(IDC_STATIC_WINNUM)->ShowWindow(iShowStat);
    GetDlgItem(IDC_COMBO_WNDNUM)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_PREVIEW_LISTEN)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_SALVE)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_CIRCLE_PREVIEW)->ShowWindow(iShowStat);
    GetDlgItem(IDC_EDIT_PREVIEW_HANDLE)->ShowWindow(iShowStat);
    GetDlgItem(IDC_BTN_CLOSE_PREVIEW)->ShowWindow(iShowStat);

    if (bFullScreen)
    {
        //for full screen while backplay
        g_pMainDlg->AddLog(g_pMainDlg->m_iCurDeviceIndex, OPERATION_SUCC_T, 0, "full screen display");
        m_dlgOutputCtrl->ShowWindow(SW_HIDE);
        GetWindowPlacement(&m_struOldWndpl);
        CRect rectWholeDlg;//entire client(including title bar)
        CRect rectClient;//client area(not including title bar)
        CRect rectFullScreen;
        GetWindowRect(&rectWholeDlg);
        RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rectClient);
        ClientToScreen(&rectClient);
        
        rectFullScreen.left = rectWholeDlg.left - rectClient.left;
        rectFullScreen.top = rectWholeDlg.top - rectClient.top;
        rectFullScreen.right = rectWholeDlg.right + g_iCurScreenWidth - rectClient.right;
        rectFullScreen.bottom = rectWholeDlg.bottom + g_iCurScreenHeight - rectClient.bottom;

        //enter into full screen;
        WINDOWPLACEMENT struWndpl;
        struWndpl.length = sizeof(WINDOWPLACEMENT);
        struWndpl.flags = 0;
        struWndpl.showCmd = SW_SHOWNORMAL;
        struWndpl.rcNormalPosition = rectFullScreen;
        SetWindowPlacement(&struWndpl);
        g_pMainDlg->ArrangeOutputs(1);
    }
    else
    {
        m_dlgOutputCtrl->ShowWindow(SW_SHOW);
        SetWindowPlacement(&m_struOldWndpl);
    }
    if (m_iMainType == PREVIEW_T)//dealing while preview
    {
        //refresh backgroud box
        if (bFullScreen)
        {
            GetDlgItem(IDC_STATIC_PREVIEWBG)->MoveWindow(0, 0, g_iCurScreenWidth, g_iCurScreenHeight, true);
            g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].MoveWindow(0, 0, g_iCurScreenWidth, g_iCurScreenHeight, TRUE);
            g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].ShowWindow(SW_SHOW);
        }
        else
        {
            GetDlgItem(IDC_STATIC_PREVIEWBG)->MoveWindow(&m_rectPreviewBG, true);
            g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].ShowWindow(SW_SHOW);
        }
    }
}

void CEHomeDemoDlg::OnBnClickedBtnSound()
{
    // TODO:  在此添加控件通知处理程序代码
    char szLan[128] = { 0 };
    if (m_bSound)
    {
        m_bSound = FALSE;
        g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_bSoundEnabled = FALSE;
        g_StringLanType(szLan, "打开声音", "Open Audio");
    }
    else
    {
        m_bSound = TRUE;
        g_pMainDlg->m_pDlgPreview[m_iCurWndIndex].m_bSoundEnabled = TRUE;
        g_StringLanType(szLan, "关闭声音", "Close Audio");
    }
    GetDlgItem(IDC_BTN_SOUND)->SetWindowText(szLan);

}


void CEHomeDemoDlg::OnEnableCfg()
{
    // TODO:  在此添加命令处理程序代码
    CDlgMotionConfig dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnRecodrCfg()
{
    // TODO:  在此添加命令处理程序代码
    CDlgRecordCfg dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnCapturePic()
{
    // TODO:  在此添加命令处理程序代码
    CDlgCaptureCfg dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnMontionArea()
{
    // TODO:  在此添加命令处理程序代码
    CDlgMotionArea dlg;
    dlg.DoModal();
}



void CEHomeDemoDlg::OnPrivateArea()
{
    // TODO:  在此添加命令处理程序代码
    CDlgPrivateArea dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnHideAlarm()
{
    // TODO:  在此添加命令处理程序代码
    CDlgHidArea dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnMenuHttpUpgrade()
{
    // TODO:  在此添加命令处理程序代码
    // TODO:  在此添加命令处理程序代码
    CDlgUpgradeHttp dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnMenuIsapiCfg()
{
    // TODO:  在此添加命令处理程序代码
    CDlgISAPIConfig dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnMenuAddEhome50DevAuth()
{
    // TODO:  在此添加命令处理程序代码
    CDlgEhome50Auth dlg;
    dlg.DoModal();
}

void CEHomeDemoDlg::OnMenuSetEhome50DevAuth()
{
    // TODO:  在此添加命令处理程序代码
    CDlgEhome50Auth dlg(m_iCurDeviceIndex);
    dlg.DoModal();
}

void CEHomeDemoDlg::OnMenuLogout()
{
    // TODO:  在此添加命令处理程序代码

    if (m_iCurDeviceIndex == -1)
    {
        return;
    }
    else
    {
        int lLoginID = g_struDeviceInfo[m_iCurDeviceIndex].lLoginID;
        if (NET_ECMS_ForceLogout(lLoginID))
        {
            AddLog(-1, OPERATION_SUCC_T, 1, "NET_ECMS_ForceLogout [%d]", lLoginID);
            DelDev(lLoginID);
        }
        else
        {
            AddLog(-1, OPERATION_FAIL_T, 1, "NET_ECMS_ForceLogout [%d]", lLoginID);
        }
    }
}


void CEHomeDemoDlg::OnBnClickedBtnIframe()
{
    // TODO:  在此添加控件通知处理程序代码
    int iDeviceIndex = -1;
    iDeviceIndex=g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex<0)
    {
        g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, 0, "there is no channel!!");
        return;
    }
    CDlgIFrame dlg;     
    dlg.m_iDeviceIndex = iDeviceIndex;
    dlg.DoModal();
    
}


void CEHomeDemoDlg::OnBnClickedBtnStreamEncrypt()
{
    // TODO:  在此添加控件通知处理程序代码
    CDlgStreamEncrypt dlg;
    dlg.DoModal();
}


void CEHomeDemoDlg::OnMenuStreamCheck()
{
    // TODO:  在此添加命令处理程序代码
    CDlgCheckStreamKey dlg;
    dlg.DoModal();
}
void CEHomeDemoDlg::OnMenuHttpAlarm()
{
    // TODO:  在此添加命令处理程序代码
   
    m_dlgHttpAlarm->ShowWindow(SW_SHOW);
}


void CEHomeDemoDlg::OnBnClickedChkAsync()
{
    // TODO:  在此添加控件通知处理程序代码
    if (((CButton*)GetDlgItem(IDC_CHK_ASYNC))->GetCheck() == 1)
    {
        m_bAsyncCalllback = TRUE;

        if (!NET_ECMS_SetCallback(NET_EHOME_REALSTREAM_CB, EcmsCallback, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_REALSTREAM_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_PLAYBACK_CB, EcmsCallback, g_pPlayBackDlg))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_PLAYBACK_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_VOICETALK_CB, EcmsCallback, g_pAudioTalkDlg))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_VOICETALK_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_PASSTHROUGH_CB, EcmsCallback, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_PASSTHROUGH_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_V2_VOICETALK_CB, EcmsCallback, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_V2_VOICETALK_CB, Failed");
        }
    }
    else
    {
        m_bAsyncCalllback = FALSE;
        if (!NET_ECMS_SetCallback(NET_EHOME_REALSTREAM_CB, NULL, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_REALSTREAM_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_PLAYBACK_CB, NULL, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_PLAYBACK_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_VOICETALK_CB, NULL, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_VOICETALK_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_PASSTHROUGH_CB, NULL, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_PASSTHROUGH_CB, Failed");
        }

        if (!NET_ECMS_SetCallback(NET_EHOME_V2_VOICETALK_CB, NULL, this))
        {
            g_pMainDlg->AddLog(0, OPERATION_FAIL_T, 1, "NET_ECMS_SetCallback, NET_EHOME_V2_VOICETALK_CB, Failed");
        }
    }
}

void CEHomeDemoDlg::OnMenuStorageserver()
{
    // TODO:  在此添加命令处理程序代码
    CDlgStorageServer dlg;
    dlg.DoModal();
}

void CEHomeDemoDlg::GetServerType(int &iServerType, char *pDasInfoPath, DWORD dwDasInfoPathLen)
{
    CXmlBase   struXMLBase;
    struXMLBase.LoadFile(".\\EHomeDemo.xml");

    LONG iMapType = -1;
    if (struXMLBase.FindElem("LocalCfg") && struXMLBase.IntoElem())
    {
        iMapType = 0;
        if (struXMLBase.FindElem("DasInfo") && struXMLBase.IntoElem())
        {
            ConvertSingleNodeData(&iServerType, struXMLBase, "ServerType", NODE_STRING_TO_INT, sizeof(iServerType));
            ConvertSingleNodeData(pDasInfoPath, struXMLBase, "DasInfoPath", NODE_STRING_TO_ARRAY, dwDasInfoPathLen);
        }
    }

}

void CEHomeDemoDlg::GetDasAddressByType(LONG iType, DWORD dwVersion, char *pOutValue, DWORD dwOutLen, char *pSubValue, DWORD dwSubLen) const
{
    CXmlBase   struXMLBase;
    struXMLBase.LoadFile(".\\EHomeDemo.xml");
    LONG iMapType = 0;

    LONG iServerType = -1;
    if (struXMLBase.FindElem("LocalCfg") && struXMLBase.IntoElem())
    {
        if (struXMLBase.FindElem("AddressMap") && struXMLBase.IntoElem())
        {
            ConvertSingleNodeData(&iMapType, struXMLBase, "Enable", NODE_STRING_TO_INT, sizeof(iMapType));
            if (iMapType != 1 && iMapType != 0 && iMapType != -1)
            {
                iMapType = -1;
            }
            struXMLBase.OutOfElem();
        }

        iServerType = 0;
        if (struXMLBase.FindElem("DasInfo") && struXMLBase.IntoElem())
        {
            ConvertSingleNodeData(&iServerType, struXMLBase, "ServerType", NODE_STRING_TO_INT, sizeof(iServerType));
            if (iServerType != 2 && iServerType != 1 && iServerType != 0 && iServerType != -1)
            {
                iServerType = -1;
            }
        }
    }

    //优先使用映射地址
    if (DASAdd == iType)
    {
        GetDasAddress(pOutValue, dwOutLen, pSubValue, dwSubLen, iMapType, struXMLBase);
    }
}


void CEHomeDemoDlg::OnMenuWakeupDevice()
{
    // TODO:  在此添加命令处理程序代码
    if (m_iCurDeviceIndex == -1)
    {
        return;
    }
    else
    {
        int lLoginID = g_struDeviceInfo[m_iCurDeviceIndex].lLoginID;
        if (NET_ECMS_WakeUp(lLoginID))
        {
            AddLog(m_iCurDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_WakeUp [%d]", lLoginID);
        }
        else
        {
            AddLog(m_iCurDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_WakeUp [%d]", lLoginID);
        }
    }
}


void CEHomeDemoDlg::OnMenuSleepDevice()
{
    // TODO:  在此添加命令处理程序代码
    if (m_iCurDeviceIndex == -1)
    {
        return;
    }
    else
    {
        bool bRet = FALSE;
        bool bNeedReboot = FALSE;
        int lLoginID = g_struDeviceInfo[m_iCurDeviceIndex].lLoginID;

        NET_EHOME_PTXML_PARAM struPTXML = { 0 };
        CString strUrl = "/ISAPI/System/consumptionMode?format=json";
        struPTXML.pRequestUrl = strUrl.GetBuffer(0);
        struPTXML.dwRequestUrlLen = strUrl.GetLength();

        CString strInput = "{\"ConsumptionMode\": {\"enabled\": true,\"devWorkMode\" : \"sleepOrWakeup\",\"Sleep\" : {\"enabled\": false},\"powerThreshold\" : 20}}";

        struPTXML.pCondBuffer = NULL;
        struPTXML.dwCondSize = 0;
        struPTXML.pInBuffer = strInput.GetBuffer(0);
        struPTXML.dwInSize = strInput.GetLength();

        char *sOutput = new char[512 * 1024];
        memset(sOutput, 0, 512 * 1024);

        struPTXML.pOutBuffer = sOutput;
        struPTXML.dwOutSize = 512 * 1024;
        struPTXML.dwRecvTimeOut = 5000;

        //需要将字符串字段转换成UTF-8
        A2UTF8((char*)struPTXML.pInBuffer, (char*)struPTXML.pInBuffer, struPTXML.dwInSize, &struPTXML.dwInSize);
        A2UTF8((char*)struPTXML.pCondBuffer, (char*)struPTXML.pCondBuffer, struPTXML.dwCondSize, &struPTXML.dwCondSize);

        if (NET_ECMS_PutPTXMLConfig(lLoginID, &struPTXML))
        {
            cJSON* pRoot = cJSON_Parse((char*)struPTXML.pOutBuffer);
            if (pRoot != NULL)
            {
                cJSON* pStatusCode = cJSON_GetObjectItem(pRoot, "statusCode");
                if (pStatusCode != NULL && pStatusCode->type == cJSON_Number && pStatusCode->valueint == 1)
                {
                    bRet = TRUE;
                }
                else if (pStatusCode != NULL && pStatusCode->type == cJSON_Number && pStatusCode->valueint == 7)
                {
                    bNeedReboot = TRUE;
                }
                else
                {
                    bRet = FALSE;
                }
                cJSON_Delete(pRoot);
            }
        }
        char *sOutput2 = NULL;

        if (bRet || bNeedReboot)//成功也需要重启
        {
            NET_EHOME_PTXML_PARAM struPTXML2 = { 0 };
            CString strUrl2 = "	/ISAPI/System/reboot";
            struPTXML2.pRequestUrl = strUrl2.GetBuffer(0);
            struPTXML2.dwRequestUrlLen = strUrl2.GetLength();

            struPTXML2.pCondBuffer = NULL;
            struPTXML2.dwCondSize = 0;
            struPTXML2.pInBuffer = NULL;
            struPTXML2.dwInSize = 0;

            sOutput2 = new char[512 * 1024];
            memset(sOutput2, 0, 512 * 1024);

            struPTXML2.pOutBuffer = sOutput2;
            struPTXML2.dwOutSize = 512 * 1024;
            struPTXML2.dwRecvTimeOut = 5000;

            //需要将字符串字段转换成UTF-8
            A2UTF8((char*)struPTXML2.pInBuffer, (char*)struPTXML2.pInBuffer, struPTXML2.dwInSize, &struPTXML2.dwInSize);
            A2UTF8((char*)struPTXML2.pCondBuffer, (char*)struPTXML2.pCondBuffer, struPTXML2.dwCondSize, &struPTXML2.dwCondSize);

            if (NET_ECMS_PutPTXMLConfig(lLoginID, &struPTXML2))
            {
                cJSON* pRoot = cJSON_Parse((char*)struPTXML2.pOutBuffer);
                if (pRoot != NULL)
                {
                    cJSON* pStatusCode = cJSON_GetObjectItem(pRoot, "statusCode");
                    if (pStatusCode != NULL && pStatusCode->type == cJSON_Number && pStatusCode->valueint == 1)
                    {
                        bRet = TRUE;
                    }
                    else
                    {
                        bRet = FALSE;
                    }
                    cJSON_Delete(pRoot);
                }
            }
        }

        if (bRet)
        {
            g_pMainDlg->AddLog(m_iCurDeviceIndex, OPERATION_SUCC_T, 1, "OnMenuSleepDevice");
        }
        else
        {
            g_pMainDlg->AddLog(m_iCurDeviceIndex, OPERATION_FAIL_T, 1, "OnMenuSleepDevice");
        }


        if (NULL != sOutput)
        {
            delete[] sOutput;
            sOutput = NULL;
        }

        if (NULL != sOutput2)
        {
            delete[] sOutput2;
            sOutput2 = NULL;
        }
    }
}

void CEHomeDemoDlg::OnMenuWebConfig()
{
    CDlgWebConfig dlg;
    dlg.DoModal();
}

BOOL  CEHomeDemoDlg::GetISAPIChannels(LONG lUserID, DWORD *dwChannelNum)
{
    NET_EHOME_PTXML_PARAM struPTXML = { 0 };
    int iAudioBitRate = 0;
    int iAudioSamplingRate = 0;
    //先通过透传获取一下设备当前的采样赫兹，编码比特率
    int m_lUserID = lUserID;

    CString csURL = "/ISAPI/System/Video/inputs/channels";
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

    if (NET_ECMS_GetPTXMLConfig(m_lUserID, &struPTXML))
    {
        // 解析设备返回的XML数据
        CXmlBase xmlBase;
        string strTemp = (char *)struPTXML.pOutBuffer;
        xmlBase.Parse((char *)struPTXML.pOutBuffer);
        int iNum = 0;
        if (xmlBase.FindElem("VideoInputChannelList") && xmlBase.IntoElem())
        {
            do
            {
                if (xmlBase.FindElem("VideoInputChannel") && xmlBase.IntoElem())
                {
                    iNum++;
                    xmlBase.OutOfElem();
                }
            } while (xmlBase.NextSibElem());
            xmlBase.OutOfElem();
        }
        *dwChannelNum = iNum;
    }
    else
    {
        g_pMainDlg->AddLog(m_iCurDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_GetPTXMLConfig");
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


void CEHomeDemoDlg::OnBnClickedCheckRtpAudio()
{
    // TODO:  在此添加控件通知处理程序代码

    UpdateData(TRUE); //刷新控件变量值
    return;
}

void CEHomeDemoDlg::ReadDevAuthInfoFromXML()
{
    std::string szDeviceIDValue;
    std::string szAuthKeyValue;
    CXmlBase xmlBase;
    if (xmlBase.LoadFile(ISUP_DEMO_CONFIG_XML))
    {
        if (xmlBase.FindElem("DeviceAuthInfo") && xmlBase.IntoElem())
        {
            if (xmlBase.FindElem("DeviceInfo"))
            {
                do
                {
                    //DeviceID
                    szDeviceIDValue = xmlBase.GetAttributeValue("dev_id");
                    //EhomeKey
                    szAuthKeyValue = xmlBase.GetAttributeValue("key");
                    m_mapDevAuthKeyInfos.insert(make_pair(szDeviceIDValue, szAuthKeyValue));
                } while (xmlBase.NextSibElem());
            }
            xmlBase.OutOfElem();
        }
    }
    return;
}

std::string CEHomeDemoDlg::GetDevAuthKey(const char* pDeviceID)
{
    if (pDeviceID == NULL)
    {
        return "";
    }
    std::string szDeviceID = pDeviceID;
    DevAuthKeyMapIter iter = m_mapDevAuthKeyInfos.find(szDeviceID);
    if (iter != m_mapDevAuthKeyInfos.end())
    {
        if (!iter->second.empty())
        {
            return iter->second;
        }
    }

    return "";
}

void CEHomeDemoDlg::RecordDevAuthInfo(const char* pDeviceID, const char* pDevAuthKey)
{
    if (pDeviceID == NULL || pDevAuthKey == NULL)
    {
        return;
    }
    CXmlBase xmlBase;
    std::string szDeviceID = pDeviceID;
    std::string szDevAuthKey = pDevAuthKey;
    DevAuthKeyMapIter iter = m_mapDevAuthKeyInfos.find(szDeviceID);
    if (iter != m_mapDevAuthKeyInfos.end())
    {
        iter->second = szDevAuthKey;
        if (xmlBase.LoadFile(ISUP_DEMO_CONFIG_XML))
        {
            if (xmlBase.FindElem("DeviceAuthInfo") && xmlBase.IntoElem())
            {
                do
                {
                    if (xmlBase.FindElem("DeviceInfo"))
                    {
                        //OTAP Key
                        string szAttribute = xmlBase.GetAttributeValue("dev_id");
                        if (szAttribute == szDeviceID)
                        {
                            string szKeyAttribute = xmlBase.GetAttributeValue("key");
                            if (szKeyAttribute == szDevAuthKey)
                            {
                                return;
                            }
                            xmlBase.SetAttribute("key", szDevAuthKey.c_str());
                            break;
                        }

                    }
                } while (xmlBase.NextSibElem());

                xmlBase.OutOfElem();
                xmlBase.WriteToFile(ISUP_DEMO_CONFIG_XML);
            }
        }
        return;
    }
    m_mapDevAuthKeyInfos.insert(make_pair(szDeviceID, szDevAuthKey));
    if (xmlBase.LoadFile(ISUP_DEMO_CONFIG_XML) && xmlBase.FindElem("DeviceAuthInfo") && !szDevAuthKey.empty())
    {
        xmlBase.AddNode("DeviceInfo");
        xmlBase.IntoElem();
        xmlBase.SetAttribute("dev_id", szDeviceID.c_str());
        xmlBase.SetAttribute("key", szDevAuthKey.c_str());
        xmlBase.OutOfElem();
        xmlBase.WriteToFile(ISUP_DEMO_CONFIG_XML);
        g_pMainDlg->AddLog(m_iCurDeviceIndex, OPERATION_SUCC_T, 1, "DevAuthKeyInfos insert DeviceID[%s] key[%s]", szDeviceID.c_str(), szDevAuthKey.c_str());
    }

    return;
}