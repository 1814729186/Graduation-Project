#define _CRT_SECURE_NO_WARNINGS
#ifndef  __mzp__
#define __mzp__
#include <iostream>
#include<stdio.h>
#include <string.h>
#define BUFFERSIZE 1000000
using namespace std;

//图数据类型
typedef struct edge {
	int tail = -1;							//尾节点
	struct edge* next = nullptr;
}edge;
typedef struct node {
	edge* firstEdge = nullptr;	//边
}node,*graph;
extern int nodeNum;	//节点数量
extern graph G;	//图数据结构
void readGraph(string filePath); //读入文件，创建图结构
void readGraph1(string filePath);
void destroyGraph(); //销毁图结构
void showGraph(string filePath); //检查图结构信息
#endif // ! __mzp__