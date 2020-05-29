#pragma once

#include <memory>
#include <vector>
#include <iostream>


template <typename T>    // allow for data of any type to be provided to a node
class Node : public std::enable_shared_from_this<Node<T>> {
public:
	Node(const T data) : data_(data) {}


	T getData() { return data_; }
	std::vector<std::shared_ptr<Node<T>>> getChildren() { return children_; }
	std::shared_ptr<Node<T>> getParent() { return parent_.lock(); }


	void setParent(std::shared_ptr<Node<T>> newParent) {
		// have to remove this node as member of previous parent's children
		if (parent_.lock() != nullptr) parent_.lock()->removeChild(this->shared_from_this());
		parent_ = newParent;
	}


	void addChild(std::shared_ptr<Node<T>> child) {
		child->setParent(this->shared_from_this());
		children_.push_back(child);
	}


	// removes first instance of child from the parent's children vector
	void removeChild(std::shared_ptr<Node<T>> node) {
		int i{ 0 };
		while (i < children_.size()) {
			if (children_[i] == node) {
				children_.erase(children_.begin() + i);
				node->setParent(nullptr);    // node should no long have this as parent
				return;
			}
			++i;
		}
	}


private:
	T data_;
	std::vector<std::shared_ptr<Node<T>>> children_{};
	std::weak_ptr<Node<T>> parent_;
};
