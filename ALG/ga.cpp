#include "stdafx.h"
#include "ga.h"
using namespace std;

//J.Holland,1975
//Coded at June,2009
const double Boltzmann_DT      = 0.05;
const double Boltzmann_MinTemp = 1;

CGA::CGA( const Pop& P, const Pop& T, const vector<size_t>& S )
{
	m_iPopNum          =  g_PopNum;
	m_iSelectMethod    =  g_SelectMethod;
	m_dCrossoverRate   =  g_CrossoverRate;
	m_dMutateRate      =  g_MutateRate;
	m_iCrossoverOp     =  g_CrossoverOperator;
	m_iMutateOp        =  g_MutateOperator;
	m_bScale           =  g_Scale;
	m_iScaleMethod     =  g_ScaleMethod;
	m_bElitism         =  g_Elitism;
	m_iNum             =  g_Num;
	m_iCopies          =  g_Copies;
	
	m_FFNN = new CFFNN( P, T, S );
	m_iChromoLength = m_FFNN->m_cWeight;
	m_dBestFitness  = 0;
	m_iNewPopBegin  = 0;
	m_dWorstFitness = 0;
	m_bExit         = false;
	m_dSigma        = 1;
}

void CGA::GenerateInitPop()
{
	for( size_t i = 0; i < m_iPopNum; i++ )
	{	
		vector<double> temp;
		for( size_t j = 0; j < m_iChromoLength; j++ )
		{	
			temp.push_back( (rand()/(double)RAND_MAX)*1 - 0.5 );
		}
		m_Pop.push_back(temp); 
	}
	m_NewPop.assign( m_Pop.begin(), m_Pop.end() );
	m_Fitness.assign( m_iPopNum, 0 );
}

//评价适应度,越大越好,F=C-E
void CGA::EvalFitness()
{
	for( size_t i = 0; i < m_iPopNum; i++ )
	{
		m_FFNN->InitWeight(m_Pop[i]);
		m_Fitness[i] = Big_Num - m_FFNN->Train();
	}

	double dBestFitness  = 0;
	double dTotalFitness = 0;
	double dWorstFitness = INT_MAX;
	
	for( size_t i = 0; i < m_iPopNum; i++ )
	{
		if( m_Fitness[i] > dBestFitness )
		{
			dBestFitness = m_Fitness[i];
			m_iFittest = i;
		}
		if( m_Fitness[i] < dWorstFitness )
		{
			dWorstFitness = m_Fitness[i];
		}
		dTotalFitness += m_Fitness[i];
	}	
	//此代总体、最好、最差、平均适应度
	m_dTotalFitness   = dTotalFitness;
	m_dBestFitness    = dBestFitness;
	m_dWorstFitness   = dWorstFitness;
	m_dAverageFitness = m_dTotalFitness/m_iPopNum;
	m_BestFitness.push_back(m_dBestFitness);
}

//选择机制
Individual CGA::Select()
{
	Individual I;
	switch(m_iSelectMethod)
	{
	case 0:
		I = RouletteWheelSelect();
		break;
	case 1:
		I = TournamentSelect();
		break;
	}
	return I;
}

//轮盘法
Individual CGA::RouletteWheelSelect()
{
	double dSlice = (rand()/(double)RAND_MAX)*m_dTotalFitness;
	double dFitnessSoFar =0;
	size_t iSelected = 0;
	for(size_t i=0; i< m_Pop.size(); i++)
	{
		dFitnessSoFar += m_Fitness[i];
		if(dFitnessSoFar > dSlice)
		{
			iSelected = i;
			break;
		}	
	}
	return m_Pop[iSelected];
}

//2-竞争选择
Individual CGA::TournamentSelect()
{
	double bestFitness = INT_MAX;
	size_t iSelect;
	for( size_t i = 0; i < 2; i++ )
	{
		size_t r = rand()%m_iPopNum;
		if( m_Fitness[r] < bestFitness )
		{
			bestFitness = m_Fitness[r];
			iSelect     = r;
		}
	}
	return m_Pop[iSelect];
}

//交叉算子
void CGA::Crossover(Individual& P1, Individual& P2)
{
	switch(m_iCrossoverOp)
	{
		case 0:
			SinglePointCrossover(P1,P2);	
			break;
		case 1:
			TwoPointCrossover(P1,P2);
			break;
		case 2:
			UniformCrossover(P1,P2);
			break;
	}
}

void CGA::SinglePointCrossover(Individual& P1,Individual& P2)
{
	if((rand()/(double)RAND_MAX) > m_dCrossoverRate)	
		return;
	size_t p1 = rand()%m_iChromoLength;
	for( size_t i = p1; i < m_iChromoLength; i++ )
	{
		swap( P1[i], P2[i] );
	}
}

void CGA::TwoPointCrossover(Individual& P1,Individual& P2)
{
	if( (rand()/(double)RAND_MAX) > m_dCrossoverRate )
	{
		return;
	}
	size_t p1 = rand()%m_iChromoLength;
	size_t p2 = rand()%m_iChromoLength;
	if( p1 == p2 )
	{
		SinglePointCrossover(P1,P2);
		return;
	}
	swap( p1, p2 );
	for( size_t i = p1; i < p2; i++ )
	{
	    swap( P1[i], P2[i] );
	}
}

void CGA::UniformCrossover(Individual& P1, Individual& P2)
{
	if((rand()/(double)RAND_MAX) > m_dCrossoverRate)	
	{
		return;
	}
	vector<int> uniform;
	for(size_t i=0; i<m_iChromoLength; i++)
	{
		uniform.push_back(rand()%2);
	}
	for(size_t i=0; i<m_iChromoLength; i++)
	{
		if(uniform[i])
		{
			swap( P1[i], P2[i] );
		}
	}
}

//变异算子
void CGA::Mutate(Individual& P)
{
	switch(m_iMutateOp)
	{
	case 0:
		for( size_t i = 0; i < m_iChromoLength; i++ )
		{
			if( (rand()/(double)RAND_MAX) < m_dMutateRate )
			{
				P[i] += RandGauss();
			}
		}
		break;
	case 1:
		for( size_t i = 0; i < m_iChromoLength; i++ )
		{
			if( (rand()/(double)RAND_MAX) < m_dMutateRate )
			{
				P[i] += RandCauchy();
			}
		}
		break;
	}
}

//变比技术,为了应对"Super Individuals",需要对原始适应度进行变比
void CGA::Scaling()
{
	switch(m_iScaleMethod)
	{
	case 0:
		RankScaling();
		break;
	case 1:
		SimpleScaling();
		break;
	case 2:
		SigmaScaling();
		break;
	case 3:
		PowerScaling();	
		break;
	case 4:
		BoltzmannScaling();
		break;
	}
}

//变比之前先按适应度由大到小排序,适应度越大分数越高
void CGA::RankScaling()
{
	for( size_t i = 0; i < m_iPopNum; i++ )
	{
	    m_Fitness[i] = m_iPopNum - i;
	}
}

void CGA::SimpleScaling()
{
	for( size_t i = 0; i < m_iPopNum; i++ )
	{
	    m_Fitness[i] -= m_dWorstFitness;
	}
}

void CGA::PowerScaling()
{
	for( size_t i = 0; i < m_iPopNum; i++ )
	{
	    m_Fitness[i] = pow( m_Fitness[i], 0.5 );
	}
}

void CGA::SigmaScaling()
{
	double sum =0;
	for( size_t i = 0; i < m_iPopNum; i++ )
	{
		sum += ( m_Fitness[i] - m_dAverageFitness )*( m_Fitness[i] - m_dAverageFitness );
	}
	//方差
	double variance = sum/m_iPopNum;
	//标准差
	m_dSigma = sqrt(variance);
	if( m_dSigma > 1e-6 )
	{
		for( size_t i = 0; i < m_iPopNum; i++ )
		{
		    //m_Fitness[i] = ( m_Fitness[i] - m_dAverageFitness )/( 2*m_dSigma );//Buckland
			m_Fitness[i] -= ( m_dAverageFitness - 2*m_dSigma );//Yao		
			if( m_Fitness[i] > 0 )
			{
				m_Fitness[i] = 0;
			}
		}
	}
	//适应度趋于相同
	else
	{
		m_Fitness.assign( m_iPopNum, 1.0 );
	}
}

//Exponential scaling f(x)' = exp(f(x)/T)
void CGA::BoltzmannScaling()
{
	m_dBoltzmannTemp -= Boltzmann_DT;
	if( m_dBoltzmannTemp < Boltzmann_MinTemp )
	{
		m_dBoltzmannTemp = Boltzmann_MinTemp;
	}
	//double average = 0;
	for( size_t i = 0; i < m_iPopNum; i++ )
	{
		m_Fitness[i] = exp( m_Fitness[i]/m_dBoltzmannTemp );
		//average += m_Fitness[i];
	}
	//average /= m_iPopNum;
	//for( size_t i = 0; i < m_iPopNum; i++ )
	//{
	//	m_Fitness[i] /= average;
	//}
}

//精英统治
void CGA::ReserveNBest( size_t N, size_t NumCopies )
{
	m_iNewPopBegin = 0;
	for( size_t i = 0; i < N; i++ )
	{
		for(size_t j = 0; j < NumCopies; j++ )
		{
			m_NewPop[m_iNewPopBegin] = m_Pop[i];
			m_iNewPopBegin++;
		}
	}
}

//冒泡排序法
void CGA::Sort()
{
	for( size_t i = 0; i < m_iPopNum-1; i++ )
	{
		for( size_t j = 0; j < m_iPopNum-1-i; j++ )
		{
			//降序
			if( m_Fitness[j] < m_Fitness[j+1] )
			{
				swap( m_Pop[j], m_Pop[j+1] );
				swap( m_Fitness[j], m_Fitness[j+1] );
			}
		}
	}
}

void CGA::Run()
{
	m_iGeneration = 0;
	size_t i;
	while( m_iGeneration < m_iMaxGeneration && !m_bExit && GetLeastMSE() > m_dGoal )
	{	
		//排序,按适应度从大到小(开启精英保留,使用变比技术时需要对适应度进行排序)
		if( m_bElitism || m_bScale )
		{
			Sort();
		}
		//是否精英统治
		if( m_bElitism )
		{
			ReserveNBest( m_iNum, m_iCopies );
		}
		if( m_bScale )
		{
			Scaling();
			//仅需更新TotalFitness,用于赌轮选择法
			m_dTotalFitness = 0;	
			for( i = 0; i < m_iPopNum; i++ )
			{
				m_dTotalFitness += m_Fitness[i];
			}
		}

		//选择以及交叉
		Individual I1,I2;
		for( i = m_iNewPopBegin; i < m_iPopNum-2; i += 2 )
		{
		    I1 = Select();
		    I2 = Select();
		    Crossover( I1, I2 );
			m_NewPop[i] = I1;
			if( i+1 < m_iPopNum )
			{
			    m_NewPop[i+1] = I2;
			}
		}
		//变异,Global Mutation
		for( i = 0; i < m_iPopNum; i++ )
		{
			Mutate( m_NewPop[i] );
		}
		m_Pop = m_NewPop;

		//评价新生成的一组解,得到每个个体的适应度
		EvalFitness();
		
		++m_iGeneration;
	}
	m_bExit = true;
}

void CGA::ComputeInitFitness() 
{
	EvalFitness();
}