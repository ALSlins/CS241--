//预测每日租用自行车的人数
//测试集暂时用2017年数据

#ifndef _data_forecasting_h
#define _data_forecasting_h

#include "std_lib_facilities.h"
#include "data_correction.h"

const int Layer_Max = 4;
const double PI = 3.1415927;
const int Layer_number[Layer_Max] = { 8,16,8,1 };
const int Neural_Max = 12;

struct error_data
{
	int i;
	int j;
	error_data(int a=0,int b=0):i(a),j(b){}
};

class bp
{
	int sum;
	double W[Layer_Max][Neural_Max][Neural_Max + 1];//权矩阵和阈值
	double Layer_Node[Layer_Max][Neural_Max];//各神经元输出
	double D[Layer_Max][Neural_Max];//各神经元误差微分
	double Study_Speed;//学习速率
	double e;//精度
	vector<error_data> avoid_data;

public:
	bp() {}
	void a_bp(vector<DATA::bp_data> &inputdata);
	double F(double x);//激发函数
	double cost(double out, double exp);//代价函数
	double networkout(DATA::bp_data &input, double hour);//对一组输入数据进行一次计算
	void alllayer_d(double ans);//计算误差微分
	void change_w();//修改权系数和阈值
	void train(vector<DATA::bp_data> &inputdata);//训练函数
	void aftertrainout();//输出训练结果
};

class bp_forecasting
{
	double W[Layer_Max][Neural_Max][Neural_Max + 1];//权矩阵和阈值
	double Layer_Node[Layer_Max][Neural_Max];//各神经元输出

public:
	bp_forecasting() {}
	void a_bp_forecasting();//初始化权矩阵
	int forcecast_cnt(double m, double d, double tt, double v, double s, double t, double h, double w);//输入预测条件
	double F(double x);//激发函数
	double networkout(DATA::bp_data &input, double hour);//计算预测结果
};

#endif