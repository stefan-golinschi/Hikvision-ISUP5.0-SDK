#pragma once
#include "afxcmn.h"
#include "afxwin.h"

typedef enum _PICTURE_FILE_TYPE_
{
    PIC_SCHEDULED_CAP = 0, //定时抓图
    PIC_MOTION_CAP = 1, //移动侦测抓图
    PIC_ALARM_CAP = 2, //报警抓图
    PIC_MOTION_OR_ALARMIN_CAP = 3, //报警|移动侦测抓图
    PIC_MOTION_AND_ALARMIN_CAP = 4, //报警&移动侦测抓图
    PIC_COMMAND_CAP = 5, //命令触发录像抓图
    PIC_MANUAL_CAP = 6, //手动抓图
    PIC_VIBRATING_ALARM_CAP = 7, //震动报警抓图
    PIC_ENVIRONMENT_ALARM_CAP = 8, //环境报警触发抓图
    PIC_VCA_ALARM = 9, //智能报警图片
    PIC_PIR_ALARM = 10, //PIR报警录像图片
    PIC_WIRELESS_ALARM = 11, //无线报警图片
    PIC_EMERGENCY_ALARM = 12, //呼救报警图片
    PIC_FACE_DETECTION = 13, //人脸侦测图片
    PIC_LINE_CROSSING_DETECTION = 14, //越界侦测图片
    PIC_INTRUSION_DETECTION = 15, //入侵区域侦测图片
    PIC_SCENCE_CHANGE_DETECTION = 16, //场景变更侦测图片
    PIC_LOCAL_PLAYBACK = 17, //设备本地回放时截图
    PIC_VCA_DETECTION = 18, //智能侦测图片
    PIC_REGION_ENTRANCE_DETECTION = 19,//进入区域侦测图片
    PIC_REGION_EXITING_DETECTION = 20,//离开区域侦测图片
    PIC_LOITERING_DETECTION = 21,//徘徊侦测图片
    PIC_PEOPLE_GATHERING_DETECTION = 22,//人员聚集侦测图片
    PIC_FAST_MOVING_DETECTION = 23,//快速运动侦测图片
    PIC_PARKING_DETECTION = 24,//停车侦测图片
    PIC_UNATTENDED_BAGGAGE_DETECTION = 25,//物品遗留侦测图片
    PIC_OBJECT_REMOVAL_DETECTION = 26,//物品拿取侦测图片
    PIC_LICENSE_DETECTION = 27,//车牌侦测图片
    PIC_UPLOADED_TO_CLIENT = 28, //客户端上传图片
    PIC_ALL_PIC = 255, //ALL
}PICTURE_FILE_TYPE;

// CDlgPicFile 对话框

class CDlgPicFile : public CDialog
{
    DECLARE_DYNAMIC(CDlgPicFile)

public:
    CDlgPicFile(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CDlgPicFile();

// 对话框数据
    enum { IDD = IDD_DIALOG_PIC_FILE };

    BOOL CheckInitParam();
    static UINT GetFileThread(LPVOID pParam);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnSearchPicFile();
    virtual BOOL OnInitDialog();
    CListCtrl m_lstPicFile;
    CTime m_dateStart;
    CTime m_timeStart;
    CTime m_DateStop;
    CTime m_timeStop;
    CComboBox m_cmbPicType;

    LONG m_iDeviceIndex;
    LONG m_lLoginID;
    BOOL m_bSearching;
    BOOL m_bQuit;
    LONG m_iChanIndex;
    LONG m_lFileHandle;
    HANDLE m_hFileThread;
    DWORD m_dwFileNum;
    NET_EHOME_FINDCOND m_struFindCond;
    NET_EHOME_FINDDATA m_struFindData;
    int m_byTimeType;
};
