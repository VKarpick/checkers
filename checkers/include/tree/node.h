/*-------------------------------------------------------------------------------------------------
 A templated class for different types of nodes.
 Forces all children of a node to have the node as their parent.
 Only allows for one parent to each node but a variable amount of children.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <memory>
#include <vector>
#include <iostream>


template <typename T>    // allow for data of any type to be provided to a node
class Node : public std::enable_shared_from_this<Node<T>> {
public:
	Node(const T data) : data_(data) {}


	T get_data() {
		return data_;
	}

	std::vector<std::shared_ptr<Node<T>>> get_children() {
		return children_;
	}

	std::shared_ptr<Node<T>> get_parent() {
		return parent_.lock();
	}


	void set_parent(const std::shared_ptr<Node<T>> new_parent) {
		// have to remove this node as member of previous parent's children
		if (parent_.lock() != nullptr) {
			parent_.lock()->remove_child(this->shared_from_this());
		}
		parent_ = new_parent;
	}


	void add_child(std::shared_ptr<Node<T>> child) {
		child->set_parent(this->shared_from_this());
		children_.push_back(child);
	}


	// removes first instance of child from the parent's children vector
	void remove_child(std::shared_ptr<Node<T>> node) {
		size_t i{ 0 };
		while (i < children_.size()) {
			if (children_[i] == node) {
				children_.erase(children_.begin() + i);
				node->set_parent(nullptr);    // node should no longer have this as parent
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
