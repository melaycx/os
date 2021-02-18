#include<Windows.h>
#include<iostream>
#include<stdio.h>
// 创建传递过来的进程的克隆过程并赋于其 ID 值
void StartClone(int nCloneID)
{
	// 提取用于当前可执行文件的文件名
	TCHAR szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);
	// 格式化用于子进程的命令行并通知其 EXE 文件名和克隆 ID
	TCHAR szCmdLine[MAX_PATH];
	::sprintf_s(szCmdLine, "\"%s\" %d", szFilename, nCloneID);
	// 用于子进程的 STARTUPINFO 结构
	STARTUPINFO si;
	::ZeroMemory(reinterpret_cast <void*>(&si), sizeof(si));
	si.cb = sizeof(si);//必须是本结构大小
	// 返回的用于子进程的进程信息
	PROCESS_INFORMATION pi;
	// 利用同样的可执行文件和命令行创建进程，并赋于其子进程的性质
	BOOL bCreateOK = ::CreateProcess(
		szFilename,// 产生这个 EXE 的应用程序的名称
		szCmdLine,// 告诉其行为像一个子进程的标志
		NULL,//缺省的进程安全性
		NULL,//缺省的线程安全性
		FALSE,//不继承句柄
		CREATE_NEW_CONSOLE,//使用新的控制台
		NULL,//新的环境
		NULL,//当前目录
		&si,//启动信息
		&pi//返回的进程信息
	);
	if (bCreateOK)std::cout << "Process create sucessfully" << std::endl;
		// 终止子进程
	BOOL bEndOK = ::TerminateProcess(pi.hProcess, 0);
	if (bEndOK)std::cout << "Process exit successfully" << std::endl;
}

int main(int argc, char* argv[]) {
	//确定进程在列表中的位置
	int nClone(0);
	if (argc > 1) {
		//从第二个参数中提取克隆id
		::sscanf_s(argv[1], "%d", &nClone);
	}
	//显示进程位置
	std::cout << "Process id:" << ::GetCurrentProcessId() 
		<< ",Clone id:" << nClone << std::endl;
	//检查是否有创建子进程的需要
	const int C_nCloneMax = 25;
	if (nClone < C_nCloneMax) {
		//发送新进程的命令行和克隆号
		StartClone(++nClone);
	}
	//终止之前暂停0.5秒
	std::cout << "Process Exit";
	::Sleep(500);
	
		return 0;
}