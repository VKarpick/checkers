#pragma once

#include <functional>

#include "node.h"


// storing values and nodes together allows for easy retrieval of whichever is needed
template<typename T>
struct MinimaxPair {
	double value;
	Node<T>* node;
};


template <typename T>    // allowing for any type of Node requires template
class Minimax
{
public:
	//TODO what should be public vs private variables?

	Minimax(std::function<double(Node<T>*)> evaluate_node, int max_depth = 0, 
		const std::function<void(Node<T>*)> extend_tree = [](Node<T>*) {}) {

		// function to evaluate the value of a given node
		evaluate_node_ = evaluate_node;
		
		max_depth_ = max_depth;

		// function to find children of a node when passing incomplete tree
		// if complete tree is provided, can be ignored
		extend_tree_ = extend_tree;
	}

	MinimaxPair<T>* minimax(Node<T>* node, int current_depth = 0, bool is_max_player = true, 
		double alpha = -DBL_MAX, double beta = -DBL_MAX) {
		
		if (current_depth == max_depth_) return new MinimaxPair<T>{ evaluate_node_(node), node };
		if (node->children().empty()) extend_tree_(node);

		if (is_max_player) {
			MinimaxPair<T>* best_pair = new MinimaxPair<T>{ -DBL_MAX, nullptr };
			for (auto child : node->children()) {
				MinimaxPair<T>* child_pair = minimax(child, current_depth + 1, false, alpha, beta);
				// use >= to avoid returning nullptr when children exist
				if (child_pair->value >= best_pair->value) best_pair = new MinimaxPair<T>{ child_pair->value, child };
				alpha = fmax(alpha, best_pair->value);
				if (beta <= alpha) break;
			}
			return best_pair;
		}
		else {
			MinimaxPair<T>* best_pair = new MinimaxPair<T>{ DBL_MAX, nullptr };
			for (auto child : node->children()) {
				MinimaxPair<T>* child_pair = minimax(child, current_depth + 1, true, alpha, beta);
				// use <= to avoid returning nullptr when children exist
				if (child_pair->value <= best_pair->value) best_pair = new MinimaxPair<T>{ child_pair->value, child };
				beta = fmin(alpha, best_pair->value);
				if (beta <= alpha) break;
			}
			return best_pair;
		}
	}

	double minimax_value(Node<T>* node, int current_depth = 0, bool is_max_player = true, 
		double alpha = -DBL_MAX, double beta = -DBL_MAX) {
		
		return minimax(node, current_depth, is_max_player, alpha, beta)->value;
	}

	Node<T>* minimax_node(Node<T>* node, int current_depth = 0, bool is_max_player = true, 
		double alpha = -DBL_MAX, double beta = -DBL_MAX) {
		
		return minimax(node, current_depth, is_max_player, alpha, beta)->node;
	}


private:
	int max_depth_;
	std::function<void(Node<T>*)> extend_tree_;
	std::function<double(Node<T>*)> evaluate_node_;
};
