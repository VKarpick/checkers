#include "estimator.h"


Estimator::Estimator(double step_size, std::vector<double> weights, bool use_eligibility_trace) {
	step_size_ = step_size;
	weights_ = weights;
	use_eligibility_trace_ = use_eligibility_trace;
	if (use_eligibility_trace_) {
		eligibility_trace_.assign(weights_.size(), 0);
	}
	else {
		eligibility_trace_.assign(weights_.size(), 1);  // eligibility traces set to 1 don't affect weights
	}
}

Estimator::Estimator(double step_size, int feature_size, bool use_eligibility_trace) {
	step_size_ = step_size;
	weights_ = std::vector<double>(feature_size, 0);
	use_eligibility_trace_ = use_eligibility_trace;
	if (use_eligibility_trace_) {
		eligibility_trace_.assign(weights_.size(), 0);
	}
	else {
		eligibility_trace_.assign(weights_.size(), 1);  // eligibility traces set to 1 don't affect weights
	}
}

std::vector<double> Estimator::weights() { return weights_; }

void Estimator::reset_eligibility_trace() { 
	if (use_eligibility_trace_) {
		fill(eligibility_trace_.begin(), eligibility_trace_.end(), 0);
	}
	else {
		fill(eligibility_trace_.begin(), eligibility_trace_.end(), 1);
	}
}


TDEstimator::TDEstimator(double step_size, std::vector<double> weights, bool use_eligibility_trace) 
	: Estimator(step_size, weights, use_eligibility_trace) {}
TDEstimator::TDEstimator(double step_size, int feature_size, bool use_eligibility_trace)
	: Estimator(step_size, feature_size, use_eligibility_trace) {}

double TDEstimator::predict(std::vector<double> features) {
	return inner_product(features.begin(), features.end(), weights_.begin(), 0);
}

void TDEstimator::update(double target, double estimate, std::vector<double> features, double discount_factor, double trace_decay) {
	double delta = target - estimate;
	for (size_t i = 0; i < weights_.size(); ++i) {
		if (use_eligibility_trace_) eligibility_trace_[i] = discount_factor * trace_decay * eligibility_trace_[i] + features[i];
		weights_[i] += step_size_ * delta * eligibility_trace_[i];
	}
}
