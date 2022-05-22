#include "HubAcc.h"
extern graph G;
int main() {
	readGraph("C:\\Users\\18147\\Desktop\\kron_g500-logn16.graph");
	//showGraph("C:\\Users\\18147\\Desktop\\test1.graph");
	/*G[0].isHub = G[0].isHubX = true;
	G[2].isHub = G[2].isHubX = true;
	G[4].isHub = G[4].isHubX = true;
	G[9].isHub = G[9].isHubX = true;
	G[11].isHub = G[11].isHubX = true;*/
	//对每一个节点执行上述操作
	/*BFSExtraction(0, 5);
	BFSExtraction(2,5);
	BFSExtraction(4,5);
	BFSExtraction(9,5);
	BFSExtraction(11,5);*/

	/*BFS_SSSP(8); */
	//setHub(10);
	//对每一个Hub调用BFSExtraction
	/*for (int i = 0; i < nodeNum; i++) {
		if (G[i].isHub) BFSExtraction(i,5);
	}*/
	cout << "extraction finished" << endl;
	//调用函数进行搜索
	//cout << HN_BBFS(0, 23426, 5);
	BFS_SSSP(0);



	cout << endl;
	destroyGraph();
}