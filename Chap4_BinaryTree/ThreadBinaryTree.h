// ThreadBinaryTree.h: interface for the ThreadBinaryTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADBINARYTREE_H__73E5ADE5_F47C_463B_B199_91EE62DE38D6__INCLUDED_)
#define AFX_THREADBINARYTREE_H__73E5ADE5_F47C_463B_B199_91EE62DE38D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ThreadBinaryTreeNode.h"

template <class T>
class ThreadBinaryTree  
{
private:
	ThreadBinaryTreeNode<T>* root;						//根结点指针
public:
	ThreadBinaryTree(){	root=NULL;};					//构造函数
	virtual ~ThreadBinaryTree(){DeleteTree(root);};
	ThreadBinaryTreeNode<T>* getRoot(){return root;};	//返回根结点指针
	//以element作为根结点，leftTree作为树的左子树，rightTree作为树的右子树，构造一棵新的二叉树
	void CreateTree(const T& elem, ThreadBinaryTree<T>& leftTree, ThreadBinaryTree<T>& rightTree);
	//删除二叉树或其子树
	void DeleteTree(ThreadBinaryTreeNode<T>* root);
	//对称序线索化二叉树
	void InThread(ThreadBinaryTreeNode<T>* root);
	//递归对称序线索化二叉树
	void InThread(ThreadBinaryTreeNode<T>* root,ThreadBinaryTreeNode<T>* &pre);
	//对称序周游
	void InOrder(ThreadBinaryTreeNode<T>* root);
	//在对称序穿线树中找指定结点在前序下的后继
	ThreadBinaryTreeNode<T>* FindNextinInorderTree(ThreadBinaryTreeNode<T>* pointer);
	//往对称序穿线树里插入一个新结点
	void InsertNode(ThreadBinaryTreeNode<T>*pointer,ThreadBinaryTreeNode<T>* newpointer);
};

template <class T>
void ThreadBinaryTree<T>::CreateTree(const T& elem, ThreadBinaryTree<T>& leftTree, ThreadBinaryTree<T>& rightTree)
{
	root=new ThreadBinaryTreeNode<T>(elem,leftTree.root,0,rightTree.root,0);//创建新树
	leftTree.root=rightTree.root=NULL;							//原来两棵子树的根结点指空，避免访问

}


//删除二叉树或其子树
template <class T>
void ThreadBinaryTree<T>::DeleteTree(ThreadBinaryTreeNode<T>* root)
{
	if(root)
	{
		if(root->lTag==0)
			DeleteTree(root->left);
		if(root->rTag==0)
		DeleteTree(root->right);
		delete root;
	}
	else
		delete root;
}


//递归对称序线索化二叉树(对称序是中序LtR)
template <class T>
void ThreadBinaryTree<T>::InThread(ThreadBinaryTreeNode<T>* root,ThreadBinaryTreeNode<T>* &pre)
//通过递归中序遍历，线索化二叉树 root是根结点 pre是根的前驱结点
{
	if(root!=NULL)
	{
		//中序线索化左子树
		InThread(root->leftchild(),pre);
		if(root->leftchild()==NULL)					//根的左孩子空，则根的left存出根的前驱
		{
		//建立前驱线索
			root->left=pre;			
			root->lTag=1;
		}
		if((pre)&&(pre->rightchild()==NULL))		//前驱的右孩子空，则前驱的right存前驱的后继，即根
		{
		//建立后继线索
			pre->right=root;
			pre->rTag=1;
		}
		pre=root;
		//中序线索化右子树
		InThread(root->rightchild(),pre);
	}
}


//非递归对称序线索化二叉树(对称序是中序LtR)
template <class T>
void ThreadBinaryTree<T>::InThread(ThreadBinaryTreeNode<T>* root)
//非递归，中序线索化二叉树
{
	//初始化
	using std::stack;
	stack<ThreadBinaryTreeNode<T>*> aStack;
	ThreadBinaryTreeNode<T>* pointer=root;			//指向当前正在线索化的结点
	ThreadBinaryTreeNode<T>* temppointer=NULL;		//指向当前正在线索化的结点的前驱
	
	while(1)										//此处的双层while单条件和LtR的单层while双条件等价
	{
		while(NULL!=pointer)
		{
			aStack.push(pointer);
			pointer=pointer->leftchild();			//沿左子树方向下降
		}
		if(aStack.empty())							//栈空，算法结束
			return;
		else
		{
			pointer=aStack.top();					//中序周游，弹栈顺序就是访问顺序
			aStack.pop();
			//本次出栈的节点是紧邻的前一次出栈结点的后继
			//***********************建立线索***********************
			if(NULL!=temppointer)
			{
				if(NULL==temppointer->right)
				{
					temppointer->rTag=1;
					temppointer->right=pointer;		//确定中序后继
				};
				if(NULL==pointer->left)
				{
					pointer->lTag=1;
					pointer->left=temppointer;		//确定中序前驱
				}
			}
			temppointer=pointer;					//保存本次的后继，作为下一次的前驱
			//***********************建立线索***********************

			pointer=pointer->rightchild();			//转向二叉树的右子树
		}
	}
}


//对称序周游 可改成以一个for循环(对称序是中序LtR)
template <class T>
void ThreadBinaryTree<T>::InOrder(ThreadBinaryTreeNode<T>* root)
{
	ThreadBinaryTreeNode<T>* pointer;
	//是否为空二叉树
	if(root==NULL)
		return;
	else pointer=root;
	//找“最左下”结点
	while(pointer->leftchild()!=NULL)
		pointer=pointer->leftchild();
	//访问当前结点并找出当前结点的对称序后继
	while(1)
	{
		AfxMessageBox(pointer->value());			//访问当前结点
		if(pointer->rightchild()==NULL)
			return;
		if(pointer->rTag==1)
			pointer=pointer->rightchild();
		else
		{
			pointer=pointer->rightchild();
			while(pointer->lTag==0)
				pointer=pointer->leftchild();
		}
	}
}


//在对称序穿线树中找指定结点在前序下的后继(对称序是中序LtR)
template <class T>
ThreadBinaryTreeNode<T>* ThreadBinaryTree<T>::FindNextinInorderTree(ThreadBinaryTreeNode<T>* pointer)
{
	ThreadBinaryTreeNode<T>* temppointer=NULL;
	if(pointer->lTag==0)						//有左孩子
		return pointer->leftchild();			//返回左子树根
	else
		temppointer=pointer;			
	while(temppointer->rTag==1)
		temppointer=temppointer->rightchild();	//temppointer指向可索引的最后一个结点
	temppointer=temppointer->rightchild();		//的右孩子
	return temppointer;							//返回temppointer
}


//往对称序穿线树里插入一个新结点(对称序是中序LtR)
template <class T>
void ThreadBinaryTree<T>::InsertNode(ThreadBinaryTreeNode<T>*pointer,ThreadBinaryTreeNode<T>* newpointer)
{
	ThreadBinaryTreeNode<T>* temppointer=NULL;
	//找指定结点的对称序后继
	if(pointer->rightchild()==NULL)
		temppointer=NULL;
	else if(pointer->rTag==1)
			temppointer=pointer->rightchild();
		 else
		 {
			temppointer=pointer->rightchild();
			while(temppointer->lTag==0)
				temppointer=temppointer->leftchild();
		 }
	//temppointer指针指向pointer结点的对称序后继
	//建立指定结点的对称序后继的左线索
	if((temppointer!=NULL)&&(temppointer->lTag==1))
		temppointer->left=newpointer;
	//建立新结点的右指针或右线索
	newpointer->rTag=pointer->rTag;
	newpointer->right=pointer->rightchild();
	//插入新结点
	pointer->rTag=0;
	pointer->right=newpointer;
	//建立新结点左线索
	newpointer->lTag=1;
	newpointer->left=pointer;
}

#endif // !defined(AFX_THREADBINARYTREE_H__73E5ADE5_F47C_463B_B199_91EE62DE38D6__INCLUDED_)
