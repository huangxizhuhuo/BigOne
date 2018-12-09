#ifndef BP_H
#define BP_H

#include "ffnn.h"

//Back-Propagation Learning Algorithm 
class CBP : public CFFNN
{
private:
	Sample m_Delta;	
public:	
	size_t m_iEpoch;
	size_t m_iMaxEpoch;
	double m_dLr; 
	double m_dInitMse;
	double m_dGoal;
	bool   m_bExit;
	std::vector<double> m_Perf;	
public:
	CBP(const Sample& P, const Sample& T, const std::vector<size_t>& S);
	void InitWeight();
	void Train();
	void Computing();
	void ForwardComputing(size_t i);
	void BackwardComputing(size_t i);
	void ComputeInitMse();
	void SetWeight(const std::vector<double>& individual);
	void SetPerf(const std::vector<double>& fitness);
	void SetInitMse(double bestfitness)
	{
		m_dInitMse = Big_Num - bestfitness;
	}
};
#endif