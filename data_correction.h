//��ȡ���������ݲ����д���
//����ÿ�ա�ÿ�µ�cntֵ
//����2015��2016�����ݽ��в���
//�����������Ϊ��ͬ
//��������������

#ifndef _data_correction_h
#define _data_correction_h

#include "std_lib_facilities.h"

namespace DATA
{
	struct data//�����������ڿ��ӻ�
	{
		int year;
		int month;
		int day;
		int hours;
		int cnt;
		bool vacation;
		double season;
		bool timenum[24];
		vector<int> lines;

		data(int y = 0, int m = 0, int d = 0, int h = 0, int c = 0, bool v = 0, double s = 0) :
			year(y), month(m), day(d), hours(h), cnt(c), vacation(v), season(s)
		{
			for (int i = 0; i < 24; ++i)timenum[i] = 0;
		}
	};

	struct bp_data//������������bp����
	{
		double month;
		double day;
		double vacation;
		double season;
		vector<double> t;
		vector<double> hum;
		vector<double> wind_speed;
		vector<double> cnt;

		bp_data(int m = 0, int d = 0, bool v = 0, double s = 0) :month(m), day(d), vacation(v), season(s) {}
	};
}

struct err//��������������
{
	int year;
	int month;
	int day;
	bool is_day;
	bool is_correct;
	vector<int> lost_time;
	err(int y = 0, int m = 0, int d = 0, bool i = 0, bool c = 0) :year(y), month(m), day(d), is_day(i), is_correct(c) {}
};

struct coo//ţ�ٲ�ֵ���ĵ�
{
	double x;
	double y;
	coo(double a, double b = 0)
	{
		x = a, y = b;
	}
};

class data_correction
{
	vector<err> error_data;//��¼������
	vector<coo> hoo;//����ţ�ٲ�ֵ��
	double num;//ţ�ٲ�ֵ��ʱҪԤ��ĵ�ı��

public:
	data_correction() {}
	void assignment(vector<DATA::data> &res, vector<DATA::bp_data> &ans, vector<DATA::bp_data> &test);//��������
	void out_traverse(vector<DATA::data> &res);//�������
	void traverse(vector<DATA::data> &res);//����������

	//ţ�ٲ�ֵ��
	double value_t(double t, int i);//����t��ֵ
	double newton();//ţ�ٲ�ֵ��
	int find(int i, int t, vector<DATA::bp_data> &ans, vector<int> &u, vector<DATA::data> &res);//Ѱ�Һ��ʵĲ�ֵ��

	//����������
	void repair_day(vector<DATA::data> &res);//ֱ�Ӳ�һ���cnt
	void repair_time(vector<DATA::data> &res, vector<DATA::bp_data> &ans);//��һСʱ��cnt
};

#endif

