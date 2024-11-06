#ifndef ROUTE_H
#define ROUTE_H

#include "Enums.h"
#include <string>

#include "ListNode.h"

struct Destination;

using namespace std;

struct Route : public ListNode<Route>{
	Destination *destination;
	double travelTime;
	TransportMethod transportMethod;
	Route *next;

	Route(Destination *dest, double time, TransportMethod method);
	[[nodiscard]] double calculatePoints() const;
	[[nodiscard]] string getTransportMethod() const;
};

#endif // ROUTE_H
