#pragma once


// CDialogBPParam �Ի���

class CDialogBPParam : public CDialog
{
	DECLARE_DYNAMIC(CDialogBPParam)

public:
	CDialogBPParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogBPParam();

// �Ի�������
	enum { IDD = IDD_DIALOG_BPPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dLr;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};
extern double g_Lr;