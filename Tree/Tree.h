/*
/
*/

#ifndef ALGORITHM_TREE_H_
#define ALGORITHM_TREE_H_

#define TREE_SEQUENCE_DFS_PRE	0x02
#define TREE_SEQUENCE_DFS_IN	0x04
#define	TREE_SEQUENCE_DFS_POST	0x05
#define TREE_SEQUENCE_BFS		0x08

#include <vector>
#include <deque>
#include <unordered_map>
#include <utility>

template <typename Data> class TreeNode
{
public:
	virtual TreeNode();
	virtual ~TreeNode();
	template <typename PreArgs, typename InArgs, typename PostArgs> struct DfsOperations
	{
		bool(*pre_op)(TreeNode *node, PreArgs pre_args);
		bool(*in_op)(TreeNode *node, InArgs in_args);
		bool(*post_op)(TreeNode *node, PostArgs post_args);
	};
	bool GenerateVisitingSequence(int sequence_type, std::deque<TreeNode*> **output_seq = nullptr)	// TODO: Move to CPP file.
	{
		if (!output_seq || !(*output_seq))
			return (false);
		TreeNode *current_node;
		switch (sequence_type)
		{
		case TREE_SEQUENCE_BFS:
			std::deque<TreeNode*> bfs_queue;
			bfs_queue.push_back(this);
			while (!bfs_queue.empty())
			{
				(*output_seq)->push_back(current_node = bfs_queue.front());
				bfs_queue.pop_front();
				for (std::vector<TreeNode*>::iterator it = current_node->children_.begin(); it != current_node->children_.end(); ++it)
					bfs_queue.push_back(*it);
			}
			return (true);
		case TREE_SEQUENCE_DFS_PRE:
			std::deque<TreeNode*> dfs_stack;
			dfs_stack.push_back(this);
			while (!dfs_stack.empty())
			{
				(*output_seq)->push_back(current_node = dfs_stack.back());
				dfs_stack.pop_back();
				for (std::vector<TreeNode*>::reverse_iterator it = current_node->children_.rbegin(); it != current_node->children_.rend(); ++it)
					dfs_stack.push_back(*it);
			}
			return (true);
		case TREE_SEQUENCE_DFS_IN:
			std::deque<std::pair<TreeNode*, bool>> dfs_stack;
			dfs_stack.push_back(std::make_pair(this, false));
			while (!dfs_stack.empty())
			{
				if (std::get<2>(dfs_stack.back()))
					(*output_seq)->push_back(current_node = dfs_stack.back());
				dfs_stack.pop_back();
				if (current_node->children_->empty())
					dfs_stack.push_back(std::make_pair(current_node, true);
				else
					for (std::vector<TreeNode*>::itrator it = current_node->children_.begin(); it != current_node->children_.end(); ++it)
					{
						dfs_stack.push_back(std::make_pair(*it, false));
						dfs_stack.push_back(std::make_pair(current_node, true));
					}
			}
			return (true);
		case TREE_SEQUENCE_DFS_POST:
			std::deque<TreeNode> dfs_stack;
			dfs_stack.push_back(root);
			while (!dfs_stack.empty())
			{
				(*output_seq)->push_back(current_node = dfs_stack.back());
				dfs_stack.pop_back();
				for (std::vector<TreeNode*>::itrator it = current_node->children_.begin(); it != current_node->children_.end(); ++it)
					dfs_stack.push_back(*it);
			}
			return (true);
		default:
			return (false);
		}
	}
	template <
		typename PreArgs, typename InArgs, typename PostArgs> static bool DfsNode_Recursive(
			TreeNode *root, DfsOperations <PreArgs, InArgs, PostArgs> *ops,
			PreArgs pre_args, InArgs in_args, PostArgs post_args
	)
	{
		if (!ops)
			return false;
		else
		{
			bool error = false;
			if (ops->pre_op)
				error = error || !ops->pre_op(root, pre_args);
			if (!error && this->children_->size() > 0)
				error = error || this->children_[0]->DfsNode_Recursive(ops, pre_args, in_args, post_args);
			if (!error && ops->in_op && this->children_->size() == 2)
				error = error || !ops->in_op(root, in_args);
			else if (this->children_->size() > 2)
			{
				for (std::vector<TreeNode*>::iterator it = this->children_->begin() + 2; it != this->children_->end(); ++it)
				{
					if (!error)
						error = error || (*it)->DfsNode_Recursive(ops, pre_args, in_args, post_args);
				}
			}
			if (!error && ops->post_op)
				error = error || !ops->post_op(root, post_args);
			return (!error);
		}
	}
	template <
		typename PreArgs, typename InArgs, typename PostArgs> bool DfsNode(
			DfsOperations <PreArgs, InArgs, PostArgs> *ops,
			PreArgs pre_args, InArgs in_args, PostArgs post_args
		)
	{
		if (!ops)
			return (false);
		bool error = false;
		std::deque<TreeNode*> *sequence = new std::deque<TreeNode*>();
		if (ops->pre_op)
		{
			(*sequence)->clear();
			this->GenerateVisitingSequence(TREE_SEQUENCE_DFS_PRE, &sequence);
			for (std::deque<TreeNode*>::reverse_iterator it = (*sequence)->begin(); !error && it != (*sequence)->end(); ++it)
				error = error || !ops->pre_op(*it, pre_args);
		}
		if (ops->in_op)
		{
			(*sequence)->clear();
			this->GenerateVisitingSequence(TREE_SEQUENCE_DFS_IN, &sequence);
			for (std::deque<TreeNode*>::reverse_iterator it = (*sequence)->rbegin(); !error && it != (*sequence)->rend(); ++it)
				error = error || !ops->in_op(*it, pre_args);
		}
		if (ops->post_op)
		{
			(*sequence)->clear();
			this->GenerateVisitingSequence(TREE_SEQUENCE_DFS_POST, &sequence);
			for (std::deque<TreeNode*>::reverse_iterator it = (*sequence)->rbegin(); !error && it != (*sequence)->rend(); ++it)
				error = error || !ops->post_op(*it, pre_args);
		}
		return (!error);
	}
	template <typename BfsArgs> bool BfsNode(bool(*bfs_op)(TreeNode *node, BfsArgs bfs_args), BfsArgs bfs_args)
	{
		if (!bfs_op)
			return (false);
		bool error = false;
		std::deque<TreeNode*> *sequence = new std::deque<TreeNode*>();
		this->GenerateVisitingSequence(TREE_SEQUENCE_BFS, &sequence);
		for (std::deque<TreeNode*>::iterator it = (*sequence)->begin(); !error && it != (*sequence)->end(); ++it)
			error = error || bfs_op(*it, bfs_args);
		return (!error);
	}

private:
	Data* data_;
	std::vector <TreeNode*> children_;
};

template <typename Data, class Alloc = std::allocator<Data> > class Tree
{
public:
	virtual Tree();
	virtual ~Tree();
	bool dfsTree()
	{
		return this->root->DfsNode_Recursive();
	}
	bool bfsTree()
	{
		;
	}

private:
	TreeNode<Data>* root;
};

// #include "Tree.cpp"

#endif // ALGORITHM_TREE_H_
