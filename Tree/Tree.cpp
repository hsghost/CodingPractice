/*
/
*/

#include "Tree.h"

template <typename Data> TreeNode<Data>::TreeNode()
{
	;
}

template <typename Data> TreeNode<Data>::~TreeNode()
{
	delete this->data_;
	this->data_ = nullptr;
	delete this->temp_;
	this->temp_ = nullptr;
}

template <typename Data> Tree<Data>::Tree()
{
	;
}

template <typename Data> Tree<Data>::~Tree()
{
	delete this->root;
	this->root = nullptr;
}
