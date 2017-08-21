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
	Chain<CNode<T> > a; //������Chain<T>��֮ǰ�����������Chain��������������ChainNode<T>���͵�ָ��,
						//����Chain<CNode<T> >�е�������������ָ��ChainNode<CNode<T> >���ָ�롣
						//��ChainNode<CNode<T> >����������У���ŵ�������CNode<T>�ࣨdata��������һ��
						//ָ��ChainNode<CNode<T> >���ָ��link����ˣ�������ʵ�����С��ڵ㡱�����
						//��ChainNode<T>,��CNode<T>ʵ������ChainNode<T>�г䵱��һ�������ݿ�����á�
						//��Ҳ��CNode<T>�в�δ����������������link�����͡�
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
	Chain<HeadNode<T> > a; // ͷ�ڵ�����
};

//����һ��ϡ�����
template<class T>
istream& operator >> (istream& in, LinkedMatrix<T>& x)
{
	// �����������������x
	x.a.Erase(); // ɾ��x�е����нڵ�
	// ��ȡ��������
	int terms; // �����Ԫ����
	cout << "Enter number of rows, columns, and terms" << endl;
	in >> x.rows >> x.cols >> terms;
	//�����0��,��ô˵����Ϊ��Ԫ�ص����Ǵ�1��ʼ�ģ�������������Ϳ�ʼ�ж�
	//Ԫ�ص����Ƿ����ڵ�ǰ�У�����һ��ʼ��Ҫִ��Append������
	//���������if(H.row)�ֱ�֤0�в��ᱻ��ӽ�ͷ�������������ֻ�е�
	//row=1��һ�м���������row=2��row=1��һ�е�ͷ���ŻᱻAppend��
	//ͷ�������x.a�С�
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
			// ������ǵ�0�У���ѵ�ǰ�е�ͷ�ڵ�H��ӵ�ͷ�ڵ�����x.a֮��
			if (H.row) x.a.Append(H);
			// Ϊ�µ�һ��׼��H
			H.row = row;
			H.a.Zero();  // ������ͷָ��first = 0
		} 
		// �����Ԫ��
		CNode<T> *c = new CNode<T>;
		c->col = col;
		c->value = value;
		H.a.Append(*c); //Append����ǰͷ�����������������С�
	}
	// ע���������һ�У���Ϊi=1��һ�е�ͷ�������i=2��ʱ��Append��ͷ��������еġ�
	//Ҳ����i=termsʱ����ӵ���i=terms-1��һ�е�ͷ��㡣
	if (H.row) x.a.Append(H);
	H.a.Zero(); // ������ͷָ��Ϊ0�����Ӳ��š�
	return in;
}

//���һ��ϡ�����
template<class T>
ostream& operator<<(ostream& out, const LinkedMatrix<T>& x)
{
	// �Ѿ���x ���������
	ChainIterator<HeadNode<T> > p;// ͷ�ڵ������
	// ��������ά��
	out << "rows = " << x.rows << " columns = " << x.cols << endl;
	// ��hָ���һ��ͷ�ڵ�
	HeadNode<T> *h = p.Initialize(x.a);
	if (!h)
	{
		out << "No non-zero terms" << endl;
		return out;
	}
	// ÿ�����һ��
	while (h)
	{
		out << "row " << h->row << endl;
		out << h->a << endl; //���������
		h = p.Next(); // ��һ��ͷ�ڵ�
	}
	return out;
}

//ת��һ��ϡ�����
template<class T>
void LinkedMatrix<T>::Transpose(LinkedMatrix<T> &b) const
{
	//ת��*this�����ѽ������b
	b.a.Erase(); // ɾ��b�����нڵ�
	//���������ռ�b�и���Ԫ�ص�����
	Chain<CNode<T> > *bin;
	bin = new Chain<CNode<T> >[cols + 1];
	//ͷ�ڵ������
	ChainIterator<HeadNode<T> > p;
	//hָ��*this�ĵ�һ��ͷ�ڵ�
	HeadNode<T> *h = p.Initialize(a);
	//��*this��Ԫ�ظ��Ƶ�������
	while (h) 
	{ 
		//������е���
		int r = h->row; // �������е�����
		//�����������
		ChainIterator<CNode<T> > q;
		//zָ��������ĵ�һ���ڵ�
		CNode<T> *z = q.Initialize(h->a);
		CNode<T> x; // ��ʱ�ڵ�
		//*this��r���е�Ԫ�ر��b�е�r�е�Ԫ��
		x.col = r;
		//���*this�е�r�е����з�0Ԫ��
		while (z) 
		{
			//������r��
			x.value = z->value;
			bin[z->col].Append(x);
			z = q.Next(); // ���е���һ��Ԫ��
		}
		h = p.Next(); // ������һ��
	}
	//����b��ά��
	b.rows = cols;
	b.cols = rows;
	//װ��b��ͷ�ڵ�����
	HeadNode<T> H;
	// ��������
	for (int i = 1; i <= cols; i++)
		if (!bin[i].IsEmpty())
		{
			//ת�þ���ĵ�i��
			H.row = i;
			H.a = bin[i];
			b.a.Append(H);
			bin[i].Zero();//������ͷָ��Ϊ0�����Ӳ���
		}
	H.a.Zero(); // ������ͷָ��Ϊ0
	delete[] bin;
}