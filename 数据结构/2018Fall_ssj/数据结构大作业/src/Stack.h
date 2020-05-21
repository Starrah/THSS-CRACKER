#pragma once
#include <iostream>
#define DEFAULT_CAPACITY 10
using namespace std;

//栈，使用数组实现
template <typename T> class Stack
{
public:
	Stack(int capa = DEFAULT_CAPACITY);
	Stack(Stack &st);
	~Stack();
	void push(T const&e);
	void pop();
	T top();
	bool empty();
public:
	int count;
	int _capacity;
	T* _elem;
	void expand();
	void shrink();
};


template<typename T>
inline Stack<T>::Stack(int capa)
{
	_elem = new T[capa];
	count = 0;
	_capacity = capa;
}

template<typename T>
inline Stack<T>::Stack(Stack & st)
{
	_capacity = st._capacity;
	count = st.count;
	_elem = new T[_capacity];
	for (int i = 0; i < count; i++)
	{
		_elem[i] = st._elem[i];
	}
}

template<typename T>
inline Stack<T>::~Stack()
{
	delete[] _elem;
}

template<typename T>
inline void Stack<T>::push(T const& e)
{
	expand();
	_elem[count] = e;
	count++;
}

template<typename T>
inline void Stack<T>::pop()
{
	if (count == 0)
	{
		cout << "stack empty-pop" << endl;
		return;
	}
	count--;
	shrink();
}

template<typename T>
inline T Stack<T>::top()
{
	if (count != 0)
		return _elem[count - 1];
	else
	{
		cout << "stack empty-top" << endl;
	}
}

template<typename T>
inline bool Stack<T>::empty()
{
	return (count==0);
}

template<typename T>
inline void Stack<T>::expand()
{
	if (count < _capacity / 2)
		return;
	T* old_elem = _elem;
	_elem = new T[_capacity * 2];
	_capacity *= 2;
	if (_elem == NULL)
		cout << "error!";
	for (int i = 0; i < count; i++)
	{
		_elem[i] = old_elem[i];
	}
	delete[]old_elem;
}

template<typename T>
inline void Stack<T>::shrink()
{
	if ((4 * count) >= _capacity)
		return;
	T* old_elem = _elem;
	_elem = new T[_capacity / 2];
	_capacity /= 2;
	for (int i = 0; i < count; i++)
	{
		_elem[i] = old_elem[i];
	}
	delete[]old_elem;
}
