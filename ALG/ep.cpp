#include "stdafx.h"
#include "ep.h"
using namespace std;
//D.B.Fogel,1992
//X.Yao,1999,"EP Made Faster"
//Coded at June,2009

CEP::CEP( const Pop& P, const Pop& T, const std::vector<size_t>& S )
{
	m_FFNN = new CFFNN( P, T, S );	
	m_iChromoLength = m_FFNN->m_cWeight;
	t  = 1/(sqrt(2.0*sqrt(double(m_iChromoLength))));
	t1 = 1/(sqrt(2.0*m_iChromoLength));	
	m_bExit            = false;
	m_iPopNum          = g_PopNum;
	m_dBestFitness     = 0;	
	m_iChosenOpponents = 10;
}

void CEP::GenerateInitPop()
{
	for(size_t i = 0; i<2*m_iPopNum; i++)
	{	
		vector<double> temp1,temp2;
		for(size_t j = 0; j<m_iChromoLength; j++)
		{	
			temp1.push_back((rand()/(double)RAND_MAX)*1 - 0.5);
			temp2.push_back(1.0);//3.0
			//temp2.push_back( (rand()/(double)RAND_MAX)*1 - 0.5 );
		}
		m_Pop.push_back(temp1); 
		m_PopDelta.push_back(temp2);
	}
	m_NewPop.assign(m_Pop.begin(),m_Pop.begin()+m_iPopNum);
	m_NewPopDelta.assign(m_Pop.begin(),m_Pop.begin()+m_iPopNum);
	m_NewFitness.assign(2*m_iPopNum,0);
	m_Fitness.assign(2*m_iPopNum,0);
	m_Win.assign(2*m_iPopNum,0);
	m_Index.assign(2*m_iPopNum,0);
}

//赋给网络2μ次权值,计算每次的输出误差(对应于EP的适应度)
void CEP::EvalFitness()
{
	for(size_t i = 0; i < 2*m_iPopNum; i++ )
	{
		m_FFNN->InitWeight(m_Pop[i]);
		m_Fitness[i] = Big_Num - m_FFNN->Train();
		m_Index[i]   = i;
	}
}

//冒泡排序法
void CEP::Sort()
{
	for(size_t i =0; i<2*m_iPopNum-1; i++)
	{
		for(size_t j=0; j<2*m_iPopNum-1-i; j++)
		{
			//降序
			if( m_Win[j] < m_Win[j+1] )
			{
				swap( m_Win[j], m_Win[j+1] );
				swap( m_Index[j], m_Index[j+1] );
			}
		}
	}
}

void CEP::Run()
{		
	m_iGeneration = 0; 
	size_t i,j;
	//double GaussNum;
	while( m_iGeneration < m_iMaxGeneration && !m_bExit && GetLeastMSE() > m_dGoal )
	{
		for( i = 0; i < m_iPopNum; i++ )
		{
			//GaussNum = RandGauss();       
			for( j = 0; j < m_iChromoLength; j++ )
			{	
				//EP
				//m_Pop[i+m_iPopNum][j] = m_Pop[i][j] + RandGauss();
				
				//SEP
				m_Pop[i+m_iPopNum][j] = m_Pop[i][j] + sqrt(m_Fitness[i])*RandGauss();

				//MEP
				//...

				//CEP
				//m_Pop[i+m_iPopNum][j] = m_Pop[i][j] + m_PopDelta[i][j]*RandGauss();
				//m_PopDelta[i+m_iPopNum][j] = m_PopDelta[i][j]*exp(t1*GaussNum+t*RandGauss());

				//FEP
				//...
			}
		}
		//评价父代及其子代
		EvalFitness();	
		for( i = 0; i < 2*m_iPopNum; ++i )
		{
			m_Win[i] = 0;
			size_t iChosen;
			for( j = 0; j < m_iChosenOpponents; ++j )
			{
				iChosen = rand()%( 2*m_iPopNum );
				if( m_Fitness[i] > m_Fitness[iChosen] )
				{
					++m_Win[i];
				}
			}
		}
		//选择前μ个最好的个体进入下一代
		Sort();
		for( i = 0; i < m_iPopNum; ++i )
		{
			m_NewPop[i]      = m_Pop[m_Index[i]];		
			//m_NewPopDelta[i] = m_PopDelta[m_Index[i]];
			m_NewFitness[i]  = m_Fitness[m_Index[i]];
		}
		for( i = 0; i < m_iPopNum; ++i )
		{
			m_Pop[i]         = m_NewPop[i];	
			//m_PopDelta[i]    = m_NewPopDelta[i];
			m_Fitness[i]     = m_NewFitness[i];
		}
		//计算这μ个个体中最好的一个并保存
		ComputeFitness();
		++m_iGeneration;
	}	
	m_bExit = true;
}

void CEP::ComputeInitFitness()
{
	EvalFitness();
	ComputeFitness();
}