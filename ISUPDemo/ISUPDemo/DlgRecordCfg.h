#pragma once
#include "afxwin.h"


typedef struct RecodCfg
{
    int iRecordType;
    int iStartHour;
    int iStartMin;
    int iStopHour;
    int iStopMin;
}RECOD_CFG;
// CDlgRecordCfg 对话框

class CDlgRecordCfg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRecordCfg)

public:
	CDlgRecordCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRecordCfg();

// 对话框数据
	enum { IDD = IDD_DLG_RECORD_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

    BOOL CheckInitParam();
    int m_iDeviceIndex;
    LONG m_lLoginID;
    DWORD m_dwChannelNo;
    int m_iChanIndex;
    RecodCfg m_arrRecCfg[4];

public:
    int m_iDay;
    int m_iDelay;
    int m_iPre;
    int m_iSaveTime;
    int m_iRecordType;
    BOOL m_bEnable;
    BOOL m_bRedunce;
    BOOL m_bAudio;
    BOOL m_bAllDay;
    afx_msg void OnBnClickedBtnSetRecord();
    afx_msg void OnBnClickedBtnGetRecord();
    int m_iRecord[4];

    int m_iStartH[4];

    int m_iStartM[4];

    int m_iStopH[4];

    int m_iStopM[4];

    virtual BOOL OnInitDialog();
    CComboBox m_cmbRecordType;
    CComboBox m_arrcmbRecordType[4];
};
