/*-------------------------------------------------------------------------------------------------
 Estimators for reinforcement learning.
 
 Currently includes a TD estimator for linear prediction.
-------------------------------------------------------------------------------------------------*/


#pragma once

#include <numeric>
#include <vector>


// abstract base class to be inherited from
class Estimator {
public:
	Estimator(double step_size, std::vector<double> weights, bool is_using_eligibility_trace = false);
	Estimator(double step_size, int feature_size, bool is_using_eligibility_trace = false);

	
	std::vector<double> get_weights();


	void reset_eligibility_trace();
	virtual double predict(std::vector<double> features) = 0;
	virtual void update(double target, double estimate, std::vector<double> features, 
		double discount_factor = 1.0, double trace_decay = 1.0) = 0;


protected:
	std::vector<double> weights_{};
	double step_size_{ 0.0 };    // alpha
	std::vector<double> eligibility_trace_{};
	bool is_using_eligibility_trace_{ false };


	void initialize_eligibility_trace();
};



class TDEstimator : public Estimator {
public:
	TDEstimator(double step_size, std::vector<double> weights, bool is_using_eligibility_trace = false);
	TDEstimator(double step_size, int feature_size, bool is_using_eligibility_trace = false);
	
	
	double predict(std::vector<double> features) override;
	void update(double target, double estimate, std::vector<double> features, 
		double discount_factor = 1.0, double trace_decay = 1.0) override;
};
