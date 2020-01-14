//使用折线图与柱状图两种形式展现数据

#ifndef _data_visualization_h
#define _data_visualization_h

#include "Graph_lib.h"
#include "std_lib_facilities.h"
#include "data_correction.h"
#include "data_forecasting.h"

struct important//储存可视化所需要的数据
{
	vector<int> month_2015;
	vector<int> month_2016;
	vector<int> day_2015;
	vector<int> day_2016;
	vector<int> day;
	vector<int> season_2015;
	vector<int> season_2016;
};

class data_draw
{
public:
	important important_data;
	data_draw() {}
	void start_visualization();
	void a_data_draw(vector<DATA::data> &res);//初始化important
	void visualization();//调用窗口
	void line_chart(vector<int> &res_a, vector<int> &res_b);//普通折线图
	void line_chart(int op = 1);//两年折线图
	void bar_chart(vector<int> &res_a, vector<int> &res_b);//普通柱状图
};

//用来制造按钮及回调函数
class vis
{
public:
	vis() {}
	//折线图
	static void cb_line_chart_1(Address, Address pw)
	{
		reference_to<vis>(pw).line_chart_1();
	}
	void line_chart_1()
	{
		extern data_draw d_solution;
		d_solution.line_chart(1);
	}
	//折线图
	static void cb_line_chart_2(Address, Address pw)
	{
		reference_to<vis>(pw).line_chart_2();
	}
	void line_chart_2()
	{
		extern data_draw d_solution;
		d_solution.line_chart(2);
	}
	//折线图
	static void cb_line_chart_month(Address, Address pw)
	{
		reference_to<vis>(pw).line_chart_month();
	}
	void line_chart_month()
	{
		extern data_draw d_solution;
		d_solution.line_chart(d_solution.important_data.month_2015, d_solution.important_data.month_2016);
	}
	//折线图
	static void cb_line_chart_season(Address, Address pw)
	{
		reference_to<vis>(pw).line_chart_season();
	}
	void line_chart_season()
	{
		extern data_draw d_solution;
		d_solution.line_chart(d_solution.important_data.season_2015, d_solution.important_data.season_2016);
	}
	//柱状图
	static void cb_bar_chart_season(Address, Address pw)
	{
		reference_to<vis>(pw).bar_chart_season();
	}
	void bar_chart_season()
	{
		extern data_draw d_solution;
		d_solution.bar_chart(d_solution.important_data.season_2015, d_solution.important_data.season_2016);
	}
	//柱状图
	static void cb_bar_chart_month(Address, Address pw)
	{
		reference_to<vis>(pw).bar_chart_month();
	}
	void bar_chart_month()
	{
		extern data_draw d_solution;
		d_solution.bar_chart(d_solution.important_data.month_2015, d_solution.important_data.month_2016);
	}
	//输出每天数据
	static void cb_out_data(Address, Address pw)
	{
		reference_to<vis>(pw).out_data();
	}
	void out_data()
	{
		extern data_correction solution;
		extern vector<DATA::data> solu;

		solution.out_traverse(solu);
		cout << endl;
	}
	//脏数据寻找
	static void cb_dirty_data(Address, Address pw)
	{
		reference_to<vis>(pw).dirty_data();
	}
	void dirty_data()
	{
		extern data_correction solution;
		extern vector<DATA::data> solu;
		extern bool flag_dirty;

		if (flag_dirty)//确保仅调用一次
		{
			cout << "脏数据已寻找完毕" << endl;
			return;
		}

		solution.traverse(solu);
		cout << endl;
		flag_dirty = 1;
	}
	//脏数据修正
	static void cb_repair_data(Address, Address pw)
	{
		reference_to<vis>(pw).repair_data();
	}
	void repair_data()
	{
		extern data_correction solution;
		extern vector<DATA::data> solu;
		extern vector<DATA::bp_data> bp_d;
		extern bool flag_dirty;
		extern bool flag_repair;

		if (flag_dirty == 0 || flag_repair == 1)//确保仅调用一次且在上一个函数之后调用
		{
			if (flag_dirty == 0) cout << "请先寻找脏数据" << endl;
			if (flag_repair == 1) cout << "脏数据已修正" << endl;
			return;
		}

		solution.repair_time(solu, bp_d);
		solution.repair_day(solu);
		cout << endl;
		flag_repair = 1;
	}
	//可视化数据
	static void cb_vis_data(Address, Address pw)
	{
		reference_to<vis>(pw).vis_data();
	}
	void vis_data()
	{
		extern data_draw d_solution;
		extern vector<DATA::data> solu;

		d_solution.a_data_draw(solu);
		d_solution.visualization();
	}
	//训练bp模型
	static void cb_train_data(Address, Address pw)
	{
		reference_to<vis>(pw).train_data();
	}
	void train_data()
	{
		extern bp net;
		extern vector<DATA::bp_data> bp_d;

		cout << "开始训练" << endl;
		net.a_bp(bp_d);
		net.train(bp_d);
	}
	//使用预设好的bp模型
	static void cb_test_data(Address, Address pw)
	{
		reference_to<vis>(pw).test_data();
	}
	void test_data()
	{
		extern bp_forecasting fore;
		extern vector<DATA::bp_data> bp_t;
		//vector<DATA::bp_data> &bp_t = bp_d;

		fore.a_bp_forecasting();
		vector<int> a, b;
		for (int i = 0; i < bp_t.size(); ++i)
		{
			for (int j = 0; j < bp_t[i].cnt.size(); ++j)
			{
				a.push_back(fore.forcecast_cnt(bp_t[i].month, bp_t[i].day, j, bp_t[i].vacation, bp_t[i].season, bp_t[i].t[j], bp_t[i].hum[j], bp_t[i].wind_speed[j])), b.push_back(bp_t[i].cnt[j]);
				cout << "forecast: " << a[a.size() - 1] << "  reality: " << bp_t[i].cnt[j];
				cout << "  error: " << a[a.size() - 1] - bp_t[i].cnt[j];
				cout << "  rate: " << (a[a.size() - 1] - bp_t[i].cnt[j]) / bp_t[i].cnt[j] << endl;
			}
		}
		data_draw temp;
		for (int i = 0; i < a.size(); ++i)
		{
			a[i] = a[i] / 10, b[i] = b[i] / 10;
		}
		temp.line_chart(a, b);
	}
};

#endif