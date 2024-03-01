// DlgIPSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgIPSelect.h"
#include "afxdialogex.h"

#include <WinSock2.h>
#include <Iphlpapi.h>
#include <ws2tcpip.h>

#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib,"ws2_32.lib")

// DlgIPSelect 对话框

IMPLEMENT_DYNAMIC(DlgIPSelect, CDialog)

DlgIPSelect::DlgIPSelect(CWnd* pParent /*=NULL*/)
	: CDialog(DlgIPSelect::IDD, pParent)
{
    m_nPort = 7660;
}

DlgIPSelect::~DlgIPSelect()
{
}

void DlgIPSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COM_IP, m_cmIP);
    DDX_Text(pDX, IDC_EDT_PORT, m_nPort);
    DDX_Text(pDX, IDC_COM_IP, m_csIP);
}


BEGIN_MESSAGE_MAP(DlgIPSelect, CDialog)
    ON_BN_CLICKED(IDOK, &DlgIPSelect::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgIPSelect 消息处理程序


BOOL DlgIPSelect::OnInitDialog()
{
    CDialog::OnInitDialog();

    UpdateData(FALSE);

    //GetLocalIP();
    GetLocalIPEx();

    return TRUE;
}

void DlgIPSelect::OnBnClickedOk()
{
    // TODO:  在此添加控件通知处理程序代码
    UpdateData(TRUE);

    CDialog::OnOK();
}

void DlgIPSelect::GetLocalIP()
{
    //PIP_ADAPTER_INFO结构体指针存储本机网卡信息
    BYTE *pArray = NULL;
    PIP_ADAPTER_INFO pIpAdapterInfoTemp = NULL;

    pIpAdapterInfoTemp = new(std::nothrow) IP_ADAPTER_INFO();
    if (pIpAdapterInfoTemp == NULL)
    {
        return;
    }
    PIP_ADAPTER_INFO pIpAdapterInfo = pIpAdapterInfoTemp;
    //得到结构体大小,用于GetAdaptersInfo参数
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);
    //调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
    int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        //如果函数返回的是ERROR_BUFFER_OVERFLOW
        //则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
        //这也是说明为什么stSize既是一个输入量也是一个输出量

        //重新申请内存空间用来存储所有网卡信息
        pArray = new(std::nothrow) BYTE[stSize];
        if (pArray == NULL)
        {
            //释放原先申请的内存
            if (pIpAdapterInfoTemp)
            {
                delete pIpAdapterInfoTemp;
                pIpAdapterInfoTemp = NULL;
            }

            return;
        }
        pIpAdapterInfo = (IP_ADAPTER_INFO*)pArray;

        //再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
        nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    }

    if (ERROR_SUCCESS == nRel)
    {
        m_cmIP.AddString("0.0.0.0");
        //输出网卡信息
        while (pIpAdapterInfo)
        {
            IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
            do
            {
                CString csIp;
                csIp.Format("%s", pIpAddrString->IpAddress.String);
                m_cmIP.AddString(csIp);
                pIpAddrString = pIpAddrString->Next;
            } while (pIpAddrString);
            pIpAdapterInfo = pIpAdapterInfo->Next;
        }
    }

    //释放内存空间
    if (pIpAdapterInfoTemp)
    {
        delete pIpAdapterInfoTemp;
        pIpAdapterInfoTemp = NULL;
    }

    if (pArray)
    {
        delete[]pArray;
        pArray = NULL;
    }

    m_cmIP.SetCurSel(0);

}

//可以获取ipv4和ipv6地址
void DlgIPSelect::DoGetIP()
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    struct sockaddr_in6 *sinp6;
    PHOSTENT hostinfo;
    char szHostName[255] = { 0 }; //主机名
    char *pPort = "3294";      //端口号
    const char *addr;

    gethostname(szHostName, sizeof(szHostName));
    if ((hostinfo = gethostbyname(szHostName)) == NULL) //获得本地ipv4地址
    {
        return;
    }

    LPCSTR ipv4;
    m_cmIP.AddString("0.0.0.0");
    while (*(hostinfo->h_addr_list) != NULL) //输出ipv4地址
    {
        ipv4 = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);

        CString csIp;
        csIp.Format("%s", ipv4);
        m_cmIP.AddString(csIp);

        hostinfo->h_addr_list++;
    }

    hint.ai_family = AF_INET6;        //hint 的限定设置
    hint.ai_socktype = SOCK_STREAM;   //这里可是设置 socket type    比如  SOCK_DGRAM
    hint.ai_flags = AI_PASSIVE;       // flags 的标志很多。常用的有AI_CANONNAME;
    hint.ai_protocol = 0;             //设置协议  一般为0，默认
    hint.ai_addrlen = 0;              //下面不可以设置，为0，或者为NULL
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    int ilRc = getaddrinfo(szHostName, pPort, &hint, &ailist); //通过主机名获得地址信息
    if (ilRc < 0)
    {
        return;
    }

    if (ailist == NULL)
    {
        //继续
    }

    char ipv6[128] = { 0 };
    for (aip = ailist; aip != NULL; aip = aip->ai_next) //显示获取的信息
    {
        CString csIp;
        memset(ipv6, 0, 128);

        aip->ai_family == AF_INET6;
        sinp6 = (struct sockaddr_in6 *)aip->ai_addr;
        for (int i = 0; i < 16; i++)
        {
            if (((i - 1) % 2) && (i > 0))
            {
                sprintf(ipv6, "%s:", ipv6);
            }
            sprintf(ipv6, "%s%02x", ipv6, sinp6->sin6_addr.u.Byte[i]);
        }

        csIp.Format("%s", ipv6);
        m_cmIP.AddString(csIp);
    }
}

void DlgIPSelect::GetLocalIPEx()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(1, 1);

    int err = WSAStartup(wVersionRequested, &wsaData);//initiate the ws2_32.dll and match the version
    if (err != 0)
    {
        return;
    }

    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)//if the version is not matched ,then quit and terminate the ws3_32.dll
    {
        WSACleanup();
        return;
    }

    DoGetIP();
    m_cmIP.SetCurSel(0);

    WSACleanup();
    return;
}
