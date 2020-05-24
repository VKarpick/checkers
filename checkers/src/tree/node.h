#pragma once

#include <vector>


template <typename T>    // allow for data of any type to be provided to a node
class Node {
public:
	Node(const T data) : data_(data) {}


	T getData() { return data_; }
	std::vector<Node<T>*> getChildren() { return children_; }
	Node<T>* getParent() { return parent_; }


	void setParent(Node<T>* newParent) {
		if (parent_ != nullptr) parent_->removeChild(this);    // have to remove this node as member of previous parent's children
		parent_ = newParent;
	}


	void addChild(Node<T>* child) {
		child->setParent(this);
		children_.push_back(child);
	}


	// removes first instance of child from the parent's children vector
	void removeChild(Node<T>* node) {
		for (auto child = children_.begin(); child != children_.end(); ++child) {
			if (*child == node) {
				children_.erase(child);
				node->setParent(nullptr);    // node should no longer have this as parent
				break;
			}
		}
	}


private:
	T data_;
	std::vector<Node<T>*> children_;
	Node<T>* parent_;
};
