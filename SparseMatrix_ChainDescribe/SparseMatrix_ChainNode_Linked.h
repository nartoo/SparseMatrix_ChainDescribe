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
	CNode<T> a; //������
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
	int rows, cols; // ����ά��
	HeadNode<T> a; // ͷ�ڵ�����
};

template<class T>
istream& operator >> (istream& in, LinkedMatrix<T>& x)
{
	// �����������������x
	x.a.Erase(); // ɾ��x�е����нڵ�
				 // ��ȡ��������
	int terms; // �����Ԫ����
	cout << "Enter number of rows, columns, and terms" << endl;
	in >> x.rows >> x.cols >> terms;
	// �����0��
	HeadNode<T> H; // ��ǰ�е�ͷ�ڵ�
	H.row = 0; // ��ǰ�к�
			   // ����x�ķ�0Ԫ��
	for (int i = 1; i <= terms; i++)
	{
		// ������һ��Ԫ��
		cout << "Enter row, column, and value of term " << i << endl;
		int row, col;
		T value;
		in >> row >> col >> value;
		// �����Ԫ���Ƿ����ڵ�ǰ��
		if (row > H.row)
		{
			// ��ʼһ������
			// ������ǵ�0�У���ѵ�ǰ�е�ͷ�ڵ�H��ӵ�ͷ�ڵ�����x . a֮��
			if (H.row) x.a.Append(H);
			/ / Ϊ�µ�һ��׼��H
				H.row = row;
			H.a.Zero();
		} // ������ͷָ��first = 0
		  // �����Ԫ��
		CNode<T> *c = new CNode<T>;
		c->col = col;
		c->value = value;
		H.a.Append(*c);
	}
	// ע���������һ��
	if (H.row) x.a.Append(H);
	H.a.Zero(); // ������ͷָ��Ϊ0
	return in;
}