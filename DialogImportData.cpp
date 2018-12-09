// DialogImportData.cpp : 实现文件
//

#include "stdafx.h"
#include "BigOne.h"
#include "DialogImportData.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

const double PI = 3.1415926535;

// CDialogImportData 对话框

IMPLEMENT_DYNAMIC(CDialogImportData, CDialog)

CDialogImportData::CDialogImportData(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogImportData::IDD, pParent)
	, m_uiInput1(0)
	, m_uiInput2(0)
	, m_uiOutput1(0)
	, m_uiOutput2(0)
	, m_dPercent1(0)
	, m_dPercent2(0)
{
}

CDialogImportData::~CDialogImportData()
{
}

void CDialogImportData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT1, m_uiInput1);
	DDX_Text(pDX, IDC_EDIT_INPUT2, m_uiInput2);
	DDX_Text(pDX, IDC_EDIT_OUTPUT1, m_uiOutput1);
	DDX_Text(pDX, IDC_EDIT_OUTPUT2, m_uiOutput2);
	DDX_Text(pDX, IDC_EDIT_PERCENT1, m_dPercent1);
	DDV_MinMaxDouble(pDX, m_dPercent1, 0, 100);
	DDX_Text(pDX, IDC_EDIT_PERCENT2, m_dPercent2);
	DDV_MinMaxDouble(pDX, m_dPercent2, 0, 100);
	DDX_Control(pDX, IDC_CHECK_PREPROCESS, m_cPreProcess);
	DDX_Control(pDX, IDC_COMBO_PREPROCESS, m_cbPreProcess);
}


BEGIN_MESSAGE_MAP(CDialogImportData, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, &CDialogImportData::OnBnClickedButtonTrain)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDialogImportData::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_VIEWTRAIN, &CDialogImportData::OnBnClickedButtonViewtrain)
	ON_BN_CLICKED(IDC_BUTTON_VIEWTEST, &CDialogImportData::OnBnClickedButtonViewtest)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDialogImportData::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDialogImportData::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CLEARTRAIN, &CDialogImportData::OnBnClickedButtonCleartrain)
	ON_BN_CLICKED(IDC_BUTTON_CLEARTEST, &CDialogImportData::OnBnClickedButtonCleartest)
	ON_BN_CLICKED(IDC_CHECK_PREPROCESS, &CDialogImportData::OnBnClickedCheckPreprocess)
END_MESSAGE_MAP()


// CDialogImportData 消息处理程序

void CDialogImportData::OnBnClickedButtonTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog *lpOpenFileDialog;

	lpOpenFileDialog = new CFileDialog(TRUE,L"",L"",
		OFN_FILEMUSTEXIST |OFN_HIDEREADONLY , L"txt files(*.txt)|*.txt||");
	if(lpOpenFileDialog->DoModal()==IDOK)
	{
		theApp.m_vecOrigin.clear();//
		CString strFileName;
		strFileName = lpOpenFileDialog->GetPathName();
		wcscpy_s(theApp.m_wszTrainPath,strFileName.GetBuffer());
		ifstream in(strFileName);
		if(!in)
			return;		
		CHAR line[1000];
		double element;
		int cRow = 0;
		int cColumn = 0;
		int cFirstLineCol;
		vector<double> vecRow;
		while(in.getline(line,1000,'\n'))
		{
			cRow++;
			istringstream ss(line);
			cColumn=0;
			vecRow.clear();
			while(ss)
			{
				cColumn++;
			    ss>>element;
				vecRow.push_back(element);
			}	
			cColumn--;
			vecRow.pop_back();

			if(cRow == 1)
			    cFirstLineCol = cColumn;
			else if(cColumn != cFirstLineCol)
			{
				theApp.m_vecOrigin.clear();
				MessageBox(L"Data is invalid,please check.",L"Info",MB_ICONINFORMATION);
				break;
			}	
			theApp.m_vecOrigin.push_back(vecRow);
		}
		m_uiInput1 = 1;
		m_uiInput2 = cColumn-1;
		m_uiOutput1 = cColumn;
		m_uiOutput2 = cColumn;
		UpdateData(FALSE);
		if(!theApp.m_vecTest.empty() && theApp.m_vecTest[0].size() !=cColumn)
		{			
			MessageBox(L"Training Data doesn't match Testing Data,please check.",L"Info",MB_ICONINFORMATION);
		}	
		GetDlgItem(IDC_BUTTON_VIEWTRAIN)->EnableWindow(TRUE);
		theApp.m_vecTransform = theApp.m_vecOrigin;//
	}
	delete lpOpenFileDialog;	
}

void CDialogImportData::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog *lpOpenFileDialog;

	lpOpenFileDialog = new CFileDialog(TRUE,L"",L"",
		OFN_FILEMUSTEXIST |OFN_HIDEREADONLY , L"txt files(*.txt)|*.txt||");
	if(lpOpenFileDialog->DoModal()==IDOK)
	{
		theApp.m_vecTest.clear();
		CString strFileName;
		strFileName = lpOpenFileDialog->GetPathName();
		wcscpy_s(theApp.m_wszTestPath,strFileName.GetBuffer());
		ifstream in(strFileName);
		if(!in)
			return;		
		CHAR line[1000];
		double element;
		int cRow = 0;
		int cColumn = 0;
		int cFirstLineCol;
		vector<double> vecRow;
		while(in.getline(line,1000,'\n'))
		{
			cRow++;
			istringstream ss(line);
			cColumn=0;
			vecRow.clear();
			while(ss)
			{
				cColumn++;
			    ss>>element;
				vecRow.push_back(element);
			}	
			cColumn--;
			vecRow.pop_back();

			if(cRow == 1)
			    cFirstLineCol = cColumn;
			else if(cColumn != cFirstLineCol)
			{
				theApp.m_vecTest.clear();
				MessageBox(L"Data is invalid,please check it.",L"Info",MB_ICONINFORMATION);
				break;
			}	
			theApp.m_vecTest.push_back(vecRow);
		}
		if(!theApp.m_vecTrain.empty() &&theApp.m_vecTrain[0].size() !=cColumn)
		{				
			MessageBox(L"Training Data doesn't match Testing Data,please check.",L"Info",MB_ICONINFORMATION);
		}	
		GetDlgItem(IDC_BUTTON_VIEWTEST)->EnableWindow(TRUE);
	}
	delete lpOpenFileDialog;	
	
}

void CDialogImportData::OnBnClickedButtonViewtrain()
{
	// TODO: 在此添加控件通知处理程序代码
	//WinExec("NOTEPAD.exe",SW_SHOW);
	ShellExecute(NULL,L"open",theApp.m_wszTrainPath,NULL,NULL,SW_MAXIMIZE);
}

void CDialogImportData::OnBnClickedButtonViewtest()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,L"open",theApp.m_wszTestPath,NULL,NULL,SW_MAXIMIZE);
}

void CDialogImportData::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();
}

BOOL CDialogImportData::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_uiInput1 = theApp.m_uiInput1;	
	m_uiInput2 = theApp.m_uiInput2;		
	m_uiOutput1 = theApp.m_uiOutput1;	
	m_uiOutput2 = theApp.m_uiOutput2;
	m_dPercent1 = theApp.m_dPercent1;
	m_dPercent2 = theApp.m_dPercent2;
	UpdateData(FALSE);
	if(wcscmp(theApp.m_wszTrainPath,L"") != 0 && !theApp.m_vecTrainP.empty())
		GetDlgItem(IDC_BUTTON_VIEWTRAIN)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BUTTON_VIEWTRAIN)->EnableWindow(FALSE);

	if(wcscmp(theApp.m_wszTestPath,L"") != 0 && !theApp.m_vecTestP.empty())
		GetDlgItem(IDC_BUTTON_VIEWTEST)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BUTTON_VIEWTEST)->EnableWindow(FALSE);

	m_cPreProcess.SetCheck(1);
	m_cbPreProcess.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogImportData::NormalizationLin()
{
	//对于分类问题:
	//1.输出怎么表示: 0/1?
	//2.输出要不要预处理: 不需要?
	//3.怎么确定属于哪一类: 四舍五入?
	size_t cInstance = theApp.m_vecTransform.size();
	//size_t cAttribute = theApp.m_vecTransform[0].size();
	size_t cInputAttribute = (m_uiInput2-m_uiInput1+1);
	size_t i,j;
	for(i=0; i<cInputAttribute;i++)
	{
		for(j=0;j<cInstance; j++)
		{
			//theApp.m_vecTransform[j][i] = (theApp.m_vecTransform[j][i] - m_vecMin[i])/(m_vecMax[i] - m_vecMin[i]);
			theApp.m_vecTransform[j][i] = 2.0*(theApp.m_vecTransform[j][i] - m_vecMin[i])/(m_vecMax[i] - m_vecMin[i]) -1.0;
		}
	}
	size_t cTestInstance;//对用户提供的测试实例进行归一化
	cTestInstance = theApp.m_vecTest.size();
	for(i=0; i<cInputAttribute;i++)
	{
		for(j=0;j<cTestInstance; j++)
		{
			theApp.m_vecTest[j][i] = 2.0*(theApp.m_vecTest[j][i] - m_vecMin[i])/(m_vecMax[i] - m_vecMin[i]) -1.0;
		}
	}
	/*
	vector<double> average;
	average.assign(cAttribute,0);
	double sum=0;
	for(i=0; i<cAttribute;i++)
	{
		sum=0;
		for(j=0;j<cInstance; j++)
		{
			sum += (theApp.m_vecTransform[j][i]);
		}
		average[i] = sum/cInstance;
	}
	vector<double> variance;
	variance.assign(cAttribute,0);
	for(i=0; i<cAttribute;i++)
	{
		sum=0;
		for(j=0;j<cInstance; j++)
		{
			sum += (theApp.m_vecTransform[j][i]-average[i])*(theApp.m_vecTransform[j][i]-average[i]);
		}
		variance[i] = sqrt(sum/(cInstance-1));
	}
	for(i=0; i<cAttribute;i++)
	{
		for(j=0;j<cInstance; j++)
		{
			theApp.m_vecTransform[j][i] = (theApp.m_vecTransform[j][i]-average[i])/variance[i];
		}
	}*/
}

void CDialogImportData::NormalizationLog()
{
	size_t cInstance = theApp.m_vecTransform.size();
	//size_t cAttribute = theApp.m_vecTransform[0].size();
	size_t cInputAttribute = (m_uiInput2-m_uiInput1+1);
	size_t i,j;
	for(i=0; i<cInputAttribute;i++)
	{
		for(j=0;j<cInstance; j++)
		{
			if(theApp.m_vecTransform[j][i] <= 1)
				theApp.m_vecTransform[j][i] = 0;
			else
				theApp.m_vecTransform[j][i] = log10(theApp.m_vecTransform[j][i]);
			if(m_vecMax[i]>1)
			    theApp.m_vecTransform[j][i] /= log10(m_vecMax[i]);
		}
	}
}

void CDialogImportData::NormalizationAtn()
{
	size_t cInstance = theApp.m_vecTransform.size();
	//size_t cAttribute = theApp.m_vecTransform[0].size();
	size_t cInputAttribute = (m_uiInput2-m_uiInput1+1);
	size_t i,j;
	for(i=0; i<cInputAttribute;i++)
	{
		for(j=0;j<cInstance; j++)
		{
			theApp.m_vecTransform[j][i] = atan(theApp.m_vecTransform[j][i])*2/PI;
		}
	}
}

void CDialogImportData::PreProcess()
{
	size_t cInstance = theApp.m_vecTransform.size();
	//size_t cAttribute = theApp.m_vecTransform[0].size();
	size_t cInputAttribute = (m_uiInput2-m_uiInput1+1);
	size_t i,j;
	m_vecMin.clear();
	m_vecMax.clear();
	m_vecMin.assign(cInputAttribute,0);
	m_vecMax.assign(cInputAttribute,0);
	double min,max;	
	for(i=0; i<cInputAttribute;i++)
	{
		min=INT_MAX;
		max=0;
		for(j=0;j<cInstance; j++)
		{
			if(theApp.m_vecTransform[j][i]<min)
			{
				min = theApp.m_vecTransform[j][i];
			}
			if(theApp.m_vecTransform[j][i]>max)
			{
				max = theApp.m_vecTransform[j][i];
			}
		}
		m_vecMin[i] = min;
		m_vecMax[i] = max;
	}
	switch(m_cbPreProcess.GetCurSel())
	{
	case 0:
		NormalizationLin();
		break;
	case 1:
		NormalizationLog();
		break;
	case 2:
		NormalizationAtn();
		break;
	}
}

bool InVector(size_t x,vector<size_t> v)
{
	for(size_t i=0; i<v.size(); i++)
	{
		if(x == v[i])
			return true;
	}
	return false;
}

void DivideRand()
{
	//Divide the Train to Train(Training)、Valid、Sim(Testing),
	//according to the Spliting Percent1 and 2
	int cInstances = (int)theApp.m_vecTransform.size();
	int cTraining  = (int)(cInstances*(theApp.m_dPercent1/100.0));
	int cValid     = (int)(cInstances*(theApp.m_dPercent2/100.0));
	int cTesting   = cInstances-cTraining-cValid;
	ofstream outf("data/DividedResult.txt");
	outf<<"共有"<<cInstances<<"个样本，其中";
	outf<<"训练样本"<<cTraining<<"个，";
	outf<<"验证样本"<<cValid<<"个，";
	outf<<"测试样本"<<cTesting<<"个。\n";
	//150*0.99 = 148
	//150*0.01 = 1;
	//150 - 148 -1 = 1;	
	size_t index=0;
	vector<size_t> selected;
	outf<<"\nValidation Data:\n";
	theApp.m_vecValid.clear();
	outf.setf(ios::fixed);
	while(cValid--)
	{
		index= rand()%(theApp.m_vecTransform.size());
		while(InVector(index,selected))
		{
			index = rand()%(theApp.m_vecTransform.size());
		}
		selected.push_back(index);
		theApp.m_vecValid.push_back(theApp.m_vecTransform[index]);
	
		outf<<"\n";
		outf<<index<<":\t";
		for(size_t i=0; i<theApp.m_vecTransform[index].size(); i++)
		{
			outf<<setw(6)<<theApp.m_vecTransform[index][i]<<"\t";
		}
		outf<<"\n";
	}

	outf<<"\nTesting Data:\n";
	if(wcscmp(theApp.m_wszTestPath,L"") == 0)
	{
		theApp.m_vecTest.clear();
		while(cTesting--)
		{
			index= rand()%(theApp.m_vecTransform.size());
			while(InVector(index,selected))
			{		
				index = rand()%(theApp.m_vecTransform.size());
			}	
			selected.push_back(index);
			theApp.m_vecTest.push_back(theApp.m_vecTransform[index]);

			outf<<"\n";
			outf<<index<<":\t";
			for(size_t i=0; i<theApp.m_vecTransform[index].size(); i++)
			{
				outf<<setw(6)<<theApp.m_vecTransform[index][i]<<"\t";
			}
			outf<<"\n";
		}
	}
	else//使用用户导入的测试数据
	{	
	}
	theApp.m_vecTrain.clear();
	outf<<"\nTraining Data:\n";
	for(size_t i=0; i<theApp.m_vecTransform.size(); i++)
	{
		if(!InVector(i,selected))
		{
			outf<<"\n";
			outf<<i<<":\t";
			for(size_t j=0; j<theApp.m_vecTransform[0].size(); j++)
			{
				outf<<setw(6)<<theApp.m_vecTransform[i][j]<<"\t";
			}
			outf<<"\n";
			theApp.m_vecTrain.push_back(theApp.m_vecTransform[i]);
		}
	}

}

void DivideToInOut(const vector<vector<double>> &Sample,vector<vector<double>>& SampleP,		
				   vector<vector<double>>& SampleT)
{
	if(Sample.empty())
	{
		SampleP.clear();
		SampleT.clear();
		return;
	}
	
	SampleP.clear();
    SampleT.clear();

	vector<double> temp;
	for(size_t i=0; i<Sample.size(); i++)
	{
		temp.clear();
		for(size_t j=theApp.m_uiInput1-1; j<theApp.m_uiInput2; j++)
		{
			temp.push_back(Sample[i][j]);
		}
		SampleP.push_back(temp);
	}		
	for(size_t i=0; i<Sample.size(); i++)
	{
		temp.clear();
		for(size_t j=theApp.m_uiOutput1-1; j<theApp.m_uiOutput2; j++)
		{
			temp.push_back(Sample[i][j]);
			
		}
		SampleT.push_back(temp);
	}
}

void CDialogImportData::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_dPercent1+m_dPercent2>100.0)
	{
		MessageBox(L"The Split coefficient is invalid,please check.",L"Info",MB_ICONINFORMATION);
		return;
	}
	if(!theApp.m_vecTrain.empty() && !theApp.m_vecTest.empty() &&
		(theApp.m_vecTest[0].size() !=theApp.m_vecTrain[0].size()))
	{			
		MessageBox(L"Training Data doesn't match Testing Data,please check.",L"Info",MB_ICONINFORMATION);
	}
	theApp.m_vecTransform = theApp.m_vecOrigin;//
	theApp.m_uiInput1 = m_uiInput1;
	theApp.m_uiInput2 = m_uiInput2;
	theApp.m_uiOutput1 = m_uiOutput1;
	theApp.m_uiOutput2 = m_uiOutput2;
	theApp.m_dPercent1 = m_dPercent1;
	theApp.m_dPercent2 = m_dPercent2;

	size_t cColumn = theApp.m_vecTransform[0].size();
	if(m_uiInput1<1 || m_uiInput2<1 || m_uiOutput1<1 || m_uiOutput1<1
		||m_uiInput1>cColumn || m_uiInput2>cColumn || m_uiOutput1>cColumn || m_uiOutput1>cColumn)
	{
		MessageBox(L"Some index may be out of range,please check.",L"Info",MB_ICONINFORMATION);
		return;
	}
	if(wcscmp(theApp.m_wszTrainPath,L"") != 0)
	{
		if(m_cPreProcess.GetCheck())
			PreProcess();
		DivideRand();
		DivideToInOut(theApp.m_vecTrain,theApp.m_vecTrainP,theApp.m_vecTrainT);
		DivideToInOut(theApp.m_vecValid,theApp.m_vecValidP,theApp.m_vecValidT);				
		DivideToInOut(theApp.m_vecTest,theApp.m_vecTestP,theApp.m_vecTestT);	
	}

	CDialog::OnOK();
	ShellExecute(NULL,L"open",L"data\\DividedResult.txt",NULL,NULL,SW_MAXIMIZE);
}

void CDialogImportData::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

void CDialogImportData::OnBnClickedButtonCleartrain()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_vecTrain.clear();
	theApp.m_vecTrainP.clear();
	theApp.m_vecTrainT.clear();
	GetDlgItem(IDC_BUTTON_VIEWTRAIN)->EnableWindow(FALSE);
	wcscpy_s(theApp.m_wszTrainPath,L"");
}

void CDialogImportData::OnBnClickedButtonCleartest()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_vecTest.clear();
	theApp.m_vecTestP.clear();
	theApp.m_vecTestT.clear();
	GetDlgItem(IDC_BUTTON_VIEWTEST)->EnableWindow(FALSE);
	wcscpy_s(theApp.m_wszTestPath,L"");
}

void CDialogImportData::OnBnClickedCheckPreprocess()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_cPreProcess.GetCheck())
		GetDlgItem(IDC_COMBO_PREPROCESS)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_COMBO_PREPROCESS)->EnableWindow(TRUE);


}
