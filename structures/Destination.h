#ifndef DESTINATION_H
#define DESTINATION_H

#include <string>

#include "Edge.h"
#include "EntryPoint.h"
#include "ListNode.h"

struct Route;

struct Destination : public ListNode<Destination> {
	std::string name;
	std::string entryPointName;
	EntryPoint *entryPoint;
	Route* routes;
	Destination *next;

	// Variables auxiliares para algoritmos
	bool visited;             // Marca si el destino ha sido visitado

	Destination(const std::string &countryName, const std::string& entryPointName, EntryPoint *entryPoint);

	[[nodiscard]] std::string toString() const {
		return name + " - " + entryPointName;
	}
};

#endif // DESTINATION_H
