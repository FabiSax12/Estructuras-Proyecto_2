#include "Destination.h"
#include "Route.h"

#include <utility>

Destination::Destination(const std::string &countryName, const std::string& entryPointName, EntryPoint *entryPoint) {
	this->name = countryName;
	this->entryPointName = entryPointName;
	this->entryPoint = entryPoint;
	this->next = nullptr;
	this->routes = nullptr;
	this->visited = false;
}
