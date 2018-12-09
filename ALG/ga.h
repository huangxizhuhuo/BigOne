#ifndef GA_H
#define GA_H

#include "ea.h"

class CGA : public CEA
{
private:	
	//------ѡ�����------
	int    m_iSelectMethod;
	//------��������------
	int    m_iCrossoverOp;
	double m_dCrossoverRate;
	//------��������------
	int    m_iMutateOp;	
	double m_dMutateRate;
	//------��Ӣͳ��------
	bool   m_bElitism;           
	size_t m_iNum;
	size_t m_iCopies;
	int    m_iNewPopBegin;
	//------��ȼ���------
	bool   m_bScale;
	int    m_iScaleMethod;
	double m_dSigma;  
	double m_dBoltzmannTemp;

	Individual Select();
	Individual RouletteWheelSelect();
	Individual TournamentSelect();	

	void Crossover(Individual& P1,Individual& P2);
	void SinglePointCrossover(Individual& P1,Individual& P2);
	void TwoPointCrossover(Individual& P1,Individual& P2);
	void UniformCrossover(Individual& P1,Individual& P2);

	void Mutate(Individual& P);
	
	void ReserveNBest(size_t N,size_t NumCopies);
	
	void Scaling();
	void RankScaling();
	void SimpleScaling();
	void SigmaScaling();
	void PowerScaling();
	void BoltzmannScaling();

	void Sort();
public:
	CGA( const Pop& P, const Pop& T, const std::vector<size_t>& S );
	void   GenerateInitPop();
	void   ComputeInitFitness(); 
	void   EvalFitness();
	void   Run();
};

#endif