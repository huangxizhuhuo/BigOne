// DialogBPParam.cpp : 实现文件
//

#include "stdafx.h"
#include "BigOne.h"
#include "DialogBPParam.h"

double g_Lr= 0.01;
// CDialogBPParam 对话框

IMPLEMENT_DYNAMIC(CDialogBPParam, CDialog)

CDialogBPParam::CDialogBPParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBPParam::IDD, pParent)
	, m_dLr(0)
{

}

CDialogBPParam::~CDialogBPParam()
{
}

void CDialogBPParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LR, m_dLr);
}


BEGIN_MESSAGE_MAP(CDialogBPParam, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogBPParam::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogBPParam 消息处理程序

void CDialogBPParam::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_Lr = m_dLr;
	OnOK();
}

BOOL CDialogBPParam::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_dLr = g_Lr;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
