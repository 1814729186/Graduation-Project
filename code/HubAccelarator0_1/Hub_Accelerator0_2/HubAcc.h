#define _CRT_SECURE_NO_WARNINGS
#ifndef  __mzp__
#define __mzp__
#include <iostream>
#include<stdio.h>
#include <string.h>
#define BUFFERSIZE 1000000
using namespace std;

//ͼ��������
typedef struct edge {
	int tail = -1;							//β�ڵ�
	struct edge* next = nullptr;
}edge;
typedef struct node {
	edge* firstEdge = nullptr;	//��
}node,*graph;
extern int nodeNum;	//�ڵ�����
extern graph G;	//ͼ���ݽṹ
void readGraph(string filePath); //�����ļ�������ͼ�ṹ
void readGraph1(string filePath);
void destroyGraph(); //����ͼ�ṹ
void showGraph(string filePath); //���ͼ�ṹ��Ϣ
#endif // ! __mzp__