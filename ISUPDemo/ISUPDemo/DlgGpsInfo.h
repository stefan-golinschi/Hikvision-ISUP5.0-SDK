#pragma once
#include "EHomeDemo.h"
#include "Public/Convert/Convert.h"
#include "Public/TinyXML/XmlBase.h"

// CDlgGpsInfo 对话框

typedef struct _NET_EHOME_GPS_PARAM
{
    int iInteralTime;
    int iMaxSpeed;
    int iMinSpeed;
    char bEnable;
}NET_EHOME_GPS_PARAM, *LPNET_EHOME_GPS_PARAM;

class CDlgGpsInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgGpsInfo)

public:
	CDlgGpsInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGpsInfo();

// 对话框数据
	enum { IDD = IDD_DLG_GPS_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    int m_iInteral;
    int m_iMaxSpeed;
    int m_iMinSpeed;
    BOOL m_bEnable;
    afx_msg void OnBnClickedBtnGet();
private:
    int     m_lUserID;
    char    m_szCommand[32];
    char    m_szInputXml[MAX_EHOME_PROTOCOL_LEN];
    char    m_szOutputXml[MAX_EHOME_PROTOCOL_LEN];
    NET_EHOME_XML_CFG    m_struCfg;
    NET_EHOME_GPS_PARAM   m_struGPSInfo;
public:
    afx_msg void OnBnClickedBtnSet();
    virtual BOOL OnInitDialog();
};
