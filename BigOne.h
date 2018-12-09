// BigOne.h : BigOne Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include <vector>
using std::vector;

// CBigOneApp:
// �йش����ʵ�֣������ BigOne.cpp
//

class CBigOneApp : public CWinApp
{
public:
	CBigOneApp();

// ��д
public:
	virtual BOOL InitInstance();
    vector<vector<double>> m_vecOrigin;
	vector<vector<double>> m_vecTransform;
	vector<vector<double>> m_vecTrain;//Training
	vector<vector<double>> m_vecTrainP;
	vector<vector<double>> m_vecTrainT;
	vector<vector<double>> m_vecValid;//Validation
	vector<vector<double>> m_vecValidP;
	vector<vector<double>> m_vecValidT;
	vector<vector<double>> m_vecTest;//Testing
	vector<vector<double>> m_vecTestP;
	vector<vector<double>> m_vecTestT;
	UINT   m_uiInput1;
	UINT   m_uiInput2;
	UINT   m_uiOutput1;
	UINT   m_uiOutput2;
	double m_dPercent1;
	double m_dPercent2;
	WCHAR  m_wszTrainPath[MAX_PATH];
	WCHAR  m_wszTestPath[MAX_PATH];

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBigOneApp theApp;