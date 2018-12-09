#ifndef EP_H
#define EP_H

#include "ea.h"

//Evolutionary Programming
class CEP :public CEA
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
public:
	CEP( const Pop& P, const Pop& T, const std::vector<size_t>& S );
	void GenerateInitPop();
	void ComputeInitFitness();
	void EvalFitness();
	void Run();
};
#endif