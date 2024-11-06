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
	double distance;          // Usado para Dijkstra y otras búsquedas
	bool visited;             // Marca si el destino ha sido visitado
	Destination* previous;    // Puntero al destino anterior en la ruta
	Edge* edgeToHere;         // Arista usada para llegar a este destino

	Destination(const std::string &countryName, const std::string& entryPointName, EntryPoint *entryPoint);

	[[nodiscard]] std::string toString() const {
		return name + " - " + entryPointName;
	}
};

#endif // DESTINATION_H
