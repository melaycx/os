#include<Windows.h>
#include<iostream>
#include<cstdlib>
#include<queue>
#include<time.h>
using namespace std;

const int N = 10;//缓冲区的大小
HANDLE m_S_Empty;// 生产者Semaphore
HANDLE m_S_Full;// 消费者Semaphore
HANDLE m_M_Mutex;;//互斥信号量
queue<int> buffer;//定义共享缓冲区
int i = 0;

DWORD WINAPI Producer(PVOID pParam) {

	while (1) {
		srand((int)time(0));
		int tmp = rand() % 100;
		if (WaitForSingleObject(m_S_Empty, INFINITE) == WAIT_OBJECT_0) {
			if (WaitForSingleObject(m_M_Mutex, INFINITE) == WAIT_OBJECT_0) {
				i++;
				if (i == 20)system("PAUSE");//当生产者执行20次以后系统暂停，观察输出情况
				buffer.push(tmp + i);
				printf("生产者放入的数字是：%d\n", tmp + i);
				printf("此时共享区的内容为：");
				//遍历输出队列内容
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
		if (i > 10) Sleep(1500);//当生产者执行10次后，让生产者休眠，以此来模拟消费者不会从空共享区取数的情况
	}

}

DWORD WINAPI Consumer(PVOID pParam) {
	while (1)
	{
		if (WaitForSingleObject(m_S_Full, INFINITE) == WAIT_OBJECT_0) {
			if (WaitForSingleObject(m_M_Mutex, INFINITE) == WAIT_OBJECT_0) {
				int tmp = buffer.front();
				buffer.pop();
				printf("消费者拿出的数字是：%d\n", tmp);
				printf("此时共享区的内容为：");
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
		if (i < 10) Sleep(1500);//生产者执行10次前，让消费者休眠，以此来模拟生产者不会向已经满的共享区放数据
	}
}

int main() {
	CreateThread(NULL, 0, Producer, 0, 0, 0);
	Sleep(2000);
	CreateThread(NULL, 0, Consumer, 0, 0, 0);
	m_S_Empty = CreateSemaphore(NULL, N, N, NULL);
	m_S_Full = CreateSemaphore(NULL, 0, N, NULL);
	m_M_Mutex = CreateMutex(NULL, FALSE, NULL);
	Sleep(INFINITE);//主线程永久休眠，让消费者和生产者执行
	//system("PAUSE");
}