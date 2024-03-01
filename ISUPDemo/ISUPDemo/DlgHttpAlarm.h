#pragma once
#include "afxwin.h"


// CDlgHttpAlarm 对话框

class CDlgHttpAlarm : public CDialog
{
	DECLARE_DYNAMIC(CDlgHttpAlarm)
    BOOL OnInitDialog();
public:
	CDlgHttpAlarm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHttpAlarm();
// 对话框数据
	enum { IDD = IDD_DLG_HTTPALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    BOOL SeparteMutipart();
    
    BOOL  m_bFinished;
    char* m_pXML; 
    char* m_pAlarmInfo;
    char* m_pURL;
    DWORD m_dwAlarmLen;
    char m_szVisibleFile[MAX_PATH];
    char m_szThermal[MAX_PATH];
	DECLARE_MESSAGE_MAP()
public:
    CStatic m_AlarmPicture1;
    CStatic m_AlarmPicture2;
    LRESULT OnProcessHttpAlarm(WPARAM wParam, LPARAM lParam);
    void DrawImg(CDC* pDC, CString strPath, int x, int y);
    INT32 FindStrIndex(char *s, char *d);
    afx_msg void OnBnClickedBtnRefresh();
    BOOL XmlPrase(char* pXml, char* pInputBefore, char* pInputAfter, char* pOutput);
    CEdit m_edtXMLBuff;
};
