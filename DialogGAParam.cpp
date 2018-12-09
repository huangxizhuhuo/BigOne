// DialogEAParam.cpp : 实现文件
//

#include "stdafx.h"
#include "BigOne.h"
#include "DialogGAParam.h"

#include "ALG\\ea.h"

// CDialogEAParam 对话框

IMPLEMENT_DYNAMIC(CDialogGAParam, CDialog)

CDialogGAParam::CDialogGAParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGAParam::IDD, pParent)
	, m_dCrossoverRate(0)
	, m_dMutateRate(0)
{

}

CDialogGAParam::~CDialogGAParam()
{
}

void CDialogGAParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CROSSOVERRATE, m_dCrossoverRate);
	DDX_Text(pDX, IDC_EDIT_MUTATERATE, m_dMutateRate);
	DDX_Control(pDX, IDC_COMBO_CROSSOVER, m_cbCrossoverOperator);
	DDX_Control(pDX, IDC_COMBO_MUTATE, m_cbMutateOperator);
	DDX_Control(pDX, IDC_CHECK_ELITISM, m_cElitism);
	DDX_Control(pDX, IDC_COMBO_N, m_cbNum);
	DDX_Control(pDX, IDC_COMBO_COPIES, m_cbCopies);
	DDX_Control(pDX, IDC_COMBO_SCALE, m_cbScale);
	DDX_Control(pDX, IDC_CHECK_SCALE, m_cScale);
	DDX_Control(pDX, IDC_COMBO_SELECT, m_cbSelect);
}


BEGIN_MESSAGE_MAP(CDialogGAParam, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDialogGAParam::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDialogGAParam::OnBnClickedButtonCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_N, &CDialogGAParam::OnCbnSelchangeComboN)
	ON_BN_CLICKED(IDC_CHECK_ELITISM, &CDialogGAParam::OnBnClickedCheckElitism)
	ON_BN_CLICKED(IDC_CHECK_SCALE, &CDialogGAParam::OnBnClickedCheckScale)
END_MESSAGE_MAP()


// CDialogEAParam 消息处理程序

void CDialogGAParam::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_CrossoverRate = m_dCrossoverRate;
	g_MutateRate = m_dMutateRate;
	g_CrossoverOperator = m_cbCrossoverOperator.GetCurSel();
	g_MutateOperator = m_cbMutateOperator.GetCurSel();
	g_Elitism = m_cElitism.GetCheck()? true :false;
	g_Scale   = m_cScale.GetCheck() ? true:false;
	g_Num = GetDlgItemInt(IDC_COMBO_N);
	g_Copies = GetDlgItemInt(IDC_COMBO_COPIES);
	g_ScaleMethod = m_cbScale.GetCurSel();
	g_SelectMethod = m_cbSelect.GetCurSel();
	CDialog::OnOK();
}

void CDialogGAParam::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

BOOL CDialogGAParam::OnInitDialog()
{
	CDialog::OnInitDialog();
	WCHAR wstr[50];
	// TODO:  在此添加额外的初始化
	for(int i=1; i<=(int)g_PopNum; i++)
	{
		swprintf_s(wstr,L"%d",i);	
		m_cbNum.AddString(wstr);
		//m_cbNum.SetItemData(i-1,i);
	}
	for(int i=1; i<=(int)(g_PopNum/g_Num); i++)
	{
		swprintf_s(wstr,L"%d",i);
		m_cbCopies.AddString(wstr);
	}
	m_cbCrossoverOperator.SetCurSel(g_CrossoverOperator);
	m_cbMutateOperator.SetCurSel(g_MutateOperator);
	m_dCrossoverRate = g_CrossoverRate;
	m_dMutateRate = g_MutateRate;
	m_cElitism.SetCheck(g_Elitism);
	m_cScale.SetCheck(g_Scale);
	
	SetDlgItemInt(IDC_COMBO_N,(UINT)g_Num);
	SetDlgItemInt(IDC_COMBO_COPIES,(UINT)g_Copies);
	m_cbScale.SetCurSel(g_ScaleMethod);
	m_cbSelect.SetCurSel(g_SelectMethod);
	if(!g_Elitism)
	{
		GetDlgItem(IDC_COMBO_N)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COPIES)->EnableWindow(FALSE);
	}
	if(!g_Scale)
	{
		GetDlgItem(IDC_COMBO_SCALE)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogGAParam::OnCbnSelchangeComboN()
{
	// TODO: 在此添加控件通知处理程序代
	WCHAR wstr[100];	
	CHAR str[100];
	UINT count = (UINT)m_cbCopies.GetCount();
	while(count--)
	{
		m_cbCopies.DeleteString(0);
	}	
	//tempNum = GetDlgItemInt(IDC_COMBO_N);
	CString strNum;
	m_cbNum.GetLBText(m_cbNum.GetCurSel(),strNum);
	WideCharToMultiByte(CP_ACP,0,strNum.GetBuffer(),-1,str,100,NULL,NULL);
	size_t tempNum =atoi(str);
	for(int i=1; i<=(int)(g_PopNum/tempNum); i++)
	{
		swprintf_s(wstr,L"%d",i);
		m_cbCopies.AddString(wstr);
	}
}

void CDialogGAParam::OnBnClickedCheckElitism()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bElitism = m_cElitism.GetCheck() ? true :false;
	if(bElitism)
	{
		GetDlgItem(IDC_COMBO_N)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_COPIES)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_N)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_COPIES)->EnableWindow(FALSE);
	}
}

void CDialogGAParam::OnBnClickedCheckScale()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bScale = m_cScale.GetCheck() ? true :false;
	if(bScale)
	{
		GetDlgItem(IDC_COMBO_SCALE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_SCALE)->EnableWindow(FALSE);
	}
}
