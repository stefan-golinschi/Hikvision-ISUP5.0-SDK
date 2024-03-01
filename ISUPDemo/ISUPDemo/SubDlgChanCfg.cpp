// SubDlgChanCfg.cpp : implementation file
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "SubDlgChanCfg.h"


// CSubDlgChanCfg dialog

IMPLEMENT_DYNAMIC(CSubDlgChanCfg, CDialog)

CSubDlgChanCfg::CSubDlgChanCfg(CWnd* pParent /*=NULL*/)
: CDialog(CSubDlgChanCfg::IDD, pParent)
, m_dwSelfDefBitrate(0)
, m_iDeviceIndex(-1)
, m_lLoginID(-1)
, m_iChanIndex(-1)
, m_iBright(0)
, m_iContrast(0)
, m_iSaturation(0)
, m_iHue(0)
, m_iInteralFrameI(0)
, m_iInteralFrameBP(0)
{
    memset(&m_struCompressionCond, 0, sizeof(m_struCompressionCond));
    m_struCompressionCond.dwSize = sizeof(m_struCompressionCond);
    memset(&m_struCompressionCfg, 0, sizeof(m_struCompressionCfg));
    m_struCompressionCfg.dwSize = sizeof(m_struCompressionCfg);
    memset(&m_struImageCfg, 0, sizeof(m_struImageCfg));
    m_struImageCfg.dwSize = sizeof(m_struImageCfg);
}

CSubDlgChanCfg::~CSubDlgChanCfg()
{
}

void CSubDlgChanCfg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_COMPRESSION_TYPE, m_cmbCompressionType);
    DDX_Control(pDX, IDC_COMBO_IMAGE_QUALITY, m_cmbImageQuality);
    DDX_Control(pDX, IDC_COMBO_STREAM_TYPE, m_cmbStreamType);
    DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_cmbResolution);
    DDX_Control(pDX, IDC_COMBO_BIT_RATE_TYPE, m_cmbBitrateType);
    DDX_Control(pDX, IDC_COMBO_FRAME_RATE, m_cmbFrameRate);
    DDX_Control(pDX, IDC_COMBO_MAX_BIT_RATE, m_cmbMaxBitrate);
    DDX_Text(pDX, IDC_EDIT_BIT_RATE, m_dwSelfDefBitrate);
    DDX_Text(pDX, IDC_EDIT_BRIGHT, m_iBright);
    DDX_Text(pDX, IDC_EDIT_CONTRAST, m_iContrast);
    DDX_Text(pDX, IDC_EDIT_SATURATION, m_iSaturation);
    DDX_Text(pDX, IDC_EDIT_HUE, m_iHue);
    DDX_Text(pDX, IDC_EDIT_INTERAL_FRAME_I, m_iInteralFrameI);
    DDX_Text(pDX, IDC_EDIT_INTERAL_FRAME_BP, m_iInteralFrameBP);
}


BEGIN_MESSAGE_MAP(CSubDlgChanCfg, CDialog)
    ON_CBN_SELCHANGE(IDC_COMBO_COMPRESSION_TYPE, &CSubDlgChanCfg::OnCbnSelchangeComboCompressionType)
    ON_CBN_SELCHANGE(IDC_COMBO_MAX_BIT_RATE, &CSubDlgChanCfg::OnCbnSelchangeComboMaxBitRate)
    ON_CBN_SELCHANGE(IDC_COMBO_BIT_RATE_TYPE, &CSubDlgChanCfg::OnSelchangeComboBitRateType)
END_MESSAGE_MAP()


// CSubDlgChanCfg message handlers

BOOL CSubDlgChanCfg::OnInitDialog()
{
    CDialog::OnInitDialog();

    AddResolution();
    m_cmbCompressionType.SetCurSel(0);
    // TODO:  Add extra initialization here
    CRect rc(0, 0, 0, 0);
    GetParent()->GetClientRect(&rc);
    ((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
    MoveWindow(&rc);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSubDlgChanCfg::CheckInitParam()
{
    char szLan[1024] = {0};
    m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (m_iDeviceIndex < 0)
    {
        g_StringLanType(szLan, "请先选择一个设备!", "please select a device first!");
        g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, 0, szLan);
        return FALSE;
    }
    m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    if (m_lLoginID < 0)
    {
        g_StringLanType(szLan, "设备未注册!", "device offline!");
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, szLan);
        return FALSE;
    }
    m_iChanIndex = g_pMainDlg->GetCurChanIndex();
    if (m_iChanIndex < 0)
    {
        g_StringLanType(szLan, "请选择一个通道!", "please select a channel first!");
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 0, szLan);
        return FALSE;
    }
    return TRUE;
}

void CSubDlgChanCfg::CurCfgUpdate()
{
    if(!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    m_struCompressionCond.dwSize = sizeof(m_struCompressionCond);
    m_struCompressionCond.dwChannelNum = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChannelNO;
    m_struCompressionCond.byCompressionType = (BYTE)m_cmbCompressionType.GetCurSel() + 1;
    NET_EHOME_CONFIG struCfg = {0};
    //获取压缩参数
    struCfg.pCondBuf = &m_struCompressionCond;
    struCfg.dwCondSize = sizeof(m_struCompressionCond);
    struCfg.pOutBuf = &m_struCompressionCfg;
    struCfg.dwOutSize = sizeof(m_struCompressionCfg);
    memset(&m_struCompressionCfg, 0, sizeof(m_struCompressionCfg));
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_COMPRESSION_CFG, &struCfg, sizeof(struCfg)))
    {
        //目前没有要进行字符转换的字段
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_COMPRESSION_CFG");
        m_cmbImageQuality.SetCurSel(m_struCompressionCfg.byPicQuality);
        m_cmbStreamType.SetCurSel(m_struCompressionCfg.byStreamType);
        SetResolutionCurSel(m_struCompressionCfg.dwResolution);
        m_cmbBitrateType.SetCurSel(m_struCompressionCfg.byBitRateType);
        m_cmbFrameRate.SetCurSel(m_struCompressionCfg.dwVideoFrameRate);
        m_cmbMaxBitrate.SetCurSel(m_struCompressionCfg.dwVideoBitRate);
        if (m_cmbMaxBitrate.GetCurSel() == m_cmbMaxBitrate.GetCount()-1)
        {
            GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(FALSE);
        }
        OnSelchangeComboBitRateType();//刷新数据，更新图片质量下拉框显示
        m_dwSelfDefBitrate = m_struCompressionCfg.dwMaxBitRate;
        m_iInteralFrameI = m_struCompressionCfg.wIntervalFrameI;
        m_iInteralFrameBP = m_struCompressionCfg.byIntervalBPFrame;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_COMPRESSION_CFG");
    }
    //获取图像参数
    memset(&struCfg, 0, sizeof(struCfg));
    struCfg.pCondBuf = &g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChannelNO;
    struCfg.dwCondSize = 4;
    struCfg.pOutBuf = &m_struImageCfg;
    struCfg.dwOutSize = sizeof(m_struImageCfg);
    if (NET_ECMS_GetDevConfig(m_lLoginID, NET_EHOME_GET_IMAGE_CFG, &struCfg, sizeof(struCfg)))
    {
        //目前没有要进行字符转换的字段
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_GET_IMAGE_CFG");
        m_iBright = m_struImageCfg.byBright;
        m_iContrast = m_struImageCfg.byContrast;
        m_iSaturation = m_struImageCfg.bySaturation;
        m_iHue = m_struImageCfg.byHue;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_GET_IMAGE_CFG");
    }
    UpdateData(FALSE);
}

void CSubDlgChanCfg::SetResolutionCurSel(DWORD dwData)
{
    for (int i = 0; i < m_cmbResolution.GetCount(); i++)
    {
        if (m_cmbResolution.GetItemData(i) == dwData)
        {
            m_cmbResolution.SetCurSel(i);
            return;
        }
    }
}
void CSubDlgChanCfg::AddResolution()
{
    int index = 0;
    m_cmbResolution.InsertString(index, "DCIF");
    m_cmbResolution.SetItemData(index, 0);
    index++;

    m_cmbResolution.InsertString(index, "CIF");
    m_cmbResolution.SetItemData(index, 1);
    index++;

    m_cmbResolution.InsertString(index, "QCIF");
    m_cmbResolution.SetItemData(index, 2);
    index++;

    m_cmbResolution.InsertString(index, "4CIF");
    m_cmbResolution.SetItemData(index, 3);
    index++;

    m_cmbResolution.InsertString(index, "2CIF");
    m_cmbResolution.SetItemData(index, 4);
    index++;

    m_cmbResolution.InsertString(index, "reserve");
    m_cmbResolution.SetItemData(index, 5);
    index++;

    m_cmbResolution.InsertString(index, "QVGA(320*240)");
    m_cmbResolution.SetItemData(index, 6);
    index++;

    m_cmbResolution.InsertString(index, "QQVGA(160*120)");
    m_cmbResolution.SetItemData(index, 7);
    index++;

    m_cmbResolution.InsertString(index, "384*288");
    m_cmbResolution.SetItemData(index, 12);
    index++;
    m_cmbResolution.InsertString(index, "576*576");
    m_cmbResolution.SetItemData(index, 13);
    index++;

    m_cmbResolution.InsertString(index, "VGA(640*480)");
    m_cmbResolution.SetItemData(index, 16);
    index++;    

    m_cmbResolution.InsertString(index, "UXGA (1600*1200)");
    m_cmbResolution.SetItemData(index, 17);
    index++;

    m_cmbResolution.InsertString(index, "SVGA(800*600)");
    m_cmbResolution.SetItemData(index, 18);
    index++;
    m_cmbResolution.InsertString(index, "HD720p(1280*720)");
    m_cmbResolution.SetItemData(index, 19);
    index++;

    m_cmbResolution.InsertString(index, "XVGA");
    m_cmbResolution.SetItemData(index, 20);
    index++;

    m_cmbResolution.InsertString(index, "HD900p");
    m_cmbResolution.SetItemData(index, 21);
    index++;
    m_cmbResolution.InsertString(index, "1536*1536");
    m_cmbResolution.SetItemData(index, 23);
    index++;

    m_cmbResolution.InsertString(index, "1920*1920");
    m_cmbResolution.SetItemData(index, 24);
    index++;    

    m_cmbResolution.InsertString(index, "1080P");
    m_cmbResolution.SetItemData(index, 27);
    index++;

    m_cmbResolution.InsertString(index, "2560*1920");
    m_cmbResolution.SetItemData(index, 28);
    index++;
    m_cmbResolution.InsertString(index, "1600*304");
    m_cmbResolution.SetItemData(index, 29);
    index++;

    m_cmbResolution.InsertString(index, "2048*1536");
    m_cmbResolution.SetItemData(index, 30);
    index++;

    m_cmbResolution.InsertString(index, "2448*2048");
    m_cmbResolution.SetItemData(index, 31);
    index++;
    m_cmbResolution.InsertString(index, "2448*1200");
    m_cmbResolution.SetItemData(index, 32);
    index++;

    m_cmbResolution.InsertString(index, "2448*800");
    m_cmbResolution.SetItemData(index, 33);
    index++;    

    m_cmbResolution.InsertString(index, "XGA(1024*768)");
    m_cmbResolution.SetItemData(index, 34);
    index++;

    m_cmbResolution.InsertString(index, "SXGA（1280*1024）");
    m_cmbResolution.SetItemData(index, 35);
    index++;
    m_cmbResolution.InsertString(index, "WD1(960*576/960*480)");
    m_cmbResolution.SetItemData(index, 36);
    index++;

    m_cmbResolution.InsertString(index, "1080i");
    m_cmbResolution.SetItemData(index, 37);
    index++;

    m_cmbResolution.InsertString(index, "WSXGA(1440*900)");
    m_cmbResolution.SetItemData(index, 38);
    index++;    

    m_cmbResolution.InsertString(index, "HD_F(1920*1080/1280*720)");
    m_cmbResolution.SetItemData(index, 39);
    index++;

    m_cmbResolution.InsertString(index, "HD_H(1920*540/1280*360)");
    m_cmbResolution.SetItemData(index, 40);
    index++;
    m_cmbResolution.InsertString(index, "HD_Q(960*540/630*360)");
    m_cmbResolution.SetItemData(index, 41);
    index++;

    m_cmbResolution.InsertString(index, "2336*1744");
    m_cmbResolution.SetItemData(index, 42);
    index++;

    m_cmbResolution.InsertString(index, "1920*1456");
    m_cmbResolution.SetItemData(index, 43);
    index++;    
    m_cmbResolution.InsertString(index, "2592*2048");
    m_cmbResolution.SetItemData(index, 44);
    index++;    
    m_cmbResolution.InsertString(index, "3296*2472");
    m_cmbResolution.SetItemData(index, 45);
    index++;    
    m_cmbResolution.InsertString(index, "1376*768");
    m_cmbResolution.SetItemData(index, 46);
    index++;    
    m_cmbResolution.InsertString(index, "1366*768");
    m_cmbResolution.SetItemData(index, 47);
    index++;    
    m_cmbResolution.InsertString(index, "1360*768");
    m_cmbResolution.SetItemData(index, 48);
    index++;    
    m_cmbResolution.InsertString(index, "WSXGA+");
    m_cmbResolution.SetItemData(index, 49);
    index++;    
    m_cmbResolution.InsertString(index, "720*720");
    m_cmbResolution.SetItemData(index, 50);
    index++;    

    m_cmbResolution.InsertString(index, "1280*1280");
    m_cmbResolution.SetItemData(index, 51);
    index++;    

    m_cmbResolution.InsertString(index, "2048*768");
    m_cmbResolution.SetItemData(index, 52);
    index++;

    m_cmbResolution.InsertString(index, "2048*2048");
    m_cmbResolution.SetItemData(index, 53);
    index++;

    m_cmbResolution.InsertString(index, "2560*2048");
    m_cmbResolution.SetItemData(index, 54);
    index++;

    m_cmbResolution.InsertString(index, "3072*2048");
    m_cmbResolution.SetItemData(index, 55);
    index++;

    m_cmbResolution.InsertString(index, "2304*1296");
    m_cmbResolution.SetItemData(index, 56);
    index++;

    m_cmbResolution.InsertString(index, "WXGA(1280*800)");
    m_cmbResolution.SetItemData(index, 57);
    index++;

    m_cmbResolution.InsertString(index, "1600*600");
    m_cmbResolution.SetItemData(index, 58);
    index++;

    m_cmbResolution.InsertString(index, "1600*9000");
    m_cmbResolution.SetItemData(index, 59);
    index++;

    m_cmbResolution.InsertString(index, "2752*2208");
    m_cmbResolution.SetItemData(index, 60);
    index++;

    m_cmbResolution.InsertString(index, "384*288");
    m_cmbResolution.SetItemData(index, 61);
    index++;

    m_cmbResolution.InsertString(index, "4000*3000");
    m_cmbResolution.SetItemData(index, 62);
    index++;

    m_cmbResolution.InsertString(index, "4096*2160");
    m_cmbResolution.SetItemData(index, 63);
    index++;

    m_cmbResolution.InsertString(index, "3840*2160");
    m_cmbResolution.SetItemData(index, 64);
    index++;

    m_cmbResolution.InsertString(index, "4000*2250");
    m_cmbResolution.SetItemData(index, 65);
    index++;

    m_cmbResolution.InsertString(index, "3072*1728");
    m_cmbResolution.SetItemData(index, 66);
    index++;

    m_cmbResolution.InsertString(index, "2592*1944");
    m_cmbResolution.SetItemData(index, 67);
    index++;

    m_cmbResolution.InsertString(index, "2464*1520");
    m_cmbResolution.SetItemData(index, 68);
    index++;

    m_cmbResolution.InsertString(index, "1280*1920");
    m_cmbResolution.SetItemData(index, 69);
    index++;

    m_cmbResolution.InsertString(index, "2560*1440");
    m_cmbResolution.SetItemData(index, 70);
    index++;

    m_cmbResolution.InsertString(index, "1024*1024");
    m_cmbResolution.SetItemData(index, 71);
    index++;

    m_cmbResolution.InsertString(index, "160*128");
    m_cmbResolution.SetItemData(index, 72);
    index++;

    m_cmbResolution.InsertString(index, "324*240");
    m_cmbResolution.SetItemData(index, 73);
    index++;

    m_cmbResolution.InsertString(index, "324*256");
    m_cmbResolution.SetItemData(index, 74);
    index++;

    m_cmbResolution.InsertString(index, "336*256");
    m_cmbResolution.SetItemData(index, 75);
    index++;

    m_cmbResolution.InsertString(index, "640*512");
    m_cmbResolution.SetItemData(index, 76);
    index++;

    m_cmbResolution.InsertString(index, "2720*2048");
    m_cmbResolution.SetItemData(index, 77);
    index++;

    m_cmbResolution.InsertString(index, "384*256");
    m_cmbResolution.SetItemData(index, 78);
    index++;

    m_cmbResolution.InsertString(index, "384*216");
    m_cmbResolution.SetItemData(index, 79);
    index++;

    m_cmbResolution.InsertString(index, "320*256");
    m_cmbResolution.SetItemData(index, 80);
    index++;

    m_cmbResolution.InsertString(index, "320*180");
    m_cmbResolution.SetItemData(index, 81);
    index++;

    m_cmbResolution.InsertString(index, "320*192");
    m_cmbResolution.SetItemData(index, 82);
    index++;

    m_cmbResolution.InsertString(index, "512*384");
    m_cmbResolution.SetItemData(index, 83);
    index++;

    m_cmbResolution.InsertString(index, "325*256");
    m_cmbResolution.SetItemData(index, 84);
    index++;

    m_cmbResolution.InsertString(index, "256*192");
    m_cmbResolution.SetItemData(index, 85);
    index++;

    m_cmbResolution.InsertString(index, "640*360");
    m_cmbResolution.SetItemData(index, 86);
    index++;

    m_cmbResolution.InsertString(index, "1776*1340");
    m_cmbResolution.SetItemData(index, 87);
    index++;

    m_cmbResolution.InsertString(index, "1936*1092");
    m_cmbResolution.SetItemData(index, 88);
    index++;

    m_cmbResolution.InsertString(index, "2080*784");
    m_cmbResolution.SetItemData(index, 89);
    index++;

    m_cmbResolution.InsertString(index, "2144*604");
    m_cmbResolution.SetItemData(index, 90);
    index++;

    m_cmbResolution.InsertString(index, "1920*1200");
    m_cmbResolution.SetItemData(index, 91);
    index++;

    m_cmbResolution.InsertString(index, "4064*3040");
    m_cmbResolution.SetItemData(index, 92);
    index++;

    m_cmbResolution.InsertString(index, "3040*3040");
    m_cmbResolution.SetItemData(index, 93);
    index++;

    m_cmbResolution.InsertString(index, "3072*2304");
    m_cmbResolution.SetItemData(index, 94);
    index++;

    m_cmbResolution.InsertString(index, "3072*1152");
    m_cmbResolution.SetItemData(index, 95);
    index++;
    m_cmbResolution.InsertString(index, "2560*2560");
    m_cmbResolution.SetItemData(index, 96);
    index++;

    m_cmbResolution.InsertString(index, "2688*1536");
    m_cmbResolution.SetItemData(index, 97);
    index++;

    m_cmbResolution.InsertString(index, "2688*1520");
    m_cmbResolution.SetItemData(index, 98);
    index++;

    m_cmbResolution.InsertString(index, "3072*3072");
    m_cmbResolution.SetItemData(index, 99);
    index++;
    m_cmbResolution.InsertString(index, "3392*2008");
    m_cmbResolution.SetItemData(index, 100);
    index++;

    m_cmbResolution.InsertString(index, "4000*3080");
    m_cmbResolution.SetItemData(index, 101);
    index++;

    m_cmbResolution.InsertString(index, "960*720");
    m_cmbResolution.SetItemData(index, 102);
    index++;
    m_cmbResolution.InsertString(index, "1024*1536");
    m_cmbResolution.SetItemData(index, 103);
    index++;

    m_cmbResolution.InsertString(index, "704*1056");
    m_cmbResolution.SetItemData(index, 104);
    index++;

    m_cmbResolution.InsertString(index, "105-352*528");
    m_cmbResolution.SetItemData(index, 105);
    index++;

    m_cmbResolution.InsertString(index, "2048*1530");
    m_cmbResolution.SetItemData(index, 106);
    index++;
    m_cmbResolution.InsertString(index, "2560*1600");
    m_cmbResolution.SetItemData(index, 107);
    index++;

    m_cmbResolution.InsertString(index, "2800*2100");
    m_cmbResolution.SetItemData(index, 108);
    index++;

    m_cmbResolution.InsertString(index, "4088*4088");
    m_cmbResolution.SetItemData(index, 109);
    index++;
    m_cmbResolution.InsertString(index, "4000*3072");
    m_cmbResolution.SetItemData(index, 110);
    index++;

    m_cmbResolution.InsertString(index, "960*1080(1080p Lite)");
    m_cmbResolution.SetItemData(index, 111);
    index++;

    m_cmbResolution.InsertString(index, "640*720(half 720p)");
    m_cmbResolution.SetItemData(index, 112);
    index++;

    m_cmbResolution.InsertString(index, "640*960");
    m_cmbResolution.SetItemData(index, 113);
    index++;
    m_cmbResolution.InsertString(index, "320*480");
    m_cmbResolution.SetItemData(index, 114);
    index++;

    m_cmbResolution.InsertString(index, "3840*2400");
    m_cmbResolution.SetItemData(index, 115);
    index++;

    m_cmbResolution.InsertString(index, "3840*1680");
    m_cmbResolution.SetItemData(index, 116);
    index++;
    m_cmbResolution.InsertString(index, "2560*1120");
    m_cmbResolution.SetItemData(index, 117);
    index++;

    m_cmbResolution.InsertString(index, "704*320");
    m_cmbResolution.SetItemData(index, 118);
    index++;

    m_cmbResolution.InsertString(index, "1200*1920");
    m_cmbResolution.SetItemData(index, 119);
    index++;

    m_cmbResolution.InsertString(index, "480*768");
    m_cmbResolution.SetItemData(index, 120);
    index++;
    m_cmbResolution.InsertString(index, "768*480");
    m_cmbResolution.SetItemData(index, 121);
    index++;

    m_cmbResolution.InsertString(index, "320*512");
    m_cmbResolution.SetItemData(index, 122);
    index++;

    m_cmbResolution.InsertString(index, "512*320");
    m_cmbResolution.SetItemData(index, 123);
    index++;

    m_cmbResolution.InsertString(index, "4096*1800");
    m_cmbResolution.SetItemData(index, 124);
    index++;

    m_cmbResolution.InsertString(index, "1280*560");
    m_cmbResolution.SetItemData(index, 125);
    index++;
    m_cmbResolution.InsertString(index, "2400*3840");
    m_cmbResolution.SetItemData(index, 126);
    index++;

    m_cmbResolution.InsertString(index, "480*272");
    m_cmbResolution.SetItemData(index, 127);
    index++;

    m_cmbResolution.InsertString(index, "512*272");
    m_cmbResolution.SetItemData(index, 128);
    index++;

    m_cmbResolution.InsertString(index, "2592*2592");
    m_cmbResolution.SetItemData(index, 129);
    index++;

    m_cmbResolution.InsertString(index, "1792*2880");
    m_cmbResolution.SetItemData(index, 130);
    index++;

    m_cmbResolution.InsertString(index, "1600*2560");
    m_cmbResolution.SetItemData(index, 131);
    index++;

    m_cmbResolution.InsertString(index, "2720*1192");
    m_cmbResolution.SetItemData(index, 132);
    index++;

    m_cmbResolution.InsertString(index, "2560*960");
    m_cmbResolution.SetItemData(index, 135);
    index++;

    m_cmbResolution.InsertString(index, "2064*1544");
    m_cmbResolution.SetItemData(index, 136);
    index++;

    m_cmbResolution.InsertString(index, "4096*1200");
    m_cmbResolution.SetItemData(index, 137);
    index++;

    m_cmbResolution.InsertString(index, "3840*1080");
    m_cmbResolution.SetItemData(index, 138);
    index++;

    m_cmbResolution.InsertString(index, "2720*800");
    m_cmbResolution.SetItemData(index, 139);
    index++;

    m_cmbResolution.InsertString(index, "512*232");
    m_cmbResolution.SetItemData(index, 140);
    index++;

    m_cmbResolution.InsertString(index, "704*200");
    m_cmbResolution.SetItemData(index, 141);
    index++;

    m_cmbResolution.InsertString(index, "512*152");
    m_cmbResolution.SetItemData(index, 142);
    index++;

    m_cmbResolution.InsertString(index, "2048*896");
    m_cmbResolution.SetItemData(index, 143);
    index++;

    m_cmbResolution.InsertString(index, "2048*600");
    m_cmbResolution.SetItemData(index, 144);
    index++;

    m_cmbResolution.InsertString(index, "1280*376");
    m_cmbResolution.SetItemData(index, 145);
    index++;

    m_cmbResolution.InsertString(index, "2592*1520");
    m_cmbResolution.SetItemData(index, 146);
    index++;

    m_cmbResolution.InsertString(index, "2592*1536");
    m_cmbResolution.SetItemData(index, 147);
    index++;

    m_cmbResolution.InsertString(index, "3072*8192");
    m_cmbResolution.SetItemData(index, 148);
    index++;

    m_cmbResolution.InsertString(index, "768*2048");
    m_cmbResolution.SetItemData(index, 149);
    index++;

    m_cmbResolution.InsertString(index, "8208*3072");
    m_cmbResolution.SetItemData(index, 150);
    index++;

    m_cmbResolution.InsertString(index, "4096*1536");
    m_cmbResolution.SetItemData(index, 151);
    index++;

    m_cmbResolution.InsertString(index, "6912*2800");
    m_cmbResolution.SetItemData(index, 152);
    index++;

    m_cmbResolution.InsertString(index, "3456*1400");
    m_cmbResolution.SetItemData(index, 153);
    index++;

    m_cmbResolution.InsertString(index, "480*720");
    m_cmbResolution.SetItemData(index, 154);
    index++;

    m_cmbResolution.InsertString(index, "800*450");
    m_cmbResolution.SetItemData(index, 155);
    index++;

    m_cmbResolution.InsertString(index, "480*270");
    m_cmbResolution.SetItemData(index, 156);
    index++;

    m_cmbResolution.InsertString(index, "2560*1536");
    m_cmbResolution.SetItemData(index, 157);
    index++;

    m_cmbResolution.InsertString(index, "1456*1456");
    m_cmbResolution.SetItemData(index, 158);
    index++;

    m_cmbResolution.InsertString(index, "3026*3026");
    m_cmbResolution.SetItemData(index, 159);
    index++;

    m_cmbResolution.InsertString(index, "3264*2448");
    m_cmbResolution.SetItemData(index, 160);
    index++;

    m_cmbResolution.InsertString(index, "288*320");
    m_cmbResolution.SetItemData(index, 161);
    index++;

    m_cmbResolution.InsertString(index, "144*176");
    m_cmbResolution.SetItemData(index, 162);
    index++;

    m_cmbResolution.InsertString(index, "480*640");
    m_cmbResolution.SetItemData(index, 163);
    index++;

    m_cmbResolution.InsertString(index, "240*320");
    m_cmbResolution.SetItemData(index, 164);
    index++;

    m_cmbResolution.InsertString(index, "120*160");
    m_cmbResolution.SetItemData(index, 165);
    index++;

    m_cmbResolution.InsertString(index, "576*720");
    m_cmbResolution.SetItemData(index, 166);
    index++;

    m_cmbResolution.InsertString(index, "720*1280");
    m_cmbResolution.SetItemData(index, 167);
    index++;

    m_cmbResolution.InsertString(index, "576*960");
    m_cmbResolution.SetItemData(index, 168);
    index++;

    m_cmbResolution.InsertString(index, "2944*1656");
    m_cmbResolution.SetItemData(index, 169);
    index++;

    m_cmbResolution.InsertString(index, "432*240");
    m_cmbResolution.SetItemData(index, 170);
    index++;

    m_cmbResolution.InsertString(index, "2160*3840");
    m_cmbResolution.SetItemData(index, 171);
    index++;

    m_cmbResolution.InsertString(index, "1080*1920");
    m_cmbResolution.SetItemData(index, 172);
    index++;

    m_cmbResolution.InsertString(index, "7008*1080");
    m_cmbResolution.SetItemData(index, 173);
    index++;

    m_cmbResolution.InsertString(index, "3504*540");
    m_cmbResolution.SetItemData(index, 174);
    index++;

    m_cmbResolution.InsertString(index, "1752*270");
    m_cmbResolution.SetItemData(index, 175);
    index++;

    m_cmbResolution.InsertString(index, "876*135");
    m_cmbResolution.SetItemData(index, 176);
    index++;

    m_cmbResolution.InsertString(index, "4096*1440");
    m_cmbResolution.SetItemData(index, 177);
    index++;

    m_cmbResolution.InsertString(index, "4096*1080");
    m_cmbResolution.SetItemData(index, 178);
    index++;

    m_cmbResolution.InsertString(index, "1536*864");
    m_cmbResolution.SetItemData(index, 179);
    index++;

    m_cmbResolution.InsertString(index, "3200*1800");
    m_cmbResolution.SetItemData(index, 186);
    index++;

    m_cmbResolution.InsertString(index, "1752*272");
    m_cmbResolution.SetItemData(index, 187);
    index++;

    m_cmbResolution.InsertString(index, "872*136");
    m_cmbResolution.SetItemData(index, 188);
    index++;

    m_cmbResolution.InsertString(index, "1280*1440");
    m_cmbResolution.SetItemData(index, 189);
    index++;

    m_cmbResolution.InsertString(index, "3520*544");
    m_cmbResolution.SetItemData(index, 190);
    index++;

    m_cmbResolution.InsertString(index, "1728*256");
    m_cmbResolution.SetItemData(index, 191);
    index++;

    m_cmbResolution.InsertString(index, "864*128");
    m_cmbResolution.SetItemData(index, 192);
    index++;

    m_cmbResolution.InsertString(index, "5470*3684");
    m_cmbResolution.SetItemData(index, 193);
    index++;

    m_cmbResolution.InsertString(index, "2560*750");
    m_cmbResolution.SetItemData(index, 194);
    index++;

    m_cmbResolution.InsertString(index, "1600*480");
    m_cmbResolution.SetItemData(index, 195);
    index++;

    m_cmbResolution.InsertString(index, "1024*300");
    m_cmbResolution.SetItemData(index, 196);
    index++;

    m_cmbResolution.InsertString(index, "848*480");
    m_cmbResolution.SetItemData(index, 197);
    index++;

    m_cmbResolution.InsertString(index, "8160*3616");
    m_cmbResolution.SetItemData(index, 198);
    index++;

    m_cmbResolution.InsertString(index, "8160*2304");
    m_cmbResolution.SetItemData(index, 199);
    index++;

    m_cmbResolution.InsertString(index, "4064*2560");
    m_cmbResolution.SetItemData(index, 200);
    index++;

    m_cmbResolution.InsertString(index, "1400*1050");
    m_cmbResolution.SetItemData(index, 201);
    index++;

    m_cmbResolution.InsertString(index, "2688*1512");
    m_cmbResolution.SetItemData(index, 202);
    index++;

    m_cmbResolution.InsertString(index, "704*288");
    m_cmbResolution.SetItemData(index, 203);
    index++;

    m_cmbResolution.InsertString(index, "1560*656");
    m_cmbResolution.SetItemData(index, 204);
    index++;

    m_cmbResolution.InsertString(index, "3072*1280");
    m_cmbResolution.SetItemData(index, 205);
    index++;

    m_cmbResolution.InsertString(index, "4320*1808");
    m_cmbResolution.SetItemData(index, 206);
    index++;

    m_cmbResolution.InsertString(index, "6120*2560");
    m_cmbResolution.SetItemData(index, 207);
    index++;

    m_cmbResolution.InsertString(index, "1280*960");
    m_cmbResolution.SetItemData(index, 208);
    index++;

    m_cmbResolution.InsertString(index, "3000*3000");
    m_cmbResolution.SetItemData(index, 209);
    index++;

    m_cmbResolution.InsertString(index, "2032*3616");
    m_cmbResolution.SetItemData(index, 210);
    index++;

    m_cmbResolution.InsertString(index, "7680*4320");
    m_cmbResolution.SetItemData(index, 211);
    index++;

    m_cmbResolution.InsertString(index, "5760*3240");
    m_cmbResolution.SetItemData(index, 212);
    index++;

    m_cmbResolution.InsertString(index, "3264*1836");
    m_cmbResolution.SetItemData(index, 213);
    index++;

    m_cmbResolution.InsertString(index, "2712*1536");
    m_cmbResolution.SetItemData(index, 214);
    index++;

    m_cmbResolution.InsertString(index, "1080*720");
    m_cmbResolution.SetItemData(index, 215);
    index++;

    m_cmbResolution.InsertString(index, "360x640");
    m_cmbResolution.SetItemData(index, 216);
    index++;

    m_cmbResolution.InsertString(index, "1440*812");
    m_cmbResolution.SetItemData(index, 217);
    index++;

    m_cmbResolution.InsertString(index, "8160*2416");
    m_cmbResolution.SetItemData(index, 218);
    index++;

    m_cmbResolution.InsertString(index, "5760*1712");
    m_cmbResolution.SetItemData(index, 219);
    index++;

    m_cmbResolution.InsertString(index, "4080*1200");
    m_cmbResolution.SetItemData(index, 220);
    index++;

    m_cmbResolution.InsertString(index, "6144*2560");
    m_cmbResolution.SetItemData(index, 221);
    index++;

    m_cmbResolution.InsertString(index, "1568*656");
    m_cmbResolution.SetItemData(index, 222);
    index++;

    m_cmbResolution.InsertString(index, "4096*1808");
    m_cmbResolution.SetItemData(index, 223);
    index++;

    m_cmbResolution.InsertString(index, "2720*1200");
    m_cmbResolution.SetItemData(index, 224);
    index++;

    m_cmbResolution.InsertString(index, "2896*848");
    m_cmbResolution.SetItemData(index, 225);
    index++;

    m_cmbResolution.InsertString(index, "2048*608");
    m_cmbResolution.SetItemData(index, 226);
    index++;

    m_cmbResolution.InsertString(index, "3840*1120");
    m_cmbResolution.SetItemData(index, 227);
    index++;

    m_cmbResolution.InsertString(index, "2720*784");
    m_cmbResolution.SetItemData(index, 228);
    index++;

    m_cmbResolution.InsertString(index, "1920*560");
    m_cmbResolution.SetItemData(index, 229);
    index++;

    m_cmbResolution.InsertString(index, "1360*400");
    m_cmbResolution.SetItemData(index, 230);
    index++;

    m_cmbResolution.InsertString(index, "704*208");
    m_cmbResolution.SetItemData(index, 231);
    index++;

    m_cmbResolution.InsertString(index, "496*144");
    m_cmbResolution.SetItemData(index, 232);
    index++;

    m_cmbResolution.InsertString(index, "352*128");
    m_cmbResolution.SetItemData(index, 233);
    index++;

    m_cmbResolution.InsertString(index, "512*240");
    m_cmbResolution.SetItemData(index, 234);
    index++;

    m_cmbResolution.InsertString(index, "512*160");
    m_cmbResolution.SetItemData(index, 235);
    index++;

    m_cmbResolution.InsertString(index, "368*128");
    m_cmbResolution.SetItemData(index, 236);
    index++;

    m_cmbResolution.InsertString(index, "256*128");
    m_cmbResolution.SetItemData(index, 237);
    index++;

    m_cmbResolution.InsertString(index, "2048*592");
    m_cmbResolution.SetItemData(index, 238);
    index++;

    m_cmbResolution.InsertString(index, "1024*304");
    m_cmbResolution.SetItemData(index, 240);
    index++;

    m_cmbResolution.InsertString(index, "1280*368");
    m_cmbResolution.SetItemData(index, 241);
    index++;

    m_cmbResolution.InsertString(index, "912*256");
    m_cmbResolution.SetItemData(index, 242);
    index++;

    m_cmbResolution.InsertString(index, "640*192");
    m_cmbResolution.SetItemData(index, 243);
    index++;

    m_cmbResolution.InsertString(index, "252*112 （未使用）");
    m_cmbResolution.SetItemData(index, 244);
    index++;

    m_cmbResolution.InsertString(index, "(576*704[P制]/480*704[N制])");
    m_cmbResolution.SetItemData(index, 245);
    index++;

    m_cmbResolution.InsertString(index, "5760*1696");
    m_cmbResolution.SetItemData(index, 246);
    index++;

    m_cmbResolution.InsertString(index, "8160*2400");
    m_cmbResolution.SetItemData(index, 247);
    index++;

    m_cmbResolution.InsertString(index, "5520*2400");
    m_cmbResolution.SetItemData(index, 248);
    index++;

    m_cmbResolution.InsertString(index, "2784*800");
    m_cmbResolution.SetItemData(index, 249);
    index++;

    m_cmbResolution.InsertString(index, "1968*848");
    m_cmbResolution.SetItemData(index, 250);
    index++;

    m_cmbResolution.InsertString(index, "1392*608");
    m_cmbResolution.SetItemData(index, 251);
    index++;

    m_cmbResolution.InsertString(index, "2736*1184");
    m_cmbResolution.SetItemData(index, 252);
    index++;

    m_cmbResolution.InsertString(index, "1920*848");
    m_cmbResolution.SetItemData(index, 253);
    index++;

    m_cmbResolution.InsertString(index, "Auto");
    m_cmbResolution.SetItemData(index, 0xff);
    index++;
}

void CSubDlgChanCfg::OnCbnSelchangeComboCompressionType()
{
    // TODO: Add your control notification handler code here
    CurCfgUpdate();
}

void CSubDlgChanCfg::CurCfgSetup()
{
    if(!CheckInitParam())
    {
        return;
    }
    UpdateData(TRUE);
    m_struCompressionCond.dwSize = sizeof(m_struCompressionCond);
    m_struCompressionCond.dwChannelNum = g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChannelNO;
    m_struCompressionCond.byCompressionType = (BYTE)m_cmbCompressionType.GetCurSel() + 1;
    m_struCompressionCfg.byPicQuality = (BYTE)m_cmbImageQuality.GetCurSel();
    m_struCompressionCfg.byStreamType = (BYTE)m_cmbStreamType.GetCurSel();
    m_struCompressionCfg.dwResolution = (BYTE)m_cmbResolution.GetItemData(m_cmbResolution.GetCurSel());
    m_struCompressionCfg.byBitRateType = (BYTE)m_cmbBitrateType.GetCurSel();
    m_struCompressionCfg.dwVideoFrameRate = m_cmbFrameRate.GetCurSel();
    m_struCompressionCfg.dwVideoBitRate = m_cmbMaxBitrate.GetCurSel();
    m_struCompressionCfg.dwMaxBitRate = m_dwSelfDefBitrate;
    m_struCompressionCfg.wIntervalFrameI = (WORD)m_iInteralFrameI;
    m_struCompressionCfg.byIntervalBPFrame = (BYTE)m_iInteralFrameBP;

    //目前没有要进行字符转换的字段

    //设置压缩参数
    NET_EHOME_CONFIG struCfg = {0};
    struCfg.pCondBuf = &m_struCompressionCond;
    struCfg.dwCondSize = sizeof(m_struCompressionCond);
    struCfg.pInBuf = &m_struCompressionCfg;
    struCfg.dwInSize = sizeof(m_struCompressionCfg);
    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_COMPRESSION_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_COMPRESSION_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_COMPRESSION_CFG");
    }
    //设置图像参数
    m_struImageCfg.byBright = (BYTE)m_iBright;
    m_struImageCfg.byContrast = (BYTE)m_iContrast;
    m_struImageCfg.bySaturation = (BYTE)m_iSaturation;
    m_struImageCfg.byHue = (BYTE)m_iHue;

    //目前没有要进行字符转换的字段

    memset(&struCfg, 0, sizeof(struCfg));
    struCfg.pCondBuf = &g_struDeviceInfo[m_iDeviceIndex].struChanInfo[m_iChanIndex].iChannelNO;
    struCfg.dwCondSize = 4;
    struCfg.pInBuf = &m_struImageCfg;
    struCfg.dwInSize = sizeof(m_struImageCfg);
    if (NET_ECMS_SetDevConfig(m_lLoginID, NET_EHOME_SET_IMAGE_CFG, &struCfg, sizeof(struCfg)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, 1, "NET_EHOME_SET_IMAGE_CFG");
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, 1, "NET_EHOME_SET_IMAGE_CFG");
    }
}

void CSubDlgChanCfg::OnCbnSelchangeComboMaxBitRate()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_cmbMaxBitrate.GetCurSel() == m_cmbMaxBitrate.GetCount()-1)
    {
        GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_EDIT_BIT_RATE)->EnableWindow(FALSE);
    }
}


void CSubDlgChanCfg::OnSelchangeComboBitRateType()
{
    // TODO:  在此添加控件通知处理程序代码
    if (m_cmbBitrateType.GetCurSel() == 1)
    {
        GetDlgItem(IDC_COMBO_IMAGE_QUALITY)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_COMBO_IMAGE_QUALITY)->EnableWindow(TRUE);
    }
}
