#pragma once


#define MAX_CYCLE_NUM  4

// CCyclePlayBack 对话框

extern char g_szFileName[MAX_CYCLE_NUM][100];


#define CYCLE_PLAYBACK_TIMER  (WM_USER + 1)

typedef struct tagCYCLE_PLAY_BACK
{
    LONG lStreamSession;
    LONG lPort;
    HWND hWnd;
    LONG lCmsSession;
}CYCLE_PLAY_BACK, *LPCYCLE_PLAY_BACK;

class CDlgCyclePlayBack : public CDialog
{
    DECLARE_DYNAMIC(CDlgCyclePlayBack)

public:
    CDlgCyclePlayBack(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgCyclePlayBack();

// 对话框数据
    enum { IDD = IDD_DIALOG_CYCLE_PLAYBACK };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonStartCycle();

    static UINT CycleThreadProc(LPVOID pParam);

    LONG m_iDeviceIndex;

    LONG m_lCycleListenHandle[MAX_CYCLE_NUM];

    HWND m_PlayWnd[4];
    CYCLE_PLAY_BACK m_struPlayParam[MAX_CYCLE_NUM];


    void AddPlayBackFile(int index, char* pFile);

    char m_szFileName[MAX_CYCLE_NUM][100];



    WORD m_wPort[4];


    virtual BOOL OnInitDialog();


    BOOL IsFull();
    int FindIndex(LONG lSession);
    BOOL SetPlayPort(LONG lSession, LONG lPort);
    BOOL SetPlaySession(LONG lSession);
  
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedButtonStopCycle();

    void CycleStartPlayBack();
    void CycleStopPlayBack();
    void StartPlayBack(int iIndex);
    void StopPlayBack(int iIndex);

    BOOL m_bCycleStart;

    HANDLE m_hCycleThread[MAX_CYCLE_NUM];
    BOOL m_bExitCycle;

    //CRITICAL_SECTION m_cs;

    
};
