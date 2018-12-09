// DialogBP.cpp : 实现文件
//
#include "stdafx.h"
#include "BigOne.h"
#include "DialogBP.h"
#include "DialogGAParam.h"
#include "DialogBPParam.h"
#include "DialogEABPParam.h"
#include "DialogImportData.h"
#include "DialogPlot.h"
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;

// CDialogBP 对话框
IMPLEMENT_DYNAMIC(CDialogBP, CDialog)

CDialogBP::CDialogBP(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBP::IDD, pParent)
	, m_uiPopSize(30)
	, m_dGoal(1e-1)
	, m_cstrHidNeuron(_T("10"))
	, m_uiMaxEpoch(10000)
	, m_bFuckMatlab(FALSE)
{
	m_pBP = NULL;
	m_pEA = NULL;
	/*m_pGA = NULL;
	m_pEP = NULL;
	m_pES = NULL;*/
	m_pThread = NULL;
}

CDialogBP::~CDialogBP()
{
}

void CDialogBP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POPSIZE, m_uiPopSize);
	DDX_Text(pDX, IDC_EDIT_GOAL, m_dGoal);
	DDX_Text(pDX, IDC_EDIT_HIDNEURON, m_cstrHidNeuron);
	DDX_Text(pDX, IDC_EDIT_MAXEPOCH, m_uiMaxEpoch);
	DDX_Control(pDX, IDC_PROGRESS_PERFORMANCE, m_pcPerformance);
	DDX_Control(pDX, IDC_PROGRESS_EPOCH, m_pcEpoch);
	DDX_Control(pDX, IDC_PROGRESS_TIME, m_pcTime);
	DDX_Control(pDX, IDC_PROGRESS_GRADIENT, m_pcGradient);
	//DDX_Check(pDX, IDC_CHECK_FUCKMATLAB, m_bFuckMatlab);
	DDX_Control(pDX, IDC_COMBO_ALG, m_cbAlg);
	DDX_Control(pDX, IDC_CHECK_REDIVIDE, m_cRedivide);
}


BEGIN_MESSAGE_MAP(CDialogBP, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, &CDialogBP::OnBnClickedButtonTrain)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_STOPTRAIN, &CDialogBP::OnBnClickedButtonStoptrain)
	ON_BN_CLICKED(IDC_BUTTON_PLOT, &CDialogBP::OnBnClickedButtonPlot)
	ON_BN_CLICKED(IDC_BUTTON_PREDICT, &CDialogBP::OnBnClickedButtonPredict)
	ON_BN_CLICKED(IDC_BUTTON_PARAM, &CDialogBP::OnBnClickedButtonParam)
END_MESSAGE_MAP()

static HWND g_hWnd;
static bool g_Eabp = false;
static bool g_EaOver = false;

UINT BpThreadProc(LPVOID pParam)
{
	srand(GetTickCount());//
	CBP* pBP = (CBP*)pParam;
	if(pBP== NULL )
		return 1;
	pBP->Train();	    
	MessageBox(g_hWnd,L"Done",L"Info",MB_ICONINFORMATION);
	AfxEndThread(0);
	return 0;
}

UINT EAThreadProc(LPVOID pParam)
{
	srand(GetTickCount());
	CEA* pEA = (CEA*)pParam;
	if(pEA == NULL)
		return 1;
	pEA->Run();
	if(g_Eabp)
	{
		g_EaOver = true;
	}
	else
	{
	    MessageBox(g_hWnd,L"Done",L"Info",MB_ICONINFORMATION);
	}
	AfxEndThread(0);
	return 0;
}

void CDialogBP::NewBP(double goal,size_t gen)
{
	m_pBP->m_dGoal = goal;
	m_pBP->m_iMaxEpoch = gen;
	m_pBP->m_dLr = g_Lr;
	
	m_initLev = (int)floor(log10(m_pBP->m_dInitMse));
	m_goalLev = (int)floor(log10(m_pBP->m_dGoal));
	m_levDiff = m_initLev - m_goalLev;
	m_Lev.clear();
	m_Lev.push_back(m_pBP->m_dInitMse);
	for(int i=m_initLev; i>m_goalLev; i--)
	{
		m_Lev.push_back(pow(10.0,i));
	}
	m_Lev.push_back(m_pBP->m_dGoal);
	m_pThread = AfxBeginThread(BpThreadProc,m_pBP);
}

void CDialogBP::NewEA( EA_TYPE type, double goal, size_t gen )
{
	switch(type)
	{
	case EA_TYPE_GA:
		m_pEA = new CGA(theApp.m_vecTrainP,theApp.m_vecTrainT,S);
		break;
	case EA_TYPE_EP:
		m_pEA = new CEP(theApp.m_vecTrainP,theApp.m_vecTrainT,S);
		break;
	case EA_TYPE_ES:
		m_pEA = new CES(theApp.m_vecTrainP,theApp.m_vecTrainT,S);
		break;
	default:
		break;
	}
	
	m_pEA->m_iMaxGeneration = gen;
	m_pEA->m_dGoal = goal;
	m_pEA->GenerateInitPop();
	m_pEA->ComputeInitFitness();
	
	m_initLev = (int)floor(log10(m_pEA->GetLeastMSE()));

	m_goalLev = (int)floor(log10(m_pEA->m_dGoal));
	m_levDiff = m_initLev - m_goalLev;
	m_Lev.clear();

	m_Lev.push_back(m_pEA->GetLeastMSE());
	for(int i=m_initLev; i>m_goalLev; i--)
	{
		m_Lev.push_back(pow(10.0,i));
	}
	m_Lev.push_back(m_pEA->m_dGoal);
	m_pThread = AfxBeginThread( EAThreadProc, m_pEA );
}

void CDialogBP::OnBnClickedButtonTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pThread)
	{
		DWORD dwExitCode;
		GetExitCodeThread(m_pThread->m_hThread,&dwExitCode);
		if(dwExitCode == STILL_ACTIVE )
		{
			MessageBox(L"Sorry,you can't do this when the thread is running,you should firstly stop it",L"Sorry",MB_ICONINFORMATION);
			return;
		}
	}
	SAFE_DELETE(m_pBP);
	SAFE_DELETE(m_pEA);

	UpdateData(TRUE);
	g_PopNum = m_uiPopSize;
	char str[1000];
	WideCharToMultiByte(CP_ACP,0,m_cstrHidNeuron.GetBuffer(),-1,str,1000,NULL,NULL);

	istringstream is(str);
	S.clear();
	size_t neuron;
	while(is)
	{
		is>>neuron;
		S.push_back(neuron);
	}
	S.pop_back();
	
	if(theApp.m_vecTrainP.empty() || theApp.m_vecTrainT.empty() || S.empty())
	{
		MessageBox(L"Invalid Data",L"Info",MB_ICONINFORMATION);
		return;
	}
	if(m_cRedivide.GetCheck())
	{//对theApp.m_vecTransform进行重新划分
		DivideRand();
		DivideToInOut(theApp.m_vecTrain,theApp.m_vecTrainP,theApp.m_vecTrainT);
		DivideToInOut(theApp.m_vecValid,theApp.m_vecValidP,theApp.m_vecValidT);				
		DivideToInOut(theApp.m_vecTest,theApp.m_vecTestP,theApp.m_vecTestT);	
	}
	g_Eabp = false;
	g_EaOver = false;
	m_StartTime =CTime::GetCurrentTime();
	switch(m_cbAlg.GetCurSel())
	{
	case 0://BP
		m_pBP = new CBP(theApp.m_vecTrainP, theApp.m_vecTrainT, S);
	    m_pBP->InitWeight();
		m_pBP->ComputeInitMse();
		NewBP(m_dGoal, m_uiMaxEpoch);
	break;
	case 1://GA
		NewEA(EA_TYPE_GA, m_dGoal, m_uiMaxEpoch);
		break;	
	case 2://EP
		NewEA(EA_TYPE_EP, m_dGoal, m_uiMaxEpoch);
		break;
	case 3://ES
		NewEA(EA_TYPE_ES, m_dGoal, m_uiMaxEpoch);
		break;
	case 4://EA-BP
		g_Eabp = true;
		NewEA( (EA_TYPE)g_EabpAlg, g_EabpErr, g_EabpGen );
		break;
	}
}

BOOL CDialogBP::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	g_hWnd = this->GetSafeHwnd();
	m_uiPopSize = (UINT)g_PopNum;
	SetTimer(0,100,NULL);//每0.1s
	m_pcPerformance.SetRange(0,10000);
	m_pcTime.SetRange(0,10000);
	m_pcEpoch.SetRange(0,10000);
	m_pcGradient.SetRange(0,10000);
	m_cbAlg.SetCurSel(0);
	m_cRedivide.SetCheck(1);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogBP::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(g_EaOver)
	{
		m_pBP = new CBP(theApp.m_vecTrainP,theApp.m_vecTrainT,S);
		m_pBP->m_dLr = g_Lr;
		m_pBP->InitWeight();
		
		m_pBP->SetWeight( m_pEA->m_Pop[m_pEA->m_iFittest] );
	    //m_pBP->SetInitMse(m_pEA->m_BestFitness[0]);
		m_pBP->SetInitMse( m_pEA->GetLeastMSE() );
	    m_pBP->SetPerf( m_pEA->m_BestFitness );
		SAFE_DELETE( m_pEA );

		NewBP(m_dGoal,m_uiMaxEpoch);
		g_EaOver = false;
	}
	static CTimeSpan timeDiff;
	if(m_pBP)
	{
		
		double gap;
		int pos; 
		int curLev = (int)floor(log10(m_pBP->m_dError));
		int index = (int)fabs((double)(curLev-m_initLev));
		if(index>(int)m_Lev.size()-2)
			index = (int)m_Lev.size()-2;
		gap = (m_Lev[index]-m_Lev[index+1])/(10000.0/(m_levDiff+1));
		pos = (long)((10000.0/(m_levDiff+1))*(m_initLev -curLev) + (m_Lev[index] -m_pBP->m_dError)/gap);
		m_pcPerformance.SetPos(pos);

		gap = (m_pBP->m_iMaxEpoch)/10000.0; 
		pos = (int)(m_pBP->m_iEpoch/gap);
		m_pcEpoch.SetPos(pos);
		static WCHAR wstr[100];
		swprintf_s(wstr,L"%.3e",m_pBP->m_dError);
		SetDlgItemText(IDC_STATIC_PERF,wstr);
		swprintf_s(wstr,L"%d",m_pBP->m_iEpoch);
		SetDlgItemText(IDC_STATIC_EPOCH,wstr);
		if(!m_pBP->m_bExit)
		{
			timeDiff = CTime::GetCurrentTime() - m_StartTime;
			CString cstr = timeDiff.Format("%H:%M:%S");
			SetDlgItemText(IDC_STATIC_TIME,cstr.GetBuffer());
		}
	}
	else if(m_pEA)
	{
		double gap;
		int pos;
		
		int curLev = (int)floor( log10(m_pEA->GetLeastMSE()) );
		int index = (int)fabs((double)(curLev-m_initLev));
		if(index>(int)m_Lev.size()-2)
			index = (int)m_Lev.size()-2;
		gap = (m_Lev[index]-m_Lev[index+1])/(10000.0/(m_levDiff+1));
	
		pos = (long)((10000.0/(m_levDiff+1))*(m_initLev -curLev) + (m_Lev[index]-m_pEA->GetLeastMSE())/gap);
		m_pcPerformance.SetPos(pos);

		gap = (m_pEA->m_iMaxGeneration)/10000.0; 
		pos = (int)(m_pEA->m_iGeneration/gap);
		m_pcEpoch.SetPos(pos);
		static WCHAR wstr[100];
		
		swprintf_s(wstr,L"%.3e",m_pEA->GetLeastMSE());
		SetDlgItemText(IDC_STATIC_PERF,wstr);
		swprintf_s(wstr,L"%d",m_pEA->m_iGeneration);
		SetDlgItemText(IDC_STATIC_EPOCH,wstr);
		if(!m_pEA->m_bExit)
		{
			timeDiff = CTime::GetCurrentTime() - m_StartTime;
			CString cstr = timeDiff.Format("%H:%M:%S");
			SetDlgItemText(IDC_STATIC_TIME,cstr.GetBuffer());
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDialogBP::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_pThread)
	{
		DWORD dwExitCode;
		GetExitCodeThread(m_pThread->m_hThread,&dwExitCode);
		if(dwExitCode == STILL_ACTIVE )
		{
			MessageBox(L"Sorry,you can't do this when the thread is running,you should firstly stop it",L"Sorry",MB_ICONINFORMATION);
			return;
		}
	}
	SAFE_DELETE(m_pBP);
	SAFE_DELETE(m_pEA);
	CDialog::OnClose();
}

void CDialogBP::OnBnClickedButtonStoptrain()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pThread)
	{
		DWORD dwExitCode;
		GetExitCodeThread(m_pThread->m_hThread,&dwExitCode);
		if(dwExitCode == STILL_ACTIVE )
		{
			if(m_pBP)
			{
			    m_pBP->m_bExit=true;
			}
			if(m_pEA)
			{
				m_pEA->m_bExit=true;
			}
		}
	}
}

void CDialogBP::OnBnClickedButtonPlot()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_pThread)
	{
		DWORD dwExitCode;
		GetExitCodeThread(m_pThread->m_hThread,&dwExitCode);
		if(dwExitCode == STILL_ACTIVE )
		{
			MessageBox(L"Sorry,you can't do this when the thread is running,you should firstly stop it",L"Sorry",MB_ICONINFORMATION);
			return;
		}
	}
	if(m_pBP || m_pEA)
	{
	    CDialogPlot dlg;
		dlg.m_pDialogBP = this;
	    dlg.DoModal();	
	}
}

vector<double> Sim(CFFNN* pNN,const vector<vector<double>>& P, const vector<vector<double>>& T)
{
	size_t cInst   = P.size();
	size_t cInput  = P[0].size();
	size_t cOutput = T[0].size();

	vector<double> error;
	if(pNN->m_cInput!=cInput || pNN->m_cOutput!=cOutput)
	{
		MessageBox(g_hWnd,L"The data doesn't match the ANN,please check",L"Info",MB_ICONINFORMATION);
		return error;
	}
	
	vector<vector<vector<double>>> valueEachInst;
	vector<double> valueThisLayer;
	vector<vector<double>> valueThisInst;
	size_t i,j,k,m,n;
	for(i=0; i<cInst; i++)	
	{
		valueThisInst.clear();
		for(j=0; j<=pNN->m_cHidLayer+1; j++)
		{
			valueThisLayer.clear();
			valueThisLayer.assign(pNN->m_NeuronEachLayer[j],1.0);
			valueThisInst.push_back(valueThisLayer);
		}
		valueEachInst.push_back(valueThisInst);
	}
	for(i=0; i<cInst; i++)//对每个实例	
	{
		for(k=0; k<cInput; k++)
		{
			valueEachInst[i][0][k] = P[i][k];
		}
	}
	for(i=0; i<cInst; i++)
	{
		double sum=0;
		for(j=0; j<pNN->m_cHidLayer+1; j++)//需要计算(隐层数+1)次
		{
			for(m=0; m<pNN->m_NeuronEachLayer[j+1]; m++)//计算该层每个神经元的输出
			{
				sum=0;
				for(n=0; n<pNN->m_NeuronEachLayer[j]+1; n++)//多出来的1为阈值,恒为-1
				{
					if(n == pNN->m_NeuronEachLayer[j])
						sum+= (-1)*pNN->m_Bias[j][m];//(-1)*该层第m个神经元的Bias
					else//该层第m个神经元第i个实例的输出值*该层第m个神经元与前层第n个神经元的权值
						sum+= valueEachInst[i][j][n]*pNN->m_Weight[j][n][m];
				}	
				valueEachInst[i][j+1][m] = pNN->LogSigmoid(sum);//激活值
				
				if(j == pNN->m_cHidLayer)
				{	
					error.push_back(T[i][m] - valueEachInst[i][j+1][m]);
				}
			}		
		}	
	}
	ofstream outf("data/SimResult.txt");
	outf<<"SimResult:\n\n";
	double sumErr=0;
	outf.setf(ios::fixed);
	size_t cCorrect = 0;
	size_t cOutCorrect = 0;
	for(i=0; i<cInst; i++)
	{
		cOutCorrect = 0;
		for(j=0; j<cOutput; j++)//计算该层每个神经元的输出
		{
			//outf<<setw(8)<<T[i][j]<<"\t"<<setw(8)<<valueEachInst[i][pNN->m_cHidLayer+1][j]<<"\t";
			//针对分类问题,四舍五入
			outf<<setw(8)<<T[i][j]<<"\t"<<setw(8)<<double(int(valueEachInst[i][pNN->m_cHidLayer+1][j]+0.5))<<"\t";
			if(int(T[i][j]) ==int(valueEachInst[i][pNN->m_cHidLayer+1][j]+0.5))
			{
				cOutCorrect++;
			}
			sumErr +=(T[i][j] - valueEachInst[i][pNN->m_cHidLayer+1][j])*(T[i][j] - valueEachInst[i][pNN->m_cHidLayer+1][j]);
		}
		if(cOutCorrect == cOutput)
		{
			cCorrect++;
		}
		outf<<"\n";
	}
	sumErr = sumErr/(2.0*cInst);
	outf<<"\nMeanSquaredError: "<<sumErr<<"\n";
	outf<<"\n分类正确数: "<<cCorrect<<"\n";
	outf<<"\n分类正确率: "<<cCorrect/(double)cInst<<"\n";
	ShellExecute(NULL,L"open",L"data\\SimResult.txt",NULL,NULL,SW_MAXIMIZE);
	return error;
}

void CDialogBP::OnBnClickedButtonPredict()
{
	// TODO: 在此添加控件通知处理程序代码
	if(theApp.m_vecTestP.empty())
		return;
	if(m_pBP)
	{
		Sim(m_pBP,theApp.m_vecTestP,theApp.m_vecTestT);
	}
	if(m_pEA)
	{
		m_pEA->SetFittest();
		Sim(m_pEA->m_FFNN,theApp.m_vecTestP,theApp.m_vecTestT);
	}
}

void CDialogBP::OnBnClickedButtonParam()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_PopNum = m_uiPopSize;
	int iSel = m_cbAlg.GetCurSel();
	if( iSel == 0 )
	{
		CDialogBPParam dlg;
		dlg.DoModal();
	}
	else if( iSel == 1 )
	{	
		CDialogGAParam dlg;
		dlg.DoModal();
	}
	else if( iSel == 4 )
	{
		CDialogEABPParam dlg;
		dlg.DoModal();
	}
}
