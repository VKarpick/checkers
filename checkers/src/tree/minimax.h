#pragma once

#include <functional>

#include "node.h"



// storing values and nodes together allows for easy retrieval of whichever is needed
template<typename T>
struct MinimaxPair {
	double value{ 0.0 };
	Node<T>* node{ nullptr };
};


template <typename T>    // allowing for any type of Node requires template
class Minimax {
public:
	Minimax(std::function<double(Node<T>*)> computeNodeValue, int maxDepth = 0, 
		const std::function<void(Node<T>*)> extendTree = [](Node<T>*) {}) {

		computeNodeValue_ = computeNodeValue;
		maxDepth_ = maxDepth;
		extendTree_ = extendTree;
	}


	MinimaxPair<T> minimax(Node<T>* node, int currentDepth = 0, bool isMaxPlayer = true, 
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		if (currentDepth == maxDepth_) return MinimaxPair<T>{ computeNodeValue_(node), node };
		if (node->getChildren().empty()) extendTree_(node);

		if (isMaxPlayer) {
			MinimaxPair<T> bestPair{ -DBL_MAX, nullptr };
			int i{ 0 };
			for (auto child : node->getChildren()) {
				MinimaxPair<T> childPair{ minimax(child, currentDepth + 1, false, alpha, beta) };

				// use >= to avoid returning nullptr when children exist
				if (childPair.value >= bestPair.value) {
					bestPair.value = childPair.value;
					bestPair.node = child;
				}

				alpha = fmax(alpha, bestPair.value);
				if (beta <= alpha) break;
			}
			return bestPair;
		}
		else {
			MinimaxPair<T> bestPair{ DBL_MAX, nullptr };
			for (auto child : node->getChildren()) {
				MinimaxPair<T> childPair{ minimax(child, currentDepth + 1, true, alpha, beta) };
				
				// use <= to avoid returning nullptr when children exist
				if (childPair.value <= bestPair.value) {
					bestPair.value = childPair.value;
					bestPair.node = child;
				}
				
				beta = fmin(beta, bestPair.value);
				if (beta <= alpha) break;
			}
			return bestPair;
		}
	}


	double minimaxValue(Node<T>* node, int currentDepth = 0, bool isMaxPlayer = true, 
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		return minimax(node, currentDepth, isMaxPlayer, alpha, beta).value;
	}


	Node<T>* minimaxNode(Node<T>* node, int currentDepth = 0, bool isMaxPlayer = true, 
		double alpha = -DBL_MAX, double beta = DBL_MAX) {
		
		return minimax(node, currentDepth, isMaxPlayer, alpha, beta).node;
	}


private:
	int maxDepth_{ 1 };

	// function to calculate the value of a given node
	std::function<double(Node<T>*)> computeNodeValue_;

	// function to find children of a node when passing incomplete tree
	// if complete tree is provided, can be ignored
	std::function<void(Node<T>*)> extendTree_;
};
