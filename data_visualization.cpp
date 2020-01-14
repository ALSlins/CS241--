//一个生成折线图函数，一个生成柱状图函数，后续就不需要再区分折线和柱状了
//一天中cnt随时间变化折线图
//一年中cnt随月份变化柱状图
//cnt随温度、体感温度变化
//cnt随季度、天气、节假日、风速变化
#include "data_visualization.h"

void data_draw::start_visualization()
{
	using namespace Graph_lib;
	Point t(100, 100);
	Simple_window win_vis(t, 980, 560, "Start Visualization");

	//加载一张欢迎图片
	Image welcome(Point(260, 45), "welcome.jpg");
	win_vis.attach(welcome);

	//六个按钮
	vis help;
	Button A(Point(50, 45), 160, 30, "find dirty data", help.cb_dirty_data);
	Button B(Point(50, 125), 160, 30, "handle dirty data", help.cb_repair_data);
	Button C(Point(50, 215), 160, 30, "data visualization", help.cb_vis_data);
	Button D(Point(50, 305), 160, 30, "training model", help.cb_train_data);
	Button E(Point(50, 395), 160, 30, "test model", help.cb_test_data);
	Button F(Point(50, 485), 160, 30, "output the data", help.cb_out_data);

	win_vis.attach(A), win_vis.attach(B), win_vis.attach(C), win_vis.attach(D), win_vis.attach(E), win_vis.attach(F);

	Fl::redraw();
	win_vis.wait_for_button();
}

void data_draw::a_data_draw(vector<DATA::data>& res)
{
	//避免重复调用导致数组越界
	important_data.day.clear();
	important_data.day_2015.clear();
	important_data.day_2016.clear();
	important_data.month_2015.clear();
	important_data.month_2016.clear();
	important_data.season_2015.clear();
	important_data.season_2016.clear();

	//初始化相应数组
	for (int i = 0; i < 4; ++i)
	{
		important_data.season_2015.push_back(0);
		important_data.season_2016.push_back(0);
	}
	for (int i = 0; i < 12; ++i)
	{
		important_data.month_2015.push_back(0);
		important_data.month_2016.push_back(0);
	}

	//储存数值
	for (int i = 0; i < res.size(); ++i)
	{
		//month
		if (res[i].month <= 0 || res[i].month > 13 || res[i].season < 0 || res[i].season > 4) cout << res[i].month << ' ' << res[i].day << endl;
		if (res[i].year == 5) important_data.month_2015[res[i].month - 1] += res[i].cnt;
		else important_data.month_2016[res[i].month - 1] += res[i].cnt;

		//day
		if (res[i].year == 5) important_data.day_2015.push_back(res[i].cnt);
		else important_data.day_2016.push_back(res[i].cnt);

		//season
		if (res[i].year == 5) important_data.season_2015[res[i].season] += res[i].cnt;
		else important_data.season_2016[res[i].season] += res[i].cnt;
	}

	//放缩
	for (int i = 0; i < 12; ++i)
	{
		important_data.month_2015[i] = important_data.month_2015[i] / 10000;
		important_data.month_2016[i] = important_data.month_2016[i] / 10000;
	}
	int temp_2015, temp_2016;
	for (int i = 0; i < 4; ++i)
	{
		important_data.season_2015[i] = important_data.season_2015[i] / 100000;
		important_data.season_2016[i] = important_data.season_2016[i] / 100000;
	}
	for (int i = 0; i < important_data.day_2015.size(); ++i) important_data.day_2015[i] = important_data.day_2015[i] / 1000;
	for (int i = 0; i < important_data.day_2016.size(); ++i) important_data.day_2016[i] = important_data.day_2016[i] / 1000;

	//两年
	for (int i = 0; i < important_data.day_2015.size(); ++i)important_data.day.push_back(important_data.day_2015[i]);
	for (int i = 0; i < important_data.day_2016.size(); ++i)important_data.day.push_back(important_data.day_2016[i]);
}

void data_draw::visualization()
{
	using namespace Graph_lib;
	Point t(100, 100);
	Simple_window win_vis(t, 980, 560, "Data Visualization");

	//加载一张欢迎图片
	Image welcome(Point(260, 45), "welcome_vis.jpg");
	win_vis.attach(welcome);

	//六个按钮
	vis help;
	Button A(Point(50, 45), 160, 30, "all day line chart", help.cb_line_chart_1);
	Button B(Point(50, 125), 160, 30, "year line chart", help.cb_line_chart_2);
	Button C(Point(50, 215), 160, 30, "month line chart", help.cb_line_chart_month);
	Button D(Point(50, 305), 160, 30, "season line chart", help.cb_line_chart_season);
	Button E(Point(50, 395), 160, 30, "month bar chart", help.cb_bar_chart_month);
	Button F(Point(50, 485), 160, 30, "season bar chart", help.cb_bar_chart_season);

	win_vis.attach(A), win_vis.attach(B), win_vis.attach(C), win_vis.attach(D), win_vis.attach(E), win_vis.attach(F);

	Fl::redraw();
	win_vis.wait_for_button();
}

void data_draw::line_chart(vector<int> &res_a, vector<int> &res_b)
{
	int max = 0;
	for (int i = 0; i < res_a.size(); ++i) {
		if (max < res_a[i])max = res_a[i];
		if (max < res_b[i])max = res_b[i];
	}
	const int xmax = 1000;//长
	const int ymax = 800;//宽
	const int xoffset = 80;//边缘距离
	const int yoffset = 50;//边缘距离
	const int xlength = xmax - 2 * xoffset;
	const int ylength = ymax - 2 * yoffset;
	const int x_scale = xlength / (res_a.size() + 1);
	const double y_scale = ylength / max;
	const double deltx = 20;
	const double delty = 20;
	double y_height = ymax - yoffset;//x轴高度

	using namespace Graph_lib;
	Point t(100, 100);
	Simple_window win_line(t, 1000, 800, "Visualization of Line Chart");

	//设定x与y轴
	Axis x(Axis::x, Point(xoffset, y_height), xlength, res_a.size() == 4 ? 5 : 13, res_a.size() == 4 ? "season" : (res_a.size() == 12 ? "month" : "time"));
	x.label.move(620, 0);
	Axis y(Axis::y, Point(xoffset, y_height), ylength + 20, 40, res_a.size() == 4 ? "bicycle/x10^5" : (res_a.size() == 12 ? "bicycle/x10^4" : "bicycle/x10"));
	y.label.move(-60, -5);
	x.set_color(Color::black);
	y.set_color(Color::black);
	win_line.attach(x);
	win_line.attach(y);

	//计算折线坐标
	Lines a, b;
	vector<double> x_axis, ya_axis, yb_axis;
	for (int i = 1; i < 1 + res_a.size(); ++i)
	{
		x_axis.push_back(i*x_scale + xoffset);
		ya_axis.push_back(y_height - y_scale * res_a[i - 1]);
		yb_axis.push_back(y_height - y_scale * res_b[i - 1]);
		if (i != 1) {
			a.add(Point(x_axis[i - 2], ya_axis[i - 2]), Point(x_axis[i - 1], ya_axis[i - 1]));
			b.add(Point(x_axis[i - 2], yb_axis[i - 2]), Point(x_axis[i - 1], yb_axis[i - 1]));
		}
	}
	a.set_color(Color::dark_blue);
	b.set_color(Color::dark_green);
	win_line.attach(a);
	win_line.attach(b);

	//绘制横坐标
	Vector_ref<Graph_lib::Text> x_txt;//横坐标
	vector<string> x_num = { " 1"," 2"," 3"," 4"," 5"," 6"," 7"," 8"," 9","10","11","12" };//横坐标
	vector<string> x__num = { "spring","summer","fall","winter" };
	for (int i = 0; i < (res_a.size() == 12 ? 12 : 4); ++i)
	{
		x_txt.push_back(new Text(Point(x_axis[i] - 10, y_height + delty), res_a.size() == 12 ? x_num[i] : x__num[i]));//调整横坐标位置
		x_txt[i].set_color(Color::black);
		if (res_a.size() < 13) win_line.attach(x_txt[i]);
	}

	//绘制纵坐标
	int dmax = max / 7;
	vector<string> y_num = { " 0",to_string(dmax),to_string(2 * dmax),to_string(3 * dmax),to_string(4 * dmax),to_string(5 * dmax),to_string(6 * dmax),to_string(7 * dmax) };
	Vector_ref<Graph_lib::Text> y_txt;//纵坐标
	for (int i = 0; i < 8; ++i)
	{
		y_txt.push_back(new Text(Point(xoffset - 30, y_height - y_scale * dmax*i/*5 * i * ylength / 35*/ + 8), y_num[i]));//调整纵坐标位置
		y_txt[i].set_color(Color::black);
		win_line.attach(y_txt[i]);
	}

	//绘制图例,常数用于调整位置
	Line aa(Point(784, 50), Point(784 + 50, 50));
	Line bb(Point(784, 50 + deltx + 10), Point(784 + 50, 50 + deltx + 10));
	aa.set_color(Color::dark_blue), bb.set_color(Color::dark_green);
	win_line.attach(aa), win_line.attach(bb);
	Graph_lib::Text c(Point(784 + 60, 50 + 5), res_a.size() < 13 ? "2015" : "forecast");
	Graph_lib::Text d(Point(784 + 60, 50 + deltx + 15), res_a.size() < 13 ? "2016" : "reality");
	c.set_color(Color::black), d.set_color(Color::black);
	win_line.attach(c), win_line.attach(d);

	Fl::redraw();
	win_line.wait_for_button();
}

void data_draw::line_chart(int op)
{
	int max = 0;
	for (int i = 0; i < important_data.day.size(); ++i) {
		if (max < important_data.day[i])max = important_data.day[i];
	}
	const int xmax = 1000;//长
	const int ymax = 800;//宽
	const int xoffset = 80;//边缘距离
	const int yoffset = 50;//边缘距离
	const int xlength = xmax - 2 * xoffset;
	const int ylength = ymax - 2 * yoffset;
	const int x_scale = op;
	const double y_scale = ylength / max;
	const double deltx = 20;
	const double delty = 20;
	double y_height = ymax - yoffset;//x轴高度

	using namespace Graph_lib;
	Point t(100, 100);
	Simple_window win_line(t, 1000, 800, "Visualization of Line Chart");

	//设定x与y轴
	Axis x(Axis::x, Point(xoffset, y_height), op == 2 ? xlength - 20 : xlength + 20, 11, "day");
	x.label.move(620, 0);
	Axis y(Axis::y, Point(xoffset, y_height), ylength - 20, 40, "bicycle/x10^3");
	y.label.move(-60, -5);
	x.set_color(Color::black);
	y.set_color(Color::black);
	win_line.attach(x);
	win_line.attach(y);

	//计算折线坐标
	Lines a, b;
	vector<double> x_axis, ya_axis, yb_axis;
	for (int i = 1; i < 1 + (op > 1 ? important_data.day_2015.size() : important_data.day.size()); ++i)
	{
		x_axis.push_back(i*x_scale + xoffset);
		ya_axis.push_back(y_height - y_scale * important_data.day[i - 1]);
		if (op > 1)yb_axis.push_back(y_height - y_scale * important_data.day_2016[i - 1]);
		if (i != 1) {
			a.add(Point(x_axis[i - 2], ya_axis[i - 2]), Point(x_axis[i - 1], ya_axis[i - 1]));
			if (op > 1)b.add(Point(x_axis[i - 2], yb_axis[i - 2]), Point(x_axis[i - 1], yb_axis[i - 1]));
		}
	}
	a.set_color(Color::dark_blue);
	b.set_color(Color::dark_green);
	win_line.attach(a);
	if (op > 1)win_line.attach(b);

	//绘制横坐标
	Vector_ref<Graph_lib::Text> x_txt;//横坐标
	vector<string> x_num = { " 80","160","240","320","400","480","560","640","720","800" };//横坐标
	vector<string> x__num = { " 37"," 74","111","148","185","222","259","296","333","370" };
	for (int i = 0; i < 10; ++i)
	{
		if (op == 1)x_txt.push_back(new Text(Point(xoffset + (i + 1)*(xlength + 20) / 11 - 10, y_height + delty), x_num[i]));//调整横坐标位置
		else x_txt.push_back(new Text(Point(xoffset + (i + 1)*(xlength - 20) / 11 - 10, y_height + delty), x__num[i]));

		x_txt[i].set_color(Color::black);
		win_line.attach(x_txt[i]);
	}

	//绘制纵坐标
	int dmax = max / 7;
	vector<string> y_num = { " 0",to_string(dmax),to_string(2 * dmax),to_string(3 * dmax),to_string(4 * dmax),to_string(5 * dmax),to_string(6 * dmax),to_string(7 * dmax) };
	Vector_ref<Graph_lib::Text> y_txt;//纵坐标
	for (int i = 0; i < 8; ++i)
	{
		y_txt.push_back(new Text(Point(xoffset - 30, y_height - y_scale * i*dmax + 8), y_num[i]));//调整纵坐标位置
		y_txt[i].set_color(Color::black);
		win_line.attach(y_txt[i]);
	}

	//绘制图例,常数用于调整位置
	Line aa(Point(784, 50), Point(784 + 50, 50));
	Line bb(Point(784, 50 + deltx + 10), Point(784 + 50, 50 + deltx + 10));
	aa.set_color(Color::dark_blue), bb.set_color(Color::dark_green);
	win_line.attach(aa); if (op > 1)win_line.attach(bb);
	Graph_lib::Text c(Point(784 + 60, 50 + 5), op > 1 ? "2015" : "2015 && 2016");
	Graph_lib::Text d(Point(784 + 60, 50 + deltx + 15), "2016");
	c.set_color(Color::black), d.set_color(Color::black);
	win_line.attach(c); if (op > 1)win_line.attach(d);

	Fl::redraw();
	win_line.wait_for_button();
}

void data_draw::bar_chart(vector<int>& res_a, vector<int>& res_b)
{
	int max = 0;
	for (int i = 0; i < res_a.size(); ++i) {
		if (max < res_a[i])max = res_a[i];
		if (max < res_b[i])max = res_b[i];
	}
	const int xmax = 1000;//长
	const int ymax = 800;//宽
	const int xoffset = 80;//边缘距离
	const int yoffset = 50;//边缘距离
	const int xlength = xmax - 2 * xoffset;
	const int ylength = ymax - 2 * yoffset;
	const int x_scale = xlength / (res_a.size() + 1);
	const double y_scale = ylength / max;
	const double delty = 20;
	double y_height = ymax - yoffset;//x轴高度
	double deltx = 20;
	if (res_a.size() == 4) deltx = 60;

	using namespace Graph_lib;
	Point t(100, 100);
	Simple_window win_bar(t, 1000, 800, "Visualization of Bar Chart");

	//设定x与y轴
	Axis x(Axis::x, Point(xoffset, y_height), xlength, res_a.size() == 4 ? 5 : 13, res_a.size() == 4 ? "season" : "month");
	x.label.move(620, 0);
	Axis y(Axis::y, Point(xoffset, y_height), ylength + 20, 35, res_a.size() == 4 ? "bicycle/x10^5" : "bicycle/x10^4");
	y.label.move(-60, -5);
	x.set_color(Color::black);
	y.set_color(Color::black);
	win_bar.attach(x);
	win_bar.attach(y);

	//计算矩形坐标
	vector<double> x_axis, y_axis;
	for (int i = 1; i < 1 + res_a.size(); ++i)
	{
		x_axis.push_back(i*x_scale + xoffset);
		y_axis.push_back(y_height - y_scale * res_a[i - 1]);
		y_axis.push_back(y_height - y_scale * res_b[i - 1]);
	}

	//建立矩形以及绘制横坐标
	Vector_ref<Graph_lib::Rectangle> rec;//矩形
	Vector_ref<Graph_lib::Text> x_txt;//横坐标
	vector<string> x_num = { " 1"," 2"," 3"," 4"," 5"," 6"," 7"," 8"," 9","10","11","12" };//横坐标
	vector<string> x__num = { "spring","summer","fall","winter" };
	for (int i = 0; i < res_a.size(); ++i)
	{
		//2015
		rec.push_back(new Graph_lib::Rectangle(Point(x_axis[i] - deltx, y_axis[2 * i]), Point(x_axis[i], y_height)));
		rec[2 * i].set_fill_color(Color::dark_blue);
		rec[2 * i].set_color(Color::black);
		win_bar.attach(rec[2 * i]);
		//2016
		rec.push_back(new Graph_lib::Rectangle(Point(x_axis[i], y_axis[2 * i + 1]), Point(x_axis[i] + deltx, y_height)));
		rec[2 * i + 1].set_fill_color(Color::dark_green);
		rec[2 * i + 1].set_color(Color::black);
		win_bar.attach(rec[2 * i + 1]);
		//横坐标数值显示
		x_txt.push_back(new Text(Point(x_axis[i] - 10, y_height + delty), x_num[i]));//调整横坐标位置
		x_txt[i].set_color(Color::black);
		win_bar.attach(x_txt[i]);
	}

	//绘制纵坐标
	int dmax = max / 7;
	vector<string> y_num = { " 0",to_string(dmax),to_string(2 * dmax),to_string(3 * dmax),to_string(4 * dmax),to_string(5 * dmax),to_string(6 * dmax),to_string(7 * dmax) };
	Vector_ref<Graph_lib::Text> y_txt;//纵坐标
	for (int i = 0; i < 8; ++i)
	{
		y_txt.push_back(new Text(Point(xoffset - 30, y_height - 5 * i * ylength / 35 + 8), y_num[i]));//调整纵坐标位置
		y_txt[i].set_color(Color::black);
		win_bar.attach(y_txt[i]);
	}

	//绘制图例,常数用于调整位置
	Graph_lib::Rectangle a(Point(784, 50), Point(784 + 50, 50 + deltx));
	Graph_lib::Rectangle b(Point(784, 50 + deltx + 10), Point(784 + 50, 50 + 2 * deltx + 10));
	a.set_color(Color::black), b.set_color(Color::black);
	a.set_fill_color(Color::dark_blue), b.set_fill_color(Color::dark_green);
	win_bar.attach(a), win_bar.attach(b);
	Graph_lib::Text c(Point(784 + 60, 50 + 17), "2015");
	Graph_lib::Text d(Point(784 + 60, 50 + deltx + 10 + 17), "2016");
	c.set_color(Color::black), d.set_color(Color::black);
	Graph_lib::Text e(Point(784 + 60, 50 + 35), "2015");
	Graph_lib::Text f(Point(784 + 60, 50 + deltx + 10 + 35), "2016");
	e.set_color(Color::black), f.set_color(Color::black);
	if (res_a.size() == 12)win_bar.attach(c), win_bar.attach(d);
	else win_bar.attach(e), win_bar.attach(f);

	Fl::redraw();
	win_bar.wait_for_button();
}