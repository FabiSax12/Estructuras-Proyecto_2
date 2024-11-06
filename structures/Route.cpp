#include "Route.h"
#include "Destination.h"

Route::Route(Destination* dest, double time, TransportMethod method) {
	this->destination = dest;
	this->travelTime = time;
	this->transportMethod = method;
	this->next = nullptr;
}

double Route::calculatePoints() const {
	switch (transportMethod) {
		case TransportMethod::PLANE: return travelTime * 100;
		case TransportMethod::CRUISE: return travelTime * 70;
		case TransportMethod::CAR: return travelTime * 25;
		default: return 0;
	}
}

string Route::getTransportMethod() const {
	switch (this->transportMethod) {
		case TransportMethod::CAR: return "Carro";
		case TransportMethod::PLANE: return "Avion";
		case TransportMethod::CRUISE: return "Crusero";
		default: return "Unknown";
	}
}

