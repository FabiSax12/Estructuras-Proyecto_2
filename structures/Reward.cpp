//
// Created by varga on 29/10/2024.
//

#include "Reward.h"

Reward::Reward(const std::string &name, int points) {
	this->next = nullptr;
	this->name = name;
	this->pointsRequired = points;
}

bool Reward::canRedeem(const int clientPoints) const {
	return clientPoints >= pointsRequired;
}
