#pragma once
#include "afxwin.h"


// CDialogEAParam �Ի���

class CDialogGAParam : public CDialog
{
	DECLARE_DYNAMIC(CDialogGAParam)

public:
	CDialogGAParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogGAParam();

// �Ի�������
	enum { IDD = IDD_DIALOG_GAPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double    m_dCrossoverRate;
	double    m_dMutateRate;
	CComboBox m_cbCrossoverOperator;
	CComboBox m_cbMutateOperator;
	CButton   m_cElitism;
	CComboBox m_cbNum;
	CComboBox m_cbCopies;
	CComboBox m_cbScale;
	CButton   m_cScale;
	CComboBox m_cbSelect;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnCbnSelchangeComboN();
	afx_msg void OnBnClickedCheckElitism();
	afx_msg void OnBnClickedCheckScale();
	virtual BOOL OnInitDialog();	
};
