// DlgISAPIPassthrough.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgISAPIPassthrough.h"
#include "Public/Convert/Convert.h"
#include "afxdialogex.h"


// CDlgISAPIPassthrough �Ի���

IMPLEMENT_DYNAMIC(CDlgISAPIPassthrough, CDialog)

CDlgISAPIPassthrough::CDlgISAPIPassthrough(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgISAPIPassthrough::IDD, pParent)
    , m_sCond(_T(""))
    , m_sInput(_T(""))
    , m_sOutput(_T(""))
    , m_sUrl(_T(""))
    , m_dwTimeout(5000)
{
    m_lUserID = -1;
    m_iDeviceIndex = -1;
}

CDlgISAPIPassthrough::~CDlgISAPIPassthrough()
{
}

void CDlgISAPIPassthrough::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_CMD_TYPE, m_cmbCmdType);
    DDX_Text(pDX, IDC_EDIT_COND, m_sCond);
    DDX_Text(pDX, IDC_EDIT_INPUT, m_sInput);
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_sOutput);
    DDX_Text(pDX, IDC_EDIT_URL, m_sUrl);
    DDX_Text(pDX, IDC_EDIT2, m_dwTimeout);
}


BEGIN_MESSAGE_MAP(CDlgISAPIPassthrough, CDialog)
    ON_BN_CLICKED(IDC_BTN_COMMAND, &CDlgISAPIPassthrough::OnClickedBtnCommand)
END_MESSAGE_MAP()


// CDlgISAPIPassthrough ��Ϣ�������

#include <io.h>
void CDlgISAPIPassthrough::OnClickedBtnCommand()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);

    int iCmdType = m_cmbCmdType.GetCurSel();

    NET_EHOME_PTXML_PARAM struPTXML = { 0 };
    struPTXML.pRequestUrl = m_sUrl.GetBuffer(0);
    struPTXML.dwRequestUrlLen = m_sUrl.GetLength();

    char *sInput = new char[512 * 1024];
    char *sCond = new char[512 * 1024];
    char *sOutput = new char[512 * 1024];

    memset(sInput, 0, 512 * 1024);
    memset(sCond, 0, 512 * 1024);
    memset(sOutput, 0, 512 * 1024);

    sprintf(sInput, "%s", m_sInput);
    sprintf(sCond, "%s", m_sCond);

    //����8K�����ݲ���
//     HANDLE handle = CreateFile("E:\\Task2018\\EHomeSDK2.0.1\\common\\HCEHomeCMS\\win\\lib64\\IsapiTestData.xml", 
//         GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
//     int nFileSize = 0;
//     DWORD dwRSize = 0;
//     if (handle != INVALID_HANDLE_VALUE)
//     {
//         nFileSize = GetFileSize(handle, NULL);
//         ReadFile(handle, sInput, GetFileSize(handle, NULL), &dwRSize, NULL);
//         CloseHandle(handle);
//     }

    struPTXML.pCondBuffer = sCond;
    struPTXML.dwCondSize = (DWORD)strlen(sCond);
    struPTXML.dwInSize = (DWORD)strlen(sInput);

    if (struPTXML.dwInSize == 0)
    {
        struPTXML.pInBuffer = NULL;
    }
    else
    {
        struPTXML.pInBuffer = sInput;
    }

    struPTXML.pOutBuffer = sOutput;
    struPTXML.dwOutSize = 512 * 1024;
    struPTXML.dwRecvTimeOut = m_dwTimeout;

    //��Ҫ���ַ����ֶ�ת����UTF-8
    //A2UTF8((char*)sInput, strlen(sInput), (char*)struPTXML.pInBuffer, 512 * 1024, &struPTXML.dwInSize); 
    if (struPTXML.dwInSize != 0)
    {
        if (!A2UTF8((char*)struPTXML.pInBuffer, (char*)struPTXML.pInBuffer, 512 * 1024, &struPTXML.dwInSize))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_GetPTXMLConfig A2UTF8 failed");
            return;
        }
    }
  
    if (struPTXML.dwCondSize != 0)
    {
        if (!A2UTF8((char*)struPTXML.pCondBuffer, (char*)struPTXML.pCondBuffer, 512 * 1024, &struPTXML.dwCondSize))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_GetPTXMLConfig A2UTF8 failed");
            return;
        }
    }
    

    if (iCmdType == 0) //GET
    {
        if (NET_ECMS_GetPTXMLConfig(m_lUserID, &struPTXML))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_GetPTXMLConfig");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_GetPTXMLConfig");
            //delete[] sInput;
            //delete[] sCond;
            //delete[] sOutput;
            //return;
        }
    }
    else if (iCmdType == 1) //PUT
    {
        if (NET_ECMS_PutPTXMLConfig(m_lUserID, &struPTXML))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_PutPTXMLConfig");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_PutPTXMLConfig");
    /*        delete[] sInput;
            delete[] sCond;
            delete[] sOutput;
            return;*/
        }
    }
    else if (iCmdType == 2) //POST
    {
        if (NET_ECMS_PostPTXMLConfig(m_lUserID, &struPTXML))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_PostPTXMLConfig");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_PostPTXMLConfig");
            //delete[] sInput;
            //delete[] sCond;
            //delete[] sOutput;
            //return;
        }
    }
    else if (iCmdType == 3) //DELETE
    {
        if (NET_ECMS_DeletePTXMLConfig(m_lUserID, &struPTXML))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_DeletePTXMLConfig");
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_DeletePTXMLConfig");
            //delete[] sInput;
            //delete[] sCond;
            //delete[] sOutput;
            //return;
        }
    }
    else
    {
        delete[] sInput;
        delete[] sCond;
        delete[] sOutput;

        AfxMessageBox("��ѡ����������");
        return;
    }
    //��Ҫ���ַ����ֶ�ת����GB2312
    UTF82A((char*)sOutput, (char*)sOutput, struPTXML.dwOutSize, &struPTXML.dwOutSize);
    m_sOutput = sOutput;

    delete[] sInput;
    delete[] sCond;
    delete[] sOutput;

    UpdateData(FALSE);
}


BOOL CDlgISAPIPassthrough::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (m_iDeviceIndex < 0)
    {
        AfxMessageBox("��ѡ��һ���豸");
        return TRUE;
    }
    m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    if (m_lUserID < 0)
    {
        AfxMessageBox("���ȵ�½�豸");
        return TRUE;
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}
