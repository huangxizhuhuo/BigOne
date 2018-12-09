#pragma once
#include "afxwin.h"


// CDialogEABPParam 对话框

class CDialogEABPParam : public CDialog
{
	DECLARE_DYNAMIC(CDialogEABPParam)

public:
	CDialogEABPParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogEABPParam();

// 对话框数据
	enum { IDD = IDD_DIALOG_EABPPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbEaAlg;
	double    m_dError;
	int       m_iGen;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonEaparam();
};

extern int    g_EabpAlg;
extern double g_EabpErr;
extern int    g_EabpGen;
