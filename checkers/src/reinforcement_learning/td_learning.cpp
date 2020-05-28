#include "td_learning.h"



TDLambda::TDLambda() {}


TDLambda::TDLambda(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, 
	Policy* policy, double discountFactor, double traceDecay) :
	
	environment_(environment),
	estimator_(estimator),
	policy_(policy),
	discountFactor_(discountFactor),
	traceDecay_(traceDecay)
{}



void TDLambda::train(int nEpisodes, bool isPrintingUpdates) {
	for (int episodeNo = 0; episodeNo < nEpisodes; ++episodeNo) {
		std::shared_ptr<State> state = environment_->reset();
		estimator_->resetEligibilityTrace();
		std::shared_ptr<State> nextState = std::make_shared<State>();

		if (isPrintingUpdates) std::cout << "Episode:  " << (episodeNo + 1) << " of " << nEpisodes << std::endl;

		do {
			std::shared_ptr<Action> action{ policy_->actionSelection(state) };
			nextState = environment_->step(action);

			// hacky way to avoid reseting node each time when using MinimaxPolicy
			if (policy_->getNode() != nullptr) nextState = policy_->getNode()->getData().state;

			std::vector<double> stateFeatures{ environment_->featurize(state) };
			std::vector<double> nextStateFeatures{ environment_->featurize(nextState) };

			double target{ nextState->reward + discountFactor_ * estimator_->predict(nextStateFeatures) };
			double estimate{ estimator_->predict(stateFeatures) };
			estimator_->update(target, estimate, stateFeatures, discountFactor_, traceDecay_);

			state = nextState;

		} while (!nextState->isTerminal);
	}
}



TDLeaf::TDLeaf(std::shared_ptr<Environment> environment, std::shared_ptr<Estimator> estimator, std::shared_ptr<Player> maxPlayer,
	int maxDepth, double discountFactor, double traceDecay) {
	
	environment_ = environment;
	estimator_ = estimator;
	policy_ = new MinimaxPolicy(environment_, estimator_, maxPlayer, maxDepth);
	discountFactor_ = discountFactor;
	traceDecay_ = traceDecay;
}
