#include "HubAcc.h"

graph G = nullptr;
int nodeNum = 0;
/**
* 读入文件，创建图结构
*/
void readGraph(string filePath) {
	char buffer[BUFFERSIZE];
	//打开文件
	FILE* f = fopen(filePath.c_str(), "r");
	edge** curPointer = nullptr;
	//三个数字，节点数，边数，最大度数
	fscanf(f,"%d %*d %*d\n", &nodeNum);
	//创建图数据结构
	G = new node[nodeNum];	//创建图数据结构
	char* index1,* index2; //数据处理
	int curNode = 0;
	while (fgets(buffer, BUFFERSIZE, f) != 0&&curNode < nodeNum) {
		index1 = buffer; index2 = buffer;
		if (buffer[0] == '\n') {
			curNode++;
			continue;
		}
		curPointer = &(G[curNode].firstEdge);
		//数据处理
		//cout <<endl<< "line [" << curNode+2 << "]:" << endl;
		while (1) {
			if (( * index1) == '\n' || (*index1)==0) break;
			index2 = strchr(index1, ' ');
			if(index2) *index2 = 0; //设置字符串结尾
			//cout << "string:"<<index1 << " ";
			int edgeTail = atoi(index1) - 1;		//数据结构中的编号为实际编号-1
			//cout << "int:"<<edgeTail << "\t";
			(*curPointer) = new edge{edgeTail};
			curPointer = &((*curPointer)->next);
			if (!index2) break;
			index1 = index2 + 1;
		}
		//检查当前节点的边
		/*edge* e = G[curNode].firstEdge;
		cout << "check "<<curNode << endl;
		while (e) {
			cout << e->tail<<" ";
			e = e->next;
		}
		cout << endl;*/
		curNode++;
	}
	//完成文件数据读入
	fclose(f);
	return;
}
void readGraph1(string filePath) {
	//打开文件
	FILE* f = fopen(filePath.c_str(), "r");
	//三个数字，节点数，边数，最大度数
	fscanf(f, "%d %*d %*d\n", &nodeNum);
	//创建图数据结构
	G = new node[nodeNum];	//创建图数据结构
	int curNode = 0;
	char ch;
	int curTail = 0;	//当前计算的中心节点
	edge** curPointer = &(G[0].firstEdge);
	while ((ch = fgetc(f))!=EOF) {
		//逐字符读入，逐字符扫描获取信息
		/*
			遇到回车，指向下一个节点的边指针位置
			遇到空格，创建新的边节点
			遇到字符，处理为数字信息，储存到curTail中
		*/
		if (ch == '\n') {
			curNode++;
			if (curNode >= nodeNum) break;
			curPointer = &(G[curNode].firstEdge);
		}
		else if (ch == ' ') {
			//创建节点，将已经得到的节点信息储存到边集中
			(*curPointer) = new edge{curTail-1};
			curPointer = &((*curPointer)->next);
			curTail = 0;
		}
		else {//遇到数字信息，储存到节点中
			curTail = curTail * 10 + ch - '0';
		}
	}
	fclose(f);
	return;
}


/**
* 按照原格式打印出图结构，用于检查图结构构造的正确性
*/
void showGraph(string filePath) {
	if (!G) return;
	FILE* f = fopen(filePath.c_str(), "w");
	fprintf(f,"%d %d %d\n", nodeNum, 6289992, 100);
	edge* curEdge = nullptr;
	for (int i = 0; i < nodeNum; i++) {
		curEdge = G[i].firstEdge;
		while (curEdge) {
			fprintf(f, "%d ", curEdge->tail + 1);
			//cout << curEdge->tail << " ";
			curEdge = curEdge->next;
		}
		fprintf(f, "\n");
		//cout << endl;
	}
	fclose(f);
}


/**
* 销毁图数据结构
*/
void destroyGraph() {
	if (!G) return;
	edge* curEdge = nullptr,*lastEdge=nullptr;
	for (int i = 0; i < nodeNum; i++) {
		curEdge = G[i].firstEdge;
		while (curEdge) {
			lastEdge = curEdge;
			curEdge = curEdge->next;
			free(lastEdge);
			cout << i << " ";
		}
	}
	//完成销毁
	free(G);
	G = nullptr;
	nodeNum = 0;
}

