#ifndef ES_H
#define ES_H

#include "ea.h"

//Evolutionary Strategy
class CES :public CEA
{
private:
	Pop    m_PopDelta;
	Pop    m_NewPopDelta;
	double t;
	double t1;
	size_t m_iChosenOpponents;
	std::vector<double> m_NewFitness;
	std::vector<size_t> m_Win;
	std::vector<size_t> m_Index;
	void Sort();	
	//ES有交叉操作,EP则没有
	void Crossover(Individual& P1, Individual& P2);
	void Select(Individual& P1, Individual& P2);
public:
	CES( const Pop& P, const Pop& T, const std::vector<size_t>& S );
	void GenerateInitPop();
	void ComputeInitFitness();
	void EvalFitness();
	void Run();
};
#endif