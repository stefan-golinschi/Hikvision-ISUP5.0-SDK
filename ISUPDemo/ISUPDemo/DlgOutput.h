#pragma once

#include "EHomeDemo.h"
#include "EHomeDemoDlg.h"
#include "GlobalDefinition.h"
// CDlgOutput dialog

class CEHomeDemoDlg;

class CDlgOutput : public CDialog
{
    DECLARE_DYNAMIC(CDlgOutput)

public:
    CDlgOutput(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgOutput();

// Dialog Data
    enum { IDD = IDD_DLG_OUTPUT };

    void DrawOutputBorder(void);
    HWND m_hWnd;
    //STRU_CHANNEL_INFO m_struChanInfo;
    HTREEITEM m_hChanItem;
    LONG    m_lPort;
    BOOL    m_bPlay;
    BOOL    m_bRecord;
    HANDLE  m_hPlayEvent;
    FILE *m_fVideoFile;
    CEHomeDemoDlg *m_pFatherDlg;

    CTime m_time;
    int m_iDeviceIndex;
    int m_iChanIndex;
    BOOL  m_bSound;
    BOOL  m_bSoundEnabled;

    LONG    m_lPlayHandle;
    int     m_iSessionID;
    int     m_iWndIndex;
    BOOL  InputStreamData(BYTE byDataType, char* pBuffer, int iDataLen);
    BOOL    SetPlayParam(LONG lPlayHandle, int iCurWndIndex, int iSession);
    BOOL    CleanPlayParam();
    CFile fileTest;
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    

    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
//    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    BOOL StartPlay(LPSTRU_CHANNEL_INFO pChanInfo, HTREEITEM hChanItem);
    BOOL StopPlay();

    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
private:
   char m_szHead[40];
};
