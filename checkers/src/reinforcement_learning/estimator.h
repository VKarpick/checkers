#pragma once

#include <vector>
#include <numeric>

class Estimator {
public:
	Estimator(double step_size, std::vector<double> weights);
	Estimator(double step_size, int feature_size);
	std::vector<double> weights();
	void reset_eligibility_trace();
	virtual double predict(std::vector<double> features) = 0;
	virtual void update(double target, double estimate, std::vector<double> features, double discount_factor, double trace_decay) = 0;

protected:
	std::vector<double> weights_;
	double step_size_;
	std::vector<double> eligibility_trace_;
};

class TDEstimator : public Estimator {
	double predict(std::vector<double> features);
	void update(double target, double estimate, std::vector<double> features, double discount_factor = 1, double trace_decay = 1);
};
