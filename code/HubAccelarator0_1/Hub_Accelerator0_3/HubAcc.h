#define _CRT_SECURE_NO_WARNINGS
#ifndef  __HubAcc
#define __HubAcc
#include <iostream>
#include<stdio.h>
#include <string.h>
using namespace std;

//ͼ��������
typedef struct edge {
	int tail;							//β�ڵ�
	struct edge* next = nullptr;
}edge;
typedef struct node {
	edge* firstEdge = nullptr;	//��
	//Hub-Accelerator ֧�ֽṹ
	int degree = 0;
	bool isHub = false;		//Hub�ڵ���
	bool isHubX = false;	//Hub*����ڵ���
}node, * graph;
extern int nodeNum;		//�ڵ�����
extern graph G;			//ͼ���ݽṹ
extern int* SP;				//���·������
extern int* parent;		// ���·��;��

void readGraph(string filePath); 
void destroyGraph(); //����ͼ�ṹ
void showGraph(string filePath); //���ͼ�ṹ��Ϣ
void BFSExtraction(int,int);	//Hub-Network �����㷨
void BFS_SSSP(int s); //����BFS
void showfb(int* parent, int* f, int* b); 
void setHub(int k);
int HN_BBFS(int s, int t, int k);

#endif // ! __HubAcc

//���нṹ
#ifndef __Queue
#define __Queue
class Queue {
	struct queueNode {
		int elem;
		struct queueNode* next;
	};
	struct queueNode* head = nullptr, * tail = nullptr;
public:
	Queue() {//���캯��
		head =new queueNode{-1,nullptr};
		tail = head;
	}
	int getHead() {//��ö���Ԫ�أ�����ջ	
		return head->elem;
	}
	int outQueue() {
		if (head == tail) return -1;	//�ӿ�
		int elem = head->elem;
		struct queueNode* h = head;
		head = head->next;
		free(h);
		return elem;
	}
	void inQueue(int elem) {
		//�����
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
