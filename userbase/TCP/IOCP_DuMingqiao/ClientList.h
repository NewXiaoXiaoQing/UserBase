#include <stdlib.h>
#include "winsock2.h"

// �ڵ�
template <class T>class DNode
{
private:
	// ָ�����ҽ���ָ��
	DNode<T>* left;
	DNode<T>* right;
public:
	// data Ϊ���г�Ա 
	T data;

	// ���캯��
	DNode(void);
	DNode(const T& item);


	// �ı��ķ���
	void InsertRight(DNode<T>* p);
	void InsertLeft(DNode<T>* p);
	DNode<T>* DeleteNode(void);


	// ȡ��ָ�����ҷ������ָ��
	DNode<T>* NextNodeRight(void) const;
	DNode<T>* NextNodeLeft(void) const;
};

// �����ձ���������δ���壬���� header
template <class T>DNode<T>::DNode(void)
{
	// ��ʼ�����ʹ��ָ������
	left = right = this;
}

// �����ձ���ʼ������
template <class T>DNode<T>::DNode(const T& item)
{
	// ����һ��ָ������Ľ�㲢��ʼ�� data ��
	left = right = this;
	data = item;
}

// ����� p ���뵽˫�������е�ǰ�����ұ�
template <class T>void DNode<T>::InsertRight(DNode<T>* p)
{
	// �� p �͵�ǰ�����Һ�̽������
	p->right = right;
	right->left = p;

	// �� p ����ߺ͵�ǰ�������
	p->left = this;
	right = p;
}

// ����� p ���뵽��ǰ������
template <class T>void DNode<T>::InsertLeft(DNode<T>* p)
{
	// �� p �͵�ǰ�������̽������
	p->left = left;
	left->right = p;

	// �� p ���ұߺ͵�ǰ�������
	p->right = this;
	left = p;
}

// ��������ɾ����ǰ��㲢�������ַ
template <class T>DNode<T>* DNode<T>::DeleteNode(void)
{
	// ��ߵĽ��������ӵ���ǰ�����ұ�
	left->right = right;

	// �ұߵĽ��������ӵ���ǰ�������
	right->left = left;

	// ���ص�ǰ����ָ��
	return this;
}

// ����ָ���ұ߽���ָ��
template <class T>DNode<T>* DNode<T>::NextNodeRight(void) const
{
	return right;
}

// ����ָ����߽���ָ��
template <class T>DNode<T>* DNode<T>::NextNodeLeft(void) const
{
	return left;
}



//˫������
template <class T>
class DCLinkedList
{
private:
	// ָ���ͷ��ָ��͵�ǰ����ָ��
	DNode<T>* header, * currPtr;

	// ���е�Ԫ�ظ����͵�ǰ��λ��ֵ
	int size;

	// ���� L ��������ǰ��β
	void CopyList(const DCLinkedList<T>& L);

public:
	// ���캯��
	DCLinkedList(void);
	DCLinkedList(const DCLinkedList<T>& L);

	// ��������
	~DCLinkedList(void);

	// ��ֵ�����
	DCLinkedList<T>& operator= (const DCLinkedList<T>& L);

	// ����״̬�ĺ���
	int ListSize(void) const;  //���ؽ����             
	bool ListEmpty(void) const;//�����Ƿ�Ϊ��

	// ������ĺ���
	void Reset(bool bheader = true);	// �Ǵӱ�ͷ��ʼ���������Ǳ�β�����������
	void Next(void);                    //
	void Prev(void);                    //
	bool EndOfList(void) const;         //�Ƿ񵽱�β

	// ���뺯��
	void InsertFront(const T& item);    //��ͷ����
	void InsertRear(const T& item);     //��β����
	void InsertAt(const T& item);       //��ǰ����
	void InsertAfter(const T& item);    //������

	// ɾ������
	void DeleteFront(void);      //ɾ����ͷ
	void DeleteRear(void);       //ɾ����β
	void DeleteAt(void);         //ɾ����ǰ

	// ���ʣ��޸�����
	T& Data(void);               //���ʵ�ǰ�������
	bool Find(const T& item);    //�����Ƿ���item���

	// ��ձ�ĺ���
	void ClearList(void);        //
};

// ���캯��
template <class T>DCLinkedList<T>::DCLinkedList(void) :size(0)
{
	// ��������λ����㣬ע�� new ���ܻ��׳��쳣
	currPtr = header = new DNode<T>();
}

// ���캯��
template <class T>DCLinkedList<T>::DCLinkedList(const DCLinkedList<T>& L) :size(0)
{
	// ��������λ����㣬ע�� new ���ܻ��׳��쳣
	currPtr = header = new DNode<T>();
	CopyList(L);
}

// ��������
template <class T>DCLinkedList<T>::~DCLinkedList(void)
{
	ClearList();
	delete header;
}

// �� L ��������ǰ��β
template <class T>void DCLinkedList<T>::CopyList(const DCLinkedList<T>& L)
{
	// ��ָ�� P ������
	DNode<T>* p = L.header->NextNodeRight();

	// ����ǰ��ı�β���� L ��ÿ��Ԫ��
	while (p != L.header)
	{
		InsertRear(p->data);
		p = p->NextNodeRight();
	}
}

template <class T>int DCLinkedList<T>::ListSize(void) const
{
	// ��������λ���
	return size;
}

template <class T>bool DCLinkedList<T>::ListEmpty(void) const
{
	return (size == 0);
}

template <class T>void DCLinkedList<T>::Reset(bool bheader)
{
	if (bheader)
		currPtr = header->NextNodeRight();	// ��ͷ
	else
		currPtr = header->NextNodeLeft();	// ��β
}

template <class T>void DCLinkedList<T>::Next(void)
{
	currPtr = currPtr->NextNodeRight();
}

template <class T>void DCLinkedList<T>::Prev(void)
{
	currPtr = currPtr->NextNodeLeft();
}

template <class T>bool DCLinkedList<T>::EndOfList(void) const
{
	return (currPtr == header);
}

// ���뺯��
template <class T>void DCLinkedList<T>::InsertFront(const T& item)
{
	Reset();
	InsertAt(item);
}

template <class T>void DCLinkedList<T>::InsertRear(const T& item)
{
	currPtr = header;
	InsertAt(item);
}

template <class T>void DCLinkedList<T>::InsertAt(const T& item)
{
	DNode<T>* newNode = new DNode<T>(item);
	currPtr->InsertLeft(newNode);
	currPtr = newNode;
	size++;
}

template <class T>void DCLinkedList<T>::InsertAfter(const T& item)
{
	Next();
	InsertAt(item);
}

// ɾ������
template <class T>void DCLinkedList<T>::DeleteFront(void)
{
	Reset();
	DeleteAt();
}

template <class T>void DCLinkedList<T>::DeleteRear(void)
{
	Reset(false);
	DeleteAt();
}

template <class T>void DCLinkedList<T>::DeleteAt(void)
{
	// ����Ϊ�ջ��ѵ���β���򷵻�
	if (currPtr == header)
		return;

	DNode<T>* p = currPtr->NextNodeRight();
	delete (currPtr->DeleteNode());
	currPtr = p;
	size--;
}

// ���ʣ��޸�����
template <class T>T& DCLinkedList<T>::Data(void)
{
	// ����Ϊ�ջ��ѵ���β�������
	if (currPtr == header)
		throw "DCLinkedList::Data: invalid reference";

	return currPtr->data;
}

template <class T>bool DCLinkedList<T>::Find(const T& item)
{
	for (Reset(); !EndOfList(); Next())
		if (Data() == item)
			return true;

	return false;
}

template <class T>DCLinkedList<T>& DCLinkedList<T>::operator= (const DCLinkedList<T>& L)
{
	if (this == &L)      // �޷���ֵ������
		return *this;

	ClearList();
	CopyList(L);
	return *this;
}

template <class T>void DCLinkedList<T>::ClearList(void)
{
	Reset();
	while (currPtr != header)
		DeleteAt();
}
