#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"


// CDlgDevLog 对话框

class CDlgAlarmHostLog : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmHostLog)

public:
	CDlgAlarmHostLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAlarmHostLog();

// 对话框数据
    enum { IDD = IDD_DLG_ALARM_HOST_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    CComboBox m_comboMajorType;
    CComboBox m_comboMinorType;
    CTime m_ctDateStart;
    CTime m_ctDateStop;
    CTime m_ctTimeStart;
    CTime m_ctTimeStop;
    CListCtrl m_listDeviceLog;
    afx_msg void OnBnClickedBtnSearchDevLog();



    LONG m_iDeviceIndex;
    LONG m_lLoginID;
    BOOL m_bSearching;
    BOOL m_bQuit;
    LONG m_iChanIndex;
    LONG m_lFileHandle;
    HANDLE m_hFileThread;
    DWORD m_dwLogNum;

    int m_iMode;
    int m_iMajType;
    int m_iMinType;
    BOOL m_bSearch;
    LONG m_lLogHandle;

    NET_EHOME_ALARM_HOST_LOG_COND m_struAlarmHostLogCond;
    NET_EHOME_ALARM_HOST_LOG m_struAlarmHostLog;

    static UINT GetFileThread(LPVOID pParam);
    BOOL CheckInitParam();

    typedef enum _ALARM_TYPE_
    {
        FILE_TIMING = 0, //定时录像
        FILE_MOTION_DETECT = 1, //移动侦测录像
        FILE_ALARMIN = 2, //报警触发录像
        FILE_MOTION_OR_ALARMIN = 3, //报警|移动侦测
        FILE_MOTION_AND_ALARMIN = 4, //报警&移动侦测
        FILE_COMMAND = 5, //命令触发录像
        FILE_MANUAL = 6, //手动录像
        FILE_VCA = 7, //智能录像
        FILE_PIR_ALARM = 10, //PIR报警录像
        FILE_WIRELESS_ALARM = 11, //无线报警录像
        FILE_CALLHELP_ALARM = 12, //呼救报警
        FILE_PIR_WIRELESS_CALLHELP = 13,  //移动|报警输入|PIR|无线报警|呼救报警
        FILE_INTELLIGENT_TRANSPORT = 14, //智能交通事件报警
        FILE_LINE_DETECTION = 15,// 越界侦测
        FILE_FIELD_DETECTUIN = 16,// 区域入侵
        FILE_AUDIO_EXCEPTION = 17,// 声音异常
        //	FILE_IPC_SMART = 18,
        FILE_SCENECHANGE_DETECTION = 18,//场景变更侦测 2013-07-16
        FILE_INTELLIGENT_DETECTION = 19,
        FILE_FACE_DETECTION = 20, //人脸侦测
        FILE_POS_RECORD = 21, //POS搜索
        FILE_REGION_ENTRANCE_DETECTION = 26, //进入区域侦测
        FILE_REGION_EXITING_DETECTION = 27,//离开区域侦测
        FILE_LOITERING_DETECTION = 28,//徘徊侦测
        FILE_PEOPLE_GATHERING_DETECTION = 29,//人员聚集侦测
        FILE_FAST_MOVING_DETECTION = 30,//快速移动侦测
        FILE_PARKING_DETECTION = 31,//停车侦测
        FILE_UNATTENDED_BAGGAGE_DETECTION = 32,//物品遗留侦测
        FILE_OBJECT_REMOVAL_DETECTION = 33,//物品拿取侦测
        FILE_OBJECT_FIRE_DETECTION = 34,//物品拿取侦测
        FILE_OBJECT_VANDALPROOF_DETECTION = 35,//防破坏检测
        FILE_THERMAL_SHIPS_DETECTION = 36, //船只检测
        FILE_THERMAL_THERMOMETRY_EARLYWARNING = 37, //测温预警
        FILE_THERMAL_THERMOMETRY_ALARM = 38, //测温报警
        FILE_THERMOMETRY_DIFF_ALARM = 42,//测差报警
        FILE_THERMOMETRY_OFFLINE_ALARM = 43,//离线测温报警
        FILE_ZONE_ALARMIN_ALARM = 44,//防区报警
        FILE_EMERGENCY_CALL_HELP_ALARM = 45,//紧急求助
        FILE_CONSULT = 46,//业务咨询
        FILE_VCA_EVENT_GET_UP = 47,// 起身检测
        FILE_VCA_EVENT_ADV_REACH_HEIGHT = 48,// 折线攀高
        FILE_VCA_EVENT_TOILET_TARRY = 49,// 如厕超时
        FILE_ALL = 0xffff //全部
    }ALARM_TYPE;
};
