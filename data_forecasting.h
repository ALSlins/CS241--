//Ԥ��ÿ���������г�������
//���Լ���ʱ��2017������

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
	double W[Layer_Max][Neural_Max][Neural_Max + 1];//Ȩ�������ֵ
	double Layer_Node[Layer_Max][Neural_Max];//����Ԫ���
	double D[Layer_Max][Neural_Max];//����Ԫ���΢��
	double Study_Speed;//ѧϰ����
	double e;//����
	vector<error_data> avoid_data;

public:
	bp() {}
	void a_bp(vector<DATA::bp_data> &inputdata);
	double F(double x);//��������
	double cost(double out, double exp);//���ۺ���
	double networkout(DATA::bp_data &input, double hour);//��һ���������ݽ���һ�μ���
	void alllayer_d(double ans);//�������΢��
	void change_w();//�޸�Ȩϵ������ֵ
	void train(vector<DATA::bp_data> &inputdata);//ѵ������
	void aftertrainout();//���ѵ�����
};

class bp_forecasting
{
	double W[Layer_Max][Neural_Max][Neural_Max + 1];//Ȩ�������ֵ
	double Layer_Node[Layer_Max][Neural_Max];//����Ԫ���

public:
	bp_forecasting() {}
	void a_bp_forecasting();//��ʼ��Ȩ����
	int forcecast_cnt(double m, double d, double tt, double v, double s, double t, double h, double w);//����Ԥ������
	double F(double x);//��������
	double networkout(DATA::bp_data &input, double hour);//����Ԥ����
};

#endif