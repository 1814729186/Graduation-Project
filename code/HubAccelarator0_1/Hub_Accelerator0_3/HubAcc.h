#define _CRT_SECURE_NO_WARNINGS
#ifndef  __HubAcc
#define __HubAcc
#include <iostream>
#include<stdio.h>
#include <string.h>
using namespace std;

//图数据类型
typedef struct edge {
	int tail;							//尾节点
	struct edge* next = nullptr;
}edge;
typedef struct node {
	edge* firstEdge = nullptr;	//边
	//Hub-Accelerator 支持结构
	int degree = 0;
	bool isHub = false;		//Hub节点标记
	bool isHubX = false;	//Hub*网络节点标记
}node, * graph;
extern int nodeNum;		//节点数量
extern graph G;			//图数据结构
extern int* SP;				//最短路径长度
extern int* parent;		// 最短路径途径

void readGraph(string filePath); 
void destroyGraph(); //销毁图结构
void showGraph(string filePath); //检查图结构信息
void BFSExtraction(int,int);	//Hub-Network 发现算法
void BFS_SSSP(int s); //常见BFS
void showfb(int* parent, int* f, int* b); 
void setHub(int k);
int HN_BBFS(int s, int t, int k);

#endif // ! __HubAcc

//队列结构
#ifndef __Queue
#define __Queue
class Queue {
	struct queueNode {
		int elem;
		struct queueNode* next;
	};
	struct queueNode* head = nullptr, * tail = nullptr;
public:
	Queue() {//构造函数
		head =new queueNode{-1,nullptr};
		tail = head;
	}
	int getHead() {//获得队首元素，不出栈	
		return head->elem;
	}
	int outQueue() {
		if (head == tail) return -1;	//队空
		int elem = head->elem;
		struct queueNode* h = head;
		head = head->next;
		free(h);
		return elem;
	}
	void inQueue(int elem) {
		//入队列
		tail->elem = elem;
		tail->next = new queueNode{-1,0};
		tail = tail->next;
	}
	bool isEmpty() {
		return head == tail;
	}
	~Queue() {
		struct queueNode* h;
		while (head) {
			h = head;
			head = head->next;
			free(h);
		}
		head = tail = nullptr;
	}
};
void forwardSearch(int s, int t, Queue& Qf, bool verification, bool* visitedF, bool* visitedB, int* levelF, int* levelB, int& dist, bool& met, bool& stop, int* Dh, int* parent);

#endif // ! __Queue
