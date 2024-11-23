#include "Client.h"
#include <utility>

Client::Client(std::string name) {
	this->name = std::move(name);
	this->accumulatedPoints = 0;
	this->next = nullptr;
	this->trips = new SimpleList<Trip>();
	this->rewardsEarned = new SimpleList<Reward>();
}

void Client::addPoints(int points) {
	accumulatedPoints += points;
}

int Client::getPoints() const {
	return accumulatedPoints;
}

void Client::loadTrip(const Trip& trip) const {
	trips->add(trip);
}

void Client::loadReward(const Reward &reward) const {
	rewardsEarned->add(reward);
}

void Client::addTrip(const Trip& trip) {
	trips->add(trip);
	addPoints(static_cast<int>(trip.pointsEarned));
}

void Client::addReward(const Reward &reward) {
	rewardsEarned->add(reward);
	accumulatedPoints -= reward.pointsRequired;
}

const SimpleList<Trip>& Client::getTrips() const {
	return *trips;
}

const SimpleList<Reward>& Client::getRewards() const {
	return *rewardsEarned;
}
