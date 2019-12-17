#ifndef Graph_h
#define Graph_h
#include <iostream>
using namespace std;

const int DefaultVertices = 30;

template <class T, class E>
struct Edge { // �߽��Ķ���
	int dest; // �ߵ���һ����λ��
	E cost; // ���ϵ�Ȩֵ
	Edge<T, E>* link; // ��һ������ָ��
};

template <class T, class E>
struct Vertex { // ����Ķ���
	T data; // ���������
	Edge<T, E>* adj; // �������ͷָ��
};

template <class T, class E>
class Graphlnk {
public:
	const E maxWeight = 100000; // ����������ֵ��=�ޣ�
	Graphlnk(int sz = DefaultVertices); // ���캯��
	~Graphlnk(); // ��������
	void inputGraph(); // �����ڽӱ��ʾ��ͼ
	void outputGraph(); // ���ͼ�е����ж���ͱ���Ϣ
	T getValue(int i); // ȡλ��Ϊi�Ķ����е�ֵ
	E getWeight(int v1, int v2); // ���رߣ�v1�� v2���ϵ�Ȩֵ
	bool insertVertex(const T& vertex); // ���붥��
	bool insertEdge(int v1, int v2, E weight); // �����
	bool removeVertex(int v); // ɾ������
	bool removeEdge(int v1, int v2); // ɾ����
	int getFirstNeighbor(int v); // ȡ����v�ĵ�һ���ڽӶ���
	int getNextNeighbor(int v, int w); // ȡ����v���ڽӶ���w����һ�ڽӶ���
	int getVertexPos(const T vertex); // ��������vertex��ͼ�е�λ��
	int numberOfVertices(); // ��ǰ������
private:
	int maxVertices; // ͼ�����Ķ�����
	int numEdges; // ��ǰ����
	int numVertices; // ��ǰ������
	Vertex<T, E>* nodeTable; // �����(���������ͷ���)
};

// ���캯��:����һ���յ��ڽӱ�
template <class T, class E>
Graphlnk<T, E>::Graphlnk(int sz) {
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	nodeTable = new Vertex<T, E>[maxVertices]; // �������������
	if (nodeTable == NULL) {
		cerr << "�洢�ռ�������" << endl;
		exit(1);
	}
	for (int i = 0; i < maxVertices; i++)
		nodeTable[i].adj = NULL;
}

// ��������
template <class T, class E>
Graphlnk<T, E>::~Graphlnk() {
	// ɾ�����������еĽ��
	for (int i = 0; i < numVertices; i++) {
		Edge<T, E>* p = nodeTable[i].adj; // �ҵ����Ӧ������׽��
		while (p != NULL) { // ���ϵ�ɾ����һ�����
			nodeTable[i].adj = p->link;
			delete p;
			p = nodeTable[i].adj;
		}
	}
	delete[]nodeTable; // ɾ�����������
}

// �����ڽӱ��ʾ��ͼ
template <class T, class E>
void Graphlnk<T, E>::inputGraph() {
	int n, m; // �洢�������ͱ���
	int i, j, k;
	T e1, e2; // ����
	E weight; // �ߵ�Ȩֵ

	cout << "�����붥�����ͱ�����" << endl;
	cin >> n >> m;
	cout << "����������㣺" << endl;
	for (i = 0; i < n; i++) {
		cin >> e1;
		insertVertex(e1); // ���붥��
	}

	cout << "������ͼ�ĸ��ߵ���Ϣ��" << endl;
	i = 0;
	while (i < m) {
		cin >> e1 >> e2 >> weight;
		j = getVertexPos(e1);
		k = getVertexPos(e2);
		if (j == -1 || k == -1)
			cout << "�����˵���Ϣ�������������룡" << endl;
		else {
			insertEdge(j, k, weight); // �����
			insertEdge(k, j, weight);
			i++;
		}
	} // while
}

// ���ͼ�е����ж���ͱ���Ϣ
template <class T, class E>
void Graphlnk<T, E>::outputGraph() {
	int n, m, i, j;
	T e1, e2; // ����
	E weight; // Ȩֵ

	n = numVertices;
	m = numEdges;
	cout << "ͼ�еĶ�����Ϊ" << n << ",����Ϊ" << m << endl;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			weight = getWeight(i, j); // ȡ�ߵ�Ȩֵ
			if (weight > 0 && weight < maxWeight) { // ��Ч
				e1 = getValue(i); // ����
				e2 = getValue(j);
				cout << "(" << e1 << "," << e2 << "," << weight << ")" << endl;
			}
		} // ��ѭ��for
	} // ��ѭ��for
}

// ȡλ��Ϊi�Ķ����е�ֵ
template <class T, class E>
T Graphlnk<T, E>::getValue(int i) {
	if (i >= 0 && i < numVertices)
		return nodeTable[i].data;
	return NULL;
}

// ���رߣ�v1�� v2���ϵ�Ȩֵ
template <class T, class E>
E Graphlnk<T, E>::getWeight(int v1, int v2) {
	if (v1 != -1 && v2 != -1) {
		Edge<T, E>* p = nodeTable[v1].adj; // v1�ĵ�һ�������ı�
		while (p != NULL && p->dest != v2) { // Ѱ���ڽӶ���v2
			p = p->link;
		}
		if (p != NULL)
			return p->cost;
	}
	return 0; // ��(v1, v2)������
}

// ���붥��
template <class T, class E>
bool Graphlnk<T, E>::insertVertex(const T & vertex) {
	if (numVertices == maxVertices) // ������������ܲ���
		return false;
	nodeTable[numVertices].data = vertex; // �����ڱ�����
	numVertices++;
	return true;
}

// �����
template <class T, class E>
bool Graphlnk<T, E>::insertEdge(int v1, int v2, E weight) {
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
		Edge<T, E>* q, * p = nodeTable[v1].adj; // v1��Ӧ�ı�����ͷָ��
		while (p != NULL && p->dest != v2) // Ѱ���ڽӶ���v2
			p = p->link;
		if (p != NULL) // �Ѵ��ڸñߣ�������
			return false;
		p = new Edge<T, E>; // �����½��
		p->dest = v2;
		p->cost = weight;
		p->link = nodeTable[v1].adj; // ����v1������
		nodeTable[v1].adj = p;

		q = new Edge<T, E>;
		q->dest = v1;
		q->cost = weight;
		q->link = nodeTable[v2].adj; // ����v2������
		nodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}

// ɾ������
template <class T, class E>
bool Graphlnk<T, E>::removeVertex(int v) {
	if (numVertices == 1 || v < 0 || v > numVertices)
		return false; // ��ջ򶥵�ų�����Χ

	Edge<T, E> * p, *s, *t;
	int k; // �洢�ڽӶ���
	while (nodeTable[v].adj != NULL) {
		p = nodeTable[v].adj;
		k = p->dest; // ȡ�ڽӶ���k
		s = nodeTable[k].adj; // �ҶԳƴ�ŵı߽��
		t = NULL;
		while (s != NULL && s->dest != v) {
			t = s;
			s = s->link;
		}
		if (s != NULL) { // ɾ���Գƴ�ŵı߽��
			if (t == NULL) // ɾ�����ǵ�һ���ڽӶ���
				nodeTable[k].adj = s->link;
			else
				t->link = s->link;
			delete s;
		}
		nodeTable[v].adj = p->link; // �������v�ı�������
		delete p;
		numEdges--; // �붥��v������ı�����1
	} // while����
	numVertices--; // ͼ�Ķ��������1
	nodeTable[v].data = nodeTable[numVertices].data; // �
	p = nodeTable[v].adj = nodeTable[numVertices].adj;
	// Ҫ����Ķ����Ӧ��λ�ø�д
	while (p != NULL) {
		s = nodeTable[p->dest].adj; // �ԳƱ�������
		while (s != NULL) {
			if (s->dest == numVertices) { // �ҵ��ԳƱ�
				s->dest = v; // �޸�ָ��v
				break;
			}
			s = s->link;
		}
		p = p->link; // ָ����һ���ڽӶ���
	}
	return true;
}

// ɾ����
template <class T, class E>
bool Graphlnk<T, E>::removeEdge(int v1, int v2) {
	if (v1 != -1 && v2 != -1) {
		Edge<T, E>* p = nodeTable[v1].adj, * q = NULL, * s = p;
		while (p != NULL && p->dest != v2) { // v1��Ӧ���������ұ�ɾ����
			q = p;
			p = p->link;
		}
		if (p != NULL) { // �ҵ���ɾ���߽��
			if (p == s) // �ý���Ǳ�������׽��
				nodeTable[v1].adj = p->link;
			else
				q->link = p->link; // ���ǣ���������
			delete p;
		}
		else // û�ҵ�
			return false;

		// v2��Ӧ��������ɾ��
		p = nodeTable[v2].adj;
		q = NULL;
		s = p; // �����׽��
		while (p != NULL && p->dest != v1) { // Ѱ�ұ�������Ҫɾ���Ľ��
			q = p;
			p = p->link;
		}
		if (p == s) // ɾ���ĸý���Ǳ�������׽��
			nodeTable[v2].adj = p->link;
		else
			q->link = p->link; // ���ǣ���������
		delete p;
		return true;
	}
	return false; // û���ҵ����
}

// ȡ����v�ĵ�һ���ڽӶ���
template <class T, class E>
int Graphlnk<T, E>::getFirstNeighbor(int v) {
	if (v != -1) {
		Edge<T, E>* p = nodeTable[v].adj; // ��Ӧ�����һ���߽��
		if (p != NULL) // ���ڣ����ص�һ���ڽӶ���
			return p->dest;
	}
	return -1; // ��һ���ڽӶ��㲻����
}

// ȡ����v���ڽӶ���w����һ�ڽӶ���
template <class T, class E>
int Graphlnk<T, E>::getNextNeighbor(int v, int w) {
	if (v != -1) {
		Edge<T, E>* p = nodeTable[v].adj; // ��Ӧ�����һ���߽��
		while (p != NULL && p->dest != w) // Ѱ���ڽӶ���w
			p = p->link;
		if (p != NULL && p->link != NULL)
			return p->link->dest;  // ������һ���ڽӶ���
	}
	return -1; // ��һ���ڽӶ��㲻����
}

// ��������vertex��ͼ�е�λ��
template <class T, class E>
int Graphlnk<T, E>::getVertexPos(const T vertex) {
	for (int i = 0; i < numVertices; i++)
		if (nodeTable[i].data == vertex)
			return i;
	return -1;
}

// ��ǰ������
template <class T, class E>
int Graphlnk<T, E>::numberOfVertices() {
	return numVertices;
}

#endif /* Graph_h */
