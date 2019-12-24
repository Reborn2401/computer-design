#ifndef Graph_h
#define Graph_h
#include <iostream>
#include"network topology definition.h"
using namespace std;

const int DefaultVertices = 30;


class Graphlnk {
	friend class r_table;
public:
	const int maxDistance = 100000; // 代表无穷大的值（=∞）
	Graphlnk(int sz = DefaultVertices); // 构造函数
	~Graphlnk(); // 析构函数
	void inputGraph(); // 建立邻接表表示的图
	void outputGraph(); // 输出图中的所有顶点和边信息
	void insertGraph(router& r);//插入一个路由器顶点
	int getIdenti(int i); // 取位置为i的顶点中的值
	int getDistance(int v1, int v2); // 返回边（v1， v2）上的权值
	bool insertRouter(const int& vertex); // 插入顶点
	bool insertEdge(int v1, int v2, int weight,int com); // 插入边
	bool removeRouter(int v); // 删除顶点
	bool removeEdge(int v1, int v2); // 删除边
	int getFirstNeighbor(int v); // 取顶点v的第一个邻接顶点
	int getNextNeighbor(int v, int w); // 取顶点v的邻接顶点w的下一邻接顶点
	int getRouterPos(const int vertex); // 给出顶点vertex在图中的位置
	int numberOfRouters(); // 当前顶点数
	void br_table(int i);//创建位置为i的路由器的路由表
	router* get_r(int i) { return &nodeTable[i]; }//给出位置为i的路由器顶点的地址
	void t_rebuild();//重建所有路由表
	r_table* get_t(int id) {
		for (int i = 0; i < numRouters; i++)if (nodeTable[i].identi == id) return nodeTable[i].table;
		return NULL;
	}
	//返回指定路由器的路由表
	void n_output() {//输出顶点的基本信息
		cout << "路由器编号\t网络号\t\n";
		for (int i = 0; i < numRouters; i++)cout << nodeTable[i].identi << ": " << nodeTable[i].net << endl;;
	}
private:
	int maxVertices; // 图中最大的顶点数
	int numEdges; // 当前边数
	int numRouters; // 当前顶点数
	router* nodeTable; // 顶点表(各边链表的头结点)
	
};

// 构造函数:建立一个空的邻接表
Graphlnk::Graphlnk(int sz) {
	maxVertices = sz;
	numRouters = 0;
	numEdges = 0;
	nodeTable = new router[maxVertices]; // 创建顶点表数组
	if (nodeTable == NULL) {
		cerr << "存储空间分配错误！" << endl;
		exit(1);
	}
	for (int i = 0; i < maxVertices; i++)
		nodeTable[i].ed= NULL;
}

// 析构函数
Graphlnk::~Graphlnk() {
	// 删除各边链表中的结点
	for (int i = 0; i < numRouters; i++) {
		edge* p = nodeTable[i].ed; // 找到其对应链表的首结点
		while (p != NULL) { // 不断地删除第一个结点
			nodeTable[i].ed = p->link;
			p->~edge();
			p = nodeTable[i].ed ;
			;
		}
	}
	delete[]nodeTable; // 删除顶点表数组
}

// 建立邻接表表示的图
void Graphlnk::inputGraph() {
	int n, m; // 存储顶点树和边数
	char net[20];
	int i, j, k;
	int e1, e2; // 顶点
	int weight; // 边的权值

	cout << "请输入顶点数和边数：" << endl;
	cin >> n >> m;
	cout << "请输入各顶点：(路由器编号及其网络号)" << endl;
	for (i = 0; i < n; i++) {
		cin >> e1; cin >> net;
		if(insertRouter(e1))strncpy_s(nodeTable[numRouters-1].net,net,20); // 插入顶点
	}

	cout << "请输入图的各边的信息：(边1，边2，权值)" << endl;
	i = 0;
	while (i < m) {
		cin >> e1 >> e2 >> weight;
		j = getRouterPos(e1);
		k = getRouterPos(e2);
		if (j == -1 || k == -1)
			cout << "边两端点信息有误，请重新输入！" << endl;
		else {
			insertEdge(j, k, weight,0); // 插入边
			insertEdge(k, j, weight,0);
			i++;
		}
	} // while
	t_rebuild();
}

void Graphlnk::br_table(int v)
{
	int i, min, j, k, w, h;  router* r = &nodeTable[v];
	rtuple* p1; p1 = r->table->getfirst();
	int S[DefaultVertices], distance[DefaultVertices], path[DefaultVertices];
	int* c = r->table->get_rid(); *c = nodeTable[v].identi;//给路由表所属路由器赋值
	for (i = 0; i < numRouters; i++)
	{
		S[i] = 0;
		distance[i] = getDistance(v,i);
		if (i != v && distance[i] < maxDistance)path[i] = v;
		else path[i] = -1;
	}
	S[v] = 1; distance[v] = 0;
	for (i = 0; i < numRouters - 1; i++)
	{
		min = maxDistance; int u = v;
		for (j = 0; j < numRouters; j++)
		{
			if (S[j] == 0 && distance[j] < min) { u = j; min = distance[j]; }
		}
		S[u] = 1;
		for (k = 0; k < numRouters; k++)
		{
			w = getDistance(u, k);
			if (S[k] == 0 && w < maxDistance && distance[u] + w < distance[k])
			{
				distance[k] = distance[u] + w;
				path[k] = u;
			}
		}
	}
	for (i = 0; i < numRouters; i++)
	{
		strncpy_s(p1->destn, nodeTable[i].net, 20);
		p1->distance = distance[i]; j = i;
		do
		{
			k = j;
			j = path[k];
		} while (j != v && j >= 0);
		if (i != v)p1->next = nodeTable[k].identi
			; else p1->next = -1;
		if (i < numRouters - 1)
		{
			p1->link = new rtuple; p1 = p1->link;
		} r->table->size_add();

	}
	p1->link = NULL;
}

void Graphlnk::insertGraph(router& r)
{
	if (numRouters == maxVertices) { cout << "图已满！插入失败！\n"; return; }
	edge* p = r.ed;
	nodeTable[numRouters] = r; numRouters++;
	while (p != NULL)numEdges++;
}

// 输出图中的所有顶点和边信息
void Graphlnk::outputGraph() {
	int n, m, i, j;
	int e1, e2; // 顶点
	int weight; // 权值

	n = numRouters;
	m = numEdges;
	cout << "图中的顶点数为" << n << ",边数为" << m << endl;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			weight = getDistance(i, j); // 取边的权值
			if (weight > 0 && weight < maxDistance) { // 有效
				e1 = getIdenti(i); // 顶点
				e2 = getIdenti(j);
				cout << "(" << e1 << "," << e2 << "," << weight << ")" << endl;
			}
		} // 内循环for
	} // 外循环for
}

// 取位置为i的顶点中的值
int Graphlnk::getIdenti(int i) {
	if (i >= 0 && i < numRouters)
		return nodeTable[i].identi;
	return NULL;
}

// 返回边（v1， v2）上的权值
int Graphlnk::getDistance(int v1, int v2) {
	if (v1 != -1 && v2 != -1) {
		edge* p = nodeTable[v1].ed; // v1的第一条关联的边
		while (p != NULL && p->router2 != v2) { // 寻找邻接顶点v2
			p = p->link;
		}
		if (p != NULL)
			return p->distance;
	}
	return maxDistance; // 边(v1, v2)不存在
}

// 插入顶点
bool Graphlnk::insertRouter(const int & vertex) {
	if (numRouters == maxVertices) // 顶点表满，不能插入
		return false;
	nodeTable[numRouters].identi = vertex; // 插入在表的最后
	numRouters++;
	return true;
}

// 插入边
bool Graphlnk::insertEdge(int v1, int v2, int weight,int com) {
	if (v1 >= 0 && v1 < numRouters && v2 >= 0 && v2 < numRouters) {
		edge* q, * p = nodeTable[v1].ed; // v1对应的边链表头指针
		while (p != NULL && p->router2 != v2) // 寻找邻接顶点v2
		{
			 p = p->link;
		}
		if (p != NULL) // 已存在该边，不插入
			return false;
		p = new edge; // 创建新结点
		p->router2 = v2;
		p->distance = weight;
		p->link = nodeTable[v1].ed; // 链入v1边链表
		nodeTable[v1].ed = p;
		numEdges++;
		q = new edge;
		q->router2 = v1;
		q->distance = weight;
		q->link = nodeTable[v2].ed; // 链入v2边链表
		nodeTable[v2].ed = q;
		numEdges++;
		if(com==1)
		t_rebuild();
		return true;
	}
	return false;
}

// 删除顶点
bool Graphlnk::removeRouter(int v) {
	if (numRouters == 1 || v < 0 || v > numRouters)
		return false; // 表空或顶点号超出范围

	edge * p, *s, *t;
	int k; // 存储邻接顶点
	while (nodeTable[v].ed != NULL) {
		p = nodeTable[v].ed;
		k = p->router2; // 取邻接顶点k
		s = nodeTable[k].ed; // 找对称存放的边结点
		t = NULL;
		while (s != NULL && s->router2 != v) {
			t = s;
			s = s->link;
		}
		if (s != NULL) { // 删除对称存放的边结点
			if (t == NULL) // 删除的是第一个邻接顶点
				nodeTable[k].ed = s->link;
			else
				t->link = s->link;
			delete s;
		}
		nodeTable[v].ed = p->link; // 清除顶点v的边链表结点
		delete p;
		numEdges--; // 与顶点v相关联的边数减1
	} // while结束
	numRouters--; // 图的顶点个数减1
	nodeTable[v] = nodeTable[numRouters]; // 填补
	p = nodeTable[v].ed = nodeTable[numRouters].ed;
	// 要将填补的顶点对应的位置改写
	while (p != NULL) {
		s = nodeTable[p->router2].ed; // 对称边链表结点
		while (s != NULL) {
			if (s->router2 == numRouters) { // 找到对称边
				s->router2 = v; // 修改指向v
				break;
			}
			s = s->link;
		}
		p = p->link; // 指向下一个邻接顶点
	}
	t_rebuild();
	return true;
}

// 删除边
bool Graphlnk::removeEdge(int v1, int v2) {
	if (v1 != -1 && v2 != -1) {
		edge* p = nodeTable[v1].ed, * q = NULL, * s = p;
		while (p != NULL && p->router2 != v2) { // v1对应边链表中找被删除边
			q = p;
			p = p->link;
		}
		if (p != NULL) { // 找到被删除边结点
			if (p == s) // 该结点是边链表的首结点
				nodeTable[v1].ed = p->link;
			else
				q->link = p->link; // 不是，重新链接
			delete p;
		}
		else // 没找到
			return false;

		// v2对应边链表中删除
		p = nodeTable[v2].ed;
		q = NULL;
		s = p; // 保存首结点
		while (p != NULL && p->router2 != v1) { // 寻找边链表中要删除的结点
			q = p;
			p = p->link;
		}
		if (p == s) // 删除的该结点是边链表的首结点
			nodeTable[v2].ed = p->link;
		else
			q->link = p->link; // 不是，重新链接
		p->~edge(); t_rebuild();
		return true;
	}
	
	return false; // 没有找到结点
}

// 取顶点v的第一个邻接顶点
int Graphlnk::getFirstNeighbor(int v) {
	if (v != -1) {
		edge* p = nodeTable[v].ed; // 对应链表第一个边结点
		if (p != NULL) // 存在，返回第一个邻接顶点
			return p->router2;
	}
	return -1; // 第一个邻接顶点不存在
}

// 取顶点v的邻接顶点w的下一邻接顶点
int Graphlnk::getNextNeighbor(int v, int w) {
	if (v != -1) {
		edge* p = nodeTable[v].ed; // 对应链表第一个边结点
		while (p != NULL && p->router2 != w) // 寻找邻接顶点w
			p = p->link;
		if (p != NULL && p->link != NULL)
			return p->link->router2;  // 返回下一个邻接顶点
	}
	return -1; // 下一个邻接顶点不存在
}

// 给出顶点vertex在图中的位置
int Graphlnk::getRouterPos(const int vertex) {
	for (int i = 0; i < numRouters; i++)
		if (nodeTable[i].identi == vertex)
			return i;
	return -1;
}

// 当前顶点数
int Graphlnk::numberOfRouters() {
	return numRouters;
}

void Graphlnk::t_rebuild()//重建所有路由表
{
	for (int i = 0; i < numRouters; i++)
	{
		if (nodeTable[i].table == NULL)continue;
		
		nodeTable[i].apply();
		br_table(i);
	}
	
}


#endif /* Graph_h */
