#pragma once


#include<iostream>
#include<string>

using namespace std;



struct rtuple //路由表元组类
{
	char destn[20];//目的网络
	int distance;//距离
	int next;//下一跳
	rtuple* link;//表中下一项
	rtuple() { link = NULL; }
	rtuple(char* de,int d,int n,rtuple*l=NULL)
		:distance(d), link(l),next(n) {
		strncpy_s(destn, de,20);
	}
};




class r_table //路由表类
{
public:
	r_table() { last = NULL; currentsize = 0; first = new rtuple;
	}//构造函数
	r_table(int id) { identi = id; first = new rtuple; last = NULL; currentsize = 0; }//构造函数
	~r_table() ;//析构函数
	rtuple* getfirst() { return first; }//取第一项
	int* get_rid() { return &identi; }//取路由表编号存储地址
	void size_add() { currentsize++; }
	int getsize() { return currentsize; }
private:
	int identi;//路由器编号
	rtuple* first;//第一项
	rtuple* last;//最后一项
	int currentsize;
	rtuple* search(char* d) { rtuple* p = first;
	while (p != NULL) { if (!strcmp(d, p->destn))return p; p = p->link; }
	 return p; }
	friend ostream& operator<<(ostream& out, r_table& r);//输出路由表
};

ostream& operator<<(ostream& out, r_table& r)
{
	rtuple* p = r.first;
	out << "路由器：" << r.identi << endl;
	out << "当前路由表长度：" << r.currentsize << endl;

	out << "\t目的网络\t距离\t下一跳\n";
	for (int i = 0; i < r.currentsize; i++)
	{
		out <<"\t"<< p->destn << "       \t" << p->distance << "\t" << p->next << endl;
		p = p->link;
	}return out;
}




r_table::~r_table()
{
	rtuple* p = first,*p2;
	while (p != NULL)
	{
		p2 = p; p = p->link; delete p2;
	}
}

struct edge//边
{
	int router2;//另一顶点位置
	int distance;//距离
	edge* link;//下一条边链指针
	edge() { link = NULL; }
	~edge(){}
};


struct router//路由器（顶点）
{
	int identi;//路由器编号
	char net[20];//网络号
	r_table* table;//路由表网
	edge* ed;//边链表头指针
	router() { ed = NULL; table = new r_table; }
	router(int i, char* n) :identi(i) { strncpy_s(net, n, 20); ed = NULL; table = new r_table; }
	void apply() { table = new r_table; }//申请一个新的表
	friend ostream& operator<<(ostream& out, router& r);//输出路由器以及它的边的信息
};

ostream& operator<<(ostream& out, router& r)
{
	edge* p = r.ed;
	cout << "路由器编号：" << r.identi << endl;
	cout << "网络号：" << r.net << endl;
	cout << "目的路由器：\t距离：\n";
	while (p != NULL)
	{
		out << p->router2 << "\t" << p->distance << endl;
		p = p->link;
	}
	return out;
}


