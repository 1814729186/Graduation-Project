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
	while ((ch = fgetc(f)) != EOF) {
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
			(*curPointer) = new edge{ curTail - 1 };
			curPointer = &((*curPointer)->next);
			curTail = 0;
			G[curNode].degree++;
		}
		else {//遇到数字信息，储存到节点中
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
	//完成销毁
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

//设置Hub点信息，将度大于k的节点设置为Hub点
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
	int* b = new int[nodeNum] {1};  //用于表示在h（源点）和v（非源点）之间的最短路径中是否存在另一个中心(为1时表示不存在，即b(v)为true时，(h,v)最短路径之间不存在hub点)
	for (int i = 0; i < nodeNum; i++) b[i] = 1;
	/**
	* 设置b(v)为0的条件：v本身是hub点或者v的父级节点的b为0（父级节点与源点之间存在一个中心），即d(h,y)=d(h,v)-1且d(y,v)=1
	*/
	int* f = new int[nodeNum] {0};
	/**
	* f用于记录集线器网络中已存在的顶点的最大数量
	* 如果v本身在中心网络中，则f(v) = max f(u) + 1或者 u是v在BFS中的父级（从h到v的最短路径通过u直接连接到v） 则 f(v) = max f(u) 
	*/

	int* level = new int[nodeNum] {0};
	bool* visited = new bool[nodeNum] {false};
	visited[h] = true;
	//初始化队列
	Queue Q;
	Q.inQueue(h);
	while (!Q.isEmpty()) {
		//showfb(parent,f,b);
		u = Q.outQueue();
		if (u == 2)
			cout << "";
		if (G[u].isHub and level[u] >= 1 and b[u] == 1) {	//基本对
			//提取最短路径，该最短路径在当前Hub网络中的点最多，将涉及的新的点加入到H*中
			int p = parent[u];
			while (p != h) {
				G[p].isHubX = true;
				//添加Hub节点后，更新f的值，向下层更新，直到到达当前的level
				p = parent[p];
			}


			//置b为0表示后续没有基本对
			b[u] = 0;
		}
		if (level[u] == k) continue;
		if (b[u] == 1 and G[u].isHubX) {
			f[u] ++;	 //f自增
		}
		//访问u的所有邻接点
		edge* e = G[u].firstEdge;
		int v;
		while (e) {
			v = e->tail;
			if (!visited[v]) {//第一次访问该节点时，只是进队列而不访问
				Q.inQueue(v);
				//更新v的level值
				level[v] = level[u] + 1;
				//设置parent的值
				parent[v] = u;
				//设置为已访问
				visited[v] = true;
				//修改并设置b，v
				b[v] = b[u];
			}
			else if (level[v] == level[u] + 1) { 
				if (b[u] == 0) {
					b[v] = 0;
				}
				else if (b[v] == 1 and f[u] > f[v]) {
					f[v] = f[u];
					parent[v] = u; //重设parent
				}
			}
			e = e->next;
		}


	}


}

/// <summary>
/// 无权图单源点最短路径BFS版
/// </summary>
/// <param name="s">源点</param>
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
			//将对应元素入栈
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
	//输出文件
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
/// HU_BBFS算法，双向搜索过程
/// </summary>
/// <param name="s">源节点</param>
/// <param name="t">目的节点</param>
/// <returns></returns>
int HN_BBFS(int s, int t,int k) {
	Queue Qf, Qb;	//搜索队列
	Qf.inQueue(s);
	Qb.inQueue(t);
	int* levelF = new int[nodeNum] {0}; //记录正向遍历层级，即距离信息
	int* levelB = new int[nodeNum] {0}; //记录反向遍历层级，即距离信息
	bool* visitedF = new bool[nodeNum] {false};
	bool* visitedB = new bool[nodeNum] {false};
	int* Dh = new int[nodeNum] {0};	//与最近Hub点的距离
	int* parent = new int[nodeNum] {0};	//记录父节点信息

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
			levelF[v] = levelF[u] + 1; //更新层级
			Dh[v] = Dh[u] + 1;	//与最近的Hub点的距离
			if (G[v].isHub) Dh[v] = 0;
			Qf.inQueue(v);
		}
		if(verification and dist >= levelF[v] +Dh[t] + 1){
			stop = true;
		}
		e = e->next;
	}

}

