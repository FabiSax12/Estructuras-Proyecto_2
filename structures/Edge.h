// Edge.h
#ifndef EDGE_H
#define EDGE_H

#include "Destination.h"
#include "Enums.h"

struct Destination;

class Edge {
public:
	Destination* origin;
	Destination* destination;
	TransportMethod transportMethod;
	double travelTime;

	Edge(Destination* origin, Destination* destination, TransportMethod transportMethod, double travelTime)
			: origin(origin), destination(destination), transportMethod(transportMethod), travelTime(travelTime) {}
};

#endif // EDGE_H

