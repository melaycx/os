#include<iostream>
using namespace std;
const int Max_process = 50;//��������
const int Max_source = 50;//�����Դ��


class bank
{
private:
	int available[Max_source];//������Դ��
	int max[Max_process][Max_source];//�������
	int allocation[Max_process][Max_source];//�ѷ�����Դ��
	int need[Max_process][Max_source];//������Դ��
	int request[Max_process][Max_source];//������Ҫ��Դ��
	bool finish[Max_process];//�ж�ϵͳ�Ƿ����㹻����Դ����
	int p[Max_process];//��¼����
	int m;//������ʾ����
	int n;//��ʾ��Դ

public:
	void Init();//��ɶԱ����ĳ�ʼ��
	bool Safe();//��ȫ����㷨
	void Banker();//���м��㷨
	void Display(int, int);//��ʾ��������Դ״̬

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

	cout << "��������̵���Ŀm��";
	cin >> m;

	cout << "��������Դ���������n:";
	cin >> n;

	cout << "������ÿ�������������ĸ���Դ��������" << m << 'X' << n << "�����ʽ����" << endl;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			cin >> max[i][j];

	cout << "������ÿ�������ѷ���ĸ���Դ��������" << m << 'X' << n << "�����ʽ����" << endl;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			cin >> allocation[i][j];
			need[i][j] = max[i][j] - allocation[i][j];//ע�������need����С��0��Ҫ���б����������룬������continue��������ǰѭ��
			if (need[i][j] < 0)
			{
				cout << "������ĵ�" << i + 1 << "�����̵ĵ�" << j + 1 << "����Դ�������⣡\n���������룡";
				j--;//���������Դ��
				continue;//��������ѭ��
			}
		}

	cout << "�����������Դ���е���Ŀ" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> available[i];
	}

}
//m��ʾ���̣�n��ʾ��Դ
void bank::Display(int n, int m)
{
	cout << endl << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;

	cout << "ϵͳ���õ���Դ��Ϊ��	";
	for (int i = 0; i < n; i++)
	{
		cout << available[i] << "	";
	}

	cout << endl << "�����̻���Ҫ����Դ����" << endl;
	for (int i = 0; i < m; i++)
	{
		cout << "	����" << i << ":";
		for (int j = 0; j < n; j++)
			cout << "		" << need[i][j];
		cout << endl;
	}


	cout << endl << "�������Ѿ��õ�����Դ:		" << endl;
	for (int i = 0; i < m; i++)
	{
		cout << "	����" << i << ":";
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
	int i, cusneed, flag = 0;//cusneed��ʾ��Դ���̺�
	int again;//����¼��һ���ַ������ж��Ƿ����������Դ
	int again1;
	while (1)
	{
		Display(n, m);
		cout << endl;
		/*������Դ*/
		while (true)
		{
			cout << "������Ҫ����Ľ��̺�" << endl;
			cin >> cusneed;
			if (cusneed >= m)
			{
				cout << "û�иý��̣�����������" << endl;
				continue;
			}
			cout << "���������������ĸ���Դ��" << endl;
			for (int i = 0; i < n; i++)
				cin >> request[cusneed][i];
			for (int i = 0; i < n; i++)
			{
				if (request[cusneed][i] > need[cusneed][i])
				{
					cout << "���������Դ��������������������������������" << endl;
					again1 = 1;
					continue;
				}
				if (request[cusneed][i] > available[i])
				{
					cout << "���������Դ����������ϵͳ��ǰ��Դӵ����������������" << endl;
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

		/*��������Դ���󲻺�����������������Դ�������ʱ��ִ�����м��㷨*/
		for (int i = 0; i < n; i++)
		{
			available[i] -= request[cusneed][i];//������Դ��ȥ�ɹ������
			allocation[cusneed][i] += request[cusneed][i];//�ѷ�����Դ���ϳɹ������
			need[cusneed][i] -= request[cusneed][i];//���̻���Ҫ�ļ�ȥ�ɹ������
		}

		/*�жϷ���������Դ��ϵͳ�Ƿ�ȫ���������ȫ�����������Դ����ϵͳ*/
		if (Safe())
			cout << "ͬ���������";
		else
		{
			cout << "������󱻾ܾ��� ����" << endl;
			/*������ϵͳ������Դ����*/
			for (int i = 0; i < n; i++)
			{
				available[i] += request[cusneed][i];
				allocation[cusneed][i] -= request[cusneed][i];
				need[cusneed][i] += request[cusneed][i];
			}
		}

		/*�Խ��̵�������Դ�����жϣ��Ƿ���Ҫ��Դ������֮�����ж�need�����Ƿ�Ϊ0*/
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
			cout << "����" << cusneed << "ռ�е���Դ���ͷţ���" << endl;
			flag = 0;
		}
		for (int i = 0; i < m; i++)
			finish[i] = false;
		/*�ж��Ƿ��������*/
		cout << "�㻹���ٴ�������������밴1�����밴��������" << endl;
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
	/*��work������г�ʼ������ʼ��ʱ��avilable������ͬ*/
	for (int i = 0; i < n; i++)
		work[i] = available[i];
	/*��finish�����ʼ��ȫΪfalse*/
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
				p[l++] = i;//��¼���̺�
				i = -1;
			}
			else
				continue;
		}
	}

	if (l == m)
	{
		cout << "ϵͳ�ǰ�ȫ��" << endl;
		cout << "��ȫ���У�" << endl;
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
		cout << "ϵͳ����ȫ������������" << endl; 
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