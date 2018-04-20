#define UNVISITED 0
#define VISITED 1
#define INFINITY 9999999
#define ROOT -1

#include <iostream>
#include <fstream>
#include "LList.h"
#include "minheap.h"
#include <queue>
using namespace std;

//���ݽṹ����:

/**************** ͼ�ıߵĶ��� ***************/
class Edge
{
public:
	int from,to,weight;    //from�Ǳߵ�ʼ��,to�Ǳߵ��յ�,weight�Ǳߵ�Ȩ

	Edge()                 //���캯��
	{
		from=-1;
		to=-1;
		weight=0;
	}

	Edge(int f,int t,int w)  //���캯��
	{
		from=f;
		to=t;
		weight=w;
	}

	bool operator >(Edge oneEdge)   //����Ƚ������>,�ߵĴ�С�Ƚϼ�Ϊ�ߵ�Ȩ�Ĵ�С�Ƚ�
	{
		return weight > oneEdge.weight;
	}

	bool operator <(Edge oneEdge)  //����Ƚ������<,�ߵĴ�С�Ƚϼ�Ϊ�ߵ�Ȩ�Ĵ�С�Ƚ�
	{
		return weight < oneEdge.weight;
	}
};

/**************** ͼ�Ķ��� ***************/
//ע�⣺�����ݽṹ������ͼ������ͼͳһ��������ͼ�е�һ���߽��൱������ͼ�е�������
class Graph
{
public:
	int numVertex;	//ͼ�Ķ���ĸ���
	int numEdge;    //ͼ�ıߵĸ���
	int *Mark;      //Markָ��ָ�򱣴���ͼ�Ķ���ı�־λ������,��־λ�������ĳ�����Ƿ񱻷��ʹ�
	int *Indegree;  //Indegreeָ��ָ�򱣴���ͼ�Ķ������ȵ�����

	Graph(int numVert)               //���캯��
	{
		numVertex=numVert;           //ȷ��ͼ�Ķ���ĸ���
		numEdge=0;                   //ȷ��ͼ�ı����ĸ���
		Indegree=new int[numVertex]; //Ϊ����ͼ�Ķ���������������,IndegreeΪ����ָ��
		Mark=new int[numVertex];     //Ϊͼ�Ķ���ı�־λ��������,MarkΪ����ָ��
		for(int i=0;i<numVertex;i++) //ȷ��ͼ�Ķ���ı�־λ�����,�����ж���ı�־λ��ʼ��Ϊδ�����ʹ�,��ȳ�ʼ��Ϊ0
		{
			Mark[i]=UNVISITED;
			Indegree[i]=0;
		}
	}

	~Graph()                         //��������
	{
		delete [] Mark;              //�ͷ�Mark����
		delete [] Indegree;          //�ͷ�Indegree����
	}

	virtual Edge FirstEdge(int oneVertex)=0;  //�����붥��oneVertex������ĵ�һ����
    virtual Edge NextEdge(Edge preEdge)=0;    //�������PreEdge����ͬ��������oneVertex����һ����

    int VerticesNum()              //����ͼ�Ķ������
	{
		return numVertex;
	}

    int EdgesNum()                 //����ͼ�ı���
	{
		return numEdge;
	}

    bool IsEdge(Edge oneEdge)      //���oneEdge�Ǳ��򷵻�TRUE�����򷵻�FALSE
	{
		if(oneEdge.weight>0&&oneEdge.weight<INFINITY&&oneEdge.to>=0)	//Edge�޲ι��캯����to����Ϊ-1.�洢ͼʱÿ���ߵ�from��to�����ڵ���0
			return true;
		else
			return false;
	}

	int FromVertex(Edge oneEdge)   //���ر�oneEdge��ʼ��
	{
		return oneEdge.from;
	}

	int ToVertex(Edge oneEdge)     //���ر�oneEdge���յ�
	{
		return oneEdge.to;
	}

	int Weight(Edge oneEdge)	   //���ر�oneEdge��Ȩ
	{
		return oneEdge.weight;
	}
	virtual void setEdge(int from,int to,int weight)=0;
	virtual void delEdge(int from,int to)=0;
};


/**************** ���ھ���ʽʵ�ֵ�ͼ ***************/
class Graphm: public Graph
{
private:
	int **matrix;           //ָ�����ھ����ָ��

public:

	Graphm(int numVert):Graph(numVert)    //���캯��
	{
		int i,j;                          //i,j������Ϊforѭ���еļ�����

		matrix=(int**)new int*[numVertex];     //����matrix����,��������numVertex��Ԫ��,ÿ��Ԫ��������ָ������
		for(i=0;i<numVertex;i++)               //matrix�����ÿ��Ԫ��,��ָ��һ������numVertex��Ԫ�ص�����
			matrix[i] = new int[numVertex];
		for(i=0;i<numVertex;i++)               //���ھ��������Ԫ�ض���ʼ��Ϊ0,�������Ԫ��matrix[i][j]��Ϊ0,���������i�붥��j֮����һ����,�ߵ�Ȩ��Ϊmatrix[i][j]��ֵ
			for(j=0;j<numVertex;j++)
				matrix[i][j]=0;
	}

	~Graphm()                        //��������
	{
		for(int i=0;i<numVertex;i++) //�ͷ�ÿ��matrix[i]����Ŀռ�
			delete [] matrix[i];
		delete [] matrix;            //�ͷ�matrixָ��ָ��Ŀռ�
	}

	Edge FirstEdge(int oneVertex)    //���ض���oneVertex�ĵ�һ����
	{
		Edge myEdge;                 //��myEdge����Ϊ�����ķ���ֵ
		myEdge.from=oneVertex;       //������oneVertex��Ϊ��myEdge��ʼ��
		for(int i=0;i<numVertex;i++) //Ѱ�ҵ�һ��ʹ��matrix[oneVertex][i]��Ϊ0��iֵ,��ô(oneVertex,i)����<oneVertex,i>��Ϊ����oneVertex�ĵ�һ����
		{
			if(matrix[oneVertex][i]!=0)
			{
				myEdge.to=i;         //������i��Ϊ��myEdge���յ�
				myEdge.weight=matrix[oneVertex][i];   //��myEdge��ȨΪ����Ԫ��matrix[oneVertex][i]��ֵ
				break;
			}
		}
		return myEdge;               //����ҵ��˶���oneVertex�ĵ�һ����,�򷵻ص�myEdgeȷʵ��һ����;���û���ҵ�����oneVertex�ĵ�һ����,��myEdge�ĳ�Ա����toΪ-1,����IsEdge�����жϿ�֪myEdge����һ����
	}

	Edge NextEdge(Edge preEdge)      //�������PreEdge����ͬ��������oneVertex����һ����
	{
		Edge myEdge;                 //��myEdge����Ϊ�����ķ���ֵ
		myEdge.from=preEdge.from;    //����myEdge��ʼ����Ϊ����һ����preEdge��ʼ����ͬ

		for(int i=preEdge.to+1;i<numVertex;i++) //Ѱ����һ��ʹ��matrix[preEdge.from][i]��Ϊ0��iֵ,��ô(preEdge.from,i)����<preEdge.from,i>��Ϊ����preEdge.from����һ����
		{
			if(matrix[preEdge.from][i]!=0)
			{
				myEdge.to=i;
				myEdge.weight=matrix[preEdge.from][i];
				break;
			}
		}

		return myEdge;                //����ҵ��˶���preEdge.from����һ����,�򷵻ص�myEdgeȷʵ��һ����;���û���ҵ�����preEdge.from����һ����,��myEdge�ĳ�Ա����toΪ-1,����IsEdge�����жϿ�֪myEdge����һ����
	}

	void setEdge(int from,int to,int weight)      //Ϊͼ�趨һ����
	{
		if(matrix[from][to]==0)                   //���matrix[from][to]==0�����(from,to)����<from,to>����������һ���ߣ�����ñ��Ѿ����ڣ�����ֻ�ǶԸñ߽����޸ģ�
		{
			numEdge++;
			Indegree[to]++;
		}
		matrix[from][to]=weight;
	}

	void delEdge(int from,int to)               //ɾ��ͼ��һ����
	{
		if(matrix[from][to]!=0)					//���matrix[from][to]!=0�����(from,to)����<from,to>ȷʵ���ڣ�����ñ�ʵ���ϲ������ڣ��Ӷ����ض�ͼ�ı����Ͷ���to����Ƚ����޸ģ�
		{
			numEdge--;
			Indegree[to]--;
		}
		matrix[from][to]=0;
	}
};


/**************** �ڽӱ�ʽʵ�ֵ�ͼ ***************/
struct listUnit      //�ڽӱ��Ŀ�����ݲ��ֵĽṹ����
{
	int vertex;      //�ߵ��յ�
	int weight;      //�ߵ�Ȩ
};


class Graphl: public Graph
{
	//friend class Graphdup;       //Graphdup���������ǽ����۵��ڽӶ��ر��ʵ�ַ�ʽ

private:
	LList<listUnit> *graList;    //graList�Ǳ������б߱������

public:
	Graphl(int numVert):Graph(numVert)          //���캯��
	{
		graList=new LList<listUnit>[numVertex]; //ΪgraList��������ռ䣬ͼ��numVertex�����㣬����numVertex���߱�
	}

	~Graphl()                                   //��������
	{
		delete [] graList;						//�ͷſռ�
	}

	Edge FirstEdge(int oneVertex)              //���ض���oneVertex�ĵ�һ����
	{
		Edge myEdge;                           //��myEdge����Ϊ�����ķ���ֵ
		myEdge.from=oneVertex;                 //������oneVertex��Ϊ��myEdge��ʼ��
		Link<listUnit> *temp=graList[oneVertex].head;  //graList[oneVertex].head������Ƕ���oneVertex�ı߱�temp->nextָ�򶥵�oneVertex�ĵ�һ����(���temp->next��Ϊnull)
		if(temp->next!=NULL)                   //�������oneVertex�ĵ�һ����ȷʵ����
		{
			myEdge.to=temp->next->element.vertex;
			myEdge.weight=temp->next->element.weight;
		}
		return myEdge;	                      //����ҵ��˶���oneVertex�ĵ�һ����,�򷵻ص�myEdgeȷʵ��һ����;���û���ҵ�����oneVertex�ĵ�һ����,��myEdge�ĳ�Ա����toΪ-1,����IsEdge�����жϿ�֪myEdge����һ����
	}

	Edge NextEdge(Edge preEdge)               //�������PreEdge����ͬ��������oneVertex����һ����
	{
		Edge myEdge;                          //��myEdge����Ϊ�����ķ���ֵ
		myEdge.from=preEdge.from;             //����myEdge��ʼ����Ϊ����һ����preEdge��ʼ����ͬ
		Link<listUnit> *temp=graList[preEdge.from].head;           //graList[oneVertex].head������Ƕ���oneVertex�ı߱�temp->nextָ�򶥵�oneVertex�ĵ�һ����(���temp->next��Ϊnull)
		while(temp->next!=NULL&&temp->next->element.vertex<=preEdge.to)  //ȷ����preEdge�ڱ߱��е�λ��,�����preEdge����һ����ȷʵ���ڣ���temp->nextָ��ָ����һ���ߵı�Ŀ
			temp=temp->next;
		if(temp->next!=NULL)                 //��preEdge����һ���ߴ���
		{
			myEdge.to=temp->next->element.vertex;
			myEdge.weight=temp->next->element.weight;
		}
		return myEdge;
	}

	void setEdge(int from,int to,int weight)   //Ϊͼ�趨һ����
	{
		Link<listUnit> *temp=graList[from].head;  //graList[from].head������Ƕ���from�ı߱�temp->nextָ�򶥵�from�ĵ�һ����(���temp->next��Ϊnull)
		while(temp->next!=NULL&&temp->next->element.vertex<to)   //ȷ����(from,to)��<from,to>�ڱ߱��е�λ��,���������,���(from,to)��<from,to>Ϊ�¼ӵ�һ����
			temp=temp->next;
		if(temp->next==NULL)                //��(from,to)��<from,to>�ڱ߱��в��������ڱ߱����������������,���ڱ߱��м���������
		{
			temp->next=new Link<listUnit>;
			temp->next->element.vertex=to;
			temp->next->element.weight=weight;
			numEdge++;
			Indegree[to]++;
			return;
		}
		if(temp->next->element.vertex==to) //��(from,to)��<from,to>�ڱ߱����Ѵ���,��ֻ��Ҫ�ı�ߵ�Ȩֵ
		{
            temp->next->element.weight=weight;
			return;
		}
		if(temp->next->element.vertex>to) //��(from,to)��<from,to>�ڱ߱��в�����,���ڱ߱���������������,���ڱ߱��в���������
		{
			Link<listUnit> *other=temp->next;
			temp->next=new Link<listUnit>;
			temp->next->element.vertex=to;
			temp->next->element.weight=weight;
			temp->next->next=other;
			numEdge++;
			Indegree[to]++;
		}
	}

	void delEdge(int from,int to)      //ɾ��ͼ��һ����
	{
		Link<listUnit> *temp=graList[from].head;      //graList[from].head������Ƕ���from�ı߱�temp->nextָ�򶥵�from�ĵ�һ����(���temp->next��Ϊnull)
		while(temp->next!=NULL&&temp->next->element.vertex<to)   //ȷ����(from,to)��<from,to>�ڱ߱��е�λ��(����ñߴ���)
			temp=temp->next;
		if(temp->next==NULL) return;   //��(from,to)��<from,to>�ڱ߱��в�����,����Ҫ���κβ���
		if(temp->next->element.vertex==to)  //��(from,to)��<from,to>�ڱ߱��д�����ȷ���˸ñ��ڱ߱��е�λ��,��ӱ߱��н���ɾ��
		{
			Link<listUnit> *other=temp->next->next;
			delete temp->next;
			temp->next=other;
			numEdge--;
			Indegree[to]--;
		}
	}
};


//�㷨����:


//����������Σ���һ���㿪ʼ���Σ���
void DFS(Graph& G, int V)
{
	G.Mark[V]= VISITED;                           //��Ǹõ�
	cout<<V<<"\t";								  //��ӡ
	for(Edge e=G.FirstEdge(V);G.IsEdge(e);e=G.NextEdge(e))  //�ɸõ������ı߽��������������
	{

		if(G.Mark[G.ToVertex(e)]==UNVISITED)               //����V�ڽӵ���δ�����ʹ��Ķ��㣬���ݹ�ذ���������ȵķ�ʽ��������
			DFS(G, G.ToVertex(e));
	}
}

//����������Σ���һ���㿪ʼ���Σ���
void BFS(Graph& G, int V)
{
	using std::queue;
	queue<int> Q;                   //��ʼ�������������Ҫ�õ��Ķ���

	G.Mark[V]= VISITED;             //���ʶ���V����������־λ
	cout<<V<<"\t";                  //��ӡ
	Q.push(V);                      //V���

	while(!Q.empty())               //���������Ȼ��Ԫ��
	{
		int V = Q.front();		    //����Ԫ��
		Q.pop();                    //����

		for(Edge e=G.FirstEdge(V);G.IsEdge(e);e=G.NextEdge(e))    //����õ����ڵ�ÿһ��δ���ʵ㶼���
		{
			if(G.Mark[G.ToVertex(e)]== UNVISITED)   //����V�ڽӵ�������δ�����ʹ��Ķ���
			{
				G.Mark[G.ToVertex(e)]= VISITED;     //���ʶ���V����������־λ
	        	cout<<G.ToVertex(e)<<"\t";          //��ӡ
				Q.push(G.ToVertex(e));              //���
			}
		}
	}
}

//ͼ���Σ�
void graph_traverse(Graph& G,bool useDFS)
{
	for(int i=0;i<G.VerticesNum();i++)  //��ͼ���ж���ı�־λ���г�ʼ��
		G.Mark[i]=UNVISITED;
	for(int i=0;i<G.VerticesNum();i++)      //���ͼ�����ж����Ƿ񱻱�ǹ������δ����ǣ���Ӹ�δ����ǵĶ��㿪ʼ��������
    {
		if(G.Mark[i]== UNVISITED)
		{
			if(useDFS)
				DFS(G,i);       //�����������
			else
				BFS(G,i);       //�����������
		}
	}
	cout<<endl;
}

//���з�ʽʵ�ֵ���������
void TopsortbyQueue(Graph& G)
{
	for(int i=0;i<G.VerticesNum();i++)     //��ʼ��Mark����
		G.Mark[i]=UNVISITED;

    using std::queue;
	queue<int> Q;                          //��ʼ������

	for(int i=0; i<G.VerticesNum(); i++)       //ͼ�����Ϊ0�Ķ������
	{
		if(G.Indegree[i]==0)
		{
			Q.push(i);
		}
	}

	while(!Q.empty())                 //��������л���ͼ�Ķ���
	{
		int V=Q.front();
		cout<<V<<"\t";               //��ӡ����ö���
		G.Mark[V]=VISITED;
		Q.pop();                     //һ���������

		for(Edge e= G.FirstEdge(V);G.IsEdge(e);e=G.NextEdge(e))  //������֮���ڵĵ����-1
		{
			G.Indegree[G.ToVertex(e)]--;   //��e���յ�����ֵ��1
			if(G.Indegree[G.ToVertex(e)]==0)
			{
				Q.push(G.ToVertex(e));    //���Ϊ0�Ķ������
			}
		}
	}

	cout<<endl;

	for(int i=0; i<G.VerticesNum(); i++)
	{
      if(G.Mark[i]==UNVISITED)
	  {
	    cout<<"��ͼ�л���"<<endl;        //ͼ�л�
        break;
      }
	}
}


void Do_topsort(Graph& G, int V,int *result,int& tag)    //�������������ʽʵ����������
{
	G.Mark[V]= VISITED;
	for(Edge e= G.FirstEdge(V);G.IsEdge(e);e=G.NextEdge(e))
    {
		if(G.Mark[G.ToVertex(e)]== UNVISITED)            //����V�ڽӵ�������δ�����ʹ��Ķ���
			Do_topsort(G, G.ToVertex(e),result,tag);     //�ݹ����
	}
	result[tag++]=V;
}

void TopsortbyDFS(Graph& G)               //�������������ʽʵ�ֵ���������,����ǵߵ���
{
	for(int i=0; i<G.VerticesNum(); i++)  //��ͼ���ж���ı�־λ���г�ʼ��
		G.Mark[i]=UNVISITED;

	int *result=new int[G.VerticesNum()];
	int tag=0;
	for(int i=0; i<G.VerticesNum(); i++)      //��ͼ�� !!!���ж���!!! ���д���
		if(G.Mark[i]== UNVISITED)			  //ֻ�иö���û�б�����ʱ��DFS
			Do_topsort(G,i,result,tag);   //���õݹ麯��

	for(int i=G.VerticesNum()-1;i>=0;i--)     //�������
	{
		cout<<result[i]<<"\t";
	}
	cout<<endl;
}

//Dijkstra�㷨

//����С�ѵķ���ʵ��ʵ��Ѱ��������������С�ĵ�Ĺ���
//Ϊ����minheap�������ر��DijElem��
class DijElem
{
public:
	int vertex;
	int distance;

	DijElem(int v=-1,int d=INFINITY)
	{
		vertex=v;
		distance=d;
	}

	bool operator<(const DijElem &ele) const	//overload > �� < ������minheap��siftdown()��siftup()
	{
		return distance<ele.distance;
	}

	bool operator>(const DijElem &ele) const
	{
		return distance>ele.distance;
	}

	bool operator==(const DijElem &ele) const
	{
		return distance==ele.distance;
	}
};

//D�ĳ�ʼֵΪD[s]=0, D[else]=INFINITY,��row 836~838
void Dijkstra(Graph& G,int *D,int s)
{
	int v;
	for(int i=0;i<G.VerticesNum();i++)           //��ʼ��Mark����
		G.Mark[i]=UNVISITED;

	//����С�ѷ�ʽʵ���Ҿ�����С�Ķ���
	DijElem *E=new DijElem[G.VerticesNum()];	//��С�ѵ�heapArray[]
	DijElem temp;								//���
	temp.vertex=s;								//��Դ���·�������s
	temp.distance=0;							//���s���Լ������·��Ϊ0
	E[0]=temp;
	minheap<DijElem> H(E,1,G.VerticesNum());	//���ù��캯������ʼ��һ����С��
												//�˴�ʹ�õ���С�ѣ�����һ���ⲿ������E��Ϊ��heapArray[]
	for(int i=0;i<G.VerticesNum();i++)			//��G.VerticesNum()����㣬һ����һ������ѭ��G.VerticesNum()��
	{
		//VISITED��ʾ����S���ϣ�UNVISITED��ʾ����V-S����
		//����Ҫ�ҳ�V-S�����еģ�������Сdistance�Ľ��
		do
		{
			if(H.isempty())
				return;
			temp=H.removemin();					//��֤temp�Ǿ�����Сdistance�Ľ��
			v=temp.vertex;
		}while(G.Mark[v]==VISITED);				//��֤v����V-S���ϡ���v��S�����еģ�������Ƴ�minheap�е�min		 


		if(D[v]==INFINITY)						//����Դ������������INFINITY���㷨����
			break;
		G.Mark[v]=VISITED;                       //ֻ�ڴ�minheap����ʱ�������ѷ�����S,��row 509
		cout<<v<<"\t";                           //���
		for(Edge e=G.FirstEdge(v);G.IsEdge(e);e=G.NextEdge(e)) //ˢ��D�е�ֵ����Ϊv�ļ��룬D��ֵ��Ҫ�ı䣬ֻҪˢ����v���ڵĵ��ֵ
		{
			if(D[G.ToVertex(e)]>(D[v]+G.Weight(e)))			   //ÿ��ֻ�ѵ�Դ����뱻��С�Ľ�����minheap
			{												   //���ڳ�ʼ��D[]�г�Դ����ȫΪINFINITY,�ʵ�һ���ɳ�ʱ��Դ��ɴ�Ľ�㶼������minheap
				D[G.ToVertex(e)]=D[v]+G.Weight(e);
				temp.vertex=G.ToVertex(e);
				temp.distance=D[G.ToVertex(e)];
				H.insert(temp);					 //ֻ����minheap������û�з��ʡ���row 500
			}
		}
	}
}

//ÿ�Զ���֮�����̾��룬��Floyd�㷨ʵ��
void Floyd(Graph& G,int **D)
{

	int i,j,v;                 //i,j�Ǽ�������v��¼��Ӧ����

	//��ʼ��D����
	for(i=0;i<G.VerticesNum();i++)				//��ʼ����һ�����洢0��INFINITY
	{
		for(j=0;j<G.VerticesNum();j++)
		{
			if(i==j)
				D[i][j]=0;
			else
				D[i][j]=INFINITY;
		}
	}

	//D=adj[0]�����ߵ�ֵ����D����
	//����G�Ĵ洢��ʽ�����֣����Բ���ֱ����matirx[][]��ֵ�������ִ洢��ʽ���еĳ�Ա������D[][]��ֵ
	for(v=0;v<G.VerticesNum();v++)				//��ʼ���ڶ������洢ͼ�����еı�.��ɺ�D[][]��adj(0)
	{
		for(Edge e=G.FirstEdge(v);G.IsEdge(e);e=G.NextEdge(e))
		{
			D[v][e.to]=G.Weight(e);				//��Kruskal�Աȡ�Floydͬһ���ߴ洢�����Σ�Kruskal�洢һ��
		}
	}

	//����������������·����������v������D[i][j]>(D[i][v]+D[v][j])
	//ͨ����v��ѭ�����൱�ڽ�ͼһ������һ�����������
	for(v=0;v<G.VerticesNum();v++)						//v��ֵ����ڵ�����adj(k)��k-1
		for(i=0;i<G.VerticesNum();i++)					//i��j���ɽ��ԣ���n^2������
			for(j=0;j<G.VerticesNum();j++)				//ÿ�Խ�㶼������vΪ��ת��㵽��Է�
				if(D[i][j]>(D[i][v]+D[v][j]))			//��һ��ѭ��ʱ��v����0,�õ�adj(1)
					D[i][j]=D[i][v]+D[v][j];			//���տ��н��ĸ�����ȷ��С�ڵȻ�����С��
														//���н���1��ţ�С�ڵ���k�����ý�㹲k��
	for(i=0;i<G.VerticesNum();i++)						//�������0��ţ�С��k�����ý�㹲k��
	{
		for(j=0;j<G.VerticesNum();j++)
			cout<<D[i][j]<<"\t";
		cout<<endl;
	}
}


//��С֧�������㷨

//��ӱ�
void AddEdgetoMST(int i,int j,Edge *MST,int tag)
{
	MST[tag].from=i;
	MST[tag].to=j;
}

//��С֧������Prim�㷨,Ѱ����һ��Ȩ��С�ı߲�����С�ѵķ�ʽ
//ֻ��������ͨ����ͼ
void Prim(Graph& G, int s)
{
	int MSTtag=0;                            //��С֧�����ߵı��
	Edge *MST=new Edge[G.VerticesNum()-1];   //�洢��С֧�����ı�,e = n - 1

	int Etag=0;
	Edge *E=new Edge[G.EdgesNum()];			 //��С�ѵ�����

	for(int i=0;i<G.VerticesNum();i++)       //��ʼ��Mark����
	{
		G.Mark[i]=UNVISITED;
	}
	G.Mark[s]=VISITED;

	for(Edge e=G.FirstEdge(s);G.IsEdge(e);e=G.NextEdge(e))
	{
		E[Etag++]=e;
	}

	minheap<Edge> H(E,Etag,G.EdgesNum());

	for(int i=1; i<G.VerticesNum(); i++)					//��V-S��(n-1)�����㣬ѭ��(n-1)��,1��(n-1)
	{
        //Ѱ��Ȩ��С�ı�,��S�м���G.VerticesNum()-1���ڵ㣬������G.Mark[temp.to] = VISITED.
		//��ѭ����һ�Σ�������С�ѿյ�ʱ���return
		if(H.isempty())
		{
			cout<<"��������С֧������";
			return;
		}
		Edge temp=H.removemin();

		if(G.Mark[temp.to]==UNVISITED)						//��V-S�ҵ�һ�㣬�ҵ�����MST�ıߣ���Ч���
		{
			AddEdgetoMST(temp.from,temp.to,MST,MSTtag++);	//ֻ����Ч�����MST[]�����ΪVISITED
			G.Mark[temp.to]=VISITED;						//�����뼯��S��
			for(Edge e=G.FirstEdge(temp.to);G.IsEdge(e);e=G.NextEdge(e))
			{
				if(G.Mark[e.to]==VISITED)
					continue;
				H.insert(e);								//��������ӽ����δ���ʽ��ı����
			}
		}
		else												//δ�ҵ�����MST�ıߣ���Ч����i--
			i--;
	}

	//���
	for(int i=0;i<G.VerticesNum()-1;i++)
		cout<<MST[i].from<<"-"<<MST[i].to<<"\t";
	cout<<endl;
}

//Kruskal�㷨
//�á���ָ�롱����ʾ�ȼ������
class Gentree
{
private:
	int* array;             //Ҷ�������
	int size;               //Ҷ���Ĵ�С
public:
	Gentree(int sz)         //���캯��
	{
		size=sz;
		array=new int[sz];
		for(int i=0;i<sz;i++)	//ÿ����㶼��������һ��������ÿ��array[i]�ж��洢ROOT
			array[i]=ROOT;  //ROOT��ʾ�������ĸ����
	}
	~Gentree()           //�����������ͷſռ�
	{
		delete [] array;
	}
	int FIND(int curr)   //Ѱ�Ҹ��ĺ���		
	{										//i�ǽ���ţ�array[i]��i�ĸ������
		while(array[curr]!=ROOT)			//�����±��ʾ����ţ��±�Ϊi��ʾi�Ž��
			curr=array[curr];				//array[i]��i�Ž��ĸ����ı��
		return curr;
	}

	void UNION(int a,int b)     //��a��b�ϲ���һ���ȼ����У�a��b��һ���ȼ����о�����������ͬ�ĸ�
	{
		int root1=FIND(a);
		int root2=FIND(b);
		if(root1!=root2)
			array[root2]=root1;		//Ϊb���ĸ�������һ��������b���ĸ�����Ϊa���ĸ�������
	}

	bool differ(int a,int b)    //�ж�a��b�Ƿ񲻵ȼ�
	{
		int root1=FIND(a);
		int root2=FIND(b);
		return root1!=root2;
	}
};

//��С֧������Kruskal�㷨,ֻ��������ͨ����ͼ
void Kruskal(Graph& G)
{
	Gentree A(G.VerticesNum());            //�ȼ���,���캯����ʼ��ÿ����㶼�Ǹ���������ÿ��Ԫ�ش�Ķ���ROOT
    Edge *E=new Edge[G.EdgesNum()];        //��¼ͼ�����б�
	Edge *MST=new Edge[G.VerticesNum()-1]; //��С֧����,G��n�����㣬MST����n-1����
	int MSTtag=0;
	int edgecount=0;

    for(int i=0; i<G.VerticesNum(); i++)    //��ͼ�����б߼�¼������E��
	{
		for(Edge e= G.FirstEdge(i);G.IsEdge(e);e=G.NextEdge(e))	
		{
			if(G.FromVertex(e)< G.ToVertex(e))		//i��0��ʼ��������������ͼ��ͬһ���ߴ洢����
				E[edgecount++]=e;
		}
	}

	edgecount--;                                    //�õ�ͼ�ı���
	minheap<Edge> H(E,edgecount,edgecount);        //��С�ѣ�min-heap��

	int EquNum=G.VerticesNum();  //��ʼʱ��|V|���ȼ���
    for(int i=0; EquNum>1;i++)      //�ϲ�G.VerticesNum()���ȼ��࣬��ҪG.VerticesNum()-1��
	{
		Edge e=H.removemin();       //�����һ��Ȩ��С�ı�
		if(e.weight>=INFINITY)
		{
			cout<<"��������С֧����."<<endl;
			return;
		}
        int from=G.FromVertex(e);   //��¼�����ߵ���Ϣ
        int to= G.ToVertex(e);
        if(A.differ(from,to))       //�����e���������㲻��һ���ȼ���
		{
			A.UNION(from,to);       //����e�������������ڵ������ȼ���ϲ�Ϊһ��
			AddEdgetoMST(from,to,MST,MSTtag++); //����e�ӵ�MST
			EquNum--;             //���ȼ���ĸ�����1
		}
	}

	for(int i=0;i<G.VerticesNum()-1;i++)
		cout<<MST[i].from<<"-"<<MST[i].to<<"\t";
	cout<<endl;
}


int main()
{
	//���û���ѡ��ʵ�ֹ���
	cout<<"��������������һ��������ͼ������������"<<endl;
	//��ȡ��ͼ��ʽ
	cout<<"��ѡ��ͼ��ʽ��"<<endl;
	cout<<"(1)���ھ���"<<endl;
	cout<<"(2)�ڽӱ�"<<endl;

	int choice;
	cout<<"���ѡ����(���磬����1��Ȼ��س�������ʾ�����ھ���Ĺ�ͼ��ʽ)��";
	cin>>choice;
	if(choice>2||choice<1)
	{
		cout<<"��������룡"<<endl;
		return 0;
	}
	cout<<endl;

	int isDirected;                        //����Ƿ�����ͼ
	int numVertex;                         //ͼ�Ķ����������������setEdge�б��Զ��޸ģ�
	int from,to,weight;                    //����ÿ���ߵ���㣬�յ��Ȩ
	ifstream GraphSou;                     //�����ļ���
	//�ļ���ʽ������ȷ���󣬷����޷�����
	cout<<"�����빹ͼ�ļ������磬����a.txt��Ȼ��س�����ȷ����ͼ�ļ�����ȷ�ĸ�ʽ���ɲμ�ͬĿ¼���ļ�a.txt�ĸ�ʽ��˵���ļ�readme.txt��:";
	char filename[20];
	cin>>filename;                         //��ȡ�ļ���

	GraphSou.open(filename);
	GraphSou>>isDirected;                  //�Ƿ�����
	if(isDirected!=1&&isDirected!=0)
	{
		cout<<"�ļ���ʽ����ȷ�����������롣"<<endl;
		return 0;
	}

	GraphSou>>numVertex;                   //�������
	Graph *myGra;                          //ͼ����
	if(choice==1)                          //���ھ���
		myGra=new Graphm(numVertex);
	else                                   //��Ϊ�ڽӶ��ر���ͨ���ڽӱ�����ʼ����
		myGra=new Graphl(numVertex);

	while(!GraphSou.eof())                 //˳�ζ�ȡ�ߵ���Ϣ
	{
		GraphSou>>from>>to>>weight;
		if(from>=0&&to>=0&&weight>0&&from<numVertex&&to<numVertex)
		{
			myGra->setEdge(from,to,weight);
			if(!isDirected)
				myGra->setEdge(to,from,weight);
		}
		else
		{
			cout<<"�ļ����ݷǷ������������롣"<<endl;
			return 0;
		}
	}

	cout<<endl;
	//���ͼ�Ĺ�������Ա��û����
	cout<<"**********���������ͼ����������£�**********"<<endl;
	if(isDirected)
		cout<<"��ͼΪ����ͼ��"<<endl;
	else
		cout<<"��ͼΪ����ͼ��"<<endl;
	cout<<"����������"<<myGra->VerticesNum()<<endl;
	cout<<"���ڱ����¡���"<<endl;
	for(int i=0;i<myGra->VerticesNum();i++)
	{
		for(Edge e=myGra->FirstEdge(i);myGra->IsEdge(e);e=myGra->NextEdge(e))
		{
			cout<<"ʼ�㣺"<<e.from<<" �յ㣺"<<e.to<<" Ȩ��"<<e.weight<<endl;
		}
		cout<<endl;
	}

	//�����û���ѡ������֤�����㷨
	cout<<"�������������ڶ�������֤ͼ���㷨������������"<<endl;
	cout<<"��ѡ��Ҫ���еĲ��������磬����1��Ȼ��س�����ʾ��ѡ����С�ͼ������������Ρ�������1-8֮������������Զ��˳���:"<<endl;
	cout<<"(1)ͼ�������������"<<endl;
	cout<<"(2)ͼ�Ĺ����������"<<endl;
	cout<<"(3)�ɶ��з�ʽʵ�ֵ���������"<<endl;
	cout<<"(4)���������������ʽʵ�ֵ���������"<<endl;
	cout<<"(5)��Դ���·��(Dijkstra�㷨)"<<endl;
	cout<<"(6)ÿ�Զ���֮������·��(Floyd�㷨)"<<endl;
	cout<<"(7)��С֧����(Prim�㷨)"<<endl;
	cout<<"(8)��С֧����(Kruskal�㷨)"<<endl;
	while(1)
	{
		cout<<"���ѡ���ǣ�";
		cin>>choice;
		if(choice==1)
			graph_traverse(*myGra,true);
		else if(choice==2)
			graph_traverse(*myGra,false);
		else if(choice==3)
		{
			if(!isDirected)
			{
				cout<<"��ͼ������ͼ�������������㷨�����������޻�ͼ��"<<endl;
				continue;
			}
			TopsortbyQueue(*myGra);
		}
		else if(choice==4)
		{
			if(!isDirected)
			{
				cout<<"��ͼ������ͼ�������������㷨�����������޻�ͼ��"<<endl;
				continue;
			}
			TopsortbyDFS(*myGra);
		}
		else if(choice==5)
		{
			int start;
			while(1)
			{
				cout<<"��ѡ��Դ��(���磬����0��Ȼ��س�����ʾ��ѡ���Զ���0ΪԴ��)��";
				cin>>start;
				if(start<0||start>=myGra->VerticesNum())
					cout<<"����Ƿ���"<<endl;
				else break;
			}
			int *DforDijkstra=new int[myGra->VerticesNum()];
			for(int i=0;i<myGra->VerticesNum();i++)
				DforDijkstra[i]=INFINITY;
			DforDijkstra[start]=0;
			Dijkstra(*myGra,DforDijkstra,start);
			cout<<endl;
			for(int i=0;i<myGra->VerticesNum();i++)
				cout<<start<<"��"<<i<<"="<<DforDijkstra[i]<<endl;
		}
		else if(choice==6)
		{
			int **DforFloyd=(int **)new int*[myGra->VerticesNum()];
			for(int i=0;i<myGra->VerticesNum();i++)
				DforFloyd[i]=new int[myGra->VerticesNum()];
			Floyd(*myGra,DforFloyd);
		}
		else if(choice==7)
		{
			if(isDirected)
			{
				cout<<"��ͼ������ͼ������С֧������Prim�㷨��������ͨ����ͼ��"<<endl;
				continue;
			}
			int start;
			while(1)
			{
				cout<<"��ѡ����ʼ��(���磬����0��Ȼ��س�����ʾ��ѡ���Զ���0Ϊʼ��)��";
				cin>>start;
				if(start<0||start>=myGra->VerticesNum())
					cout<<"����Ƿ���"<<endl;
				else break;
			}
			Prim(*myGra,start);
		}
		else if(choice==8)
		{
			if(isDirected)
			{
				cout<<"��ͼ������ͼ������С֧������Kruskal�㷨��������ͨ����ͼ��"<<endl;
				continue;
			}
			Kruskal(*myGra);
		}
		else
			return 0;
		cout<<endl;
	}
}
