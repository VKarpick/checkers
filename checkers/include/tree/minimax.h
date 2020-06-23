/*-------------------------------------------------------------------------------------------------
 A class for implementing minimax algorithm, templated for use of different types of nodes.

 Allows for retrieval of the value, the node, or both.

 Requires:
	A function to determine the value of a given node.
	The maximum depth to search to.
	And, if passing in an incomplete tree, a function for determining a node's children.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <functional>

#include "node.h"


// storing values and nodes together allows for easy retrieval of whichever is needed
template<typename T>
struct MinimaxPair {
	double value{ 0.0 };
	std::shared_ptr<Node<T>> node{ nullptr };
};


template <typename T>    // allowing for any type of Node requires template
class Minimax {
public:
	Minimax(std::function<double(std::shared_ptr<Node<T>>)> compute_node_value, unsigned int max_depth = 0,
		const std::function<void(std::shared_ptr<Node<T>>)> extend_tree = [](std::shared_ptr<Node<T>>) {}) {

		compute_node_value_ = compute_node_value;
		max_depth_ = max_depth;
		extend_tree_ = extend_tree;
	}


	MinimaxPair<T> minimax(std::shared_ptr<Node<T>> node, const unsigned int current_depth = 0, const bool is_max_player = true,
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		if (current_depth == max_depth_) {
			return MinimaxPair<T>{ compute_node_value_(node), node };
		}

		if (node->get_children().empty()) {
			extend_tree_(node);
		}

		if (is_max_player) {
			MinimaxPair<T> best_pair{ -DBL_MAX, nullptr };
			for (auto child : node->get_children()) {
				MinimaxPair<T> child_pair{ minimax(child, current_depth + 1, false, alpha, beta) };

				// use >= to avoid returning nullptr when children exist
				if (child_pair.value >= best_pair.value) {
					best_pair.value = child_pair.value;
					best_pair.node = child;
				}

				alpha = fmax(alpha, best_pair.value);
				if (beta <= alpha) break;
			}
			return best_pair;
		}
		else {
			MinimaxPair<T> best_pair{ DBL_MAX, nullptr };
			for (auto child : node->get_children()) {
				MinimaxPair<T> child_pair{ minimax(child, current_depth + 1, true, alpha, beta) };
				
				// use <= to avoid returning nullptr when children exist
				if (child_pair.value <= best_pair.value) {
					best_pair.value = child_pair.value;
					best_pair.node = child;
				}
				
				beta = fmin(beta, best_pair.value);
				if (beta <= alpha) break;
			}
			return best_pair;
		}
	}


	double minimax_value(std::shared_ptr<Node<T>> node, const int current_depth = 0, const bool is_max_player = true,
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		return minimax(node, current_depth, is_max_player, alpha, beta).value;
	}


	std::shared_ptr<Node<T>> minimax_node(std::shared_ptr<Node<T>> node, const int current_depth = 0, const bool is_max_player = true,
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		return minimax(node, current_depth, is_max_player, alpha, beta).node;
	}


private:
	unsigned int max_depth_{ 1 };
	std::function<double(std::shared_ptr<Node<T>>)> compute_node_value_;
	std::function<void(std::shared_ptr<Node<T>>)> extend_tree_;
};
