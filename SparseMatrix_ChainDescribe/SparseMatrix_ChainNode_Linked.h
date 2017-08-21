#pragma once
template<class T>
class CNode {
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
	CNode<T> a; //行链表
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
	HeadNode<T> a; // 头节点链表
};

template<class T>
istream& operator >> (istream& in, LinkedMatrix<T>& x)
{
	// 从输入流中输入矩阵x
	x.a.Erase(); // 删除x中的所有节点
				 // 获取矩阵特征
	int terms; // 输入的元素数
	cout << "Enter number of rows, columns, and terms" << endl;
	in >> x.rows >> x.cols >> terms;
	// 虚设第0行
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
			// 如果不是第0行，则把当前行的头节点H添加到头节点链表x . a之中
			if (H.row) x.a.Append(H);
			/ / 为新的一行准备H
				H.row = row;
			H.a.Zero();
		} // 置链表头指针first = 0
		  // 添加新元素
		CNode<T> *c = new CNode<T>;
		c->col = col;
		c->value = value;
		H.a.Append(*c);
	}
	// 注意矩阵的最后一行
	if (H.row) x.a.Append(H);
	H.a.Zero(); // 置链表头指针为0
	return in;
}