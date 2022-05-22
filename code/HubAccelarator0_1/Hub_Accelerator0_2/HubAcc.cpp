#include "HubAcc.h"

graph G = nullptr;
int nodeNum = 0;
/**
* �����ļ�������ͼ�ṹ
*/
void readGraph(string filePath) {
	char buffer[BUFFERSIZE];
	//���ļ�
	FILE* f = fopen(filePath.c_str(), "r");
	edge** curPointer = nullptr;
	//�������֣��ڵ�����������������
	fscanf(f,"%d %*d %*d\n", &nodeNum);
	//����ͼ���ݽṹ
	G = new node[nodeNum];	//����ͼ���ݽṹ
	char* index1,* index2; //���ݴ���
	int curNode = 0;
	while (fgets(buffer, BUFFERSIZE, f) != 0&&curNode < nodeNum) {
		index1 = buffer; index2 = buffer;
		if (buffer[0] == '\n') {
			curNode++;
			continue;
		}
		curPointer = &(G[curNode].firstEdge);
		//���ݴ���
		//cout <<endl<< "line [" << curNode+2 << "]:" << endl;
		while (1) {
			if (( * index1) == '\n' || (*index1)==0) break;
			index2 = strchr(index1, ' ');
			if(index2) *index2 = 0; //�����ַ�����β
			//cout << "string:"<<index1 << " ";
			int edgeTail = atoi(index1) - 1;		//���ݽṹ�еı��Ϊʵ�ʱ��-1
			//cout << "int:"<<edgeTail << "\t";
			(*curPointer) = new edge{edgeTail};
			curPointer = &((*curPointer)->next);
			if (!index2) break;
			index1 = index2 + 1;
		}
		//��鵱ǰ�ڵ�ı�
		/*edge* e = G[curNode].firstEdge;
		cout << "check "<<curNode << endl;
		while (e) {
			cout << e->tail<<" ";
			e = e->next;
		}
		cout << endl;*/
		curNode++;
	}
	//����ļ����ݶ���
	fclose(f);
	return;
}
void readGraph1(string filePath) {
	//���ļ�
	FILE* f = fopen(filePath.c_str(), "r");
	//�������֣��ڵ�����������������
	fscanf(f, "%d %*d %*d\n", &nodeNum);
	//����ͼ���ݽṹ
	G = new node[nodeNum];	//����ͼ���ݽṹ
	int curNode = 0;
	char ch;
	int curTail = 0;	//��ǰ��������Ľڵ�
	edge** curPointer = &(G[0].firstEdge);
	while ((ch = fgetc(f))!=EOF) {
		//���ַ����룬���ַ�ɨ���ȡ��Ϣ
		/*
			�����س���ָ����һ���ڵ�ı�ָ��λ��
			�����ո񣬴����µı߽ڵ�
			�����ַ�������Ϊ������Ϣ�����浽curTail��
		*/
		if (ch == '\n') {
			curNode++;
			if (curNode >= nodeNum) break;
			curPointer = &(G[curNode].firstEdge);
		}
		else if (ch == ' ') {
			//�����ڵ㣬���Ѿ��õ��Ľڵ���Ϣ���浽�߼���
			(*curPointer) = new edge{curTail-1};
			curPointer = &((*curPointer)->next);
			curTail = 0;
		}
		else {//����������Ϣ�����浽�ڵ���
			curTail = curTail * 10 + ch - '0';
		}
	}
	fclose(f);
	return;
}


/**
* ����ԭ��ʽ��ӡ��ͼ�ṹ�����ڼ��ͼ�ṹ�������ȷ��
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
* ����ͼ���ݽṹ
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
	//�������
	free(G);
	G = nullptr;
	nodeNum = 0;
}

