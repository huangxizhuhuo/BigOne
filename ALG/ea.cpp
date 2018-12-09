#include "stdafx.h"
#include "ea.h"
using std::vector;

//只能使用全局变量
size_t g_PopNum    = 30;
bool   g_Elitism   = false;
bool   g_Scale     = false;
size_t g_Num       = 4;
size_t g_Copies    = 1;
double g_CrossoverRate     = 0.9;
double g_MutateRate        = 0.01;
int    g_CrossoverOperator = 0;
int    g_MutateOperator    = 0;
int    g_SelectMethod      = 0;
int    g_ScaleMethod       = 0;

CEA::CEA()
{
}

CEA::~CEA()
{
	SAFE_DELETE(m_FFNN);
}

void CEA::SetFittest()
{
	m_FFNN->InitWeight(m_Pop[m_iFittest]);
}

void CEA::ComputeFitness()
{
	double dBestFitness = 0;
	size_t iFittest = 0;
	size_t i;
	for( i = 0; i < m_iPopNum; i++ )
	{
		if( m_Fitness[i] > dBestFitness )
		{
			dBestFitness = m_Fitness[i];
			iFittest = i;
		}
	}	
	m_dBestFitness = dBestFitness;
	m_iFittest     = iFittest;
	m_BestFitness.push_back(m_dBestFitness);
	/*m_dBestFitness = m_Fitness[0];
	m_iFittest     = 0;
	m_BestFitness.push_back(m_dBestFitness);*/
}

double CEA::RandCauchy()
{
	double U = 1;
	double U1,U2;
	while(U>=1)
	{
		U1 = (rand()/(double)RAND_MAX)*2 - 1;
		U2 = (rand()/(double)RAND_MAX)*2 - 1;
		U = U1*U1 + U2*U2;
	}
	return U1*sqrt(-2*log(U)/U);//log=ln,log10=lg
}
