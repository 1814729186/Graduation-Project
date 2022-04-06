#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
#define PATH "C:\\Users\\18147\\Desktop\\"
#define QUEUESIZE 50 //队列大小
/*
Hub-Accelarator中对于图的访问需求：
	（1）对于节点的随机访问
	（2）对于节点的每一个邻居的访问
	选定图数据结构 —— 邻接表
*/
//节点结构
typedef struct edge {
	int tail;
	struct edge* next = nullptr;	//下一条边

}edge;
typedef struct node {
	//int val;
	int degree;	//总度数
	bool isHub = false;
	bool isHubXing = false;
	bool isTraveled = false;	//访问标记
	edge* firstEdge = nullptr;
}node;

//图结构，使用分别的集合进行表示（等待后续优化）
class graph {
public:
	node* V;	//节点集
	int nodeNum;
	//构造函数,通过文件信息构造图结构
	graph(string path) {
		fstream f;
		f.open(path, ios::in);
		//读入文件信息，文件格式：首行一个数字：表示顶点的数量，接下来的每行对应一个顶点，第一个数表示边的数量，后面紧接其零界点
		f >> this->nodeNum;	//节点数量
		//创建节点储存结构
		V = new node[nodeNum];
		int edgeNum = 0, neighbor;
		struct edge* curEdge = nullptr;
		for (int i = 0; i < nodeNum; i++) {
			//读取一个数，表示边的数量
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
		//完成内存读取
		f.close();
	}
	//展示图结构（按照邻接表结构打印图结构）
	void showGraph() {
		//依次打印节点信息
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
	//设置hub中心
	void setHub(int k) {
		//遍历整个图，按照一定规则设置Hub中心
		//度数大于k的节点被置为中心
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
	//isHubXing初始化
	for (int i = 0; i < g.nodeNum; i++) {
		g.V[i].isHubXing = g.V[i].isHub;
	}
	//循环队列结构
	int* queue = new int[QUEUESIZE];
	int head =0, tail = 0;	//队列指针
	queue[tail++] = centerNode;	g.V[centerNode].isTraveled = true;//入队列
	while (head != tail) {	//队列非空
		int curNodeNo = queue[head++]; head = head % QUEUESIZE;	//出队列
		if (g.V[curNodeNo].isHub && level[curNodeNo] >= 1 && b[curNodeNo] == 1) {

			g.V[curNodeNo].isHubXing = true;
			b[curNodeNo] = 0;
		}
		if (level[curNodeNo] == k) continue;
		if (b[curNodeNo] == 1 && g.V[curNodeNo].isHubXing) {
			f[curNodeNo]++;
		}
		//对于u的每一个邻接点
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
	//读取文件信息
	graph g(PATH+string("graph.txt"));
	g.showGraph();
	cout <<"end"<< endl;
	return 0;
}
