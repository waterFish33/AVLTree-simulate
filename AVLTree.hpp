#pragma once

#include <stdlib.h>     /* abs */
template<class T>
struct AVLTreeNode
{
	AVLTreeNode(const T& data = T())
		: _pLeft(nullptr)
		, _pRight(nullptr)
		, _pParent(nullptr)
		, _data(data)
		, _bf(0)
	{}

	AVLTreeNode<T>* _pLeft;
	AVLTreeNode<T>* _pRight;
	AVLTreeNode<T>* _pParent;
	T _data;
	int _bf;   // �ڵ��ƽ������
};


// AVL: ���������� + ƽ�����ӵ�����
template<class T>
class AVLTree
{
	typedef AVLTreeNode<T> Node;
public:
	AVLTree()
		: _pRoot(nullptr)
	{}

	// ��AVL���в���ֵΪdata�Ľڵ�
	bool Insert(const T& data) {
		if (_pRoot == nullptr) {	//������ڵ�Ϊ�գ�ֱ���ڸ��ڵ����
			_pRoot = new Node(data);
			return true;
		}
		else {						//Ĭ���������Ϊ���򣬱Ƚڵ�С�������ߣ��Ƚڵ�С��������
			Node* parent = nullptr;
			Node* cur = _pRoot;
			while (cur) {
				if (cur->_data > data) {
					parent = cur;
					cur = cur->_pLeft;
				}
				else if (cur->_data < data) {
					parent = cur;
					cur = cur->_pRight;
				}
				else {
					return false;
				}
			}
			cur = new Node(data);
			if (parent->_data > data) {
				parent->_pLeft = cur;
				cur->_pParent = parent;
			}
			else if (parent->_data < data) {
				parent->_pRight = cur;
				cur->_pParent = parent;
			}
			//�����½ڵ��Խڵ����ƽ�����ӵĵ���
			while (parent) {
				if (cur == parent->_pLeft) {
					parent->_bf--;
				}
				else {
					parent->_bf++;
				}
				if (parent == 0) {
					break;
				}

				//����һ�κ�����жϣ��Ƿ�Ҫ����ƽ��
				if (parent->_bf == 1 || parent->_bf == -1) {
					cur = parent;
					parent = parent->_pParent;
				}
				else if (parent->_bf == 2) {
					if (cur->_bf == 1) {
						RotateL(parent);
						break;
					}
					else if (cur->_bf == -1) {
						RotateRL(parent);
						break;
					}
				}
				else if (parent->_bf == -2) {
					if (cur->_bf == -1) {
						RotateR(parent);
						break;
					}
					else if (cur->_bf == 1) {
						RotateLR(parent);
						break;
					}
				}

			}
		}
		return true;
	}


	// AVL������֤
	bool IsAVLTree()
	{
		return _IsAVLTree(_pRoot);
	}
	size_t Height() {
		return _Height(_pRoot);
	}
private:
	// ����AVL���ĸ�����֤pRoot�Ƿ�Ϊ��Ч��AVL��
	bool _IsAVLTree(Node* pRoot) {
		if (pRoot == nullptr) {
			return true;
		}
		if (abs(((int)_Height(pRoot->_pLeft) - (int)_Height(pRoot->_pRight))) > 1) {
			return false;
		}
		
		return _IsAVLTree(pRoot->_pLeft)&&_IsAVLTree(pRoot->_pRight);
	}
	size_t _Height(Node* pRoot) {
		if (pRoot == nullptr) {
			return 0;
		}
		size_t l = _Height(pRoot->_pLeft) + 1;
		size_t r = _Height(pRoot->_pRight) + 1;
		return l > r ? l : r;
	}
	// �ҵ���
	void RotateR(Node* pParent) {
		Node* cur = pParent->_pLeft;
		Node* curR = cur->_pRight;
		pParent->_pLeft = curR;
		if (curR) {
			curR->_pParent = pParent;
		}
		cur->_pRight = pParent;
		
		if (pParent == _pRoot) {
			_pRoot = cur;
			cur->_pParent = nullptr;
		}
		else {
			cur->_pParent = pParent->_pParent;
			cur->_pParent->_pLeft = cur;
		}
		pParent->_pParent = cur;
		
		//��ת�󣬵���ƽ������
		cur->_bf = 0;
		pParent->_bf = 0;
	}
	// ����
	void RotateL(Node* pParent) {
		Node* cur = pParent->_pRight;
		Node* curL = cur->_pLeft;
		pParent->_pRight = curL;
		if (curL) {
			curL->_pParent = pParent;
		}
		cur->_pLeft = pParent;
		
		if (pParent == _pRoot) {
			_pRoot = cur;
			cur->_pParent = nullptr;
		}
		else {
			cur->_pParent = pParent->_pParent;
			cur->_pParent->_pRight = cur;
		}
		pParent->_pParent = cur;
		
		//��ת�󣬵���ƽ������
		cur->_bf = 0;
		pParent->_bf = 0;
	}
	// ����˫��
	void RotateRL(Node* pParent) {

		Node* parent = pParent;
		Node* cur = pParent->_pRight;
		Node* curL = cur->_pLeft;

		RotateR(cur);
		RotateL(parent);

		if (curL->_bf == 0) {
			parent->_bf = 0;
			curL->_bf = 0;
			cur->_bf = 0;
		}else if (curL->_bf == 1) {
			parent->_bf = 0;
			curL->_bf = 0;
			cur->_bf = -1;
		}else if (curL->_bf == -1) {
			parent->_bf = 1;
			curL->_bf = 0;
			cur->_bf = 0;
		}
	}
	// ����˫��
	void RotateLR(Node* pParent) {
		Node* parent = pParent;
		Node* cur = pParent->_pLeft;
		Node* curR = cur->_pRight;

		RotateL(cur);
		RotateR(parent);

		if (curR->_bf == 0) {
			parent->_bf = 0;
			curR->_bf = 0;
			cur->_bf = 0;
		}
		else if (curR->_bf == 1) {
			parent->_bf = -1;
			curR->_bf = 0;
			cur->_bf = 0;
		}
		else if (curR->_bf == -1) {
			parent->_bf = 0;
			curR->_bf = 0;
			cur->_bf = 1;
		}
	}

private:
	Node* _pRoot;
};