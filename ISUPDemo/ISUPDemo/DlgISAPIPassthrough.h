#pragma once
#include "afxwin.h"


// CDlgISAPIPassthrough �Ի���

class CDlgISAPIPassthrough : public CDialog
{
	DECLARE_DYNAMIC(CDlgISAPIPassthrough)

public:
	CDlgISAPIPassthrough(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgISAPIPassthrough();

// �Ի�������
	enum { IDD = IDD_DLG_ISAPI_PT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
    LONG m_lUserID;
    int m_iDeviceIndex;

public:
    CComboBox m_cmbCmdType;
    CString m_sCond;
    CString m_sInput;
    CString m_sOutput;
    CString m_sUrl;
    afx_msg void OnClickedBtnCommand();
    virtual BOOL OnInitDialog();
    DWORD m_dwTimeout;
};
