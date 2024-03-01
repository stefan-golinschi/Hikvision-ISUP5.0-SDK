// DlgAlarmChanList.cpp : 实现文件
//

#include "stdafx.h"
#include "EHomeDemo.h"
#include "DlgAlarmChanList.h"


// CDlgAlarmChanList 对话框

IMPLEMENT_DYNAMIC(CDlgAlarmChanList, CDialog)

CDlgAlarmChanList::CDlgAlarmChanList(CWnd* pParent /*=NULL*/)
: CDialog(CDlgAlarmChanList::IDD, pParent)
{
    m_strDlgTitle = _T("");
    m_byDataType = (BYTE)-1;
    m_byNum = 0;
    memset(m_byAnalogAlarmOut, 0, MAX_ANALOG_ALARMOUT);
    memset(m_byAnalogAlarmChan, 0, MAX_ANALOG_CHANNUM);
    memset(m_byDigitalAlarmChan, 0, MAX_DIGIT_CHANNUM);
    m_dwDigitalAlarmChan = 0;
}

CDlgAlarmChanList::~CDlgAlarmChanList()
{
}

void CDlgAlarmChanList::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE1, m_treeAlarmChan);
}


BEGIN_MESSAGE_MAP(CDlgAlarmChanList, CDialog)
    ON_BN_CLICKED(IDC_BTN_OK, &CDlgAlarmChanList::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgAlarmChanList::OnBnClickedBtnCancel)
    ON_NOTIFY(NM_CLICK, IDC_TREE1, &CDlgAlarmChanList::OnNMClickTree1)
    ON_UPDATE_COMMAND_UI(ID_MENU_WIRELESS_INFO, &CDlgAlarmChanList::OnUpdateMenuWirelessInfo)
END_MESSAGE_MAP()


// CDlgAlarmChanList 消息处理程序

BOOL CDlgAlarmChanList::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    SetWindowText(m_strDlgTitle);
    InitTreeList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CDlgAlarmChanList::InitTreeList()
{
    m_treeAlarmChan.ModifyStyle(TVS_CHECKBOXES, 0);
    m_treeAlarmChan.ModifyStyle(0, TVS_CHECKBOXES);
    HTREEITEM hTreeNode = NULL;
    CString strItemText = _T("");
    BYTE byItemData = 0;
    if (m_byDataType == 0)
    {
        for (int iNodeCnt = 0; iNodeCnt < MAX_ANALOG_ALARMOUT; iNodeCnt++)
        {
            strItemText.Format("AnalogAlarmOut %d", iNodeCnt + 1);
            byItemData = (BYTE)iNodeCnt;
            hTreeNode = m_treeAlarmChan.InsertItem(strItemText, TVI_ROOT, TVI_LAST);
            m_treeAlarmChan.SetItemData(hTreeNode, byItemData);
            if (m_byAnalogAlarmOut[iNodeCnt] == 1)
            {
                m_treeAlarmChan.SetCheck(hTreeNode);
            }
        }
    }
    else if (m_byDataType == 1)
    {
        for (int iNodeCnt = 0; iNodeCnt < MAX_ANALOG_CHANNUM; iNodeCnt++)
        {
            strItemText.Format("AnalogChan %d", iNodeCnt + 1);
            byItemData = (BYTE)iNodeCnt;
            hTreeNode = m_treeAlarmChan.InsertItem(strItemText, TVI_ROOT, TVI_LAST);
            m_treeAlarmChan.SetItemData(hTreeNode, byItemData);
            if (m_byAnalogAlarmChan[iNodeCnt] == 1)
            {
                m_treeAlarmChan.SetCheck(hTreeNode);
            }
        }
    }
    else
    {
        for (int iNodeCnt = 0; iNodeCnt < (int)m_dwDigitalAlarmChan; iNodeCnt++)
        {
            strItemText.Format("DigitalChan %d", iNodeCnt + 1);
            byItemData = (BYTE)iNodeCnt;
            hTreeNode = m_treeAlarmChan.InsertItem(strItemText, TVI_ROOT, TVI_LAST);
            m_treeAlarmChan.SetItemData(hTreeNode, byItemData);
            if (m_byDigitalAlarmChan[iNodeCnt] == 1)
            {
                m_treeAlarmChan.SetCheck(hTreeNode);
            }
        }
    }
}

void CDlgAlarmChanList::GetDataFromTreeList()
{
    UpdateData(TRUE);
    HTREEITEM hTreeNode = m_treeAlarmChan.GetRootItem();
    HTREEITEM hNextSibling = hTreeNode;
    BOOL bChecked = FALSE;
    int iCheckedCnt = 0;
    while (hNextSibling)
    {
        BYTE byItemData = (BYTE)m_treeAlarmChan.GetItemData(hNextSibling);
        if (m_byDataType == 0)
        {
            bChecked = m_treeAlarmChan.GetCheck(hNextSibling);
            if (bChecked)
            {
                if (iCheckedCnt++ >= m_byNum)
                {
                    break;
                }
            }
            m_byAnalogAlarmOut[byItemData] = (BYTE)bChecked;
        }
        else if (m_byDataType == 1)
        {
            bChecked = m_treeAlarmChan.GetCheck(hNextSibling);
            if (bChecked)
            {
                if (iCheckedCnt++ >= m_byNum)
                {
                    break;
                }
            }
            m_byAnalogAlarmChan[byItemData] = (BYTE)bChecked;
        }
        else
        {
            bChecked = m_treeAlarmChan.GetCheck(hNextSibling);
            if (bChecked)
            {
                if (iCheckedCnt++ >= m_byNum)
                {
                    break;
                }
            }
            m_byDigitalAlarmChan[byItemData] = (BYTE)bChecked;
        }
        hNextSibling = m_treeAlarmChan.GetNextSiblingItem(hNextSibling);
    }
    UpdateData(FALSE);
}

void CDlgAlarmChanList::OnBnClickedBtnOk()
{
    // TODO: 在此添加控件通知处理程序代码
    GetDataFromTreeList();
    return CDialog::OnOK();
}

void CDlgAlarmChanList::OnBnClickedBtnCancel()
{
    // TODO: 在此添加控件通知处理程序代码
    return CDialog::OnCancel();
}

void CDlgAlarmChanList::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    UN_REFERENCED_PARAMETER(pNMHDR)
    CPoint ptCursor;
    GetCursorPos(&ptCursor);
    m_treeAlarmChan.ScreenToClient(&ptCursor);
    HTREEITEM hClickedNode = m_treeAlarmChan.HitTest(ptCursor);
    if (hClickedNode)
    {
        m_treeAlarmChan.SelectItem(hClickedNode);
    }

    *pResult = 0;
}


void CDlgAlarmChanList::OnUpdateMenuWirelessInfo(CCmdUI *pCmdUI)
{
    // TODO:  在此添加命令更新用户界面处理程序代码
}
