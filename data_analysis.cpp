#include "std_lib_facilities.h"
#include "data_correction.h"
#include "data_forecasting.h"
#include "data_visualization.h"
#include "Graph_lib.h"

//��Ҫ��Ϊ�˰�ť��Ʒ���
data_correction solution;//�������ݶ�ȡ������������һ����
data_draw d_solution;//���ڿ��ӻ���������
bp_forecasting fore;//����Ԥ�⣬������
bp net;//����ѵ��ģ�ͣ�������
//��������
vector<DATA::data> solu;//�����������
vector<DATA::bp_data> bp_d;//ѵ����
vector<DATA::bp_data> bp_t;//���Լ�
vector<double> w_array;//ģ��Ȩ����
bool flag_dirty = 0;
bool flag_repair = 0;

int main()
{
	solution.assignment(solu, bp_d, bp_t);//��ȡ����
	d_solution.start_visualization();//����������

	//double m, d, tt, v, s, t, h, w;
	return 0;
}