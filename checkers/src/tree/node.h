#pragma once

#include <vector>


template <typename T>    // allow for data of any type to be provided to a node
class Node {
public:
	Node(const T data) : data_(data) {}

	// getters
	T data() { return data_; }
	std::vector<Node<T>*> children() { return children_; }
	Node<T>* parent() { return parent_; }

	// setter
	void parent(Node<T>* new_parent) {
		// have to remove this node as member of previous parent's children
		if (parent_ != nullptr) parent_->remove_child(this);
		parent_ = new_parent;
	}

	void add_child(Node<T>* child) {
		if (child->parent() == nullptr) {
			child->parent(this);
			children_.push_back(child);
		}
	}

	// removes first instance of child from the parent's children vector
	void remove_child(Node<T>* node) {
		for (auto child = children_.begin(); child != children_.end(); ++child) {
			if (*child == node) {
				children_.erase(child);
				node->parent(nullptr);    // node should no longer have this as parent
				break;
			}
		}
	}

private:
	T data_;
	std::vector<Node<T>*> children_;
	Node<T>* parent_;
};
