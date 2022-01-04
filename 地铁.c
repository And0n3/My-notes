#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define SIZE 110  
#define INF 1000000;
struct station {
	char name[30];
	int people;
	int isTransfer; //是否是中转站 1是，2不是 
};

struct sortPeople {
	char name[30];
	int people;
};
station sta[SIZE];	//定义站点数组 
int map[SIZE][SIZE];  //邻接矩阵存储 
int len[SIZE];   //d[i]表示源点到i这个点的距离 
int visit[SIZE];  //节点是否被访问 
int j, pos, ans = INF;
int n = 0;  //记录当前有几个站点 


void quickSort();   //快速排序 
void selectSort();  //选择排序 
void bubbleSort();	//冒泡排序 
void recordTraffic();   //记录人流量 
void getAdjacentStation();   //查找相邻站点 
void getPayOfRoute();   //通过站点找路线计算钱 
void alterStation(); 	//修改某个节点的信息 
void deleteStation();   //删除某个站点 
void displayAllStation();	//打印所有站点名称 
void readFromFile();//从文件读取 
void initMapAndStation();//初始化map 和station 
int dijkstra(int from, int to);//从源点到目标点最小站点数量 
void insertStation(); //插入新站点 
int findStation(char *name); //通过站点名字查找该站点的位置，若不存在返回-1； 
//从文件读取 
void readFromFile()
{
	FILE *fp;
	int sum = 0;
	fp = fopen("station.txt", "r");
	if (fp == NULL)
	{
		printf_s("\t\t\tFail to open station.txt\n");
		return;
	}
	fscanf(fp, "%d", &sum);
	for (int i = 1; i <= sum; i++) {
		fscanf(fp, "%s", sta[n].name);
		sta[n].people = 0;
		sta[n].isTransfer = 0;
		n++;
		if (n >= SIZE) {
			printf("\t\t\t站点数量超过设定数量，结束导入！\n");
			break;
		}
	}
	int k, t;
	fscanf(fp, "%d", &k);
	for (int i = 0; i < k; i++) {
		fscanf(fp, "%d", &t);
		sta[t].isTransfer = 1;
	}
	printf("\t\t\t共导入%d个站点信息\n", n);
	fclose(fp);
}
//初始化map 和station 
void initMapAndStation()
{
	int i, j;
	for (i = 0; i < SIZE; i++) { //设一开始每个点都不可达 
		for (j = 0; j < SIZE; j++) {
			map[i][j] = INF;
		}
	}
	readFromFile();
	FILE *fp;
	int a, b;
	fp = fopen("route.txt", "r");
	if (fp == NULL)
	{
		printf("\t\t\tFail to open route.txt\n");
		return;
	}
	while (fscanf(fp, "%d %d", &a, &b) != EOF) {
		map[a][b] = 1;
		map[b][a] = 1;
	}
	printf("\t\t\t路线信息导入成功！\n");
	fclose(fp);
}
int dijkstra(int from, int to) { //从源点到目标点 
	int i;
	int pre[n] = { -1 }; //记录路径 
	for (int i = 0; i < n; i++) {
		pre[i] = -1;
	}
	for (i = 0; i < SIZE; i++) { //初始化 
		visit[i] = 0; //一开始每个点都没被访问 
		len[i] = map[from][i]; //先假设源点到其他点的距离 
	}

	for (i = 0; i < n; ++i) { //对除源点的每一个点进行最短计算 
		int min = INF;  //记录最小len[i] 
		  //记录小len[i] 的点 
		for (j = 0; j < n; ++j) {
			if (!visit[j] && min > len[j]) {
				pos = j;
				min = len[j];
			}
		}
		visit[pos] = 1;


		for (j = 0; j < n; ++j) {
			if (!visit[j] && (len[j] > (len[pos] + map[pos][j]))) { //如果j节点没有被访问过&&j节点到源节点的最短路径>pos节点到源节点的最短路径+pos节点到j节点的路径  
				len[j] = len[pos] + map[pos][j]; //更新j节点到源节点的最短路径 
				pre[j] = pos;
			}
		}
	}
	printf("\t\t\t途径站点：");
	for (int i = to; i != -1; i = pre[i]) {
		if (sta[i].isTransfer) {
			printf("*%s <- ", sta[i].name);
		}
		else {
			printf("%s <- ", sta[i].name);
		}
	}
	if (sta[from].isTransfer) {
		printf("*%s", sta[from].name);
	}
	else {
		printf("%s", sta[from].name);
	}
	return len[to];
}


void getPayOfRoute()
{
	char be[30], en[30]; 	//记录开始站点和结束站点
	int bpos, epos;
	while (1) {
		printf("\t\t\t请输入您的出发站点：");
		scanf("%s", be);
		bpos = findStation(be);
		if (bpos == -1) {
			printf("\t\t\t请您输入的站点名称不存在，请重新输入！\n");
		}
		else {
			break;
		}
	}
	while (1) {
		printf("\t\t\t请输入您的结束站点：");
		scanf("%s", en);
		epos = findStation(en);
		if (epos == -1) {
			printf("\t\t\t请您输入的站点名称不存在，请重新输入！\n");
		}
		else {
			break;
		}
	}
	int price;
	//printf("%d %d\n",bpos,epos);
	int v = dijkstra(bpos, epos);
	if (v % 2 == 0) {	//得到价格 
		price = v / 2 + 1;
	}
	else {
		price = v / 2 + 2;
	}
	printf("\n\t\t\t您所需要的价格为：%d\n", price);
	int bol;
	while (1)
	{
		printf("\t\t\t是否购票？(1.是，2.否)\n\t\t\t");
		scanf("%d", &bol);
		if (bol == 1) {
			printf("\t\t\t购票成功！\n");
			sta[bpos].people++;
			break;
		}
		else if (bol == 2) {
			printf("\t\t\t取消购票！\n");
			break;
		}
		else {
			printf("\t\t\t输入选项错误，请重新输入！\n");
		}
	}


}

void displayAllStation()
{
	printf("\t\t\t站点列表\n\t\t\t");

	for (int i = 0; i < n; i++) {
		printf("%15s", sta[i].name);
		if ((i + 1) % 5 == 0) printf("\n\t\t\t");
	}
	printf("\n");
}

int findStation(char *name)
{
	for (int i = 0; i < n; i++) {
		if (strcmp(sta[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

void insertStation()
{
	int m;
	printf("\t\t\t请输入您要插入的站点数量：");
	scanf("%d", &m);
	while (m--) {
		int k;
		if (n + 1 >= SIZE) {
			printf("\t\t\t站点数据容量已满，不允许输入更多站点数据，请扩大容量！\n");
			return;
		}
		printf("\t\t\t请输入您要插入的站点名称：");
		scanf("%s", &sta[n].name);
		sta[n].people = 0;
		printf("\t\t\t该站点是否是是中转站，1、是 0、否：");
		scanf("%d", &sta[n].isTransfer);

		displayAllStation(); 		//打印所有的站点名称 
		printf("\t\t\t请输入该站点所相邻的站点数量：");
		scanf("%d", &k);
		char name[30];
		int l;
		while (k) {
			printf("\t\t\t请输入该站点所相邻站点的名称（站点必须已存在）：");
			scanf("%s", name);
			if (l = findStation(name) >= 0) {
				map[l][n] = 1;
				map[n][l] = 1;
				k--;
			}
			else {
				printf("\t\t\t站点不存在，请重新输入\n");
			}
		}
		n++;
	}
	printf("\t\t\t插入成功！\n");
}


void deleteStation()
{
	char name[30];
	int pos;

	printf("\t\t\t请输入您要删除的站点名称：");
	scanf("%s", name);
	pos = findStation(name);
	if (pos == -1) {
		printf("\t\t\t请您输入的站点不存在，结束删除操作\n");
	}
	else {
		for (int i = pos; i < n; i++) {
			if (i != n - 1) {
				strcpy(sta[i].name, sta[i + 1].name);
				sta[i].isTransfer = sta[i + 1].isTransfer;
				sta[i].people = sta[i + 1].people;
			}
			for (int j = 0; j < n; j++) {	//因为删除了一个节点，后面的都要往前移，当该节点作为最后一个的时候需要特殊考虑 
				if (map[i][j]) {
					if (i != n - 1) {
						map[i][j] = map[i + 1][j];
						map[j][i] = map[j][i + 1];
					}
					else {
						map[i][j] = 0;
						map[j][i] = 0;
					}
				}
			}
		}
		n--;	//总数-- 
		printf("\t\t\t删除成功！\n");
	}
}


void alterStation()
{
	char name[30];
	int pos;

	printf("\t\t\t请输入您要修改的站点名称：");
	scanf("%s", name);
	pos = findStation(name);
	if (pos == -1) {
		printf("\t\t\t该站点不存在，结束修改操作\n");
	}
	else {
		printf("\t\t\t%12s %12s %12s\n\t\t\t", "站点名称", "是否为中转站", "当前客流量");
		printf("%12s %12s %12d\n", sta[pos].name, sta[pos].isTransfer == 1 ? "是" : "否", sta[pos].people);
		printf("\t\t\t请输入您要修改的信息（站点名称 是否为中转1是0否 客流量）\n\t\t\t");
		scanf("%s %d %d", &sta[pos].name, &sta[pos].isTransfer, &sta[pos].people);
		printf("\t\t\t修改成功！\n\t\t\t%12s %12s %12s\n\t\t\t", "站点名称", "是否为中转站", "当前客流量");
		printf("%12s %12s %12d\n", sta[pos].name, sta[pos].isTransfer == 1 ? "是" : "否", sta[pos].people);
	}
}

void getAdjacentStation()
{
	char name[30];
	int pos;
	printf("\t\t\t请输入您要修改的站点名称：");
	scanf("%s", name);
	pos = findStation(name);
	if (pos == -1) {
		printf("\t\t\t该站点不存在，结束查询操作\n");
		return;
	}
	else {
		printf("\t\t\t相邻的站点有：");
		for (int i = 0; i < n; i++) {
			if (map[pos][i] == 1) {	//此时说明点i与pos相邻 
				printf("%s ", sta[i].name);
			}
		}
		printf("\n\t\t\t查找结束\n");
	}
}

void recordTraffic()
{
	char name[30];
	int pos;
	while (1)
	{
		printf("\t\t\t请输入您要记录人流量的站点名称(键入EOF结束记录、DIS查看站点)：");
		scanf("%s", name);
		if (strcmp("EOF", name) == 0) {
			printf("\t\t\t结束记录\n");
			return;
		}
		if (strcmp("DIS", name) == 0) {
			displayAllStation();
			continue;
		}
		pos = findStation(name);
		if (pos == -1) {
			printf("\t\t\t请您输入的站点不存在,重新输入\n");
		}
		else {
			printf("\t\t\t当前客流量为：%d，请输入新的客流量信息：", sta[pos].people);
			scanf("%d", &sta[pos].people);
			printf("\t\t\t修改成功，当前客流量为：%d\n", sta[pos].people);
		}
	}

}

void printPeople(sortPeople *link)
{
	printf("\t\t\t排序成功\n\t\t\t%12s %12s %12s\n", "序号", "站点名称", "人流量");
	for (int i = 0; i < n; i++) {
		printf("\t\t\t%12d %12s %12d\n", i + 1, link[i].name, link[i].people);
	}
}

int comp(const void* a, const void* b)
{
	struct sortPeople *aa = (struct sortPeople *)a;
	struct sortPeople *bb = (struct sortPeople *)b;
	return ((bb->people) - (aa->people));		//aa->c为结构体的成员，bb->c也为结构体的成员 
}

void quickSort()   //快速排序并打印 
{
	sortPeople link[n];
	for (int i = 0; i < n; i++) {
		strcpy(link[i].name, sta[i].name);
		link[i].people = sta[i].people;
	}
	int low = 0, high = n - 1;
	int pos;
	qsort(link, n, sizeof(link[0]), comp);
	printPeople(link);
}
void selectSort()  //选择排序并打印 
{
	sortPeople link[n];
	for (int i = 0; i < n; i++) {
		strcpy(link[i].name, sta[i].name);
		link[i].people = sta[i].people;
	}
	int index;
	char name[30];
	for (int i = 0; i < n - 1; i++) {
		index = i;
		for (int j = i + 1; j < n; j++) {
			if (link[index].people < link[index].people) {
				index = j;
			}
		}
		if (index != i) {
			strcpy(name, link[index].name);
			strcpy(link[index].name, link[i].name);
			strcpy(link[i].name, name);
			int t = link[index].people;
			link[index].people = link[i].people;
			link[i].people = t;
		}
	}
	printPeople(link);
}
void bubbleSort()	//冒泡排序并打印
{
	sortPeople link[n];
	for (int i = 0; i < n; i++) {
		strcpy(link[i].name, sta[i].name);
		link[i].people = sta[i].people;
	}
	char name[30];
	for (int i = 0; i < n - 1; i++) {
		for (int j = i; j < n - 1; j++) {
			if (link[j].people < link[j + 1].people) {
				strcpy(name, link[j].name);
				strcpy(link[j].name, link[j + 1].name);
				strcpy(link[j + 1].name, name);
				int t = link[j].people;
				link[j].people = link[j + 1].people;
				link[j + 1].people = t;
			}
		}
	}
	printPeople(link);
}
// 
void deepMap(int pos, int epos, int *list, int slen, int rout[][SIZE])
{
	if (pos == epos) {
		printf("\t\t\t路线：");
		list[slen] = pos;
		slen++;
		for (int i = 0; i < slen; i++) {
			if (i != slen - 1) {
				if (sta[list[i]].isTransfer == 1)
					printf("*%s->", sta[list[i]].name);
				else
					printf("%s->", sta[list[i]].name);
			}

			else
				if (sta[list[i]].isTransfer == 1)
					printf("*%s", sta[list[i]].name);
				else
					printf("%s", sta[list[i]].name);
		}
		printf("\n");
		return;
	}
	for (int i = 0; i < n; i++) {
		if (rout[pos][i] == 1) {	//说明此时路可以走通 
			rout[pos][i] = 0;
			rout[i][pos] = 0;
			list[slen] = pos;
			slen++;
			deepMap(i, epos, list, slen, rout);
			rout[pos][i] = 1;
			rout[i][pos] = 1;
			list[slen - 1] = -1;
			slen--;
		}
	}
}



void lineQuery()
{
	char be[30], en[30]; 	//记录开始站点和结束站点
	int bpos, epos;
	while (1) {
		printf("\t\t\t请输入您的出发站点：");
		scanf("%s", be);
		bpos = findStation(be);
		if (bpos == -1) {
			printf("\t\t\t您输入的站点名称不存在，请重新输入！\n");
		}
		else {
			break;
		}
	}
	while (1) {
		printf("\t\t\t请输入您的结束站点：");
		scanf("%s", en);
		epos = findStation(en);
		if (epos == -1) {
			printf("\t\t\t您输入的站点名称不存在，请重新输入！\n");
		}
		else {
			break;
		}
	}
	int list[SIZE] = { -1 };
	int rout[SIZE][SIZE] = { 0 };
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			rout[i][j] = map[i][j];
		}
	}
	deepMap(bpos, epos, list, 0, rout);
}


//显示系统菜单
void showMenu() {
	int cmd;
	while (1)
	{
		printf("******************** 系统菜单 *********************\n");
		printf("*             1. 地铁站点插入                     *\n");
		printf("*             2. 地铁站点删除                     *\n");
		printf("*             3. 地铁站点信息修改                 *\n");
		printf("*             4. 用户购票                         *\n");
		printf("*             5. 查相邻站点                       *\n");
		printf("*             6. 地铁站点人流量记录               *\n");
		printf("*             7. 地铁站店人流量排序               *\n");
		printf("*             8. 地铁路线统计                     *\n");
		printf("*             9. 地铁站点打印                     *\n");
		printf("*             0. 结束程序                         *\n");
		printf("***************************************************\n");
		printf("请选择功能 \n");
		scanf("%d", &cmd);
		switch (cmd) {
		case 1:
			insertStation();
			break;
		case 2:
			deleteStation();
			break;
		case 3:
			alterStation();
			break;
		case 4:
			getPayOfRoute();
			break;
		case 5:
			getAdjacentStation();
			break;
		case 6:
			recordTraffic();
			break;
		case 7:
			int q;
			printf("\t1. 冒泡排序\n");
			printf("\t2. 选择排序\n");
			printf("\t3. 快速排序\n");
			printf("\t请选择您想要的排序算法：");
			scanf("%d", &q);
			switch (q) {
			case 1:
				bubbleSort();
				break;
			case 2:
				selectSort();
				break;
			case 3:
				quickSort();
				break;
			default:
				printf("\t输入错误\n");
			}
			break;
		case 8:
			lineQuery();
			break;
		case 9:
			displayAllStation();
			break;
		case 0:
			return;
		default:
			continue;
		}
	}
}

int main()
{
	initMapAndStation();
	showMenu();
	return 0;
}
