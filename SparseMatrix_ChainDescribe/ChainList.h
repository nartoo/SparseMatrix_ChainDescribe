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
	void Zero() { first = 0; }  //类内定义，可能是内联函数，这种编程风格不好？
	Chain<T>& Append(const T& x)
private:
	ChainNode<T> *first; //指向第一个节点的指针
	ChainNode<T> *last;  //指向最后一个节点的指针
};

template<class T>
Chain<T>::~Chain()
{
	// 链表的析构函数，用于删除链表中的所有节点
	last = 0;
	ChainNode<T> *next; // 下一个节点
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
	// 返回链表中的元素总数
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
	//寻找链表中的第k个元素，并将其传送至x
	//如果不存在第k个元素，则返回false，否则返回true
		if (k < 1) return false;
	ChainNode<T> *current = first;
	int index = 1; // current的索引
	while (index < k && current) 
	{
		current = current->link;
		index++;
	}
	if (current) { x = current->data; return true; }
	return false; // 不存在第k个元素
}

template<class T>
int Chain<T>::Search(const T& x) const
{
	//本函数假定对于类型T定义了 != 操作
	//寻找x，如果发现x，则返回x的地址
	//如果x不在链表中，则返回0
	ChainNode<T> *current = first;
	int index = 1; // current的索引
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
	//本函数要求对于类型T必须定义<<操作
	//将链表元素送至输出流
	ChainNode<T> *current;
	for (current = first; current; current = current->link)
		out << current->data << " ";
}
// 重载<<
template <class T>
ostream& operator<<(ostream& out, const Chain<T>& x)
{
	//用法就是，cout << L (L是一个Chain链表)，
	//从这里触发对Output()的调用，所以Output()是一个实用函数。
	x.Output(out); 
	return out;
}

template<class T>
Chain<T>& Chain<T>::Delete(int k, T& x)
{
	//把第k个元素取至x，然后从链表中删除第k个元素
	//如果不存在第k个元素，则引发异常OutOfBounds
		if (k < 1 || !first)
			throw OutOfBounds(); // 不存在第k个元素
	// p最终将指向第k个节点
	ChainNode<T> *p = first;
	// 将p移动至第k个元素，并从链表中删除该元素
	if (k == 1) // p已经指向第k个元素
		first = first->link; // 删除之
	else
	{
		//用q指向第k - 1个元素
		ChainNode<T> *q = first;
		for (int index = 1; index < k - 1 && q; index++)
			q = q->link;
		if (!q || !q->link)
			throw OutOfBounds(); //不存在第k个元素
		p = q->link; //存在第k个元素
		if (p == last) last = q; //对last指针的一种可能处理，
								 //如果刚好删除最后一个元素
		q->link = p->link;  //从链表中删除该元素,如果p指向最后一个节点，
							//则此处保证q->link=NULL
	}
	//保存第k个元素并释放节点p
	x = p->data;
	delete p;
	return *this;
}
template<class T>
Chain<T>& Chain<T>::Insert(int k, const T& x)
{
	//在第k个元素之后插入x
	//如果不存在第k个元素，则引发异常OutOfBounds
	//如果没有足够的空间，则传递NoMem异常
	if (k < 0) 
		throw OutOfBounds();
	//p最终将指向第k个节点
	ChainNode<T> *p = first;
	//将p移动至第k个元素
	for (int index = 1; index < k && p; index++)
		p = p->link;
	if (k > 0 && !p) 
		throw OutOfBounds(); //不存在第k个元素
	//插入
	ChainNode<T> *y = new ChainNode<T>;
	y->data = x;
	if (k) 
	{
		//在p之后插入
		y->link = p->link; //如果实在最后插入元素，
						   //那么此处可以保证y->link=NULL
		p->link = y;
	}
	else 
	{
		//作为第一个元素插入
		y->link = first;
		first = y;
	}
	if (!y->link) last = y;	//对last指针的一种可能处理，
							//如果刚好在最后的位置插入元素
	return *this;
}

template<class T>
void Chain<T>::Erase()
{
	//删除链表中的所有节点
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
	// 在链表尾部添加x
	ChainNode<T> *y;
	y = new ChainNode<T>;
	y->data = x; y->link = 0;
	if (first)
	{
		//链表非空
		last->link = y;
		last = y;
	}
	else //链表为空
		first = last = y;
	return *this;
}

//假定Output()不是Chain类的成员函数，并且在该类中没有重载操作符<<。
//链表遍历器类
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

//采用以上链表遍历器输出链表
//int *x;
//ChainIterator<int> c;
//x = c.Initialize(X);
//while (x) {
//	cout << *x << ' ';
//	x = c.Next();
//}
//cout << endl;