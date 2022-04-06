#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
#define PATH "C:\\Users\\18147\\Desktop\\"
#define QUEUESIZE 50 //���д�С
/*
Hub-Accelarator�ж���ͼ�ķ�������
	��1�����ڽڵ���������
	��2�����ڽڵ��ÿһ���ھӵķ���
	ѡ��ͼ���ݽṹ ���� �ڽӱ�
*/
//�ڵ�ṹ
typedef struct edge {
	int tail;
	struct edge* next = nullptr;	//��һ����

}edge;
typedef struct node {
	//int val;
	int degree;	//�ܶ���
	bool isHub = false;
	bool isHubXing = false;
	bool isTraveled = false;	//���ʱ��
	edge* firstEdge = nullptr;
}node;

//ͼ�ṹ��ʹ�÷ֱ�ļ��Ͻ��б�ʾ���ȴ������Ż���
class graph {
public:
	node* V;	//�ڵ㼯
	int nodeNum;
	//���캯��,ͨ���ļ���Ϣ����ͼ�ṹ
	graph(string path) {
		fstream f;
		f.open(path, ios::in);
		//�����ļ���Ϣ���ļ���ʽ������һ�����֣���ʾ�������������������ÿ�ж�Ӧһ�����㣬��һ������ʾ�ߵ��������������������
		f >> this->nodeNum;	//�ڵ�����
		//�����ڵ㴢��ṹ
		V = new node[nodeNum];
		int edgeNum = 0, neighbor;
		struct edge* curEdge = nullptr;
		for (int i = 0; i < nodeNum; i++) {
			//��ȡһ��������ʾ�ߵ�����
			f >> edgeNum;
			V[i].degree = edgeNum;
			if (edgeNum > 0) {
				f >> neighbor;
				V[i].firstEdge = new (struct edge){ neighbor,nullptr };
				curEdge = V[i].firstEdge;
			}
			for (int j = 1; j < edgeNum; j++) {
				f >> neighbor;
				curEdge->next = new (struct edge){ neighbor,nullptr };
				curEdge = curEdge->next;
			}
		}
		//����ڴ��ȡ
		f.close();
	}
	//չʾͼ�ṹ�������ڽӱ�ṹ��ӡͼ�ṹ��
	void showGraph() {
		//���δ�ӡ�ڵ���Ϣ
		struct edge* curEdge = nullptr;
		for (int i = 0; i < nodeNum; i++) {
			cout <<"node: "<< i << " edge: ";
			curEdge = V[i].firstEdge;
			while (curEdge) {
				cout << curEdge->tail << " ";
				curEdge = curEdge->next;
			}
			cout << endl;
		}
	}
	//����hub����
	void setHub(int k) {
		//��������ͼ������һ����������Hub����
		//��������k�Ľڵ㱻��Ϊ����
		for (int i = 0; i < this->nodeNum; i++) {
			if (V[i].degree > k) {
				V[i].isHub = true;
			}
		}
	}
};
void BFSExtraction(graph &g,int centerNode,int k) {
	int* b = new int[g.nodeNum] {1};
	int* f = new int[g.nodeNum]{ 0 };
	int* level = new int[g.nodeNum]{ 0 };
	int* parent = new int[g.nodeNum]{ -1 };
	//isHubXing��ʼ��
	for (int i = 0; i < g.nodeNum; i++) {
		g.V[i].isHubXing = g.V[i].isHub;
	}
	//ѭ�����нṹ
	int* queue = new int[QUEUESIZE];
	int head =0, tail = 0;	//����ָ��
	queue[tail++] = centerNode;	g.V[centerNode].isTraveled = true;//�����
	while (head != tail) {	//���зǿ�
		int curNodeNo = queue[head++]; head = head % QUEUESIZE;	//������
		if (g.V[curNodeNo].isHub && level[curNodeNo] >= 1 && b[curNodeNo] == 1) {

			g.V[curNodeNo].isHubXing = true;
			b[curNodeNo] = 0;
		}
		if (level[curNodeNo] == k) continue;
		if (b[curNodeNo] == 1 && g.V[curNodeNo].isHubXing) {
			f[curNodeNo]++;
		}
		//����u��ÿһ���ڽӵ�
		edge* e = g.V[curNodeNo].firstEdge;
		while (e) {
			if (!g.V[e->tail].isTraveled) {
				queue[tail++] = e->tail; tail = tail % QUEUESIZE;
			}
			else if (level[e->tail] == level[curNodeNo] + 1) {
				if (b[curNodeNo] == 0) {
					b[e->tail] = 0;
				}
				else if (b[e->tail] == 1 && f[curNodeNo] > f[e->tail]) {
					f[e->tail] = f[curNodeNo];
					parent[e->tail] = curNodeNo;
				}
			}
			e = e->next;
		}
	}
}
int main() {
	//��ȡ�ļ���Ϣ
	graph g(PATH+string("graph.txt"));
	g.showGraph();
	cout <<"end"<< endl;
	return 0;
}
