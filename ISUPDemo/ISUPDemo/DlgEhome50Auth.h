#pragma once
#include "afxwin.h"


// CDlgEhome50Auth 对话框

class CDlgEhome50Auth : public CDialogEx
{
    DECLARE_DYNAMIC(CDlgEhome50Auth)

public:
    CDlgEhome50Auth(int iDevID = -1, CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgEhome50Auth();

// 对话框数据
    enum { IDD = IDD_DLG_EHOME50_AUTH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
    afx_msg void OnBnClickedBtnCommand();
    virtual BOOL OnInitDialog();
    CString m_cmDeviceID;
    CString m_cmEhomeKey;

private:
    int m_iDevID;
};
