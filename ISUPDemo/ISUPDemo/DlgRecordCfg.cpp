// DlgRecordCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgRecordCfg.h"
#include "afxdialogex.h"


// CDlgRecordCfg 对话框

IMPLEMENT_DYNAMIC(CDlgRecordCfg, CDialogEx)

CDlgRecordCfg::CDlgRecordCfg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRecordCfg::IDD, pParent)
    , m_iDay(0)
    , m_iDelay(0)
    , m_iPre(0)
    , m_iSaveTime(0)
    , m_iRecordType(0)
    , m_bEnable(FALSE)
    , m_bRedunce(FALSE)
    , m_bAudio(FALSE)
    , m_bAllDay(FALSE)
{
    memset(m_iRecord, 0, sizeof(m_iRecord));
    memset(m_iStartH, 0, sizeof(m_iStartH));
    memset(m_iStartM, 0, sizeof(m_iStartM)); 
    memset(m_iStopH, 0, sizeof(m_iStopH));
    memset(m_iStopM, 0, sizeof(m_iStopM));
    memset(m_arrRecCfg, 0, sizeof(m_arrRecCfg));

}

CDlgRecordCfg::~CDlgRecordCfg()
{
}

void CDlgRecordCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_CBIndex(pDX, IDC_COMBO1_DAY, m_iDay);
    DDX_CBIndex(pDX, IDC_COMBO1_DAY2, m_iDelay);
    DDX_CBIndex(pDX, IDC_COMBO1_DAY3, m_iPre);
    DDX_Text(pDX, IDC_EDIT1, m_iSaveTime);
    DDX_CBIndex(pDX, IDC_COMBO1_DAY4, m_iRecordType);
    DDX_Check(pDX, IDC_CHECK1, m_bEnable);
    DDX_Check(pDX, IDC_CHECK2, m_bRedunce);
    DDX_Check(pDX, IDC_CHECK3, m_bAudio);
    DDX_Check(pDX, IDC_CHECK4, m_bAllDay);
    DDX_CBIndex(pDX, IDC_COMBO_RECORD1, m_iRecord[0]);
    DDX_CBIndex(pDX, IDC_COMBO_RECORD2, m_iRecord[1]);
    DDX_CBIndex(pDX, IDC_COMBO_RECORD3, m_iRecord[2]);
    DDX_CBIndex(pDX, IDC_COMBO_RECORD4, m_iRecord[3]);
    DDX_Text(pDX, IDC_EDIT3, m_iStartH[0]);
    DDX_Text(pDX, IDC_EDIT5, m_iStartH[1]);
    DDX_Text(pDX, IDC_EDIT7, m_iStartH[2]);
    DDX_Text(pDX, IDC_EDIT9, m_iStartH[3]);
    DDX_Text(pDX, IDC_EDIT4, m_iStartM[0]);
    DDX_Text(pDX, IDC_EDIT6, m_iStartM[1]);
    DDX_Text(pDX, IDC_EDIT8, m_iStartM[2]);
    DDX_Text(pDX, IDC_EDIT10, m_iStartM[3]);
    DDX_Text(pDX, IDC_EDIT11, m_iStopH[0]);
    DDX_Text(pDX, IDC_EDIT13, m_iStopH[1]);
    DDX_Text(pDX, IDC_EDIT15, m_iStopH[2]);
    DDX_Text(pDX, IDC_EDIT17, m_iStopH[3]);
    DDX_Text(pDX, IDC_EDIT12, m_iStopM[0]);
    DDX_Text(pDX, IDC_EDIT14, m_iStopM[1]);
    DDX_Text(pDX, IDC_EDIT16, m_iStopM[2]);
    DDX_Text(pDX, IDC_EDIT18, m_iStopM[3]);
    DDX_Control(pDX, IDC_COMBO1_DAY4, m_cmbRecordType);
    DDX_Control(pDX, IDC_COMBO_RECORD1, m_arrcmbRecordType[0]);
    DDX_Control(pDX, IDC_COMBO_RECORD2, m_arrcmbRecordType[1]);
    DDX_Control(pDX, IDC_COMBO_RECORD3, m_arrcmbRecordType[2]);
    DDX_Control(pDX, IDC_COMBO_RECORD4, m_arrcmbRecordType[3]);
}


BEGIN_MESSAGE_MAP(CDlgRecordCfg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_SET_RECORD, &CDlgRecordCfg::OnBnClickedBtnSetRecord)
    ON_BN_CLICKED(IDC_BTN_GET_RECORD, &CDlgRecordCfg::OnBnClickedBtnGetRecord)
END_MESSAGE_MAP()


// CDlgRecordCfg 消息处理程序



BOOL CDlgRecordCfg::CheckInitParam()
{
    // TODO:  在此添加额外的初始化
    char szLan[1024] = { 0 };
    m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (m_iDeviceIndex < 0)
    {
        g_StringLanType(szLan, "请先选择一个设备!", "please select a device first!");
        g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, 0, szLan);
        return FALSE;
    }
    m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    if (m_lLoginID < 0)
    {
        g_StringLanType(szLan, "设备未注册!", "device offline!");
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, szLan);
        return FALSE;
    }
    m_iChanIndex = g_pMainDlg->GetCurChanIndex();
    if (m_iChanIndex < 0)
    {
        //if (m_dwChannelNo < 0)
        {
            g_StringLanType(szLan, "请选择一个通道!", "please select a channel first!");
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, szLan);
            return FALSE;
        }
    }
    else
    {
        m_dwChannelNo = m_iChanIndex + 1;
    }

    return TRUE;
}

void CDlgRecordCfg::OnBnClickedBtnSetRecord()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);

    CString str;
    for (int i = 0; i < 4; i++)
    {
        CString strTemp;
        strTemp.Format("<TIME>\r\n"\
            "<RecordType>%d</RecordType>\r\n"\
            "<StartTime>%02d:%02d</StartTime>\r\n"\
            "<StopTime>%02d:%02d</StopTime>\r\n"\
            "</TIME>\r\n"
            , m_iRecord[i],m_iStartH[i],m_iStartM[i],\
            m_iStopH[i],m_iStopM[i]);
        str += strTemp;
    }
    char szRecordTime[1024] = { 0 };
    memcpy(szRecordTime, str.GetBuffer(0), str.GetLength());
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024*2] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>SetRecordPlanPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigParam2>%d</ConfigParam2>\r\n"\
        "<ConfigXML>\r\n"
        "<RECORDTIMECFG>\r\n"
        "<IsUseRecord>%d</IsUseRecord>\r\n"
        "<RecordTime>%d</RecordTime>\r\n"
        "<PreRecordTime>%d</PreRecordTime>\r\n"
        "<RecorderDuration>%d</RecorderDuration>\r\n"
        "<RedundancyRec>%d</RedundancyRec>\r\n"
        "<AudioRec>%d</AudioRec>\r\n"
        "<RECORDPLAY>\r\n"
        "<IsAllDayRecord>%d</IsAllDayRecord>\r\n"
        "<AllDayRecordType>%d</AllDayRecordType>\r\n"
        "%s"
        "</RECORDPLAY>\r\n"
        "</RECORDTIMECFG>\r\n"
        "</ConfigXML>\r\n"
        "</Params>", m_dwChannelNo, m_iDay,\
        m_bEnable,m_iDelay,m_iPre,m_iSaveTime,m_bRedunce,m_bAudio,\
        m_bAllDay,m_iRecordType,szRecordTime);
    char szCmd[32] = "SETDEVICECONFIG";
    char szOutBuffer[1024] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");
}


void CDlgRecordCfg::OnBnClickedBtnGetRecord()
{
    // TODO:  在此添加控件通知处理程序代码

    if (!CheckInitParam())
    {
        return;
    }
    memset(m_iRecord, 0, sizeof(m_iRecord));
    memset(m_iStartH, 0, sizeof(m_iStartH));
    memset(m_iStartM, 0, sizeof(m_iStartM));
    memset(m_iStopH, 0, sizeof(m_iStopH));
    memset(m_iStopM, 0, sizeof(m_iStopM));
    memset(m_arrRecCfg, 0, sizeof(m_arrRecCfg));
    UpdateData(TRUE);
    NET_EHOME_XML_CFG struXmlCfg = { 0 };
    char szInBuf[1024] = { 0 };
    sprintf(szInBuf, "<Params>\r\n"
        "<ConfigCmd>GetRecordPlanPara</ConfigCmd>\r\n"\
        "<ConfigParam1>%d</ConfigParam1>\r\n"\
        "<ConfigParam2>%d</ConfigParam2>\r\n"\
        "</Params>", m_dwChannelNo, m_iDay);
    char szCmd[32] = "GETDEVICECONFIG";
    char szOutBuffer[1024*10] = { 0 };
    struXmlCfg.pCmdBuf = szCmd;
    struXmlCfg.dwCmdLen = sizeof(szCmd);
    struXmlCfg.pInBuf = szInBuf;
    struXmlCfg.dwInSize = sizeof(szInBuf);
    struXmlCfg.pOutBuf = szOutBuffer;
    struXmlCfg.dwOutSize = sizeof(szOutBuffer);
    char szStatusBuf[1024*2] = { 0 };
    struXmlCfg.pStatusBuf = szStatusBuf;
    struXmlCfg.dwStatusSize = 1024*2;

    if (!NET_ECMS_XMLConfig(m_lLoginID, &struXmlCfg, sizeof(struXmlCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_ECMS_XMLConfig");
        return;
    }
    g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_ECMS_XMLConfig");

    char *pTmp = szOutBuffer + strlen(szOutBuffer) - 2;
    if (strcmp(pTmp, "\r\n") != 0)
    {
        memcpy(szOutBuffer + strlen(szOutBuffer), "\r\n", strlen("\r\n"));
    }
    CXmlBase    xmlBase;
    if (xmlBase.Parse(szOutBuffer) != NULL)
    {

        if (xmlBase.FindElem("Params") && xmlBase.IntoElem())
        {
            if (xmlBase.FindElem("ConfigXML") && xmlBase.IntoElem())
            {
                if (xmlBase.FindElem("RECORDTIMECFG") && xmlBase.IntoElem())
                {
                    ConvertSingleNodeData(&m_bEnable, xmlBase, "IsUseRecord", NODE_STRING_TO_INT);
                    ConvertSingleNodeData(&m_iDelay, xmlBase, "RecordTime", NODE_STRING_TO_INT);
                    ConvertSingleNodeData(&m_iPre, xmlBase, "PreRecordTime", NODE_STRING_TO_INT);
                    ConvertSingleNodeData(&m_iSaveTime, xmlBase, "RecorderDuration", NODE_STRING_TO_INT);
                    ConvertSingleNodeData(&m_bRedunce, xmlBase, "RedundancyRec", NODE_STRING_TO_INT);
                  
                    ConvertSingleNodeData(&m_bAudio, xmlBase, "AudioRec", NODE_STRING_TO_INT);
                    if (xmlBase.FindElem("RECORDPLAY") && xmlBase.IntoElem())
                    {
                        ConvertSingleNodeData(&m_bAllDay, xmlBase, "IsAllDayRecord", NODE_STRING_TO_INT);
                        ConvertSingleNodeData(&m_iRecordType, xmlBase, "AllDayRecordType", NODE_STRING_TO_INT);

                        int i = 0;
                        do
                        {
                            if (xmlBase.FindElem("TIME") && xmlBase.IntoElem())
                            {
                                char szStartTime[64] = { 0 };
                                char szStopTime[64] = { 0 };
                                ConvertSingleNodeData(&m_arrRecCfg[i].iRecordType, xmlBase, "RecordType", NODE_STRING_TO_INT);
                                ConvertSingleNodeData(szStartTime, xmlBase, "StartTime", NODE_STRING_TO_ARRAY,64);
                                sscanf(szStartTime, "%d:%d", &m_arrRecCfg[i].iStartHour, &m_arrRecCfg[i].iStartMin);
                                ConvertSingleNodeData(szStopTime, xmlBase, "StopTime", NODE_STRING_TO_ARRAY,64);
                                sscanf(szStopTime, "%d:%d", &m_arrRecCfg[i].iStopHour, &m_arrRecCfg[i].iStopMin);
                                m_iRecord[i] = m_arrRecCfg[i].iRecordType;
                                m_iStartH[i] = m_arrRecCfg[i].iStartHour;
                                m_iStartM[i] = m_arrRecCfg[i].iStartMin;
                                m_iStopH[i] = m_arrRecCfg[i].iStopHour;
                                m_iStopM[i] = m_arrRecCfg[i].iStopMin;

                                xmlBase.OutOfElem();
                                i++;
                            }
                        } while (xmlBase.NextSibElem());
                    }

                    UpdateData(FALSE);
                    return;
                }

            }
        }
    }
    MessageBox("Parase xml failed");
}


BOOL CDlgRecordCfg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化

    int index = 0;
    m_cmbRecordType.ResetContent();
    char szLan[128] = { 0 };
    g_StringLanType(szLan, "定时录像", "Schedule");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_TIMING);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "移动侦测", "Motion Detect");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_MOTION_DETECT);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "报警触发", "Alarm");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_ALARMIN);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "报警|动测", "Alarm|Motion");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_MOTION_OR_ALARMIN);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "报警&动测", "Alarm&Motion");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_MOTION_AND_ALARMIN);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "命令触发", "Command Record");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_COMMAND);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "手动录像", "Manual Record");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_MANUAL);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "震动报警", "Shake Alarm");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_SHAKE_ALARM);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "环境报警", "Environment Alarm");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_ENVIRONMENT_ALARM);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "智能报警", "VCA Alarm");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_VCA_ALARM);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "PIR报警", "PIR Alarm");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_PIR_ALARM);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "无线报警", "WIRELESS Alarm");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_WIRELESS_ALARM);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "呼救报警", "CALLHELP Alarm");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_CALLHELP_ALARM);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    g_StringLanType(szLan, "全部报警录像", "Alarm Record");
    m_cmbRecordType.InsertString(index, szLan);
    m_cmbRecordType.SetItemData(index, FILE_ALL_ALARM);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].InsertString(index, szLan);
        m_arrcmbRecordType[i].SetItemData(index, FILE_TIMING);
    }
    index++;

    m_cmbRecordType.SetCurSel(0);
    for (int i = 0; i < 4; i++)
    {
        m_arrcmbRecordType[i].SetCurSel(0);
    }
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}
