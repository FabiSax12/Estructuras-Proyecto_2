#ifndef REWARD_H
#define REWARD_H

#include <string>

#include "ListNode.h"

class Reward : public ListNode<Reward> {
public:
	std::string name;
	int pointsRequired;
	Reward *next;

	Reward(const std::string& name, int points);
	[[nodiscard]] bool canRedeem(int clientPoints) const;

	[[nodiscard]] std::string toString() const {
		return "Nombre del premio: " + name + ", Puntos requeridos: " + std::to_string(pointsRequired);
	}
};

#endif // REWARD_H
