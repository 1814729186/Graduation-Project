#include "HubAcc.h"

graph G = nullptr;
int nodeNum = 0;
int *SP = nullptr;
int* parent = nullptr;

/// <summary>
/// Reads the file and create graph structure.
/// </summary>
/// <param name="filePath">The file path.</param>
void readGraph(string filePath) {
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
	while ((ch = fgetc(f)) != EOF) {
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
			(*curPointer) = new edge{ curTail - 1 };
			curPointer = &((*curPointer)->next);
			curTail = 0;
			G[curNode].degree++;
		}
		else {//����������Ϣ�����浽�ڵ���
			curTail = curTail * 10 + ch - '0';
		}
	}
	fclose(f);
	return;
}


/// <summary>
/// Show the graph and output the information to file.
/// </summary>
/// <param name="filePath">The file path.</param>
void showGraph(string filePath) {
	if (!G) return;
	FILE* f = fopen(filePath.c_str(), "w");
	fprintf(f, "%d %d %d\n", nodeNum, 6289992, 100);
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

/// <summary>
/// Destroys the graph.
/// </summary>
void destroyGraph() {
	if (!G) return;
	edge* curEdge = nullptr, * lastEdge = nullptr;
	for (int i = 0; i < nodeNum; i++) {
		curEdge = G[i].firstEdge;
		while (curEdge) {
			lastEdge = curEdge;
			curEdge = curEdge->next;
			free(lastEdge);
		}
	}
	//�������
	free(G);
	G = nullptr;
	nodeNum = 0;
}

void showfb(int* parent, int* f, int* b) {
	cout << endl<< "-------------------------------------------------"<<endl;
	cout <<endl<< "node" << "\t";
	for (int i = 0; i < nodeNum; i++)
		cout << i << "\t";

	cout << endl << "isHub" << "\t";
	for (int i = 0; i < nodeNum; i++)
		cout << G[i].isHub << "\t";

	cout << endl << "isHub*" << "\t";
	for (int i = 0; i < nodeNum; i++)
		cout << G[i].isHubX << "\t";
	
	cout << endl << "f" << "\t";
	for (int i = 0; i < nodeNum; i++)
		cout << f[i] << "\t";

	cout << endl << "b" << "\t";
	for (int i = 0; i < nodeNum; i++)
		cout << b[i] << "\t";

	cout << endl << "p" << "\t";
	for (int i = 0; i < nodeNum; i++)
		cout << parent[i] << "\t";
}

//����Hub����Ϣ�����ȴ���k�Ľڵ�����ΪHub��
void setHub(int k) {
	for (int i = 0; i < nodeNum; i++) {
		if (G[i].degree >= k) { 
			G[i].isHub = true; 
			G[i].isHubX = true; 
		}
	}
}


/// <summary>
/// Hub-Accelerator Discovery method
/// </summary>
/// <param name="h">source point</param>
/// <param name="k"></param>
void BFSExtraction(int h, int k) {
	int u;
	parent = new int[nodeNum] {0};
	parent[h] = -1;
	int* b = new int[nodeNum] {1};  //���ڱ�ʾ��h��Դ�㣩��v����Դ�㣩֮������·�����Ƿ������һ������(Ϊ1ʱ��ʾ�����ڣ���b(v)Ϊtrueʱ��(h,v)���·��֮�䲻����hub��)
	for (int i = 0; i < nodeNum; i++) b[i] = 1;
	/**
	* ����b(v)Ϊ0��������v������hub�����v�ĸ����ڵ��bΪ0�������ڵ���Դ��֮�����һ�����ģ�����d(h,y)=d(h,v)-1��d(y,v)=1
	*/
	int* f = new int[nodeNum] {0};
	/**
	* f���ڼ�¼�������������Ѵ��ڵĶ�����������
	* ���v���������������У���f(v) = max f(u) + 1���� u��v��BFS�еĸ�������h��v�����·��ͨ��uֱ�����ӵ�v�� �� f(v) = max f(u) 
	*/

	int* level = new int[nodeNum] {0};
	bool* visited = new bool[nodeNum] {false};
	visited[h] = true;
	//��ʼ������
	Queue Q;
	Q.inQueue(h);
	while (!Q.isEmpty()) {
		//showfb(parent,f,b);
		u = Q.outQueue();
		if (u == 2)
			cout << "";
		if (G[u].isHub and level[u] >= 1 and b[u] == 1) {	//������
			//��ȡ���·���������·���ڵ�ǰHub�����еĵ���࣬���漰���µĵ���뵽H*��
			int p = parent[u];
			while (p != h) {
				G[p].isHubX = true;
				//���Hub�ڵ�󣬸���f��ֵ�����²���£�ֱ�����ﵱǰ��level
				p = parent[p];
			}


			//��bΪ0��ʾ����û�л�����
			b[u] = 0;
		}
		if (level[u] == k) continue;
		if (b[u] == 1 and G[u].isHubX) {
			f[u] ++;	 //f����
		}
		//����u�������ڽӵ�
		edge* e = G[u].firstEdge;
		int v;
		while (e) {
			v = e->tail;
			if (!visited[v]) {//��һ�η��ʸýڵ�ʱ��ֻ�ǽ����ж�������
				Q.inQueue(v);
				//����v��levelֵ
				level[v] = level[u] + 1;
				//����parent��ֵ
				parent[v] = u;
				//����Ϊ�ѷ���
				visited[v] = true;
				//�޸Ĳ�����b��v
				b[v] = b[u];
			}
			else if (level[v] == level[u] + 1) { 
				if (b[u] == 0) {
					b[v] = 0;
				}
				else if (b[v] == 1 and f[u] > f[v]) {
					f[v] = f[u];
					parent[v] = u; //����parent
				}
			}
			e = e->next;
		}


	}


}

/// <summary>
/// ��Ȩͼ��Դ�����·��BFS��
/// </summary>
/// <param name="s">Դ��</param>
void BFS_SSSP(int s) {
	int* distance = new int[nodeNum];
	for (int i = 0; i < nodeNum; i++) distance[i] = 99999999;
	bool* visited = new bool[nodeNum] {false};
	int* parent = new int[nodeNum] {false};
	for (int i = 0; i < nodeNum; i++) parent[i] = -1;
	Queue q;
	q.inQueue(s);
	distance[s] = 0; visited[s] = true;
	int curNode,curTail; edge* e;
	while (!q.isEmpty()) {
		curNode = q.outQueue();
		e = G[curNode].firstEdge;
		while (e) {
			//����ӦԪ����ջ
			curTail = e->tail;
			if (!visited[curTail]) {
				distance[curTail] = distance[curNode] + 1;
				parent[curTail] = curNode;
				q.inQueue(curTail);
				visited[curTail] = true;	
			}
			e = e->next;
		}
	}
	//����ļ�
	FILE* file = fopen("C:\\Users\\18147\\Desktop\\output.txt","w");
	for (int i = 0; i < nodeNum; i++) {
		fprintf(file, "nodeNo:%d distance: %d path:%d ", i, distance[i],i);
		int p = parent[i];
		while (p != -1) {
			fprintf(file,"%d ", p);
			p = parent[p];
		}
		fprintf(file, "\n");
	}
	fclose(file);
	delete[]distance;
	delete[] visited;
	delete[] parent;
}

/// <summary>
/// HU_BBFS�㷨��˫����������
/// </summary>
/// <param name="s">Դ�ڵ�</param>
/// <param name="t">Ŀ�Ľڵ�</param>
/// <returns></returns>
int HN_BBFS(int s, int t,int k) {
	Queue Qf, Qb;	//��������
	Qf.inQueue(s);
	Qb.inQueue(t);
	int* levelF = new int[nodeNum] {0}; //��¼��������㼶����������Ϣ
	int* levelB = new int[nodeNum] {0}; //��¼��������㼶����������Ϣ
	bool* visitedF = new bool[nodeNum] {false};
	bool* visitedB = new bool[nodeNum] {false};
	int* Dh = new int[nodeNum] {0};	//�����Hub��ľ���
	int* parent = new int[nodeNum] {0};	//��¼���ڵ���Ϣ

	int dist = k + 1;
	bool met = false;
	bool stopf=false, stopb = false;
	while ((not Qf.isEmpty() and !Qb.isEmpty()) and not met and levelF[Qf.getHead()] + levelB[Qb.getHead()] < dist) {
		forwardSearch(s,t,Qf, false, visitedF, visitedB, levelF, levelB, dist, met, stopf,Dh,parent);
		forwardSearch(t,s,Qb, false, visitedB, visitedF, levelB, levelF, dist, met, stopb,Dh,parent);
	}
	stopf = false; stopb = false;
	while (not (Qf.isEmpty() or stopf) and (Qb.isEmpty() or stopb)) {
		if (not stopf) {
			forwardSearch(s, t, Qf, true, visitedF, visitedB, levelF, levelB, dist, met, stopf, Dh,parent);
		}
		if (not stopb) {
			forwardSearch(t, s, Qb, true, visitedB, visitedF, levelB, levelF, dist, met, stopb, Dh,parent);
		}
	}
	delete[]parent;
	delete[]Dh;
	delete[]visitedF;
	delete[]visitedB;
	delete[]levelF;
	delete[]levelB;
	return dist;
}

void forwardSearch(int s,int t,Queue &Qf,bool verification, bool* visitedF,bool* visitedB,int*levelF,int*levelB,int &dist,bool & met,bool &stop,int*Dh,int *parent) {
	int u = Qf.outQueue(),v;
	edge* e = G[u].firstEdge;
	while (e) {
		v = e->tail;
		if (visitedB[v]) {
			if (levelF[u] + levelB[v] + 1 < dist) {
				dist = levelF[u] + levelB[v] + 1;
				if (not met) met = true;
			}
		}
		if (not visitedF[v] and not (verification and G[v].isHubX)) {
			levelF[v] = levelF[u] + 1; //���²㼶
			Dh[v] = Dh[u] + 1;	//�������Hub��ľ���
			if (G[v].isHub) Dh[v] = 0;
			Qf.inQueue(v);
		}
		if(verification and dist >= levelF[v] +Dh[t] + 1){
			stop = true;
		}
		e = e->next;
	}

}

