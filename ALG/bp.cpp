#include "stdafx.h"
#include "bp.h"
#include <fstream>
using std::ofstream;
using std::vector;

//Rumelhart,1985
//Coded at June,2009
CBP::CBP( const Sample &P, const Sample &T, const std::vector<size_t> &S )
{	
	m_iEpoch    = 0;
	m_dLr       = 0.1;
	m_dGoal     = 0.01;
	m_iMaxEpoch = 100000;
	m_dError    = 1.0;
	m_bExit     = false;

	m_InputValue     = P;		                      		
	m_OutputValue    = T;		
	m_NeuronHidLayer = S;
	m_cInstance      = P.size();;
	m_cHidLayer      = S.size();;
	m_cInput         = P[0].size(); 
	m_cOutput        = T[0].size();
}

void CBP::Train()
{
	size_t i,j,k;
	double error;
	while( m_dError > m_dGoal && m_iEpoch < m_iMaxEpoch &&!m_bExit )
	{
		++m_iEpoch;
		//每输入一个示例,权值就调整一次
		Computing();
		error = 0;
		//计算神经网络的输出误差
		for( i = 0; i < m_cInstance; i++ )
		{
			ForwardComputing(i);
		}
		for( i = 0; i < m_cInstance; i++ )
		{
			for( j = 0; j < m_cOutput; j++ )
			{
				error += m_ErrorEachInst[i][m_cHidLayer+1][j]*m_ErrorEachInst[i][m_cHidLayer+1][j];
			}
		}
		//平方误差的均值
		error = error/(2.0*m_cInstance);
		m_dError = error;
		m_Perf.push_back(m_dError);
	}
	ofstream outf("data/ANNWeights.txt");
	for( i = 0; i < m_cHidLayer+1; i++ )
	{
		outf<<"\n第"<<i+1<<"层到第"<<i+2<<"层的连接权值：\n";
		for( j = 0; j < m_NeuronEachLayer[i]; j++ )
		{
			for( k = 0; k < m_NeuronEachLayer[i+1]; k++ )
			{
				outf<<m_Weight[i][j][k]<<",";
			}
		}
		outf<<"\n";
	}
	for( i = 0; i < m_cHidLayer+1; i++ )
	{
		outf<<"\n第"<<i+2<<"层神经元的阈值：\n";
		for( j = 0; j < m_NeuronEachLayer[i+1]; j++ )
		{
			outf<<m_Bias[i][j]<<",";
		}
		outf<<"\n";
	}
	m_bExit = true;
}

void CBP::InitWeight()
{
	size_t i,j,k;
	//m_NeuronEachLayer
	m_NeuronEachLayer.push_back(m_cInput);	
	for(i=0; i<m_NeuronHidLayer.size(); i++)
	{
		m_NeuronEachLayer.push_back(m_NeuronHidLayer[i]);
	}
	m_NeuronEachLayer.push_back(m_cOutput);
		
	vector<double> weight;
	vector<vector<double>> weightThisLayer;
	//m_Weight,m_Bias
	for(i=0; i<m_cHidLayer+1; i++)
	{
		weightThisLayer.clear();
		for(j=0; j<m_NeuronEachLayer[i]; j++)
		{
			weight.clear();
			for(k=0; k<m_NeuronEachLayer[i+1]; k++)
			{
				weight.push_back((rand()/(double)RAND_MAX)-0.5);
			}
			weightThisLayer.push_back(weight);
		}
		m_Weight.push_back(weightThisLayer);
	}
	for(i=0; i<m_cHidLayer+1; i++)
	{
		weight.clear();
		for(k=0; k<m_NeuronEachLayer[i+1]; k++)
		{
			weight.push_back((rand()/(double)RAND_MAX)-0.5);
		}
		m_Bias.push_back(weight);
	}
	//m_ValueEachInst,m_ErrorEachInst
	vector<double> valueThisLayer;
	vector<vector<double>> valueThisInst;
	//对每个实例
	for(i=0; i<m_cInstance; i++)	
	{
		valueThisInst.clear();
		//隐层输出值+输出层输出值
		for(j=0; j<=m_cHidLayer+1; j++)
		{
			valueThisLayer.clear();
			valueThisLayer.assign(m_NeuronEachLayer[j],1.0);
			valueThisInst.push_back(valueThisLayer);
		}
		m_ValueEachInst.push_back(valueThisInst);
		m_ErrorEachInst.push_back(valueThisInst);
	}
	//对每个实例
	for(i=0; i<m_cInstance; i++)	
	{	
		for(j=0; j<m_cInput; j++)
		{
			m_ValueEachInst[i][0][j] = m_InputValue[i][j];
		}	
	}
	//m_Delta
	vector<double> delta;
	for(i=0;i<=m_cHidLayer+1; i++)
	{
		delta.clear();
		delta.assign(m_NeuronEachLayer[i],1.0);
		m_Delta.push_back(delta);
	}
}

void CBP::Computing()
{
	//对每个实例
	for( size_t iInst = 0; iInst < m_cInstance; iInst++ )
	{	
		ForwardComputing( iInst );
		BackwardComputing( iInst );
	}
}

void CBP::ForwardComputing( size_t i )
{
	size_t j,m,n;
	double sum=0;
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

void CBP::BackwardComputing(size_t i)
{
	size_t j,m,n,p,q;
	//BackwardComputing
	for(j=m_cHidLayer+1; j>0; j--)
	{
		//调整该层第m个神经元与前层所有神经元的连接权值,含阈值
		for(m=0; m<m_NeuronEachLayer[j]; m++)
		{
			//首先从输出层开始
			m_Delta[j][m] = m_ValueEachInst[i][j][m]*
				(1-m_ValueEachInst[i][j][m])*m_ErrorEachInst[i][j][m];
			for(n=0; n<m_NeuronEachLayer[j-1]; n++)
			{
				m_Weight[j-1][n][m] += (m_dLr*m_Delta[j][m]*m_ValueEachInst[i][j-1][n]);//
			}
			m_Bias[j-1][m] += (m_dLr*m_Delta[j][m]*(-1));
			//计算j-1层的模拟误差,m_ErrorEachLayer[j-1][i][m]		
			for(p=0; p<m_NeuronEachLayer[j-1]; p++)
			{
				m_ErrorEachInst[i][j-1][p] =0;
				for(q=0; q<m_NeuronEachLayer[j]; q++)
				{	
					m_ErrorEachInst[i][j-1][p] += m_Delta[j][q]*m_Weight[j-1][p][q];	
				}
			}						
		}	
	}
}

void CBP::ComputeInitMse()
{
	size_t i,j,m,n;
	double sum=0;
	//对每个实例
	for(i=0; i<m_cInstance; i++)
	{
		//ForwardComputing,需要计算(隐层数+1)次
		for(j=0; j<m_cHidLayer+1; j++)
		{
			//计算该层每个神经元的输出
			for(m=0; m<m_NeuronEachLayer[j+1]; m++)
			{
				sum = 0;
				//多出来的1为阈值,恒为-1
				for(n=0; n<m_NeuronEachLayer[j]+1; n++)
				{
					//(-1)*该层第m个神经元的Bias
					if(n == m_NeuronEachLayer[j])
						sum+= (-1)*m_Bias[j][m];
					//该层第m个神经元第i个实例的输出值*该层第m个神经元与前层第n个神经元的权值
					else
						sum+= m_ValueEachInst[i][j][n]*m_Weight[j][n][m];
				}	
				//激活值
				m_ValueEachInst[i][j+1][m] = LogSigmoid(sum);	
				if(j == m_cHidLayer)
				{	
					//输出层误差
					m_ErrorEachInst[i][j+1][m] = 		
						(m_OutputValue[i][m] - m_ValueEachInst[i][j+1][m]);
				}
			}		
		}
	}
	m_dInitMse = 0;	
	for(size_t i=0; i<m_cInstance; i++)
	{
		m_dInitMse += m_ErrorEachInst[i][m_cHidLayer+1][0]*m_ErrorEachInst[i][m_cHidLayer+1][0];
	}
	m_dInitMse=m_dInitMse/(2.0*m_cInstance);
	m_Perf.push_back(m_dInitMse);
}

//根据EA的个体设置神经网络的权值
void CBP::SetWeight(const std::vector<double>& individual)
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

//用于EA-BP,记录EA的每代误差
void CBP::SetPerf(const vector<double>& fitness)
{
	m_iEpoch += fitness.size();
	//m_Perf = fitness;
	for( size_t i = 0; i < fitness.size(); ++i )
	{
		m_Perf.push_back( Big_Num - fitness[i]);
	}
}