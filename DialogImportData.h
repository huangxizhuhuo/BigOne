#pragma once
#include "afxwin.h"


// CDialogImportData 对话框

class CDialogImportData : public CDialog
{
	DECLARE_DYNAMIC(CDialogImportData)

public:
	CDialogImportData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogImportData();

// 对话框数据
	enum { IDD = IDD_DIALOG_IMPORTDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTrain();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonViewtrain();
	afx_msg void OnBnClickedButtonViewtest();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonCleartrain();
	afx_msg void OnBnClickedButtonCleartest();
	afx_msg void OnBnClickedCheckPreprocess();
public:
	UINT      m_uiInput1;
	UINT      m_uiInput2;
	UINT      m_uiOutput1;
	// Output range upper boundary
	UINT      m_uiOutput2;
	double    m_dPercent1;
	double    m_dPercent2;
	CButton   m_cPreProcess;	
	CComboBox m_cbPreProcess;
	void NormalizationLin();
	void NormalizationLog();
	void NormalizationAtn();
	void PreProcess();//对theApp.m_vecOrigin进行变换
	vector<double> m_vecMin,m_vecMax;
};

extern void DivideRand();
extern void  DivideToInOut(const vector<vector<double>> &Sample,vector<vector<double>>& SampleP,
						   vector<vector<double>>& SampleT);
