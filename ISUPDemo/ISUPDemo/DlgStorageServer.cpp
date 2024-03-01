// DlgStorageServer.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgStorageServer.h"
#include "afxdialogex.h"

// CDlgStorageServer 对话框

IMPLEMENT_DYNAMIC(CDlgStorageServer, CDialog)

CDlgStorageServer::CDlgStorageServer(CWnd* pParent /*=NULL*/)
: CDialog(CDlgStorageServer::IDD, pParent)
, m_strPoolId(_T("1"))
, m_csDownloadUrl(_T("/pic?5DBF16C7080D9B8E3D3ABF04ECF27FD1test.jpg"))
, m_csFileStorePath(_T(""))
, m_csDlAk(_T("test"))
, m_csDlSk(_T("12345"))
, m_csDlServerIP(_T("10.21.84.32"))
, m_dwDlPort(7334)
, m_strUpKMSname(_T("test"))
, m_strUpPass(_T("12345"))
, m_strDownKMSName(_T("test"))
, m_strDownKMSPass(_T("12345"))
, m_strDelKMSName(_T("test"))
, m_strDelKMSPass(_T("12345"))
, m_strDelAK(_T(""))
, m_strDelSK(_T(""))
, m_iDelPort(0)
, m_iFileNum(0)
, m_strDelUrl(_T(""))
, m_strDelIp(_T("10.21.84.32"))
, m_cbBufferUp(FALSE)
, m_bHttpsUpload(FALSE)
, m_bHttpsDownload(FALSE)
, m_bHttpsDelete(FALSE)
, m_dwLFHandle(INVALID_LINK_INDEX)
, m_pFile(NULL)
, m_dwSequence(0)
, m_csLFPort(8555)
, m_csLFSessionId(_T("AFWGJDF"))
, m_csLFURL(_T("http://10.14.98.49:80/picture/Streaming/tracks/103/?name=ch00001_00000000546040778598400609593&size=609593"))
, m_csLFStart(0)
, m_csLFEnd(0)
, m_csLFPath(_T(""))
, m_csLFIP(_T("10.19.82.106"))
, m_bLFResume(FALSE)
, m_lUserID(-1)
, m_iDeviceIndex(-1)
, m_bSetPath(FALSE)
{

}

CDlgStorageServer::~CDlgStorageServer()
{
}

void CDlgStorageServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERVERTYPE, m_cmbServerType);
	DDX_Text(pDX, IDC_EDIT_FILEURL, m_csFileName);
	DDX_Text(pDX, IDC_EDIT_URLRESULT, m_csFileURL);
	DDX_Text(pDX, IDC_EDIT_LISTENIP, m_csListenIP);
	DDX_Text(pDX, IDC_EDIT_LISTENPORT, m_nPort);
	DDX_Control(pDX, IDC_EDIT_AK, m_oEditAK);
	DDX_Control(pDX, IDC_EDIT_SK, m_oEditSK);
	DDX_Text(pDX, IDC_EDIT_POOL_ID, m_strPoolId);
	DDX_Text(pDX, IDC_EDT_DOWNLOAD_URL, m_csDownloadUrl);
	DDX_Text(pDX, IDC_EDT_FILE_STORE_PATH, m_csFileStorePath);
	DDX_Control(pDX, IDC_CMB_DL_SERVERTYPE, m_cmbDlServerType);
	DDX_Text(pDX, IDC_EDT_DL_AK, m_csDlAk);
	DDX_Text(pDX, IDC_EDT_DL_SK, m_csDlSk);
	DDX_Text(pDX, IDC_EDT_DL_SERVER_IP, m_csDlServerIP);
	DDX_Text(pDX, IDC_EDT_SERVER_PORT, m_dwDlPort);
	DDX_Text(pDX, IDC_EDIT_KMSNAME3, m_strUpKMSname);
	DDX_Text(pDX, IDC_EDIT_KMSPASS3, m_strUpPass);
	DDX_Text(pDX, IDC_EDIT_KMSNAME4, m_strDownKMSName);
	DDX_Text(pDX, IDC_EDIT_KMSPASS4, m_strDownKMSPass);
	DDX_Text(pDX, IDC_EDIT_KMSNAME5, m_strDelKMSName);
	DDX_Text(pDX, IDC_EDIT_KMSPASS5, m_strDelKMSPass);
	DDX_Text(pDX, IDC_EDT_DL_AK2, m_strDelAK);
	DDX_Text(pDX, IDC_EDT_DL_SK2, m_strDelSK);
	DDX_Text(pDX, IDC_EDT_SERVER_PORT2, m_iDelPort);
	DDX_Text(pDX, IDC_EDT_DL_SERVER_IP2, m_strDelIp);
	DDX_Text(pDX, IDC_EDT_DOWNLOAD_URL2, m_strDelUrl);
	DDX_Control(pDX, IDC_CMB_DL_SERVERTYPE2, m_cboxDelType);
	DDX_Check(pDX, IDC_CHECK_BUFFUP, m_cbBufferUp);
	DDX_Check(pDX, IDC_CHECK1, m_bHttpsUpload);
	DDX_Check(pDX, IDC_CHECK2, m_bHttpsDownload);
	DDX_Check(pDX, IDC_CHECK3, m_bHttpsDelete);
	DDX_Text(pDX, IDC_LF_EDIT_PORT, m_csLFPort);
	DDX_Text(pDX, IDC_EDIT4, m_csLFSessionId);
	DDX_Text(pDX, IDC_LF_EDIT_URL, m_csLFURL);
	DDX_Text(pDX, IDC_EDIT_LF_START, m_csLFStart);
	DDX_Text(pDX, IDC_EDIT_LF_END, m_csLFEnd);
	DDX_Text(pDX, IDC_LF_EDIT_PATH, m_csLFPath);
	DDX_Text(pDX, IDC_LF_IP_EDIT, m_csLFIP);
	DDX_Check(pDX, IDC_CHECK_LF_RESUME, m_bLFResume);
}


BEGIN_MESSAGE_MAP(CDlgStorageServer, CDialog)
    ON_BN_CLICKED(IDC_BTN_COMMAND, &CDlgStorageServer::OnBnClickedBtnCommand)
    ON_BN_CLICKED(IDC_BTN_PICFILE, &CDlgStorageServer::OnBnClickedBtnPicfile)
    ON_CBN_SELCHANGE(IDC_COMBO_SERVERTYPE, &CDlgStorageServer::OnCbnSelchangeComboServertype)
    ON_BN_CLICKED(IDC_BTN_STOREPATH, &CDlgStorageServer::OnBnClickedBtnStorepath)
    ON_BN_CLICKED(IDC_BTN_DOWLOAD_FILE, &CDlgStorageServer::OnBnClickedBtnDowloadFile)
    ON_CBN_SELCHANGE(IDC_CMB_DL_SERVERTYPE, &CDlgStorageServer::OnCbnSelchangeCmbDlServertype)
    ON_BN_CLICKED(IDC_BTN_DOWLOAD_FILE2, &CDlgStorageServer::OnBnClickedBtnDowloadFile2)
    ON_BN_CLICKED(IDC_BUTTON_LF_DOWNLOAD, &CDlgStorageServer::OnBnClickedButtonLfDownload)
    ON_BN_CLICKED(IDC_BUTTON_LF_CREATE_SERVER, &CDlgStorageServer::OnBnClickedButtonLfCreateServer)
	ON_BN_CLICKED(IDC_BUTTON_LF_UPLOAD, &CDlgStorageServer::OnBnClickedButtonLfUpload)
	ON_BN_CLICKED(IDC_BUTTON_LF_CREATE_CHANNEL, &CDlgStorageServer::OnBnClickedButtonLfCreateChannel)
	ON_BN_CLICKED(IDC_BUTTON_LF_DEL_CHANNEL, &CDlgStorageServer::OnBnClickedButtonLfDelChannel)
	ON_BN_CLICKED(IDC_BTN_STOREPATH_LF, &CDlgStorageServer::OnBnClickedBtnStorepathLf)
	ON_BN_CLICKED(IDC_BUTTON_LF_STOPLF, &CDlgStorageServer::OnBnClickedButtonLfStoplf)
END_MESSAGE_MAP()


// CDlgPicServer 消息处理程序


void CDlgStorageServer::OnBnClickedBtnCommand()
{
    //TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    //图片上传到图片服务器示例代码
    NET_EHOME_SS_CLIENT_PARAM struClientParam;
    memset(&struClientParam, 0, sizeof(struClientParam));
    struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_TOMCAT;//图片上传客户端类型
    memcpy(struClientParam.struAddress.szIP, m_csListenIP, sizeof(g_pMainDlg->m_sLocalIP));
    struClientParam.struAddress.wPort = m_nPort; ////图片服务器端口
    if (m_bHttpsUpload)
    {
        struClientParam.byHttps = 1;  //是否启用HTTPs
    }
    else
    {
        struClientParam.byHttps = 0;  //是否启用HTTPs
    }
    LONG lPssClientHandle = -1;
    char szUrl[MAX_URL_LEN_SS] = { 0 };
    char szPicUrl[MAX_URL_LEN_SS] = { 0 };

    if (m_cmbServerType.GetCurSel() == 0)
    {
        //VRB图片上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_VRB;
    }
    else if (m_cmbServerType.GetCurSel() == 1)
    {
        //KMS图片上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_KMS;
    }
    else if (m_cmbServerType.GetCurSel() == 2)
    {
        //Cloud图片上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_CLOUD;
    }
    else if (m_cmbServerType.GetCurSel() == 3)
    {
        //Cloud图片上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_TOMCAT;
    }
    else if (m_cmbServerType.GetCurSel() == 4)
    {
        //中心存储视频上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_CENTRAL;
    }
    lPssClientHandle = NET_ESS_CreateClient(&struClientParam);
    if (lPssClientHandle < 0)
    {
        return;
    }

    NET_ESS_ClientSetTimeout(lPssClientHandle, 60 * 1000, 60 * 1000);

    //NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_FILE_PATH_PARAM_NAME, m_csFileName);
    if (m_cmbServerType.GetCurSel() == 0)
    {
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_VRB_FILENAME_CODE, "filename=1.jpg&code=");
        //NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_VRB_FILENAME_CODE, "2=1.jpg&code=");
    }
    else if (m_cmbServerType.GetCurSel() == 2)
    {
        CString strAK, strSK;
        m_oEditAK.GetWindowText(strAK);
        m_oEditSK.GetWindowText(strSK);
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_CLOUD_AK_NAME, strAK.GetBuffer());
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_CLOUD_SK_NAME, strSK.GetBuffer());
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_CLOUD_POOL_ID, m_strPoolId.GetBuffer());
    }
    else if (m_cmbServerType.GetCurSel() == 1)
    {
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_KMS_USER_NAME, m_strUpKMSname.GetBuffer());
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_KMS_PASSWIRD, m_strUpPass.GetBuffer());
    }
    else if (m_cmbServerType.GetCurSel() == 4)
    {
        CString strAK, strSK;
        m_oEditAK.GetWindowText(strAK);
        m_oEditSK.GetWindowText(strSK);
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_CENTRAL_AK_NAME, strAK.GetBuffer());
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_CENTRAL_SK_NAME, strSK.GetBuffer());
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_CENTRAL_POOL_ID, m_strPoolId.GetBuffer());
    }

    if (!m_cbBufferUp)
    {
        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_FILE_PATH_PARAM_NAME, m_csFileName);
        if (NET_ESS_ClientDoUpload(lPssClientHandle, szUrl, MAX_URL_LEN_SS - 1))
        {
            CString m_csPicServerIP = g_pMainDlg->m_struServInfo.struPictureSever.szIP;
            int iPort = g_pMainDlg->m_struServInfo.struPictureSever.wPort;
            sprintf(szPicUrl, "http://%s:%d%s", m_csPicServerIP, iPort, szUrl);

            m_csFileURL = szPicUrl;
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 4, "NET_ESS_ClientDoUpload, success");
        }
        else
        {
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 4, "NET_ESS_ClientDoUpload, Error[%d]", NET_ESS_GetLastError());
        }
    }
    else
    {
        unsigned char * pBuff = NULL;
        DWORD dwFileLen = 0, curpos;
        FILE* pFile = fopen(m_csFileName, "rb+");
        if (pFile == NULL)
        {
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 4, "NET_ESS_ClientDoUpload, file open failed");
            UpdateData(FALSE);
            return;
        }
        curpos = ftell(pFile);
        fseek(pFile, 0, SEEK_END);
        dwFileLen = ftell(pFile);
        if (dwFileLen > 50 * 1024 * 1024)
        {
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 4, "NET_ESS_ClientDoUpload, file excced 50M");
            fclose(pFile);
            UpdateData(FALSE);
            return;
        }
        fseek(pFile, curpos, SEEK_SET);
        pBuff = new unsigned char[dwFileLen];
        memset(pBuff, 0, dwFileLen);
        size_t dwRead = fread(pBuff, 1, dwFileLen, pFile);
        if (dwRead != dwFileLen)
        {
        }

        NET_ESS_ClientSetParam(lPssClientHandle, SS_CLIENT_FILE_PATH_PARAM_NAME, m_csFileName);
        if (NET_ESS_ClientDoUploadBuffer(lPssClientHandle, szUrl, MAX_URL_LEN_SS - 1, pBuff, dwFileLen))
        {
            CString m_csPicServerIP = g_pMainDlg->m_struServInfo.struPictureSever.szIP;
            int iPort = g_pMainDlg->m_struServInfo.struPictureSever.wPort;
            sprintf(szPicUrl, "http://%s:%d%s", m_csPicServerIP, iPort, szUrl);

            m_csFileURL = szPicUrl;
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 4, "NET_ESS_ClientDoUpload, success");
        }
        else
        {
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 4, "NET_ESS_ClientDoUpload, Error[%d]", NET_ESS_GetLastError());
        }
    }

    NET_ESS_DestroyClient(lPssClientHandle);

    UpdateData(FALSE);
}

void CDlgStorageServer::OnBnClickedBtnPicfile()
{
    UpdateData(TRUE);
    // TODO:  在此添加控件通知处理程序代码
    CFileDialog dlg(TRUE, "mpg", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "File(*.jpg;*.jpeg;*.mp4)|*.jpg;*.jpeg;*.mp4|All Files(*.*)|*.*||", this);
    //CFileDialog dlg(TRUE, "mpg", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "File(*.jpg;*.jpeg)|*.jpg;*.jpeg|All Files(*.*)|*.*||", this);
    if (dlg.DoModal() == IDCANCEL)
    {
#ifdef DEMO_LAN_EN
        AfxMessageBox("Open file failed!");
#else
        AfxMessageBox("打开文件失败");
#endif
        return;
    }

    *(&m_csFileName) = dlg.GetPathName();
    UpdateData(FALSE);
}


BOOL CDlgStorageServer::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_cmbServerType.SetCurSel(4);
    m_cmbDlServerType.SetCurSel(3);
    m_cboxDelType.SetCurSel(3);
    m_csListenIP = g_pMainDlg->m_sLocalIP;
    m_strDelIp = g_pMainDlg->m_sLocalIP;
    m_csDlServerIP = g_pMainDlg->m_sLocalIP;
    m_nPort = 6011;
    m_oEditAK.SetWindowTextA("test");
    m_oEditSK.SetWindowTextA("12345");
    UpdateData(FALSE);
    OnCbnSelchangeComboServertype();
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();

	if (m_iDeviceIndex < 0)
	{
		AfxMessageBox("请选择一个设备");
		return TRUE;
	}
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	if (m_lUserID < 0)
	{
		AfxMessageBox("请先登陆设备");
		return TRUE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control


    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}


void CDlgStorageServer::OnCbnSelchangeComboServertype()
{
    // TODO:  在此添加控件通知处理程序代码

    if (m_cmbServerType.GetCurSel() == 2 || m_cmbServerType.GetCurSel() == 4)
    {
        m_oEditAK.ShowWindow(SW_SHOW);
        m_oEditSK.ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_AK)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_SK)->ShowWindow(SW_SHOW);
    }
    else
    {
        m_oEditAK.ShowWindow(SW_HIDE);
        m_oEditSK.ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_AK)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_SK)->ShowWindow(SW_HIDE);
    }
}


void CDlgStorageServer::OnBnClickedBtnStorepath()
{
    UpdateData(TRUE);
    CString  FullPath;
    BROWSEINFO bi;                        //BROWSEINFO结构体  
    TCHAR Buffer[512] = "";
    TCHAR FullPaths[512] = "";
    bi.hwndOwner = m_hWnd;                    //m_hWnd你的程序主窗口  
    bi.pidlRoot = NULL;
    bi.pszDisplayName = Buffer;                //返回选择的目录名的缓冲区  
    bi.lpszTitle = "请选择BMP文件";            //弹出的窗口的文字提示  
    bi.ulFlags = NULL;//BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_BROWSEFORCOMPUTER ;    //只返回目录。其他标志看MSDN  
    bi.lpfn = NULL;                        //回调函数，有时很有用  
    bi.lParam = 0;
    bi.iImage = 0;
    ITEMIDLIST* pidl = ::SHBrowseForFolder(&bi);        //显示弹出窗口，ITEMIDLIST很重要  
    ::SHGetPathFromIDList(pidl, FullPaths);        //在ITEMIDLIST中得到目录名的整个路径  
    if (FullPaths[0] != NULL)
    {
        *(&m_csFileStorePath) = FullPaths;
        UpdateData(FALSE);
    }
}


void CDlgStorageServer::OnBnClickedBtnDowloadFile()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    if (m_csFileStorePath.IsEmpty() || m_csDownloadUrl.IsEmpty())
    {
#ifdef DEMO_LAN_EN
        AfxMessageBox("input valid path or url!");
#else
        AfxMessageBox("输入有效路径或url");
#endif
        return;
    }
    NET_EHOME_SS_CLIENT_PARAM struClientParam;
    memset(&struClientParam, 0, sizeof(struClientParam));

    if (m_bHttpsDownload)
    {
        struClientParam.byHttps = 1;  //是否启用HTTPs
    }
    else
    {
        struClientParam.byHttps = 0;  //是否启用HTTPs
    }

    memcpy(struClientParam.struAddress.szIP, m_csDlServerIP, strlen(m_csDlServerIP));
    struClientParam.struAddress.wPort = (WORD)m_dwDlPort;

    if (m_cmbDlServerType.GetCurSel() == 0)
    {
        //VRB图片上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_VRB;
    }
    else if (m_cmbDlServerType.GetCurSel() == 1)
    {
        //KMS图片上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_CLOUD;
    }
    else if (m_cmbDlServerType.GetCurSel() == 2)
    {
        //Cloud图片上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_KMS;
    }
    else if (m_cmbDlServerType.GetCurSel() == 3)
    {
        //中心存储视频上传示例代码
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_CENTRAL;
    }

    LONG lSSClientHandle = NET_ESS_CreateClient(&struClientParam);

    if (1 == m_cmbDlServerType.GetCurSel())
    { //云存储
        NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CLOUD_AK_NAME, m_csDlAk.GetBuffer());
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CLOUD_SK_NAME, m_csDlSk.GetBuffer());
    }
    else if (m_cmbDlServerType.GetCurSel() == 2)
    {//KMS
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_KMS_USER_NAME, m_strDownKMSName.GetBuffer());
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_KMS_PASSWIRD, m_strDownKMSPass.GetBuffer());
    }
    else if (m_cmbDlServerType.GetCurSel() == 3)
    {//中心存储
        NET_ESS_ClientSetTimeout(lSSClientHandle, 60 * 1000, 60 * 1000);
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CENTRAL_AK_NAME, m_csDlAk.GetBuffer());
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CENTRAL_SK_NAME, m_csDlSk.GetBuffer());
    }
    else
    {//非安全

    }
    byte* bFileContent = NULL;
    DWORD dwFileLen = 0;
    char pDownloadUrl[256] = { 0 };
    strcpy(pDownloadUrl, m_csDownloadUrl);
    if (NET_ESS_ClientDoDownload(lSSClientHandle, pDownloadUrl, (void**)&bFileContent, dwFileLen))
    {
        char strFilePath[MAX_PATH] = { 0 };
        CString strFilePathTmp(m_csFileStorePath);
        m_iFileNum++;
        if (m_cmbDlServerType.GetCurSel() == 3)
        {
            sprintf_s(strFilePath, MAX_PATH - 1, "%s\\%d_%s", strFilePathTmp.GetBuffer(), m_iFileNum, "central.mp4");  //随便起个名字存储，只是测试文件是否收到
        }
        else
        {
            sprintf_s(strFilePath, MAX_PATH - 1, "%s\\%d_%s", strFilePathTmp.GetBuffer(), m_iFileNum, "cloud.jpg");  //随便起个名字存储，只是测试文件是否收到
        }

        FILE* pFile = fopen(strFilePath, "wb+");
        if (pFile != NULL)
        {

            DWORD dwWriteLen = fwrite(bFileContent, 1, (unsigned int)dwFileLen, pFile);

            fclose(pFile);
            if (dwWriteLen != (DWORD)dwFileLen)
            {
#ifdef DEMO_LAN_EN
                AfxMessageBox("dwWriteLen != (DWORD)dwFileLen fail!");
#else
                AfxMessageBox("写文件长度和文件长度不符 失败");
#endif
                NET_ESS_DestroyClient(lSSClientHandle);
                return;
            }
            g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_SUCC_T, 4, "NET_ESS_ClientDoDownload, success");
            NET_ESS_DestroyClient(lSSClientHandle);
            return;
        }
        else
        {
#ifdef DEMO_LAN_EN
            AfxMessageBox("open file fail");
#else
            AfxMessageBox("打开文件失败");
#endif
            NET_ESS_DestroyClient(lSSClientHandle);
            return;
        }
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 4, "NET_ESS_ClientDoDownload, Error[%d]", NET_ESS_GetLastError());
        NET_ESS_DestroyClient(lSSClientHandle);
        return;
    }
    UpdateData(FALSE);

}


void CDlgStorageServer::OnCbnSelchangeCmbDlServertype()
{
    // TODO:  在此添加控件通知处理程序代码

}


void CDlgStorageServer::OnBnClickedBtnDowloadFile2()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);
    // TODO:  在此添加控件通知处理程序代码
    NET_EHOME_SS_CLIENT_PARAM struClientParam;
    memset(&struClientParam, 0, sizeof(struClientParam));

    if (m_bHttpsDelete)
    {
        struClientParam.byHttps = 1;  //是否启用HTTPs
    }
    else
    {
        struClientParam.byHttps = 0;  //是否启用HTTPs
    }

    memcpy(struClientParam.struAddress.szIP, m_strDelIp.GetBuffer(), m_strDelIp.GetLength());
    struClientParam.struAddress.wPort = (WORD)m_iDelPort;
    if (m_cboxDelType.GetCurSel() == 1)
    {//云存储
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_CLOUD;
    }
    else if (m_cboxDelType.GetCurSel() == 2)
    {//KMS
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_KMS;
    }
    else if (m_cboxDelType.GetCurSel() == 3)
    {//中心存储
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_CENTRAL;
    }
    else
    {//非安全
        struClientParam.enumType = NET_EHOME_SS_CLIENT_TYPE_TOMCAT;
    }

    LONG lSSClientHandle = NET_ESS_CreateClient(&struClientParam);
    if (m_cboxDelType.GetCurSel() == 1)
    {//云存储
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CLOUD_AK_NAME, m_strDelAK.GetBuffer());
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CLOUD_SK_NAME, m_strDelSK.GetBuffer());
    }
    else if (m_cboxDelType.GetCurSel() == 2)
    {//KMS
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_KMS_USER_NAME, m_strDelKMSName.GetBuffer());
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_KMS_PASSWIRD, m_strDelKMSPass.GetBuffer());
    }
    else if (m_cmbDlServerType.GetCurSel() == 3)
    {//中心存储
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CENTRAL_AK_NAME, m_strDelAK.GetBuffer());
        NET_ESS_ClientSetParam(lSSClientHandle, SS_CLIENT_CENTRAL_SK_NAME, m_strDelSK.GetBuffer());
    }
    else
    {//非安全

    }
    BOOL ret = NET_ESS_ClientDoDelete(lSSClientHandle, m_strDelUrl.GetBuffer());
    if (!ret)
    {
        DWORD error = NET_ESS_GetLastError();
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 4, "NET_ESS_ClientDoDelete, Failed[%d]", error);
    }
    else
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), OPERATION_FAIL_T, 4, "NET_ESS_ClientDoDelete, Succed");
    }

    UpdateData(FALSE);
}


void CDlgStorageServer::OnBnClickedButtonLfDownload()
{

	if (!m_bSetPath)
	{
		AfxMessageBox(_T("请先选择文件路径"));
		return;
	}
    // TODO:  在此添加控件通知处理程序代码
    //LONG iHandle = -1;
    //memcpy(&iHandle, param, sizeof(LONG));
	//while (TRUE)
	//{
	UpdateData(TRUE);
	NET_ISUP_CMS_LF_CONFIG_PARAM struConfig = { 0 };
	char szFileUrl[1024] = { 0 };
	memcpy(szFileUrl, m_csLFURL.GetBuffer(), m_csLFURL.GetLength());
	struConfig.pFileId = szFileUrl;
	struConfig.dwFileIdLen = strlen(szFileUrl);
	//struConfig.dwSequence = CreateSequence(); //管理url和sequence的对应关系
	//m_LFDownMap.insert(make_pair(struConfig.dwSequence, struConfig.pFileId));

	if (m_bLFResume)
	{
		struConfig.dwFirstTag = m_csLFStart;
		struConfig.dwLastTag = m_csLFEnd;
		NET_ISUP_CMS_LFSetParam(g_pMainDlg->m_lCmsLFHandle, FUN_DOWNLOAD_SET_PARAM, DATATYPE_RESUME_BREAKPOINT_URL, &struConfig);
	}
	else
	{
		NET_ISUP_CMS_LFSetParam(g_pMainDlg->m_lCmsLFHandle, FUN_DOWNLOAD_SET_PARAM, DATATYPE_URL, &struConfig);
	}
	NET_ISUP_CMS_LFDownload(g_pMainDlg->m_lCmsLFHandle); //handle和sessionid要有对应关系，这里简写
	//}

}

/** @fn BOOL CALLBACK CDlgStorageServer::StaticLargeFileCB(LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser)
 *  @brief
 *  @param LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser
 *  @return BOOL CALLBACK
 *  @note
 */
BOOL CALLBACK CDlgStorageServer::StaticLargeFileCB(LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser)
{
    CDlgStorageServer* pThis = (CDlgStorageServer*)pUser;
    if (pThis != NULL)
    {
        pThis->LargefileCallback(lLinkHandle, dwDataType, pOutBuffer, dwOutLen, pInBuffer, dwInLen, pUser);
    }
    return TRUE;
}

/** @fn BOOL CDlgStorageServer::LargefileCallback(LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser)
 *  @brief
 *  @param LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser
 *  @return BOOL
 *  @note
 */
BOOL CDlgStorageServer::LargefileCallback(LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser)
{
    CDlgStorageServer* pThis = (CDlgStorageServer*)pUser;
	BOOL bDownState = FALSE;
    if (pThis != NULL)
    {
        switch (dwDataType)
        {
        case DATETYPE_EXPECTION: //这里需管理handle和sessionid的关系
        {
            NET_ISUP_CMS_LFStopLink(lLinkHandle);
        }
        break;
        case DATETYPE_LINK_INFO: //这里需管理handle和sessionid的关系
        {
            NET_ISUP_CMS_LF_LINK_INFO* pLinkInfo = (NET_ISUP_CMS_LF_LINK_INFO*)pOutBuffer;
			g_pMainDlg->m_lCmsLFHandle = lLinkHandle; 
        }
        break;
        case DATATYPE_SESSION_ID: //这里需管理handle和sessionid的关系
        {
            NET_ISUP_CMS_LF_SESSION_INFO* pSessionInfo = (NET_ISUP_CMS_LF_SESSION_INFO*)pOutBuffer;
        }
        break;
        case DATETYPE_UPLOAD_RESULT:
        {
            NET_ISUP_CMS_LF_UPLOAD_RSP_INFO* pUploadInfo = (NET_ISUP_CMS_LF_UPLOAD_RSP_INFO*)pOutBuffer;
            printf("DATETYPE_UPLOAD_RESULT handle[%d] result[%d] URL[%s] sequence[%d]\r\n",lLinkHandle, pUploadInfo->dwResult, pUploadInfo->pFileURI, pUploadInfo->dwSequence);
        }
        break;
        case DATETYPE_DOWNLOAD:
        {
            NET_ISUP_CMS_LF_DOWNLOAD_DATA_INFO* pDownloadInfo = (NET_ISUP_CMS_LF_DOWNLOAD_DATA_INFO*)pOutBuffer;
// 			UpdateData(FALSE);
			if (!bDownState)
			{
				bDownState = TRUE;
				char szPath[1024] = { 0 };
				memcpy(szPath, m_csLFPath.GetBuffer(), m_csLFPath.GetLength());
				char szFileName[2048] = { 0 };
				sprintf(szFileName, "%s\\SDK_%d.jpg", szPath, pDownloadInfo->dwSequence); //这里需对应sequence和url的关系
				m_pFile = fopen(szFileName, "wb+");
			}
			if (m_pFile != NULL)
			{
				fwrite(pDownloadInfo->pbuf, 1, pDownloadInfo->dwbufLen, m_pFile);
			}
            //pDownloadInfo->pbuf

        }
        break;
        case DATETYPE_DOWNLOAD_END:
        {
			
            NET_ISUP_CMS_LF_DWONLOAD_RSP_INFO* pDownRspInfo = (NET_ISUP_CMS_LF_DWONLOAD_RSP_INFO*)pOutBuffer;
			bDownState = FALSE;
			if (pDownRspInfo->dwResult == 0)
			{
				fclose(m_pFile);
				m_pFile = NULL;
			}
			else
			{
				printf("CDlgStorageServer::LargefileCallback DATETYPE_DOWNLOAD_END, result[%d] sequence[%d]", pDownRspInfo->dwResult, pDownRspInfo->dwSequence);
			}

        }
        break;
        default:
            break;
        }
    }

    return TRUE;
}

/** @fn DWORD WINAPI CDlgStorageServer::LFDownThreadFunc(PVOID param)
 *  @brief
 *  @param PVOID param
 *  @return DWORD WINAPI
 *  @note
 */
DWORD WINAPI CDlgStorageServer::LFDownThreadFunc(PVOID param)
{
    return -1;
}


void CDlgStorageServer::OnBnClickedButtonLfCreateServer()
{
    // TODO:  在此添加控件通知处理程序代码
	if (g_pMainDlg->m_lCMSLFListenHandle > -1)
	{
		AfxMessageBox(_T("请先停止服务"));
		return;
	}
	UpdateData(TRUE);
    NET_EHOME_IPADDRESS struAddr = { 0 };
    memcpy(struAddr.szIP, m_csLFIP.GetBuffer(), m_csLFIP.GetLength());
    struAddr.wPort = m_csLFPort;
    NET_ISUP_CMS_LF_LISTEN_PARAM struListenParam = { 0 };
    memcpy((void*)&struListenParam.struAddress, &struAddr, sizeof(NET_EHOME_IPADDRESS));
    struListenParam.fnCB = StaticLargeFileCB;
    struListenParam.pUserData = this;

    
	LONG lLFlistenHandle = NET_ISUP_CMS_LFStartListen(&struListenParam);
	if (lLFlistenHandle < 0)
	{
		AfxMessageBox("创建失败");
	}
	else
	{
		g_pMainDlg->m_lCMSLFListenHandle = lLFlistenHandle;
		AfxMessageBox("创建成功");
	}
// 	if (g_pMainDlg->m_lCMSLFListenHandle != -1)
// 	{
// 		CreateThread();
// 	}
}


void CDlgStorageServer::OnBnClickedButtonLfUpload()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_bSetPath)
	{
		AfxMessageBox(_T("请先选择文件路径"));
		return;
	}

	UpdateData(TRUE);
	NET_ISUP_CMS_LF_CONFIG_PARAM struConfig = { 0 };
	char szFileUrl[1024] = { 0 };
	memcpy(szFileUrl, m_csLFURL.GetBuffer(), m_csLFURL.GetLength());
	struConfig.pFileId = szFileUrl;
	struConfig.dwFileIdLen = strlen(szFileUrl);
	//struConfig.dwSequence = CreateSequence();  //管理url和sequence的对应关系
	if (m_bLFResume)
	{
		struConfig.dwFirstTag = m_csLFStart;
		struConfig.dwLastTag = m_csLFEnd;
		NET_ISUP_CMS_LFSetParam(g_pMainDlg->m_lCmsLFHandle, FUN_UPLOAD_SET_PARAM, DATATYPE_RESUME_BREAKPOINT_URL, &struConfig);
	}
	else
	{
		NET_ISUP_CMS_LFSetParam(g_pMainDlg->m_lCmsLFHandle, FUN_UPLOAD_SET_PARAM, DATATYPE_URL, &struConfig);
	}
	//m_dwSequence = *struConfig.pSequence;
	NET_ISUP_CMS_LF_UPLOAD_PARAM struUpload = { 0 };
	char szPath[1024] = { 0 };
	//memcpy(szPath, m_csLFPath.GetBuffer(), m_csLFPath.GetLength()); 
	sprintf(szPath, "%s/download.jpg", m_csLFPath.GetBuffer());
	m_pFile = fopen(szPath, "rb+");
 	if (m_pFile == NULL)
 	{
 		return;
 	}
	char* pUpBuf = new char[2*1024*1024];
	

	int iBufLen = fread(pUpBuf, 1, 2 * 1024 * 1024, m_pFile);
	struUpload.pbuf = pUpBuf;
	struUpload.dwbufLen = iBufLen;
	struUpload.byTransStage = 0x11;
	NET_ISUP_CMS_LFUpload(g_pMainDlg->m_lCmsLFHandle, &struUpload); 
	delete[] pUpBuf;
}
/** @fn DWORD CDlgStorageServer::CreateSequence()
 *  @brief
 *  @param
 *  @return DWORD
 *  @note
 */
DWORD CDlgStorageServer::CreateSequence()
{
	m_dwSequence += 1;
	return m_dwSequence;
}


void CDlgStorageServer::OnBnClickedButtonLfCreateChannel()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	NET_EHOME_PTXML_PARAM struISAPIJSON = { 0 };
	char szRequestUrl[64] = { 0 };
	sprintf(szRequestUrl, "PUT /ISAPI/ISUPTransport/ISUPFileChannel?format=json");
	struISAPIJSON.pRequestUrl = szRequestUrl;
	struISAPIJSON.dwRequestUrlLen = strlen(szRequestUrl);
	char szRequestBody[512] = { 0 };
	sprintf(szRequestBody, "{\r\n"
		"\"ISUPFileChannel\": {\r\n"
		"\"address\": \"%s\",\r\n"
		"\"port\" : %d,\r\n"
		"\"sessionid\" : \"%s\"}}",
		m_csLFIP.GetBuffer(),
		m_csLFPort,
		m_csLFSessionId.GetBuffer());

	struISAPIJSON.pInBuffer = szRequestBody;
	struISAPIJSON.dwInSize = strlen(szRequestBody);
	char szOutput[1024 * 10] = { 0 };
	struISAPIJSON.pOutBuffer = szOutput;
	struISAPIJSON.dwOutSize = sizeof(szOutput);

	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;

	if (NET_ECMS_ISAPIPassThrough(m_lUserID, &struISAPIJSON))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_ISAPIPassThrough");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "%s", szOutput);
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_ISAPIPassThrough");
		return;
	}

	//AfxMessageBox(_T("%s", sOutput));

}


void CDlgStorageServer::OnBnClickedButtonLfDelChannel()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (NET_ISUP_CMS_LFStopLink(g_pMainDlg->m_lCmsLFHandle))
	{
		AfxMessageBox("关闭通道成功");
	}
	else
	{
		AfxMessageBox("关闭通道失败");
	}

}


void CDlgStorageServer::OnBnClickedBtnStorepathLf()
{
	// TODO:  在此添加控件通知处理程序代码
	m_bSetPath = TRUE;
	UpdateData(TRUE);
	CString  FullPath;
	BROWSEINFO bi;                        //BROWSEINFO结构体  
	TCHAR Buffer[512] = "";
	TCHAR FullPaths[512] = "";
	bi.hwndOwner = m_hWnd;                    //m_hWnd你的程序主窗口  
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;                //返回选择的目录名的缓冲区  
	bi.lpszTitle = "请选择路径";            //弹出的窗口的文字提示  
	bi.ulFlags = NULL;//BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_BROWSEFORCOMPUTER ;    //只返回目录。其他标志看MSDN  
	bi.lpfn = NULL;                        //回调函数，有时很有用  
	bi.lParam = 0;
	bi.iImage = 0;
	ITEMIDLIST* pidl = ::SHBrowseForFolder(&bi);        //显示弹出窗口，ITEMIDLIST很重要  
	::SHGetPathFromIDList(pidl, FullPaths);        //在ITEMIDLIST中得到目录名的整个路径  
	if (FullPaths[0] != NULL)
	{
		*(&m_csLFPath) = FullPaths;
		UpdateData(FALSE);
	}

}


void CDlgStorageServer::OnBnClickedButtonLfStoplf()
{
	// TODO:  在此添加控件通知处理程序代码
	if (NET_ISUP_CMS_LFStopListen(g_pMainDlg->m_lCMSLFListenHandle))
	{
		g_pMainDlg->m_lCMSLFListenHandle = -1;
		AfxMessageBox(_T("停止服务成功"));
	}
	else
	{
		AfxMessageBox(_T("停止服务失败"));
	}
}
