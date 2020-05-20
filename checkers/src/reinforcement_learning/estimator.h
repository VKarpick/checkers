#pragma once

#include <numeric>
#include <vector>


// generic reinforcement learning estimator to be inherited from
class Estimator {
public:
	// constructors - can either provide starting weights or a feature size to initialize weights to 0
	Estimator(double step_size, std::vector<double> weights, bool use_eligibility_trace = false);
	Estimator(double step_size, int feature_size, bool use_eligibility_trace = false);

	// getter
	std::vector<double> weights();

	void reset_eligibility_trace();
	virtual double predict(std::vector<double> features) = 0;
	virtual void update(double target, double estimate, std::vector<double> features, double discount_factor = 1, double trace_decay = 1) = 0;

protected:
	std::vector<double> weights_;
	double step_size_;    // alpha
	std::vector<double> eligibility_trace_;
	bool use_eligibility_trace_;
};


// Estimator for temporal difference learning
//TODO allow for both prediction and control, possibly non-linear learning
class TDEstimator : public Estimator {
public:
	TDEstimator(double step_size, std::vector<double> weights, bool use_eligibility_trace = false);
	TDEstimator(double step_size, int feature_size, bool use_eligibility_trace = false);
	double predict(std::vector<double> features) override;
	void update(double target, double estimate, std::vector<double> features, double discount_factor = 1, double trace_decay = 1) override;
};
