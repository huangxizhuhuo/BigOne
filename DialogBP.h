#pragma once

#include "ALG\\bp.h"
#include "ALG\\ga.h"
#include "ALG\\ep.h"
#include "ALG\\es.h"
#include "afxcmn.h"
#include "afxwin.h"
// CDialogBP �Ի���

enum EA_TYPE
{
	EA_TYPE_GA,
	EA_TYPE_EP,
	EA_TYPE_ES
};

class CDialogBP : public CDialog
{
	DECLARE_DYNAMIC(CDialogBP)

public:
	CDialogBP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogBP();

// �Ի�������
	enum { IDD = IDD_DIALOG_BP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTrain();
public:
	UINT        m_uiPopSize;
	double      m_dGoal;
	UINT        m_uiMaxEpoch;
	CBP*        m_pBP;
	CEA*        m_pEA;
	CWinThread* m_pThread;
public:
	virtual BOOL OnInitDialog();	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ��������Ԫ��,�ո����
	CString       m_cstrHidNeuron;
	CProgressCtrl m_pcPerformance;
	CProgressCtrl m_pcEpoch;
	CProgressCtrl m_pcTime;
	CProgressCtrl m_pcGradient;
	// �Ƿ��Matlab�ķǾ��ȿ̶�֮����
	BOOL          m_bFuckMatlab;
	CComboBox     m_cbAlg;
	CButton       m_cRedivide;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonStoptrain();
	afx_msg void OnBnClickedButtonPlot();	
	afx_msg void OnBnClickedButtonPredict();
	afx_msg void OnBnClickedButtonParam();
public:
	CTime m_StartTime;
	int   m_initLev;
	int   m_goalLev;
	int   m_levDiff;
	vector<double> m_Lev;
	vector<size_t> S;
	void NewBP( double goal, size_t gen );
	void NewEA( EA_TYPE type, double goal, size_t gen );	
};
