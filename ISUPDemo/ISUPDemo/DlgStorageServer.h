#pragma once
#include "afxwin.h"


// CDlgStorageServer 对话框

class CDlgStorageServer : public CDialog
{
    DECLARE_DYNAMIC(CDlgStorageServer)

public:
    CDlgStorageServer(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgStorageServer();

    // 对话框数据
    enum { IDD = IDD_DLG_STORAGESERVER };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnCommand();
    afx_msg void OnBnClickedBtnPicfile();
    virtual BOOL OnInitDialog();
    static BOOL CALLBACK StaticLargeFileCB(LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser);
    BOOL LargefileCallback(LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser);
    //typedef BOOL(CALLBACK * LF_LINK_INFO)(LONG lLinkHandle, DWORD dwDataType, void* pOutBuffer, DWORD dwOutLen, void* pInBuffer, DWORD dwInLen, void* pUser);
    static DWORD WINAPI LFDownThreadFunc(PVOID param);
	DWORD CreateSequence();
public:
    CComboBox m_cmbServerType;
    CString m_csFileName;
    CString m_csFileURL;
    CString m_csListenIP;
    int m_nPort;

    CEdit m_oEditAK;
    CEdit m_oEditSK;
    afx_msg void OnCbnSelchangeComboServertype();
    CString m_strPoolId;
    CString m_csDownloadUrl;
    CString m_csFileStorePath;
    afx_msg void OnBnClickedBtnStorepath();
    afx_msg void OnBnClickedBtnDowloadFile();
    CComboBox m_cmbDlServerType;
    CString m_csDlAk;
    CString m_csDlSk;
    CString m_csDlServerIP;
    int m_dwDlPort;
    afx_msg void OnCbnSelchangeCmbDlServertype();
    CString m_strUpKMSname;
    CString m_strUpPass;
    CString m_strDownKMSName;
    CString m_strDownKMSPass;
    CString m_strDelKMSName;
    CString m_strDelKMSPass;
    CString m_strDelAK;
    CString m_strDelSK;
    int m_iDelPort;
    int m_iFileNum;
    CString m_strDelIp;
    afx_msg void OnBnClickedBtnDowloadFile2();
    CString m_strDelUrl;
    CComboBox m_cboxDelType;
    BOOL m_cbBufferUp;
    BOOL m_bHttpsUpload;
    BOOL m_bHttpsDownload;
    BOOL m_bHttpsDelete;
    afx_msg void OnBnClickedButtonLfDownload();
    LONG m_dwLFHandle;
    FILE* m_pFile;
    DWORD m_dwSequence;
    afx_msg void OnBnClickedButtonLfCreateServer();
	afx_msg void OnBnClickedButtonLfUpload();
	
	int m_csLFPort;
	CString m_csLFSessionId;
	CString m_csLFURL;
	int m_csLFStart;
	int m_csLFEnd;
	CString m_csLFPath;
	CString m_csLFIP;
	BOOL m_bLFResume;
	//map<int, char*> m_LFDownMap;
	afx_msg void OnBnClickedButtonLfCreateChannel();
	afx_msg void OnBnClickedButtonLfDelChannel();

	LONG m_lUserID;
	LONG m_iDeviceIndex;
	afx_msg void OnBnClickedBtnStorepathLf();
	afx_msg void OnBnClickedButtonLfStoplf();

	BOOL m_bSetPath;
};
