//�Ż�����·�鲢����

#include "iostream.h"
#include "stdlib.h"
#include "string.h"
#include "Compare.h"
#include "ImprovedTwoWayMergeSorter.h"

// �趨�漴����������
inline void Randomize() 
  { srand(1); }

//����һ��0��n-1֮��������
inline int Random(int n)
  { return rand() % (n); }

void main()
{
	//�����������,����Ϊ100
	Randomize();	
	int * sortarray =new int[100];
	int * temparray =new int[100];
	for(int i=0;i<100;i++)
		sortarray[i]=Random(100);
	//ʵ�����Ż�����·�鲢������
	ImprovedTwoWayMergeSorter<int,Compare> sorter;
	//�������ǰ��������
	cout<<"����ǰ��";cout<<endl;
	sorter.PrintArray(sortarray,100);
	//����
	sorter.Sort(sortarray,temparray,0,99);
	//����������������
	cout<<"�����";cout<<endl;
	sorter.PrintArray(sortarray,100);
}