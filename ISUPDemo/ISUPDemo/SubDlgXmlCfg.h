#pragma once
#include "afxwin.h"


// CSubDlgXmlCfg 对话框
#define GETDEVICECONFIG_XML_PATTERN "<Params>\r\n<ConfigCmd>%s</ConfigCmd>\r\n"\
            "<ConfigParam1>%s</ConfigParam1>\r\n"\
            "<ConfigParam2>%s</ConfigParam2>\r\n"\
            "<ConfigParam3>%s</ConfigParam3>\r\n"\
            "<ConfigParam4>%s</ConfigParam4>\r\n"\
            "</Params>\r\n"

#define SETDEVICECONFIG_XML_PATTERN "<Params>\r\n<ConfigCmd>%s</ConfigCmd>\r\n"\
            "<ConfigParam1>%s</ConfigParam1>\r\n"\
            "<ConfigParam2>%s</ConfigParam2>\r\n"\
            "<ConfigParam3>%s</ConfigParam3>\r\n"\
            "<ConfigParam4>%s</ConfigParam4>\r\n"\
            "<ConfigXML>\r\n%s</ConfigXML>\r\n"\
            "</Params>\r\n"

enum CMD_TYPE
{
    BASIC = 1,     
    VIDEO_OSD,
    MOVE_DETECTION,
    HIDE_ALARM,
    VIDEO_LOST,
    HIDE_PRIVATE,
    VIDEO_STATUS,
    INFRARED_WIFI_CALL,
    Alarm,
    CAPTUREPIC,
    RS,
    RECORD,
    CMSINFO,
    GPS,
    ZERO_CHAN,
    DISK_FORMAT,
    PREVIEW,
    ABILITY,
    PASSENGER_PARAMETER,
    VCA,
    RECORD_STATUS,
    NET_STATUS,
    DIAL_MODE,
    ACS,
    OPERATOR_CODE,
    WIRELESS_INFO,
};

BOOL CALLBACK LongConfigCallback(LONG lHandle, LONG_LINK_MSG enMsg, void *pOutBuffer, DWORD dwOutLen, void *pUser);

class CSubDlgXmlCfg : public CDialog
{
    DECLARE_DYNAMIC(CSubDlgXmlCfg)

public:
    CSubDlgXmlCfg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CSubDlgXmlCfg();

// 对话框数据
    enum { IDD = IDD_SUB_DLG_XML_CFG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    BOOL CheckInitParam();
    void CurCfgUpdate();
    void CurCfgSetup();
    void InitGetParam(CMD_TYPE enumType);
    void InitSetParam(CMD_TYPE enumType);
    void InitControlParam();
    void InitCmdType();

public:
    int m_iDeviceIndex;
    LONG m_lLoginID;
    LRESULT OnWMAsyncResp(WPARAM wParam, LPARAM lParam);
    CMD_TYPE m_enumType;
    NET_EHOME_XML_CFG m_struXmlCfg;
    CString m_strInputXml;
    CString m_strOutputXml;
    CString m_strCmd;

    afx_msg void OnBnClickedButtonXmlControl();
    CString m_csCmd;
    CString m_csDetail;
    CString m_csCmdName;
    afx_msg void OnBnClickedButtonXmlConfig();
    CComboBox m_cmbDetail;
    afx_msg void OnSelchangeComboCmd();
    afx_msg void OnSelchangeComboCmdDetail();
    CComboBox m_cmbCmd;
    CString m_csType;
    CComboBox m_cmbType;
    afx_msg void OnSelchangeComboType();
    afx_msg void OnCbnSelchangeComboGpsInfo();
    CString m_cmFunction;
    void ProcessData(LONG lHandle, LONG_LINK_MSG enMsg, void *pOutBuffer, DWORD dwOutLen);
    afx_msg void OnBnClickedBtnShot();
    afx_msg void OnBnClickedBtnRelease();
    afx_msg void OnBnClickedBtnMaterial();
    afx_msg void OnBnClickedBtnInsert();
    afx_msg void OnBnClickedBtnUpgrate();
    afx_msg void OnBnClickedBtnInsertStop();
    CString m_strIP;
    DWORD m_dwSeq;
    DWORD m_dwTimeID;
    DWORD m_dwCheduleID;
    CString m_strPlayTime;
//    afx_msg void OnCbnSetfocusComboCmdDetail();
    //afx_msg void OnBnClickedBtnGetTerminalInfo();
    //afx_msg void OnBnClickedBtnInfoDiffusionAddition();
    //afx_msg void OnBnClickedButtonBinaryConfig();
	afx_msg void OnBnClickedBtnInfopublsih();
};
