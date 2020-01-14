//读取、调整数据并进行储存
//储存每日、每月的cnt值
//仅对2015、2016年数据进行操作
//节日与假日视为等同
//不处理天气代码

#ifndef _data_correction_h
#define _data_correction_h

#include "std_lib_facilities.h"

namespace DATA
{
	struct data//储存数据用于可视化
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

	struct bp_data//储存数据用于bp网络
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

struct err//用于修正脏数据
{
	int year;
	int month;
	int day;
	bool is_day;
	bool is_correct;
	vector<int> lost_time;
	err(int y = 0, int m = 0, int d = 0, bool i = 0, bool c = 0) :year(y), month(m), day(d), is_day(i), is_correct(c) {}
};

struct coo//牛顿插值法的点
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
	vector<err> error_data;//记录脏数据
	vector<coo> hoo;//用于牛顿插值法
	double num;//牛顿插值法时要预测的点的编号

public:
	data_correction() {}
	void assignment(vector<DATA::data> &res, vector<DATA::bp_data> &ans, vector<DATA::bp_data> &test);//读入数据
	void out_traverse(vector<DATA::data> &res);//输出数据
	void traverse(vector<DATA::data> &res);//检验脏数据

	//牛顿插值法
	double value_t(double t, int i);//计算t的值
	double newton();//牛顿插值法
	int find(int i, int t, vector<DATA::bp_data> &ans, vector<int> &u, vector<DATA::data> &res);//寻找合适的插值点

	//处理脏数据
	void repair_day(vector<DATA::data> &res);//直接补一天的cnt
	void repair_time(vector<DATA::data> &res, vector<DATA::bp_data> &ans);//补一小时的cnt
};

#endif

