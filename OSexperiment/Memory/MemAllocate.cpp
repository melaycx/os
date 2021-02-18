// MemAllocate.cpp: implementation of the CMemAllocate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Memory.h"
#include "MemAllocate.h"

#include "..\OSgraphics\Rectangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemAllocate::CMemAllocate()
{	
	//设置初始状态
	m_MemList = 0;
	InitMemoryBlock();
	InitJobs();
	flag = m_MemList;
}

CMemAllocate::~CMemAllocate()
{
	for (MemoryBlock * p = m_MemList; p!=0; p = m_MemList)
	{
		m_MemList = p->nextPointer;
		delete p;
	}
}
void CMemAllocate::DrawMemory(CDC*pDC)//通过给内存块填充颜色显示内存的分配情况
{
	CPoint ptMemLeftTop = CPoint(100,50);
	CRectangle myRect(RGB(255,111,0),4,0,RGB(100,100,100),0,1,ptMemLeftTop,ptMemLeftTop);
	for (MemoryBlock * p = m_MemList; p!=0; p = p->nextPointer)
	{
		CPoint tmpLeftTop;
		tmpLeftTop.x = ptMemLeftTop.x ;
		tmpLeftTop.y = ptMemLeftTop.y + p ->StartAddr;
		CPoint tmpRightBottom;
		tmpRightBottom.x = ptMemLeftTop.x + 200;
		tmpRightBottom.y = tmpLeftTop.y + p->BlockLength;
		myRect.SetRectPeak(tmpLeftTop,tmpRightBottom);
		if (p->JobIndex  == 0 ) //如果内存块空， 填充浅色
			myRect.SetColorFill(RGB(200,200,200));
		else
			myRect.SetColorFill(RGB(100,100,100));
		
		myRect.Draw(pDC);
	}
}

void CMemAllocate::InitMemoryBlock()//初始化内存块，大小为1KB
{
	for (MemoryBlock * p = m_MemList; p!=0; p = m_MemList)
	{
		m_MemList = p->nextPointer;
		delete p;
	}	
	MemoryBlock * tmpMemBlock = new MemoryBlock;
	tmpMemBlock->BlockLength = 1024;/*千字节KB*/
	tmpMemBlock->JobIndex = 0;
	tmpMemBlock->StartAddr = 0;
	tmpMemBlock->nextPointer = 0;
	tmpMemBlock->prePointer = 0;
	
	m_MemList = tmpMemBlock;
}
void CMemAllocate::AddAJobInEnd(char * JobName,int JobIndex,int JobLength,int* JobPageTable)//添加进程
{
	JCBInfo * tmpJob = new JCBInfo;
	tmpJob ->JobName = JobName;
	tmpJob ->JobIndex =JobIndex;
	tmpJob ->JobLength = JobLength;
	tmpJob ->JobPageTable = JobPageTable;

	tmpJob ->nextPointer = 0;
	tmpJob ->prePointer = m_JobsEnd;

	m_JobsEnd = tmpJob;
	if (m_JobsEnd ->prePointer != 0)
		m_JobsEnd ->prePointer ->nextPointer = tmpJob;
	else
		m_JobsHead = tmpJob;
}

void CMemAllocate::InitJobs()//初始化进程列表，其中Joblength是指该作业所需的内存大小，如180表示180字节
{
	m_JobsHead = m_JobsEnd = 0;
	AddAJobInEnd("Job1",1,180,NULL);
	AddAJobInEnd("Job2",2,360,NULL);
	AddAJobInEnd("Job3",3,180,NULL);
	AddAJobInEnd("Job4",4,60,NULL);
	AddAJobInEnd("Job5",5,020,NULL);
}

void CMemAllocate::AllocateMem_FirstAdapt(CMemAllocate::JCBInfo& job)//实现了首次适配算法
{
	for (MemoryBlock * p = m_MemList; p != 0; p=p->nextPointer)
	{
		if (job.JobLength < p->BlockLength && p->JobIndex == 0)			//如果作业需要空间小于内存块空间，分配
		{
			MemoryBlock * tmpMemBlock = new MemoryBlock;
			tmpMemBlock ->BlockLength = job.JobLength;
			tmpMemBlock ->JobIndex = job.JobIndex;
			tmpMemBlock ->StartAddr = p->StartAddr;
			tmpMemBlock ->nextPointer = p;
			tmpMemBlock ->prePointer = p ->prePointer;
			
			p->StartAddr = p->StartAddr + job.JobLength;
			p->BlockLength = p->BlockLength -job.JobLength;
			if (p->prePointer == 0 )
			{
				m_MemList = tmpMemBlock;
			}
			else
			{
				p->prePointer->nextPointer = tmpMemBlock;
			}
			p ->prePointer = tmpMemBlock;
			break;
		}
		else
		{
			continue;
		}
	}
}

void CMemAllocate::AllocateMem_BestAdapt(CMemAllocate::JCBInfo& job)//实现了最佳适配算法
{	
	long MinBlockLength=1024;
	MemoryBlock * q;
	for (MemoryBlock * p = m_MemList; p != 0; p=p->nextPointer)
	{
		if (job.JobLength <= p->BlockLength && p->JobIndex == 0)			//如果作业需要空间小于内存块空间，分配
		{
			if(p->BlockLength<=MinBlockLength)
			{
				MinBlockLength=p->BlockLength;
				q=p;
			}
		}
	}
	if(q!=0)
	{
		MemoryBlock * tmpMemBlock = new MemoryBlock;
			tmpMemBlock ->BlockLength = job.JobLength;
			tmpMemBlock ->JobIndex = job.JobIndex;
			tmpMemBlock ->StartAddr = q->StartAddr;
			tmpMemBlock ->nextPointer = q;
			tmpMemBlock ->prePointer = q ->prePointer;
			
			q->StartAddr = q->StartAddr + job.JobLength;
			q->BlockLength = q->BlockLength -job.JobLength;
		if (q->prePointer == 0 )
		{
			m_MemList = tmpMemBlock;
		}
		else
		{
			q->prePointer->nextPointer = tmpMemBlock;
		}
		q ->prePointer = tmpMemBlock;
		
	}
}




void CMemAllocate::AllocateMem_NextAdapt(CMemAllocate::JCBInfo& job) //实现了邻近适配算法
{
	for(MemoryBlock * p = flag;p!=0;p = p->nextPointer) //从最近一次分配的位置开始进行首次适配算法
	{
		if(job.JobLength <= p->BlockLength && p->JobIndex == 0) //如果作业需要空间小于内存块空间，分配
		{
			MemoryBlock * tmpMemBlock = new MemoryBlock;
			tmpMemBlock ->BlockLength = job.JobLength;
			tmpMemBlock ->JobIndex = job.JobIndex;
			tmpMemBlock ->StartAddr = p->StartAddr;
			tmpMemBlock ->nextPointer = p;
			tmpMemBlock ->prePointer = p ->prePointer;
			
			p->StartAddr = p->StartAddr + job.JobLength;
			p->BlockLength = p->BlockLength - job.JobLength;
			if(p->prePointer == 0 )
			{
				m_MemList = tmpMemBlock;
			}
			else
			{
				p->prePointer->nextPointer = tmpMemBlock;
			}
			p->prePointer = tmpMemBlock;
			flag = tmpMemBlock; //记录最近一次分配的作业
			break;
		}
		else
		{
			continue;
		}
	}
}

void CMemAllocate::DeleteJob(int JobIndex) //删除作业 增加代码
{
	for(MemoryBlock * p = m_MemList;p->JobIndex != JobIndex;p = p->nextPointer); //找到需要删除的作业
	MemoryBlock * pre = p->prePointer;
	MemoryBlock * next = p->nextPointer;
	if(pre!=0 && next!=0 && pre->JobIndex == 0 && next->JobIndex == 0) //作业上一块和下一块都空闲
	{
		pre->BlockLength += p->BlockLength + next->BlockLength;
		pre->nextPointer = next->nextPointer;
		if(flag->JobIndex == JobIndex)
			flag=pre;
		if(next->nextPointer != 0)
			next->nextPointer->prePointer = pre;
	}
	else if(next!=0 && next->JobIndex == 0) //作业下一块空闲
	{
		next->StartAddr = p->StartAddr;
		next->BlockLength += p->BlockLength;
		next->prePointer = pre;
		if(flag->JobIndex == JobIndex)
			flag=next;
		if(pre!=0)
			pre->nextPointer = next;
	}
	else if(pre!=0 && pre->JobIndex == 0) //作业上一块空闲
	{
		pre->BlockLength += p->BlockLength;
		pre->nextPointer = next;
		if(flag->JobIndex == JobIndex)
			flag=pre;
		if(next!=0)
			next->prePointer = pre;
	}
	else //作业上下都被占用，直接释放
		p->JobIndex = 0;
}
void CMemAllocate::test()
{
	for (JCBInfo * p = m_JobsHead; p!= 0 ; p = p->nextPointer)
	{
//		AllocateMem_FirstAdapt(*p);
//		AllocateMem_BestAdapt(*p);
		AllocateMem_NextAdapt(*p);
		
	}
	DeleteJob(3);	
	AddAJobInEnd("Job6",6,100,NULL);                          
//	AllocateMem_FirstAdapt(*m_JobsEnd);
//	AllocateMem_BestAdapt(*m_JobsEnd);
	AllocateMem_NextAdapt(*m_JobsEnd);
	
}