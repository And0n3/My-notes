#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define SIZE 110  
#define INF 1000000;
struct station {
	char name[30];
	int people;
	int isTransfer; //�Ƿ�����תվ 1�ǣ�2���� 
};

struct sortPeople {
	char name[30];
	int people;
};
station sta[SIZE];	//����վ������ 
int map[SIZE][SIZE];  //�ڽӾ���洢 
int len[SIZE];   //d[i]��ʾԴ�㵽i�����ľ��� 
int visit[SIZE];  //�ڵ��Ƿ񱻷��� 
int j, pos, ans = INF;
int n = 0;  //��¼��ǰ�м���վ�� 


void quickSort();   //�������� 
void selectSort();  //ѡ������ 
void bubbleSort();	//ð������ 
void recordTraffic();   //��¼������ 
void getAdjacentStation();   //��������վ�� 
void getPayOfRoute();   //ͨ��վ����·�߼���Ǯ 
void alterStation(); 	//�޸�ĳ���ڵ����Ϣ 
void deleteStation();   //ɾ��ĳ��վ�� 
void displayAllStation();	//��ӡ����վ������ 
void readFromFile();//���ļ���ȡ 
void initMapAndStation();//��ʼ��map ��station 
int dijkstra(int from, int to);//��Դ�㵽Ŀ�����Сվ������ 
void insertStation(); //������վ�� 
int findStation(char *name); //ͨ��վ�����ֲ��Ҹ�վ���λ�ã��������ڷ���-1�� 
//���ļ���ȡ 
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
			printf("\t\t\tվ�����������趨�������������룡\n");
			break;
		}
	}
	int k, t;
	fscanf(fp, "%d", &k);
	for (int i = 0; i < k; i++) {
		fscanf(fp, "%d", &t);
		sta[t].isTransfer = 1;
	}
	printf("\t\t\t������%d��վ����Ϣ\n", n);
	fclose(fp);
}
//��ʼ��map ��station 
void initMapAndStation()
{
	int i, j;
	for (i = 0; i < SIZE; i++) { //��һ��ʼÿ���㶼���ɴ� 
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
	printf("\t\t\t·����Ϣ����ɹ���\n");
	fclose(fp);
}
int dijkstra(int from, int to) { //��Դ�㵽Ŀ��� 
	int i;
	int pre[n] = { -1 }; //��¼·�� 
	for (int i = 0; i < n; i++) {
		pre[i] = -1;
	}
	for (i = 0; i < SIZE; i++) { //��ʼ�� 
		visit[i] = 0; //һ��ʼÿ���㶼û������ 
		len[i] = map[from][i]; //�ȼ���Դ�㵽������ľ��� 
	}

	for (i = 0; i < n; ++i) { //�Գ�Դ���ÿһ���������̼��� 
		int min = INF;  //��¼��Сlen[i] 
		  //��¼Сlen[i] �ĵ� 
		for (j = 0; j < n; ++j) {
			if (!visit[j] && min > len[j]) {
				pos = j;
				min = len[j];
			}
		}
		visit[pos] = 1;


		for (j = 0; j < n; ++j) {
			if (!visit[j] && (len[j] > (len[pos] + map[pos][j]))) { //���j�ڵ�û�б����ʹ�&&j�ڵ㵽Դ�ڵ�����·��>pos�ڵ㵽Դ�ڵ�����·��+pos�ڵ㵽j�ڵ��·��  
				len[j] = len[pos] + map[pos][j]; //����j�ڵ㵽Դ�ڵ�����·�� 
				pre[j] = pos;
			}
		}
	}
	printf("\t\t\t;��վ�㣺");
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
	char be[30], en[30]; 	//��¼��ʼվ��ͽ���վ��
	int bpos, epos;
	while (1) {
		printf("\t\t\t���������ĳ���վ�㣺");
		scanf("%s", be);
		bpos = findStation(be);
		if (bpos == -1) {
			printf("\t\t\t���������վ�����Ʋ����ڣ����������룡\n");
		}
		else {
			break;
		}
	}
	while (1) {
		printf("\t\t\t���������Ľ���վ�㣺");
		scanf("%s", en);
		epos = findStation(en);
		if (epos == -1) {
			printf("\t\t\t���������վ�����Ʋ����ڣ����������룡\n");
		}
		else {
			break;
		}
	}
	int price;
	//printf("%d %d\n",bpos,epos);
	int v = dijkstra(bpos, epos);
	if (v % 2 == 0) {	//�õ��۸� 
		price = v / 2 + 1;
	}
	else {
		price = v / 2 + 2;
	}
	printf("\n\t\t\t������Ҫ�ļ۸�Ϊ��%d\n", price);
	int bol;
	while (1)
	{
		printf("\t\t\t�Ƿ�Ʊ��(1.�ǣ�2.��)\n\t\t\t");
		scanf("%d", &bol);
		if (bol == 1) {
			printf("\t\t\t��Ʊ�ɹ���\n");
			sta[bpos].people++;
			break;
		}
		else if (bol == 2) {
			printf("\t\t\tȡ����Ʊ��\n");
			break;
		}
		else {
			printf("\t\t\t����ѡ��������������룡\n");
		}
	}


}

void displayAllStation()
{
	printf("\t\t\tվ���б�\n\t\t\t");

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
	printf("\t\t\t��������Ҫ�����վ��������");
	scanf("%d", &m);
	while (m--) {
		int k;
		if (n + 1 >= SIZE) {
			printf("\t\t\tվ�����������������������������վ�����ݣ�������������\n");
			return;
		}
		printf("\t\t\t��������Ҫ�����վ�����ƣ�");
		scanf("%s", &sta[n].name);
		sta[n].people = 0;
		printf("\t\t\t��վ���Ƿ�������תվ��1���� 0����");
		scanf("%d", &sta[n].isTransfer);

		displayAllStation(); 		//��ӡ���е�վ������ 
		printf("\t\t\t�������վ�������ڵ�վ��������");
		scanf("%d", &k);
		char name[30];
		int l;
		while (k) {
			printf("\t\t\t�������վ��������վ������ƣ�վ������Ѵ��ڣ���");
			scanf("%s", name);
			if (l = findStation(name) >= 0) {
				map[l][n] = 1;
				map[n][l] = 1;
				k--;
			}
			else {
				printf("\t\t\tվ�㲻���ڣ�����������\n");
			}
		}
		n++;
	}
	printf("\t\t\t����ɹ���\n");
}


void deleteStation()
{
	char name[30];
	int pos;

	printf("\t\t\t��������Ҫɾ����վ�����ƣ�");
	scanf("%s", name);
	pos = findStation(name);
	if (pos == -1) {
		printf("\t\t\t���������վ�㲻���ڣ�����ɾ������\n");
	}
	else {
		for (int i = pos; i < n; i++) {
			if (i != n - 1) {
				strcpy(sta[i].name, sta[i + 1].name);
				sta[i].isTransfer = sta[i + 1].isTransfer;
				sta[i].people = sta[i + 1].people;
			}
			for (int j = 0; j < n; j++) {	//��Ϊɾ����һ���ڵ㣬����Ķ�Ҫ��ǰ�ƣ����ýڵ���Ϊ���һ����ʱ����Ҫ���⿼�� 
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
		n--;	//����-- 
		printf("\t\t\tɾ���ɹ���\n");
	}
}


void alterStation()
{
	char name[30];
	int pos;

	printf("\t\t\t��������Ҫ�޸ĵ�վ�����ƣ�");
	scanf("%s", name);
	pos = findStation(name);
	if (pos == -1) {
		printf("\t\t\t��վ�㲻���ڣ������޸Ĳ���\n");
	}
	else {
		printf("\t\t\t%12s %12s %12s\n\t\t\t", "վ������", "�Ƿ�Ϊ��תվ", "��ǰ������");
		printf("%12s %12s %12d\n", sta[pos].name, sta[pos].isTransfer == 1 ? "��" : "��", sta[pos].people);
		printf("\t\t\t��������Ҫ�޸ĵ���Ϣ��վ������ �Ƿ�Ϊ��ת1��0�� ��������\n\t\t\t");
		scanf("%s %d %d", &sta[pos].name, &sta[pos].isTransfer, &sta[pos].people);
		printf("\t\t\t�޸ĳɹ���\n\t\t\t%12s %12s %12s\n\t\t\t", "վ������", "�Ƿ�Ϊ��תվ", "��ǰ������");
		printf("%12s %12s %12d\n", sta[pos].name, sta[pos].isTransfer == 1 ? "��" : "��", sta[pos].people);
	}
}

void getAdjacentStation()
{
	char name[30];
	int pos;
	printf("\t\t\t��������Ҫ�޸ĵ�վ�����ƣ�");
	scanf("%s", name);
	pos = findStation(name);
	if (pos == -1) {
		printf("\t\t\t��վ�㲻���ڣ�������ѯ����\n");
		return;
	}
	else {
		printf("\t\t\t���ڵ�վ���У�");
		for (int i = 0; i < n; i++) {
			if (map[pos][i] == 1) {	//��ʱ˵����i��pos���� 
				printf("%s ", sta[i].name);
			}
		}
		printf("\n\t\t\t���ҽ���\n");
	}
}

void recordTraffic()
{
	char name[30];
	int pos;
	while (1)
	{
		printf("\t\t\t��������Ҫ��¼��������վ������(����EOF������¼��DIS�鿴վ��)��");
		scanf("%s", name);
		if (strcmp("EOF", name) == 0) {
			printf("\t\t\t������¼\n");
			return;
		}
		if (strcmp("DIS", name) == 0) {
			displayAllStation();
			continue;
		}
		pos = findStation(name);
		if (pos == -1) {
			printf("\t\t\t���������վ�㲻����,��������\n");
		}
		else {
			printf("\t\t\t��ǰ������Ϊ��%d���������µĿ�������Ϣ��", sta[pos].people);
			scanf("%d", &sta[pos].people);
			printf("\t\t\t�޸ĳɹ�����ǰ������Ϊ��%d\n", sta[pos].people);
		}
	}

}

void printPeople(sortPeople *link)
{
	printf("\t\t\t����ɹ�\n\t\t\t%12s %12s %12s\n", "���", "վ������", "������");
	for (int i = 0; i < n; i++) {
		printf("\t\t\t%12d %12s %12d\n", i + 1, link[i].name, link[i].people);
	}
}

int comp(const void* a, const void* b)
{
	struct sortPeople *aa = (struct sortPeople *)a;
	struct sortPeople *bb = (struct sortPeople *)b;
	return ((bb->people) - (aa->people));		//aa->cΪ�ṹ��ĳ�Ա��bb->cҲΪ�ṹ��ĳ�Ա 
}

void quickSort()   //�������򲢴�ӡ 
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
void selectSort()  //ѡ�����򲢴�ӡ 
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
void bubbleSort()	//ð�����򲢴�ӡ
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
		printf("\t\t\t·�ߣ�");
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
		if (rout[pos][i] == 1) {	//˵����ʱ·������ͨ 
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
	char be[30], en[30]; 	//��¼��ʼվ��ͽ���վ��
	int bpos, epos;
	while (1) {
		printf("\t\t\t���������ĳ���վ�㣺");
		scanf("%s", be);
		bpos = findStation(be);
		if (bpos == -1) {
			printf("\t\t\t�������վ�����Ʋ����ڣ����������룡\n");
		}
		else {
			break;
		}
	}
	while (1) {
		printf("\t\t\t���������Ľ���վ�㣺");
		scanf("%s", en);
		epos = findStation(en);
		if (epos == -1) {
			printf("\t\t\t�������վ�����Ʋ����ڣ����������룡\n");
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


//��ʾϵͳ�˵�
void showMenu() {
	int cmd;
	while (1)
	{
		printf("******************** ϵͳ�˵� *********************\n");
		printf("*             1. ����վ�����                     *\n");
		printf("*             2. ����վ��ɾ��                     *\n");
		printf("*             3. ����վ����Ϣ�޸�                 *\n");
		printf("*             4. �û���Ʊ                         *\n");
		printf("*             5. ������վ��                       *\n");
		printf("*             6. ����վ����������¼               *\n");
		printf("*             7. ����վ������������               *\n");
		printf("*             8. ����·��ͳ��                     *\n");
		printf("*             9. ����վ���ӡ                     *\n");
		printf("*             0. ��������                         *\n");
		printf("***************************************************\n");
		printf("��ѡ���� \n");
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
			printf("\t1. ð������\n");
			printf("\t2. ѡ������\n");
			printf("\t3. ��������\n");
			printf("\t��ѡ������Ҫ�������㷨��");
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
				printf("\t�������\n");
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
