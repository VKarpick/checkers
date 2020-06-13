#include "reinforcement_learning/estimator.h"



Estimator::Estimator(double step_size, std::vector<double> weights, bool is_using_eligibility_trace) :
	step_size_(step_size),
	weights_(weights),
	is_using_eligibility_trace_(is_using_eligibility_trace)
{
	initialize_eligibility_trace();
}


Estimator::Estimator(double step_size, int feature_size, bool is_using_eligibility_trace) :
	step_size_(step_size),
	weights_(std::vector<double>(feature_size, 0)),
	is_using_eligibility_trace_(is_using_eligibility_trace)
{
	initialize_eligibility_trace();
}


std::vector<double> Estimator::get_weights() { 
	return weights_; 
}


void Estimator::reset_eligibility_trace() { 
	if (is_using_eligibility_trace_) {
		fill(eligibility_trace_.begin(), eligibility_trace_.end(), 0);
	}
	else {
		fill(eligibility_trace_.begin(), eligibility_trace_.end(), 1);    // eligibility traces set to 1 don't affect weights
	}
}


void Estimator::initialize_eligibility_trace() {
	eligibility_trace_.assign(weights_.size(), 0);
	reset_eligibility_trace();
}



TDEstimator::TDEstimator(double step_size, std::vector<double> weights, bool is_using_eligibility_trace) 
	: Estimator(step_size, weights, is_using_eligibility_trace) {}

TDEstimator::TDEstimator(double step_size, int feature_size, bool is_using_eligibility_trace)
	: Estimator(step_size, feature_size, is_using_eligibility_trace) {}


double TDEstimator::predict(std::vector<double> features) {
	return inner_product(features.begin(), features.end(), weights_.begin(), 0.0);
}


void TDEstimator::update(double target, double estimate, std::vector<double> features, double discount_factor, double trace_decay) {
	double delta = target - estimate;
	for (size_t i = 0; i < weights_.size(); ++i) {
		if (is_using_eligibility_trace_) eligibility_trace_[i] = discount_factor * trace_decay * eligibility_trace_[i] + features[i];
		weights_[i] += step_size_ * delta * eligibility_trace_[i];
	}
}
