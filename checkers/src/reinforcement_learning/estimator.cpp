#include "estimator.h"


Estimator::Estimator(std::vector<double> weights, double step_size) {
	weights_ = weights;
	step_size_ = step_size;
	eligibility_trace_.assign(weights_.size(), 0);
}

std::vector<double> Estimator::weights() { return weights_; }

void Estimator::reset_eligibility_trace() { fill(eligibility_trace_.begin(), eligibility_trace_.end(), 0); }


double TDEstimator::predict(std::vector<double> features) {
	return inner_product(features.begin(), features.end(), weights_.begin(), 0);
}

void TDEstimator::update(double target, double estimate, std::vector<double> features, double discount_factor, double trace_decay) {
	double delta = target - estimate;
	for (size_t i = 0; i < eligibility_trace_.size(); ++i) {
		eligibility_trace_[i] = discount_factor * trace_decay * eligibility_trace_[i] + features[i];
		weights_[i] += step_size_ * delta * eligibility_trace_[i];
	}
}