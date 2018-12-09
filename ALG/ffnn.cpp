#include "stdafx.h"
#include "ffnn.h"
using std::vector;

//Used For EAs to optimize the weight
CFFNN::CFFNN( const Sample &P, const Sample &T, const std::vector<size_t> &S )
{
	m_InputValue = P;		                      		
	m_OutputValue = T;		
	m_NeuronHidLayer = S;

	m_cInstance = P.size();
	m_cHidLayer = S.size();
	m_cInput = P[0].size(); 
	m_cOutput = T[0].size();

	size_t i,j;
	//m_NeuronEachLayer
	m_NeuronEachLayer.push_back(m_cInput);	
	for(i=0; i<m_NeuronHidLayer.size(); i++)
	{
		m_NeuronEachLayer.push_back(m_NeuronHidLayer[i]);
	}
	m_NeuronEachLayer.push_back(m_cOutput);
	vector<double> temp;
	vector<double> weight;
	vector<vector<double>> weightThisLayer;
	//m_Weight,m_Bias
	m_cWeight = 0;
	for(i=0; i<m_cHidLayer+1; i++)
	{
		weightThisLayer.clear();
		for(j=0; j<m_NeuronEachLayer[i]; j++)
		{
			weight.clear();
			weight.assign(m_NeuronEachLayer[i+1],0);
			weightThisLayer.push_back(weight);
			m_cWeight+=m_NeuronEachLayer[i+1];
		}
		m_Weight.push_back(weightThisLayer);
	}
	for(i=0; i<m_cHidLayer+1; i++)
	{
		weight.clear();
		weight.assign(m_NeuronEachLayer[i+1],0);
		m_Bias.push_back(weight);
		m_cWeight+=m_NeuronEachLayer[i+1];
	}
	//m_ValueEachInst,m_ErrorEachInst
	vector<double> valueThisLayer;
	vector<vector<double>> valueThisInst;
	for(i=0; i<m_cInstance; i++)//对每个实例	
	{
		valueThisInst.clear();
		for(j=0; j<=m_cHidLayer+1; j++)//隐层输出值+输出层输出值
		{
			valueThisLayer.clear();
			valueThisLayer.assign(m_NeuronEachLayer[j],1.0);
			valueThisInst.push_back(valueThisLayer);
		}
		m_ValueEachInst.push_back(valueThisInst);
		m_ErrorEachInst.push_back(valueThisInst);
	}
	for(i=0; i<m_cInstance; i++)//对每个实例	
	{	
		for(j=0; j<m_cInput; j++)
		{
			m_ValueEachInst[i][0][j] = m_InputValue[i][j];
		}	
	}
}

double CFFNN::Train()
{
	Computing();	
	double mse = 0;	
	for(size_t i=0; i<m_cInstance; i++)
	{
		for(size_t j=0; j<m_cOutput;j++)
		{
			mse+=m_ErrorEachInst[i][m_cHidLayer+1][j]*m_ErrorEachInst[i][m_cHidLayer+1][j];
		}//
	}
	mse = mse/(2.0*m_cInstance);//平方误差的均值
	return mse;
}

void CFFNN::InitWeight(const std::vector<double>& individual)
{
	size_t i,j,k;
	size_t cWeight=0;
	for(i=0; i<m_cHidLayer+1; i++)
	{
		for(j=0; j<m_NeuronEachLayer[i]; j++)
		{
			for(k=0; k<m_NeuronEachLayer[i+1];k++)
			{
				m_Weight[i][j][k] = individual[cWeight++];
			}
		}
	}
	for(i=0; i<m_cHidLayer+1; i++)
	{
		for(j=0; j<m_NeuronEachLayer[i+1]; j++)
		{
			m_Bias[i][j] = individual[cWeight++];
		}		
	}
}

void CFFNN::Computing()
{
	size_t i,j,m,n;
	double sum=0;
	for(i=0; i<m_cInstance; i++)//对每个实例
	{
		//ForwardComputing
		for(j=0; j<m_cHidLayer+1; j++)//需要计算(隐层数+1)次
		{
			for(m=0; m<m_NeuronEachLayer[j+1]; m++)//计算该层每个神经元的输出
			{
				sum=0;
				for(n=0; n<m_NeuronEachLayer[j]+1; n++)//多出来的1为阈值,恒为-1
				{
					if(n == m_NeuronEachLayer[j])
						sum+= (-1)*m_Bias[j][m];//(-1)*该层第m个神经元的Bias
					else//该层第m个神经元第i个实例的输出值*该层第m个神经元与前层第n个神经元的权值
						sum+= m_ValueEachInst[i][j][n]*m_Weight[j][n][m];
				}
				m_ValueEachInst[i][j+1][m] = LogSigmoid(sum);//激活值	
				if(j == m_cHidLayer)
				{	
					m_ErrorEachInst[i][j+1][m] = 		
						(m_OutputValue[i][m] - m_ValueEachInst[i][j+1][m]);//输出层误差
				}
			}		
		}	
	}
}

