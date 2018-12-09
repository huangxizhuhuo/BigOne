#ifndef EA_H
#define EA_H

#include "ffnn.h"

#define SAFE_DELETE(x) if(x) { delete x; x = NULL; }

extern size_t g_PopNum;
extern double g_CrossoverRate;
extern double g_MutateRate;
extern bool   g_Elitism;
extern size_t g_Num;
extern size_t g_Copies;
extern int    g_CrossoverOperator;
extern int    g_MutateOperator;
extern int    g_ScaleMethod;
extern bool   g_Scale;
extern int    g_SelectMethod;

typedef std::vector<double> Individual;
typedef std::vector<std::vector<double>> Pop;

class CEA
{
protected:
	size_t  m_iPopNum;
	size_t  m_iChromoLength;
	Pop     m_NewPop;
	double  m_dTotalFitness;
	double  m_dAverageFitness;
	double  m_dWorstFitness;
	double  m_dBestFitness;
public:	
	Pop     m_Pop;
	size_t  m_iMaxGeneration;
	size_t  m_iGeneration;
	size_t  m_iFittest;
	double  m_dGoal;	
	std::vector<double> m_BestFitness;
	std::vector<double> m_Fitness;
	CFFNN* m_FFNN;
	bool   m_bExit;
public:
	CEA();
	virtual ~CEA();
public:
	virtual void ComputeInitFitness() = 0;
	virtual void GenerateInitPop() = 0;
	virtual void Run() = 0;
	virtual void EvalFitness() = 0;
	void    ComputeFitness();
	void    SetFittest();
	double  RandGauss()
	{
		return 0.5*tan( 3.1415926*( rand()/(double)RAND_MAX - 0.5 ) );
	}
	double  GetLeastMSE() 
	{ 
		return Big_Num - m_dBestFitness;
	}
	double  RandCauchy();
};
#endif