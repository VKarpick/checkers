#pragma once

#include "node.h"
#include <functional>


template<typename T>
struct MinimaxPair {
	double value;
	Node<T>* node;
};


template <typename T>
class Minimax
{
public:
	// what should be public vs private variables?

	Minimax(std::function<double(Node<T>*)> evaluate_node, int init_max_depth = 0, 
		const std::function<void(Node<T>*)> extend_tree = [](Node<T>*) -> {}) {

		evaluate_node_ = evaluate_node;
		max_depth = init_max_depth;
		extend_tree_ = extend_tree;
	}

	int max_depth;

	MinimaxPair* minimax(Node<T>* node, int current_depth = 0, bool is_max_player = true, double alpha = -DBL_MAX, double beta = -DBL_MAX) {
		if (current_depth == max_depth) return evaluate_node_(node);
		if (node->children().empty()) extend_tree_(node);

		if (is_max_player) {
			MinimaxPair* best_pair = new MinimaxPair(-DBL_MAX, nullptr);
			for (auto child : node->children()) {
				MinimaxPair* child_pair = minimax(child, current_depth + 1, false, alpha, beta);
				// use >= to avoid returning nullptr when children exist
				if (child_pair->value >= best_pair->value) best_pair = new MinimaxPair(child_pair->value, child);
				alpha = fmax(alpha, best_pair->value);
				if (beta <= alpha) break;
			}
			return best_pair;
		}
		else {
			MinimaxPair* best_pair = new MinimaxPair(DBL_MAX, nullptr);
			for (auto child : node->children()) {
				MinimaxPair* child_pair = minimax(child, current_depth + 1, true, alpha, beta);
				// use <= to avoid returning nullptr when children exist
				if (child_pair->value <= best_pair->value) best_pair = new MinimaxPair(child_pair->value, child);
				beta = fmin(alpha, best_pair->value);
				if (beta <= alpha) break;
			}
			return best_pair;
		}
	}

	double minimax_value(Node<T>* node, int current_depth = 0, bool is_max_player = true, double alpha = -DBL_MAX, double beta = -DBL_MAX) {
		return minimax(node, current_depth, is_max_player, alpha, beta)->value;
	}

	double minimax_node(Node<T>* node, int current_depth = 0, bool is_max_player = true, double alpha = -DBL_MAX, double beta = -DBL_MAX) {
		return minimax(node, current_depth, is_max_player, alpha, beta)->node;
	}


private:
	std::function<void(Node<T>*)> extend_tree_;
	std::function<double(Node<T>*> evaluate_node_;
};
