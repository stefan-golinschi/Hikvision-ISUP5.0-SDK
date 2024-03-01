#pragma once
#include "EHomeDemo.h"
#include "Public/Convert/Convert.h"
#include "Public/TinyXML/XmlBase.h"

// CDlgWirelessInfoPara 对话框

class CDlgWirelessInfoPara : public CDialog
{
	DECLARE_DYNAMIC(CDlgWirelessInfoPara)

public:
	CDlgWirelessInfoPara(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWirelessInfoPara();

// 对话框数据
	enum { IDD = IDD_DLG_WIRELESSINFO_UPLOAD_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
    int     m_lUserID;
    char    m_szCommand[32];
    char    m_szInputXml[MAX_EHOME_PROTOCOL_LEN];
    char    m_szOutputXml[MAX_EHOME_PROTOCOL_LEN];
    NET_EHOME_XML_CFG    m_struCfg;
    NET_EHOME_WIRELESS_INFO_CFG m_struWirelessInfo;
public:
    int m_iInternal;
    BOOL m_bEnable;
    afx_msg void OnBnClickedBtnGet();
    afx_msg void OnBnClickedBtnSet();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnGetWirelessinfo();
    afx_msg void OnBnClickedBtnSetWirelessinfo();
};
