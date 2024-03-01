#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "HCISUPCMS.h"
#include "ATLComTime.h"


// CDlgRecordFile 对话框

typedef enum _RECORD_FILE_TYPE_
{
    FILE_TIMING = 0, //定时录像
    FILE_MOTION_DETECT = 1, //移动侦测录像
    FILE_ALARMIN = 2, //报警触发录像
    FILE_MOTION_OR_ALARMIN = 3, //报警|移动侦测
    FILE_MOTION_AND_ALARMIN = 4, //报警&移动侦测
    FILE_COMMAND = 5, //命令触发录像
    FILE_MANUAL = 6, //手动录像
    FILE_SHAKE_ALARM = 7, //震动报警
    FILE_ENVIRONMENT_ALARM = 8, //环境报警
    FILE_VCA_ALARM = 9, //智能报警（或取证录像）
    FILE_PIR_ALARM = 10, //PIR报警录像
    FILE_WIRELESS_ALARM = 11, //无线报警录像
    FILE_CALLHELP_ALARM = 12, //呼救报警
    FILE_ALL_ALARM = 13,  //全部报警
    PIC_ALL = 100,  //全部类型图片
    PIC_LICENSE_RECOGNITION = 101,  //车牌识别图片
    PIC_INSPECT_ALARM = 102,  //稽查报警图片
    PIC_MANUAL_SNAP = 103,    //手动抓拍图片
    PIC_PLAYBACK_SNAP = 104,    //回放抓拍图片
    FILE_ALL = 0xff //全部
}RECORD_FILE_TYPE;

typedef struct _tagPlaybackParam_
{
    LONG    lPort;
    HWND    hPlayWnd;
    char    strFileName[256];
    LONG    lPlayBackHandle;
    LONG    lSessionID;
}PLAYBACK_PARAM, *LPPLAYBACK_PARAM;

class CDlgRecordFile : public CDialog
{
    DECLARE_DYNAMIC(CDlgRecordFile)

public:
    CDlgRecordFile(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgRecordFile();


    BOOL CheckInitParam();
    static UINT GetFileThread(LPVOID pParam);

// 对话框数据
    enum { IDD = IDD_DIALOG_REC_FILE };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_lstRecFile;
    CComboBox m_cmbRecFileType;
    CTime m_dateStart;
    CTime m_timeStart;
    CTime m_DateStop;
    CTime m_timeStop;
    afx_msg void OnBnClickedBtnSearchRecFile();
    virtual BOOL OnInitDialog();
    BOOL  InputStreamData(BYTE byDataType, BYTE* pBuffer, int iDataLen);

    LONG m_iDeviceIndex;
    LONG m_lLoginID;
    BOOL m_bSearching;
    BOOL m_bQuit;
    LONG m_iChanIndex;
    LONG m_lFileHandle;
    HANDLE m_hFileThread;
    DWORD m_dwFileNum;
    BOOL m_bSave;
    FILE *m_pVideoFile;
    BOOL m_bStart;
    BOOL  m_bPause;
    NET_EHOME_FINDCOND m_struFindCond;
    NET_EHOME_FINDDATA m_struFindData;
    afx_msg void OnNMDblclkListRecFile(NMHDR *pNMHDR, LRESULT *pResult);
    CStatic m_staticPlayWnd;

    LONG    m_lPlayBackListenHandle;
    PLAYBACK_PARAM m_struPlayParam;

    int m_nFileNum;
    char m_szFileName[512][128];


    afx_msg void OnBnClickedButtonCycle();
    afx_msg void OnBnClickedButtonStartPlay();
    afx_msg void OnBnClickedButtonStopPlay();
    CComboBox m_cmbSeekType;

    int m_iSessionID;
    afx_msg void OnStnClickedStaticPlayWnd();
    afx_msg void OnNMClickListRecFile(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonSave();
    COleDateTime m_PlayDateStart;
    COleDateTime m_PlayTimeStart;
    COleDateTime m_PlayDateStop;
    COleDateTime m_PlayTimeStop;
    int m_byTimeType;
    int m_byRecordType;
    int m_bTypeTime;
    CComboBox m_cmbTimeType;
    afx_msg void OnBnClickedButtonPause();
    afx_msg void OnBnClickedPlaybackSound();

    DWORD   m_dwVersion;
    afx_msg void OnBnClickedButtonFastPlay();
    afx_msg void OnBnClickedButtonSlowPlay();
    CComboBox m_protocolType;
    BOOL m_byEncrypt;
    int m_byStreamType;
    BOOL m_bHls;
    DWORD m_dwPlaybackPort;

    char m_szTime[64];
};
