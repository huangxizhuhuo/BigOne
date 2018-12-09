// DialogEABPParam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BigOne.h"
#include "DialogEABPParam.h"
#include "DialogGAParam.h"


int g_EabpAlg = 0;
double g_EabpErr = 0.1;
int g_EabpGen = 100;
// CDialogEABPParam �Ի���

IMPLEMENT_DYNAMIC(CDialogEABPParam, CDialog)

CDialogEABPParam::CDialogEABPParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEABPParam::IDD, pParent)
	, m_dError(0)
	, m_iGen(0)
{

}

CDialogEABPParam::~CDialogEABPParam()
{
}

void CDialogEABPParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EAALG, m_cbEaAlg);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_dError);
	DDX_Text(pDX, IDC_EDIT_GEN, m_iGen);
}


BEGIN_MESSAGE_MAP(CDialogEABPParam, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogEABPParam::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_EAPARAM, &CDialogEABPParam::OnBnClickedButtonEaparam)
END_MESSAGE_MAP()


// CDialogEABPParam ��Ϣ�������

BOOL CDialogEABPParam::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_cbEaAlg.SetCurSel(g_EabpAlg);
	m_dError = g_EabpErr;
	m_iGen   = g_EabpGen;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDialogEABPParam::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	g_EabpAlg = m_cbEaAlg.GetCurSel();
	g_EabpErr = m_dError;
	g_EabpGen = m_iGen ;
	OnOK();
}

void CDialogEABPParam::OnBnClickedButtonEaparam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_cbEaAlg.GetCurSel() == 0)
	{
		CDialogGAParam dlg;
		dlg.DoModal();
	}
}
