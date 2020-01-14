/*
"timestamp" - timestamp field for grouping the data
"cnt" - the count of a new bike shares   //int
"t1" - real temperature in C    //double
"t2" - temperature in C "feels like"    //double
"hum" - humidity in percentage    //double
"wind_speed" - wind speed in km/h     //double
"weather_code" - category of the weather
"is_holiday" - boolean field - 1 holiday / 0 non holiday
"is_weekend" - boolean field - 1 if the day is weekend
"season" - category field meteorological seasons: 0-spring ; 1-summer; 2-fall; 3-winter.

"weathe_code" category description:
1 = Clear ; mostly clear but have some values with haze/fog/patches of fog/ fog in vicinity
2 = scattered clouds / few clouds
3 = Broken clouds
4 = Cloudy
7 = Rain/ light Rain shower/ Light rain
10 = rain with thunderstorm
26 = snowfall
94 = Freezing Fog
*/
#include "data_correction.h"

void data_correction::assignment(vector<DATA::data>& res, vector<DATA::bp_data> &ans, vector<DATA::bp_data> &test)
{
	ifstream infile;
	infile.open("london_merged.csv");
	string temp;//读取csv
	getline(infile, temp);//跳过开头

	string temp_month, temp_day, temp_hour, temp_cnt, temp_t, temp_hum, temp_wind;
	string later_month = "01", later_day = "03", later_hour = "00";
	int line = 1;

	while (getline(infile, temp))
	{
		++line;
		int flag = 0;//,的数目
		if (temp[3] == '7') break;

		temp_month += temp[5], temp_day += temp[8], temp_hour += temp[11];//计算月份天数小时
		temp_month += temp[6], temp_day += temp[9], temp_hour += temp[12];

		for (int i = 0; i < temp.length(); ++i)
		{
			if (temp[i] == ',')++flag;
			if (flag == 1 && temp[i] != ',') temp_cnt += temp[i];
			if (flag == 2 && temp[i] != ',')temp_t += temp[i];
			if (flag == 4 && temp[i] != ',')temp_hum += temp[i];
			if (flag == 5 && temp[i] != ',')temp_wind += temp[i];
			if (flag == 6)break;
		}

		//修改ans
		if (later_day != temp_day) ans.push_back(DATA::bp_data(stod(temp_month), stod(temp_day), temp[temp.length() - 7] - '0' + temp[temp.length() - 11] - '0', temp[temp.length() - 3] - '0'));
		ans[ans.size() - 1].cnt.push_back(stod(temp_cnt));
		ans[ans.size() - 1].t.push_back(stod(temp_t));
		ans[ans.size() - 1].hum.push_back(stod(temp_hum));
		ans[ans.size() - 1].wind_speed.push_back(stod(temp_wind));

		//修改储存值
		if (later_day != temp_day) res.push_back(DATA::data(temp[3] - '0', stod(temp_month), stod(temp_day), 1, stod(temp_cnt), temp[temp.length() - 7] - '0' + temp[temp.length() - 11] - '0', temp[temp.length() - 3] - '0'));
		else res[res.size() - 1].cnt += stod(temp_cnt), res[res.size() - 1].hours++;
		//记录小时数
		int dtime = stod(temp_hour);
		res[res.size() - 1].timenum[dtime] = 1;
		res[res.size() - 1].lines.push_back(line);

		//初始化相关值
		temp_t.clear(), temp_hum.clear(), temp_wind.clear();
		later_day = temp_day, later_hour = temp_hour, later_month = temp_month;
		temp_day.clear(), temp_hour.clear(), temp_month.clear(), temp_cnt.clear();
	}

	//将17年数据输入测试集
	while (1)
	{
		++line;
		int flag = 0;//,的数目

		temp_month += temp[5], temp_day += temp[8], temp_hour += temp[11];//计算月份天数小时
		temp_month += temp[6], temp_day += temp[9], temp_hour += temp[12];

		for (int i = 0; i < temp.length(); ++i)
		{
			if (temp[i] == ',')++flag;
			if (flag == 1 && temp[i] != ',') temp_cnt += temp[i];
			if (flag == 2 && temp[i] != ',')temp_t += temp[i];
			if (flag == 4 && temp[i] != ',')temp_hum += temp[i];
			if (flag == 5 && temp[i] != ',')temp_wind += temp[i];
			if (flag == 6)break;
		}

		//修改test
		if (later_day != temp_day) test.push_back(DATA::bp_data(stod(temp_month), stod(temp_day), temp[temp.length() - 7] - '0' + temp[temp.length() - 11] - '0', temp[temp.length() - 3] - '0'));
		test[test.size() - 1].cnt.push_back(stod(temp_cnt));
		test[test.size() - 1].t.push_back(stod(temp_t));
		test[test.size() - 1].hum.push_back(stod(temp_hum));
		test[test.size() - 1].wind_speed.push_back(stod(temp_wind));

		//初始化相关值
		temp_t.clear(), temp_hum.clear(), temp_wind.clear();
		later_day = temp_day, later_hour = temp_hour, later_month = temp_month;
		temp_day.clear(), temp_hour.clear(), temp_month.clear(), temp_cnt.clear();

		if (!getline(infile, temp)) break;
	}

	infile.close();
}

void data_correction::out_traverse(vector<DATA::data>& res)
{
	//输出每天的cnt
	cout << endl;
	for (int i = 0; i < res.size(); ++i)
		cout << "201" << res[i].year << '.' << res[i].month << '.' << res[i].day << ' ' << res[i].cnt << endl;
}

void data_correction::traverse(vector<DATA::data>& res)
{
	int day = 0;
	int daymax = 31;
	for (int i = 0; i < res.size(); ++i)
	{
		//判断月份最大天数
		switch (res[i].month)
		{
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			daymax = 31; break;
		case 4: case 6: case 9: case 11:
			daymax = 30; break;
		case 2:
			if (res[i].year == 5)daymax = 28;
			else daymax = 29;
			break;
		}

		++day;

		//检查天数
		while (day != res[i].day)
		{
			cout << "201" << res[i].year << '.' << res[i].month << '.' << day << " is missing. Line:" << res[i].lines[0] << endl;
			error_data.push_back(err(res[i].year, res[i].month, day++, 1));
		}
		if (day == daymax)day = 0;

		//检查小时数
		bool flag = 1;
		for (int j = 0, k = 0; j < 24; ++j)
			if (!res[i].timenum[j]) {
				cout << "201" << res[i].year << '.' << res[i].month << '.' << res[i].day << " lost " << j << " hour. Line: " << (k ? res[i].lines[k - 1] : res[i].lines[k]) << endl;
				if (flag) error_data.push_back(err(res[i].year, res[i].month, res[i].day, 0)), flag = 0;
				error_data[error_data.size() - 1].lost_time.push_back(j);
			}
			else ++k;
		int account = 0;
		for (int j = 0; j < 24; ++j)account += res[i].timenum[j];
		if (account != res[i].lines.size())cout << "201" << res[i].year << '.' << res[i].month << '.' << res[i].day << " time repetition" << endl;
	}
}

double data_correction::value_t(double t, int i)
{
	double value = t;
	for (int j = i - 1; j > 0; --j)
		value *= (t - j);
	return value;
}

double data_correction::newton()
{
	double result = 0, temp = 0;

	int len = hoo.size();
	double array[101][101];

	for (int i = 0; i < len; ++i) //下三角
		array[i][0] = hoo[i].y;

	for (int i = 0; i < len - 1; ++i) //下三角
	{
		for (int j = i + 1; j < len; ++j)
		{
			for (int k = i + 1; k < len; ++k)
				array[j][k] = array[j][k - 1] - array[j - 1][k - 1];
		}
	}

	/*for (int i = 0; i < len; ++i) //output;
	{
		for (int j = 0; j <= i; ++j)
			cout << array[i][j] << ' ';
		cout << endl;
	}
	cout << endl << endl << endl;*/

	double t = (num - hoo[0].x) / (hoo[1].x - hoo[0].x);

	result = array[0][0];
	for (int i = 1; i < len; ++i)
	{
		long long int help = 1;
		for (int j = 1; j <= i; ++j)
			help *= j;

		temp = array[i][i] * value_t(t, i) / help;
		result += temp;
	}

	return result;
}

int data_correction::find(int i, int t, vector<DATA::bp_data>& ans, vector<int> &u, vector<DATA::data> &res)
{
	//判断点i是否被使用过
	if (u.size())
	{
		for (int j = 0; j < u.size(); ++j)
		{
			if (u[j] == i) return find(i + 1, t, ans, u, res);
		}
	}
	//判断点i是否有相应的时间
	if (ans[i].cnt.size() > t &&res[i].timenum[t])
	{
		int account = 0;
		for (int v = 0; v < t + 1; ++v) account += res[i].timenum[v];
		return ans[i].cnt[account - 1];
	}
	else return find(i + 1, t, ans, u, res);
}

void data_correction::repair_day(vector<DATA::data>& res)
{
	for (int i = 4; i < error_data.size(); ++i)//前三个不做处理
	{
		if (error_data[i].is_correct) continue;
		hoo.clear();

		for (int j = 0; j < res.size(); ++j)
		{
			if (res[j].year != error_data[i].year)continue;

			if ((res[j].month == error_data[i].month) && (res[j].day == error_data[i].day))//先找到当天
			{
				//使用六个点进行牛顿插值法
				hoo.push_back(coo(1, res[j - 5].cnt));
				hoo.push_back(coo(3, res[j - 3].cnt));
				hoo.push_back(coo(5, res[j - 1].cnt));
				hoo.push_back(coo(7, res[j + 1].cnt));
				hoo.push_back(coo(9, res[j + 3].cnt));
				hoo.push_back(coo(11, res[j + 5].cnt));
				num = 6;

				res[j].cnt = newton();
				error_data[i].is_correct = 1;
				cout << "201" << res[j].year << '.' << res[j].month << "." << res[j].day << " " << res[j].cnt << endl;
				break;
			}

			//2016.9.2缺失的处理办法
			if ((res[j].month == error_data[i].month) && (res[j].day + 1 == error_data[i].day) && (res[j + 1].day != error_data[i].day))
			{
				//使用六个点进行牛顿插值法
				hoo.push_back(coo(1, res[j - 5].cnt));
				hoo.push_back(coo(3, res[j - 3].cnt));
				hoo.push_back(coo(5, res[j - 1].cnt));
				hoo.push_back(coo(7, res[j + 1].cnt));
				hoo.push_back(coo(9, res[j + 3].cnt));
				hoo.push_back(coo(11, res[j + 5].cnt));
				num = 6;

				int n_res = newton();
				res.push_back(DATA::data(6, 9, 2, 0, n_res, 0, 2));

				error_data[i].is_correct = 1;
				cout << "201" << res[j].year << '.' << res[j].month << "." << res[j].day + 1 << " " << res[j].cnt << endl;
				break;
			}
		}
	}
}

void data_correction::repair_time(vector<DATA::data>& res, vector<DATA::bp_data> &ans)
{
	vector<int> used;//标记被加入牛顿插值法的点

	//2015.1.7单独处理
	hoo.clear(), used.clear();
	int ttime = error_data[3].lost_time[0];
	//对六个点使用牛顿插值法
	hoo.push_back(coo(1, find(0, ttime, ans, used, res)));
	hoo.push_back(coo(3, find(2, ttime, ans, used, res)));
	hoo.push_back(coo(5, find(4, ttime, ans, used, res)));
	hoo.push_back(coo(7, find(6, ttime, ans, used, res)));
	hoo.push_back(coo(9, find(8, ttime, ans, used, res)));
	num = 4;//待求值所在点为4
	//调用牛顿插值法并输出结果
	int nn_res = newton();
	res[3].cnt += nn_res, error_data[3].is_correct = 1;
	cout << "201" << res[3].year << '.' << res[3].month << "." << res[3].day << " " << nn_res << endl;

	//前三天不在此处处理
	for (int i = 4; i < error_data.size(); ++i)
	{
		if (error_data[i].lost_time.size() > 6)//缺失小时数大于6直接按缺一天处理
		{
			error_data[i].is_day = 1;
			continue;
		}

		if (error_data[i].is_correct || error_data[i].is_day) continue;//跳过不需要处理的

		for (int j = 0; j < res.size(); ++j)
		{
			if (res[j].year != error_data[i].year)continue;

			if ((res[j].month == error_data[i].month) && (res[j].day == error_data[i].day))//在全部数据中找到该天
			{
				//把所有losttime都预测一次
				for (int v = 0; v < error_data[i].lost_time.size(); ++v)
				{
					hoo.clear(), used.clear();
					int time = error_data[i].lost_time[v];

					//对六个点使用牛顿插值法
					hoo.push_back(coo(1, find(j - 5, time, ans, used, res)));
					hoo.push_back(coo(3, find(j - 3, time, ans, used, res)));
					hoo.push_back(coo(5, find(j - 1, time, ans, used, res)));
					hoo.push_back(coo(7, find(j + 1, time, ans, used, res)));
					hoo.push_back(coo(9, find(j + 3, time, ans, used, res)));
					hoo.push_back(coo(11, find(j + 5, time, ans, used, res)));
					num = 6;//待求值所在点为6

					//调用牛顿插值法并输出结果
					int n_res = newton();
					res[j].cnt += n_res;
					cout << "201" << res[j].year << '.' << res[j].month << "." << res[j].day << " " << n_res << endl;
				}

				error_data[i].is_correct = 1;
				break;
			}
		}
	}
}
