//
// Created by varga on 5/11/2024.
//

#ifndef TRIP_H
#define TRIP_H

#include <string>
#include "Enums.h"
#include "ListNode.h"

using namespace std;

class Trip : public ListNode<Trip> {
public:
	std::string originCountry;
	std::string originEntryPoint;
	std::string destinationCountry;
	std::string destinationEntryPoint;
	TransportMethod transportMethod;
	double travelTime;
	double pointsEarned;
	Trip *next;

	Trip(
		const string& originCountry, const string& originEntryPoint, const string& destinationCountry,
		const string& destinationEntryPoint, TransportMethod transportMethod, double travelTime
	);
	void calculatePoints();
	[[nodiscard]] std::string getTransportMethodString() const;
	[[nodiscard]] std::string toString() const;
};

#endif // TRIP_H
