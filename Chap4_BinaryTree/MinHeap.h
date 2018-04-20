// MinHeap.h: interface for the MinHeap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINHEAP_H__6BC12E9A_B926_47C2_8ACF_AA4004A0546F__INCLUDED_)
#define AFX_MINHEAP_H__6BC12E9A_B926_47C2_8ACF_AA4004A0546F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T>
class MinHeap  
{
private:
	T* heapArray;
	int CurrentSize;
	int MaxSize;
public:
	MinHeap(const int n);
	virtual ~MinHeap()
	{delete []heapArray;};
	void BuildHeap();
	bool isLeaf(int pos) const;
	int leftchild(int pos) const;
	int rightchild(int pos) const;
	// Return parent position
	int parent(int pos) const;
	// 删除给定下标的元素
	bool Remove(int pos, T& node);
	void SiftDown(int left);
	//从position向上开始调整，使序列成为堆
	void SiftUp(int position); 
	BOOL Insert(const T& newNode);
	T& RemoveMin();
};

template<class T>
MinHeap<T>::MinHeap(const int n)
{
	if(n<=0)
		return;
	CurrentSize=0;
	MaxSize=n;
	heapArray=new T[MaxSize];
	BuildHeap();
}

template<class T>
void MinHeap<T>::BuildHeap()
{
	for (int i=CurrentSize/2-1; i>=0; i--) 
		SiftDown(i); 
}

template<class T>
bool MinHeap<T>::isLeaf(int pos) const
{
		return (pos>=CurrentSize/2)&&(pos<CurrentSize);
}

template<class T>
int MinHeap<T>::leftchild(int pos) const
{
	return 2*pos+1;						//返回左孩子位置
}

template<class T>
int MinHeap<T>::rightchild(int pos) const
{
	return 2*pos+2;						//返回右孩子位置
}

template<class T>
int MinHeap<T>::parent(int pos) const	// 返回父节点位置
{
	return (pos-1)/2;
}

template<class T>
void MinHeap<T>::SiftDown(int left)     //根，左孩子，右孩子找最小的作为根
{
	//准备
	int i=left;							//标识父结点
	int j=2*i+1;						//标识关键值较小的子结点，j指向左右孩子的较小者		
	T	temp=heapArray[i];				//temp就是需要siftdown的值，所以后面左右孩子的较小者恒与temp比较
	//过筛
	while(j<CurrentSize)
	{
		if((j<CurrentSize-1)&&(heapArray[j]>heapArray[j+1]))
			j++;						//j指向右子结点
		if(temp>heapArray[j])			//j指向左右孩子中较小的一个,恒与temp比较
		{
			heapArray[i]=heapArray[j];
			i=j;
			j=2*j+1;                    
		}
		else break;						//根比左右孩子都小，不必再下移
	}
	heapArray[i]=temp;					//heapArray[i]比自己左右孩子都小 或 i没有左孩子
}

template<class T>
void MinHeap<T>::SiftUp(int position) 
{//从position向上开始调整，使序列成为堆
	int temppos=position;
	T temp=heapArray[temppos];
	while((temppos>0)&&(heapArray[parent(temppos)]>temp))
	{
		heapArray[temppos]=heapArray[parent(temppos)];
		temppos=parent(temppos);
	}
	heapArray[temppos]=temp;
}

template<class T>
BOOL MinHeap<T>::Insert(const T& newNode)
{//向堆中插入一个结点
	if(CurrentSize==MaxSize)
		return FALSE;
	heapArray[CurrentSize]=newNode;
	SiftUp(CurrentSize);
	CurrentSize++;
}

template<class T>
T& MinHeap<T>::RemoveMin()
{
	if(CurrentSize==0)
	{
		AfxMessageBox("Can't Delete");

	}
	else
	{
		T temp=heapArray[0];					//取堆顶元素
		heapArray[0]=heapArray[CurrentSize-1];	//堆末元素上升至堆顶
		CurrentSize--;
		if(CurrentSize>1)
			SiftDown(0);						//从堆顶开始筛选
		return temp;
	}
}

template<class T>
bool MinHeap<T>::Remove(int pos, T& node)
{// 删除给定下标的元素
	if((pos<0)||(pos>=CurrentSize))
		return false;
	T temp=heapArray[pos];
	heapArray[pos]=heapArray[--CurrentSize];	//指定元素置于最后
	if(heapArray[pos] < heapArray[parent(pos)])
		SiftUp(pos);								//上升筛
	else SiftDown(pos);								//向下筛	
	node=temp;
	return true;
}

#endif // !defined(AFX_MINHEAP_H__6BC12E9A_B926_47C2_8ACF_AA4004A0546F__INCLUDED_)
