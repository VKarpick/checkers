#pragma once

#include <vector>

template <typename T>
class Node {
public:
	Node(const T data) {
		data_ = data;
		parent_ = nullptr;
	}

	T data() { return data_; }
	std::vector<Node<T>*> children() { return children_; }
	Node<T>* parent() { return parent_; }

	void parent(const Node<T>* new_parent) {
		if (parent_ == nullptr) parent_ = new_parent;
	}

	void add_child(Node<T>* child) {
		if (child->parent() == nullptr) {
			child->parent(this);
			children_.push_back(child);
		}
	}

private:
	T data_;
	std::vector<Node<T>*> children_;
	Node<T>* parent_;
};
