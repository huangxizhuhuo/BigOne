#include "stdafx.h"
#include "es.h"
using namespace std;

//Coded at April,2010
//(��+��)-ES

CES::CES( const Pop& P, const Pop& T, const std::vector<size_t>& S )
{
	m_FFNN = new CFFNN(P,T,S);	
	m_iChromoLength = m_FFNN->m_cWeight;
	t  = 1.0/(sqrt(2.0*sqrt(double(m_iChromoLength))));
	t1 = 1.0/(sqrt(2.0*m_iChromoLength));	
	m_bExit            = false;
	m_iPopNum          = g_PopNum;
	m_dBestFitness     = 0;	
	m_iChosenOpponents = 10;
}

void CES::GenerateInitPop()
{
	for( size_t i = 0; i < 2*m_iPopNum; i++ )
	{	
		vector<double> temp1,temp2;
		for(size_t j = 0; j<m_iChromoLength; j++)
		{	
			temp1.push_back((rand()/(double)RAND_MAX)*1.0 - 0.5);
			temp2.push_back(1.0);//3.0
			//temp2.push_back((rand()/(double)RAND_MAX)*1.0 - 0.5);
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

//ð������
void CES::Sort()
{
	for(size_t i =0; i < 2*m_iPopNum-1; i++)
	{
		for(size_t j=0; j < 2*m_iPopNum-1-i; j++)
		{
			//����
			if(m_Win[j] < m_Win[j+1])
			{
				swap( m_Win[j], m_Win[j+1] );
				swap( m_Index[j], m_Index[j+1] );
			}
		}
	}
}

void CES::Run()
{		
	m_iGeneration = 0; 
	size_t i,j;
	//double GaussNum;
	Individual I1,I2;
	while(m_iGeneration < m_iMaxGeneration && !m_bExit && GetLeastMSE() > m_dGoal)
	{
		//����,���ѡ������������н���
		for( i = 0; i < m_iPopNum; i+=2 )
		{
			Select(I1,I2);
		    Crossover(I1,I2);
			m_Pop[i+m_iPopNum]   = I1;
			m_Pop[i+1+m_iPopNum] = I2;
		}	
		//����,���²����ĸ�����б���
		for( i = 0; i < m_iPopNum; ++i )
		{
			//GaussNum = RandGauss();       
			for( j = 0; j< m_iChromoLength; ++j )
			{
				//m_Pop[i+m_iPopNum][j] = m_Pop[i][j] + m_PopDelta[i][j]*RandGauss();
				//m_PopDelta[i+m_iPopNum][j] = m_PopDelta[i][j]*exp(t1*GaussNum+t*RandGauss());
				m_Pop[i+m_iPopNum][j] = m_Pop[i][j] + RandGauss();
			}
		}
		//���۸��������Ӵ�
		EvalFitness();	
		//q-����ѡ��
		for( i = 0; i < 2*m_iPopNum; i++ )
		{
			m_Win[i] = 0;
			size_t iChosen;
			for( j = 0; j < m_iChosenOpponents; j++ )
			{
				iChosen= rand()%(2*m_iPopNum);
				if( m_Fitness[i] > m_Fitness[iChosen] )
				{
					m_Win[i]++;
				}
			}
		}
		//ѡ��ǰ�̸���õĸ��������һ��
		Sort();
		for( i = 0; i < m_iPopNum; i++ )
		{
			m_NewPop[i]      = m_Pop[m_Index[i]];		
			//m_NewPopDelta[i] = m_PopDelta[m_Index[i]];
			m_NewFitness[i]  = m_Fitness[m_Index[i]];
		}
		for( i = 0; i < m_iPopNum; i++ )
		{
			m_Pop[i]         = m_NewPop[i];	
			//m_PopDelta[i]    = m_NewPopDelta[i];
			m_Fitness[i]     = m_NewFitness[i];
		}
		//�������������������ڴ�Ⱥ���е���Ӧ�ȸߵ�,������̸���������õ�һ��������
		ComputeFitness();
		++m_iGeneration;
	}	
	m_bExit = true;
}

//��������2�̴�Ȩֵ,����ÿ�ε�������(��Ӧ��EP����Ӧ��)
void CES::EvalFitness()
{
	for(size_t i = 0; i < 2*m_iPopNum; i++)
	{
		m_FFNN->InitWeight(m_Pop[i]);
		//��ʵ����,Ϊ��GAͳһ,�򵥴���
		m_Fitness[i] = Big_Num - m_FFNN->Train();
		m_Index[i]   = i;
	}
}

void CES::Crossover(Individual& P1, Individual& P2)
{
	//ESʹ�þ��Ƚ���,����CGA::UniformCrossover
	vector<int> uniform;
	for( size_t i = 0; i < m_iChromoLength; i++ )
	{
		uniform.push_back(rand()%2);
	}
	for( size_t i = 0; i < m_iChromoLength; i++ )
	{
		if(uniform[i])
		{
			swap( P1[i], P2[i] );
		}
	}
}

void CES::Select(Individual& P1, Individual& P2)
{
	P1 = m_Pop[rand()%m_iPopNum];
	P2 = m_Pop[rand()%m_iPopNum];
}

void CES::ComputeInitFitness()
{
	EvalFitness();
	ComputeFitness();
}