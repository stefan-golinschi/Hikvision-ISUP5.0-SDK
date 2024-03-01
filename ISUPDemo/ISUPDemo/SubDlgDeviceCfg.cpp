// SubDlgDeviceCfg.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "SubDlgDeviceCfg.h"
#include "Public/Convert/Convert.h"


// CSubDlgDeviceCfg dialog

IMPLEMENT_DYNAMIC(CSubDlgDeviceCfg, CDialog)

CSubDlgDeviceCfg::CSubDlgDeviceCfg(CWnd* pParent /*=NULL*/)
: CDialog(CSubDlgDeviceCfg::IDD, pParent)
, m_iDeviceIndex(-1)
, m_lLoginID(-1)
, m_strDeviceName(_T(""))
, m_bCycleRecord(FALSE)
, m_iAnalogChanNum(0)
, m_iTotalChanNum(0)
, m_iStartChanNum(0)
, m_iDiskNum(0)
, m_iVoiceNum(0)
, m_strDeviceType(_T(""))
, m_iRS232Num(0)
, m_iRS485Num(0)
, m_iNetportNum(0)
, m_iAuxoutNum(0)
, m_iAlarmInputNum(0)
, m_iTotalAlarmInputNum(0)
, m_iAlarmOutputNum(0)
, m_iTotalAlarmOutputNum(0)
, m_iZeroChanNum(0)
, m_iZeroStartChanNum(0)
, m_iMaxDigitalNum(0)
, m_strSIMSerialNO(_T(""))
, m_iDeviceNO(0)
{
    memset(&m_struDevInfo, 0, sizeof(m_struDevInfo));
    m_struDevInfo.dwSize = sizeof(m_struDevInfo);
    memset(&m_struDevCfg, 0, sizeof(m_struDevCfg));
    m_struDevCfg.dwSize = sizeof(m_struDevCfg);
    memset(&m_struVerInfo, 0 ,sizeof(m_struVerInfo));
    m_struVerInfo.dwSize = sizeof(m_struVerInfo);
}

CSubDlgDeviceCfg::~CSubDlgDeviceCfg()
{
}

void CSubDlgDeviceCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDT_DEVICE_NAME, m_strDeviceName);
    DDX_Check(pDX, IDC_CK_CYCLE_RECORD, m_bCycleRecord);
    DDX_Text(pDX, IDC_EDT_ANALOGCHAN_NUM, m_iAnalogChanNum);
    DDX_Text(pDX, IDC_EDT_TOTALCHAN_NUM, m_iTotalChanNum);
    DDX_Text(pDX, IDC_EDT_DISK_NUM, m_iDiskNum);
    DDX_Text(pDX, IDC_EDT_VOICECHAN_NUM, m_iVoiceNum);
    DDX_Control(pDX, IDC_CMB_AUDIO_TYPE, m_cmbAudioType);
    DDX_Text(pDX, IDC_EDIT_DEVICE_TYPE, m_strDeviceType);
    DDX_Text(pDX, IDC_EDIT_STARTCHAN_NUM, m_iStartChanNum);
    DDX_Text(pDX, IDC_EDIT_STARTCHAN_NUM, m_iStartChanNum);
    DDX_Text(pDX, IDC_EDIT_CELLPHONE_NO, m_strCellphoneNO);
    DDX_Text(pDX, IDC_EDIT_ALARMINPUT_NUM, m_iAlarmInputNum);
    DDX_Text(pDX, IDC_EDIT_ALARMOUTPUT_NUM, m_iAlarmOutputNum);
    DDX_Text(pDX, IDC_EDT_DISK_NUM, m_iDiskNum);
    DDX_Text(pDX, IDC_EDIT_ZEROCHAN_NUM, m_iZeroChanNum);
    DDX_Text(pDX, IDC_EDT_DEVICE_SERIAL_NO, m_strDeviceSerialNO);
    DDX_Text(pDX, IDC_EDT_RS232_NUM, m_iRS232Num);
    DDX_Text(pDX, IDC_EDT_RS485_NUM, m_iRS485Num);
    DDX_Text(pDX, IDC_EDT_NETPORT_NUM, m_iNetportNum);
    DDX_Text(pDX, IDC_EDT_AUXOUT_NUM, m_iAuxoutNum);
    DDX_Control(pDX, IDC_CMB_MAJOR_SCALE, m_cmbMajorScale);
    DDX_Control(pDX, IDC_COMBO_MINOR_SCALE, m_cmbMinorScale);
    DDX_Text(pDX, IDC_EDIT_TOTALALARMINPUT_NUM, m_iTotalAlarmInputNum);
    DDX_Text(pDX, IDC_EDIT_TOTALALARMOUTPUT_NUM, m_iTotalAlarmOutputNum);
    DDX_Text(pDX, IDC_EDIT_ZEROCHAN_STARTNUM, m_iZeroStartChanNum);
    DDX_Text(pDX, IDC_EDIT_MAXDIGITAL_NUM, m_iMaxDigitalNum);
    DDX_Text(pDX, IDC_EDIT_SIM_SERIAL_NO, m_strSIMSerialNO);
    DDX_Text(pDX, IDC_EDIT_DEVICE_NO, m_iDeviceNO);
    DDX_Control(pDX, IDC_COMBO_INTELLIGENT_TYPE, m_cmbSmartType);
	DDV_MaxChars(pDX, m_strDeviceName, 32);
}


BEGIN_MESSAGE_MAP(CSubDlgDeviceCfg, CDialog)
END_MESSAGE_MAP()


// CSubDlgDeviceCfg message handlers

BOOL CSubDlgDeviceCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    CRect rc(0, 0, 0, 0);
    GetParent()->GetClientRect(&rc);
    ((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
    MoveWindow(&rc);
    AddAudioType();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSubDlgDeviceCfg::CheckInitParam()
{
    char szLan[1024] = {0};
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
    return TRUE;
}
void CSubDlgDeviceCfg::CurCfgUpdate()
{
    if(!CheckInitParam())
    {
        return;
    }
    NET_EHOME_CONFIG struCfg = {0};

    //获取版本信息
    struCfg.pOutBuf = &m_struVerInfo;
    struCfg.dwOutSize = sizeof(m_struVerInfo);
    memset(&m_struVerInfo, 0, sizeof(m_struVerInfo));
    m_struVerInfo.dwSize = sizeof(m_struVerInfo);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_VERSION_INFO, &struCfg, sizeof(struCfg)))
    {
        //需要将字符串字段转换成GB2312
        DWORD dwConvertLen = 0;
        UTF82A((char*)m_struVerInfo.sSoftwareVersion, (char*)m_struVerInfo.sSoftwareVersion, MAX_VERSION_LEN, &dwConvertLen);
        UTF82A((char*)m_struVerInfo.sDSPSoftwareVersion, (char*)m_struVerInfo.sDSPSoftwareVersion, MAX_VERSION_LEN, &dwConvertLen);
        UTF82A((char*)m_struVerInfo.sPanelVersion, (char*)m_struVerInfo.sPanelVersion, MAX_VERSION_LEN, &dwConvertLen);
        UTF82A((char*)m_struVerInfo.sHardwareVersion, (char*)m_struVerInfo.sHardwareVersion, MAX_VERSION_LEN, &dwConvertLen);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_VERSION_INFO");
        GetDlgItem(IDC_SUB_SOFTWARE_VERSION)->SetWindowText((char*)m_struVerInfo.sSoftwareVersion);
        GetDlgItem(IDC_SUB_HARDWARE_VERSION)->SetWindowText((char*)m_struVerInfo.sHardwareVersion);
        GetDlgItem(IDC_SUB_DSP_VERSION)->SetWindowText((char*)m_struVerInfo.sDSPSoftwareVersion);
        GetDlgItem(IDC_SUB_QMB_VERSION)->SetWindowText((char*)m_struVerInfo.sPanelVersion);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_VERSION_INFO");
    }

    //获取设备基本信息
    struCfg.pOutBuf = &m_struDevInfo;
    struCfg.dwOutSize = sizeof(m_struDevInfo);
    memset(&m_struDevInfo, 0, sizeof(m_struDevInfo));
    m_struDevInfo.dwSize = sizeof(m_struDevInfo);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_DEVICE_INFO, &struCfg, sizeof(struCfg)))
    {
        //需要将字符串字段转换成GB2312
        DWORD dwConvertLen = 0;
        UTF82A((char*)m_struDevInfo.sSerialNumber, (char*)m_struDevInfo.sSerialNumber, MAX_SERIALNO_LEN, &dwConvertLen);
        UTF82A((char*)m_struDevInfo.sSIMCardSN, (char*)m_struDevInfo.sSIMCardSN, MAX_SERIALNO_LEN, &dwConvertLen);
        UTF82A((char*)m_struDevInfo.sSIMCardPhoneNum, (char*)m_struDevInfo.sSIMCardPhoneNum, MAX_PHOMENUM_LEN, &dwConvertLen);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_DEVICE_INFO");
        m_strDeviceType.Format("%d", m_struDevInfo.dwDevType);
        //m_strDeviceType = GetDVRType(m_struDevInfo.dwDevType);
        m_iTotalChanNum = m_struDevInfo.dwChannelAmount;
        m_iStartChanNum = m_struDevInfo.dwStartChannel;
        m_iDiskNum = m_struDevInfo.dwDiskNumber;
        m_strCellphoneNO = m_struDevInfo.sSIMCardPhoneNum;
        m_iVoiceNum = m_struDevInfo.dwAudioChanNum;
        m_iAlarmInputNum = m_struDevInfo.dwAlarmInPortNum;
        m_iAlarmOutputNum = m_struDevInfo.dwAlarmOutPortNum;
        m_iTotalAlarmInputNum= m_struDevInfo.dwAlarmInAmount;
        m_iTotalAlarmOutputNum = m_struDevInfo.dwAlarmOutAmount;
        m_iZeroChanNum = m_struDevInfo.dwSupportZeroChan;
        if (0 == m_struDevInfo.dwStartZeroChan)
        {
            m_iZeroStartChanNum = 10000;
        }
        else
        {
            m_iZeroStartChanNum = m_struDevInfo.dwStartZeroChan;
        }
        m_iAnalogChanNum = m_struDevInfo.dwChannelNumber;
        m_iMaxDigitalNum = m_struDevInfo.dwMaxDigitChannelNum;
        m_strDeviceSerialNO = m_struDevInfo.sSerialNumber;
        m_strSIMSerialNO = m_struDevInfo.sSIMCardSN;
        //m_cmbAudioType.SetCurSel(m_struDevInfo.dwAudioEncType);
        //m_struDevInfo.dwAudioEncType = (BYTE)(m_cmbAudioType.GetItemData(m_cmbAudioType.GetCurSel()));
        SetResolutionCurSel(m_struDevInfo.dwAudioEncType);
        m_cmbSmartType.SetCurSel(m_struDevInfo.dwSmartType);
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_DEVICE_INFO");
    }

    //获取设备配置信息
    struCfg.pOutBuf = &m_struDevCfg;
    struCfg.dwOutSize = sizeof(m_struDevCfg);
    memset(&m_struDevCfg, 0, sizeof(m_struDevCfg));
    m_struDevCfg.dwSize = sizeof(m_struDevInfo);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_DEVICE_CFG, &struCfg, sizeof(struCfg)))
    {
        //需要将字符串字段转换成GB2312
        DWORD dwConvertLen = 0;
        UTF82A((char*)m_struDevCfg.sServerName, (char*)m_struDevCfg.sServerName, MAX_DEVICE_NAME_LEN, &dwConvertLen);
        UTF82A((char*)m_struDevCfg.sSerialNumber, (char*)m_struDevCfg.sSerialNumber, MAX_SERIALNO_LEN, &dwConvertLen);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_DEVICE_CFG");
        m_strDeviceName.SetString((char*)m_struDevCfg.sServerName, MAX_DEVICE_NAME_LEN);
        m_iRS232Num = m_struDevCfg.dwRS232Num;
        m_iRS485Num = m_struDevCfg.dwRS485Num;
        m_iNetportNum = m_struDevCfg.dwNetworkPortNum;
        m_iAuxoutNum = m_struDevCfg.dwAuxoutNum;
        m_cmbMajorScale.SetCurSel(m_struDevCfg.dwMajorScale);
        m_cmbMinorScale.SetCurSel(m_struDevCfg.dwMinorScale);
        m_iDeviceNO = m_struDevCfg.dwServerID;
        m_bCycleRecord = m_struDevCfg.dwRecycleRecord;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_DEVICE_CFG");
    }
    UpdateData(FALSE);
}

void CSubDlgDeviceCfg::CurCfgSetup()
{
    if(!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    memset(m_struDevCfg.sServerName, 0, MAX_DEVICE_NAME_LEN);
    memcpy(m_struDevCfg.sServerName, m_strDeviceName.GetBuffer(0), (sizeof(m_struDevCfg.sServerName) <= (m_strDeviceName.GetLength()) ? (MAX_DEVICE_NAME_LEN) : (m_strDeviceName.GetLength())));
    m_struDevCfg.dwMajorScale = m_cmbMajorScale.GetCurSel();
    m_struDevCfg.dwMinorScale = m_cmbMinorScale.GetCurSel();
    m_struDevCfg.dwRecycleRecord = m_bCycleRecord;

    //需要将字符串字段转换成UTF-8
    DWORD dwConvertLen = 0;
    //2.0库里面会转换成GB2312
    char temp[MAX_DEVICE_NAME_LEN + 1] = { 0 };//多预留了一个字节，为了保存字符串的结束符
    if (!A2UTF8((char*)m_struDevCfg.sServerName, temp, MAX_DEVICE_NAME_LEN + 1, &dwConvertLen))
    {
        MessageBox("ServerName'length must less than 32 bytes");
        return;
    }
    else
    {
        memcpy(m_struDevCfg.sServerName, temp, MAX_DEVICE_NAME_LEN);
    }
    if (dwConvertLen > MAX_DEVICE_NAME_LEN + 1)
    {
        AfxMessageBox("ServerName convert to UTF-8 length than 32 bytes");
        return;
    }

    if (!A2UTF8((char*)m_struDevCfg.sSerialNumber, (char*)m_struDevCfg.sSerialNumber, MAX_SERIALNO_LEN, &dwConvertLen))
    {
        MessageBox("sSerialNumber'length must less than 128 bytes");
        return;
    }
    if (dwConvertLen > MAX_SERIALNO_LEN + 1)
    {
        AfxMessageBox("ServerName convert to UTF-8 length than 128 byte");
        return;
    }

    NET_EHOME_CONFIG    struCfg = {0};
    struCfg.pInBuf = &m_struDevCfg;
    struCfg.dwInSize = sizeof(m_struDevCfg);

    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_DEVICE_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_DEVICE_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_DEVICE_CFG");
    }
}

CString CSubDlgDeviceCfg::GetDVRType(DWORD dwDVRType)
{
    DWORD dwTypeAmount = sizeof(DVRTypeName)/sizeof(NET_DVR_TYPE_NAME);
    for (DWORD dwTypeCnt = 0; dwTypeCnt < dwTypeAmount; dwTypeCnt++)
    {
        if (DVRTypeName[dwTypeCnt].dwDVRType == dwDVRType)
        {
            return DVRTypeName[dwTypeCnt].byDevName;
        }
    }
    return _T("UNKNOWN TYPE");
}

void CSubDlgDeviceCfg::AddAudioType()
{

    int index = 0;
    m_cmbAudioType.InsertString(index, "OggVorbis");
    m_cmbAudioType.SetItemData(index, 0);
    index++;

    m_cmbAudioType.InsertString(index, "G711U");
    m_cmbAudioType.SetItemData(index, 1);
    index++;

    m_cmbAudioType.InsertString(index, "G711A");
    m_cmbAudioType.SetItemData(index, 2);
    index++;

    m_cmbAudioType.InsertString(index, "G726");
    m_cmbAudioType.SetItemData(index, 3);
    index++;

    m_cmbAudioType.InsertString(index, "AAC");
    m_cmbAudioType.SetItemData(index, 4);
    index++;

    m_cmbAudioType.InsertString(index, "MP2L2");
    m_cmbAudioType.SetItemData(index, 5);
    index++;

    m_cmbAudioType.InsertString(index, "PCM");
    m_cmbAudioType.SetItemData(index, 6);
    index++;

    m_cmbAudioType.InsertString(index, "MP3");
    m_cmbAudioType.SetItemData(index, 7);
    index++;
}

void CSubDlgDeviceCfg::SetResolutionCurSel(DWORD dwData)
{
    for (int i = 0; i < m_cmbAudioType.GetCount(); i++)
    {
        if (m_cmbAudioType.GetItemData(i) == dwData)
        {
            m_cmbAudioType.SetCurSel(i);
            return;
        }
    }
}