// DlgOutputCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgOutputCtrl.h"

int g_iPtzBtnIndex = 0;

/*********************************************************
Function:    CPTZButton
Desc:        Constructor
Input:    
Output:    
Return:    
**********************************************************/
CPTZButton::CPTZButton()
: m_lPlayHandle(-1)
{
    m_iSubBtnIndex = g_iPtzBtnIndex++;
}

/*********************************************************
Function:    ~CPTZButton
Desc:        destructor
Input:    
Output:    
Return:    
**********************************************************/
CPTZButton::~CPTZButton()
{
}

/*********************************************************
Function:    BEGIN_MESSAGE_MAP
Desc:        the map between control and function
Input:    
Output:    
Return:    
**********************************************************/
BEGIN_MESSAGE_MAP(CPTZButton, CButton)
    //{{AFX_MSG_MAP(CPTZButton)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL PtzControlAll(LONG lUserID, LONG lChannel, LPNET_EHOME_PTZ_PARAM lpStruParam, char *sLogoInfo)
{
    NET_EHOME_REMOTE_CTRL_PARAM struCtrlParam = {0};
    struCtrlParam.dwSize = sizeof(NET_EHOME_REMOTE_CTRL_PARAM);
    struCtrlParam.lpCondBuffer = &lChannel;
    struCtrlParam.dwCondBufferSize = sizeof(lChannel);
    struCtrlParam.lpInbuffer = lpStruParam;
    struCtrlParam.dwInBufferSize = sizeof(NET_EHOME_PTZ_PARAM);

    //目前没有要进行字符转换的字段

    if (!NET_ECMS_RemoteControl(lUserID, NET_EHOME_PTZ_CTRL, &struCtrlParam))
    {
        g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), 1, OPERATION_FAIL_T, "%s", sLogoInfo);
        return FALSE;
    }

    g_pMainDlg->AddLog(g_pMainDlg->GetCurDeviceIndex(), 1, OPERATION_SUCC_T, "%s", sLogoInfo);
    return TRUE;
}
/*********************************************************
Function:    OnLButtonDown
Desc:        
Input:    
Output:    
Return:    
**********************************************************/
void CPTZButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    BOOL bRet = FALSE;
    //int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    //int iChanIndex = g_pMainDlg->GetCurChanIndex();
    int iChanIndex = g_pMainDlg->m_pDlgPreview[g_pMainDlg->m_iCurWndIndex].m_iChanIndex;
    int iDevIndex = g_pMainDlg->m_pDlgPreview[g_pMainDlg->m_iCurWndIndex].m_iDeviceIndex;

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;
    NET_EHOME_PTZ_PARAM struPtzParam = {0};
    struPtzParam.dwSize = sizeof(struPtzParam);
    struPtzParam.bySpeed = (BYTE)g_pMainDlg->m_dlgOutputCtrl->m_comboSpeed.GetCurSel() + 1;
    struPtzParam.byAction = 0;

    switch (m_iSubBtnIndex)
    {
    case 0:
        struPtzParam.byPTZCmd = PTZ_UP;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_UP_START");
        break;
    case 1:
        struPtzParam.byPTZCmd = PTZ_DOWN;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_DOWN_START");
        break;
    case 2:
        struPtzParam.byPTZCmd = PTZ_LEFT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_LEFT_START");
        break;
    case 3:
        struPtzParam.byPTZCmd = PTZ_RIGHT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_RIGHT_START");
        break;
    case 4:
        struPtzParam.byPTZCmd = PTZ_UPLEFT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_LEFTUP_START");
        break;
    case 5:
        struPtzParam.byPTZCmd = PTZ_UPRIGHT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_RIGHTUP_START");
        break;
    case 6:
        struPtzParam.byPTZCmd = PTZ_DOWNLEFT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_LEFTDOWN_START");
        break;
    case 7:
        struPtzParam.byPTZCmd = PTZ_DOWNRIGHT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_RIGHTDOWN_START");
        break;
    case 8:
        struPtzParam.byPTZCmd = PTZ_ZOOMOUT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_ZOOMOUT_START");
        break;
    case 9:
        struPtzParam.byPTZCmd = PTZ_ZOOMIN;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_ZOOMIN_START");
        break;    
    case 10:
        struPtzParam.byPTZCmd = PTZ_FOCUSNEAR;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_FOCUSNEAR_START");
        break;
    case 11:
        struPtzParam.byPTZCmd = PTZ_FOCUSFAR;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_FOCUSFAR_START");
        break;
    case 12:
        struPtzParam.byPTZCmd = PTZ_IRISSTARTUP;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_IRISSTARTUP_START");
        break;
    case 13:
        struPtzParam.byPTZCmd = PTZ_IRISSTOPDOWN;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_IRISSTOPDOWN_START");
        break;
    default:
        break;
    }

    if (!bRet)
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_FAIL_T, 1, "PTZ_CONTROL_STOP");
    }

    CButton::OnLButtonDown(nFlags, point);
}

/*********************************************************
Function:    OnLButtonUp
Desc:        
Input:    
Output:    
Return:    
**********************************************************/
void CPTZButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
    BOOL bRet = FALSE;
    //int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    //int iChanIndex = g_pMainDlg->GetCurChanIndex();
    int iChanIndex = g_pMainDlg->m_pDlgPreview[g_pMainDlg->m_iCurWndIndex].m_iChanIndex;
    int iDevIndex = g_pMainDlg->m_pDlgPreview[g_pMainDlg->m_iCurWndIndex].m_iDeviceIndex;;

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;
    NET_EHOME_PTZ_PARAM struPtzParam = {0};
    struPtzParam.dwSize = sizeof(struPtzParam);
    struPtzParam.bySpeed = (BYTE)g_pMainDlg->m_dlgOutputCtrl->m_comboSpeed.GetCurSel() + 1;
    struPtzParam.byAction = 1;

    switch (m_iSubBtnIndex)
    {
    case 0:
        struPtzParam.byPTZCmd = PTZ_UP;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_UP_STOP");
        break;
    case 1:
        struPtzParam.byPTZCmd = PTZ_DOWN;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_DOWN_STOP");
        break;
    case 2:
        struPtzParam.byPTZCmd = PTZ_LEFT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_LEFT_STOP");
        break;
    case 3:
        struPtzParam.byPTZCmd = PTZ_RIGHT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_RIGHT_STOP");
        break;
    case 4:
        struPtzParam.byPTZCmd = PTZ_UPLEFT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_LEFTUP_STOP");
        break;
    case 5:
        struPtzParam.byPTZCmd = PTZ_UPRIGHT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_RIGHTUP_STOP");
        break;
    case 6:
        struPtzParam.byPTZCmd = PTZ_DOWNLEFT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_LEFTDOWN_STOP");
        break;
    case 7:
        struPtzParam.byPTZCmd = PTZ_DOWNRIGHT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_RIGHTDOWN_STOP");
        break;
    case 8:
        struPtzParam.byPTZCmd = PTZ_ZOOMOUT;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_ZOOMOUT_STOP");
        break;
    case 9:
        struPtzParam.byPTZCmd = PTZ_ZOOMIN;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_ZOOMIN_STOP");
        break;    
    case 10:
        struPtzParam.byPTZCmd = PTZ_FOCUSNEAR;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_FOCUSNEAR_STOP");
        break;
    case 11:
        struPtzParam.byPTZCmd = PTZ_FOCUSFAR;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_FOCUSFAR_STOP");
        break;
    case 12:
        struPtzParam.byPTZCmd = PTZ_IRISSTARTUP;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_IRISSTARTUP_STOP");
        break;
    case 13:
        struPtzParam.byPTZCmd = PTZ_IRISSTOPDOWN;
        bRet = PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_IRISSTOPDOWN_STOP");
        break;
    default:
        break;
    }

    if (!bRet)
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_FAIL_T, 1, "PTZ_CONTROL");
    }

    CButton::OnLButtonUp(nFlags, point);
}


// CDlgOutputCtrl dialog

IMPLEMENT_DYNAMIC(CDlgOutputCtrl, CDialog)

CDlgOutputCtrl::CDlgOutputCtrl(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgOutputCtrl::IDD, pParent)
{
    memset(&m_struImageCfg, 0, sizeof(m_struImageCfg));
}

CDlgOutputCtrl::~CDlgOutputCtrl()
{
}

void CDlgOutputCtrl::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_SPEED, m_comboSpeed);
    DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPresetIndex);
    DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_sliderBrightness);
    DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_sliderContrast);
    DDX_Control(pDX, IDC_SLIDER_SATURATION, m_sliderSaturation);
    DDX_Control(pDX, IDC_SLIDER_HUE, m_sliderHue);
}


BEGIN_MESSAGE_MAP(CDlgOutputCtrl, CDialog)
    ON_BN_CLICKED(IDC_BTN_PRESET_SETUP, &CDlgOutputCtrl::OnBnClickedBtnPresetSetup)
    ON_BN_CLICKED(IDC_BTN_PRESET_ADJUST, &CDlgOutputCtrl::OnBnClickedBtnPresetAdjust)
    ON_BN_CLICKED(IDC_BTN_PRESET_DELETE, &CDlgOutputCtrl::OnBnClickedBtnPresetDelete)
    ON_BN_CLICKED(IDC_BTN_LIGHT_ON, &CDlgOutputCtrl::OnBnClickedBtnLightOn)
    ON_BN_CLICKED(IDC_BTN_LIGHT_OFF, &CDlgOutputCtrl::OnBnClickedBtnLightOff)
    ON_BN_CLICKED(IDC_BTN_WIPER_ON, &CDlgOutputCtrl::OnBnClickedBtnWiperOn)
    ON_BN_CLICKED(IDC_BTN_WIPER_OFF, &CDlgOutputCtrl::OnBnClickedBtnWiperOff)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_BTN_IMAGE_REFRESH, &CDlgOutputCtrl::OnBnClickedBtnImageRefresh)
END_MESSAGE_MAP()


// CDlgOutputCtrl message handlers

void CDlgOutputCtrl::OnBnClickedBtnPresetSetup()
{
    // TODO: Add your control notification handler code here
    NET_EHOME_PRESET_PARAM struPresetParam = {0};

    struPresetParam.dwSize = sizeof(NET_EHOME_PRESET_PARAM);
    struPresetParam.byPresetCmd = 1;
    struPresetParam.dwPresetIndex = m_comboPresetIndex.GetCurSel() + 1;

    /*int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    int iChanIndex = g_pMainDlg->GetCurChanIndex();*/

    int iChanIndex = g_pMainDlg->m_pDlgPreview[g_pMainDlg->m_iCurWndIndex].m_iChanIndex;
    int iDevIndex = g_pMainDlg->m_pDlgPreview[g_pMainDlg->m_iCurWndIndex].m_iDeviceIndex;

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;

    NET_EHOME_REMOTE_CTRL_PARAM struCtrlParam = {0};
    struCtrlParam.dwSize = sizeof(struCtrlParam);
    struCtrlParam.lpCondBuffer = &lChannel;
    struCtrlParam.dwCondBufferSize = sizeof(lChannel);
    struCtrlParam.lpInbuffer = &struPresetParam;
    struCtrlParam.dwInBufferSize = sizeof(struPresetParam);

    //目前没有要进行字符转换的字段

    if (!NET_ECMS_RemoteControl(lUserID, NET_EHOME_PRESET_CTRL, &struCtrlParam))
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_FAIL_T, 1, "Set preset fail!");
    }
    else
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_SUCC_T, 1, "Set preset success");
    }    
}

BOOL CDlgOutputCtrl::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    m_sliderBrightness.SetRange(0, 255,TRUE);
    m_sliderHue.SetRange(0,255,TRUE);
    m_sliderSaturation.SetRange(0,255,TRUE);
    m_sliderContrast.SetRange(0,255,TRUE);
    m_sliderBrightness.SetPos(0);
    m_sliderHue.SetPos(0);
    m_sliderSaturation.SetPos(0);
    m_sliderContrast.SetPos(0);

    m_btnPtzUp.SubclassDlgItem(IDC_BTN_PTZ_UP,this);
    m_btnPtzDown.SubclassDlgItem(IDC_BTN_PTZ_DOWN,this);
    m_btnPtzLeft.SubclassDlgItem(IDC_BTN_PTZ_LEFT,this);
    m_btnPtzRight.SubclassDlgItem(IDC_BTN_PTZ_RIGHT,this);
    m_btnPtzLeftUp.SubclassDlgItem(IDC_BTN_PTZ_LEFT_UP,this);
    m_btnPtzRightUp.SubclassDlgItem(IDC_BTN_PTZ_RIGHT_UP,this);
    m_btnPtzLeftDown.SubclassDlgItem(IDC_BTN_PTZ_LEFT_DOWN,this);
    m_btnPtzRightDown.SubclassDlgItem(IDC_BTN_PTZ_RIGHT_DOWN,this);
    m_btnZoomIn.SubclassDlgItem(IDC_BTN_ZOOM_IN,this);
    m_btnZoomOut.SubclassDlgItem(IDC_BTN_ZOOM_OUT,this);
    m_btnFocusNear.SubclassDlgItem(IDC_BTN_FOCUS_NEAR,this);
    m_btnFocusFar.SubclassDlgItem(IDC_BTN_FOCUS_FAR,this);
    m_btnIrisOpen.SubclassDlgItem(IDC_BTN_IRIS_OPEN,this);
    m_btnIrisClose.SubclassDlgItem(IDC_BTN_IRIS_CLOSE,this);

    int i = 0;
    char szTemp[8] = {0};
    m_comboPresetIndex.ResetContent();

    for (i = 0; i < 300; i++)
    {
        sprintf(szTemp, "%d", i + 1);
        m_comboPresetIndex.AddString(szTemp);
    }

    m_comboSpeed.ResetContent();
    for (i = 0; i < 7; i++)
    {
        sprintf(szTemp, "%d", i + 1);
        m_comboSpeed.AddString(szTemp);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgOutputCtrl::OnBnClickedBtnPresetAdjust()
{
    // TODO: 在此添加控件通知处理程序代码
    NET_EHOME_PRESET_PARAM struPresetParam = {0};

    struPresetParam.dwSize = sizeof(NET_EHOME_PRESET_PARAM);
    struPresetParam.byPresetCmd = 3;
    struPresetParam.dwPresetIndex = m_comboPresetIndex.GetCurSel() + 1;

    int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    int iChanIndex = g_pMainDlg->GetCurChanIndex();

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;

    NET_EHOME_REMOTE_CTRL_PARAM struCtrlParam = {0};
    struCtrlParam.dwSize = sizeof(struCtrlParam);
    struCtrlParam.lpCondBuffer = &lChannel;
    struCtrlParam.dwCondBufferSize = sizeof(lChannel);
    struCtrlParam.lpInbuffer = &struPresetParam;
    struCtrlParam.dwInBufferSize = sizeof(struPresetParam);

    //目前没有要进行字符转换的字段

    if (!NET_ECMS_RemoteControl(lUserID, NET_EHOME_PRESET_CTRL, &struCtrlParam))
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_FAIL_T, 1, "Call preset fail!");
    }
    else
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_SUCC_T, 1, "Call preset success");   
    }     
}

void CDlgOutputCtrl::OnBnClickedBtnPresetDelete()
{
    // TODO: 在此添加控件通知处理程序代码
    NET_EHOME_PRESET_PARAM struPresetParam = {0};

    struPresetParam.dwSize = sizeof(NET_EHOME_PRESET_PARAM);
    struPresetParam.byPresetCmd = 2;
    struPresetParam.dwPresetIndex = m_comboPresetIndex.GetCurSel() + 1;

    int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    int iChanIndex = g_pMainDlg->GetCurChanIndex();

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;

    NET_EHOME_REMOTE_CTRL_PARAM struCtrlParam = {0};
    struCtrlParam.dwSize = sizeof(struCtrlParam);
    struCtrlParam.lpCondBuffer = &lChannel;
    struCtrlParam.dwCondBufferSize = sizeof(lChannel);
    struCtrlParam.lpInbuffer = &struPresetParam;
    struCtrlParam.dwInBufferSize = sizeof(struPresetParam);

    //目前没有要进行字符转换的字段

    if (!NET_ECMS_RemoteControl(lUserID, NET_EHOME_PRESET_CTRL, &struCtrlParam))
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_FAIL_T, 1, "Delete preset fail!");
    }
    else
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_SUCC_T, 1, "Delete preset success");
    }      
}

void CDlgOutputCtrl::OnBnClickedBtnLightOn()
{
    // TODO: 在此添加控件通知处理程序代码
    NET_EHOME_PTZ_PARAM struPtzParam = {0};
    struPtzParam.dwSize = sizeof(struPtzParam);
    struPtzParam.byPTZCmd = PTZ_LIGHT;

    int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    int iChanIndex = g_pMainDlg->GetCurChanIndex();

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;

    PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_LIGHT_ON");
}

void CDlgOutputCtrl::OnBnClickedBtnLightOff()
{
    // TODO: 在此添加控件通知处理程序代码
    NET_EHOME_PTZ_PARAM struPtzParam = {0};
    struPtzParam.dwSize = sizeof(struPtzParam);
    struPtzParam.byPTZCmd = PTZ_LIGHT;
    struPtzParam.byAction = 1;

    int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    int iChanIndex = g_pMainDlg->GetCurChanIndex();

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;

    PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_LIGHT_OFF");
}

void CDlgOutputCtrl::OnBnClickedBtnWiperOn()
{
    // TODO: 在此添加控件通知处理程序代码
    NET_EHOME_PTZ_PARAM struPtzParam = { 0 };
    struPtzParam.dwSize = sizeof(struPtzParam);
    struPtzParam.byPTZCmd = PTZ_WIPER;

    int iDevIndex = g_pMainDlg->GetCurDeviceIndex();
    int iChanIndex = g_pMainDlg->GetCurChanIndex();

    if (iDevIndex == -1 || iChanIndex == -1)
    {
        char szLan[128] = { 0 };
        g_StringLanType(szLan, "请选择一个设备通道", "Please select a channel");
        AfxMessageBox(szLan);
        return;
    }

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;

    PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_WIPER_ON");
}


void CDlgOutputCtrl::OnBnClickedBtnWiperOff()
{
    // TODO: 在此添加控件通知处理程序代码
    NET_EHOME_PTZ_PARAM struPtzParam = {0};
    struPtzParam.dwSize = sizeof(struPtzParam);
    struPtzParam.byPTZCmd = PTZ_WIPER;
    struPtzParam.byAction = 1;

    int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    int iChanIndex = g_pMainDlg->GetCurChanIndex();

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;

    PtzControlAll(lUserID, lChannel, &struPtzParam, "PTZ_WIPER_OFF");
}

void CDlgOutputCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: Add your message handler code here and/or call default
    if (nSBCode == SB_ENDSCROLL)
    {
        BOOL    bRet = FALSE;
        if (GetDlgItem(IDC_SLIDER_BRIGHTNESS) == pScrollBar)
        {        
            m_struImageCfg.byBright = (BYTE)m_sliderBrightness.GetPos();
            bRet = TRUE;
        }
        else if (GetDlgItem(IDC_SLIDER_CONTRAST) == pScrollBar)
        {
            m_struImageCfg.byContrast = (BYTE)m_sliderContrast.GetPos();
            bRet = TRUE;
        }
        else if (GetDlgItem(IDC_SLIDER_SATURATION) == pScrollBar)
        {
            m_struImageCfg.bySaturation = (BYTE)m_sliderSaturation.GetPos();
            bRet = TRUE;
        }
        else if (GetDlgItem(IDC_SLIDER_HUE) == pScrollBar)
        {
            m_struImageCfg.byHue = (BYTE)m_sliderHue.GetPos();
            bRet = TRUE;
        }

        if (bRet)
        {
            int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
            int iChanIndex = g_pMainDlg->GetCurChanIndex();

            LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
            LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;
            m_struImageCfg.dwSize = sizeof(NET_EHOME_IMAGE_CFG);
            NET_EHOME_CONFIG    struCfg = {0};
            struCfg.pCondBuf = &lChannel;
            struCfg.dwCondSize = sizeof(lChannel);
            struCfg.pInBuf = &m_struImageCfg;
            struCfg.dwInSize = sizeof(m_struImageCfg);

            //目前没有要进行字符转换的字段
            if (NET_ECMS_SetDevConfig(lUserID, NET_EHOME_SET_IMAGE_CFG, &struCfg, sizeof(struCfg)))
            {
                g_pMainDlg->AddLog(iDevIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_IMAGE_CFG");
            }
            else
            {
                g_pMainDlg->AddLog(iDevIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_IMAGE_CFG");
            }
        }
    }   

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgOutputCtrl::OnBnClickedBtnImageRefresh()
{
    // TODO: Add your control notification handler code here
    int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
    int iChanIndex = g_pMainDlg->GetCurChanIndex();

    LONG lUserID = g_struDeviceInfo[iDevIndex].lLoginID;
    LONG lChannel = g_struDeviceInfo[iDevIndex].struChanInfo[iChanIndex].iChannelNO;

    NET_EHOME_CONFIG    struCfg = {0};
    struCfg.pCondBuf = &lChannel;
    struCfg.dwCondSize = sizeof(lChannel);
    struCfg.pOutBuf = &m_struImageCfg;
    struCfg.dwOutSize = sizeof(m_struImageCfg);

    //没有需要转换成UTF-8的字段
    if (NET_ECMS_GetDevConfig(lUserID, NET_EHOME_GET_IMAGE_CFG, &struCfg, sizeof(struCfg)))
    {
        //目前没有要进行字符转换的字段

        m_sliderBrightness.SetPos(m_struImageCfg.byBright);
        m_sliderContrast.SetPos(m_struImageCfg.byContrast);
        m_sliderHue.SetPos(m_struImageCfg.byHue);
        m_sliderSaturation.SetPos(m_struImageCfg.bySaturation);

        g_pMainDlg->AddLog(iDevIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_IMAGE_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(iDevIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_IMAGE_CFG");
    }
}

