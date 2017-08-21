#pragma once
#include "ChainList.h"
template<class T>
class CNode
{
public:
	int operator !=(const CNode<T>& y)
	{
		return (value != y.value);
	}
	void Output(ostream& out) const
	{
		out << "column " << col << " value " << value;
	}
private:
	int col;
	T value;
};

template<class T>
ostream& operator<<(ostream& out, const CNode<T>& x)
{
	x.Output(out); out << endl; return out;
}

template<class T>
class HeadNode 
{
public:
	int operator !=(const HeadNode<T>& y)
	{
		return (row != y.row);
	}
	void Output(ostream& out) const
	{
		out << "row " << row;
	}
private:
	int row;
	Chain<CNode<T> > a; //行链表，Chain<T>是之前定义的链，而Chain的数据域是两个ChainNode<T>类型的指针,
						//所以Chain<CNode<T> >中的数据域是两个指向ChainNode<CNode<T> >类的指针。
						//在ChainNode<CNode<T> >类的数据域中，存放的数据是CNode<T>类（data），还有一个
						//指向ChainNode<CNode<T> >类的指针link。因此，在这里实际上有“节点”意义的
						//是ChainNode<T>,而CNode<T>实际上在ChainNode<T>中充当了一种像数据块的作用。
						//这也是CNode<T>中并未定义文中所描述的link域额解释。
};

template<class T>
ostream& operator<<(ostream& out, const HeadNode<T>& x)
{
	x.Output(out); out << endl; return out;
}

template<class T>
class LinkedMatrix
{
	friend ostream& operator<<
		(ostream&, const LinkedMatrix<T>&);
	friend istream& operator >>
		(istream&, LinkedMatrix<T>&);
public:
	LinkedMatrix() { }
	~LinkedMatrix() { }
	void Transpose(LinkedMatrix<T> &b) const;
	void Add(Const LinkedMatrix<T> &b, LinkedMatrix<T> &c) const;
private:
	int rows, cols; // 矩阵维数
	Chain<HeadNode<T> > a; // 头节点链表
};

//输入一个稀疏矩阵
template<class T>
istream& operator >> (istream& in, LinkedMatrix<T>& x)
{
	// 从输入流中输入矩阵x
	x.a.Erase(); // 删除x中的所有节点
	// 获取矩阵特征
	int terms; // 输入的元素数
	cout << "Enter number of rows, columns, and terms" << endl;
	in >> x.rows >> x.cols >> terms;
	//虚设第0行,这么说是因为，元素的行是从1开始的，而程序接下来就开始判断
	//元素的行是否属于当前行，所以一开始就要执行Append操作，
	//但是下面的if(H.row)又保证0行不会被添加进头结点链表，接下来只有当
	//row=1这一行加满后遇到row=2，row=1这一行的头结点才会被Append进
	//头结点链表x.a中。
	HeadNode<T> H; // 当前行的头节点
	H.row = 0; // 当前行号
			   // 输入x的非0元素
	for (int i = 1; i <= terms; i++)
	{
		// 输入下一个元素
		cout << "Enter row, column, and value of term " << i << endl;
		int row, col;
		T value;
		in >> row >> col >> value;
		// 检查新元素是否属于当前行
		if (row > H.row)
		{
			// 开始一个新行
			// 如果不是第0行，则把当前行的头节点H添加到头节点链表x.a之中
			if (H.row) x.a.Append(H);
			// 为新的一行准备H
			H.row = row;
			H.a.Zero();  // 置链表头指针first = 0
		} 
		// 添加新元素
		CNode<T> *c = new CNode<T>;
		c->col = col;
		c->value = value;
		H.a.Append(*c); //Append进当前头结点所带领的行链表中。
	}
	// 注意矩阵的最后一行，因为i=1这一行的头结点是在i=2的时候Append进头结点链表中的。
	//也就是i=terms时，添加的是i=terms-1这一行的头结点。
	if (H.row) x.a.Append(H);
	H.a.Zero(); // 置链表头指针为0，过河拆桥。
	return in;
}

//输出一个稀疏矩阵
template<class T>
ostream& operator<<(ostream& out, const LinkedMatrix<T>& x)
{
	// 把矩阵x 送至输出流
	ChainIterator<HeadNode<T> > p;// 头节点遍历器
	// 输出矩阵的维数
	out << "rows = " << x.rows << " columns = " << x.cols << endl;
	// 将h指向第一个头节点
	HeadNode<T> *h = p.Initialize(x.a);
	if (!h)
	{
		out << "No non-zero terms" << endl;
		return out;
	}
	// 每次输出一行
	while (h)
	{
		out << "row " << h->row << endl;
		out << h->a << endl; //输出行链表
		h = p.Next(); // 下一个头节点
	}
	return out;
}

//转置一个稀疏矩阵
template<class T>
void LinkedMatrix<T>::Transpose(LinkedMatrix<T> &b) const
{
	//转置*this，并把结果放入b
	b.a.Erase(); // 删除b中所有节点
	//创建用来收集b中各行元素的箱子
	Chain<CNode<T> > *bin;
	bin = new Chain<CNode<T> >[cols + 1];
	//头节点遍历器
	ChainIterator<HeadNode<T> > p;
	//h指向*this的第一个头节点
	HeadNode<T> *h = p.Initialize(a);
	//把*this的元素复制到箱子中
	while (h) 
	{ 
		//检查所有的行
		int r = h->row; // 行链表中的行数
		//行链表遍历器
		ChainIterator<CNode<T> > q;
		//z指向行链表的第一个节点
		CNode<T> *z = q.Initialize(h->a);
		CNode<T> x; // 临时节点
		//*this第r行中的元素变成b中第r列的元素
		x.col = r;
		//检查*this中第r行的所有非0元素
		while (z) 
		{
			//遍历第r行
			x.value = z->value;
			bin[z->col].Append(x);
			z = q.Next(); // 该行的下一个元素
		}
		h = p.Next(); // 继续下一行
	}
	//设置b的维数
	b.rows = cols;
	b.cols = rows;
	//装配b的头节点链表
	HeadNode<T> H;
	// 搜索箱子
	for (int i = 1; i <= cols; i++)
		if (!bin[i].IsEmpty())
		{
			//转置矩阵的第i行
			H.row = i;
			H.a = bin[i];
			b.a.Append(H);
			bin[i].Zero();//置链表头指针为0，过河拆桥
		}
	H.a.Zero(); // 置链表头指针为0
	delete[] bin;
}