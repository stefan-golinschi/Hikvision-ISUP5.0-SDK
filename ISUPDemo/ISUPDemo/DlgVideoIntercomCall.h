#include "afxcmn.h"
//#include "DlgAudioTalk.h"
#if !defined(AFX_DLGVIDEOINTERCOMCALL_H__F40DE5CD_E347_4383_A2F8_DAB4A919B723__INCLUDED_)
#define AFX_DLGVIDEOINTERCOMCALL_H__F40DE5CD_E347_4383_A2F8_DAB4A919B723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVideoIntercomCall.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVideoIntercomCall dialog

class CDlgVideoIntercomCall : public CDialog
{
// Construction
public:
	CDlgVideoIntercomCall(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgVideoIntercomCall)
	enum { IDD = IDD_DLG_VIDEO_INTERCOM_CALL };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVideoIntercomCall)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVideoIntercomCall)
	afx_msg void OnBtnInquest();
	afx_msg void OnBtnAnswer();
	afx_msg void OnBtnHangUp();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBtnCancel();
#if (_MSC_VER >= 1500)	//vs2008
	afx_msg void OnTimer(UINT_PTR nIDEvent);
#else
	afx_msg void OnTimer(UINT nIDEvent);
#endif	
	afx_msg void OnBtnRefuse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
    BYTE m_byRequestType; 
    LONG m_iDevIndex;
    LONG m_iChannelIndex;
private:
    BOOL m_bClientCall;
    BOOL m_bPlayVideo;
    BOOL m_bPlayVoice;

private:
    void ChangeBtnStates();
    DWORD m_dwCallStatus;

    DWORD m_dwPeriod;
    DWORD m_dwBuildingNumber;
    DWORD m_dwUnitNumber;
    int m_dwFloorNumber;
    DWORD m_dwRoomNumber;
    DWORD m_dwDevIndex;
    CComboBox m_cmbUnitType;
    BOOL m_bWorking;
public:
    void SendByeCmd();
    void SendBellTimeoutCmd();    
    void SendAcceptCmd();
    void SendRefuseCmd();
    void SendCancellCmd();
    void SendCallCmd();
    void AddTalkLog(int cmd, char* szDeviceID, char * szLogInfo);
    CString IsapiPassthroughRequest(LONG lUserID, int iCmdType, CString sUrl, CString sInput);
protected:
    afx_msg LRESULT OnVedioIntercomeCmd(WPARAM wParam, LPARAM lParam);
public:
    CListCtrl m_listVTalkLog;
    BOOL StartPlayVideo();
    BOOL StopPlayVideo();
    BOOL StartPlayVoice();
    BOOL StopPlayVoice();
    afx_msg void OnBnClickedBtnVideoTest();
    LONG m_lListenHandle;
    LONG m_lPreviewHandle;
    LONG m_lPort;
    HWND m_hPlayWnd;
    BOOL  ProcStreamData(BYTE byDataType, char* pBuffer, int iDataLen);
    afx_msg void OnBnClickedBtnVoiceTest();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIDEOINTERCOMCALL_H__F40DE5CD_E347_4383_A2F8_DAB4A919B723__INCLUDED_)
