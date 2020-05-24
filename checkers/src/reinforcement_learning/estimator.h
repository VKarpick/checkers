#pragma once

#include <numeric>
#include <vector>


// generic reinforcement learning estimator to be inherited from
class Estimator {
public:
	// can either provide starting weights or a feature size to initialize weights to 0
	Estimator(double stepSize, std::vector<double> weights, bool isUsingEligibilityTrace = false);
	Estimator(double stepSize, int featureSize, bool isUsingEligibilityTrace = false);

	
	std::vector<double> getWeights();


	void resetEligibilityTrace();
	virtual double predict(std::vector<double> features) = 0;
	virtual void update(double target, double estimate, std::vector<double> features, 
		double discountFactor = 1.0, double traceDecay = 1.0) = 0;


protected:
	std::vector<double> weights_;
	double stepSize_;    // alpha
	std::vector<double> eligibilityTrace_;
	bool isUsingEligibilityTrace_;
};



// Estimator for temporal difference learning
//TODO allow for both prediction and control, possibly non-linear learning
class TDEstimator : public Estimator {
public:
	TDEstimator(double stepSize, std::vector<double> weights, bool isUsingEligibilityTrace = false);
	TDEstimator(double stepSize, int featureSize, bool isUsingEligibilityTrace = false);
	
	
	double predict(std::vector<double> features) override;
	void update(double target, double estimate, std::vector<double> features, 
		double discountFactor = 1.0, double traceDecay = 1.0) override;
};
