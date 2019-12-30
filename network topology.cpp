// network topology.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//该方法



#include <iostream>
#include<fstream>
#include<stdlib.h>
#include"Graph.h"
#include"network topology definition.h"
ifstream fin;
Graphlnk r_graph;

using namespace std;

void g_build();//创建网络拓扑图
void f_table();//查询路由表
void g_update();//更新网络拓扑图
void del_edge();//删除边
void del_rou();//删除顶点
int main()
{
	char option; int circle = 1;
	g_build();
	while (circle)
	{
		cout << "请选择您想进行的操作：\n";
		cout << "1.查询路由表\n2.更新网络拓扑图\n3.退出系统\n";
		cin >> option;
		switch (option)
		{
		case '1':f_table(); break;
		case'2':g_update(); break;
		case'3':circle = 0; break;
		default:
			break;
		}
		cout << "输入任意字符继续：\n";
		fflush(stdin);
		cin.get(); 
		cin.get();
		system("cls");
	}
	return 0;
}

void g_build()
{
	router R; int r1, r2,d,n;
	fin.open("router.txt");//导入顶点信息
	if (!fin.is_open()) { cout << "顶点文件打开失败！退出程序！\n"; exit(EXIT_FAILURE); }
	while (fin >> n) { if (n == 0)break; R.identi = n; fin >> R.net; r_graph.insertGraph(R); }
	//导入边信息
	while (fin >> r1) {
		fin >> r2; fin >> d; r_graph.insertEdge(r1-1, r2-1, d, 0); 
	}
	fin.close(); 
	r_graph.t_rebuild();
}

void f_table()
{
	int id, locate; r_table* t;
	r_graph.n_output();
	cout << "请输入路由器的编号：\n";
	cin >> id;
	t = r_graph.get_t(id); if (t == NULL)return;
	cout << *t->get_rid()<<endl;
	cout << *t;
}

void g_update()//更新拓扑图
{
	r_graph.outputGraph();
	char option;
	cout << "请选择您想进行的操作：\n";
	cout << "1.删除边\n2.插入路由器、边\n3.删除顶点\n其他：返回主菜单\n";
	cin >> option;
	switch (option)
	{
	case'1':del_edge(); break;
	case'2':r_graph.inputGraph(); break;
	case'3':del_rou(); break;
	default:return;
		break;
	}

}

void del_edge()//删除边
{
	int id, n, id2,i,j; router* p; edge* p2;
	cout << "删除哪个路由器的边？\n"; cin >> id;
	n = r_graph.getRouterPos(id); p = r_graph.get_r(n); p2 = p->ed; 
	cout << "路由器编号：" << p->identi << endl;
	cout << "网络号：" << p->net << endl;
	cout << "目的路由器：\t距离：\n";
	while (p2 != NULL)
	{
		cout << r_graph.get_r(p2->router2)->identi << "\t" << p2->distance << endl;
		p2 = p2->link;
	}p2 = p->ed;
	cout << "删除的边到达哪个路由器？\n";
	cin >> id2;
	i = r_graph.getRouterPos(id); j = r_graph.getRouterPos(id2);
	while (p2 != NULL) { if (r_graph.get_r( p2->router2)->identi == id2)break; p2 = p2->link; }
	if (p2 != NULL) if (r_graph.removeEdge(i,j))cout << "删除成功！\n"; else cout << "删除失败！\n";
	else cout << "没有到达该路由器的边！\n";
}

void del_rou()//删除路由器顶点
{
	int id,n;
	r_graph.n_output();
	cout << "删除哪个路由器？\n";
	cin >> id;
	n = r_graph.getRouterPos(id);
	if (r_graph.removeRouter(n))cout << "删除成功！\n";
	else cout << "删除失败！\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
