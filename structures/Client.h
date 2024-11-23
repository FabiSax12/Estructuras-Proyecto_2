#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "Reward.h"
#include "SimpleList.h"
#include "Trip.h"

class Client : public ListNode<Client> {
public:
	std::string name;
	int accumulatedPoints;
	SimpleList<Trip> *trips;
	SimpleList<Reward> *rewardsEarned;
	Client *next;

	explicit Client(std::string name);
	void addPoints(int points);
	[[nodiscard]] int getPoints() const;
	void addTrip(const Trip& trip);
	void addReward(const Reward& reward);
	void loadTrip(const Trip& trip) const;
	void loadReward(const Reward& reward) const;
	[[nodiscard]] const SimpleList<Trip>& getTrips() const;   // Devuelve una referencia constante a los viajes
	[[nodiscard]] const SimpleList<Reward>& getRewards() const; // Devuelve una referencia constante a los premios


	[[nodiscard]] std::string toString() const {
		return "Cliente: " + name + ", Puntos acumulados: " + std::to_string(accumulatedPoints);
	}
};

#endif // CLIENT_H
