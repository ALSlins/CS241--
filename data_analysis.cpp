#include "std_lib_facilities.h"
#include "data_correction.h"
#include "data_forecasting.h"
#include "data_visualization.h"
#include "Graph_lib.h"

//主要是为了按钮设计方便
data_correction solution;//用于数据读取及修正，任务一、二
data_draw d_solution;//用于可视化，任务三
bp_forecasting fore;//用于预测，任务四
bp net;//用于训练模型，任务四
//储存数据
vector<DATA::data> solu;//保存基本数据
vector<DATA::bp_data> bp_d;//训练集
vector<DATA::bp_data> bp_t;//测试集
vector<double> w_array;//模型权矩阵
bool flag_dirty = 0;
bool flag_repair = 0;

int main()
{
	solution.assignment(solu, bp_d, bp_t);//读取数据
	d_solution.start_visualization();//主操作界面

	//double m, d, tt, v, s, t, h, w;
	return 0;
}