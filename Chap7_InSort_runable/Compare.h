//比较两个记录大小
class Compare{
public:
	static bool lt(int x,int y) {return x<y;}			//less than 小于
	static bool eq(int x,int y) {return x==y;}			//equal 等于
	static bool gt(int x,int y) {return x>y;}			//greater than 大于
};
