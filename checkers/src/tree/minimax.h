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
	Minimax(std::function<double(Node<T>*)> computeNodeValue, int maxDepth = 0, 
		const std::function<void(Node<T>*)> extendTree = [](Node<T>*) {}) {

		// function to calculate the value of a given node
		computeNodeValue_ = computeNodeValue;
		
		maxDepth_ = maxDepth;

		// function to find children of a node when passing incomplete tree
		// if complete tree is provided, can be ignored
		extendTree_ = extendTree;
	}


	MinimaxPair<T>* minimax(Node<T>* node, int currentDepth = 0, bool isMaxPlayer = true, 
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		if (currentDepth == maxDepth_) return new MinimaxPair<T>{ computeNodeValue_(node), node };
		if (node->getChildren().empty()) extendTree_(node);

		if (isMaxPlayer) {
			MinimaxPair<T>* bestPair = new MinimaxPair<T>{ -DBL_MAX, nullptr };
			int i = 0;
			for (auto child : node->getChildren()) {
				MinimaxPair<T>* childPair = minimax(child, currentDepth + 1, false, alpha, beta);
				// use >= to avoid returning nullptr when children exist
				if (childPair->value >= bestPair->value) bestPair = new MinimaxPair<T>{ childPair->value, child };
				alpha = fmax(alpha, bestPair->value);
				if (beta <= alpha) break;
			}
			return bestPair;
		}
		else {
			MinimaxPair<T>* bestPair = new MinimaxPair<T>{ DBL_MAX, nullptr };
			for (auto child : node->getChildren()) {
				MinimaxPair<T>* childPair = minimax(child, currentDepth + 1, true, alpha, beta);
				// use <= to avoid returning nullptr when children exist
				if (childPair->value <= bestPair->value) bestPair = new MinimaxPair<T>{ childPair->value, child };
				beta = fmin(beta, bestPair->value);
				if (beta <= alpha) break;
			}
			return bestPair;
		}
	}


	double minimax_value(Node<T>* node, int currentDepth = 0, bool isMaxPlayer = true, 
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		return minimax(node, currentDepth, isMaxPlayer, alpha, beta)->value;
	}


	Node<T>* minimax_node(Node<T>* node, int currentDepth = 0, bool isMaxPlayer = true, 
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		return minimax(node, currentDepth, isMaxPlayer, alpha, beta)->node;
	}


private:
	int maxDepth_;
	std::function<void(Node<T>*)> extendTree_;
	std::function<double(Node<T>*)> computeNodeValue_;
};
