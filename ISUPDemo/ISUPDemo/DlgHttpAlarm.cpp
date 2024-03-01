// DlgHttpAlarm.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgHttpAlarm.h"
#include "afxdialogex.h"
#include "Public/TinyXML/XmlBase.h"
#include "public/cjson/cJson.h"


#define MAX_ALARM_BUFF_LEN 2*1024*1024
// CDlgHttpAlarm 对话框

IMPLEMENT_DYNAMIC(CDlgHttpAlarm, CDialog)

CDlgHttpAlarm::CDlgHttpAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHttpAlarm::IDD, pParent)
{
    m_pAlarmInfo = NULL;
    m_dwAlarmLen = 0;
    m_bFinished = TRUE;
    m_pXML = NULL;
    memset(m_szVisibleFile, 0, MAX_PATH);
    memset(m_szThermal, 0, MAX_PATH);
}

CDlgHttpAlarm::~CDlgHttpAlarm()
{
    m_dwAlarmLen = 0;
    if (m_pAlarmInfo != NULL)
    {
        delete[] m_pAlarmInfo;
        m_pAlarmInfo = NULL;
    }
    if (m_pXML != NULL)
    {
        delete[] m_pXML;
        m_pXML = NULL;
    }

}

void CDlgHttpAlarm::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_ALARMPICTURE_COLOR, m_AlarmPicture1);
    DDX_Control(pDX, IDC_STATIC_ALARMPICTURE_GRAY, m_AlarmPicture2);
    DDX_Control(pDX, IDC_EDIT_DETAIL, m_edtXMLBuff);
}


// CDlgHttpAlarm 消息处理程序
BEGIN_MESSAGE_MAP(CDlgHttpAlarm, CDialog)
    ON_BN_CLICKED(IDC_BTN_REFRESH, &CDlgHttpAlarm::OnBnClickedBtnRefresh)
    ON_MESSAGE(WM_HTTP_ALARM, OnProcessHttpAlarm)
END_MESSAGE_MAP()

BOOL CDlgHttpAlarm::OnInitDialog()
{
    CDialog::OnInitDialog();
   
    CString dir = "C:\\Http_Alarm_Guard";
    DWORD dwAttr = GetFileAttributes(dir);
    if (dwAttr == -1 || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)   // 目录不存在
    {
        if (!CreateDirectory(dir, NULL))
        {
            return FALSE;
        }
    }

    if (m_pAlarmInfo == NULL)
    {
        m_pAlarmInfo = new char[MAX_ALARM_BUFF_LEN];
    }

    return TRUE;
}

LRESULT CDlgHttpAlarm::OnProcessHttpAlarm(WPARAM wParam, LPARAM lParam)
{
    if (m_bFinished)
    {
        m_bFinished = FALSE;
        
        memset(m_pAlarmInfo, 0, MAX_ALARM_BUFF_LEN);
        memcpy(m_pAlarmInfo, (void*)wParam, lParam);
 
        SYSTEMTIME struTime = { 0 };
        ::GetSystemTime(&struTime);
        memset(m_szVisibleFile, 0, MAX_PATH);
        memset(m_szThermal, 0, MAX_PATH);

        sprintf_s(m_szVisibleFile, "%s\\%4d-%2d-%2d-%2d-%2d-%2d-%3d_visible.jpg", "C:\\Http_Alarm_Guard", struTime.wYear, struTime.wMonth, struTime.wDay, 
            struTime.wHour, struTime.wMinute, struTime.wSecond, struTime.wMilliseconds);
        sprintf_s(m_szThermal, "%s\\%4d-%2d-%2d-%2d-%2d-%2d-%3d_thermal.jpg", "C:\\Http_Alarm_Guard", struTime.wYear, struTime.wMonth, struTime.wDay,
            struTime.wHour, struTime.wMinute, struTime.wSecond, struTime.wMilliseconds);
        
        SeparteMutipart();
        OnBnClickedBtnRefresh();
        m_bFinished = TRUE;
    }
    return 0;
}


BOOL CDlgHttpAlarm::XmlPrase(char* pXml, char* pInputBefore, char* pInputAfter, char* pOutput)
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

BOOL CDlgHttpAlarm::SeparteMutipart()
{
    char bBrBr[MAX_PATH]                = "\r\n\r\n";
    char bContentLength[MAX_PATH]       = "Content-Length:";
    char bTestXML[MAX_PATH]             = "application";
    char bIMGMJPG[MAX_PATH]             = "image/pjpeg";

    //XML报文
    int iXMLIndexBegin = FindStrIndex(m_pAlarmInfo, bTestXML);
    if (iXMLIndexBegin > MAX_ALARM_BUFF_LEN || iXMLIndexBegin < 0)
    {
        return FALSE;
    }

    int iXMLLenBegin = FindStrIndex(m_pAlarmInfo + iXMLIndexBegin, bContentLength) + iXMLIndexBegin + strlen(bContentLength) + 1;
    if (iXMLLenBegin > MAX_ALARM_BUFF_LEN || iXMLIndexBegin < 0)
    {
        return FALSE;
    }
    int iXMLLenEnd = FindStrIndex(m_pAlarmInfo + iXMLLenBegin, bBrBr) + iXMLLenBegin;
    if (iXMLLenEnd > MAX_ALARM_BUFF_LEN || iXMLLenEnd < 0)
    {
        return FALSE;
    }

    char strXMLLen[MAX_PATH] = { 0 };
    memcpy(strXMLLen, m_pAlarmInfo + iXMLLenBegin, iXMLLenEnd - iXMLLenBegin);
    int iXMLLen = atoi(strXMLLen);
    if (iXMLLen <= 0)
    {
        return FALSE;
    }

    if (m_pXML == NULL)
    {
        m_pXML = new char[iXMLLen + 1];
    }
  
    if (m_pXML && iXMLLen > 0)
    {
        memset(m_pXML, 0, iXMLLen + 1);
        memcpy(m_pXML, m_pAlarmInfo + iXMLLenEnd + 4, iXMLLen);
    }

    char szDeviceID[128] = { 0 };
    bool bHasDeviceID = false;

    if (!XmlPrase((char*)m_pXML, "<deviceID>", "</deviceID>", szDeviceID))
    {
        cJSON* pRoot = cJSON_Parse(m_pXML);
        if (pRoot != NULL)
        {
            cJSON* pPercent = cJSON_GetObjectItem(pRoot, "deviceID");
            if (pPercent != NULL || pPercent->type == cJSON_Number)
            {
                bHasDeviceID = true;
            }
            cJSON_Delete(pRoot);
        }
    }
    else
    {
        bHasDeviceID = true;
    }

    if (!bHasDeviceID)
    {
        g_pMainDlg->AddLog(-1, ALARM_INFO_T, 0, "The device did not report the device ID!");
    }

    char szInfoBuf[1024] = { 0 };
    char byOutput[32] = { 0 };
    int  iPicNum = 0;
    memset(byOutput, 0, 32);

    if (XmlPrase((char*)m_pXML, "<eventType>", "</eventType>", byOutput))
    {
        sprintf(szInfoBuf + strlen(szInfoBuf), "%s ", byOutput);
    }
    else
    {
        cJSON* pRoot = cJSON_Parse((char*)m_pXML);
        if (pRoot != NULL)
        {
            cJSON* pPercent = cJSON_GetObjectItem(pRoot, "eventType");
            if (pPercent != NULL || pPercent->type == cJSON_String)
            {
                sprintf(szInfoBuf + strlen(szInfoBuf), "%s ", pPercent->valuestring);
            }
            cJSON_Delete(pRoot);
        }
    }

    g_pMainDlg->AddLog(-1, ALARM_INFO_T, 0, szInfoBuf);

    if (XmlPrase((char*)m_pXML, "<detectionPicturesNumber>", "</detectionPicturesNumber>", byOutput))
    {
        iPicNum = atoi(byOutput);
    }
    //第一张图
    int iImgJpg1StrIndexBegin = -1;
    int iImgJpg1LenIndexBegin = -1;
    int iImgJpg1LenIndexEnd = -1;
    int iImgJpg1Len = -1;

    iImgJpg1StrIndexBegin = FindStrIndex(m_pAlarmInfo, bIMGMJPG);                             //相对于m_pAlarmInfo的偏移位置
    if (iImgJpg1StrIndexBegin > MAX_ALARM_BUFF_LEN || iImgJpg1StrIndexBegin < 0)
    {
        return FALSE;
    }
    iImgJpg1LenIndexBegin = FindStrIndex(m_pAlarmInfo + iImgJpg1StrIndexBegin, bContentLength) + iImgJpg1StrIndexBegin + strlen(bContentLength) + 1;
    if (iImgJpg1LenIndexBegin > MAX_ALARM_BUFF_LEN || iImgJpg1LenIndexBegin < 0)
    {
        return FALSE;
    }
    int i = FindStrIndex(m_pAlarmInfo + iImgJpg1LenIndexBegin, bBrBr);
    if (i > MAX_ALARM_BUFF_LEN || i < 0)
    {
        return FALSE;
    }
    iImgJpg1LenIndexEnd = FindStrIndex(m_pAlarmInfo + iImgJpg1LenIndexBegin, bBrBr) + iImgJpg1LenIndexBegin;        //相对于m_pAlarmInfo的偏移位置
    if (iImgJpg1LenIndexEnd > MAX_ALARM_BUFF_LEN || iImgJpg1LenIndexEnd < 0)
    {
        return FALSE;
    }

    char strImgJpgContentLen[MAX_PATH] = { 0 };
    memcpy(strImgJpgContentLen, m_pAlarmInfo + iImgJpg1LenIndexBegin, iImgJpg1LenIndexEnd - iImgJpg1LenIndexBegin);
    iImgJpg1Len = atoi(strImgJpgContentLen);

    if (iImgJpg1Len <= 0)
    {
        return FALSE;
    }
    //第一张图
    char* strSrc = new char[iImgJpg1Len + 1];
    if (strSrc && iImgJpg1Len > 0)
    {
        memset(strSrc, 0, iImgJpg1Len + 1);
        memcpy(strSrc, m_pAlarmInfo + iImgJpg1LenIndexEnd + 4, iImgJpg1Len);

        CFile file;
        file.Open(m_szVisibleFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);
        file.Write(strSrc, iImgJpg1Len);
        file.Close();
        delete strSrc;
        strSrc = NULL;
    }

    if (iPicNum > 1)
    {
        //第二张图
        int iImgJpg2StrIndexBegin = -1;
        int iImgJpg2LenIndexBegin = -1;
        int iImgJpg2LenIndexEnd = -1;
        int iImgJpg2Len = -1;

        iImgJpg2StrIndexBegin = FindStrIndex(m_pAlarmInfo + iImgJpg1LenIndexEnd + 4 + iImgJpg1Len, bIMGMJPG) + iImgJpg1LenIndexEnd + iImgJpg1Len + 4;                             //相对于m_pAlarmInfo的偏移位置
        if (iImgJpg2StrIndexBegin > MAX_ALARM_BUFF_LEN || iImgJpg2StrIndexBegin < 0)
        {
            return FALSE;
        }
        iImgJpg2LenIndexBegin = FindStrIndex(m_pAlarmInfo + iImgJpg2StrIndexBegin, bContentLength) + iImgJpg2StrIndexBegin + strlen(bContentLength) + 1;
        if (iImgJpg2LenIndexBegin > MAX_ALARM_BUFF_LEN || iImgJpg2LenIndexBegin < 0)
        {
            return FALSE;
        }
        i = FindStrIndex(m_pAlarmInfo + iImgJpg2LenIndexBegin, bBrBr);
        if (i > MAX_ALARM_BUFF_LEN || i < 0)
        {
            return FALSE;
        }
        iImgJpg2LenIndexEnd = FindStrIndex(m_pAlarmInfo + iImgJpg2LenIndexBegin, bBrBr) + iImgJpg2LenIndexBegin;        //相对于m_pAlarmInfo的偏移位置
        if (iImgJpg2LenIndexEnd > MAX_ALARM_BUFF_LEN || iImgJpg2LenIndexEnd < 0)
        {
            return FALSE;
        }

        memset(strImgJpgContentLen, 0, MAX_PATH);
        memcpy(strImgJpgContentLen, m_pAlarmInfo + iImgJpg2LenIndexBegin, iImgJpg2LenIndexEnd - iImgJpg2LenIndexBegin);
        iImgJpg2Len = atoi(strImgJpgContentLen);

        strSrc = new char[iImgJpg2Len + 1];
        if (strSrc && iImgJpg2Len > 0)
        {
            memset(strSrc, 0, iImgJpg2Len + 1);
            memcpy(strSrc, (char*)m_pAlarmInfo + iImgJpg2LenIndexEnd + 4, iImgJpg2Len);
            CFile file;
            file.Open(m_szThermal, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate, NULL);
            file.Write(strSrc, iImgJpg2Len);
            file.Close();
            delete strSrc;
            strSrc = NULL;
        }
    }
    
    return TRUE;
} 





void CDlgHttpAlarm::OnBnClickedBtnRefresh()
{
    // TODO:  在此添加控件通知处理程序代码
    if (m_pXML)
    {
        CString str = m_pXML;
        //str.Format(_T("%s"), m_pXML);
        m_edtXMLBuff.SetWindowTextA(str);
    }

    CDC *pDC = NULL;
    CString strPath = NULL;
    pDC = GetDlgItem(IDC_STATIC_ALARMPICTURE_COLOR)->GetDC();
    strPath.Format(_T("%s"), m_szVisibleFile);
    if (strPath && pDC)
    {
        DrawImg(pDC, strPath, 0, 0);
    }
    ReleaseDC(pDC);
    pDC = GetDlgItem(IDC_STATIC_ALARMPICTURE_GRAY)->GetDC();
    strPath.Format(_T("%s"), m_szThermal);
    if (strPath && pDC)
    {
        DrawImg(pDC, strPath, 0, 0);
    }
    ReleaseDC(pDC);
    UpdateData(FALSE);
}

void CDlgHttpAlarm::DrawImg(CDC* pDC, CString strPath, int x, int y)
{
    IPicture *pPic = NULL;
    //利用流加载图像
    OleLoadPicturePath(CComBSTR(strPath.GetBuffer()), (LPUNKNOWN)NULL, 0, 0, IID_IPicture, (LPVOID*)&pPic);
    if (NULL == pPic)
    {
        return;
    }

    // 获取图像宽和高,注意这里的宽和高不是图像的分辨率    
    OLE_XSIZE_HIMETRIC hmWidth;
    OLE_YSIZE_HIMETRIC hmHeight;
    pPic->get_Width(&hmWidth);
    pPic->get_Height(&hmHeight);

    // 获取显示图片窗口的宽度和高度    
    RECT rtWnd;
    pDC->GetWindow()->GetWindowRect(&rtWnd);
    int iWndWidth = rtWnd.right - rtWnd.left;
    int iWndHeight = rtWnd.bottom - rtWnd.top;
    if (FAILED(pPic->Render(*pDC, x, y, iWndWidth, iWndHeight, 0, hmHeight, hmWidth, -hmHeight, NULL)))
    {
        pPic->Release();
        return;
    }

    //记得释放资源，不然会导致内存泄露    
    pPic->Release();
}


INT32 CDlgHttpAlarm::FindStrIndex(char *s, char *d)
{
    char *p = s, *q;
    int i;

    while (*p)
    {
        i = p - s;
        q = d;
        while (*p == *q) 
        { 
            p++; 
            q++; 
        }
        if (*q == '\0') 
            return i;
        p = s + (++i);
    }

    return -1;
}