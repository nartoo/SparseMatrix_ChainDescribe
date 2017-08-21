#pragma once
template <class T>
class ChainNode
{
	friend Chain<T>;
private:
	T data;
	ChainNode<T> *link;
};

template<class T>
class Chain
{
public:
	Chain() { first = 0; last = 0}
	~Chain();
	bool IsEmpty() const { return first == 0; }
	int Length() const;
	bool Find(int k, T& x) const;
	int Search(const T& x) const;
	Chain<T>& Delete(int k, T& x);
	Chain<T>& Insert(int k, const T& x);
	void Output(ostream& out) const;
	void Erase();
	void Zero() { first = 0; }  //���ڶ��壬�������������������ֱ�̷�񲻺ã�
	Chain<T>& Append(const T& x)
private:
	ChainNode<T> *first; //ָ���һ���ڵ��ָ��
	ChainNode<T> *last;  //ָ�����һ���ڵ��ָ��
};

template<class T>
Chain<T>::~Chain()
{
	// �������������������ɾ�������е����нڵ�
	last = 0;
	ChainNode<T> *next; // ��һ���ڵ�
	while (first) 
	{
		next = first->link;
		delete first;
		first = next;
	}
}

template<class T>
int Chain<T>::Length() const
{
	// ���������е�Ԫ������
	ChainNode<T> *current = first;
	int len = 0;
	while (current)
	{
		len++;
		current = current->link;
	}
    return len;
}

template<class T>
bool Chain<T>::Find(int k, T& x) const
{
	//Ѱ�������еĵ�k��Ԫ�أ������䴫����x
	//��������ڵ�k��Ԫ�أ��򷵻�false�����򷵻�true
		if (k < 1) return false;
	ChainNode<T> *current = first;
	int index = 1; // current������
	while (index < k && current) 
	{
		current = current->link;
		index++;
	}
	if (current) { x = current->data; return true; }
	return false; // �����ڵ�k��Ԫ��
}

template<class T>
int Chain<T>::Search(const T& x) const
{
	//�������ٶ���������T������ != ����
	//Ѱ��x���������x���򷵻�x�ĵ�ַ
	//���x���������У��򷵻�0
	ChainNode<T> *current = first;
	int index = 1; // current������
	while (current && current->data != x)
	{
		current = current->link;
		index++;
	}
	if (current)
		return index;
	return 0;
}

template<class T>
void Chain<T>::Output(ostream& out) const
{
	//������Ҫ���������T���붨��<<����
	//������Ԫ�����������
	ChainNode<T> *current;
	for (current = first; current; current = current->link)
		out << current->data << " ";
}
// ����<<
template <class T>
ostream& operator<<(ostream& out, const Chain<T>& x)
{
	//�÷����ǣ�cout << L (L��һ��Chain����)��
	//�����ﴥ����Output()�ĵ��ã�����Output()��һ��ʵ�ú�����
	x.Output(out); 
	return out;
}

template<class T>
Chain<T>& Chain<T>::Delete(int k, T& x)
{
	//�ѵ�k��Ԫ��ȡ��x��Ȼ���������ɾ����k��Ԫ��
	//��������ڵ�k��Ԫ�أ��������쳣OutOfBounds
		if (k < 1 || !first)
			throw OutOfBounds(); // �����ڵ�k��Ԫ��
	// p���ս�ָ���k���ڵ�
	ChainNode<T> *p = first;
	// ��p�ƶ�����k��Ԫ�أ�����������ɾ����Ԫ��
	if (k == 1) // p�Ѿ�ָ���k��Ԫ��
		first = first->link; // ɾ��֮
	else
	{
		//��qָ���k - 1��Ԫ��
		ChainNode<T> *q = first;
		for (int index = 1; index < k - 1 && q; index++)
			q = q->link;
		if (!q || !q->link)
			throw OutOfBounds(); //�����ڵ�k��Ԫ��
		p = q->link; //���ڵ�k��Ԫ��
		if (p == last) last = q; //��lastָ���һ�ֿ��ܴ���
								 //����պ�ɾ�����һ��Ԫ��
		q->link = p->link;  //��������ɾ����Ԫ��,���pָ�����һ���ڵ㣬
							//��˴���֤q->link=NULL
	}
	//�����k��Ԫ�ز��ͷŽڵ�p
	x = p->data;
	delete p;
	return *this;
}
template<class T>
Chain<T>& Chain<T>::Insert(int k, const T& x)
{
	//�ڵ�k��Ԫ��֮�����x
	//��������ڵ�k��Ԫ�أ��������쳣OutOfBounds
	//���û���㹻�Ŀռ䣬�򴫵�NoMem�쳣
	if (k < 0) 
		throw OutOfBounds();
	//p���ս�ָ���k���ڵ�
	ChainNode<T> *p = first;
	//��p�ƶ�����k��Ԫ��
	for (int index = 1; index < k && p; index++)
		p = p->link;
	if (k > 0 && !p) 
		throw OutOfBounds(); //�����ڵ�k��Ԫ��
	//����
	ChainNode<T> *y = new ChainNode<T>;
	y->data = x;
	if (k) 
	{
		//��p֮�����
		y->link = p->link; //���ʵ��������Ԫ�أ�
						   //��ô�˴����Ա�֤y->link=NULL
		p->link = y;
	}
	else 
	{
		//��Ϊ��һ��Ԫ�ز���
		y->link = first;
		first = y;
	}
	if (!y->link) last = y;	//��lastָ���һ�ֿ��ܴ���
							//����պ�������λ�ò���Ԫ��
	return *this;
}

template<class T>
void Chain<T>::Erase()
{
	//ɾ�������е����нڵ�
	last = 0;
	ChainNode<T> *next;
	while (first)
	{
		next = first->link;
		delete first;
		first = next;
	}
}

template<class T>
Chain<T>& Chain<T>::Append(const T& x)
{
	// ������β�����x
	ChainNode<T> *y;
	y = new ChainNode<T>;
	y->data = x; y->link = 0;
	if (first)
	{
		//����ǿ�
		last->link = y;
		last = y;
	}
	else //����Ϊ��
		first = last = y;
	return *this;
}

//�ٶ�Output()����Chain��ĳ�Ա�����������ڸ�����û�����ز�����<<��
//�����������
template<class T>
class ChainIterator 
{
public:
	T* Initialize(const Chain<T>& c)
	{
		location = c.first;
		if (location) return &location->data;
		return 0;
	}
	T* Next()
	{
		if(!location) return 0;
		location = location->link;
		if (location) return &location->data;
		return 0;
	}
private:
	ChainNode<T> *location;
};

//������������������������
//int *x;
//ChainIterator<int> c;
//x = c.Initialize(X);
//while (x) {
//	cout << *x << ' ';
//	x = c.Next();
//}
//cout << endl;