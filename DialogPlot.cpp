// DialogPlot.cpp : 实现文件
//

#include "stdafx.h"
#include "BigOne.h"
#include "DialogPlot.h"

// CDialogPlot 对话框

IMPLEMENT_DYNAMIC(CDialogPlot, CDialog)

CDialogPlot::CDialogPlot(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPlot::IDD, pParent)
{

}

CDialogPlot::~CDialogPlot()
{
}

void CDialogPlot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogPlot, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()

void CDialogPlot::PrepareData()
{
	static int Dist = 40;
	m_Points.clear();
	m_YScale.clear();
	m_XScale.clear();
	if(m_pDialogBP->m_pBP)
	{
		m_NNPerf = m_pDialogBP->m_pBP->m_Perf;
		m_NNGoal = m_pDialogBP->m_pBP->m_dGoal;
		m_Epochs = m_pDialogBP->m_pBP->m_iEpoch;
	}
	else if(m_pDialogBP->m_pEA)
	{
		m_NNPerf = m_pDialogBP->m_pEA->m_BestFitness;
		for( size_t i = 0; i < m_NNPerf.size(); ++i )
		{
			m_NNPerf[i] = Big_Num - m_NNPerf[i];
		}
		m_NNGoal = m_pDialogBP->m_pEA->m_dGoal;
		m_Epochs = m_pDialogBP->m_pEA->m_iGeneration;
	}
		
	RECT rect;
	GetClientRect(&rect);
	m_xLength = rect.right;
	m_yLength = rect.bottom;

	m_ptOrigin = CPoint(Dist,m_yLength - Dist);
	m_ptXAxis  = CPoint(m_xLength - Dist,m_yLength - Dist);
	m_ptYAxis  = CPoint(Dist,Dist);

	m_ptsRect[0] = m_ptOrigin;
	m_ptsRect[1] = m_ptYAxis;
	m_ptsRect[2] = CPoint(m_ptXAxis.x,m_ptYAxis.y);
	m_ptsRect[3] = m_ptXAxis;

	//matlab的非均匀刻度
	int level1,level2,diffLevel;
	if(m_pDialogBP->m_bFuckMatlab)
	{
		level1 = (int)floor(log10(m_NNPerf[0]));
		level2 = (int)floor(log10(m_NNGoal));
		diffLevel = (level1-level2+3);
		for(int i=0; i<diffLevel; i++)
		{
			m_YScale.push_back(CPoint(Dist,60+i*(m_yLength -120)/(diffLevel-1)));
			m_YScale.push_back(CPoint(Dist+5,60+i*(m_yLength -120)/(diffLevel-1)));
		}
	}
	else//Y轴为均匀刻度
	{
	}
	//X轴刻度
	for(int i=1; i<10; i++)
	{
		m_XScale.push_back(CPoint(Dist+i*((m_xLength-Dist*2)/10),m_yLength-Dist));
		m_XScale.push_back(CPoint(Dist+i*((m_xLength-Dist*2)/10),m_yLength-Dist-5));
	}

	double gap;
	gap = (m_xLength-Dist*2)/(double)(m_NNPerf.size());
	double xAdd;
	double iStep;
	if(gap>1)
	{
		xAdd = gap;
		iStep = 1.0;
	}
	else
	{
		xAdd = 1.0;
		iStep = 1/gap;
	}

	POINT tPoint;
	for(int i=0,j=0; i<(int)m_NNPerf.size(); j++)
	{
		tPoint.x = (long)(j*xAdd + Dist);
		//matlab的非均匀刻度
		if(m_pDialogBP->m_bFuckMatlab)
		{
			int curLevel = (int)floor(log10(m_NNPerf[i]));
			int index = (int)fabs((double)(curLevel-level1));
			
			if(index>(int)m_YScale.size()/2-1)
				index = (int)m_YScale.size()/2-1;

			tPoint.y = (long)((pow(10.0,curLevel+1)-m_NNPerf[i])/(pow(10.0,curLevel+1)-pow(10.0,curLevel))	
				*(m_yLength -120)/(diffLevel-1)+m_YScale[index*2].y);	
		}
		else//Y轴为均匀刻度
		{
		    tPoint.y = (long)((m_NNPerf[0]-m_NNPerf[i])/(m_NNPerf[0]-m_NNGoal)*(m_yLength-120)+60);
		}	
		m_Points.push_back(tPoint);
		i = (int)((j+1)*iStep);
	}
	//matlab的非均匀刻度
	long goalY;
	if(m_pDialogBP->m_bFuckMatlab)
	{
		int curLevel = (int)floor(log10(m_NNGoal));	
		int index = (int)fabs((double)(curLevel-level1));
		goalY= (long)((pow(10.0,curLevel+1)-m_NNGoal)/(pow(10.0,curLevel+1)-pow(10.0,curLevel))
				*(m_yLength-120)/(diffLevel-1)+m_YScale[index*2].y);	
	}
	else
	{
		goalY = m_yLength-60;
	}
	m_ptsGoal[0] = CPoint(Dist,goalY);		
	m_ptsGoal[1] = CPoint(m_xLength-Dist,goalY);
	swprintf_s(m_wszEpoch,L"%d  Epochs",m_Epochs);
	swprintf_s(m_wszInitPerf,L"%.3f",m_NNPerf[0]);
	swprintf_s(m_wszGoal,L"%.0e",m_NNGoal);
}
// CDialogPlot 消息处理程序

int CDialogPlot::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	PrepareData();
	return 0;
}

void CDialogPlot::OnPaint()
{	
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	PAINTSTRUCT ps;
	BeginPaint(&ps);
	static HPEN OldPen = NULL;
	static HBRUSH OldBrush = NULL;
	static HPEN BluePen = CreatePen(PS_SOLID,1,RGB(0,0,255));
	static HPEN BlackDashPen = CreatePen(PS_DASH,1,RGB(0,0,0));
	static HBRUSH WhiteBrush = CreateSolidBrush(RGB(255,255,255));
	
	OldBrush = (HBRUSH)SelectObject(dc, WhiteBrush);
	Polygon(dc,m_ptsRect,4);
	
	//纵轴刻度
	for(size_t i=0; i<m_YScale.size(); i+=2)
	{
		MoveToEx(dc,m_YScale[i].x,m_YScale[i].y,NULL);
		LineTo(dc,m_YScale[i+1].x,m_YScale[i+1].y);
	}
	//横轴刻度
	for(size_t i=0; i<m_XScale.size(); i+=2)
	{
		MoveToEx(dc,m_XScale[i].x,m_XScale[i].y,NULL);
		LineTo(dc,m_XScale[i+1].x,m_XScale[i+1].y);
	}
	//目标误差
	OldPen = (HPEN)SelectObject(dc, BlackDashPen);
	MoveToEx(dc,m_ptsGoal[0].x,m_ptsGoal[0].y,NULL);
	LineTo(dc,m_ptsGoal[1].x,m_ptsGoal[1].y);
	//误差走势
	OldPen = (HPEN)SelectObject(dc, BluePen);
	for(size_t i=0; i<m_Points.size()-1; i++)
	{
		MoveToEx(dc,m_Points[i].x,m_Points[i].y,NULL);
		LineTo(dc,m_Points[i+1].x,m_Points[i+1].y);
	}
	static HFONT hfnt = NULL;
	static HFONT hOldFont = NULL; 
 
    hfnt = (HFONT)GetStockObject(ANSI_VAR_FONT); 
    if (hOldFont = (HFONT)SelectObject(dc, hfnt)) 
    { 
		SetBkMode(dc,TRANSPARENT);
		TextOut(dc,m_xLength/2-50,m_yLength-30,m_wszEpoch,(int)wcslen(m_wszEpoch));
		TextOut(dc,1,m_Points[0].y-20,m_wszInitPerf,(int)wcslen(m_wszInitPerf));
		TextOut(dc,1,m_ptsGoal[0].y,m_wszGoal,(int)wcslen(m_wszGoal));
		TextOut(dc,m_ptsRect[0].x,m_ptsRect[0].y+5,L"0",1);
        SelectObject(dc, hOldFont); 
    } 

	SelectObject(dc,OldPen);
	SelectObject(dc,OldBrush);
	EndPaint(&ps);
}

void CDialogPlot::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if(cx==0 || cy==0)
		return;
	PrepareData();
	InvalidateRect(NULL,TRUE);
}

void CDialogPlot::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);
	// TODO: 在此处添加消息处理程序代码
	PrepareData();
	InvalidateRect(NULL,TRUE);
}
