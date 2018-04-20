// Tree.h: interface for the Tree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREE_H__703FFCCE_C340_4198_98E7_00F503CF8261__INCLUDED_)
#define AFX_TREE_H__703FFCCE_C340_4198_98E7_00F503CF8261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TreeNode.h"
#include "DualTagTreeNode.h"
#include "LeftLinkTreeNode.h"
#include <queue>
#include <stack>

template <class T>
class Tree  
{
private:
	TreeNode<T>* root;								//树根结点
													//返回current父节点,由Parent调用
	TreeNode<T>* getParent(TreeNode<T>* root,TreeNode<T>* current);
	void DestroyNodes(TreeNode<T>*root);			//删除root为根的子树

public:
	Tree();											//构造函数
	Tree(DualTagTreeNode<T>* nodeArray,int count);	//构造函数,利用双标记位先根次序构造树
	Tree(LeftLinkTreeNode<T>* nodeArray,int count);	//构造函数,利用左链层次顺序构造树

	virtual ~Tree();								//析构函数
	TreeNode<T>* getRoot();							//返回树中的根结点
	void CreateRoot(const T& rootValue);			//创建树的根结点，使根结点元素的值为rootValue
	bool isEmpty();									//判断是否为空树，若是则返回true

	TreeNode<T>* Parent(TreeNode<T>* current);	    //返回current父节点
	TreeNode<T>* PrevSibling(TreeNode<T>* current); //返回current的前一个邻居结点，以广搜为基础
	TreeNode<T>* PrevSibling2(TreeNode<T>* current);//返回current的前一个邻居结点，以广搜为基础

	void DeleteSubTree(TreeNode<T>* subroot);		//释放以root为根的子树的所有结点

	void RootFirstTraverse(TreeNode<T>* root);		//先根深度优先周游树
	void RootLastTraverse(TreeNode<T>* root);		//后根深度优先周游树

	void WidthTraverse(TreeNode<T>* root);			//宽度优先周游树
	void WidthTraverse2(TreeNode<T>* root);			//宽度优先周游树:版本2
};

template <class T>
Tree<T>::Tree()
{//构造函数
	root=NULL;
}

//构造函数,利用双标记位先根次序构造左孩子有兄弟表示的树
template <class T>
Tree<T>::Tree(DualTagTreeNode<T>* nodeArray,int count)
{
	//使用STL中的stack
	using std::stack;
	stack<TreeNode<T>* > aStack;
	//准备建立根结点
	TreeNode<T>* pointer=new TreeNode<T>;
	root=pointer;
	//for循环一次只处理一个结点
	for(int i=0;i<count-1;i++)							//已经new了根结点，还剩count-1个结点
	{
		pointer->setValue(nodeArray[i].info);			//结点赋值
		if(nodeArray[i].rtag==0)						//右标记为0，有右兄弟的结点入栈
			aStack.push(pointer);
		else											//右标记为1，无右兄弟的结点的右兄弟设为NULL
			pointer->setSibling(NULL);

		TreeNode<T>* temppointer=new TreeNode<T>;		//准备下一个
		if(nodeArray[i].ltag==0)						//左标记为0，有左孩子
			pointer->setChild(temppointer);				//设置左孩子
		else											//左标记为1，无左孩子
		{
			pointer->setChild(NULL);					//左孩子指针设为NULL
			pointer=aStack.top();						//取栈顶元素
  			aStack.pop();		
			pointer->setSibling(temppointer);			//为栈顶元素设置兄弟结点
		}
		pointer=temppointer;
	}
	//处理最后一个结点
	pointer->setValue(nodeArray[count-1].info);
	pointer->setChild(NULL);
	pointer->setSibling(NULL);
}

//构造函数,利用左链层次顺序构造树
template <class T>
Tree<T>::Tree(LeftLinkTreeNode<T>* nodeArray,int count)
{//构造函数,利用左链层次顺序构造树
	using std::queue;							//使用STL队列
	queue<TreeNode<T>*> aQueue;

	//准备建立根结点
	TreeNode<T>* pointer=new TreeNode<T>;
	root=pointer;

	int currentIndex=-1;

	while(pointer||!aQueue.empty())
	{
		if(pointer)
		{
			currentIndex++;
			pointer->setValue(nodeArray[currentIndex].info);
			if(nodeArray[currentIndex].llink)
			{
				TreeNode<T>* leftpointer=new TreeNode<T>;				
				pointer->setChild(leftpointer);
				aQueue.push(leftpointer);
			} 
			else pointer->setChild(NULL);

			if(nodeArray[currentIndex].rtag==0)
			{
				TreeNode<T>* rightpointer=new TreeNode<T>;				
				pointer->setSibling(rightpointer);
			}
			else pointer->setSibling(NULL);

			pointer=pointer->RightSibling();//访问pointer的兄弟
		}
		else
		{
			pointer=aQueue.front();
			aQueue.pop();
		}
	}
}

template <class T>
Tree<T>::~Tree()
{//析构函数
	while(root)
		DeleteSubTree(root);
}

template <class T>
TreeNode<T>* Tree<T>::getRoot()
{//返回树中的根结点
		return root;
}

template <class T>
void Tree<T>::CreateRoot(const T& rootValue)
{//创建树的根结点，使根结点元素的值为rootValue
	if(!root)
		root=new TreeNode<T>(rootValue);
}

template <class T>
bool Tree<T>::isEmpty()
{//判断是否为空树，若是则返回true
	if(root)
		return false;
	return true;
}

template <class T>
void Tree<T>::RootFirstTraverse(TreeNode<T>* root)
{//先根深度优先周游树
	while(NULL!=root)
	{
		AfxMessageBox(root->Value());				//访问当前结点
		RootFirstTraverse(root->LeftMostChild());	//访问头一棵树树根的子树
		root=root->RightSibling();					//周游其他的树
	}
}

template <class T>
void Tree<T>::RootLastTraverse(TreeNode<T>* root)
{//后根深度优先周游树
	while(NULL!=root)
	{
		RootLastTraverse(root->LeftMostChild());	//访问头一棵树树根的子树
		AfxMessageBox(root->Value());				//访问当前结点
		root=root->RightSibling();					//周游其他的树
	}
}

template <class T>
void Tree<T>::WidthTraverse(TreeNode<T>* root)
{
	using std::queue;							//使用STL队列
	queue<TreeNode<T>*> aQueue;
	TreeNode<T>* pointer=root;
	if(pointer)
	{
		aQueue.push(pointer);
		while(!aQueue.empty())
		{
			pointer=aQueue.front();							//STL的front()只获得队首元素，未删除
			AfxMessageBox(pointer->Value());
			while(pointer->RightSibling())					//pointer有右兄弟
			{
				if(pointer->LeftMostChild())				//pointer的左孩子入队
					aQueue.push(pointer->LeftMostChild());	
				pointer=pointer->RightSibling();			//pointer右移
				AfxMessageBox(pointer->Value());			//访问pointer
			}
			if(pointer->LeftMostChild())					//pointer的最右边的兄弟的左孩子入队
				aQueue.push(pointer->LeftMostChild());
			aQueue.pop();									//pop()才能真正让队首元素出队
		}
	}
}

template <class T>
void Tree<T>::WidthTraverse2(TreeNode<T>* root)
{//宽度优先周游树:版本2
	using std::queue;							//使用STL队列
	queue<TreeNode<T>*> aQueue;
	TreeNode<T>* pointer=root;
	if(pointer)
	{
		while(NULL!=pointer)					//pointer的所有兄弟，从左至右依次入队
		{
			aQueue.push(pointer);
			pointer=pointer->RightSibling();
		}
		while(!aQueue.empty())			
		{
			pointer=aQueue.front();
			aQueue.pop();
			AfxMessageBox(pointer->Value());	//只在出队时访问
			pointer=pointer->LeftMostChild();	//把它的所有孩子入队
			while(pointer)
			{
				aQueue.push(pointer);
				pointer=pointer->RightSibling();
			}
		}
	}
}

template <class T>
TreeNode<T>* Tree<T>::PrevSibling(TreeNode<T>* current)
{//返回current的前一个邻居结点
	using std::queue;								//使用STL的队列
	queue<TreeNode<T>*> aQueue;
	TreeNode<T>* pointer=root;
	TreeNode<T>* prev=NULL;

	if(pointer)
	{
		aQueue.push(pointer);
		while(!aQueue.empty())
		{
			pointer=aQueue.front();				 	//取队列首结点
			if(pointer==current)
				return prev;
			while(pointer->RightSibling())			//结束时pointer指向一层中的最后一个结点
			{
				prev=pointer;
				pointer=pointer->pSibling;
				if(pointer==current)
					return prev;
				else 
				{
					if(prev->LeftMostChild())
						aQueue.push(prev->LeftMostChild());
				}
			}
			if(pointer->LeftMostChild())			//一层中的最后一个结点的左孩子入队
				aQueue.push(pointer->LeftMostChild());
			aQueue.pop();							//与aQueue.front()配套
			prev=NULL;								//prev指向一层中的倒数第二个结点，本层中的所有结点
		}											//都不等于current，故置prev为空，为下一层做准备
			
	}
	return NULL;
}

template <class T>
TreeNode<T>* Tree<T>::PrevSibling2(TreeNode<T>* current)
{//返回current的前一个邻居结点
	using std::queue;								//使用STL的队列
	queue<TreeNode<T>*> aQueue;
	TreeNode<T>* pointer=root;
	TreeNode<T>* prev=NULL;
	if((NULL==current)||(NULL==pointer)||(current==pointer))
		return NULL;
	while(pointer)
	{
		if(pointer==current)
			return prev;
		aQueue.push(pointer);
		prev=pointer;
		pointer=pointer->pSibling;
	}
	while(!aQueue.empty())
	{
		prev=NULL;
		pointer=aQueue.front();				 	//取队列首结点
		aQueue.pop();
		pointer=pointer->LeftMostChild();
		while(pointer)
		{
			if(pointer==current)
				return prev;
			aQueue.push(pointer);
			prev=pointer;
			pointer=pointer->pSibling;
		}
	}
	return NULL;
}

template <class T>
TreeNode<T>* Tree<T>::getParent(TreeNode<T>* root,TreeNode<T>* current)
{//返回current父节点,由Parent调用
	TreeNode<T>* temp;
	if(root==NULL)
		return NULL;
	//找到父结点
	if(root->LeftMostChild()==current)
		return root;
	//递归寻找父结点
	if((temp=getParent(root->LeftMostChild(),current))!=NULL)
		return temp;
	else return getParent(root->RightSibling(),current);	

}

template <class T>
TreeNode<T>* Tree<T>::Parent(TreeNode<T>* current)
{//返回current父节点
	TreeNode<T>* pointer=current;
	if(NULL!=pointer)
	{
		TreeNode<T>* leftmostChild=NULL;
		while((leftmostChild=PrevSibling(pointer))!=NULL)
			pointer=leftmostChild;
		leftmostChild=pointer;
		pointer=root;
		if(leftmostChild==root)
			return NULL;
		else return getParent(pointer,leftmostChild);
	}
}

template <class T>
void Tree<T>::DestroyNodes(TreeNode<T>* root)
{//删除root为根的子树的结点
	if(root)
	{
		DestroyNodes(root->LeftMostChild());		//递归删除root的以左子结点为根的子树
		DestroyNodes(root->RightSibling());		    //递归删除root的以右兄弟结点为根的子树
		delete root;								//删除根结点
	}

}

template <class T>
void Tree<T>::DeleteSubTree(TreeNode<T>* subroot)
{//释放以subroot为根的子树的所有结点
	TreeNode<T>* pointer=PrevSibling(subroot);
	if(NULL==pointer)
	{
		pointer=Parent(subroot);
		if(pointer)
		{
			pointer->pChild=subroot->RightSibling();
			subroot->pSibling=NULL;
		}
		else
		{
			root=subroot->RightSibling();
			subroot->pSibling=NULL;
			
		}
	}
	else
	{
		pointer->pSibling=subroot->RightSibling();
		subroot->pSibling=NULL;
	}
	DestroyNodes(subroot);
}

#endif // !defined(AFX_TREE_H__703FFCCE_C340_4198_98E7_00F503CF8261__INCLUDED_)
