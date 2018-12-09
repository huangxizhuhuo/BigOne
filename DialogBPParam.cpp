// DialogBPParam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BigOne.h"
#include "DialogBPParam.h"

double g_Lr= 0.01;
// CDialogBPParam �Ի���

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


// CDialogBPParam ��Ϣ�������

void CDialogBPParam::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	g_Lr = m_dLr;
	OnOK();
}

BOOL CDialogBPParam::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_dLr = g_Lr;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
