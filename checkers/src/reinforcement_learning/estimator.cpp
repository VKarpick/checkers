#include "estimator.h"



Estimator::Estimator(double stepSize, std::vector<double> weights, bool isUsingEligibilityTrace) {
	stepSize_ = stepSize;
	weights_ = weights;
	isUsingEligibilityTrace_ = isUsingEligibilityTrace;
	if (isUsingEligibilityTrace_) {
		eligibilityTrace_.assign(weights_.size(), 0);
	}
	else {
		eligibilityTrace_.assign(weights_.size(), 1);  // eligibility traces set to 1 don't affect weights
	}
}


Estimator::Estimator(double stepSize, int featureSize, bool isUsingEligibilityTrace) {
	//TODO can just use other constructor?
	stepSize_ = stepSize;
	weights_ = std::vector<double>(featureSize, 0);
	isUsingEligibilityTrace_ = isUsingEligibilityTrace;
	if (isUsingEligibilityTrace_) {
		eligibilityTrace_.assign(weights_.size(), 0);
	}
	else {
		eligibilityTrace_.assign(weights_.size(), 1);  // eligibility traces set to 1 don't affect weights
	}
}


std::vector<double> Estimator::getWeights() { 
	return weights_; 
}


void Estimator::resetEligibilityTrace() { 
	if (isUsingEligibilityTrace_) {
		fill(eligibilityTrace_.begin(), eligibilityTrace_.end(), 0);
	}
	else {
		fill(eligibilityTrace_.begin(), eligibilityTrace_.end(), 1);
	}
}



TDEstimator::TDEstimator(double stepSize, std::vector<double> weights, bool isUsingEligibilityTrace) 
	: Estimator(stepSize, weights, isUsingEligibilityTrace) {}
TDEstimator::TDEstimator(double stepSize, int featureSize, bool isUsingEligibilityTrace)
	: Estimator(stepSize, featureSize, isUsingEligibilityTrace) {}


double TDEstimator::predict(std::vector<double> features) {
	return inner_product(features.begin(), features.end(), weights_.begin(), 0);
}


void TDEstimator::update(double target, double estimate, std::vector<double> features, double discountFactor, double traceDecay) {
	double delta = target - estimate;
	for (size_t i = 0; i < weights_.size(); ++i) {
		if (isUsingEligibilityTrace_) eligibilityTrace_[i] = discountFactor * traceDecay * eligibilityTrace_[i] + features[i];
		weights_[i] += stepSize_ * delta * eligibilityTrace_[i];
	}
}
