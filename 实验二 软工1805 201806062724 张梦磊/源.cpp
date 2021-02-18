#include<Windows.h>
#include<iostream>
#include<cstdlib>
#include<queue>
#include<time.h>
using namespace std;

const int N = 10;//�������Ĵ�С
HANDLE m_S_Empty;// ������Semaphore
HANDLE m_S_Full;// ������Semaphore
HANDLE m_M_Mutex;;//�����ź���
queue<int> buffer;//���干������
int i = 0;

DWORD WINAPI Producer(PVOID pParam) {

	while (1) {
		srand((int)time(0));
		int tmp = rand() % 100;
		if (WaitForSingleObject(m_S_Empty, INFINITE) == WAIT_OBJECT_0) {
			if (WaitForSingleObject(m_M_Mutex, INFINITE) == WAIT_OBJECT_0) {
				i++;
				if (i == 20)system("PAUSE");//��������ִ��20���Ժ�ϵͳ��ͣ���۲�������
				buffer.push(tmp + i);
				printf("�����߷���������ǣ�%d\n", tmp + i);
				printf("��ʱ������������Ϊ��");
				//���������������
				for (int j = 0; j < buffer.size(); j++) {
					int x = buffer.front();
					buffer.pop();
					printf("%d ", x);
					buffer.push(x);
				}
				printf("\n");
				ReleaseMutex(m_M_Mutex);
				ReleaseSemaphore(m_S_Full, 1, NULL);
			}
		}
		if (i > 10) Sleep(1500);//��������ִ��10�κ������������ߣ��Դ���ģ�������߲���ӿչ�����ȡ�������
	}

}

DWORD WINAPI Consumer(PVOID pParam) {
	while (1)
	{
		if (WaitForSingleObject(m_S_Full, INFINITE) == WAIT_OBJECT_0) {
			if (WaitForSingleObject(m_M_Mutex, INFINITE) == WAIT_OBJECT_0) {
				int tmp = buffer.front();
				buffer.pop();
				printf("�������ó��������ǣ�%d\n", tmp);
				printf("��ʱ������������Ϊ��");
				for (int j = 0; j < buffer.size(); j++) {
					int x = buffer.front();
					buffer.pop();
					printf("%d ", x);
					buffer.push(x);
				}
				printf("\n");
				ReleaseMutex(m_M_Mutex);
				ReleaseSemaphore(m_S_Empty, 1, NULL);
			}
		}
		if (i < 10) Sleep(1500);//������ִ��10��ǰ�������������ߣ��Դ���ģ�������߲������Ѿ����Ĺ�����������
	}
}

int main() {
	CreateThread(NULL, 0, Producer, 0, 0, 0);
	Sleep(2000);
	CreateThread(NULL, 0, Consumer, 0, 0, 0);
	m_S_Empty = CreateSemaphore(NULL, N, N, NULL);
	m_S_Full = CreateSemaphore(NULL, 0, N, NULL);
	m_M_Mutex = CreateMutex(NULL, FALSE, NULL);
	Sleep(INFINITE);//���߳��������ߣ��������ߺ�������ִ��
	//system("PAUSE");
}