#pragma once

#include "DialogBP.h"
// CDialogPlot 对话框

class CDialogPlot : public CDialog
{
	DECLARE_DYNAMIC(CDialogPlot)

public:
	CDialogPlot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogPlot();	
	CDialogBP* m_pDialogBP;
private:
	void PrepareData();
	
	double m_NNGoal;
	size_t m_Epochs;
	int    m_xLength;
	int    m_yLength;
	WCHAR  m_wszEpoch[50];
	WCHAR  m_wszInitPerf[50];
	WCHAR  m_wszGoal[50];
	POINT  m_ptOrigin;
	POINT  m_ptXAxis;
	POINT  m_ptYAxis;
	POINT  m_ptsRect[4];
	POINT  m_ptsGoal[2];
	vector<POINT>  m_Points;
	vector<POINT>  m_YScale;
	vector<POINT>  m_XScale;
	vector<double> m_NNPerf;
// 对话框数据
	enum { IDD = IDD_DIALOG_PLOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
