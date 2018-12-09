#ifndef FFNN_H
#define FFNN_H

#include <cmath>
#include <vector>

const int Big_Num = 100;

//Feed-Froward Neural Network
class CFFNN
{
protected:
	typedef std::vector<std::vector<double>> Sample;
	Sample m_InputValue;
	Sample m_OutputValue;
	std::vector<size_t> m_NeuronHidLayer;
	std::vector<Sample> m_ValueEachInst;
	std::vector<Sample> m_ErrorEachInst;
public:	
	size_t m_cInstance;
	size_t m_cHidLayer;
    size_t m_cInput;
    size_t m_cOutput;
	Sample m_Bias;
	double m_dError;
	//权值总数(含Bias),用于EA(对应于染色体长度)
	size_t m_cWeight;
	std::vector<Sample> m_Weight;
	std::vector<size_t> m_NeuronEachLayer;
public:
	CFFNN(){}
	~CFFNN(){}
	CFFNN( const Sample& P, const Sample& T, const std::vector<size_t>& S );
	void   InitWeight( const std::vector<double>& individual );
	double Train();
	void   Computing();
	double LogSigmoid(double x)
	{
		return 1/( 1 + exp(-x) );
	}
	double DetLogSigmoid(double x)
	{
		return LogSigmoid(x) - pow( LogSigmoid(x), 2.0 );
	}
};
#endif