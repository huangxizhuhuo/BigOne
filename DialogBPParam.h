#pragma once


// CDialogBPParam 对话框

class CDialogBPParam : public CDialog
{
	DECLARE_DYNAMIC(CDialogBPParam)

public:
	CDialogBPParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogBPParam();

// 对话框数据
	enum { IDD = IDD_DIALOG_BPPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dLr;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};
extern double g_Lr;