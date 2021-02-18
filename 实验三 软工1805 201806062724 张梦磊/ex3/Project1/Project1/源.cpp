#include<iostream>
using namespace std;
const int Max_process = 50;//最大进程数
const int Max_source = 50;//最大资源数


class bank
{
private:
	int available[Max_source];//可用资源数
	int max[Max_process][Max_source];//最大需求
	int allocation[Max_process][Max_source];//已分配资源数
	int need[Max_process][Max_source];//还需资源数
	int request[Max_process][Max_source];//进程需要资源数
	bool finish[Max_process];//判断系统是否有足够的资源分配
	int p[Max_process];//记录序列
	int m;//用来表示进程
	int n;//表示资源

public:
	void Init();//完成对变量的初始化
	bool Safe();//安全检测算法
	void Banker();//银行家算法
	void Display(int, int);//显示进程与资源状态

};

void bank::Init()
{
	m = 0;
	n = 0;
	for (int i = 0;i < Max_source;i++) {
		available[i] = 0;
	}
	for (int i = 0;i < Max_process;i++) {
		for (int j = 0;j < Max_source;j++) {
			max[i][j] = 0;
		}
	}
	for (int i = 0;i < Max_process;i++) {
		for (int j = 0;j < Max_source;j++) {
			allocation[i][j] = 0;
		}
	}
	for (int i = 0;i < Max_process;i++) {
		for (int j = 0;j < Max_source;j++) {
			need[i][j] = 0;
		}
	}
	for (int i = 0;i < Max_process;i++) {
		for (int j = 0;j < Max_source;j++) {
			request[i][j] = 0;
		}
	}
	for (int i = 0;i < Max_process;i++) {
		finish[i] = false;
	}
	for (int i = 0;i < Max_process;i++) {
		p[i] = 0;
	}

	cout << "请输入进程的数目m：";
	cin >> m;

	cout << "请输入资源的种类个数n:";
	cin >> n;

	cout << "请输入每个进程最多所需的各资源数，按照" << m << 'X' << n << "矩阵格式输入" << endl;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			cin >> max[i][j];

	cout << "请输入每个进程已分配的各资源数，按照" << m << 'X' << n << "矩阵格式输入" << endl;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			cin >> allocation[i][j];
			need[i][j] = max[i][j] - allocation[i][j];//注意这里的need可能小于0；要进行报错并重新输入，可以用continue来跳出当前循环
			if (need[i][j] < 0)
			{
				cout << "你输入的第" << i + 1 << "个进程的第" << j + 1 << "个资源数有问题！\n请重新输入！";
				j--;//忽略这个资源数
				continue;//跳出本次循环
			}
		}

	cout << "请输入各个资源现有的数目" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> available[i];
	}

}
//m表示进程，n表示资源
void bank::Display(int n, int m)
{
	cout << endl << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

	cout << "系统可用的资源数为：	";
	for (int i = 0; i < n; i++)
	{
		cout << available[i] << "	";
	}

	cout << endl << "各进程还需要的资源量：" << endl;
	for (int i = 0; i < m; i++)
	{
		cout << "	进程" << i << ":";
		for (int j = 0; j < n; j++)
			cout << "		" << need[i][j];
		cout << endl;
	}


	cout << endl << "各进程已经得到的资源:		" << endl;
	for (int i = 0; i < m; i++)
	{
		cout << "	进程" << i << ":";
		for (int j = 0; j < n; j++)
		{
			cout << "		" << allocation[i][j];
		}
		cout << endl;
	}

	cout << endl << endl;
}


void bank::Banker()
{
	int i, cusneed, flag = 0;//cusneed表示资源进程号
	int again;//键盘录入一个字符用于判断是否继续请求资源
	int again1;
	while (1)
	{
		Display(n, m);
		cout << endl;
		/*请求资源*/
		while (true)
		{
			cout << "请输入要申请的进程号" << endl;
			cin >> cusneed;
			if (cusneed >= m)
			{
				cout << "没有该进程，请重新输入" << endl;
				continue;
			}
			cout << "请输入进程所请求的各资源数" << endl;
			for (int i = 0; i < n; i++)
				cin >> request[cusneed][i];
			for (int i = 0; i < n; i++)
			{
				if (request[cusneed][i] > need[cusneed][i])
				{
					cout << "你输入的资源请求数超过进程需求量！请重新输入" << endl;
					again1 = 1;
					continue;
				}
				if (request[cusneed][i] > available[i])
				{
					cout << "你输入的资源请求数超过系统当前资源拥有数！请重新输入" << endl;
					again1 = 1;
					break;
				}
			}
			if (again1 == 1) { 
				again1 = 0;
				continue;
			}
			break;
		}

		/*上述是资源请求不合理的情况，下面是资源请求合理时则执行银行家算法*/
		for (int i = 0; i < n; i++)
		{
			available[i] -= request[cusneed][i];//可用资源减去成功申请的
			allocation[cusneed][i] += request[cusneed][i];//已分配资源加上成功申请的
			need[cusneed][i] -= request[cusneed][i];//进程还需要的减去成功申请的
		}

		/*判断分配申请资源后系统是否安全，如果不安全则将申请过的资源还给系统*/
		if (Safe())
			cout << "同意分配请求！";
		else
		{
			cout << "你的请求被拒绝！ ！！" << endl;
			/*进行向系统还回资源操作*/
			for (int i = 0; i < n; i++)
			{
				available[i] += request[cusneed][i];
				allocation[cusneed][i] -= request[cusneed][i];
				need[cusneed][i] += request[cusneed][i];
			}
		}

		/*对进程的需求资源进行判断，是否还需要资源，简言之就是判断need数组是否为0*/
		for (int i = 0; i < n; i++)
			if (need[cusneed][i] <= 0)
				flag++;
		if (flag == n)
		{
			for (int i = 0; i < n; i++)
			{
				available[i] += allocation[cusneed][i];
				allocation[cusneed][i] = 0;
				need[cusneed][i] = 0;
			}
			cout << "进程" << cusneed << "占有的资源已释放！！" << endl;
			flag = 0;
		}
		for (int i = 0; i < m; i++)
			finish[i] = false;
		/*判断是否继续申请*/
		cout << "你还想再次请求分配吗？是请按1，否请按其他键！" << endl;
		cin >> again;
		if (again == 1)
			continue;
		break;
	}
}

bool bank::Safe()
{
	int l = 0, j, i;
	int work[Max_source];
	/*对work数组进行初始化，初始化时和avilable数组相同*/
	for (int i = 0; i < n; i++)
		work[i] = available[i];
	/*对finish数组初始化全为false*/
	for (int i = 0; i < m; i++)
		finish[i] = false;
	for (int i = 0; i < m; i++)
	{
		if (finish[i] == true)
			continue;
		else
		{
			for (j = 0; j < n; j++)
			{
				if (need[i][j] > work[j])
					break;
			}
			if (j == n)
			{
				finish[i] = true;
				for (int k = 0; k < n; k++)
					work[k] += allocation[i][k];
				p[l++] = i;//记录进程号
				i = -1;
			}
			else
				continue;
		}
	}

	if (l == m)
	{
		cout << "系统是安全的" << endl;
		cout << "安全序列：" << endl;
		for (int i = 0; i < l; i++)
		{
			cout << p[i];
			if (i != l - 1)
				cout << "-->";
		}
		cout << endl << endl;
		return true;
	}
	else { 
		cout << "系统不安全，请重新输入" << endl; 
		return false;
	}

}


int main()
{
	bank peter;
	peter.Init();
	while (!peter.Safe()) {
		peter.Init();
	}
	peter.Banker();
	return 0;
}