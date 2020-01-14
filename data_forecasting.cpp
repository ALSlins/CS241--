#include "data_forecasting.h"

const double t_MIN = 2;//取绝对值
const double t_MAX = 36;
const double wind_speed_MAX = 58;
const double cnt_MAX = 4000;

void bp::a_bp(vector<DATA::bp_data> &inputdata)
{
	sum = 0;

	srand(time(nullptr));
	for (int i = 1; i < Layer_Max; ++i)//Layer=0时是输入层
	{
		for (int j = 0; j < Layer_number[i]; ++j)
		{
			for (int k = 0; k < Layer_number[i - 1] + 1; ++k)//k=Layer_number[i-1]时是第[i][j]神经元阈值
			{
				W[i][j][k] = rand();
				W[i][j][k] /= RAND_MAX;//确保不是0
				cout << W[i][j][k] << ',';
			}
		}
	}
	cout << endl;

	if (inputdata[0].day < 1)
	{
		Study_Speed = 0.02;
		e = 0.0001;
		return;
	}

	//输入输出归一化
	for (int i = 0; i < inputdata.size(); ++i)
	{
		inputdata[i].month /= 12.0;
		inputdata[i].day /= 31.0;
		inputdata[i].season /= 3.0;

		for (int j = 0; j < inputdata[i].cnt.size(); ++j)
		{
			++sum;

			inputdata[i].t[j] = (inputdata[i].t[j] + t_MIN) / t_MAX;
			inputdata[i].hum[j] /= 100.0;
			inputdata[i].wind_speed[j] /= wind_speed_MAX;
			inputdata[i].cnt[j] /= cnt_MAX;

			if (inputdata[i].t[j] < 0 || inputdata[i].t[j] >1) cout << inputdata[i].t[j] << " t is overflow" << endl;
			if (inputdata[i].hum[j] > 1)cout << inputdata[i].hum[j] << " hum is overflow" << endl;
			if (inputdata[i].wind_speed[j] > 1)cout << inputdata[i].wind_speed[j] << " wind is overflow" << endl;
			if (inputdata[i].cnt[j] > 1)
			{
				avoid_data.push_back(error_data(i, j));
				cout << inputdata[i].cnt[j] << " cnt is overflow" << endl;
			}
		}
	}

	Study_Speed = 0.02;
	e = 0.0001;
}

double bp::F(double x)//激发函数
{
	return (1.0 / (1.0 + exp(-x)));
}

double bp::cost(double out, double exp)
{
	return (pow(out - exp, 2));
}

double bp::networkout(DATA::bp_data &input, double hour)
{
	int i, j, k;
	double N_node[Layer_Max][Neural_Max];//第[i][j]个神经元的总输入

	Layer_Node[0][0] = input.month;
	Layer_Node[0][1] = input.day;
	Layer_Node[0][2] = hour / 23.0;
	Layer_Node[0][3] = input.vacation;
	Layer_Node[0][4] = input.season;
	Layer_Node[0][5] = input.t[hour];
	Layer_Node[0][6] = input.hum[hour];
	Layer_Node[0][7] = input.wind_speed[hour];

	for (i = 1; i < Layer_Max; ++i)
	{
		for (j = 0; j < Layer_number[i]; ++j)
		{
			N_node[i][j] = 0.0;
			for (k = 0; k < Layer_number[i - 1]; ++k)
			{
				N_node[i][j] += Layer_Node[i - 1][k] * W[i][j][k];
			}
			N_node[i][j] = N_node[i][j] - W[i][j][k];//减阈值
			Layer_Node[i][j] = F(N_node[i][j]);//第[i][j]个神经元的输出
		}
	}

	return Layer_Node[Layer_Max - 1][0];//最终输出结果
}

void bp::alllayer_d(double ans)
{
	int i, j, k;
	double temp;
	D[Layer_Max - 1][0] = Layer_Node[Layer_Max - 1][0] * (1 - Layer_Node[Layer_Max - 1][0])*(Layer_Node[Layer_Max - 1][0] - ans);

	for (int i = Layer_Max - 1; i > 0; i--)
	{
		for (int j = 0; j < Layer_number[i - 1]; ++j)
		{
			temp = 0;
			for (int k = 0; k < Layer_number[i]; ++k)
			{
				temp = temp + W[i][k][j] * D[i][k];
			}
			D[i - 1][j] = Layer_Node[i - 1][j] * (1 - Layer_Node[i - 1][j])*temp;
		}
	}
}

void bp::change_w()
{
	int i, j, k;
	for (i = 1; i < Layer_Max; ++i)
	{
		for (j = 0; j < Layer_number[i]; ++j)
		{
			for (k = 0; k < Layer_number[i - 1]; ++k)
			{
				W[i][j][k] = W[i][j][k] - Study_Speed * D[i][j] * Layer_Node[i - 1][k];
			}
			W[i][j][k] = W[i][j][k] + Study_Speed * D[i][j];
		}
	}
}

void bp::train(vector<DATA::bp_data> &inputdata)
{
	int i, j;
	int ok = 0;
	double res;
	long int count = 0;
	double err;
	ofstream Out_W_File("Out_W_File.txt");
	//ofstream Out_r("Out_r.csv");

	while (ok < 2400)
	{
		count++;
		int u = 0;

		for (i = 0, ok = 0; i < inputdata.size(); ++i)
		{
			for (double j = 0; j < inputdata[i].cnt.size(); ++j)
			{
				if ((u < avoid_data.size()) && (i == avoid_data[u].i) && (j == avoid_data[u].j))
				{
					++u;
					continue;
				}

				res = networkout(inputdata[i], j);
				alllayer_d(inputdata[i].cnt[j]);
				err = cost(res, inputdata[i].cnt[j]);//计算误差
				//Out_r << count << ',' << res << ',' << err << endl;//保存中间结果
				if (err < e)ok++;
				else change_w();
			}
		}
		if (count % 500 == 0)//保存中间权系数
		{
			if (count == 500)Study_Speed = 0.01;
			if (count == 1000)Study_Speed = 0.005;
			cout << endl;
			cout << count << " " << err << endl;
			Out_W_File << count << ",";
			Out_W_File << err << ",\n";

			for (int p = 1; p < Layer_Max; ++p)
			{
				for (int j = 0; j < Layer_number[p - 1]; ++j)
				{
					for (int k = 0; k < Layer_number[p - 1] + 1; ++k)
					{
						Out_W_File << "W" << '[' << p << ']' << '[' << j << ']' << '[' << k << ']' << '=' << W[p][j][k] << ' ' << ' ';
					}
				}
			}
			Out_W_File << '\n' << '\n';
		}
	}
	cout << err << endl;

	aftertrainout();
}

void bp::aftertrainout()
{
	//输出训练结果
	cout << endl;
	cout << "训练后的权系数以及阈值" << endl;
	for (int i = 1; i < Layer_Max; ++i)
	{
		for (int j = 0; j < Layer_number[i]; ++j)
		{
			for (int k = 0; k < Layer_number[i - 1] + 1; ++k)
			{
				cout << W[i][j][k] << ",";
			}
			cout << endl;
		}
	}
	//保存训练结果
	ofstream final_result("final_result.txt");

	for (int i = 1; i < Layer_Max; ++i)
	{
		for (int j = 0; j < Layer_number[i]; ++j)
		{
			for (int k = 0; k < Layer_number[i - 1] + 1; ++k)
			{
				final_result << W[i][j][k] << ",";
			}
		}
	}
	//储存训练结果
	extern vector<double> w_array;
	w_array.clear();
	for (int i = 1; i < Layer_Max; ++i)
	{
		for (int j = 0; j < Layer_number[i]; ++j)
		{
			for (int k = 0; k < Layer_number[i - 1] + 1; ++k)
			{
				w_array.push_back(W[i][j][k]);
			}
		}
	}
}

void bp_forecasting::a_bp_forecasting()
{
	extern vector<double> w_array;
	//预先训练的模型数据
	if (w_array.size() == 0)
	{
		vector<double> temp_w_array = { 0.212075,-0.206815,1.06514,2.08119,0.961459,-0.423253,0.680656,1.06724,1.07328,
0.448584,0.0337334,1.978,-0.18589,1.27646,0.957269,0.408095,1.31561,0.335522,
-0.0478375,-0.186441,8.31009,0.895605,0.186059,-0.516406,0.63926,-0.0680498,6.11277,
0.138677,0.105984,14.2223,-6.4579,0.242179,-0.0606927,0.123339,0.492167,6.00523,
1.18191,1.70748,0.282855,0.353265,1.30193,-0.789726,0.387579,1.50847,0.581764,
-0.772663,0.0261306,0.19706,-0.0346176,0.825058,-2.54119,2.14912,0.155339,-0.0418111,
0.910191,-0.576656,-2.25658,1.31725,1.09503,-1.07637,2.60159,0.983482,1.63008,
0.483614,1.03464,-0.62625,2.27348,0.040483,1.64922,-0.622383,0.712243,1.48666,
-0.154935,-0.303983,10.3833,1.02009,-0.19546,0.129344,0.104992,-0.624986,2.27508,
-0.414433,0.038133,0.848695,-0.935424,-1.5831,-1.34729,1.77754,0.432189,1.32262,
0.258876,0.146101,5.24316,-1.52866,-0.419764,1.85579,-1.30962,-0.298937,4.01992,
0.0373439,0.215688,11.5222,-0.643614,0.018,0.57848,-1.20328,-0.245072,8.37708,
0.286794,1.11179,0.114978,0.428135,1.14552,0.780837,0.370111,0.299514,0.385689,
-0.138426,-0.0491718,0.256524,-0.173457,0.453586,0.485811,-0.201903,-0.490795,0.318475,
-0.80469,0.965614,0.261682,-0.193292,0.00345098,-0.0315989,0.994867,-0.353892,-0.208788,
0.861622,-2.71905,3.37012,-0.189435,3.3618,-0.963945,1.4012,-2.85534,1.42211,
0.286794,1.11179,0.114978,0.428135,1.14552,0.780837,0.370111,0.299514,0.385689,0.484066,0.468895,0.936492,0.766583,-0.138426,-0.0491718,0.256524,-0.173457,
-0.138426,-0.0491718,0.256524,-0.173457,0.453586,0.485811,-0.201903,-0.490795,0.318475,0.537726,0.260789,0.151832,-0.290496,-0.80469,0.965614,0.261682,-0.193292,
-0.80469,0.965614,0.261682,-0.193292,0.00345098,-0.0315989,0.994867,-0.353892,-0.208788,0.385107,-0.321153,1.01419,-0.0143164,0.861622,-2.71905,3.37012,-0.189435,
0.861622,-2.71905,3.37012,-0.189435,3.3618,-0.963945,1.4012,-2.85534,1.42211,0.0481476,0.370174,1.86491,-1.60766,-1.81502,0.191833,-2.02214,-0.124714,
-1.81502,0.191833,-2.02214,-0.124714,-0.316043,2.52484,1.19095,-0.403813,-0.237613,0.934031,-1.24293,1.67566,0.0489258,-0.0911117,-0.602466,-0.485746,0.185215,
-0.0911117,-0.602466,-0.485746,0.185215,-0.177955,1.79584,2.65359,0.920384,2.0567,2.63713,-1.14763,8.86838,0.0587396,-0.515441,1.11546,-0.295353,8.70012,
-0.515441,1.11546,-0.295353,8.70012,2.42305,-0.324722,-0.858406,-1.77995,7.45052,1.62192,4.08299,0.135825,1.94879,-0.927554,-0.493127,-5.78041,9.24417,
-0.927554,-0.493127,-5.78041,9.24417,-1.7846,-0.628324,0.386734,0.950396,4.55029,-1.75893,-3.18589,-5.67989,-0.314541,-0.422501,-1.31081,-1.35678,1.84919,
0.377074,-0.0969586,-0.00919853,-0.506039,-2.93952,-8.68091,8.28821,-7.21162,1.19296};
		for (int i = 0; i < temp_w_array.size(); ++i)w_array.push_back(temp_w_array[i]);
		temp_w_array.clear();
	}

	//载入权系数矩阵
	int u = 0;
	for (int i = 1; i < Layer_Max; ++i)
	{
		for (int j = 0; j < Layer_number[i]; ++j)
		{
			for (int k = 0; k < Layer_number[i - 1] + 1; ++k)
			{
				W[i][j][k] = w_array[u++];
			}
		}
	}
}

int bp_forecasting::forcecast_cnt(double m, double d, double tt, double v, double s, double t, double h, double w)
{
	//归一化
	DATA::bp_data temp_data(m / 12.0, d / 31.0, v, s / 3.0);
	temp_data.t.push_back((t + t_MIN) / t_MAX), temp_data.hum.push_back(h / 100.0), temp_data.wind_speed.push_back(w / wind_speed_MAX);

	double fore_res = networkout(temp_data, tt);//计算

	//反归一化
	return (fore_res*cnt_MAX);
}

double bp_forecasting::F(double x)
{
	return (1.0 / (1.0 + exp(-x)));
}

double bp_forecasting::networkout(DATA::bp_data & input, double hour)
{
	int i, j, k;
	double N_node[Layer_Max][Neural_Max];//第[i][j]个神经元的总输入

	//初始化
	Layer_Node[0][0] = input.month;
	Layer_Node[0][1] = input.day;
	Layer_Node[0][2] = hour / 23.0;
	Layer_Node[0][3] = input.vacation;
	Layer_Node[0][4] = input.season;
	Layer_Node[0][5] = input.t[0];
	Layer_Node[0][6] = input.hum[0];
	Layer_Node[0][7] = input.wind_speed[0];

	//计算
	for (i = 1; i < Layer_Max; ++i)
	{
		for (j = 0; j < Layer_number[i]; ++j)
		{
			N_node[i][j] = 0.0;
			for (k = 0; k < Layer_number[i - 1]; ++k)
			{
				N_node[i][j] += Layer_Node[i - 1][k] * W[i][j][k];
			}
			N_node[i][j] = N_node[i][j] - W[i][j][k];//减阈值
			Layer_Node[i][j] = F(N_node[i][j]);//第[i][j]个神经元的输出
		}
	}

	return Layer_Node[Layer_Max - 1][0];//最终输出结果
}
